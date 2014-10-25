#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"
#include "error.h"

static int ldbus_error_free(lua_State *L) {
	DBusError *error = check_DBusError(L, 1);
	dbus_error_free(error);
	return 0;
}

/* Creates a new DBusError, and pushes it onto the lua stack */
LDBUS_INTERNAL DBusError* new_DBusError(lua_State *L) {
	DBusError *res = lua_newuserdata(L, sizeof(DBusError));
	if (luaL_newmetatable(L, DBUS_ERROR_METATABLE)) {
		lua_newtable(L);
		lua_pushcfunction(L, ldbus_error_free);
		lua_setfield(L, -2, "__gc");
	}
	dbus_error_init(res);
	return res;
}
