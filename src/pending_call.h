#include <lua.h>

#include <dbus/dbus.h>

#include "ldbus.h"

LDBUS_INTERNAL void push_DBusPendingCall(lua_State *L, DBusPendingCall*);
