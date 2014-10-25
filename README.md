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

  - Functions that normally take a `DBusError` return `nil, "error_message"` on failure.
  - Errors will be thrown on out of memory

 C                                                                                                                                                         | Lua                                                                     | Comments
-----------------------------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------|--------------------------
[`dbus_bus_get()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga77ba5250adb84620f16007e1b023cf26)                                        | `ldbus.bus.get()`                                                       | Takes `type` as one of `"session"`, `"system"` or `"starter"`
[`dbus_bus_register()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0c21cf74d05c0bd8003846b56a588a4b)                                   | `ldbus.bus.register()`                                                  |
[`dbus_bus_set_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0366177076e88bf37771341f32b0551c)                            | `ldbus.bus.set_unique_name()`                                           |
[`dbus_bus_get_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8c10339a1e62f6a2e5752d9c2270d37b)                            | `ldbus.bus.get_unique_name()`                                           |
[`dbus_bus_request_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8a9024c78c4ea89b6271f19dbc7861b2)                               | `ldbus.bus.request_name()`                                              | Takes `flags` as a table contain keys `"allow_replacement"`, `"do_not_queue"` and `"replace_existing"`; On success returns one of `"primary_owner"`, `"in_queue"`, `"exists"` or `"already_owner"`
[`dbus_bus_release_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#gaa4aa5ebe51cdbe8c0651609fc72e841a)                               | `ldbus.bus.release_name()`                                              | On success, returns one of `"released"`, `"non_existant"` or `"not_owner"`
[`dbus_bus_name_has_owner()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga5331b172dd8ed00eec130e894c5c2a0b)                             | `ldbus.bus.name_has_owner()`                                            |
[`dbus_bus_start_service_by_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga81d303bf29d7c97ad4690ce35071b090)                      | `ldbus.bus.start_service_by_name()`                                     |
[`dbus_bus_add_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga4eb6401ba014da3dbe3dc4e2a8e5b3ef)                                  | `ldbus.bus.add_match()`                                                 | On success, returns one of `"success"` or `"already_running"`
[`dbus_bus_remove_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga6e6b98e19fa4400de7ef99c27b866b99)                               | `ldbus.bus.remove_match()`                                              |
[`dbus_connection_open()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gacd32f819820266598c6b6847dfddaf9c)                         | `ldbus.connection.open()`                                               |
[`dbus_connection_get_is_connected()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga611ae94556af36fe30bfb547366ca4e1)             | `my_conn:get_is_connected()`                                            |
[`dbus_connection_get_is_authenticated()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga2f1fa02c9897b6f07f4d33c862de4a1d)         | `my_conn:get_is_authenticated()`                                        |
[`dbus_connection_get_is_anonymous()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaa6c5d523e16d8a5b9316c92d9ff1ac17)             | `my_conn:get_is_anonymous()`                                            |
[`dbus_connection_get_server_id()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gae6c19e146a37f9de6a06c1617874bed9)                | `my_conn:get_server_id()`                                               |
[`dbus_connection_send()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gae1cb64f4cf550949b23fd3a756b2f7d0)                         | `my_conn:send()`                                                        |
[`dbus_connection_send_with_reply()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaa215df7ab7ca6dce7be153c6b9cc80ba)              | `my_conn:send_with_reply()`                                             |
[`dbus_connection_send_with_reply_and_block()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga8d6431f17a9e53c9446d87c2ba8409f0)    | `my_conn:send_with_reply_and_block()`                                   |
[`dbus_connection_flush()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga10e68d9d2f41d655a4151ddeb807ff54)                        | `my_conn:flush()`                                                       |
[`dbus_connection_read_write_dispatch()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga580d8766c23fe5f49418bc7d87b67dc6)          | `my_conn:read_write_dispatch()`                                         |
[`dbus_connection_read_write()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga371163b4955a6e0bf0f1f70f38390c14)                   | `my_conn:read_write()`                                                  |
[`dbus_connection_pop_message()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga1e40d994ea162ce767e78de1c4988566)                  | `my_conn:pop_message()`                                                 |
[`dbus_connection_get_dispatch_status()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga893d18d8b36ffb371f16d13645071289)          | `my_conn:get_dispatch_status()`                                         |
[`dbus_connection_dispatch()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga66ba7df50d75f4bda6b6e942430b81c7)                     | `my_conn:dispatch()`                                                    |
[`dbus_connection_set_watch_functions()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gaebf031eb444b4f847606aa27daa3d8e6)          | `my_conn:set_watch_functions()`                                         |
[`dbus_connection_set_timeout_functions()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gab3cbc68eec427e9ce1783b25d44fe93c)        | `my_conn:set_timeout_functions()`                                       |
[`dbus_connection_set_dispatch_status_function()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga55ff88cd22c0672441c7deffbfb68fbf) | `my_conn:set_dispatch_status_function()`                                |
[`dbus_connection_register_object_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga24730ca6fd2e9132873962a32df7628c)         | `my_conn:register_object_path()`                                        |
[`dbus_connection_register_fallback()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gac4473b37bfa74ccf7459959d27e7bc59)            | `my_conn:register_fallback()`                                           |
[`dbus_connection_unregister_object_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga6ae8f005dedf24c5f2df1768795392fb)       | `my_conn:unregister_object_path()`                                      |
[`dbus_connection_set_max_message_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga0d783462274a6c71d3767f5821c29ce9)         | `my_conn:set_max_message_size()`                                        |
[`dbus_connection_get_max_message_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga7a459e64d7dca7b592136cec0a73422c)         | `my_conn:get_max_message_size()`                                        |
[`dbus_connection_set_max_received_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga6565d75f16e6e803372b2ae3d94d991b)        | `my_conn:set_max_received_size()`                                       |
[`dbus_connection_get_max_received_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga376529acf41d1d34b4f46c0d9d515c85)        | `my_conn:get_max_received_size()`                                       |
[`dbus_connection_get_outgoing_size()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#ga47aff801f586e7116f9c54532bb1baf9)            | `my_conn:get_outgoing_size()`                                           |
[`dbus_connection_has_messages_to_send()`](http://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html#gac40563ec4c0e309d936daf3163ba9bb7)         | `my_conn:has_messages_to_send()`                                        |
[`dbus_message_new()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gab9e5bf8d87a95c5ca7026a791148ebd4)                                | `ldbus.message.new()`                                                   |
[`dbus_message_new_method_call()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga98ddc82450d818138ef326a284201ee0)                    | `ldbus.message.new_method_call()`                                       |
[`dbus_message_new_signal()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga6ce3213dfb17be7956affba40207a5a0)                         | `ldbus.message.new_signal()`                                            |
[`dbus_message_get_serial()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga390710c25564c80025a006c376da2030)                         | `my_message:get_serial()`                                               |
[`dbus_message_set_reply_serial()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaec08603ff3d49bbcded67d25188a23f1)                   | `my_message:set_reply_serial()`                                         |
[`dbus_message_get_reply_serial()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga94c43b2b237d842a6b91da6f94818d47)                   | `my_message:get_reply_serial()`                                         |
[`dbus_message_new_method_return()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga95142bd8288f397194ee0eefb1d27125)                  | `my_message:new_method_return()`                                        |
[`dbus_message_new_error()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga2ab896965aec97fb21293affeed36232)                          | `my_message:new_error()`                                                |
[`dbus_message_copy()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga4bed3858b3b48ec7c86d9fc56a6ce372)                               | `my_message:copy()`                                                     |
[`dbus_message_get_type()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga41cace31999105137772b6257ea540f9)                           | `my_message:get_type()`                                                 |
[`dbus_message_iter_init()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga9f98b47c84f0e401ea985e681de4e963)                          | `my_message:iter_init()`                                                |
[`dbus_message_iter_init_append()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaf733047c467ce21f4a53b65a388f1e9d)                   | `my_message:iter_init_append()`                                         |
[`dbus_message_set_no_reply()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga0e86aeb2dc6831ccc9a21fcbf8cc16f7)                       | `my_message:set_no_reply()`                                             |
[`dbus_message_get_no_reply()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga622d051a2e5f578814116a958b240aa4)                       | `my_message:get_no_reply()`                                             |
[`dbus_message_set_auto_start()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga1596d92a8d604f954b48c7410263d2f0)                     | `my_message:set_auto_start()`                                           |
[`dbus_message_get_auto_start()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga85d396a3a774e15c3dbb7704aa173384)                     | `my_message:get_auto_start()`                                           |
[`dbus_message_set_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaaf25da0ba1482266293d329314c21786)                           | `my_message:set_path()`                                                 |
[`dbus_message_get_path()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga18adf731bb42d324fe2624407319e4af)                           | `my_message:get_path()`                                                 |
[`dbus_message_get_path_decomposed()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaf2b5b3319da838b1f1b360c04a33f153)                | `my_message:get_path_decomposed()`                                      |
[`dbus_message_set_interface()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga02b754855e4d9a1cade8e4fc17a3f5c7)                      | `my_message:set_interface()`                                            |
[`dbus_message_get_interface()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga1ad192bd4538cae556121a71b4e09d42)                      | `my_message:get_interface()`                                            |
[`dbus_message_set_member()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga3afdda6016816cc70b451d8657065208)                         | `my_message:set_member()`                                               |
[`dbus_message_get_member()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaf5c6b705c53db07a5ae2c6b76f230cf9)                         | `my_message:get_member()`                                               |
[`dbus_message_set_error_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga892f9857707371c2a53cec6b54c843c7)                     | `my_message:set_error_name()`                                           |
[`dbus_message_get_error_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga4e98b2283707a8e0313fc7c6fe3b1b5f)                     | `my_message:get_error_name()`                                           |
[`dbus_message_set_destination()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gacc47c1af23addfc4198b70084ba068fc)                    | `my_message:set_destination()`                                          |
[`dbus_message_get_destination()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaac65c926e6253e49aa689b4f032fad45)                    | `my_message:get_destination()`                                          |
[`dbus_message_set_sender()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaa2170744c2c19217d9df02551f16bc92)                         | `my_message:set_sender()`                                               |
[`dbus_message_get_sender()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga13ce514ceb2d1598751f3a7760cf1375)                         | `my_message:get_sender()`                                               |
[`dbus_message_get_signature()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaed63e4c2baaa50d782e8ebb7643def19)                      | `my_message:get_signature()`                                            |
[`dbus_message_iter_has_next()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaaffc75a699c96ff6197287f166df2149)                      | `bool = my_message_iter:has_next()`                                     |
[`dbus_message_iter_next()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga554e9fafd4dcc84cebe9da9344846a82)                          | `bool = my_message_iter:next()`                                         |
[`dbus_message_iter_get_element_type()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga868a7aeddb9b54b2805776b512f68cb4)              | `type = my_message_iter:get_element_type()`                             | `DBUS_TYPE_INVALID` is returned as `nil`
[`dbus_message_iter_recurse()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga7652e1208743da5dd4ecc5aef07bf207)                       | `sub_iter = my_message_iter:recurse([sub_iter])`                        | Creates a new `DBusMessageIter` if one is not passed
[`dbus_message_iter_get_signature()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gab4579a88a1a7eaf648350466f585ef8b)                 | `sig = my_message_iter:get_signature()`                                 |
[`dbus_message_iter_get_basic()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga41c23a05e552d0574d0444d4693d18ab)                     | `object = my_message_iter:get_basic()`                                  | number types are all returned as lua numbers (even 64bit integers at the moment; so watch out); strings, object paths and signatures will be returned as strings
[`dbus_message_iter_append_basic()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga17491f3b75b3203f6fc47dcc2e3b221b)                  | `my_message_iter:append_basic(object [, type])`                         |
[`dbus_message_iter_open_container()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#ga943150f4e87fd8507da224d22c266100)                | `sub_iter = my_message_iter:open_container(type [, sign [, sub_iter]])` | Creates `sub_iter` if not passed. Returns the `sub_iter` or `nil` on failure
[`dbus_message_iter_close_container()`](http://dbus.freedesktop.org/doc/api/html/group__DBusMessage.html#gaf00482f63d4af88b7851621d1f24087a)               | `ok = my_message_iter:close_container(sub_iter)`                        |
[`dbus_pending_call_cancel()`](http://dbus.freedesktop.org/doc/api/html/group__DBusPendingCall.html#ga6530d18f891d3ca5f5df87ea7c2b155c)                    | `my_pending_call:cancel()`                                              |
[`dbus_pending_call_get_completed()`](http://dbus.freedesktop.org/doc/api/html/group__DBusPendingCall.html#gacbf33ae8a1cc125628f9ea44d175c159)             | `completed = my_pending_call:get_completed()                            |
[`dbus_pending_call_steal_reply()`](http://dbus.freedesktop.org/doc/api/html/group__DBusPendingCall.html#ga5a738928c2369fef093ce00658903d06)               | `message = my_pending_call:steal_reply()                                |
[`dbus_pending_call_block()`](http://dbus.freedesktop.org/doc/api/html/group__DBusPendingCall.html#ga67b99f749a7f477c7b5d70f2acee5452)                     | `my_pending_call:block()`                                               |
[`dbus_timeout_get_interval()`](http://dbus.freedesktop.org/doc/api/html/group__DBusTimeout.html#ga67ba21b6189438875c6007ee79da5e37)                       | `secs = my_timeout:get_interval()`                                      | Returned number is in seconds (rather than milliseconds)
[`dbus_timeout_handle()`](http://dbus.freedesktop.org/doc/api/html/group__DBusTimeout.html#ga038b67c8d3db2624a1e4a8bc45f25d12)                             | `ok = my_timeout:handle()`                                              |
[`dbus_timeout_get_enabled()`](http://dbus.freedesktop.org/doc/api/html/group__DBusTimeout.html#ga58954b2edb45ec1632529d35525ea45c)                        | `enabled = my_timeout:get_enabled()`                                    |


### Other

#### `my_iter = ldbus.message.iter.new()`

Creates a new `DBusMessageIter` object


