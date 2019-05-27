#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "list.h"

node *node_create(uintmax_t value) {
	node *n = NULL;
	n = malloc(sizeof *n);
	if (n == NULL) {
		fprintf(stderr, "unable to allocate node for %lu\n", value);
		exit(1);
	}
	n->val = value;
	n->next = NULL;
	return n;
}

void list_append(node *list, uintmax_t value) {
	node *iter = list;
	while (iter->next) {
		iter = iter->next;
	}
	iter->next = node_create(value);
}

node *list_new_head(node *list, uintmax_t value) {
	node *head = node_create(value);
	head->next = list;
	return head;
}

node *list_remove_value(node *list, uintmax_t value) {
	node *iter, *tmp;
	iter = list;
	if (iter->val == value) {
		tmp = iter->next;
		free(iter);
		return tmp;
	}

	while (iter->next) {
		if (iter->next->val == value) {
			tmp = iter->next;
			iter->next = iter->next->next;
			free(tmp);
			break;
		}
		iter = iter->next;
	}

	return list;
}

node *list_remove_node(node *list, node *target) {
	node *iter, *tmp;
	iter = list;
	if (iter == target) {
		tmp = iter->next;
		free(iter);
		return tmp;
	}
	while (iter->next) {
		if (iter->next == target) {
			tmp = iter->next;
			iter->next = iter->next->next;
			free(tmp);
			break;
		}
		iter = iter->next;
	}

	return list;
}

size_t list_length(node *list) {
	size_t length = 0;
	node *iter;
	for (iter = list; iter; iter = iter->next)
		length++;

	return length;
}

void list_free(node *list) {
	node *iter, *tmp;
	iter = list;
	while (iter) {
		tmp = iter->next;
		free(iter);
		iter = tmp;
	}
}

