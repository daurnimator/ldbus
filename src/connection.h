#include <stdbool.h>

#include <lua.h>

#include <dbus/dbus.h>

/* What we store in a DBusConnection userdata */
typedef struct {
    /* First field should be the pointer to DBusConnection,
       so we can cast this struct to a 'DBusConnection*'
    */
    DBusConnection *connection;
    /* Whether this connection is 'shared'
       If so, we need to call `dbus_connection_close` on collection
    */
    bool close;
} lDBusConnection;

#define DBUS_CONNECTION_METATABLE "ldbus_DBusConnection"

#define check_lDBusConnection(L, arg) ((lDBusConnection *)luaL_checkudata(L, arg, DBUS_CONNECTION_METATABLE))
#define check_DBusConnection(L, arg) (check_lDBusConnection(L, arg)->connection)

void push_DBusConnection(lua_State *L, DBusConnection *connection, bool close);
int luaopen_ldbus_connection(lua_State *L);
