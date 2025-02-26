#ifndef LDBUS_MESSAGE_ITER_H
#define LDBUS_MESSAGE_ITER_H

#include <lua.h>

#include "ldbus.h"

typedef struct {
    /* This must be the first member, thus for binding functions that doesn't
       care about the underlying DBusMessage, this structure could be safely
       referred with a DBusMessageIter pointer.
     */
    DBusMessageIter iter;
    /* The message which this iterator refers to. NULL if this iterator hasn't
       been initialized with a message.
       We need to unref it when the iterator is garbagecollected.
     */
    DBusMessage *message;
} lDBusMessageIter;

#define DBUS_MESSAGE_ITER_METATABLE "ldbus_DBusMessageIter"

LDBUS_INTERNAL int push_DBusMessageIter(lua_State *L);
LDBUS_INTERNAL void load_dbus_message_iter(lua_State *L);
LDBUS_INTERNAL void unref_ldbus_message_iter(lua_State *L, int index);

#endif
