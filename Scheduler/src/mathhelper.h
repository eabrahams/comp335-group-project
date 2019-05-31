#ifndef mathhelper_h_
#define mathhelper_h_

#define max(max_x,max_y) ({ \
		__typeof__ (max_x) _max_x = (max_x); \
		__typeof__ (max_y) _max_y = (max_y); \
		_max_x > _max_y ? _max_x : _max_y; })

#define min(min_x,min_y) ({ \
		__typeof__ (min_x) _min_x = (min_x); \
		__typeof__ (min_y) _min_y = (min_y); \
		_min_x < _min_y ? _min_x : _min_y; })

#define ge_zero(ge_zero_val) ({ \
		__typeof__ (ge_zero_val) _ge_zero_val = (ge_zero_val); \
		__typeof__ (ge_zero_val) _zero = 0; \
		_ge_zero_val > _zero ? _ge_zero_val : _zero; })

#define difference(diff_x,diff_y) ({ \
		__typeof__ (diff_x) _diff_x = (diff_x); \
		__typeof__ (diff_y) _diff_y = (diff_y); \
		max(_diff_x,_diff_y) - min(_diff_x,_diff_y); })

#define clamp(val,lower,upper) ({ \
		__typeof__ (val) _val = (val); \
		__typeof__ (lower) _lower = (lower); \
		__typeof__ (upper) _upper = (upper); \
		min(_upper, max(_lower, _val)); })

#define lerp(lerp_a,lerp_b,lerp_amount) ({ \
		__typeof__ (lerp_a) _lerp_a = (lerp_a); \
		__typeof__ (lerp_b) _lerp_b = (lerp_b); \
		__typeof__ (lerp_amount) _lerp_amount = (lerp_amount); \
		_lerp_a + (_lerp_b - _lerp_a) * _lerp_amount; })

#define mean(arr,size) ({ \
		__typeof__ (size) _size = (size); \
		__typeof__ (*arr) _result = 0; \
		__typeof__ (size) _index; \
		for (_index = 0; _index < _size; _index++) { \
			_result += (arr[_index]); \
		} \
		_result / (__typeof__ (_result)) _size; })

#define dpow(dpow_x, dpow_y) ({ \
		__typeof__ (dpow_x) _dpow_x = (dpow_x); \
		__typeof__ (dpow_y) _dpow_y = (dpow_y); \
		__typeof__ (dpow_x) _dpow_result = 0; \
		__typeof__ (dpow_y) _dpow_index; \
		for (_index = 0; _index < _y; _index++) { \
			_result += _dpow_x \
		} \
		_result == 0 ? 1 : _result; })

#include <stdbool.h>

bool isprime(const unsigned long x);
void factors(const size_t x, size_t **output, size_t *size);
bool isperfect(const unsigned long x);
unsigned long gcd(const unsigned long a, const unsigned long b);

#endif
