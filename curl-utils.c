
#include "curl-utils.h"
#include "string-builder.h"

/* Curl write callback */
static size_t curl_write_data(void *buffer,
                              size_t size,
                              size_t nmemb,
                              void *userp) {
  u_string_builder_append_n(userp, buffer, size*nmemb);
  return size*nmemb;
}

/* Wrapper for simple curl get request */
gchar* u_curl_simple_get(const gchar* url,
                               CURLcode* code) {
  CURL *curl = curl_easy_init();
  CURLcode _code;
  UStringBuilder* response_chunked = u_string_builder_new();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_chunked);
  _code = curl_easy_perform(curl);
  if(code) {
    *code = _code;
  }
  curl_easy_cleanup(curl);
  gchar* response = u_string_builder_build(response_chunked);
  g_object_unref(response_chunked);
  return response;
}

#ifdef CURL_UTILS_TESTS

int main(int argc, char const *argv[]) {
  printf("%s\n", u_curl_simple_get("www.google.com", NULL));
  return 0;
}

#endif
