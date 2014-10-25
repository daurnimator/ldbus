#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.2.h"

#include <dbus/dbus.h>

#include "ldbus.h"
#include "watch.h"


static int ldbus_watch_get_unix_fd(lua_State *L) {
	DBusWatch *watch = check_DBusWatch(L, 1);
	int fd;
	if (watch == NULL || (fd = dbus_watch_get_unix_fd(watch)) == -1) {
		lua_pushnil(L);
	} else {
		lua_pushinteger(L, fd);
	}
	return 1;
}

static int ldbus_watch_get_socket(lua_State *L) {
	DBusWatch *watch = check_DBusWatch(L, 1);
	int fd;
	if (watch == NULL || (fd = dbus_watch_get_socket(watch)) == -1) {
		lua_pushnil(L);
	} else {
		lua_pushinteger(L, fd);
	}
	return 1;
}

static int ldbus_watch_get_flags(lua_State *L) {
	DBusWatch *watch = check_DBusWatch(L, 1);
	unsigned int flags = (watch == NULL) ? 0 : dbus_watch_get_flags(watch);
	lua_pushunsigned(L, flags);
	return 1;
}

static int ldbus_watch_handle(lua_State *L) {
	DBusWatch *watch = check_DBusWatch(L, 1);
	int flags;
	dbus_bool_t ok;
	luaL_argcheck(L, watch != NULL, 1, "watch invalid");
	flags = luaL_checkint(L, 2);
	ok = dbus_watch_handle(watch, flags);
	lua_pushboolean(L, ok);
	return 1;
}

static int ldbus_watch_get_enabled(lua_State *L) {
	DBusWatch *watch = check_DBusWatch(L, 1);
	dbus_bool_t enabled = (watch == NULL) ? FALSE : dbus_watch_get_enabled(watch);
	lua_pushboolean(L, enabled);
	return 1;
}

LDBUS_INTERNAL void push_DBusWatch(lua_State *L, DBusWatch *watch) {
	static luaL_Reg const methods [] = {
		{ "get_unix_fd", ldbus_watch_get_unix_fd },
		{ "get_socket", ldbus_watch_get_socket },
		{ "get_flags", ldbus_watch_get_flags },
		{ "handle", ldbus_watch_handle },
		{ "get_enabled", ldbus_watch_get_enabled },
		{ NULL, NULL }
	};

	DBusWatch **udata = lua_newuserdata(L, sizeof(DBusWatch*));
	*udata = watch;

	if (luaL_newmetatable(L, DBUS_WATCH_METATABLE)) {
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushstring(L, "DBusWatch");
		lua_setfield(L, -2, "__udtype");
	}
	lua_setmetatable(L, -2);
}

LDBUS_INTERNAL dbus_bool_t ldbus_watch_add_function(DBusWatch *watch, void *data) {
	lua_State *L = ((ldbus_watch_udata*)data)->L;
	int ref = ((ldbus_watch_udata*)data)->ref;

	if (!lua_checkstack(L, 4)) return FALSE;

	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_ADD);

	push_DBusWatch(L, watch);
	/* Save DBusWatch in registry */
	lua_pushvalue(L, -1);
	lua_rawsetp(L, LUA_REGISTRYINDEX, watch);

	return lua_pcall(L, 1, 0, 0) != LUA_ERRMEM;
}

LDBUS_INTERNAL void ldbus_watch_remove_function(DBusWatch *watch, void *data) {
	lua_State *L = ((ldbus_watch_udata*)data)->L;
	int ref = ((ldbus_watch_udata*)data)->ref;

	DBusWatch **udata;

	/* Lookup remove callback from ref in `data` */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -2, DBUS_LUA_FUNC_REMOVE);

	/* Grab watch from registry */
	/* this way we can't throw an error */
	lua_rawgetp(L, LUA_REGISTRYINDEX, watch);
	/* We need to keep a copy of userdata around so we can invalidate it */
	lua_pushvalue(L, -1);
	/* Replace the callback table at a higher stack slot with this function.
	   Has to be below the function to we have it after the pcall */
	lua_replace(L, -4);

	/* Delete from registry */
	/* setting `nil` can't error (undocumented) */
	lua_pushnil(L);
	lua_rawsetp(L, LUA_REGISTRYINDEX, watch);

	/* Call the callback, with looked-up watch as argument */
	lua_pcall(L, 1, 0, 0);

	/* Invalidate watch object */
	udata = lua_touserdata(L, -1);
	if (udata != NULL) {
		*udata = NULL;
	}

	return;
}

LDBUS_INTERNAL void ldbus_watch_toggled_function(DBusWatch *watch, void *data) {
	lua_State *L = ((ldbus_watch_udata*)data)->L;
	int ref = ((ldbus_watch_udata*)data)->ref;

	/* Lookup remove callback from ref in `data` */
	lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
	lua_rawgeti(L, -1, DBUS_LUA_FUNC_TOGGLE);
	/* Remove callback table from stack */
	lua_remove(L, lua_gettop(L)-1);

	/* Grab watch from registry */
	/* this way we can't throw an error */
	lua_rawgetp(L, LUA_REGISTRYINDEX, watch);

	/* Call the callback, with looked-up watch as argument */
	lua_pcall(L, 1, 0, 0);
	return;
}

LDBUS_INTERNAL void ldbus_watch_free_data_function(void *data) {
	lua_State *L = ((ldbus_watch_udata*)data)->L;
	int ref = ((ldbus_watch_udata*)data)->ref;

	luaL_unref(L, LUA_REGISTRYINDEX, ref);
	free(data);
}
