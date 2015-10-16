#ifndef __U_CURL_UTILS_H__
#define __U_CURL_UTILS_H__

#include <curl/curl.h>
#include <glib.h>

gchar* u_curl_simple_get(const gchar* url,
                         CURLcode* code);

//GHashTable* u_curl_cache_new();

// gchar* u_curl_simple_get_cached(GHashTable* cache,
//                                 const gchar* url,
//                                 CURLcode* code);

#endif
