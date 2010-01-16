#include <stdlib.h>
#include <lua.h>
#include <lauxlib.h>

#include <dbus/dbus.h>

#include "bus.h"
#include "connection.h"
#include "message.h"

#include "ldbus.h"

int tostring ( lua_State *L ) {
	if ( !luaL_getmetafield ( L , 1 , "__udtype" ) ) {
                lua_pushstring ( L , "object with a generic __tostring metamethod but no __type metafield" );
        }
	return 1;
}

static int ldbus_get_version ( lua_State *L ) {
	int maj , min , mic;
	dbus_get_version ( &maj , &min , &mic );
	
	lua_pushinteger ( L , maj );
	lua_pushinteger ( L , min );
	lua_pushinteger ( L , mic );
	
	return 3;
}

int luaopen_ldbus ( lua_State *L ) {
	static const struct luaL_Reg ldbus [] = {
		{ "get_version" , 	ldbus_get_version },
		{ NULL , NULL }
	};
	luaL_register ( L , "ldbus" , ldbus );
	
	// Type Constants:
	lua_newtable ( L );
	lua_pushstring ( L , DBUS_TYPE_INVALID_AS_STRING );
	lua_setfield ( L , -2 , "invalid" );
	lua_pushstring ( L , DBUS_TYPE_BYTE_AS_STRING );
	lua_setfield ( L , -2 , "byte" );
	lua_pushstring ( L , DBUS_TYPE_BOOLEAN_AS_STRING );
	lua_setfield ( L , -2 , "boolean" );
	lua_pushstring ( L , DBUS_TYPE_INT16_AS_STRING );
	lua_setfield ( L , -2 , "int16" );
	lua_pushstring ( L , DBUS_TYPE_UINT16_AS_STRING );
	lua_setfield ( L , -2 , "uint16" );
	lua_pushstring ( L , DBUS_TYPE_INT32_AS_STRING );
	lua_setfield ( L , -2 , "int32" );
	lua_pushstring ( L , DBUS_TYPE_UINT32_AS_STRING );
	lua_setfield ( L , -2 , "uint32" );
	lua_pushstring ( L , DBUS_TYPE_INT64_AS_STRING );
	lua_setfield ( L , -2 , "int64" );
	lua_pushstring ( L , DBUS_TYPE_UINT64_AS_STRING );
	lua_setfield ( L , -2 , "uint64" );
	lua_pushstring ( L , DBUS_TYPE_STRING_AS_STRING );
	lua_setfield ( L , -2 , "string" );
	lua_pushstring ( L , DBUS_TYPE_OBJECT_PATH_AS_STRING );
	lua_setfield ( L , -2 , "object_path" );
	lua_pushstring ( L , DBUS_TYPE_SIGNATURE_AS_STRING );
	lua_setfield ( L , -2 , "signature" );
	lua_pushstring ( L , DBUS_TYPE_ARRAY_AS_STRING );
	lua_setfield ( L , -2 , "array" );
	lua_pushstring ( L , DBUS_TYPE_VARIANT_AS_STRING );
	lua_setfield ( L , -2 , "variant" );
	lua_pushstring ( L , DBUS_TYPE_STRUCT_AS_STRING );
	lua_setfield ( L , -2 , "struct" );
	lua_pushstring ( L , DBUS_TYPE_DICT_ENTRY_AS_STRING );
	lua_setfield ( L , -2 , "dict_entry" );
	lua_setfield ( L , -2 , "types" );
	
	load_dbus_connection ( L );
	lua_setfield ( L , -2 , "connection" );
	
	load_dbus_bus ( L );
	lua_setfield ( L , -2 , "bus" );
	
	load_dbus_message ( L );
	lua_setfield ( L , -2 , "message" );
	
	return 1;
}
