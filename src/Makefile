LUA_LIBDIR = /usr/local/lib/lua/5.1

CFLAGS += -Wall -Wextra --pedantic --std=c89
CFLAGS += `pkg-config --cflags lua5.1 dbus-1`
CFLAGS += -fPIC
CFLAGS += -I ../vendor/compat-5.2/c-api

LIBS = `pkg-config --libs dbus-1`

OBJS = ldbus.o error.o pending_call.o connection.o bus.o message.o message_iter.o watch.o

all:	ldbus.so

ldbus.so: $(OBJS)
	$(CC) -o ldbus.so -shared $(CFLAGS) $^ $(LIBS)

clean:
	rm -f -- ldbus.so $(OBJS)

install: ldbus.so
	mkdir -p $(LUA_LIBDIR)
	cp ldbus.so $(LUA_LIBDIR)
