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
	 "lua >= 5.1"
}
external_dependencies = {
	DBUS = {
		header = "dbus/dbus.h";
		library = "dbus-1";
	};
	DBUS_ARCH = { header = "dbus/dbus-arch-deps.h" }
}
build = {
	type = "builtin",
	modules = {
		ldbus = {
			sources = {
				"src/ldbus.c",
				"src/error.c",
				"src/bus.c",
				"src/connection.c",
				"src/message.c",
				"src/message_iter.c",
				"src/pending_call.c",
				"src/timeout.c",
				"src/watch.c",
				"vendor/compat-5.3/c-api/compat-5.3.c"
			},
			libraries = { "dbus-1" },
			incdirs = { "$(DBUS_INCDIR)", "$(DBUS_ARCH_INCDIR)", "vendor/compat-5.3/c-api/" }
		};
		["ldbus.message.variant"] = "src/message/variant.lua";
	}
}
