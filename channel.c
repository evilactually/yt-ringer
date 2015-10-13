
#include "channel.h"
#include "curl-utils.h"

struct _YChannel
{
  GObject parent_instance;
  /* Private */
  gchar* api_key;
  gchar* channel_id;
  int cache_valid;
  CURL *curl;
};

G_DEFINE_TYPE (YChannel, y_channel, G_TYPE_OBJECT)

static void
y_channel_class_init (YChannelClass *klass)
{
  /* vtable initialization */
}

static void
y_channel_init (YChannel *self)
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
  //gchar* url_buffer = g_strjoin(NULL, )
  //curl_easy_setopt(self->curl, CURLOPT_URL, "https://www.googleapis.com/youtube/v3/channels?part=contentDetails&forUsername=exvsk&key=AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g");
  // curl_easy_setopt(self->curl, CURLOPT_WRITEFUNCTION, curl_write_data);
  // code = curl_easy_perform(self->curl);
}

#ifdef CHANNEL_TESTS

int main(int argc, char const *argv[]) {
  // YChannel *channel = y_channel_new();
  // g_assert(channel->api_key == NULL);
  // g_assert(channel->channel_id == NULL);
  // y_channel_authenticate(channel, "YCkE4DcI-HGgoWxpmfEcUnmHEC");
  // g_assert(s_str_match(channel->api_key, "YCkE4DcI-HGgoWxpmfEcUnmHEC"));

  CURLcode c;
  const char* response = u_curl_simple_get("https://www.googleapis.com/youtube/v3/channels?part=contentDetails&forUsername=exvsk&key=AIzaSyC6OAlPAuIa2YDC_w3xRrDMIX_M1fGF41g", &c);
  printf("%s ->%d<-\n", response, c);
  return 0;
}

#endif
