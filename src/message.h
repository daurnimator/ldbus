#ifndef LDBUS_MESSAGE_H
#define LDBUS_MESSAGE_H

#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#define DBUS_MESSAGE_METATABLE "ldbus_DBusMessage"

#define check_DBusMessage(L, arg) (*(DBusMessage **)luaL_checkudata((L), (arg), DBUS_MESSAGE_METATABLE))


LDBUS_INTERNAL void push_DBusMessage(lua_State *L, DBusMessage * message);
int luaopen_ldbus_message(lua_State *L);

#endif
