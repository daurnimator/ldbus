#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>

#include "compat-5.3.h"

#include <dbus/dbus.h>

#include "bus.h"
#include "connection.h"
#include "message.h"
#include "watch.h"
#include "ldbus.h"

int tostring(lua_State *L) {
	if (!luaL_getmetafield(L, 1, "__udtype")) {
		lua_pushstring(L, "object with a generic __tostring metamethod but no __type metafield");
	}
	lua_pushfstring(L, "%s: %p", lua_tostring(L, -1), lua_topointer(L, -2));
	return 1;
}

static int ldbus_get_version(lua_State *L) {
	int maj, min, mic;
	dbus_get_version(&maj, &min, &mic);

	lua_pushinteger(L, maj);
	lua_pushinteger(L, min);
	lua_pushinteger(L, mic);

	return 3;
}

int luaopen_ldbus(lua_State *L) {
	static const struct luaL_Reg ldbus [] = {
		{ "get_version", ldbus_get_version },
		{ NULL, NULL }
	};
	luaL_newlib(L, ldbus);

	/* Type Constants: */
	lua_newtable(L);
	lua_pushliteral(L, DBUS_TYPE_INVALID_AS_STRING);
	lua_setfield(L, -2, "invalid");
	lua_pushliteral(L, DBUS_TYPE_BYTE_AS_STRING);
	lua_setfield(L, -2, "byte");
	lua_pushliteral(L, DBUS_TYPE_BOOLEAN_AS_STRING);
	lua_setfield(L, -2, "boolean");
	lua_pushliteral(L, DBUS_TYPE_DOUBLE_AS_STRING);
	lua_setfield(L, -2, "double");
	lua_pushliteral(L, DBUS_TYPE_INT16_AS_STRING);
	lua_setfield(L, -2, "int16");
	lua_pushliteral(L, DBUS_TYPE_UINT16_AS_STRING);
	lua_setfield(L, -2, "uint16");
	lua_pushliteral(L, DBUS_TYPE_INT32_AS_STRING);
	lua_setfield(L, -2, "int32");
	lua_pushliteral(L, DBUS_TYPE_UINT32_AS_STRING);
	lua_setfield(L, -2, "uint32");
	lua_pushliteral(L, DBUS_TYPE_INT64_AS_STRING);
	lua_setfield(L, -2, "int64");
	lua_pushliteral(L, DBUS_TYPE_UINT64_AS_STRING);
	lua_setfield(L, -2, "uint64");
	lua_pushliteral(L, DBUS_TYPE_STRING_AS_STRING);
	lua_setfield(L, -2, "string");
	lua_pushliteral(L, DBUS_TYPE_OBJECT_PATH_AS_STRING);
	lua_setfield(L, -2, "object_path");
	lua_pushliteral(L, DBUS_TYPE_SIGNATURE_AS_STRING);
	lua_setfield(L, -2, "signature");
	lua_pushliteral(L, DBUS_TYPE_ARRAY_AS_STRING);
	lua_setfield(L, -2, "array");
	lua_pushliteral(L, DBUS_TYPE_VARIANT_AS_STRING);
	lua_setfield(L, -2, "variant");
	lua_pushliteral(L, DBUS_TYPE_STRUCT_AS_STRING);
	lua_setfield(L, -2, "struct");
	lua_pushliteral(L, DBUS_TYPE_DICT_ENTRY_AS_STRING);
	lua_setfield(L, -2, "dict_entry");
	lua_setfield(L, -2, "types");

	/* Seperate table of just the basic types */
	lua_newtable(L);
	lua_pushliteral(L, DBUS_TYPE_BYTE_AS_STRING);
	lua_setfield(L, -2, "byte");
	lua_pushliteral(L, DBUS_TYPE_BOOLEAN_AS_STRING);
	lua_setfield(L, -2, "boolean");
	lua_pushliteral(L, DBUS_TYPE_DOUBLE_AS_STRING);
	lua_setfield(L, -2, "double");
	lua_pushliteral(L, DBUS_TYPE_INT16_AS_STRING);
	lua_setfield(L, -2, "int16");
	lua_pushliteral(L, DBUS_TYPE_UINT16_AS_STRING);
	lua_setfield(L, -2, "uint16");
	lua_pushliteral(L, DBUS_TYPE_INT32_AS_STRING);
	lua_setfield(L, -2, "int32");
	lua_pushliteral(L, DBUS_TYPE_UINT32_AS_STRING);
	lua_setfield(L, -2, "uint32");
	lua_pushliteral(L, DBUS_TYPE_INT64_AS_STRING);
	lua_setfield(L, -2, "int64");
	lua_pushliteral(L, DBUS_TYPE_UINT64_AS_STRING);
	lua_setfield(L, -2, "uint64");
	lua_pushliteral(L, DBUS_TYPE_STRING_AS_STRING);
	lua_setfield(L, -2, "string");
	lua_pushliteral(L, DBUS_TYPE_OBJECT_PATH_AS_STRING);
	lua_setfield(L, -2, "object_path");
	lua_pushliteral(L, DBUS_TYPE_SIGNATURE_AS_STRING);
	lua_setfield(L, -2, "signature");
	lua_setfield(L, -2, "basic_types");

	/* Errors */
	lua_newtable(L);
	lua_pushstring(L, DBUS_ERROR_FAILED);
	lua_setfield(L, -2, "Failed");
	lua_pushstring(L, DBUS_ERROR_NO_MEMORY);
	lua_setfield(L, -2, "NoMemory");
	lua_pushstring(L, DBUS_ERROR_SERVICE_UNKNOWN);
	lua_setfield(L, -2, "ServiceUnknown");
	lua_pushstring(L, DBUS_ERROR_NAME_HAS_NO_OWNER);
	lua_setfield(L, -2, "NameHasNoOwner");
	lua_pushstring(L, DBUS_ERROR_NO_REPLY);
	lua_setfield(L, -2, "NoReply");
	lua_pushstring(L, DBUS_ERROR_IO_ERROR);
	lua_setfield(L, -2, "IOError");
	lua_pushstring(L, DBUS_ERROR_BAD_ADDRESS);
	lua_setfield(L, -2, "BadAddress");
	lua_pushstring(L, DBUS_ERROR_NOT_SUPPORTED);
	lua_setfield(L, -2, "NotSupported");
	lua_pushstring(L, DBUS_ERROR_LIMITS_EXCEEDED);
	lua_setfield(L, -2, "LimitsExceeded");
	lua_pushstring(L, DBUS_ERROR_ACCESS_DENIED);
	lua_setfield(L, -2, "AccessDenied");
	lua_pushstring(L, DBUS_ERROR_AUTH_FAILED);
	lua_setfield(L, -2, "AuthFailed");
	lua_pushstring(L, DBUS_ERROR_NO_SERVER);
	lua_setfield(L, -2, "NoServer");
	lua_pushstring(L, DBUS_ERROR_TIMEOUT);
	lua_setfield(L, -2, "Timeout");
	lua_pushstring(L, DBUS_ERROR_NO_NETWORK);
	lua_setfield(L, -2, "NoNetwork");
	lua_pushstring(L, DBUS_ERROR_ADDRESS_IN_USE);
	lua_setfield(L, -2, "AddressInUse");
	lua_pushstring(L, DBUS_ERROR_DISCONNECTED);
	lua_setfield(L, -2, "Disconnected");
	lua_pushstring(L, DBUS_ERROR_INVALID_ARGS);
	lua_setfield(L, -2, "InvalidArgs");
	lua_pushstring(L, DBUS_ERROR_FILE_NOT_FOUND);
	lua_setfield(L, -2, "FileNotFound");
	lua_pushstring(L, DBUS_ERROR_FILE_EXISTS);
	lua_setfield(L, -2, "FileExists");
	lua_pushstring(L, DBUS_ERROR_UNKNOWN_METHOD);
	lua_setfield(L, -2, "UnknownMethod");
	lua_pushstring(L, DBUS_ERROR_TIMED_OUT);
	lua_setfield(L, -2, "TimedOut");
	lua_pushstring(L, DBUS_ERROR_MATCH_RULE_NOT_FOUND);
	lua_setfield(L, -2, "MatchRuleNotFound");
	lua_pushstring(L, DBUS_ERROR_MATCH_RULE_INVALID);
	lua_setfield(L, -2, "MatchRuleInvalid");
	lua_newtable(L);
	lua_pushstring(L, DBUS_ERROR_SPAWN_EXEC_FAILED);
	lua_setfield(L, -2, "ExecFailed");
	lua_pushstring(L, DBUS_ERROR_SPAWN_FORK_FAILED);
	lua_setfield(L, -2, "ForkFailed");
	lua_pushstring(L, DBUS_ERROR_SPAWN_CHILD_EXITED);
	lua_setfield(L, -2, "ChildExited");
	lua_pushstring(L, DBUS_ERROR_SPAWN_CHILD_SIGNALED);
	lua_setfield(L, -2, "ChildSignaled");
	lua_pushstring(L, DBUS_ERROR_SPAWN_FAILED);
	lua_setfield(L, -2, "Failed");
	lua_pushstring(L, DBUS_ERROR_SPAWN_SETUP_FAILED);
	lua_setfield(L, -2, "FailedToSetup");
	lua_pushstring(L, DBUS_ERROR_SPAWN_CONFIG_INVALID);
	lua_setfield(L, -2, "ConfigInvalid");
	lua_pushstring(L, DBUS_ERROR_SPAWN_SERVICE_INVALID);
	lua_setfield(L, -2, "ServiceNotValid");
	lua_pushstring(L, DBUS_ERROR_SPAWN_SERVICE_NOT_FOUND);
	lua_setfield(L, -2, "ServiceNotFound");
	lua_pushstring(L, DBUS_ERROR_SPAWN_PERMISSIONS_INVALID);
	lua_setfield(L, -2, "PermissionsInvalid");
	lua_pushstring(L, DBUS_ERROR_SPAWN_FILE_INVALID);
	lua_setfield(L, -2, "FileInvalid");
	lua_pushstring(L, DBUS_ERROR_SPAWN_NO_MEMORY);
	lua_setfield(L, -2, "NoMemory");
	lua_setfield(L, -2, "Spawn");
	lua_pushstring(L, DBUS_ERROR_UNIX_PROCESS_ID_UNKNOWN);
	lua_setfield(L, -2, "UnixProcessIdUnknown");
	lua_pushstring(L, DBUS_ERROR_INVALID_SIGNATURE);
	lua_setfield(L, -2, "InvalidSignature");
	lua_pushstring(L, DBUS_ERROR_INVALID_FILE_CONTENT);
	lua_setfield(L, -2, "InvalidFileContent");
	lua_pushstring(L, DBUS_ERROR_SELINUX_SECURITY_CONTEXT_UNKNOWN);
	lua_setfield(L, -2, "SELinuxSecurityContextUnknown");

	/* Unofficial but common additions: */
	lua_pushstring(L, "org.freedesktop.UnknownObject");
	lua_setfield(L, -2, "UnknownObject");
	lua_pushstring(L, "org.freedesktop.UnknownMethod");
	lua_setfield(L, -2, "UnknownMethod");
	lua_setfield(L, -2, "errors");

	luaL_requiref(L, "ldbus.connection", luaopen_ldbus_connection, 0);
	lua_setfield(L, -2, "connection");

	luaL_requiref(L, "ldbus.bus", luaopen_ldbus_bus, 0);
	lua_setfield(L, -2, "bus");

	luaL_requiref(L, "ldbus.message", luaopen_ldbus_message, 0);
	lua_setfield(L, -2, "message");

	luaL_requiref(L, "ldbus.watch", luaopen_ldbus_watch, 0);
	lua_setfield(L, -2, "watch");

	return 1;
}
