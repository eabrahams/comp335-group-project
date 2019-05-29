#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "llist.h"

node *node_create(unsigned long value) {
	node *n = NULL;
	n = malloc(sizeof *n);
	if (n == NULL) {
		fprintf(stderr, "unable to allocate memory for %lu\n", value);
		exit(1);
	}
	n->val = value;
	n->next = NULL;
	return n;
}

void list_push(node **head, unsigned long value) {
	node *iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = node_create(value);
}

void list_remove(node **head, unsigned long value) {
	if (!*head)
		return;
	node *tmp;
	if ((*head)->val == value) {
		tmp = (*head)->next;
		free(*head);
		*head = tmp;
		return;
	}

	node *iter = *head;
	while (iter->next) {
		if (iter->next->val == value) {
			tmp = iter->next;
			iter->next = iter->next->next;
			free(tmp);
			break;
		}
		iter = iter->next;
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
	unsigned long value = (*head)->val;
	node *tmp = *head;
	*head = (*head)->next;
	free(tmp);
	return value;
}

unsigned long list_at(node **head, size_t index) {
	size_t counter = 0;
	node *iter = *head;
	while (counter < index) {
		if (iter->next)
			iter = iter->next;
		else
			break;
		counter++;
	}

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
	size_t counter = 0;
	node *iter = *head;
	while (counter < index - 1) {
		if (iter->next)
			iter = iter->next;
		else
			break;
		counter++;
	}

	node *tmp = iter->next;
	iter->next = node_create(value);
	iter->next->next = tmp;
}

void list_to_ptr(node **head, unsigned long **ptr, size_t *size) {
	*ptr = NULL;
	*size = 0;
	node *iter;
	for (iter = *head; iter; iter = iter->next) {
		(*size)++;
		*ptr = realloc(*ptr, sizeof **ptr * *size);
		*(*ptr + (*size - 1)) = iter->val;
	}
}

