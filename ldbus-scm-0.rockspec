package = "ldbus"
version = "scm-0"
source = {
	 url = "git://github.com/daurnimator/ldbus.git"
}
description = {
	 summary = "A Lua library to access dbus.",
	 homepage = "https://github.com/daurnimator/ldbus",
	 license = "MIT/X11"
}
dependencies = {
	 "lua >= 5.1, < 5.3"
}
external_dependencies = {
	DBUS = { header = "dbus/dbus.h" },
	DBUS_ARCH = { header = "dbus/dbus-arch-deps.h" }
}
build = {
	type = "builtin",
	modules = {
		ldbus = {
			sources = {
				"src/ldbus.c",
				"src/bus.c",
				"src/connection.c",
				"src/message.c",
				"src/message_iter.c",
				"src/pending_call.c",
				"vendor/compat-5.2/c-api/compat-5.2.c"
			},
			libraries = { "dbus-1" },
			incdirs = { "$(DBUS_INCDIR)", "$(DBUS_ARCH_INCDIR)", "vendor/compat-5.2/c-api/" }
		}
	}
}
