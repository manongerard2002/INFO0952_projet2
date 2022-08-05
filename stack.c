#include <stdlib.h>
#include <stdio.h>

#include "stack.h"


typedef struct Node_t {
	void *data;
	struct Node_t *next;
} Node;

struct Stack_t {
	Node *top;
	int size;
};


static void terminate(char *m);

static void terminate(char *m) {
	printf("%s\n",m);
	exit(EXIT_FAILURE);
}

Stack *stackCreate() {
	Stack *s = malloc(sizeof(Stack));
	if (!s)
		terminate("Stack cannot be created");
	s->top = NULL;
	s->size = 0;
	return s;
}

void stackFree(Stack *s) {
	Node *n = s->top;
	while (n) {
		Node *nNext = n->next;
		free(n);
		n = nNext;
	}
	free(s);
}

void stackPush(Stack *s, void *data) {
	Node *n = malloc(sizeof(Node));
	if (!n)
		terminate("Stack node cannot be created");
	n->data = data;
	n->next = s->top;
	s->top = n;
	s->size++;
}

void *stackTop(Stack *s) {
	if (!(s->top))
		terminate("Stack is empty");
	return s->top->data;
}

void *stackPop(Stack *s) {
	if (!(s->top))
		terminate("Stack is empty");
	Node *n = s->top;
	void *data = n->data;
	s->top = n->next;
	s->size--;
	free(n);
	return data;
}

int stackSize(Stack *s) {
	return s->size;
}

int stackIsEmpty(Stack *s) {
	return (s->size == 0);
}

