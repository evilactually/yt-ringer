#include "string-builder.h"
#include <string.h> //strlen

struct _UStringBuilder
{
  GObject parent_instance;
  GQueue* queue;
  gsize length;
};

struct string_chunk {
  gchar* string;
  gsize length;
};

typedef struct string_chunk string_chunk;

G_DEFINE_TYPE (UStringBuilder, u_string_builder, G_TYPE_OBJECT)

void free_chunk(string_chunk *data, gchar **user_data) {
  g_free(data->string);
  g_free(data);
}
#include <stdio.h>
void u_string_builder_finilize (GObject* gobject){
  UStringBuilder* instance = U_STRING_BUILDER(gobject);
  g_queue_foreach (instance->queue, (GFunc)free_chunk, NULL);
  g_queue_free(instance->queue); // TODO: release the strings
  G_OBJECT_CLASS (u_string_builder_parent_class)->finalize (gobject);
}

static void u_string_builder_class_init (UStringBuilderClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = u_string_builder_finilize;
}

static void u_string_builder_init (UStringBuilder *self)
{
  self->queue = g_queue_new();
}

UStringBuilder *u_string_builder_new (void) {
  return g_object_new(U_TYPE_STRING_BUILDER, NULL);
}

void u_string_builder_append_n (UStringBuilder *self, const gchar* str, gsize n) {
  if(!str) return;
  string_chunk *chunk = g_new(string_chunk, 1);
  chunk->string = g_strndup (str, n);
  chunk->length = n;
  g_queue_push_tail(self->queue, chunk);
  self->length += n;
}

void u_string_builder_append (UStringBuilder *self, const gchar* str) {
   if(!str) return;
   gsize n = strlen(str);
   if (!n) return;
   string_chunk *chunk = g_new(string_chunk, 1);
   chunk->string = g_memdup(str, n*sizeof(gchar));
   chunk->length = n;
   g_queue_push_tail(self->queue, chunk);
   self->length += n;
 }

void u_string_builder_prepend_n (UStringBuilder *self, const gchar* str, gsize n) {
  if(!str) return;
  string_chunk *chunk = g_new(string_chunk, 1);
  chunk->string = g_strndup (str, n);
  chunk->length = n;
  g_queue_push_head(self->queue, chunk);
  self->length += n;
}

void u_string_builder_prepend (UStringBuilder *self, const gchar* str) {
  if(!str) return;
  gsize n = strlen(str);
  if (!n) return;
  string_chunk *chunk = g_new(string_chunk, 1);
  chunk->string = g_memdup(str, n*sizeof(gchar));
  chunk->length = n;
  g_queue_push_head(self->queue, chunk);
  self->length += n;
}

gint u_string_builder_length (UStringBuilder *self) {
  return self->length;
}

void chunk_process(string_chunk *data, gchar **user_data) {
  memcpy(*user_data, data->string, data->length);
  *user_data += data->length;
}

gchar* u_string_builder_build (UStringBuilder *self) {
  gchar *result, *cursor;
  result = cursor = (gchar*)g_malloc(sizeof(gchar)*(self->length + 1)); // extra char for \0
  g_queue_foreach (self->queue, (GFunc)chunk_process, &cursor);
  result[self->length] = '\0'; // null terminate
  return result;
}

#ifdef STRING_BUILDER_TESTS

#include "string-utils.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  UStringBuilder* sb = u_string_builder_new();
  u_string_builder_append(sb,"<header string>");
  u_string_builder_append(sb,"<body string>");
  u_string_builder_append(sb,"<footer string>");
  u_string_builder_prepend(sb,"<super header string>");
  gchar* str = u_string_builder_build(sb);
  g_assert(u_str_match(str, "<super header string><header string><body string><footer string>"));
  printf("%s\n", str);
  g_object_unref(sb);
  g_free(str);

  // no calls to append/prepend, builds empty string
  sb = u_string_builder_new();
  str = u_string_builder_build(sb);
  g_assert_cmpstr(str, ==, "");
  g_assert(str[0] == '\0');
  g_object_unref(sb);
  g_free(str);

  // nulls do nothing
  sb = u_string_builder_new();
  u_string_builder_append(sb,NULL);
  u_string_builder_append_n(sb,NULL,10);
  str = u_string_builder_build(sb);
  g_assert_cmpstr(str, ==, "");
  g_object_unref(sb);
  g_free(str);

  // empty strings so nothing
  sb = u_string_builder_new();
  u_string_builder_append(sb,"");
  u_string_builder_append(sb,"");
  u_string_builder_append(sb,"");
  g_assert(u_string_builder_length(sb) == 0);
  str = u_string_builder_build(sb);
  g_assert_cmpstr(str,==,"");
  g_object_unref(sb);
  g_free(str);

  // null terminatation
  sb = u_string_builder_new();
  u_string_builder_append(sb,"word");
  str = u_string_builder_build(sb);
  printf("%s\n", str);
  g_assert(str[4] == '\0');
  g_object_unref(sb);
  g_free(str);

  return 0;
}

#endif
