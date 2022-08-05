#ifndef STACK_H
#define STACK_H

// A structure representing a stack (LIFO data structure).
typedef struct Stack_t Stack;

/* Create an empty stack.
 *
 * RETURNS: the new stack.
 */
Stack *stackCreate(void);

/* Deallocate the memory occupied by a stack.
 * NB: the memory associated to the data pushed on the stack is not freed.
 *
 * PARAMETERS:
 * - `s`: the stack
 */
void stackFree(Stack *s);

/* Push a new element on a stack.
 *
 * PARAMETERS:
 * - `s`: the stack
 * - `data`: the data to push on the stack.
 */
void stackPush(Stack *s, void *data);

/* Return the data that is at the top of a stack.
 *
 * PARAMETERS:
 * - `s`: the stack
 *
 * RETURNS: a void pointer to the data.
 */
void *stackTop(Stack *s);

/* Pop the data that is at the top of a stack and return it.
 *
 * PARAMETERS:
 * - `s`: the stack
 *
 * RETURNS: a void pointer to the data.
 */
void *stackPop(Stack *s);

/* Return the size of a stack.
 *
 * PARAMETERS:
 * - `s`: the stack
 *
 * RETURNS: the size of the stack.
 */
int stackSize(Stack *s);

/* Check whether or not a stack is empty.
 *
 * PARAMETERS:
 * - `s`: the stack
 *
 * RETURNS: 1 if the stack is empty, 0 otherwise.
 */
int stackIsEmpty(Stack *s);


#endif
