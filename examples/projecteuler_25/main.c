#include <bigint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

	bigint_t limit = bigint_pow(bigint_num_init(bigint_create(500), 10), 999);

	bigint_t a, b, c;

	a = bigint_num_init(bigint_create(500), 1);
	b = bigint_make_copy(a);
	c = bigint_create(500);

	unsigned int index = 2;

	while(bigint_cmp(a, limit) == BIGINT_LESS){
		bigint_copy(c, a);
		bigint_add(a, b);
		bigint_copy(b, c);
		index++;
	}

	bigint_destroy(a);
	bigint_destroy(b);
	bigint_destroy(c);
	bigint_destroy(limit);

	printf("index is %u\n", index);

	return 0;
}
