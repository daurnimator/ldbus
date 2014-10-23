#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "ldbus.h"

#include "message_iter.h"

#include "message.h"


static int ldbus_message_get_serial(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	lua_pushinteger(L, dbus_message_get_serial(message));

	return 1;
}

static int ldbus_message_set_reply_serial(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	dbus_uint32_t reply_serial = luaL_checkint(L, 2);

	lua_pushboolean(L, dbus_message_set_reply_serial(message, reply_serial));

	return 1;
}

static int ldbus_message_get_reply_serial(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	lua_pushinteger(L, dbus_message_get_reply_serial(message));

	return 1;
}

static int ldbus_message_new(lua_State *L) {
	int message_type = dbus_message_type_from_string(luaL_checkstring(L, 1));

	DBusMessage * message = dbus_message_new(message_type);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_new_method_call(lua_State *L) {
	const char * destination = luaL_optstring(L, 1, NULL);
	const char * path = luaL_checkstring(L, 2);
	const char * interface = luaL_optstring(L, 3, NULL);
	const char * method = luaL_checkstring(L, 4);

	DBusMessage * message = dbus_message_new_method_call(destination, path, interface, method);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_new_method_return(lua_State *L) {
	DBusMessage *methodcall = check_DBusMessage(L, 1);

	DBusMessage *message = dbus_message_new_method_return(methodcall);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_new_signal(lua_State *L) {
	const char * path = luaL_checkstring(L, 1);
	const char * interface = luaL_checkstring(L, 2);
	const char * name = luaL_checkstring(L, 3);

	DBusMessage * message = dbus_message_new_signal(path, interface, name);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_new_error(lua_State *L) {
	DBusMessage *reply_to = check_DBusMessage(L, 1);
	const char *name = luaL_checkstring(L, 2);
	const char *error_message = luaL_optstring(L, 3, NULL);

	DBusMessage * message = dbus_message_new_error(reply_to, name, error_message);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_copy(lua_State *L) {
	DBusMessage *orig = check_DBusMessage(L, 1);

	DBusMessage *message = dbus_message_copy(orig);
	if (message == NULL) {
		return luaL_error(L, LDBUS_NO_MEMORY);
	}

	push_DBusMessage(L, message);
	return 1;
}

static int ldbus_message_unref(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	dbus_message_unref(message);

	return 0;
}

static int ldbus_message_get_type(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	lua_pushstring(L, dbus_message_type_to_string(dbus_message_get_type(message)));
	return 1;
}

static int ldbus_message_iter_init(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	DBusMessageIter * iter = luaL_checkudata(L, 2, "ldbus_DBusMessageIter");

	lua_pushboolean(L, dbus_message_iter_init(message, iter));

	return 1;
}

static int ldbus_message_iter_init_append(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	DBusMessageIter * iter;
	if (lua_gettop(L) == 1) {
		push_DBusMessageIter(L);
	} else {
		lua_settop(L, 2);
	}
	iter = luaL_checkudata(L, 2, "ldbus_DBusMessageIter");

	dbus_message_iter_init_append(message, iter);

	return 1;
}

static int ldbus_message_set_no_reply(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	int no_reply = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));

	dbus_message_set_no_reply(message, no_reply);

	return 0;
}

static int ldbus_message_get_no_reply(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	lua_pushboolean(L, dbus_message_get_no_reply(message));

	return 1;
}

static int ldbus_message_set_auto_start(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	int auto_start = (luaL_checktype(L, 2, LUA_TBOOLEAN), lua_toboolean(L, 2));

	dbus_message_set_auto_start(message, auto_start);

	return 0;
}

static int ldbus_message_get_auto_start(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	lua_pushboolean(L, dbus_message_get_auto_start(message));

	return 1;
}

static int ldbus_message_set_path(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * object_path = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_path(message, object_path));

	return 1;
}

static int ldbus_message_get_path(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * object_path = dbus_message_get_path(message);
	if (object_path == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, object_path);
	}

	return 1;
}

static int ldbus_message_get_path_decomposed(lua_State *L) {
	int i;
	DBusMessage *message = check_DBusMessage(L, 1);

	char ** path;
	if (dbus_message_get_path_decomposed(message, &path) == FALSE) {
		lua_pushboolean(L, FALSE);
	} else if (path == NULL) {
		lua_pushnil(L);
	} else {
		lua_newtable(L);
		for (i=0 ; path [ i ] != NULL ;) {
			lua_pushstring(L, path [ i ]);
			lua_rawseti(L, -2, ++i);
		}
		dbus_free_string_array(path);
	}

	return 1;
}

static int ldbus_message_set_interface(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * interface = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_interface(message, interface));

	return 1;
}

static int ldbus_message_get_interface(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * interface = dbus_message_get_interface(message);
	if (interface == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, interface);
	}

	return 1;
}

static int ldbus_message_set_member(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * member = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_member(message, member));

	return 1;
}

static int ldbus_message_get_member(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * member = dbus_message_get_member(message);
	if (member == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, member);
	}

	return 1;
}

static int ldbus_message_set_error_name(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * error_name = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_error_name(message, error_name));

	return 1;
}

static int ldbus_message_get_error_name(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * error_name = dbus_message_get_error_name(message);
	if (error_name == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, error_name);
	}

	return 1;
}

static int ldbus_message_set_destination(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * destination = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_destination(message, destination));

	return 1;
}

static int ldbus_message_get_destination(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * destination = dbus_message_get_destination(message);
	if (destination == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, destination);
	}

	return 1;
}

static int ldbus_message_set_sender(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);
	const char * sender = luaL_optstring(L, 2, NULL);

	lua_pushboolean(L, dbus_message_set_sender(message, sender));

	return 1;
}

static int ldbus_message_get_sender(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * sender = dbus_message_get_sender(message);
	if (sender == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, sender);
	}

	return 1;
}

static int ldbus_message_get_signature(lua_State *L) {
	DBusMessage *message = check_DBusMessage(L, 1);

	const char * signature = dbus_message_get_signature(message);
	if (signature == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, signature);
	}

	return 1;
}

void push_DBusMessage(lua_State *L, DBusMessage * message) {
	static luaL_Reg const methods [] = {
		{ "get_serial",          ldbus_message_get_serial },
		{ "set_reply_serial",    ldbus_message_set_reply_serial },
		{ "get_reply_serial",    ldbus_message_get_reply_serial },
		{ "new_method_return",   ldbus_message_new_method_return },
		{ "new_error",           ldbus_message_new_error },
		{ "copy",                ldbus_message_copy },
		{ "get_type",            ldbus_message_get_type },
		{ "iter_init",           ldbus_message_iter_init },
		{ "iter_init_append",    ldbus_message_iter_init_append },
		{ "set_no_reply",        ldbus_message_set_no_reply },
		{ "get_no_reply",        ldbus_message_get_no_reply },
		{ "set_auto_start",      ldbus_message_set_auto_start },
		{ "get_auto_start",      ldbus_message_get_auto_start },
		{ "set_path",            ldbus_message_set_path },
		{ "get_path",            ldbus_message_get_path },
		{ "get_path_decomposed", ldbus_message_get_path_decomposed },
		{ "set_interface",       ldbus_message_set_interface },
		{ "get_interface",       ldbus_message_get_interface },
		{ "set_member",          ldbus_message_set_member },
		{ "get_member",          ldbus_message_get_member },
		{ "set_error_name",      ldbus_message_set_error_name },
		{ "get_error_name",      ldbus_message_get_error_name },
		{ "set_destination",     ldbus_message_set_destination },
		{ "get_destination",     ldbus_message_get_destination },
		{ "set_sender",          ldbus_message_set_sender },
		{ "get_sender",          ldbus_message_get_sender },
		{ "get_signature",       ldbus_message_get_signature },
		{ NULL, NULL }
	};

	DBusMessage ** udata = lua_newuserdata(L, sizeof(DBusMessage *));
	*udata = message;

	if (luaL_newmetatable(L, DBUS_MESSAGE_METATABLE)) {
		lua_newtable(L);
		luaL_register(L, NULL, methods);
		lua_setfield(L, -2, "__index");

		lua_pushcfunction(L, ldbus_message_unref) ;
		lua_setfield(L, -2, "__gc");

		lua_pushcfunction(L, tostring);
		lua_setfield(L, -2, "__tostring");

		lua_pushstring(L, "DBusMessage");
		lua_setfield(L, -2, "__udtype");
	}
	lua_setmetatable(L, -2);
}

int luaopen_ldbus_message(lua_State *L) {
	static const struct luaL_Reg ldbus_message [] = {
		{ "new",             ldbus_message_new },
		{ "new_method_call", ldbus_message_new_method_call },
		{ "new_signal",      ldbus_message_new_signal },
		{ NULL, NULL }
	};
	lua_newtable(L);
	luaL_register(L, NULL, ldbus_message);

	load_dbus_message_iter(L);
	lua_setfield(L, -2, "iter");
	return 1;
}
