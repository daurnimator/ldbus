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
	lua_State *L = ((ldbus_timeout_udata*)data)->L;
	int ref = ((ldbus_timeout_udata*)data)->ref;

	if (!lua_checkstack(L, 4)) return FALSE;

	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_ADD);

	push_DBusTimeout(L, timeout);
	/* Save DBusTimeout in registry */
	lua_pushvalue(L, -1);
	lua_rawsetp(L, LUA_REGISTRYINDEX, timeout);

	return lua_pcall(L, 1, 0, 0) != LUA_ERRMEM;
}

LDBUS_INTERNAL void ldbus_timeout_remove_function(DBusTimeout *timeout, void *data) {
	lua_State *L = ((ldbus_timeout_udata*)data)->L;
	int ref = ((ldbus_timeout_udata*)data)->ref;

	DBusTimeout **udata;

	/* Lookup remove callback from ref in `data` */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_REMOVE);

	/* Grab timeout from registry */
	/* this way we can't throw an error */
	lua_rawgetp(L, LUA_REGISTRYINDEX, timeout);
	/* We need to keep a copy of userdata around so we can invalidate it */
	lua_pushvalue(L, -1);
	/* Replace the callback table at a higher stack slot with this function.
	   Has to be below the function to we have it after the pcall */
	lua_replace(L, -4);

	/* Delete from registry */
	/* setting `nil` can't error (undocumented) */
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, timeout);

	/* Call the callback, with looked-up timeout as argument */
	lua_pcall(L, 1, 0, 0);

	/* Invalidate timeout object */
	udata = lua_touserdata(L, -1);
	if (udata != NULL) {
		*udata = NULL;
	}

	return;
}

LDBUS_INTERNAL void ldbus_timeout_toggled_function(DBusTimeout *timeout, void *data) {
	lua_State *L = ((ldbus_timeout_udata*)data)->L;
	int ref = ((ldbus_timeout_udata*)data)->ref;

	/* Lookup remove callback from ref in `data` */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_TOGGLE);
	/* Remove callback table from stack */
	lua_remove(L, lua_gettop(L)-1);

	/* Grab timeout from registry */
	/* this way we can't throw an error */
	lua_rawgetp(L, LUA_REGISTRYINDEX, timeout);

	/* Call the callback, with looked-up timeout as argument */
	lua_pcall(L, 1, 0, 0);
	return;
}

LDBUS_INTERNAL void ldbus_timeout_free_data_function(void *data) {
	lua_State *L = ((ldbus_timeout_udata*)data)->L;
	int ref = ((ldbus_timeout_udata*)data)->ref;

	luaL_unref(L, LUA_REGISTRYINDEX, ref);
	free(data);
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
