# [DBus](http://www.freedesktop.org/wiki/Software/dbus/) for Lua.

ldbus is a C binding to dbus for Lua.

Compatible with Lua 5.1 and 5.2 (thanks [compat-5.2](https://github.com/hishamhm/lua-compat-5.2)).


# Status


# Installation

ldbus is on moonrocks: https://rocks.moonscript.org/modules/daurnimator/ldbus

Install via luarocks, you will need to provide paths to `dbus/dbus.h` and `dbus/dbus-arch-deps.h`

    luarocks install --server=http://rocks.moonscript.org/manifests/daurnimator ldbus DBUS_INCDIR=/usr/include/dbus-1.0/ DBUS_ARCH_INCDIR=/usr/lib/dbus-1.0/include


# Usage

example.lua is a lua version of [Matthew Johnson's example](http://dbus.freedesktop.org/doc/dbus/libdbus-tutorial.html)


### Bound from C

Functions that normally take a `DBusError` return `nil, "error_message"` on failure.

 C                                                                                                                                                         | Lua                                               | Comments
-----------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------|--------------------------
[`dbus_bus_get()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga77ba5250adb84620f16007e1b023cf26)                                        | `ldbus.bus.get()`                                 | Takes `type` as one of `"session"`, `"system"` or `"starter"`
[`dbus_bus_register()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0c21cf74d05c0bd8003846b56a588a4b)                                   | `ldbus.bus.register()`                            |
[`dbus_bus_set_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0366177076e88bf37771341f32b0551c)                            | `ldbus.bus.set_unique_name()`                     |
[`dbus_bus_get_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8c10339a1e62f6a2e5752d9c2270d37b)                            | `ldbus.bus.get_unique_name()`                     |
[`dbus_bus_request_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8a9024c78c4ea89b6271f19dbc7861b2)                               | `ldbus.bus.request_name()`                        | Takes `flags` as a table contain keys `"allow_replacement"`, `"do_not_queue"` and `"replace_existing"`; On success returns one of `"primary_owner"`, `"in_queue"`, `"exists"` or `"already_owner"`
[`dbus_bus_release_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#gaa4aa5ebe51cdbe8c0651609fc72e841a)                               | `ldbus.bus.release_name()`                        | On success, returns one of `"released"`, `"non_existant"` or `"not_owner"`
[`dbus_bus_name_has_owner()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga5331b172dd8ed00eec130e894c5c2a0b)                             | `ldbus.bus.name_has_owner()`                      |
[`dbus_bus_start_service_by_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga81d303bf29d7c97ad4690ce35071b090)                      | `ldbus.bus.start_service_by_name()`               |
[`dbus_bus_add_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga4eb6401ba014da3dbe3dc4e2a8e5b3ef)                                  | `ldbus.bus.add_match()`                           | On success, returns one of `"success"` or `"already_running"`
[`dbus_bus_remove_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga6e6b98e19fa4400de7ef99c27b866b99)                               | `ldbus.bus.remove_match()`                        |
[`dbus_connection_open()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gacd32f819820266598c6b6847dfddaf9c)                         | `ldbus.connection.open()`                         |
[`dbus_connection_get_is_connected()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga611ae94556af36fe30bfb547366ca4e1)             | `my_conn:get_is_connected()`                      |
[`dbus_connection_get_is_authenticated()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga2f1fa02c9897b6f07f4d33c862de4a1d)         | `my_conn:get_is_authenticated()`                  |
[`dbus_connection_get_is_anonymous()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaa6c5d523e16d8a5b9316c92d9ff1ac17)             | `my_conn:get_is_anonymous()`                      |
[`dbus_connection_get_server_id()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gae6c19e146a37f9de6a06c1617874bed9)                | `my_conn:get_server_id()`                         |
[`dbus_connection_send()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gae1cb64f4cf550949b23fd3a756b2f7d0)                         | `my_conn:send()`                                  |
[`dbus_connection_send_with_reply()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaa215df7ab7ca6dce7be153c6b9cc80ba)              | `my_conn:send_with_reply()`                       |
[`dbus_connection_send_with_reply_and_block()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga8d6431f17a9e53c9446d87c2ba8409f0)    | `my_conn:send_with_reply_and_block()`             |
[`dbus_connection_flush()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga10e68d9d2f41d655a4151ddeb807ff54)                        | `my_conn:flush()`                                 |
[`dbus_connection_read_write_dispatch()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga580d8766c23fe5f49418bc7d87b67dc6)          | `my_conn:read_write_dispatch()`                   |
[`dbus_connection_read_write()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga371163b4955a6e0bf0f1f70f38390c14)                   | `my_conn:read_write()`                            |
[`dbus_connection_pop_message()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga1e40d994ea162ce767e78de1c4988566)                  | `my_conn:pop_message()`                           |
[`dbus_connection_get_dispatch_status()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga893d18d8b36ffb371f16d13645071289)          | `my_conn:get_dispatch_status()`                   |
[`dbus_connection_dispatch()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga66ba7df50d75f4bda6b6e942430b81c7)                     | `my_conn:dispatch()`                              |
[`dbus_connection_set_watch_functions()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaebf031eb444b4f847606aa27daa3d8e6)          | `my_conn:set_watch_functions()`                   |
[`dbus_connection_set_timeout_functions()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gab3cbc68eec427e9ce1783b25d44fe93c)        | `my_conn:set_timeout_functions()`                 |
[`dbus_connection_set_dispatch_status_function()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga55ff88cd22c0672441c7deffbfb68fbf) | `my_conn:set_dispatch_status_function()`          |
[`dbus_connection_register_object_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga24730ca6fd2e9132873962a32df7628c)         | `my_conn:register_object_path()`                  |
[`dbus_connection_register_fallback()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gac4473b37bfa74ccf7459959d27e7bc59)            | `my_conn:register_fallback()`                     |
[`dbus_connection_unregister_object_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga6ae8f005dedf24c5f2df1768795392fb)       | `my_conn:unregister_object_path()`                |
[`dbus_connection_set_max_message_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga0d783462274a6c71d3767f5821c29ce9)         | `my_conn:set_max_message_size()`                  |
[`dbus_connection_get_max_message_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga7a459e64d7dca7b592136cec0a73422c)         | `my_conn:get_max_message_size()`                  |
[`dbus_connection_set_max_received_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga6565d75f16e6e803372b2ae3d94d991b)        | `my_conn:set_max_received_size()`                 |
[`dbus_connection_get_max_received_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga376529acf41d1d34b4f46c0d9d515c85)        | `my_conn:get_max_received_size()`                 |
[`dbus_connection_get_outgoing_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga47aff801f586e7116f9c54532bb1baf9)            | `my_conn:get_outgoing_size()`                     |
[`dbus_connection_has_messages_to_send()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gac40563ec4c0e309d936daf3163ba9bb7)         | `my_conn:has_messages_to_send()`                  |
