#include <bigint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){

	bigint_t limit = bigint_create(bigint_byte_size_n(1024, 10));
	bigint_num_init(limit, 10);
	bigint_pow(limit, 999);

	bigint_t a, b, c;

	a = bigint_num_init(bigint_create(bigint_get_size(limit)), 1);
	b = bigint_make_copy(a);
	c = bigint_create(bigint_get_size(limit));

	unsigned int index = 2;

	while(bigint_test(a, BIGINT_LESS, limit)){
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
