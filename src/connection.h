#include <lua.h>

#include <dbus/dbus.h>

#define DBUS_CONNECTION_METATABLE "ldbus_DBusConnection"

#define check_DBusConnection(L, arg) (*(DBusConnection **)luaL_checkudata(L, arg, DBUS_CONNECTION_METATABLE))

void push_DBusConnection(lua_State *L, DBusConnection * connection);
int luaopen_ldbus_connection(lua_State *L);
