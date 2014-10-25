#ifndef LDBUS_PENDING_CALL_H
#define LDBUS_PENDING_CALL_H

#include <lua.h>

#include <dbus/dbus.h>

#include "ldbus.h"

LDBUS_INTERNAL void push_DBusPendingCall(lua_State *L, DBusPendingCall*);

#endif
