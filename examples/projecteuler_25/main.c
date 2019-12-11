#include <bigint.h>
#include <stdio.h>

int main(int argc, char** argv){

	bigint_t a, b, c;

	a = bigint_num_init(bigint_create(500), 1);
	b = bigint_make_copy(a);
	c = bigint_create(500);

	unsigned int index = 2;

	while(strlen(bigint_to_string(c, 10)) < 1000){
		bigint_copy(c, a);
		bigint_add(c, b);
		bigint_copy(b, a);
		bigint_copy(a, c);
		index++;
	}

	bigint_destroy(a);
	bigint_destroy(b);
	bigint_destroy(c);
	bigint_to_string(NULL, 0);

	printf("index is %u\n", index);

	return 0;
}
