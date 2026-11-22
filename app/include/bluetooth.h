#ifndef MABEBE_BLUETOOTH_H
#define MABEBE_BLUETOOTH_H

#include <glib.h>

typedef struct
{
    gchar   *name;
    gboolean paired;
    gboolean connected;
    // Will probably need object path too
    gboolean allocated;
} Device;

// TODO: make an error type or something

int  bluetooth_init(void);
void bluetooth_deinit(void);

void debug_devices(void);

#endif
