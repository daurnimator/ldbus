#include <lua.h>

#include <dbus/dbus.h>

void push_DBusConnection ( lua_State *L , DBusConnection * connection );
void load_dbus_connection ( lua_State *L );
