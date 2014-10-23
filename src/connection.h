#include <lua.h>

#include <dbus/dbus.h>

void push_DBusConnection(lua_State *L, DBusConnection * connection);
int luaopen_ldbus_connection(lua_State *L);
