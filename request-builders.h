
#include "string-utils.h"

const char* YT_BASE_URL = "https://www.googleapis.com/youtube/v3";

gchar* yt_channel_basic_info_request_by_user(const gchar* key, const gchar* username) {
  return u_str_substitute_vars("${BASE}/channels?part=contentDetails&forUsername=${USERNAME}&key=${KEY}",
  "BASE", YT_BASE_URL,
  "USERNAME", username,
  "KEY", key, NULL);
}

gchar* yt_channel_basic_info_request_by_id(const gchar* key, const gchar* id) {
  return u_str_substitute_vars("${BASE}/channels?part=contentDetails&id=${ID}&key=${KEY}",
  "BASE", YT_BASE_URL,
  "ID", id,
  "KEY", key, NULL);
}

//"https://www.googleapis.com/youtube/v3/channels?part=contentDetails&forUsername=${USERNAME}&key=${KEY}"
