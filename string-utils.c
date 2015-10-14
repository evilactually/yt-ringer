
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
  g_assert(b->current_buffer == NULL); // make sure no one starts a result_buffers twice
  b->current_buffer_size = 0;          // (without getting yelled at)
  b->current_buffer = str;
}

void buffer_advance(string_build_buffers *b) {
  g_assert(b->current_buffer);
  b->current_buffer_size++;
}

void buffer_advance_n(string_build_buffers *b, gint n) {
  g_assert(b->current_buffer);
  b->current_buffer_size += n;
}

void buffer_advance_untill_null(string_build_buffers *b) {
  g_assert(b->current_buffer);
  const gchar* c = b->current_buffer;
  while (*c++) { b->current_buffer_size++; }
}

void buffer_finish(string_build_buffers *b) {
  g_assert(b->current_buffer);
  u_string_builder_append_n(b->finished_buffers, b->current_buffer, b->current_buffer_size);
  b->current_buffer = NULL;
  b->current_buffer_size = 0;
}

const gchar* merge_build_buffers(string_build_buffers *b) {
  return u_string_builder_build(b->finished_buffers);
}

/* end */
// TODO: overrise equality function to work with fixed sized strings
#define MAX_VARIABLE_SIZE 8
#define ALLOW_EMPTY_VARIABLES 1

enum {
  STRING_OR_SIGIL,
  OPEN_CURL_OR_STRING,
  SPACE_OR_VARIABLE,
  VARIABLE_OR_SPACE_OR_CLOSE_CURL,
  SPACE_OR_CLOSE_CURL,
  ERROR
};

enum {
  SPACE,
  ALPHANUMERAL,
  CLOSE_CURL
};

const gchar* u_str_substitute_vars(const gchar* template, ...) {
  GHashTable* bindings = g_hash_table_new(g_str_hash, g_str_equal);
  va_list ap;
  va_start(ap,template);
  const gchar* key;
  while (key = va_arg(ap, const gchar*)) {
    const gchar* value = va_arg(ap, const gchar*);
    g_assert(value); // every key must be followed by value
    g_hash_table_insert(bindings, (gpointer)key, (gpointer)value);
  }
  va_end(ap);

  string_build_buffers result_buffers;
  gchar variable_buffer[MAX_VARIABLE_SIZE+1];
  gint variable_size;
  gint state;
  gint expected;
  const gchar* template_cursor = template;

  init_build_buffers(&result_buffers);

  // transition to STRING state
  buffer_start(&result_buffers, template_cursor);
  state=STRING_OR_SIGIL;

  while(*template_cursor && state != ERROR) {
    switch (state) {
      case STRING_OR_SIGIL:               // vvvv
        if(*template_cursor == '$') {     // xyz${ VAR }
          state = OPEN_CURL_OR_STRING;
        } else {
          buffer_advance(&result_buffers);
        }
        template_cursor++;
        //printf("%s\n", "STRING_OR_SIGIL");
        break;                  //  v     v
      case OPEN_CURL_OR_STRING: // ${ or $xyz ...
        if(*template_cursor == '{') {
          state = SPACE_OR_VARIABLE;
          buffer_finish(&result_buffers);
        } else {
          state = STRING_OR_SIGIL;
          buffer_advance(&result_buffers); // save $ as a string
          buffer_advance(&result_buffers);
        }
        template_cursor++;
        //printf("%s\n", "OPEN_CURL_OR_STRING");
        break;                //       vvv
      case SPACE_OR_VARIABLE: // ... ${  VARIABLE } ...
        variable_size = 0;
        if (*template_cursor == ' ') {
          template_cursor++;
        } else if (g_ascii_isalnum(*template_cursor)) {
          variable_buffer[variable_size++] = *template_cursor;
          state = VARIABLE_OR_SPACE_OR_CLOSE_CURL;
          template_cursor++;
        } else {
          state = ERROR;
          expected = SPACE | ALPHANUMERAL;
        }
        //printf("%s\n", "SPACE_OR_VARIABLE");
        break;                                 //   v       v        v
      case VARIABLE_OR_SPACE_OR_CLOSE_CURL:    // VAR or VAR } or VAR}
        if (g_ascii_isalnum(*template_cursor)) {
          if(variable_size >= MAX_VARIABLE_SIZE) {
            state = ERROR;
            expected = SPACE | CLOSE_CURL;
          } else {
            variable_buffer[variable_size++] = *template_cursor;
            template_cursor++;
          }
        } else if (*template_cursor == ' ' || *template_cursor == '}') {
          variable_buffer[variable_size] = '\0';
          const gchar* variable_value = g_hash_table_lookup(bindings, variable_buffer);
          g_assert(variable_value || ALLOW_EMPTY_VARIABLES);
          if(variable_value) {
            buffer_start(&result_buffers, variable_value);
            buffer_advance_untill_null(&result_buffers);
            buffer_finish(&result_buffers);
          }
          if (*template_cursor == '}') {
            state = STRING_OR_SIGIL;
            buffer_start(&result_buffers, template_cursor + 1);
          } else {
            state = SPACE_OR_CLOSE_CURL;
          }
          template_cursor++;
        } else {
          state = ERROR;
          expected = SPACE | CLOSE_CURL | ALPHANUMERAL;
        }
        //printf("%s\n", "VARIABLE_OR_SPACE_OR_CLOSE_CURL");
        break;                                  //     vvvv
      case SPACE_OR_CLOSE_CURL:                 // VAR    }
        if (*template_cursor == ' ') {
          template_cursor++;
        } else if (*template_cursor == '}') {
          state = STRING_OR_SIGIL;
          buffer_start(&result_buffers, template_cursor + 1);
          template_cursor++;
        } else {
          state = ERROR;
          expected = SPACE | CLOSE_CURL;
        }
        //printf("%s\n", "SPACE_OR_CLOSE_CURL");
        break;
    }
  }

  if(state == ERROR) {
    fprintf(stderr, "%s\n", "ERROR");
    g_assert(state != ERROR);
  }

  buffer_finish(&result_buffers);
  const gchar* result = merge_build_buffers(&result_buffers);
  dispose_build_buffers(&result_buffers);
  return result;
}

#ifdef STRING_UTILS_TESTS

#include <stdio.h>

int main(int argc, char const *argv[]) {
  const gchar* SOURCE_STRING_A = "'Twas brillig, and the slithy toves; Did gyre and gimble in the wabe;";
  string_build_buffers b;
  init_build_buffers(&b);

  // brillig
  buffer_start(&b, SOURCE_STRING_A + 6);
  buffer_advance_n(&b,7);
  buffer_finish(&b);
  g_assert(u_str_match(merge_build_buffers(&b), "brillig"));

  // space
  buffer_start(&b, " ");
  buffer_advance(&b);
  buffer_finish(&b);

  // gimble
  buffer_start(&b, SOURCE_STRING_A + 50);
  buffer_advance_n(&b,6);
  buffer_finish(&b);
  g_assert(u_str_match(merge_build_buffers(&b), "brillig gimble"));

  // jabberwock
  buffer_start(&b, "; Beware the Jabberwock");
  buffer_advance_untill_null(&b);
  buffer_finish(&b);
  g_assert(u_str_match(merge_build_buffers(&b), "brillig gimble; Beware the Jabberwock"));

  dispose_build_buffers(&b);

  const gchar* str = u_str_substitute_vars("Twas ${B} and the slithy ${T}.....", "B", "brillig", "T", "toves", NULL);
  printf("%s\n", str);
  printf("%s\n", u_str_substitute_vars("Beware the ${MONSTER}, who lurks in ${PLACE}..", "MONSTER", "Jabberwock", "PLACE", "Nimberland", NULL));
  printf("%s\n", u_str_substitute_vars("Beware the ${MONSTER}, who lurks in ${PLACE}", "MONSTER", "Jabberwock", "PLACE", "Nimberland", NULL));
  printf("%s\n", u_str_substitute_vars("Beware the ${M}, who lurks in ${PLACE}", "MONSTER", "Jabberwock", "PLACE", "Nimberland", NULL));
  printf("%s\n", u_str_substitute_vars("Beware the ${}, who lurks in ${PLACE}", "MONSTER", "Jabberwock", "PLACE", "Nimberland", NULL));
  //u_str_substitute_vars("'Twas '${B} and the slithy ${T}", NULL);

  return 0;
}

#endif
