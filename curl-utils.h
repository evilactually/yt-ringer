#ifndef __U_CURL_UTILS_H__
#define __U_CURL_UTILS_H__

#include <curl/curl.h>

const char* u_curl_simple_get(const char* url,
                              CURLcode* code);

#endif
