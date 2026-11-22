#include <bluetooth.h>
#include <bridge.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <resources_utils.h>
#include <webkit/webkit.h>

static void signal_front_of_connection_status(gpointer user_data)
{
    Device *connected_device;
    gchar  *json;

    connected_device = get_connected_device();

    if (connected_device)
    {
        json = g_strdup_printf("{\"event\":\"connection_status\", "
                               "\"payload\":{\"status\":\"connected\","
                               "\"device\":{\"name\":\"%s\"}}}",
                               connected_device->name);

        bridge_send(WEBKIT_WEB_VIEW(user_data), json);

        g_free(json);
    }
}

static gboolean periodic_javascript_invocation(gpointer user_data)
{
    const gchar message_detail[] = "{\"event\": \"alert\"}";

    bridge_send(WEBKIT_WEB_VIEW(user_data), message_detail);

    return G_SOURCE_CONTINUE;
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWindow             *window;
    WebKitWebView         *web_view;
    WebKitWebContext      *context;
    WebKitSecurityManager *security;
    WebKitSettings        *settings;

    window   = GTK_WINDOW(gtk_application_window_new(app));
    settings = webkit_settings_new();
    context  = webkit_web_context_new();
    security = webkit_web_context_get_security_manager(context);
    web_view = WEBKIT_WEB_VIEW(
        g_object_new(WEBKIT_TYPE_WEB_VIEW, "web-context", context, NULL)
    );

    gtk_window_set_title(window, "Ma Bébé Audio");
    gtk_window_set_default_size(window, 1280, 720);

    bridge_init(web_view);
    bluetooth_init();

    // Delay here is completely arbitrary and this is bad
    g_timeout_add_once(3000,
                       signal_front_of_connection_status,
                       (gpointer)web_view);
    g_timeout_add(2500, periodic_javascript_invocation, (gpointer)web_view);

#if defined(DEBUG)
    webkit_settings_set_enable_developer_extras(settings, TRUE);

    webkit_web_view_set_settings(web_view, settings);

    // Use Vite debug server in debug config
    webkit_web_view_load_uri(web_view, "http://localhost:5173");
#else
    webkit_web_context_register_uri_scheme(context,
                                           "app",
                                           uri_scheme_request_cb,
                                           NULL,
                                           NULL);

    webkit_security_manager_register_uri_scheme_as_secure(security, "app");

    webkit_security_manager_register_uri_scheme_as_cors_enabled(security,
                                                                "app");

    webkit_web_view_load_uri(web_view, "app://localhost/index.html");
#endif

    gtk_window_set_child(window, GTK_WIDGET(web_view));

    gtk_window_present(window);

    g_object_unref(context);
    g_object_unref(settings);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int             status;

    app = gtk_application_new("com.mabebe.audio", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    bluetooth_deinit();

    g_object_unref(app);

    return status;
}
