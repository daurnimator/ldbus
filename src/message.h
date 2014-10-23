#include <lua.h>

#include <dbus/dbus.h>

void push_DBusMessage(lua_State *L, DBusMessage * message);
int luaopen_ldbus_message(lua_State *L);
