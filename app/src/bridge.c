#include <bridge.h>
#include <gtk/gtk.h>
#include <jsc/jsc.h>
#include <webkit/WebKitUserContentManager.h>
#include <webkit/webkit.h>

static void on_javascript_message(WebKitUserContentManager *manager,
                                  JSCValue                 *message,
                                  gpointer                  user_data)
{
    gchar *json;

    json = jsc_value_to_json(message, 0);

    if (!json)
    {
        g_warning("Could not serialize JavaScript message");
        return;
    }

    g_print("Received message from JavaScript: %s\n", json);

    g_free(json);
}

void bridge_init(WebKitWebView *web_view)
{
    WebKitUserContentManager *manager;

    manager = webkit_web_view_get_user_content_manager(web_view);

    g_signal_connect(manager,
                     "script-message-received::native",
                     G_CALLBACK(on_javascript_message),
                     (gpointer)web_view);

    webkit_user_content_manager_register_script_message_handler(manager,
                                                                "native",
                                                                NULL);
}

void bridge_send(WebKitWebView *web_view, const gchar *json) {}
