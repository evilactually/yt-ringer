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
#define R_TYPE_CHANNEL y_channel_get_type ()
G_DECLARE_FINAL_TYPE (YChannel, y_channel, Y, CHANNEL, GObject)

/*
 * Method definitions.
 */
YChannel *y_channel_new (void);

void y_channel_errors(YChannel *self, GError *error);

void y_channel_authenticate (YChannel *self, const char* api_key);

void y_channel_open (YChannel *self, const char* channel_id);

void y_channel_open_by_user (YChannel *self, const char* user_id);

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

//YPlaylistSequence *y_channel_playlists (RYoutube *self); // [RPlaylist]

//YCommentSequence *y_channel_comments (RYoutube *self);

//YSubscriptionsSequence *y_channel_subscriptions (RYoutube *self);

G_END_DECLS

#endif /* __Y_YOTUBE_H__ */
