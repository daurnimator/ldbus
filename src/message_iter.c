#include <stdint.h>
#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#include "message_iter.h"

typedef union {
	/* int8_t      int8; */
	uint8_t     uint8;
	int16_t     int16;
	uint16_t    uint16;
	int32_t     int32;
	uint32_t    uint32;
	int64_t     int64;
	uint64_t    uint64;
	double	    dbl;
	/* float       flt; */
	const char *str;
} basic_type_u;

static int ldbus_message_iter_has_next(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	lua_pushboolean(L, dbus_message_iter_has_next(iter));

	return 1;
}

static int ldbus_message_iter_next(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	lua_pushboolean(L, dbus_message_iter_next(iter));

	return 1;
}

static int ldbus_message_iter_get_arg_type(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	char type = (char)dbus_message_iter_get_arg_type(iter);
	lua_pushlstring(L, &type, 1);

	return 1;
}

static int ldbus_message_iter_get_element_type(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	char type = (char)dbus_message_iter_get_element_type(iter);
	if (type == DBUS_TYPE_INVALID) {
		lua_pushboolean(L, FALSE);
	} else {
		lua_pushlstring(L, &type, 1);
	}

	return 1;
}

static int ldbus_message_iter_recurse(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");
	DBusMessageIter * sub;
	if (lua_gettop(L) == 1) {
		push_DBusMessageIter(L);
	} else {
		lua_settop(L, 2);
	}
	sub = luaL_checkudata(L, 2, "ldbus_DBusMessageIter");

	dbus_message_iter_recurse(iter, sub);

	return 1;
}

static int ldbus_message_iter_get_signature(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	char * sig = dbus_message_iter_get_signature(iter);

	lua_pushstring(L, sig);

	dbus_free(sig);

	return 1;
}
static int ldbus_message_iter_get_basic(lua_State *L) {
	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");

	basic_type_u value;
	switch (dbus_message_iter_get_arg_type(iter)) {
		case DBUS_TYPE_BOOLEAN:
			dbus_message_iter_get_basic(iter, &value.uint32);
			lua_pushboolean(L, value.uint32);
			break;
		case DBUS_TYPE_BYTE:
			dbus_message_iter_get_basic(iter, &value.uint8);
			lua_pushnumber(L, value.uint8);
			break;
		case DBUS_TYPE_INT16:
			dbus_message_iter_get_basic(iter, &value.int16);
			lua_pushnumber(L, value.int16);
			break;
		case DBUS_TYPE_UINT16:
			dbus_message_iter_get_basic(iter, &value.uint16);
			lua_pushnumber(L, value.uint16);
			break;
		case DBUS_TYPE_INT32:
			dbus_message_iter_get_basic(iter, &value.int32);
			lua_pushnumber(L, value.int32);
			break;
		case DBUS_TYPE_UINT32:
			dbus_message_iter_get_basic(iter, &value.uint32);
			lua_pushnumber(L, value.uint32);
			break;
		case DBUS_TYPE_INT64:
			dbus_message_iter_get_basic(iter, &value.int64);
			lua_pushnumber(L, value.int64);
			break;
		case DBUS_TYPE_UINT64:
			dbus_message_iter_get_basic(iter, &value.uint64);
			lua_pushnumber(L, value.uint64);
			break;
		case DBUS_TYPE_DOUBLE:
			dbus_message_iter_get_basic(iter, &value.dbl);
			lua_pushnumber(L, value.dbl);
			break;
		case DBUS_TYPE_STRING:
		case DBUS_TYPE_OBJECT_PATH:
		case DBUS_TYPE_SIGNATURE:
			dbus_message_iter_get_basic(iter, &value.str);
			lua_pushstring(L, value.str);
			break;
		case DBUS_TYPE_ARRAY:
		case DBUS_TYPE_STRUCT:
		case DBUS_TYPE_DICT_ENTRY:
		case DBUS_TYPE_VARIANT:
		default:
			lua_pushnil(L);
			lua_pushstring(L, "Encountered non-basic type");
			return 2;
	}
	return 1;
}

static int ldbus_message_iter_append_basic(lua_State *L) {
	int argtype;
	size_t l;
	basic_type_u value;

	DBusMessageIter * iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");
	luaL_checkany(L, 2);

	/* Get basic type we're appending... 0 if we want to use what's natural for lua. */
	switch (lua_type(L, 3)) {
		case LUA_TNONE:
		case LUA_TNIL:
			argtype = 0;
			break;
		default:
			argtype = (int) luaL_checklstring(L, 3, &l) [ 0 ];
			if (l != 1) return luaL_typerror(L, 3, "character or nil");
			break;
	}

	switch (lua_type(L, 2)) {
		case LUA_TNUMBER:
			value.dbl = lua_tonumber(L, 2);
			switch (argtype) {
				case 0:
					argtype = DBUS_TYPE_DOUBLE; /* Lua numbers are doubles by default */
				case DBUS_TYPE_DOUBLE:
					break;
				case DBUS_TYPE_BYTE:
					value.uint8 = (uint8_t)value.dbl;
					break;
				case DBUS_TYPE_INT16:
					value.int16 = (int16_t)value.dbl;
					break;
				case DBUS_TYPE_UINT16:
					value.uint16 = (uint16_t)value.dbl;
					break;
				case DBUS_TYPE_INT32:
					value.int32 = (int32_t)value.dbl;
					break;
				case DBUS_TYPE_BOOLEAN:
				case DBUS_TYPE_UINT32:
					value.uint32 = (uint32_t)value.dbl;
					break;
				case DBUS_TYPE_INT64:
					value.int64 = (int64_t)value.dbl;
					break;
				case DBUS_TYPE_UINT64:
					value.uint64 = (uint64_t)value.dbl;
					break;
				default:
					return luaL_argerror(L, 3, "cannot convert number to given type");
			}
			break;
		case LUA_TBOOLEAN:
			value.uint32 = (uint32_t)lua_toboolean(L, 2);
			switch (argtype) {
				case 0:
					argtype = DBUS_TYPE_BOOLEAN;
				case DBUS_TYPE_BOOLEAN:
				case DBUS_TYPE_BYTE:
				case DBUS_TYPE_INT16:
				case DBUS_TYPE_UINT16:
				case DBUS_TYPE_INT32:
				case DBUS_TYPE_UINT32:
				case DBUS_TYPE_INT64:
				case DBUS_TYPE_UINT64:
					break;
				case DBUS_TYPE_DOUBLE:
					value.dbl = (double)value.uint32;
					break;
				default:
					return luaL_argerror(L, 3, "cannot convert boolean to given type");
			}
			break;
		case LUA_TSTRING:
			value.str = lua_tostring(L, 2);
			switch (argtype) {
				case 0:
					argtype = DBUS_TYPE_STRING;
				case DBUS_TYPE_STRING:
				case DBUS_TYPE_OBJECT_PATH:
				case DBUS_TYPE_SIGNATURE:
					break;
				default:
					return luaL_argerror(L, 3, "cannot convert string to given type");
			}
			break;
		case LUA_TNIL:
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
		case LUA_TLIGHTUSERDATA:
		default:
			return luaL_typerror(L, 3, "number, boolean or string");
	}

	lua_pushboolean(L, dbus_message_iter_append_basic(iter, argtype, &value));

	return 1;
}

static int ldbus_message_iter_open_container(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");
	DBusMessageIter *sub = luaL_checkudata(L, 2, "ldbus_DBusMessageIter");
	size_t l;
	int argtype = (int) luaL_checklstring(L, 3, &l) [ 0 ];
	const char *contained_signature;
	if (l != 1) return luaL_typerror(L, 3, "character");
	contained_signature = luaL_optstring(L, 4, NULL);

	lua_pushboolean(L, dbus_message_iter_open_container(iter, argtype, contained_signature, sub));

	return 1;
}

static int ldbus_message_iter_close_container(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, "ldbus_DBusMessageIter");
	DBusMessageIter *sub = luaL_checkudata(L, 2, "ldbus_DBusMessageIter");

	lua_pushboolean(L, dbus_message_iter_close_container(iter, sub));

	return 1;
}

int push_DBusMessageIter(lua_State *L) {
	static luaL_Reg const methods [] = {
		{ "has_next",         ldbus_message_iter_has_next },
		{ "next",             ldbus_message_iter_next },
		{ "get_arg_type",     ldbus_message_iter_get_arg_type },
		{ "get_element_type", ldbus_message_iter_get_element_type },
		{ "recurse",          ldbus_message_iter_recurse },
		{ "get_signature",    ldbus_message_iter_get_signature },
		{ "get_basic",        ldbus_message_iter_get_basic },
		{ "append_basic",     ldbus_message_iter_append_basic },
		{ "open_container",   ldbus_message_iter_open_container },
		{ "close_container",  ldbus_message_iter_close_container },
		{ NULL, NULL }
	};

	lua_newuserdata(L, sizeof(DBusMessageIter));

	if (luaL_newmetatable(L, "ldbus_DBusMessageIter")) {
		lua_newtable(L);
		luaL_register(L, NULL, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushstring(L, "DBusMessageIter");
		lua_setfield(L, -2, "__udtype");
	}
	lua_setmetatable(L, -2);

	return 1;
}

void load_dbus_message_iter(lua_State *L) {
	static const struct luaL_Reg ldbus_message_iter [] = {
		{ "new", push_DBusMessageIter },
		{ NULL, NULL }
	};
	lua_newtable(L);
	luaL_register(L, NULL, ldbus_message_iter);
}
