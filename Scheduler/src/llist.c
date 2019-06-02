#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "llist.h"

node *node_create(unsigned long value) {
	node *n = NULL;
	n = malloc(sizeof *n);
	if (n == NULL) {
		fprintf(stderr, "unable to allocate memory for value %lu\n", value);
		exit(1);
	}
	n->val = value;
	n->next = NULL;
	return n;
}

node *node_clone(node *n) {
	return (n != NULL) ? node_create(n->val) : NULL;
}

void list_add(node **head, unsigned long value) {
	if (!*head) {
		*head = node_create(value);
	} else {
		node *iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = node_create(value);
	}
}

void list_push(node **head, unsigned long value) {
	node *tmp = node_create(value);
	tmp->next = *head;
	*head = tmp;
}

void list_remove(node **head, unsigned long value) {
	if (!*head)
		return;
	node *tmp = NULL;
	if ((*head)->val == value) {
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
		return;
	}

	node *iter = NULL;
	for (iter = *head; iter->next; iter = iter->next) {
		if (iter->next->val == value) {
			tmp = iter->next->next;
			free(iter->next);
			iter->next = tmp;
			break;
		}
	}
}

void list_remove_all(node **head, unsigned long value) {
	if (!*head)
		return;
	size_t count, i;
	count = list_count(head, value);
	for (i = 0; i < count; i++)
		list_remove(head, value);
}

void list_remove_at(node **head, size_t index) {
	if (!*head)
		return;
	size_t counter = 0;
	node *iter = *head;
	while (counter < index - 1) {
		if (iter->next)
			iter = iter->next;
		else
			break;
		counter++;
	}

	if (counter == index - 1) {
		if (iter->next) {
			node *tmp = iter->next;
			iter->next = iter->next->next;
			free(tmp);
		}
	}
}

void list_remove_from(node **head, node **other) {
	node *iter;
	for (iter = *other; iter; iter = iter->next)
		list_remove(head, iter->val);
}

bool list_contains(node **head, unsigned long value) {
	node *iter;
	for (iter = *head; iter; iter = iter->next)
		if (iter->val == value)
			return true;
	return false;
}

size_t list_length(node **head) {
	node *iter;
	size_t length = 0;
	for (iter = *head; iter; iter = iter->next)
		length++;
	return length;
}

size_t list_count(node **head, unsigned long value) {
	size_t count = 0;
	node *iter;
	for (iter = *head; iter; iter = iter->next)
		if (iter->val == value)
			count++;
	return count;
}

unsigned long list_pop(node **head) {
	if (!*head) {
		fprintf(stderr, "cannot pop: head is null\n");
		exit(1);
	}
	unsigned long value = (*head)->val;
	node *tmp = *head;
	*head = (*head)->next;
	free(tmp);
	return value;
}

unsigned long list_at(node **head, size_t index) {
	if (!*head) {
		fprintf(stderr, "cannot access %lu: head is null\n", index);
		exit(1);
	}
	size_t counter = 0;
	node *iter = *head;
	while (counter++ < index)
		if (iter->next)
			iter = iter->next;
		else
			break;

	return iter->val;
}

unsigned long list_last(node **head) {
	return list_at(head, UINT_MAX);
}

bool list_empty(node **head) {
	return *head == NULL;
}

void list_clear(node **head) {
	node *iter, *tmp;
	iter = *head;
	while (iter) {
		tmp = iter;
		iter = iter->next;
		free(tmp);
	}
	*head = NULL;
}

void list_insert(node **head, size_t index, unsigned long value) {
	if (!*head) {
		fprintf(stderr, "cannot insert %lu to %lu: head is null\n", value, index);
		exit(1);
	}
	size_t counter = 0;
	node *iter = *head;
	while (counter++ < index - 1)
		if (iter->next)
			iter = iter->next;
		else
			break;

	node *tmp = iter->next;
	iter->next = node_create(value);
	iter->next->next = tmp;
}

void list_concat(node **head, node **other) {
	node *iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = *other;
}

node *list_clone(node **head) {
	if (!*head)
		return NULL;

	node *newhead, *curiter, *newiter;
	newhead = node_create((*head)->val);
	curiter = (*head)->next;
	newiter = newhead;
	while (curiter) {
		newiter->next = node_clone(curiter);
		curiter = curiter->next;
		newiter = newiter->next;
	}

	return newhead;
}

void list_reverse(node **head) {
	node *newhead, *iter, *tmp;
	newhead = *head;
	iter = (*head)->next;
	while (iter) {
		tmp = iter;
		iter = iter->next;
		tmp->next = newhead;
		newhead = tmp;
	}

	*head = newhead;
}

void list_to_ptr(node **head, unsigned long **ptr, size_t *size) {
	*size = list_length(head);
	*ptr = malloc(sizeof **ptr * *size);
	unsigned long *value = *ptr;
	node *iter;
	for (iter = *head; iter; iter = iter->next)
		*(value++) = iter->val;
}

node *ptr_to_list(unsigned long *ptr, size_t size) {
	if (size == 0 || !ptr)
		return NULL;
	node *head, *iter;
	head = node_create(*ptr);
	iter = head;
	size_t i;
	for (i = 1; i < size; i++) {
		iter->next = node_create(ptr[i]);
		iter = iter->next;
	}

	return head;
}

