#include <stdlib.h>

#include <lua.h>
#include <lauxlib.h>
#include "compat-5.3.h"

#include <dbus/dbus.h>

#include "ldbus.h"
#include "connection.h"
#include "message.h"
#include "error.h"

#include "bus.h"

static const char *const BusType_lst [] = {
	"session", /* DBUS_BUS_SESSION == 0 */
	"system",  /* DBUS_BUS_SYSTEM  == 1 */
	"starter", /* DBUS_BUS_STARTER == 2 */
	NULL
};

static const char *const Request_Name_Reply_lst [] = {
	NULL,            /* No 0... */
	"primary_owner", /* DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER == 1 */
	"in_queue",      /* DBUS_REQUEST_NAME_REPLY_IN_QUEUE      == 2 */
	"exists",        /* DBUS_REQUEST_NAME_REPLY_EXISTS        == 3 */
	"already_owner", /* DBUS_REQUEST_NAME_REPLY_ALREADY_OWNER == 4 */
	NULL
};

static const char *const Release_Name_Reply_lst [] = {
	NULL,           /* No 0... */
	"released",     /* DBUS_RELEASE_NAME_REPLY_RELEASED     == 1 */
	"non_existent", /* DBUS_RELEASE_NAME_REPLY_NON_EXISTENT == 2 */
	"not_owner",    /* DBUS_RELEASE_NAME_REPLY_NOT_OWNER    == 3 */
	NULL
};

static const char *const Start_Reply_lst [] = {
	NULL,              /* No 0... */
	"success",         /* DBUS_START_REPLY_SUCCESS         == 1 */
	"already_running", /* DBUS_START_REPLY_ALREADY_RUNNING == 2 */
	NULL
};

static int ldbus_bus_get(lua_State *L) {
	int type = luaL_checkoption(L, 1, NULL, BusType_lst);

	DBusError *error = new_DBusError(L);
	DBusConnection *connection = dbus_bus_get(type, error);

	if (dbus_error_is_set(error)) {
		lua_pushboolean(L, FALSE);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		dbus_connection_set_exit_on_disconnect(connection, FALSE);
		push_DBusConnection(L, connection, FALSE);
		return 1;
	}
}

static int ldbus_bus_register(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);

	DBusError *error = new_DBusError(L);
	dbus_bus_register(connection, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushboolean(L, TRUE);
		return 1;
	}
}

static int ldbus_bus_set_unique_name(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *unique_name = luaL_checkstring(L, 2);

	lua_pushboolean(L, dbus_bus_set_unique_name(connection, unique_name));

	return 1;
}

static int ldbus_bus_get_unique_name(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);

	const char *unique_name = dbus_bus_get_unique_name(connection);
	if (unique_name == NULL) {
		lua_pushnil(L);
	} else {
		lua_pushstring(L, unique_name);
	}

	return 1;
}

static int ldbus_bus_request_name(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *name = luaL_checkstring(L, 2);
	unsigned int flags = 0;
	DBusError *error;
	int result;

	switch (lua_type(L, 3)) {
		case LUA_TNIL:
		case LUA_TNONE:
			break;
		case LUA_TTABLE:
			lua_getfield(L, 3, "allow_replacement");
			if (lua_toboolean(L, -1)) flags |= DBUS_NAME_FLAG_ALLOW_REPLACEMENT;
			lua_getfield(L, 3, "do_not_queue");
			if (lua_toboolean(L, -1)) flags |= DBUS_NAME_FLAG_DO_NOT_QUEUE;
			lua_getfield(L, 3, "replace_existing");
			if (lua_toboolean(L, -1)) flags |= DBUS_NAME_FLAG_REPLACE_EXISTING;
			break;
		default:
			return luaL_argerror(L, 3, lua_pushfstring(L, "table or nil expected, got %s", luaL_typename(L, 3)));
			break;
	}

	error = new_DBusError(L);
	result = dbus_bus_request_name(connection, name, flags, error);
	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushstring(L, Request_Name_Reply_lst [ result ]);
		return 1;
	}
}

static int ldbus_bus_release_name(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *name = luaL_checkstring(L, 2);
	int result;

	DBusError *error = new_DBusError(L);
	result = dbus_bus_release_name(connection, name, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushstring(L, Release_Name_Reply_lst [ result ]);
		return 1;
	}
}

static int ldbus_bus_name_has_owner(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *name = luaL_checkstring(L, 2);
	int result;

	DBusError *error = new_DBusError(L);
	result = dbus_bus_name_has_owner(connection, name, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushboolean(L, result);
		return 1;
	}
}

static int ldbus_bus_start_service_by_name(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *name = luaL_checkstring(L, 2);
	unsigned int result;

	DBusError *error = new_DBusError(L);
	dbus_bus_start_service_by_name(connection, name, 0, &result, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushstring(L, Start_Reply_lst [ result ]);
		return 1;
	}
}

static int ldbus_bus_add_match(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *rule = luaL_checkstring(L, 2);

	DBusError *error = new_DBusError(L);
	dbus_bus_add_match(connection, rule, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushboolean(L, TRUE);
		return 1;
	}
}

static int ldbus_bus_remove_match(lua_State *L) {
	DBusConnection *connection = check_DBusConnection(L, 1);
	const char *rule = luaL_checkstring(L, 2);

	DBusError *error = new_DBusError(L);
	dbus_bus_remove_match(connection, rule, error);

	if (dbus_error_is_set(error)) {
		lua_pushnil(L);
		lua_pushstring(L, error->message);
		return 2;
	} else {
		lua_pushboolean(L, TRUE);
		return 1;
	}
}

int luaopen_ldbus_bus(lua_State *L) {
	static const struct luaL_Reg ldbus_bus [] = {
		{ "get",                   ldbus_bus_get },
		{ "register",              ldbus_bus_register },
		{ "set_unique_name",       ldbus_bus_set_unique_name },
		{ "get_unique_name",       ldbus_bus_get_unique_name },
		{ "request_name",          ldbus_bus_request_name },
		{ "release_name",          ldbus_bus_release_name },
		{ "name_has_owner",        ldbus_bus_name_has_owner },
		{ "start_service_by_name", ldbus_bus_start_service_by_name },
		{ "add_match",             ldbus_bus_add_match },
		{ "remove_match",          ldbus_bus_remove_match },
		{ NULL, NULL }
	};
	luaL_newlib(L, ldbus_bus);
	return 1;
}
