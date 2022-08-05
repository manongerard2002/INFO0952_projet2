#include <stdlib.h>
#include <stdio.h>

#include "list.h"


typedef struct Node_t {
	void *data;
	struct Node_t *next;
} Node;

struct List_t {
	Node *first;
};


static void terminate(char *m);


static void terminate(char *m) {
	printf("%s\n",m);
	exit(EXIT_FAILURE);
}

List *listCreate() {
  List *l = malloc(sizeof(List));
  if (!l)
    terminate("List cannot be created.");

	l->first = NULL;
	return l;
}

void listAdd(List *l, void *data) {
	Node *new = malloc(sizeof(Node));
  if (!new)
    terminate("Node cannot be created.");

  new->next = l->first;
  l->first = new;

  new->data = data;
}

void listFree(List *l) {
	Node *cur = l->first;
	while (cur) {
		Node *next = cur->next;
		free(cur->data);
		free(cur);
		cur = next;
	}
	free(l);
}
