#ifndef DICT_H
#define DICT_H

// A structure representing a dictionary.
typedef struct Dict_t Dict;

/* Create an empty dictionary.
 *
 * PARAMETERS:
 * - `m`: the size of the hash table.
 *
 * RETURNS: the created dictionary.
 */
Dict *dictCreate(int m);

/* Deallocate the memory occupied by a dictionary.
 * NB: The keys are freed, but not the associated values.
 *
 * PARAMETERS:
 * - `d`: the dictionary to be freed.
 */
void dictFree(Dict *d);

/* Search for a given key in a dictionary and return the associated value.
 *
 * PARAMETERS:
 * - `d`: the dictionary.
 * - `key`: the key to search for.
 *
 * RETURNS: a void pointer to the value associated to the key, or NULL if the
 * key was not found.
 */
void *dictSearch(Dict *d, char *key);

/* Check if a given key is contained in a dictionary.
 *
 * PARAMETERS:
 * - `d`: the dictionary.
 * - `key`: the key to search for.
 *
 * RETURNS: 1 if the key was found, 0 otherwise.
 */
int dictContains(Dict *d, char *key);

/* Insert a new (key, value) pair in a dictionnary, or replace the value of an
 * existing key.
 *
 * PARAMETERS:
 * - `d`: the dictionary.
 * - `key`: the key.
 * - `value`: the new value.
 */
void dictInsert(Dict *d, char *key, void *value);

#endif
