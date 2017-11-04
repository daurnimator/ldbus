#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include <dbus/dbus.h>

#include "ldbus.h"
#include "message.h"

#include "pending_call.h"

static DBusPendingCall* checkPendingCall(lua_State *L, int arg) {
	return *(DBusPendingCall**)luaL_checkudata(L, arg, "ldbus_DBusPendingCall");
}

static int ldbus_pending_call_unref(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	dbus_pending_call_unref(pending);
	return 0;
}

static void pending_notify_function(DBusPendingCall *pending, void *data) {
	lua_State *L = *(lua_State**)data;
	int top = lua_gettop(L);
	UNUSED(pending);
	lua_rawgetp(L, LUA_REGISTRYINDEX, data);
	lua_getuservalue(L, -1);
	lua_remove(L, top + 1); /* remove userdata */
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_FUNC);
	lua_remove(L, top + 1); /* remove uservalue table */
	if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
		/* unhandled error */
		lua_pop(L, 1);
	}
}

static void free_data_function(void *data) {
	lua_State *L = *(lua_State**)data;
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, data);
}

static int ldbus_pending_call_set_notify(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	lua_State **data;
	luaL_checktype(L, 2, LUA_TFUNCTION);
	data = lua_newuserdata(L, sizeof(lua_State*));
	*data = L;
	lua_createtable(L, 2, 0);
	lua_pushvalue(L, 2);
	lua_rawseti(L, -2, DBUS_LUA_FUNC_FUNC);
	lua_pushthread(L);
	lua_rawseti(L, -2, DBUS_LUA_FUNC_THREAD);
	lua_setuservalue(L, -2);
	lua_rawsetp(L, LUA_REGISTRYINDEX, data);
	if (!dbus_pending_call_set_notify(pending, pending_notify_function, data, free_data_function)) {
		free_data_function(data);
		return luaL_error(L, LDBUS_NO_MEMORY);
	}
	lua_pushboolean(L, 1);
	return 1;
}

static int ldbus_pending_call_cancel(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	dbus_pending_call_cancel(pending);
	return 0;
}

static int ldbus_pending_call_get_completed(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	lua_pushboolean(L, dbus_pending_call_get_completed(pending));
	return 1;
}

static int ldbus_pending_call_steal_reply(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	DBusMessage* message = dbus_pending_call_steal_reply(pending);
	if (message == NULL) {
		lua_pushnil(L);
	} else {
		push_DBusMessage(L, message);
	}
	return 1;
}

static int ldbus_pending_call_block(lua_State *L) {
	DBusPendingCall* pending = checkPendingCall(L, 1);
	dbus_pending_call_block(pending);
	return 0;
}

LDBUS_INTERNAL void push_DBusPendingCall(lua_State *L, DBusPendingCall* pending) {
	static luaL_Reg const methods [] = {
		{ "set_notify",    ldbus_pending_call_set_notify },
		{ "cancel",        ldbus_pending_call_cancel },
		{ "get_completed", ldbus_pending_call_get_completed },
		{ "steal_reply",   ldbus_pending_call_steal_reply },
		{ "block",         ldbus_pending_call_block },
		{ NULL, NULL }
	};

	DBusPendingCall ** udata = lua_newuserdata(L, sizeof(DBusPendingCall*));
	*udata = pending;

	if (luaL_newmetatable(L, "ldbus_DBusPendingCall")) {
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, ldbus_pending_call_unref);
		lua_setfield(L, -2, "__gc");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushstring(L, "DBusPendingCall");
		lua_setfield(L, -2, "__udtype");
	}
	lua_setmetatable(L, -2);
}
