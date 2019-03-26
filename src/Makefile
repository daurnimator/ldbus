LUA_LIBDIR = /usr/local/lib/lua/5.3
PKG_CONFIG = pkg-config
LUA_PKGNAME = lua5.3

CFLAGS += -Wall -Wextra --pedantic -Wno-long-long
CFLAGS += `$(PKG_CONFIG) --cflags $(LUA_PKGNAME) dbus-1`
CFLAGS += -fPIC
CFLAGS += -I ../vendor/compat-5.3/c-api

LIBS = `$(PKG_CONFIG) --libs dbus-1`

OBJS = ldbus.o error.o pending_call.o connection.o bus.o message.o message_iter.o timeout.o watch.o ../vendor/compat-5.3/c-api/compat-5.3.o

all:	ldbus.so

ldbus.so: $(OBJS)
	$(CC) -o ldbus.so -shared $(CFLAGS) $(LDFLAGS) $^ $(LIBS)

clean:
	rm -f -- ldbus.so $(OBJS)

install: ldbus.so
	mkdir -p $(LUA_LIBDIR)
	cp ldbus.so $(LUA_LIBDIR)
