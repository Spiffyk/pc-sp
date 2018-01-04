#include <stdlib.h>
#include "simple_table.h"



/**
 * Allocates a new table entry.
 *
 * @param key key of the entry
 * @param value value of the entry
 * @return a pointer to the newly allocated entry
 */
simple_table_entry *simple_table_entry_create(unsigned int key, unsigned int value) {
    simple_table_entry *entry;

    entry = malloc(sizeof(simple_table_entry));
    if (!entry) {
        return NULL;
    }

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    return entry;
}

/**
 * Frees the specified entry and all of its sub-entries and sets the pointer to NULL.
 *
 * @param entry the double-pointer to the entry
 */
void simple_table_entry_free(simple_table_entry **entry) {
    if ((*entry)->next) {
        simple_table_entry_free(&((*entry)->next));
    }

    free(*entry);
    *entry = NULL;
}



simple_table *simple_table_create(unsigned int table_size) {
    simple_table *result;

    if (!(result = malloc(sizeof(simple_table)))) {
        return NULL;
    }

    result->size = table_size;

    if (!(result->table = calloc(table_size, sizeof(simple_table_entry *)))) {
        free(result);
        return NULL;
    }

    return result;
}



void simple_table_free(simple_table **table) {
    int i;

    for (i = 0; i < (*table)->size; i++) {
        if ((*table)->table[i]) {
            simple_table_entry_free(&((*table)->table[i]));
        }
    }

    free((*table)->table);
    free(*table);
    *table = NULL;
}



unsigned int *simple_table_get(simple_table *table, unsigned int key) {
    unsigned int hash;
    simple_table_entry *entry;

    hash = key % table->size;
    entry = table->table[hash];

    while(entry) {
        if (entry->key == key) {
            return &(entry->value);
        }

        entry = entry->next;
    }

    return NULL;
}



void simple_table_put(simple_table *table, unsigned int key, unsigned int value) {
    unsigned int hash;
    simple_table_entry *curr, *prev;

    hash = key % table->size;
    curr = table->table[hash];

    if (!curr) {
        table->table[hash] = simple_table_entry_create(key, value);
        return;
    }

    do {
        if (curr->key == key) {
            curr->value = value;
            return;
        }
        prev = curr;
        curr = curr->next;
    } while(curr);

    prev->next = simple_table_entry_create(key, value);
}
