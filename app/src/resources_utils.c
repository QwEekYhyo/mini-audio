#include <resources_utils.h>

const char *get_mime_type(const char *path)
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

void uri_scheme_request_cb(WebKitURISchemeRequest *request, gpointer user_data)
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
