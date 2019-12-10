#include <bigint/algorithm.h>

bigint_t bigint_from_factorial(uint32_t n){
	bigint_t bigint = bigint_create(bigint_factorial_size(n));
	return bigint_take_factorial(bigint, n);
}

uint32_t bigint_factorial_size(uint32_t n){
	return 0;		
}

bigint_t bigint_take_factorial(bigint_t bigint, uint32_t n){
	return bigint;
}
