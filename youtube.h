/*
 * Copyright/Licensing information.
 */

/* inclusion guard */
#ifndef __Y_CHANNEL_H__
#define __Y_CHANNEL_H__

#include <glib-object.h>
/*
 * Potentially, include other headers on which this header depends.
 */

G_BEGIN_DECLS

/*
 * Type declaration.
 */
#define R_TYPE_CHANNEL r_youtube_get_type ()
G_DECLARE_FINAL_TYPE (YChannel, y_channel, Y, CHANNEL, GObject)

/*
 * Method definitions.
 */
YChannel *r_youtube_new (void);

void y_channel_authenticate (YChannel *self, const char* api_key);

void y_channel_open (YChannel *self, const char* channel_name);

//RYoutubeVideo *y_channel_uploads (RYoutube *self);

//RYoutubeVideo *y_channel_likes (RYoutube *self);

//RYoutubeVideo *y_channel_favorites (RYoutube *self);

//RYoutubeVideo *y_channel_uploads (RYoutube *self);

//RYoutubeVideo *y_channel_watch_history (RYoutube *self);

//RYoutubeVideo *y_channel_watch_later (RYoutube *self);

//RYoutubeVideo *y_channel_title (RYoutube *self);

//RYoutubeVideo *y_channel_description (RYoutube *self);

//int *y_channel_statistics_view_count (RYoutube *self);

//int *y_channel_statistics_comment_count (RYoutube *self);

//int *y_channel_statistics_subscriber_count (RYoutube *self);

//int *y_channel_statistics_video_count (RYoutube *self);

//GList *y_channel_thumbnails (RYoutube *self); [RImage] Image width height bytes(raw)

//YSequence *y_channel_playlists (RYoutube *self); // [RPlaylist]

//YSequence *y_channel_comments (RYoutube *self);

G_END_DECLS

#endif /* __Y_YOTUBE_H__ */
