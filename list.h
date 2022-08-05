#ifndef LIST_H
#define LIST_H

// A structure representing a linked list.
typedef struct List_t List;

/* Create an empty list.
 *
 * RETURNS: the new list.
 */
List *listCreate(void);

/* Add a new element in a list.
 *
 * PARAMETERS:
 * - `l`: the list.
 * - `data`: the data to add.
 */
void listAdd(List *l, void *data);

/* Deallocate the memory occupied by a list, as well as all the data it contains.
 *
 * PARAMETERS:
 * - `l`: the list.
 */
void listFree(List *l);

#endif
