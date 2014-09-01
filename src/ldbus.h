#include <lua.h>

#define LDBUS_NO_MEMORY "no memory"

int tostring ( lua_State * );

//static int ldbus_parse_address ( lua_State *L );

int luaopen_curses ( lua_State * );
