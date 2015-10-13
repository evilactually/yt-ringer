/*
 * Copyright/Licensing information.
 */

/* inclusion guard */
#ifndef __U_STRING_BUILDER_H__
#define __U_STRING_BUILDER_H__

#include <glib-object.h>

G_BEGIN_DECLS

/*
 * Type declaration.
 */
G_DECLARE_FINAL_TYPE (UStringBuilder, u_string_builder, U, STRING_BUILDER, GObject)

/*
 * Convenience Macros
 */
#define U_TYPE_STRING_BUILDER                               (u_string_builder_get_type ())
#define U_STRING_BUILDER(inst)                              (G_TYPE_CHECK_INSTANCE_CAST ((inst),                     \
                                                                U_TYPE_STRING_BUILDER, UStringBuilder))
/*
 * Method definitions.
 */
UStringBuilder *u_string_builder_new (void);

void u_string_builder_append_n (UStringBuilder *self, const gchar* str, gsize n);

void u_string_builder_append (UStringBuilder *self, const gchar* str);

void u_string_builder_prepend_n (UStringBuilder *self, const gchar* str, gsize n);

void u_string_builder_prepend (UStringBuilder *self, const gchar* str);

gint u_string_builder_length (UStringBuilder *self);

const gchar* u_string_builder_build (UStringBuilder *self);

G_END_DECLS

#endif /* __U_STRING_BUILDER_H__ */
