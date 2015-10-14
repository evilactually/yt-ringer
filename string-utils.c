
#include "string-utils.h"
#include "string-builder.h"
#include <stdarg.h>
#include <glib.h>

gint u_str_match(const gchar* str1, const gchar* str2) {
  return g_strcmp0(str1, str2) == 0;
}

#include <stdio.h>

/* Helper data structure for string construction in u_str_substitute_vars function */

struct string_build_buffers {
  UStringBuilder *finished_buffers;
  const gchar*    current_buffer;
  gsize           current_buffer_size;
};

typedef struct string_build_buffers string_build_buffers;

void init_build_buffers(string_build_buffers *b) {
  b->finished_buffers = u_string_builder_new();
  b->current_buffer = NULL;
  b->current_buffer_size = 0;
}

void dispose_build_buffers(string_build_buffers *b) {
  g_object_unref(b->finished_buffers);
}

void buffer_start(string_build_buffers *b, const gchar* str) {
  g_assert(b->current_buffer == NULL); // make sure no one starts a buffer twice
  b->current_buffer_size = 0;          // (without getting yelled at)
  b->current_buffer = str;
}

void buffer_advance(string_build_buffers *b) {
  b->current_buffer_size++;
}

void buffer_advance_n(string_build_buffers *b, gint n) {
  b->current_buffer_size += n;
}

void buffer_finish(string_build_buffers *b) {
  u_string_builder_append_n(b->finished_buffers, b->current_buffer, b->current_buffer_size);
  b->current_buffer = NULL;
  b->current_buffer_size = 0;
}

const gchar* merge_build_buffers(string_build_buffers *b) {
  return u_string_builder_build(b->finished_buffers);
}

/* end */

#ifdef STRING_UTILS_TESTS

#include <stdio.h>

int main(int argc, char const *argv[]) {
  //u_str_substitute_vars("'Twas '${B} and the slithy ${T}", "B", "brillig", "T", "toves", NULL);
  //u_str_substitute_vars("'Twas '${B} and the slithy ${T}", NULL);

  return 0;
}

#endif
