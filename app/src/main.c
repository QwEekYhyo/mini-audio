#include <gtk/gtk.h>
#include <webkit/webkit.h>

#define RESOURCE_PREFIX "/com/monbebe/audio"

static const char *get_mime_type(const char *path)
{
    if (g_str_has_suffix(path, ".html"))
        return "text/html";

    if (g_str_has_suffix(path, ".js"))
        return "text/javascript";

    if (g_str_has_suffix(path, ".css"))
        return "text/css";

    if (g_str_has_suffix(path, ".svg"))
        return "image/svg+xml";

    if (g_str_has_suffix(path, ".png"))
        return "image/png";

    if (g_str_has_suffix(path, ".jpg") || g_str_has_suffix(path, ".jpeg"))
        return "image/jpeg";

    if (g_str_has_suffix(path, ".woff2"))
        return "font/woff2";

    if (g_str_has_suffix(path, ".woff"))
        return "font/woff";

    if (g_str_has_suffix(path, ".json"))
        return "application/json";

    return "application/octet-stream";
}

static void uri_scheme_request_cb(WebKitURISchemeRequest *request,
                                  gpointer                user_data)
{
    const char *path = webkit_uri_scheme_request_get_path(request);

    // Root URL -> React entry point
    if (path == NULL || path[0] == '\0' || g_str_equal(path, "/"))
    {
        path = "/index.html";
    }

    gchar *resource_path = g_strconcat(RESOURCE_PREFIX, path, NULL);

    GError *error = NULL;
    GBytes *bytes = g_resources_lookup_data(resource_path,
                                            G_RESOURCE_LOOKUP_FLAGS_NONE,
                                            &error);

    g_free(resource_path);

    if (bytes == NULL)
    {
        g_warning("Resource not found: %s", error->message);

        webkit_uri_scheme_request_finish_error(request, error);

        g_error_free(error);
        return;
    }

    gsize         length;
    gconstpointer data = g_bytes_get_data(bytes, &length);

    GInputStream *stream = g_memory_input_stream_new_from_bytes(bytes);

    webkit_uri_scheme_request_finish(request,
                                     stream,
                                     length,
                                     get_mime_type(path));

    g_object_unref(stream);
    g_bytes_unref(bytes);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget             *window;
    GtkWidget             *webview;
    WebKitWebContext      *context;
    WebKitSecurityManager *security;
    WebKitSettings        *settings;

    window   = gtk_application_window_new(app);
    context  = webkit_web_context_new();
    security = webkit_web_context_get_security_manager(context);
    settings = webkit_settings_new();
    webview  = GTK_WIDGET(
        g_object_new(WEBKIT_TYPE_WEB_VIEW, "web-context", context, NULL)
    );

    gtk_window_set_title(GTK_WINDOW(window), "Mon Bébé Audio");
    gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);

    webkit_web_context_register_uri_scheme(context,
                                           "app",
                                           uri_scheme_request_cb,
                                           NULL,
                                           NULL);

    webkit_security_manager_register_uri_scheme_as_secure(security, "app");

    webkit_security_manager_register_uri_scheme_as_cors_enabled(security,
                                                                "app");

    webkit_settings_set_enable_developer_extras(settings, TRUE);

    webkit_web_view_set_settings(WEBKIT_WEB_VIEW(webview), settings);

    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview),
                             "app://localhost/index.html");

    gtk_window_set_child(GTK_WINDOW(window), webview);

    gtk_window_present(GTK_WINDOW(window));

    g_object_unref(context);
    g_object_unref(settings);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int             status;

    app = gtk_application_new("com.monbebe.audio", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return 0;
}
