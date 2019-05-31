#ifndef llist_h_
#define llist_h_

#include <stdbool.h>

typedef struct llist_node {
	unsigned long val;
	struct llist_node *next;
} node;

node *node_create(unsigned long value);
node *node_clone(node *n);
void list_add(node **head, unsigned long value);
void list_push(node **head, unsigned long value);
void list_remove(node **head, unsigned long value);
void list_remove_all(node **head, unsigned long value);
void list_remove_at(node **head, size_t index);
void list_remove_from(node **head, node **other);
bool list_contains(node **head, unsigned long value);
size_t list_length(node **head);
size_t list_count(node **head, unsigned long value);
unsigned long list_pop(node **head);
unsigned long list_at(node **head, size_t index);
unsigned long list_last(node **head);
bool list_empty(node **head);
void list_clear(node **head);
void list_insert(node **head, size_t index, unsigned long value);
void list_concat(node **head, node **other);
node *list_clone(node **head);
void list_reverse(node **head);
void list_to_ptr(node **head, unsigned long **ptr, size_t *size);
node *ptr_to_list(unsigned long *ptr, size_t size);

#endif
