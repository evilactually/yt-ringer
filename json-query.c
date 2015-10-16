
#include "json-query.h"

typedef enum ACTION ACTION;
typedef enum ACTION_TYPE ACTION_TYPE;
typedef struct Action Action;
typedef struct QueryResult QueryResult;

enum ACTION {
  ACTION_ROOT,
  ACTION_MEMBER,
  ACTION_ELEMENT
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
    const gchar* method;
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


Action* new_method_action(ACTION_TYPE type, const gchar* method) {
  Action* result =  g_new(Action, 1);
  result->action = ACTION_ROOT;
  result->type = type;
  result->argument.method = method;
  return result;
}

Action* new_element_action(ACTION_TYPE type, int index) {
  Action* result =  g_new(Action, 1);
  result->action = ACTION_ROOT;
  result->type = type;
  result->argument.index = index;
  return result;
}

QueryResult json_query_interpret(JsonNode* root, GList* actions) {
  QueryResult result, previous_result;
  do {
    Action* action = (Action*)actions->data;
    switch (action->action) {
      previous_result = result;
      result.type = action->type;
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
              result.value.as_string = json_object_get_string_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_INT:
              result.value.as_int = json_object_get_int_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_DOUBLE:
              result.value.as_double = json_object_get_double_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_BOOLEAN:
              result.value.as_boolean = json_object_get_boolean_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_ARRAY:
              result.value.as_array = json_object_get_array_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_OBJECT:
              result.value.as_object = json_object_get_object_member(previous_result.value.as_object, action->argument.method);
              break;
            case ACTION_TYPE_NULL:
              result.value.as_null = json_object_get_null_member(previous_result.value.as_object, action->argument.method);
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

#ifdef JSON_QUERY_TESTS

int main(int argc, char const *argv[]) {
  /* code */
  return 0;
}

#endif
