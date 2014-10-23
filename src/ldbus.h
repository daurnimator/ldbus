#include <lua.h>

#define UNUSED(x) (void)(x)

#define LDBUS_NO_MEMORY "no memory"

int tostring ( lua_State * );

int luaopen_curses ( lua_State * );
