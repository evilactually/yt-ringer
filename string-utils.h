#ifndef __U_STRING_UTILS_H__
#define __U_STRING_UTILS_H__

#include <glib.h>

gint u_str_match(const gchar* str1, const gchar* str2);

const gchar* u_str_substitute_vars(const gchar* template, ...);

#endif
