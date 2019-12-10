#include <bigint.h>
#include <cstdio>

int main(int argc, char** argv){
	
	bigint_t bigint;
	unsigned int base;

	if(argc < 2){
		printf("usage: %s <base> <number>\n", *argv);
		return 0;
	}

	base = (unsigned int)atoi(argv[1]);	

	// base 8 is still not implemented, so available bases are: 2, 10, 16
	if(!bigint_check_base(base)){
		printf("%s", "available bases are: 2, 10, 16\n");	
	}

	bigint = bigint_from_str(argv[2]);

	printf("%s\n", bigint_to_string(bigint, base));

	// this will tell library to free it's local string buffer
	bigint_to_string(NULL, 0);

	bigint_destroy(bigint);

	return 0;
}
