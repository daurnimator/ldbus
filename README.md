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
