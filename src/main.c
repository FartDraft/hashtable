#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hashtable.h"

#define return_defer(value)                                                                                            \
    do {                                                                                                               \
        result = (value);                                                                                              \
        goto defer;                                                                                                    \
    } while (0)

typedef char person_t[52];
typedef char phone_t[12];
GENERATE_HASHTABLE(person_t, phone_t);

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME  1099511628211UL

size_t
hash_fnv1a(const person_t* key) {
    size_t hash = FNV_OFFSET;
    for (const char* p = *key; *p; p++) {
        hash ^= (size_t)*p;
        hash *= FNV_PRIME;
    }
    return hash;
}

int
cmp_str(const person_t* a, const person_t* b) {
    return strcmp(*a, *b);
}

int
main(void) {
    int result = EXIT_SUCCESS;
    const char path[] = "res/data.txt";

    FILE* file = fopen(path, "rt");
    if (!file) {
        fprintf(stderr, "Could not open %s for reading\n", path);
        return_defer(EXIT_FAILURE);
    }

    person_t person;
    phone_t phone;
    hashtable_person_t_phone_t_t* table = ht_create_person_t_phone_t(131072, 0.7, hash_fnv1a, cmp_str);
    if (table == NULL) {
        fprintf(stderr, "Could not allocate hashtable!\n");
        return_defer(EXIT_FAILURE);
    }

    while (fscanf(file, "%[^,], %11s\n", person, phone) == 2) {
        switch (ht_insert_person_t_phone_t(table, &person, &phone)) {
            case HT_INSERTED: break;
            case HT_UPDATED: printf("%s updated\n", person); break; // 9
            case HT_SHOULD_GROW:
                fprintf(stderr, "Hashtable needs more memory!\n");
                return_defer(EXIT_FAILURE);
                break;
            case HT_FAILURE:
                fprintf(stderr, "Hashtable is corrupted!\n");
                return_defer(EXIT_FAILURE);
                break;
            default: fprintf(stderr, "Unknown hashtable status!\n"); return_defer(EXIT_FAILURE);
        }
    }

    person_t sample1 = "Isaac Egnew";
    printf("\n%s - %s\n", sample1, *ht_const_lookup_person_t_phone_t(table, &sample1));
    ht_erase_person_t_phone_t(table, &sample1);
    printf("Item after erase: %s is %s\n", sample1,
           ht_contains_person_t_phone_t(table, &sample1) == HT_FOUND ? "found" : "not found");

    person_t sample2 = "Alaine Doby";
    printf("\n%s - %s\n", sample2, *ht_const_lookup_person_t_phone_t(table, &sample2));

    // person_t iperson;
    // phone_t iphone;
    // hashtable_it_person_t_phone_t_t* it = ht_create_iterator_person_t_phone_t(table);
    // while (ht_next_person_t_phone_t(it, &iperson, &iphone) == HT_FOUND) {
    //     printf("%s %s\n", iperson, iphone);
    // }

    ht_clear_person_t_phone_t(table);
    printf("Size after clear: %zu\n", table->size);

defer:
    // ht_destroy_iterator_person_t_phone_t(it);
    fclose(file);
    ht_destroy_person_t_phone_t(table);
    return result;
}
