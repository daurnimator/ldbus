#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include <dbus/dbus.h>

#include "ldbus.h"

#include "message_iter.h"


static int ldbus_message_iter_clone(lua_State *L) {
	lDBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);
	lDBusMessageIter *clone;
	push_DBusMessageIter(L);
	clone = lua_touserdata(L, -1);
	memcpy(clone, iter, sizeof(lDBusMessageIter));

	if (clone->message) {
		dbus_message_ref(clone->message);
	}

	return 1;
}

static int ldbus_message_iter_has_next(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	lua_pushboolean(L, dbus_message_iter_has_next(iter));

	return 1;
}

static int ldbus_message_iter_next(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	lua_pushboolean(L, dbus_message_iter_next(iter));

	return 1;
}

static int ldbus_message_iter_get_arg_type(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	char type = (char)dbus_message_iter_get_arg_type(iter);
	if (type == DBUS_TYPE_INVALID) {
		lua_pushnil(L);
	} else {
		lua_pushlstring(L, &type, 1);
	}

	return 1;
}

static int ldbus_message_iter_get_element_count(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	lua_pushinteger(L, dbus_message_iter_get_element_count(iter));

	return 1;
}

static int ldbus_message_iter_get_element_type(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	char type = (char)dbus_message_iter_get_element_type(iter);
	if (type == DBUS_TYPE_INVALID) {
		lua_pushnil(L);
	} else {
		lua_pushlstring(L, &type, 1);
	}

	return 1;
}

static int ldbus_message_iter_recurse(lua_State *L) {
	lDBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);
	lDBusMessageIter *sub;
	if (lua_gettop(L) == 1) {
		push_DBusMessageIter(L);
		sub = lua_touserdata(L, 2);
	} else {
		lua_settop(L, 2);
		sub = luaL_checkudata(L, 2, DBUS_MESSAGE_ITER_METATABLE);
		/* remove possible reference to previously referred message */
		unref_ldbus_message_iter(sub);
	}

	dbus_message_iter_recurse(&iter->iter, &sub->iter);
	sub->message = iter->message;
	dbus_message_ref(sub->message);

	return 1;
}

static int ldbus_message_iter_get_signature(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	char * sig = dbus_message_iter_get_signature(iter);

	lua_pushstring(L, sig);

	dbus_free(sig);

	return 1;
}
static int ldbus_message_iter_get_basic(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);

	DBusBasicValue value;
	switch (dbus_message_iter_get_arg_type(iter)) {
		case DBUS_TYPE_BOOLEAN:
			dbus_message_iter_get_basic(iter, &value.u32);
			lua_pushboolean(L, value.u32);
			break;
		case DBUS_TYPE_BYTE:
			dbus_message_iter_get_basic(iter, &value.byt);
			lua_pushinteger(L, value.byt);
			break;
		case DBUS_TYPE_INT16:
			dbus_message_iter_get_basic(iter, &value.i16);
			lua_pushinteger(L, value.i16);
			break;
		case DBUS_TYPE_UINT16:
			dbus_message_iter_get_basic(iter, &value.u16);
			lua_pushinteger(L, value.u16);
			break;
		case DBUS_TYPE_INT32:
			dbus_message_iter_get_basic(iter, &value.i32);
			lua_pushinteger(L, value.i32);
			break;
		case DBUS_TYPE_UINT32:
			dbus_message_iter_get_basic(iter, &value.u32);
			lua_pushinteger(L, value.u32);
			break;
		case DBUS_TYPE_INT64:
			dbus_message_iter_get_basic(iter, &value.i64);
			lua_pushinteger(L, value.i64);
			break;
		case DBUS_TYPE_UINT64:
			dbus_message_iter_get_basic(iter, &value.u64);
			lua_pushinteger(L, value.u64);
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
	DBusBasicValue value;
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);
	luaL_checkany(L, 2);

	/* Get basic type we're appending... 0 if we want to use what's natural for lua. */
	switch (lua_type(L, 3)) {
		case LUA_TNONE:
		case LUA_TNIL:
			argtype = 0;
			break;
		default:
			argtype = (int) luaL_checklstring(L, 3, &l) [ 0 ];
			if (l != 1) return luaL_argerror(L, 3, lua_pushfstring(L, "character or nil expected, got %s", luaL_typename(L, 3)));
			break;
	}

	switch (lua_type(L, 2)) {
		case LUA_TNUMBER:
			if (lua_isinteger(L, 2)) {
				value.i64 = lua_tointeger(L, 2);
				switch (argtype) {
					case 0:
						argtype = DBUS_TYPE_INT64;
					case DBUS_TYPE_INT64:
						break;
					case DBUS_TYPE_BYTE:
						value.byt = value.i64;
						break;
					case DBUS_TYPE_INT16:
						value.i16 = value.i64;
						break;
					case DBUS_TYPE_UINT16:
						value.u16 = value.i64;
						break;
					case DBUS_TYPE_INT32:
						value.i32 = value.i64;
						break;
					case DBUS_TYPE_BOOLEAN:
					case DBUS_TYPE_UINT32:
						value.u32 = value.i64;
						break;
					case DBUS_TYPE_UINT64:
						value.u64 = value.i64;
						break;
					case DBUS_TYPE_DOUBLE:
						value.dbl = value.i64;
						break;
					default:
						return luaL_argerror(L, 2, "cannot convert number to given type");
				}
			} else {
				value.dbl = lua_tonumber(L, 2);
				switch (argtype) {
					case 0:
						argtype = DBUS_TYPE_DOUBLE; /* Lua numbers are doubles by default */
					case DBUS_TYPE_DOUBLE:
						break;
					case DBUS_TYPE_BYTE:
						value.byt = value.dbl;
						break;
					case DBUS_TYPE_INT16:
						value.i16 = value.dbl;
						break;
					case DBUS_TYPE_UINT16:
						value.u16 = value.dbl;
						break;
					case DBUS_TYPE_INT32:
						value.i32 = value.dbl;
						break;
					case DBUS_TYPE_BOOLEAN:
					case DBUS_TYPE_UINT32:
						value.u32 = value.dbl;
						break;
					case DBUS_TYPE_INT64:
						value.i64 = value.dbl;
						break;
					case DBUS_TYPE_UINT64:
						value.u64 = value.dbl;
						break;
					default:
						return luaL_argerror(L, 2, "cannot convert number to given type");
				}
			}
			break;
		case LUA_TBOOLEAN:
			value.u32 = lua_toboolean(L, 2);
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
					value.dbl = value.u32;
					break;
				default:
					return luaL_argerror(L, 2, "cannot convert boolean to given type");
			}
			break;
		case LUA_TSTRING:
			value.str = (char *)lua_tostring(L, 2);
			switch (argtype) {
				case 0:
					argtype = DBUS_TYPE_STRING;
				case DBUS_TYPE_STRING:
				case DBUS_TYPE_OBJECT_PATH:
				case DBUS_TYPE_SIGNATURE:
					break;
				default:
					return luaL_argerror(L, 2, "cannot convert string to given type");
			}
			break;
		case LUA_TNIL:
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
		case LUA_TLIGHTUSERDATA:
		default:
			return luaL_argerror(L, 2, lua_pushfstring(L, "number, boolean or string expected, got %s", luaL_typename(L, 2)));
	}

	lua_pushboolean(L, dbus_message_iter_append_basic(iter, argtype, &value));

	return 1;
}

static int ldbus_message_iter_open_container(lua_State *L) {
	lDBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);
	int argtype;
	const char *contained_signature;
	lDBusMessageIter *sub;
	if (lua_type(L, 2) != LUA_TSTRING || lua_rawlen(L, 2) != 1) {
		return luaL_argerror(L, 2, lua_pushfstring(L, "character expected, got %s", luaL_typename(L, 2)));
	}
	argtype = lua_tostring(L, 2)[0];
	contained_signature = luaL_optstring(L, 3, NULL);
	if (lua_gettop(L) < 4) {
		push_DBusMessageIter(L);
		sub = lua_touserdata(L, -1);
	} else {
		lua_settop(L, 4);
		sub = luaL_checkudata(L, 4, DBUS_MESSAGE_ITER_METATABLE);
		/* remove possible reference to previously referred message */
		unref_ldbus_message_iter(sub);
	}

	if (!dbus_message_iter_open_container(&iter->iter, argtype, contained_signature, &sub->iter)) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	sub->message = iter->message;
	dbus_message_ref(sub->message);

	return 1;
}

static int ldbus_message_iter_close_container(lua_State *L) {
	DBusMessageIter *iter = luaL_checkudata(L, 1, DBUS_MESSAGE_ITER_METATABLE);
	DBusMessageIter *sub = luaL_checkudata(L, 2, DBUS_MESSAGE_ITER_METATABLE);

	lua_pushboolean(L, dbus_message_iter_close_container(iter, sub));

	return 1;
}

static int ldbus_message_iter_gc(lua_State *L)
{
	lDBusMessageIter *iter = luaL_checkudata(L, -1, DBUS_MESSAGE_ITER_METATABLE);

	if (iter->message) {
		dbus_message_unref(iter->message);
		iter->message = NULL;
	}

	return 0;
}

LDBUS_INTERNAL void unref_ldbus_message_iter(lDBusMessageIter *iter) {
	if (iter->message) {
		dbus_message_unref(iter->message);
		iter->message = NULL;
	}
}

LDBUS_INTERNAL int push_DBusMessageIter(lua_State *L) {
	static luaL_Reg const methods [] = {
		{ "clone",		ldbus_message_iter_clone },
		{ "has_next",		ldbus_message_iter_has_next },
		{ "next",		ldbus_message_iter_next },
		{ "get_arg_type",	ldbus_message_iter_get_arg_type },
		{ "get_element_count",	ldbus_message_iter_get_element_count },
		{ "get_element_type",	ldbus_message_iter_get_element_type },
		{ "recurse",		ldbus_message_iter_recurse },
		{ "get_signature",	ldbus_message_iter_get_signature },
		{ "get_basic",		ldbus_message_iter_get_basic },
		{ "append_basic",	ldbus_message_iter_append_basic },
		{ "open_container",	ldbus_message_iter_open_container },
		{ "close_container",	ldbus_message_iter_close_container },
		{ NULL, NULL }
	};

	lDBusMessageIter *iter = lua_newuserdata(L, sizeof(lDBusMessageIter));
	iter->message = NULL;

	if (luaL_newmetatable(L, DBUS_MESSAGE_ITER_METATABLE)) {
		luaL_newlib(L, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushcfunction(L, ldbus_message_iter_gc);
		lua_setfield(L, -2, "__gc");

		lua_pushstring(L, "DBusMessageIter");
		lua_setfield(L, -2, "__udtype");
	}
	lua_setmetatable(L, -2);

	return 1;
}

LDBUS_INTERNAL void load_dbus_message_iter(lua_State *L) {
	static const struct luaL_Reg ldbus_message_iter [] = {
		{ "new", push_DBusMessageIter },
		{ NULL, NULL }
	};
	luaL_newlib(L, ldbus_message_iter);
}
