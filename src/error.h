#ifndef LDBUS_ERROR_H
#define LDBUS_ERROR_H

#include <lua.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#define DBUS_ERROR_METATABLE "ldbus_DBusError"

#define check_DBusError(L, arg) ((DBusError*)luaL_checkudata((L), (arg), DBUS_ERROR_METATABLE))

LDBUS_INTERNAL DBusError* new_DBusError(lua_State *L);

#endif
