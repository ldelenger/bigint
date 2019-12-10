#include <bigint/util.h>
uint8_t* bigint_to_bcd(bigint_t bigint){
	__check_bigint(bigint);
	uint32_t size = bigint_get_size(bigint);
	bigint_t copy = bigint_min_copy(bigint);
	bigint_skip_zero(&bigint, &size);	
	uint8_t* bcd = bigint_create((size + 1) << 1);	
	uint32_t carry;
	if(size){
		size = bigint_get_size(bcd);
		while(bigint_pcmp(copy, 0) != BIGINT_EQUAL){
			carry = bigint_pdiv(copy, 100);
			bcd[size - 1]  = carry % 10;	
			bcd[size - 1] |= (carry / 10) << 4;
			size--;
		}
	}
	bigint_destroy(copy);
	return bcd;
}
