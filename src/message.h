#include <lua.h>

#include <dbus/dbus.h>

void push_DBusMessage ( lua_State *L , DBusMessage * message );
void load_dbus_message ( lua_State *L );
