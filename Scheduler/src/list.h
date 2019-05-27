#ifndef list_h_
#define list_h_

#include <stdint.h>

typedef struct llist_node {
	uintmax_t val;
	struct llist_node *next;
} node;

node *node_create(uintmax_t value);
void list_append(node *list, uintmax_t value);
node *list_new_head(node *list, uintmax_t value);
node *list_remove_value(node *list, uintmax_t value);
node *list_remove_node(node *list, node *target);
size_t list_length(node *list);


#endif
