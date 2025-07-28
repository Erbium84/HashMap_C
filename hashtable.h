//single key-value pair
typedef struct{
    char* key;
    char* value;
} ht_item;

//structure of hashtable, with values size, pointers to items, and current count
typedef struct{
    int size;
    ht_item** items;
    int count;
} ht_hash_table;

ht_hash_table* ht_new();
static ht_item* ht_new_item(const char* k, const char* v);
static void ht_del_item(ht_item* i);
static void ht_del_table(ht_hash_table* table);

void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);