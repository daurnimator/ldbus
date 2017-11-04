#ifndef LDBUS_TIMEOUT_H
#define LDBUS_TIMEOUT_H

#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#define DBUS_TIMEOUT_METATABLE "ldbus_DbusTimeout"

#define check_DBusTimeout(L, arg) (*(DBusTimeout **)luaL_checkudata((L), (arg), DBUS_TIMEOUT_METATABLE))

LDBUS_INTERNAL void push_DBusTimeout(lua_State *L, DBusTimeout *timeout);
LDBUS_INTERNAL dbus_bool_t ldbus_timeout_add_function(DBusTimeout *timeout, void *data);
LDBUS_INTERNAL void ldbus_timeout_remove_function(DBusTimeout *timeout, void *data);
LDBUS_INTERNAL void ldbus_timeout_toggled_function(DBusTimeout *timeout, void *data);
LDBUS_INTERNAL void ldbus_timeout_free_data_function(void *data);

int lua_open_ldbus_timeout(lua_State *L);

#endif
