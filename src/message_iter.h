#include <lua.h>

#include "ldbus.h"

LDBUS_INTERNAL int push_DBusMessageIter(lua_State *L);
LDBUS_INTERNAL void load_dbus_message_iter(lua_State *L);
