# hashtable

`hashtable` is a dead simple yet blazingly fast hash table in pure C.

## API
`GENERATE_HASHTABLE(key_t, value_t)` - macro to generate a hash table with the specified key and value types.

### Create
`hashtable_t ht_create(size_t capacity, double growth_threshold, size_t (*ht_hash_t)(const key_t*), int (*ht_cmp_t)(const key_t*, const key_t*)` - creates a hash table with the specified capacity, growth threshold, hash function, and comparison function and returns a pointer to the created hash table.

### Destroy
`void ht_destroy(hashtable_t* table)` - frees the memory allocated for the hash table and its associated items.

### Insert
`int ht_insert(hashtable_t* table, const key_t* ket, const value_t* value)` - inserts a key-value pair into the hash table. Returns `HT_FAILURE`, `HT_SHOULD_GROW`, `HT_UPDATED` or `HT_INSERTED` respectively.

### Look up
`value_t* ht_lookup(const hashtable* table, const key_t* key)` - looks up a value in the hash table based on the provided key.

`const value_t* ht_const_lookup(const hashtable* table, const key_t* key)` - constant look up function.

### Contains
`int ht_contains(const hashtable* table, const key_t* key)` - checks if the hash table contains a specific key. Returns `HT_FOUND` and `HT_NOT_FOUND` respectively.

### Erase
`int ht_erase(hashtable* table, const key_t* key)` - erases a key-value pair from the hash table if the key is found. Returns `HT_FOUND` and `HT_NOT_FOUND` respectively.

### Clear
`int ht_clear(hashtable* table)` - clears all the key-value pairs from the hash table. Returns `HT_FOUND` and `HT_NOT_FOUND` respectively.

### Iterator
`int ht_next(hashtable_it* it, key_t* key, value_t* value)` - iterate over the key-value pairs in a hash table. `hashtable_it` needs to be initialized with table and index.

## Usage
[The main.c file](src/main.c) shows example usage of the hash table.

## Implementation
### Collisions
Linear probing for the sake of CPU’s cache locality.

### Resizing
No resizing. This decision was made to preserve simplicity and efficiency. Insert function will return `HT_SHOULD_GROW` if `table->size >= table->growth_threshold * table->capacity`. The rest is up to you.

### Deletion
Tombstone technique. Tombstone is marked with all its bits set to 1.
