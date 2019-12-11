#include <bigint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

	if(argc == 1){
		printf("usage: %s <power>\n", *argv);
		return 0;
	}

	uint64_t power = (uint64_t)atoll(argv[1]);

	/*
	 * Create a big integer object that will be to store the value of 2 ^ power 
	 * (power >> 3) + (1 && (power & 0x7)) - is the actual amount of bytes needed to store the value
	 */
	bigint_t bigint = bigint_create((power >> 3) + (1 && (power & 0x7)));

	// put the value of 1 in `bigint`
	bigint_num_init(bigint, 1);

	if(power)
		bigint_shl(bigint, power); // raise bigint to `power` power
	
	printf("%s", bigint_to_string(bigint, 10)); // print `bigint` in decimal
	printf("(0x%s)\n", bigint_to_string(bigint, 16)); // print `bigint` in hexidecimal

	bigint_to_string(NULL, 0); // free bigint_to_string()'s local buffer
	bigint_destroy(bigint); // free `bigint`'s allocated memory

	return 0;
}
