#include <gio/gio.h>
#include <stdio.h>
#include <json-glib/json-glib.h>
#include <curl/curl.h>
#include <stdio.h>

#include "request-builders.h"
#include "json-query.h"
#include "curl-utils.h"
#include <string.h>

gchar* channel_get_uploads_playlist_id (const gchar* user_id, const gchar* api_key, GError** error) {
  CURLcode code;
  gchar* request_string = yt_channel_basic_info_request_by_user(api_key, user_id);
  gchar* response = u_curl_simple_get(request_string, &code);
  JsonParser* parser = json_parser_new();
  json_parser_load_from_data (parser,
                              response,
                              strlen(response),
                              error);
  gchar* id = g_strdup(json_query_string(json_parser_get_root(parser), "$.items[0].contentDetails.relatedPlaylists.uploads"));
  g_object_unref(parser);
  g_free(request_string);
  g_free(response);
  return id;
}

JsonArray* playlist_get_videos (const gchar* api_key, const gchar* playlist_id, GError** error) {
  CURLcode code;
  gchar* request_string = yt_playlist_snippets(api_key, playlist_id, 5);
  gchar* response = u_curl_simple_get(request_string, &code);
  JsonParser* parser = json_parser_new();
  json_parser_load_from_data (parser,
                              response,
                              strlen(response),
                              error);
  printf("%s\n", response);
  JsonArray* array = json_query_array(json_parser_get_root(parser), "$.items");
  json_array_ref(array);
  g_object_unref(parser);
  g_free(request_string);
  g_free(response);
  return array;
}

void activate (GApplication *app,
               gpointer      user_data)
{
  GError* error;
  gchar* uploads_id = channel_get_uploads_playlist_id("exvsk", "AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g", &error);
  JsonArray* a = playlist_get_videos("AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g", uploads_id, &error);
  printf("%d\n", json_array_get_length(a));
  json_array_unref(a);
  g_free(uploads_id);

  /* Notification spawning */
  // GNotification *notification;
  // GFile *file;
  // GIcon *icon;
  // while(1){
  // sleep(1);
  // notification = g_notification_new ("ExVSK");
  // g_notification_set_body (notification, "Halo 3 ODST - Coop avec Kiki #1 [HD]");
  // file = g_file_new_for_path ("/tmp/photo.jpg");
  // icon = g_file_icon_new (file);
  // g_notification_set_icon (notification, G_ICON (icon));
  // g_object_unref (icon);
  // g_object_unref (file);
  //
  // g_application_send_notification (app, "new-video", notification);
  // g_object_unref (notification);
  // }
}

int main (int argc,
      char **argv)
{
  GApplication *app;
  int status;
  // read configs
  app = g_application_new ("org.gnome.Maps", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  // clean up
  return 0;
}
