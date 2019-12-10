#include <bigint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

	if(argc == 1){
		printf("usage: %s <power>\n", *argv);
		return 0;
	}

	uint64_t power = (uint64_t)atoll(argv[1]);

	bigint_t bigint = bigint_create((power >> 3) + (1 && (power & 0x7)));

	bigint_num_init(bigint, 1);

	if(power)
		bigint_shl(bigint, power);
	
	printf("%s", bigint_to_string(bigint, 10));
	printf("(0x%s)\n", bigint_to_string(bigint, 16));

	bigint_to_string(NULL, 0);
	bigint_destroy(bigint);


	return 0;
}
