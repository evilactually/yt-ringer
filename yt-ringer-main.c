#include <gio/gio.h>
#include <stdio.h>
#include <json-glib/json-glib.h>
#include <curl/curl.h>
#include <stdio.h>

//#include "channel.h"

char* buffer[512];

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
  printf("%s\n", buffer);
}

activate (GApplication *app,
          gpointer      user_data)
{

  CURL *curl = curl_easy_init();
  if(curl) {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "www.google.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    gchar msg[512];
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER , msg);
    printf("%s\n", msg);
    res = curl_easy_perform(curl);
    printf("%s\n", "****************************");
    printf("%d\n", res);
    curl_easy_cleanup(curl);
  }
  //
  // Youtube
  //
  // assert
  // open
  //
  // statistics
  // uploads
  // likes -> videos
  // playlists -> videos

//  get_uploads_playlist(channel_name);

  // most app logic is here
  /* keep a loop
     check if new video is available from any of the channels
     use icon from channel, video title for notification title,
     spawn notification

     read videos by date: remember date of last check, read videos until finding an old video



  */



  GNotification *notification;
  GFile *file;
  GIcon *icon;
  while(1){
  sleep(1);
  notification = g_notification_new ("ExVSK");
  g_notification_set_body (notification, "Halo 3 ODST - Coop avec Kiki #1 [HD]");
  file = g_file_new_for_path ("/tmp/photo.jpg");
  icon = g_file_icon_new (file);
  g_notification_set_icon (notification, G_ICON (icon));
  g_object_unref (icon);
  g_object_unref (file);

  g_application_send_notification (app, "new-video", notification);
  g_object_unref (notification);
  }
}

int
main (int argc,
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
