#include <lua.h>

#include <dbus/dbus.h>

#define DBUS_MESSAGE_METATABLE "ldbus_DBusMessage"

#define check_DBusMessage(L, arg) (*(DBusMessage **)luaL_checkudata(L, arg, DBUS_MESSAGE_METATABLE))


void push_DBusMessage(lua_State *L, DBusMessage * message);
int luaopen_ldbus_message(lua_State *L);
