#include <lua.h>

#include <dbus/dbus.h>

#define DBUS_ERROR_METATABLE "ldbus_DBusError"

#define check_DBusError(L, arg) ((DBusError*)luaL_checkudata((L), (arg), DBUS_ERROR_METATABLE))

DBusError* new_DBusError(lua_State *L);
