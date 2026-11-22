#ifndef MABEBE_RESOURCES_UTILS_H
#define MABEBE_RESOURCES_UTILS_H

#include <glib.h>
#include <webkit/webkit.h>

#define RESOURCE_PREFIX "/com/mabebe/audio"

const char *get_mime_type(const char *path);

void uri_scheme_request_cb(WebKitURISchemeRequest *request, gpointer user_data);

#endif
