#include "string-builder.h"
#include <stdio.h>
#include <string.h> //strlen

struct _UStringBuilder
{
  GObject parent_instance;
  GQueue* queue;
  gsize length;
};

struct string_chunk {
  const gchar* string;
  gsize length;
};

typedef struct string_chunk string_chunk;

G_DEFINE_TYPE (UStringBuilder, u_string_builder, G_TYPE_OBJECT)

void u_string_builder_finilize (GObject* gobject){
  UStringBuilder* instance = U_STRING_BUILDER(gobject);
  g_queue_free(instance->queue);
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
  string_chunk *chunk = g_new(string_chunk, 1);
  chunk->string = g_strndup (str, n);
  chunk->length = n;
  g_queue_push_tail(self->queue, chunk);
  self->length += n;
}

void u_string_builder_append (UStringBuilder *self, const gchar* str) {
   string_chunk *chunk = g_new(string_chunk, 1);
   gsize n = strlen(str);
   chunk->string = g_memdup(str, n*sizeof(gchar));
   chunk->length = n;
   g_queue_push_tail(self->queue, chunk);
   self->length += n;
 }

void u_string_builder_prepend_n (UStringBuilder *self, const gchar* str, gsize n) {
  string_chunk *chunk = g_new(string_chunk, 1);
  chunk->string = g_strndup (str, n);
  chunk->length = n;
  g_queue_push_head(self->queue, chunk);
  self->length += n;
}

void u_string_builder_prepend (UStringBuilder *self, const gchar* str) {
  string_chunk *chunk = g_new(string_chunk, 1);
  gsize n = strlen(str);
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

const gchar* u_string_builder_build (UStringBuilder *self) {
  gchar *result, *cursor;
  result = cursor = (gchar*)g_malloc(self->length);
  g_queue_foreach (self->queue, (GFunc)chunk_process, &cursor);
  return result;
}

#ifdef STRING_BUILDER_TESTS

#include "string-utils.h"

int main(int argc, char const *argv[]) {
  UStringBuilder* sb = u_string_builder_new();
  u_string_builder_append(sb,"<header string>");
  u_string_builder_append(sb,"<body string>");
  u_string_builder_append(sb,"<footer string>");
  u_string_builder_prepend(sb,"<super header string>");
  g_assert(u_str_match(u_string_builder_build(sb), "<super header string><header string><body string><footer string>"));
  printf("%s\n", u_string_builder_build(sb));
  g_object_unref(sb);

  sb = u_string_builder_new();
  g_assert(g_malloc(0) == NULL);
  g_assert(u_string_builder_build(sb) == NULL);
  g_object_unref(sb);
  return 0;
}

#endif
