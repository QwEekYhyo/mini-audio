#include <gio/gio.h>
#include <stdio.h>

#define BLUEZ         "org.bluez"
#define ADAPTER_PATH  "/org/bluez/hci0"
#define ADAPTER_IFACE "org.bluez.Adapter1"
#define DEVICE_IFACE  "org.bluez.Device1"
#define PROPS_IFACE   "org.freedesktop.DBus.Properties"
#define OM_IFACE      "org.freedesktop.DBus.ObjectManager"

static gboolean wait_for_powered(GDBusConnection *bus,
                                 const gchar     *adapter_path,
                                 guint            timeout_ms)
{
    const gint64 deadline = g_get_monotonic_time() + (gint64)timeout_ms * 1000;

    while (g_get_monotonic_time() < deadline)
    {
        gint64 remaining_us = deadline - g_get_monotonic_time();

        gint remaining_ms = (gint)((remaining_us + 999) / 1000);

        GError *error = NULL;

        GVariant *reply = g_dbus_connection_call_sync(
            bus,
            "org.bluez",
            adapter_path,
            "org.freedesktop.DBus.Properties",
            "Get",
            g_variant_new("(ss)", "org.bluez.Adapter1", "Powered"),
            G_VARIANT_TYPE("(v)"),
            G_DBUS_CALL_FLAGS_NONE,
            remaining_ms,
            NULL,
            &error
        );

        if (reply)
        {
            GVariant *value = NULL;
            GVariant *inner = NULL;

            g_variant_get(reply, "(@v)", &value);
            inner = g_variant_get_variant(value);

            gboolean powered = g_variant_get_boolean(inner);

            g_variant_unref(inner);
            g_variant_unref(value);
            g_variant_unref(reply);

            if (powered)
            {
                g_print("Bluetooth adapter is powered on.\n");
                return TRUE;
            }
        }
        else
        {
            g_clear_error(&error);
        }

        remaining_us = deadline - g_get_monotonic_time();

        if (remaining_us <= 0)
            break;

        g_usleep(MIN((gint64)200000, remaining_us));
    }

    g_printerr("Timed out waiting for Bluetooth adapter.\n");
    return FALSE;
}

int main(void)
{
    GError          *error   = NULL;
    GDBusConnection *bus     = NULL;
    GVariant        *reply   = NULL;
    GVariant        *objects = NULL;
    GVariant        *ifaces  = NULL;
    GVariant        *props   = NULL;
    GVariant        *value   = NULL;
    const gchar     *path;
    const gchar     *name;
    const gchar     *address;
    const gchar     *first_device = NULL;
    GVariantIter     iter;
    gboolean         powered = FALSE;
    gboolean         paired;
    gboolean         connected;
    gboolean         found  = FALSE;
    int              status = 1;

    bus = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (!bus)
        goto fail;

    // Read the adapter power state
    reply = g_dbus_connection_call_sync(
        bus,
        BLUEZ,
        ADAPTER_PATH,
        PROPS_IFACE,
        "Get",
        g_variant_new("(ss)", ADAPTER_IFACE, "Powered"),
        G_VARIANT_TYPE("(v)"),
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error
    );

    if (!reply)
        goto fail;

    g_variant_get(reply, "(@v)", &value);
    GVariant *inner = g_variant_get_variant(value);
    powered         = g_variant_get_boolean(inner);
    g_variant_unref(inner);
    g_variant_unref(value);
    g_variant_unref(reply);
    reply = NULL;
    value = NULL;

    // Turn Bluetooth on if it is off
    if (!powered)
    {
        g_print("Bluetooth is off; turning it on...\n");

        reply = g_dbus_connection_call_sync(
            bus,
            BLUEZ,
            ADAPTER_PATH,
            PROPS_IFACE,
            "Set",
            g_variant_new("(ssv)",
                          ADAPTER_IFACE,
                          "Powered",
                          g_variant_new_boolean(TRUE)),
            G_VARIANT_TYPE("()"),
            G_DBUS_CALL_FLAGS_NONE,
            -1,
            NULL,
            &error
        );

        if (!reply)
            goto fail;

        g_variant_unref(reply);
        reply = NULL;
        g_print("Power-on request succeeded.\n");

        if (!wait_for_powered(bus, ADAPTER_PATH, 20000))
        {
            goto out;
        }
    }
    else
    {
        g_print("Bluetooth is already on.\n");
    }

    // Enumerate Bluez objects and their properties.
    reply = g_dbus_connection_call_sync(bus,
                                        BLUEZ,
                                        "/",
                                        OM_IFACE,
                                        "GetManagedObjects",
                                        NULL,
                                        G_VARIANT_TYPE("(a{oa{sa{sv}}})"),
                                        G_DBUS_CALL_FLAGS_NONE,
                                        -1,
                                        NULL,
                                        &error);

    if (!reply)
        goto fail;

    g_variant_get(reply, "(@a{oa{sa{sv}}})", &objects);
    g_variant_unref(reply);
    reply = NULL;

    // List all paired devices and remember the first one
    g_variant_iter_init(&iter, objects);

    while (g_variant_iter_next(&iter, "{&o@a{sa{sv}}}", &path, &ifaces))
    {
        props = g_variant_lookup_value(ifaces,
                                       DEVICE_IFACE,
                                       G_VARIANT_TYPE("a{sv}"));

        if (!props)
        {
            g_variant_unref(ifaces);
            ifaces = NULL;
            continue;
        }

        paired    = FALSE;
        connected = FALSE;

        g_variant_lookup(props, "Paired", "b", &paired);

        if (paired)
        {
            name    = "(unknown)";
            address = "(unknown)";

            g_variant_lookup(props, "Name", "&s", &name);
            g_variant_lookup(props, "Address", "&s", &address);
            g_variant_lookup(props, "Connected", "b", &connected);

            g_print("Paired device: %s (%s) [%s]\n",
                    name,
                    address,
                    connected ? "connected" : "disconnected");

            found = TRUE;

            if (!first_device && !connected)
                first_device = g_strdup(path);
        }

        g_variant_unref(props);
        props = NULL;
        g_variant_unref(ifaces);
        ifaces = NULL;
    }

    if (!found)
    {
        g_print("No paired devices found.\n");
        status = 0;
        goto out;
    }

    // Connect to the first paired device that is not already connected
    if (!first_device)
    {
        g_print("All paired devices are already connected.\n");
        status = 0;
        goto out;
    }

    g_print("Connecting to %s...\n", first_device);

    reply = g_dbus_connection_call_sync(bus,
                                        BLUEZ,
                                        first_device,
                                        DEVICE_IFACE,
                                        "Connect",
                                        NULL,
                                        G_VARIANT_TYPE("()"),
                                        G_DBUS_CALL_FLAGS_NONE,
                                        -1,
                                        NULL,
                                        &error);

    if (!reply)
        goto fail;

    g_print("Connect request succeeded.\n");
    status = 0;
    goto out;

fail:
    g_printerr("D-Bus error: %s\n", error ? error->message : "Unknown error");

out:
    if (error)
        g_error_free(error);
    if (props)
        g_variant_unref(props);
    if (ifaces)
        g_variant_unref(ifaces);
    if (objects)
        g_variant_unref(objects);
    if (reply)
        g_variant_unref(reply);
    if (bus)
        g_object_unref(bus);
    g_free((gpointer)first_device);

    return status;
}
