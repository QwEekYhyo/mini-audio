#include <bluetooth.h>
#include <gio/gio.h>
#include <glib.h>
#include <glibconfig.h>

#define NB_DEVICE 10

static GDBusConnection *sys_bus_con = NULL;
static Device           devices[NB_DEVICE];

/**
 * Retrieve the list of devices from Bluez
 */
static void get_devices(GObject *src_obj, GAsyncResult *result, gpointer data)
{
    GDBusConnection *con;
    GError          *error;
    GVariant        *reply;
    GVariant        *objects;
    GVariantIter     iter;
    const gchar     *object_path;
    GVariant        *ifaces_and_props;
    gsize            device_index;
    Device          *connected_device;

    (void)data;

    con              = (GDBusConnection *)src_obj;
    error            = NULL;
    objects          = NULL;
    ifaces_and_props = NULL;
    device_index     = 0;

    // Reply is of type: (a{oa{sa{sv}}})
    // (out ARRAY of DICT_ENTRY<OBJPATH,ARRAY of DICT_ENTRY<STRING,ARRAY of
    // DICT_ENTRY<STRING,VARIANT>>> objpath_interfaces_and_properties)
    reply = g_dbus_connection_call_finish(con, result, &error);

    if (!reply)
    {
        g_printerr("D-Bus error: %s\n",
                   error ? error->message : "Unknown error");
        g_error_free(error);

        return;
    }

    // Get array out of the tuple
    objects = g_variant_get_child_value(reply, 0);
    g_variant_unref(reply);

    g_variant_iter_init(&iter, objects);
    while (g_variant_iter_next(&iter,
                               "{&o@a{sa{sv}}}",
                               &object_path,
                               &ifaces_and_props))
    {
        GVariant    *props;
        gboolean     prop_exists;
        gboolean     paired;
        const gchar *name;

        if (device_index >= NB_DEVICE)
        {
            g_warning("Maximum number of Bluetooth devices reached");
            g_variant_unref(ifaces_and_props);
            ifaces_and_props = NULL;
            break;
        }

        props = NULL;

        // Look for properties of objects with the Bluez Device1 interface
        // This gets a list of properties which are String -> Variant
        props = g_variant_lookup_value(ifaces_and_props,
                                       "org.bluez.Device1",
                                       G_VARIANT_TYPE("a{sv}"));

        // If the object doesn't have the Device1 interface, skip it
        if (!props)
        {
            // Only unreferencing ifaces_and_props because we Get it with '@'
            // which creates a new reference whereas '&' is a pointer that
            // shouldn't be freed
            g_variant_unref(ifaces_and_props);
            // Avoid use after free?
            ifaces_and_props = NULL;
            continue;
        }

        paired      = FALSE;
        prop_exists = g_variant_lookup(props, "Paired", "b", &paired);

        if (paired)
        {
            devices[device_index].paired = paired;

            prop_exists = g_variant_lookup(props,
                                           "Connected",
                                           "b",
                                           &devices[device_index].connected);

            prop_exists = g_variant_lookup(props, "Alias", "&s", &name);

            if (prop_exists)
            {
                devices[device_index].name = g_strdup(name);
            }

            devices[device_index].allocated = 1;

            device_index++;
        }

        g_variant_unref(props);
        g_variant_unref(ifaces_and_props);
        props            = NULL;
        ifaces_and_props = NULL;
    }

    g_variant_unref(objects);
}

int bluetooth_init(void)
{
    GError *error = NULL;

    // Doing synchronous calls for now, this could probably be optimized
    sys_bus_con = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);

    if (!sys_bus_con)
    {
        g_printerr("D-Bus error: %s\n",
                   error ? error->message : "Unknown error");
        g_error_free(error);

        return -1;
    }

    // TODO: watch out for a "deinit" call while this hasn't executed yet
    g_dbus_connection_call(sys_bus_con,
                           "org.bluez",
                           "/",
                           "org.freedesktop.DBus.ObjectManager",
                           "GetManagedObjects",
                           NULL,
                           G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
                           G_DBUS_CALL_FLAGS_NONE,
                           -1,
                           NULL,
                           get_devices,
                           NULL);

    return 0;
}

void bluetooth_deinit(void)
{
    if (sys_bus_con)
    {
        g_object_unref(sys_bus_con);
        sys_bus_con = NULL;
    }

    for (gsize device_i = 0; device_i < NB_DEVICE; device_i++)
    {
        if (!devices[device_i].allocated)
        {
            continue;
        }

        g_free(devices[device_i].name);
        devices[device_i].allocated = 0;
    }
}

Device *get_connected_device(void)
{
    Device *device;

    for (gsize device_i = 0; device_i < NB_DEVICE; device_i++)
    {
        device = &devices[device_i];

        if (device->allocated && device->connected && device->paired)
        {
            return device;
        }
    }

    return NULL;
}

void debug_devices(void)
{
    Device *device;

    for (gsize device_i = 0; device_i < NB_DEVICE; device_i++)
    {
        device = &devices[device_i];
        g_print("Device %lu {\n", device_i);
        g_print("  allocated: %d,\n", device->allocated);

        if (device->name)
            g_print("  name: %s,\n", device->name);

        g_print("  paired: %d,\n", device->paired);
        g_print("  connected: %d,\n", device->connected);
        g_print("}\n\n");
    }
}
