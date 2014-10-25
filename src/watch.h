#ifndef LDBUS_WATCH_H
#define LDBUS_WATCH_H

#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#define DBUS_WATCH_METATABLE "ldbus_DBusWatch"

#define check_DBusWatch(L, arg) (*(DBusWatch **)luaL_checkudata((L), (arg), DBUS_WATCH_METATABLE))

typedef struct {
    lua_State* L;
    int ref;
} ldbus_watch_udata;

enum DBus_lua_func_indexes {
    DBUS_LUA_FUNC_ADD,
    DBUS_LUA_FUNC_REMOVE,
    DBUS_LUA_FUNC_TOGGLE
};

LDBUS_INTERNAL void push_DBusWatch(lua_State *L, DBusWatch *watch);
LDBUS_INTERNAL dbus_bool_t ldbus_watch_add_function(DBusWatch *watch, void *data);
LDBUS_INTERNAL void ldbus_watch_remove_function(DBusWatch *watch, void *data);
LDBUS_INTERNAL void ldbus_watch_toggled_function(DBusWatch *watch, void *data);
LDBUS_INTERNAL void ldbus_watch_free_data_function(void *data);

int lua_open_ldbus_watch(lua_State *L);

#endif
