#ifndef PC_SP_SMAP_H
#define PC_SP_SMAP_H

typedef struct s_simple_table_entry {
    unsigned int key;
    unsigned int value;
    struct s_simple_table_entry *next;
} simple_table_entry;

typedef struct {
    unsigned int size;
    simple_table_entry **table;
} simple_table;

/**
 * Allocates a new simple table.
 *
 * @param table_size the size of the simple table
 * @return a pointer to the created simple table
 */
simple_table *simple_table_create(unsigned int table_size);

/**
 * Frees the simple table specified by the double-reference and all of its entries and sets the reference to NULL.
 * @param table the double-reference to the table
 */
void simple_table_free(simple_table **table);

/**
 * Allocates a new simple table with the size of 255.
 *
 * @return a pointer to the created simple table
 */
#define simple_table_default() (simple_table_create(255))

/**
 * Gets a pointer to the value specified by the key, if found or NULL if the entry does not exist.
 *
 * @param key
 * @return a pointer to the value specified by the key, if found or NULL if the entry does not exist
 */
unsigned int *simple_table_get(simple_table *table, unsigned int key);

/**
 * Puts the specified value into the entry specified by the key.
 *
 * @param key The key
 * @param value The value
 */
void simple_table_put(simple_table *table, unsigned int key, unsigned int value);

#endif /* PC_SP_SMAP_H */
