# [DBus](http://www.freedesktop.org/wiki/Software/dbus/) for Lua.

ldbus is a C binding to dbus for Lua.

Compatible with Lua 5.1 and 5.2 (thanks [compat-5.2](https://github.com/hishamhm/lua-compat-5.2)).


# Status


# Installation

ldbus is on moonrocks: https://rocks.moonscript.org/modules/daurnimator/ldbus

Install via luarocks, you will need to provide paths to `dbus/dbus.h` and `dbus/dbus-arch-deps.h`

    luarocks install --server=http://rocks.moonscript.org/manifests/daurnimator ldbus DBUS_INCDIR=/usr/include/dbus-1.0/ DBUS_ARCH_INCDIR=/usr/lib/dbus-1.0/include


# Usage

example.lua is a lua version [Matthew Johnson's example](http://dbus.freedesktop.org/doc/dbus/libdbus-tutorial.html)


### Bound from C

 C                                                                                                                                                         | Lua                                               | Comments
-----------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------|--------------------------
[`dbus_bus_get()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga77ba5250adb84620f16007e1b023cf26)                                        | `ldbus.bus.get()`                                 |
[`dbus_bus_register()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0c21cf74d05c0bd8003846b56a588a4b)                                   | `ldbus.bus.register()`                            |
[`dbus_bus_set_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga0366177076e88bf37771341f32b0551c)                            | `ldbus.bus.set_unique_name()`                     |
[`dbus_bus_get_unique_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8c10339a1e62f6a2e5752d9c2270d37b)                            | `ldbus.bus.get_unique_name()`                     |
[`dbus_bus_request_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga8a9024c78c4ea89b6271f19dbc7861b2)                               | `ldbus.bus.request_name()`                        |
[`dbus_bus_release_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#gaa4aa5ebe51cdbe8c0651609fc72e841a)                               | `ldbus.bus.release_name()`                        |
[`dbus_bus_name_has_owner()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga5331b172dd8ed00eec130e894c5c2a0b)                             | `ldbus.bus.name_has_owner()`                      |
[`dbus_bus_start_service_by_name()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga81d303bf29d7c97ad4690ce35071b090)                      | `ldbus.bus.start_service_by_name()`               |
[`dbus_bus_add_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga4eb6401ba014da3dbe3dc4e2a8e5b3ef)                                  | `ldbus.bus.add_match()`                           |
[`dbus_bus_remove_match()`](http://dbus.freedesktop.org/doc/api/html/group__DBusBus.html#ga6e6b98e19fa4400de7ef99c27b866b99)                               | `ldbus.bus.remove_match()`                        |
