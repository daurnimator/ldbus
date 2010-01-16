CFLAGS += -Wall -Wextra --pedantic --std=c99
CFLAGS += `pkg-config --cflags lua5.1 dbus-1`
CFLAGS += -fPIC

LIBS += `pkg-config --libs dbus-1 lua5.1`

OBJS = ldbus.o connection.o bus.o message.o message_iter.o

all:	ldbus.so

ldbus.so:	$(OBJS)
		$(CC) -o ldbus.so -shared $(CFLAGS) $^ $(LIBS)

clean:
	rm ldbus.so $(OBJS)