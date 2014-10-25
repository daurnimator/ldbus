#ifndef LDBUS_H
#define LDBUS_H

#include <lua.h>

#define UNUSED(x) (void)(x)
#define LDBUS_INTERNAL __attribute__ ((visibility ("internal")))
#define LDBUS_NO_MEMORY "no memory"

typedef struct {
	lua_State* L;
	int ref;
} ldbus_callback_udata;

enum ldbus_callback_indexes {
	DBUS_LUA_FUNC_ADD,
	DBUS_LUA_FUNC_REMOVE,
	DBUS_LUA_FUNC_TOGGLE
};

LDBUS_INTERNAL int tostring(lua_State *);

int luaopen_ldbus(lua_State *);

#endif
