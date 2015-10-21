
#include "json-query.h"
#include <stdio.h>

typedef enum ACTION ACTION;
typedef enum ACTION_TYPE ACTION_TYPE;
typedef struct Action Action;
typedef struct QueryResult QueryResult;

enum ACTION {
  ACTION_ROOT,    // $
  ACTION_MEMBER,  // .x
  ACTION_ELEMENT  // [x]
};

enum ACTION_TYPE {
  ACTION_TYPE_INT,
  ACTION_TYPE_STRING,
  ACTION_TYPE_DOUBLE,
  ACTION_TYPE_BOOLEAN,
  ACTION_TYPE_OBJECT,
  ACTION_TYPE_ARRAY,
  ACTION_TYPE_NULL
};

struct Action {
  ACTION action;
  ACTION_TYPE type;
  union {
    int index;
    const gchar* member;
  } argument;
};

struct QueryResult {
  ACTION_TYPE type;
  union {
    const gchar*  as_string;
    gint          as_int;
    gboolean      as_boolean;
    gdouble       as_double;
    JsonArray*    as_array;
    JsonObject*   as_object;
    gboolean      as_null;
  } value;
};

Action* new_root_action(ACTION_TYPE type) {
  Action* result =  g_new(Action, 1);
  result->action = ACTION_ROOT;
  result->type = type;
  return result;
}


Action* new_member_action(ACTION_TYPE type, const gchar* member) {
  Action* result =  g_new(Action, 1);
  result->action = ACTION_MEMBER;
  result->type = type;
  result->argument.member = member;
  return result;
}

Action* new_element_action(ACTION_TYPE type, int index) {
  Action* result =  g_new(Action, 1);
  result->action = ACTION_ELEMENT;
  result->type = type;
  result->argument.index = index;
  return result;
}

QueryResult json_query_interpret(JsonNode* root, GList* actions) {
  QueryResult result, previous_result;
  do {
    Action* action = (Action*)actions->data;
    previous_result = result;
    result.type = action->type;
    switch (action->action) {
      case ACTION_ROOT:
        switch (action->type) {
          case ACTION_TYPE_STRING:
            result.value.as_string = json_node_get_string(root);
            break;
          case ACTION_TYPE_INT:
            result.value.as_int = json_node_get_int(root);
            break;
          case ACTION_TYPE_DOUBLE:
            result.value.as_double = json_node_get_double(root);
            break;
          case ACTION_TYPE_BOOLEAN:
            result.value.as_boolean = json_node_get_boolean(root);
            break;
          case ACTION_TYPE_ARRAY:
            result.value.as_array = json_node_get_array(root);
            break;
          case ACTION_TYPE_OBJECT:
            result.value.as_object = json_node_get_object(root);
            break;
          case ACTION_TYPE_NULL:
            result.value.as_null = json_node_is_null(root);
            break;
        }
        break;
        case ACTION_MEMBER:
          switch (action->type) {
            case ACTION_TYPE_STRING:
              result.value.as_string = json_object_get_string_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_INT:
              result.value.as_int = json_object_get_int_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_DOUBLE:
              result.value.as_double = json_object_get_double_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_BOOLEAN:
              result.value.as_boolean = json_object_get_boolean_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_ARRAY:
              result.value.as_array = json_object_get_array_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_OBJECT:
              result.value.as_object = json_object_get_object_member(previous_result.value.as_object, action->argument.member);
              break;
            case ACTION_TYPE_NULL:
              result.value.as_null = json_object_get_null_member(previous_result.value.as_object, action->argument.member);
              break;
          }
          break;
          case ACTION_ELEMENT:
            switch (action->type) {
              case ACTION_TYPE_STRING:
                result.value.as_string = json_array_get_string_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_INT:
                result.value.as_int = json_array_get_int_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_DOUBLE:
                result.value.as_double = json_array_get_double_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_BOOLEAN:
                result.value.as_boolean = json_array_get_boolean_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_ARRAY:
                result.value.as_array = json_array_get_array_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_OBJECT:
                result.value.as_object = json_array_get_object_element(previous_result.value.as_array, action->argument.index);
                break;
              case ACTION_TYPE_NULL:
                result.value.as_null = json_array_get_null_element(previous_result.value.as_array, action->argument.index);
                break;
            }
            break;
    }
  } while (actions = actions->next);
  return result;
}

enum {
  EXPECT_ACTION,
  EXPECT_INDEX,
  EXPECT_MEMBER,
  READING_INDEX,
  READING_MEMBER,
  EXPECT_CLOSING_BRACKET,
  ERROR
};

#define BUFFER_SIZE 16

GList* json_query_parse(const gchar* query) {
  const gchar* query_cursor = query;
  gint state = EXPECT_ACTION;
  GList* actions = NULL;
  gchar buffer[BUFFER_SIZE];
  gchar* buffer_cursor;

  while(*query_cursor && state < ERROR) {
    printf("%c ", *query_cursor);
    switch (state) {
      case EXPECT_ACTION:
        if (*query_cursor == '$') {
          actions = g_list_prepend(actions, new_root_action(-1));
          query_cursor++;
        } else if (*query_cursor == '.') {
          state = EXPECT_MEMBER;
          query_cursor++;
        } else if (*query_cursor == '[') {
          state = EXPECT_INDEX;
          query_cursor++;
        } else if (*query_cursor == ' ') {
          query_cursor++;
        } else {
          printf("%s\n", "ERROR");
          state = ERROR;
        }
        printf("%s\n", "EXPECT_ACTION");
        break;
      case EXPECT_INDEX:
        if (*query_cursor == ' ') {
          query_cursor++;
        } else if (g_ascii_isdigit (*query_cursor)) {
          state = READING_INDEX;
          buffer_cursor = buffer;
        } else {
          state = ERROR;
        }
        printf("%s\n", "EXPECT_INDEX");
        break;
      case READING_INDEX:
        if (g_ascii_isdigit (*query_cursor)) {
          *buffer_cursor = *query_cursor;
          buffer_cursor++;
          query_cursor++;
        } else if (*query_cursor == ' ' || *query_cursor == ']') {
          *buffer_cursor = '\0';
          int i;
          sscanf(buffer, "%d\n", &i);
          actions = g_list_prepend(actions, new_element_action(-1,i));
          state = *query_cursor == ' ' ? EXPECT_CLOSING_BRACKET : EXPECT_ACTION;
          query_cursor++;
        } else {
          state = ERROR;
        }

        printf("%s\n", "READING_INDEX");
        break;
    }
  }
}

void json_query_resolve_types(GList* actions) {
  //
}

#ifdef JSON_QUERY_TESTS


int main(int argc, char const *argv[]) {
  Action* a_0 = new_root_action(ACTION_TYPE_OBJECT);
  Action* a_1 = new_member_action(ACTION_TYPE_STRING, "id");
  JsonParser* parser = json_parser_new();
  json_parser_load_from_file(parser, "json-query-test-01.json", NULL);
  JsonNode* root = json_parser_get_root(parser);
  GList* actions = g_list_prepend(NULL, a_0);
  QueryResult r = json_query_interpret(root, actions);
  g_assert(r.type == ACTION_TYPE_OBJECT);
  actions = g_list_prepend(actions, a_1);
  actions = g_list_reverse(actions);
  r = json_query_interpret(root, actions);
  g_assert(r.type == ACTION_TYPE_STRING);
  g_assert_cmpstr(r.value.as_string, ==, "asdfghjkl");
  g_object_unref(parser);
  g_list_free(actions);
  g_free(a_0);
  g_free(a_1);

  a_0 = new_root_action(ACTION_TYPE_OBJECT);
  a_1 = new_member_action(ACTION_TYPE_ARRAY, "items");
  Action* a_2 = new_element_action(ACTION_TYPE_OBJECT, 0);
  Action* a_3 = new_member_action(ACTION_TYPE_STRING, "id");
  parser = json_parser_new();
  json_parser_load_from_file(parser, "json-query-test-02.json", NULL);
  root = json_parser_get_root(parser);
  actions = g_list_prepend(NULL, a_0);
  actions = g_list_prepend(actions, a_1);
  actions = g_list_prepend(actions, a_2);
  actions = g_list_prepend(actions, a_3);
  actions = g_list_reverse(actions);
  r = json_query_interpret(root, actions);
  g_assert(r.type == ACTION_TYPE_STRING);
  g_assert_cmpstr(r.value.as_string, ==, "UCtZP0L7yoEycj6w-f7uVNaA");
  g_object_unref(parser);
  g_list_free(actions);
  g_free(a_0);
  g_free(a_1);
  g_free(a_2);
  g_free(a_3);

  a_0 = new_root_action(ACTION_TYPE_OBJECT);
  a_1 = new_member_action(ACTION_TYPE_NULL, "je_suis_null");
  parser = json_parser_new();
  json_parser_load_from_file(parser, "json-query-test-02.json", NULL);
  root = json_parser_get_root(parser);
  actions = g_list_prepend(NULL, a_0);
  actions = g_list_prepend(actions, a_1);
  actions = g_list_reverse(actions);
  r = json_query_interpret(root, actions);
  g_assert(r.type == ACTION_TYPE_NULL);
  g_assert(r.value.as_boolean == TRUE);
  g_object_unref(parser);
  g_list_free(actions);
  g_free(a_0);
  g_free(a_1);

  //json_query_parse("$.items");
  json_query_parse("$[1]");
//  Action* a_4 = new_member_action(ACTION_TYPE_STRING, "wrong");
//  r = json_query_interpret(root, actions);

  return 0;
}

#endif
