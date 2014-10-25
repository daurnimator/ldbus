#include <lua.h>

#define UNUSED(x) (void)(x)
#define LDBUS_INTERNAL __attribute__ ((visibility ("internal")))
#define LDBUS_NO_MEMORY "no memory"

LDBUS_INTERNAL int tostring(lua_State *);

int luaopen_ldbus(lua_State *);
