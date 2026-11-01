#ifndef MONBEBE_BRIDGE_H
#define MONBEBE_BRIDGE_H

#include <webkit/webkit.h>

void bridge_init(WebKitWebView *web_view);

void bridge_send(WebKitWebView *web_view, const gchar *json);

#endif
