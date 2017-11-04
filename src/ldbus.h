#ifndef LDBUS_H
#define LDBUS_H

#include <lua.h>

#define UNUSED(x) (void)(x)
#define LDBUS_INTERNAL __attribute__ ((visibility ("internal")))
#define LDBUS_NO_MEMORY "no memory"

enum ldbus_callback_indexes {
	DBUS_LUA_FUNC_FUNC = 1,
	DBUS_LUA_FUNC_THREAD = 2,
	/* for watch and timeout */
	DBUS_LUA_FUNC_ADD = 1,
	DBUS_LUA_FUNC_REMOVE = 3,
	DBUS_LUA_FUNC_TOGGLE = 4
};

LDBUS_INTERNAL int tostring(lua_State *);

int luaopen_ldbus(lua_State *);

#endif
