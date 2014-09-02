#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"
#include "compat-5.2.h"

#define NUP 3

static int test_newproxy (lua_State *L) {
  lua_settop(L, 0);
  lua_newuserdata(L, 0);
  lua_newtable(L);
  lua_pushvalue(L, -1);
  lua_pushboolean(L, 1);
  lua_setfield(L, -2, "__gc");
  lua_setmetatable(L, -3);
  return 2;
}

static int test_absindex (lua_State *L) {
  int i = 1;
  for (i = 1; i <= NUP; ++i)
    lua_pushvalue(L, lua_absindex(L, lua_upvalueindex(i)));
  lua_pushvalue(L, lua_absindex(L, LUA_REGISTRYINDEX));
  lua_pushstring(L, lua_typename(L, lua_type(L, lua_absindex(L, -1))));
  lua_replace(L, lua_absindex(L, -2));
  lua_pushvalue(L, lua_absindex(L, -2));
  lua_pushvalue(L, lua_absindex(L, -4));
  lua_pushvalue(L, lua_absindex(L, -6));
  i += 3;
  lua_pushvalue(L, lua_absindex(L, 1));
  lua_pushvalue(L, lua_absindex(L, 2));
  lua_pushvalue(L, lua_absindex(L, 3));
  i += 3;
  return i;
}

static int test_globals (lua_State *L) {
  lua_pushglobaltable(L);
  return 1;
}

static int test_tonumber (lua_State *L) {
  int isnum = 0;
  lua_Number n = lua_tonumberx(L, 1, &isnum);
  if (!isnum)
    lua_pushnil(L);
  else
    lua_pushnumber(L, n);
  return 1;
}

static int test_tointeger (lua_State *L) {
  int isnum = 0;
  lua_Integer n = lua_tointegerx(L, 1, &isnum);
  if (!isnum)
    lua_pushnil(L);
  else
    lua_pushinteger(L, n);
  return 1;
}

static int test_unsigned (lua_State *L) {
  lua_Unsigned u = luaL_checkunsigned(L, 1);
  lua_Unsigned u2 = lua_tounsigned(L, 1);
  lua_pushunsigned(L, u);
  lua_pushboolean(L, u == u2);
  return 2;
}

static int test_optunsigned (lua_State *L) {
  lua_Unsigned u = luaL_optunsigned(L, 1, 17u);
  lua_pushunsigned(L, u);
  return 1;
}

static int test_len (lua_State *L) {
  luaL_checkany(L, 1);
  lua_len(L, 1);
  lua_pushinteger(L, luaL_len(L, 1));
  return 2;
}

static int test_copy (lua_State *L) {
  int args = lua_gettop(L);
  if (args >= 2) {
    int i = 0;
    for (i = args-1; i > 0; --i)
      lua_copy(L, args, i);
  }
  return args;
}

/* need an address */
static char const dummy = 0;

static int test_rawxetp (lua_State *L) {
  if (lua_gettop(L) > 0)
    lua_pushvalue(L, 1);
  else
    lua_pushliteral(L, "hello again");
  lua_rawsetp(L, LUA_REGISTRYINDEX, &dummy);
  lua_settop(L, 0);
  lua_rawgetp(L, LUA_REGISTRYINDEX, &dummy);
  return 1;
}

static int test_udata (lua_State *L) {
  const char *tname = luaL_optstring(L, 1, "utype1");
  void *u1 = lua_newuserdata(L, 1);
  int u1pos = lua_gettop(L);
  void *u2 = lua_newuserdata(L, 1);
  int u2pos = lua_gettop(L);
  luaL_newmetatable(L, "utype1");
  luaL_newmetatable(L, "utype2");
  lua_pop(L, 2);
  luaL_setmetatable(L, "utype2");
  lua_pushvalue(L, u1pos);
  luaL_setmetatable(L, "utype1");
  lua_pop(L, 1);
  (void)u1;
  (void)u2;
  lua_pushlightuserdata(L, luaL_testudata(L, u1pos, tname));
  lua_pushlightuserdata(L, luaL_testudata(L, u2pos, tname));
  return 2;
}

static int test_subtable (lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1);
  if (luaL_getsubtable(L, 1, "xxx")) {
    lua_pushliteral(L, "oldtable");
  } else {
    lua_pushliteral(L, "newtable");
  }
  return 2;
}

static int test_uservalue (lua_State *L) {
  void *udata = lua_newuserdata(L, 1);
  int ui = lua_gettop(L);
  lua_getuservalue(L, ui);
  lua_newtable(L);
  lua_setuservalue(L, ui);
  lua_getuservalue(L, ui);
  lua_pushnil(L);
  lua_setuservalue(L, ui);
  lua_getuservalue(L, ui);
  (void)udata;
  return 3;
}

static int test_upvalues (lua_State *L) {
  int i = 1;
  for (i = 1; i <= NUP; ++i)
    lua_pushvalue(L, lua_upvalueindex(i));
  return NUP;
}

static int test_tolstring (lua_State *L) {
  size_t len = 0;
  luaL_tolstring(L, 1, &len);
  lua_pushinteger(L, (int)len);
  return 2;
}

static int my_mod (lua_State *L) {
  lua_newtable(L);
  lua_pushboolean(L, 1);
  lua_setfield(L, -2, "boolean");
  return 1;
}

static int test_requiref (lua_State *L) {
  luaL_requiref(L, "requiref1", my_mod, 0);
  luaL_requiref(L, "requiref2", my_mod, 1);
  return 2;
}

static const luaL_Reg funcs[] = {
  { "newproxy", test_newproxy },
  { "tonumber", test_tonumber },
  { "tointeger", test_tointeger },
  { "unsigned", test_unsigned },
  { "optunsigned", test_optunsigned },
  { "len", test_len },
  { "copy", test_copy },
  { "rawxetp", test_rawxetp },
  { "subtable", test_subtable },
  { "udata", test_udata },
  { "uservalue", test_uservalue },
  { "globals", test_globals },
  { "tolstring", test_tolstring },
  { "requiref", test_requiref },
  { NULL, NULL }
};

static const luaL_Reg more_funcs[] = {
  { "getupvalues", test_upvalues },
  { "absindex", test_absindex },
  { NULL, NULL }
};

int luaopen_testmod (lua_State *L) {
  int i = 1;
  luaL_newlib(L, funcs);
  /* defeats the purpose of luaL_newlib, but this is test code: */
  for (i = 1; i <= NUP; ++i)
    lua_pushnumber(L, i);
  luaL_setfuncs(L, more_funcs, NUP);
  return 1;
}

