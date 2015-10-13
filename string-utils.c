
#include "string-utils.h"
#include <stdarg.h>

gint u_str_match(const gchar* str1, const gchar* str2) {
  return g_strcmp0(str1, str2) == 0;
}

gint u_str_substitute_vars(const gchar* template, ...) {
//  va_start(ap,template);

//  va_end(ap);
}
