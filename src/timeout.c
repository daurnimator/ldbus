#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include <dbus/dbus.h>

#include "ldbus.h"
#include "timeout.h"


static int ldbus_timeout_get_interval(lua_State *L) {
	DBusTimeout *timeout = check_DBusTimeout(L, 1);
	int interval;
	if (timeout == NULL) {
		lua_pushnil(L);
	} else {
		interval = dbus_timeout_get_interval(timeout);
		lua_pushnumber(L, ((double)interval)/1000);
	}
	return 1;
}

static int ldbus_timeout_handle(lua_State *L) {
	DBusTimeout *timeout = check_DBusTimeout(L, 1);
	if (timeout == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushboolean(L, dbus_timeout_handle(timeout));
	}
	return 1;
}

static int ldbus_timeout_get_enabled(lua_State *L) {
	DBusTimeout *timeout = check_DBusTimeout(L, 1);
	dbus_bool_t enabled = (timeout == NULL) ? FALSE : dbus_timeout_get_enabled(timeout);
	lua_pushboolean(L, enabled);
	return 1;
}

LDBUS_INTERNAL void push_DBusTimeout(lua_State *L, DBusTimeout *timeout) {
	DBusTimeout **udata = lua_newuserdata(L, sizeof(DBusTimeout*));
	*udata = timeout;
	luaL_setmetatable(L, DBUS_TIMEOUT_METATABLE);
}

LDBUS_INTERNAL dbus_bool_t ldbus_timeout_add_function(DBusTimeout *timeout, void *data) {
	lua_State *L = *(lua_State**)data;
	int top = lua_gettop(L);
	if (!lua_checkstack(L, 2)) return FALSE;
	lua_rawgetp(L, LUA_REGISTRYINDEX, data);
	lua_getuservalue(L, -1);
	lua_remove(L, top + 1); /* remove userdata */
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_ADD);
	lua_remove(L, top + 1); /* remove uservalue table */
	push_DBusTimeout(L, timeout); /* XXX: could throw */
	/* Save DBusTimeout in registry */
	lua_pushvalue(L, -1);
	lua_rawsetp(L, LUA_REGISTRYINDEX, timeout);
	switch(lua_pcall(L, 1, 0, 0)) {
	case LUA_OK:
		return TRUE;
	case LUA_ERRMEM:
		lua_pop(L, 1);
		return FALSE;
	default:
		/* unhandled error */
		lua_pop(L, 1);
		return TRUE;
	}
}

LDBUS_INTERNAL void ldbus_timeout_remove_function(DBusTimeout *timeout, void *data) {
	lua_State *L = *(lua_State**)data;
	int top = lua_gettop(L);
	DBusTimeout **udata;

	/* Grab added timeout from registry */
	/* We need to keep a reference to userdata around so we can invalidate it */
	lua_rawgetp(L, LUA_REGISTRYINDEX, timeout);
	/* Delete from registry */
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, timeout);

	lua_rawgetp(L, LUA_REGISTRYINDEX, data);
	lua_getuservalue(L, -1);
	lua_remove(L, top + 2); /* remove userdata */
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_REMOVE);
	lua_remove(L, top + 2); /* remove uservalue table */
	lua_pushvalue(L, top + 1); /* push timeout as argument */
	if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
		/* unhandled error */
	}

	/* Invalidate timeout object */
	udata = lua_touserdata(L, top+1);
	if (udata != NULL) {
		*udata = NULL;
	}

	lua_settop(L, top);
	return;
}

LDBUS_INTERNAL void ldbus_timeout_toggled_function(DBusTimeout *timeout, void *data) {
	lua_State *L = *(lua_State**)data;
	int top = lua_gettop(L);
	lua_rawgetp(L, LUA_REGISTRYINDEX, data);
	lua_getuservalue(L, -1);
	lua_remove(L, top + 1); /* remove userdata */
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_TOGGLE);
	lua_remove(L, top + 1); /* remove uservalue table */
	lua_rawgetp(L, LUA_REGISTRYINDEX, timeout); /* grab added timeout from registry */
	if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
		/* unhandled error */
		lua_pop(L, 1);
	}
	return;
}

LDBUS_INTERNAL void ldbus_timeout_free_data_function(void *data) {
	lua_State *L = *(lua_State**)data;
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, data);
}

int lua_open_ldbus_timeout(lua_State *L) {
	static luaL_Reg const methods [] = {
		{ "get_interval", ldbus_timeout_get_interval },
		{ "handle", ldbus_timeout_handle },
		{ "get_enabled", ldbus_timeout_get_enabled },
		{ NULL, NULL }
	};

	if (luaL_newmetatable(L, DBUS_TIMEOUT_METATABLE)) {
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushstring(L, "DBusTimeout");
		lua_setfield(L, -2, "__udtype");
	}
	return 0;
}
