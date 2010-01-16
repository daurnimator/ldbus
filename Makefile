LUA_LIBDIR = /usr/local/lib/lua/5.1

CFLAGS += -Wall -Wextra --pedantic --std=c99
CFLAGS += `pkg-config --cflags lua5.1 dbus-1`
CFLAGS += -fPIC

OBJS = ldbus.o connection.o bus.o message.o message_iter.o

all:	ldbus.so

ldbus.so: $(OBJS)
	$(CC) -o ldbus.so -shared $(CFLAGS) $^ $(LIBS)

clean:
	rm ldbus.so $(OBJS)

install: ldbus.so
	mkdir -p $(LUA_LIBDIR)
	cp ldbus.so $(LUA_LIBDIR)
