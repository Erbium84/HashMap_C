#include <stdlib.h>
#include <string.h>
#include <math.h>
#define PRIME_A 110881
#define PRIME_B 180181

#include "hashtable.h"

static ht_item HT_DELETED_ITEM = {NULL, NULL};  //global pointer which deleted elements point to


//inserting new item in hash table
static ht_item* ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key=strdup(k);
    i->value=strdup(v);
    return i;
}

ht_hash_table* ht_new(){
    ht_hash_table* ht=malloc(sizeof(ht_hash_table));

    ht->size=60;
    ht->count=0;
    ht->items=calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
    
}

//pass pointer to ht element and call delete

static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}

//delete table, pass ptr to table

static void ht_del_table(ht_hash_table* table){
    for (int i = 0; i < table->size; i++) {
        ht_item* item = table->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(table->items);
    free(table);
}


static int ht_hash(const char* s, const int a, const int m){   //returns bucket's index to put data in
    long hash=0;

    int slen=strlen(s);

    for(int i=0; i<slen; i++){
        hash+=(long)(pow(a, slen-i-1)*s[i]);
        hash%=m;
    }
    return (int) hash;
}

static int get_hash(const char* s, const int buckets, const int attempt){
    //idx=hash_a(string)+hash_b(string)+1 mod prime

    int hash_a=ht_hash(s, PRIME_A, buckets);
    int hash_b=ht_hash(s, PRIME_B, buckets);

    int i=(hash_a+ hash_b+1)%buckets;
    return i;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    ht_item* item = ht_new_item(key, value);
    int index = get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;

    //probing to find elements
    while (cur_item != NULL ) {
        if (cur_item != &HT_DELETED_ITEM) {
            if (strcmp(cur_item->key, key) == 0) {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
    } 
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    //while loop until empty bucket
    while (item != NULL) {
        if(item!=&HT_DELETED_ITEM){
            if (strcmp(item->key, key) == 0) {
                
                return item->value;
            }
        }
        index = get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    return NULL;
}


//don't remove, just mark it as deleted



void ht_delete(ht_hash_table* ht, const char* key){
    int index = get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;    //point to universal deleted struct
            }
        }
        index = get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    } 
    ht->count--;
}