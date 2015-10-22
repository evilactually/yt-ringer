
#include "channel.h"
#include "curl-utils.h"
#include "string-utils.h"
#include "json-query.h"
#include "request-builders.h"
#include <json-glib/json-glib.h>
#include <string.h>

struct _YChannel
{
  GObject parent_instance;

  /* Private */

  gchar* api_key;
  gchar* channel_id;
  int cache_valid;
  GError *error;
};

G_DEFINE_TYPE (YChannel, y_channel, G_TYPE_OBJECT)

void y_channel_class_finilize(GObject* gobject){
  YChannel* instance = Y_CHANNEL(gobject);
  g_free(instance->api_key);
  g_free(instance->channel_id);
  G_OBJECT_CLASS (y_channel_parent_class)->finalize (gobject);
}

static void y_channel_class_init (YChannelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = y_channel_class_finilize;}

static void y_channel_init (YChannel *self)
{

}

YChannel *y_channel_new (void) {
  return g_object_new(R_TYPE_CHANNEL, NULL);
}

void y_channel_authenticate (YChannel *self,
                             const gchar* api_key) {
  if (self->api_key == NULL) {
      g_free(self->api_key);
  }
  self->api_key = g_strdup(api_key);
  self->cache_valid = FALSE;
}

//void y_channel_open (YChannel *self, const char* channel_id);

void y_channel_open_by_user (YChannel *self,
                             const char* user_id) {
  CURLcode code;
  gchar* request_string = yt_channel_basic_info_request_by_user(self->api_key, user_id);
  gchar* response = u_curl_simple_get(request_string, &code);
  JsonParser* parser = json_parser_new();
  GError* error;
  json_parser_load_from_data (parser,
                              response,
                              strlen(response),
                              &error);
  self->channel_id = g_strdup(json_query_string(json_parser_get_root(parser), "$.items[0].id"));
  g_object_unref(parser);
  g_free(request_string);
  g_free(response);
}

gint y_channel_uploads (YChannel *self, gint page_number, gint page_size, YVideo** videos) {
  CURLcode code;
  gchar* request_string = yt_channel_basic_info_request_by_id(self->api_key, self->channel_id);
  gchar* response = u_curl_simple_get(request_string, &code);
  JsonParser* parser = json_parser_new();
  GError* error;
  json_parser_load_from_data (parser,
                              response,
                              strlen(response),
                              &error);
  JsonNode* root = json_parser_get_root(parser); g_assert(root);
  JsonObject* root_object = json_node_get_object(root);
  JsonArray* items = json_object_get_array_member(root_object, "items");
  JsonObject* premier = json_array_get_object_element(items, 0);
  //printf("%s\n", json_object_get_object_member(premier, "contentDetails"));
  g_object_unref(parser);
  g_free(request_string);
  g_free(response);
}

#ifdef CHANNEL_TESTS

int main(int argc, char const *argv[]) {
  YChannel *channel = y_channel_new();
  g_assert(channel->api_key == NULL);
  g_assert(channel->channel_id == NULL);
  y_channel_authenticate(channel, "AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g");
  g_assert(u_str_match(channel->api_key, "AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g"));
  y_channel_open_by_user(channel, "exvsk");
  //y_channel_uploads(channel, 1, 5, NULL);

  g_object_unref(channel);

  //CURLcode c;
  //const char* response = u_curl_simple_get("https://www.googleapis.com/youtube/v3/channels?part=contentDetails&forUsername=exvsk&key=AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g", &c);
  //printf("%s ->%d<-\n", response, c);
  return 0;
}

#endif
