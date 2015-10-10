#include <gio/gio.h>
#include <stdio.h>

activate (GApplication *app,
          gpointer      user_data)
{
  // most app logic is here
  GNotification *notification;
  GFile *file;
  GIcon *icon;

  notification = g_notification_new ("A Notification");
  g_notification_set_body (notification, "Something happened");
  file = g_file_new_for_path ("/usr/lib64/shotwell/plugins/builtin/youtube.png");
  icon = g_file_icon_new (file);
  g_notification_set_icon (notification, G_ICON (icon));
  g_object_unref (icon);
  g_object_unref (file);

  g_application_send_notification (app, "new-video", notification);
  g_object_unref (notification);
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
