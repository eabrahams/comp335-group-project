#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mathhelper.h"

bool isprime(const unsigned long x) {
	if (x <= 1 || x % 2 == 0)
		return false;
	if (x < 4 || x == 5)
		return true;
	unsigned long limit, i;
	limit = (unsigned long)sqrt((double)x) + 1;
	for (i = 3; i <= limit; i+=2)
		if (x % i == 0)
			return false;
	return true;
}

void factors(const size_t x, size_t **output, size_t *size) {
	size_t limit, i;
	limit = x / 2;
	*output = NULL;
	*size = 0;
	for (i = 1; i <= limit; i++) {
		if (x % i == 0) {
			(*size)++;
			*output = realloc(*output, sizeof **output * *size);
			*(*output + (*size - 1)) = i;
		}
	}
}

bool isperfect(const unsigned long x) {
	unsigned long *fact, size, i, result;
	factors(x, &fact, &size);
	result = 0;
	for (i = 0; i < size; i++)
		result += fact[i];
	free(fact);

	return result == x;
}

unsigned long gcd(const unsigned long a, const unsigned long b) {
	return (b == 0) ? a : gcd(b, a % b);
}

