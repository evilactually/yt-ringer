/* A simple(buggy) storage mechanism with fast(hopefully) updates */

const gchar** get_record(const gchar* section_key, const gchar* record_key);
const gchar** get_record(const gchar* section_key, const gchar* record_key);
void update_record(const gchar* section_key, const gchar* record_key, const gchar* data, gint index);
void update_record_string(const gchar* section_key, const gchar* record_key, const gchar* string, gint index); // fixed length string
void update_record_int32(const gchar* section_key, const gchar* record_key, gint i, gint index);
void add_record(const gchar* section_key, const gchar* record_key);
void delete_record(const gchar* section_key, const gchar* record_key);
void section_info(const gchar* id,
                  const int*   field_sizes,
                  size_t       field_count,
                  size_t       record_count); // can be used to check if section exists
void add_section(const gchar* id,
                 const int*   field_sizes,
                 size_t       field_count);
void delete_section(const gchar* id);

struct record {
  // ...
  size_t source_position;
  // ...
}

// struct table_schema {
//   const gchar*   names;
//   const int      size;
//   const int*     field_sizes;
//   size_t         field_count;
// }

// typedef struct table_schema table_schema;

struct db {
  // internal
};

typedef struct db db;

// db* create_db(table_schema** tables, size_t table);
// db* load_db(const gchar* filename);
