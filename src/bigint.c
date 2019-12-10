#include "bigint.h"

#define hex2dec(A) ((A) <= '9' ? (A) - '0' : (A) <= 'F' ? (A) - 'A' + 10 : (A) - 'a' + 10)

uint64_t bigint_get64(bigint_t bigint){
	return  ((uint64_t)bigint[0] << 56) | ((uint64_t)bigint[1] << 48) |\
		((uint64_t)bigint[2] << 40) | ((uint64_t)bigint[3] << 32) |\
		((uint64_t)bigint[4] << 24) | ((uint64_t)bigint[5] << 16) |\
		((uint64_t)bigint[6] << 8 ) | ((uint64_t)bigint[7]      );
}

uint32_t bigint_get32(bigint_t bigint){
	return  ((uint32_t)bigint[0] << 24) | ((uint32_t)bigint[1] << 16) |\
		((uint32_t)bigint[2] << 8 ) | ((uint32_t)bigint[3]      );
}

uint16_t bigint_get16(bigint_t bigint){
	return  ((uint16_t)bigint[0] << 8) | ((uint16_t)bigint[1]);
}

uint8_t bigint_get8(bigint_t bigint){
	return *bigint;
}

void bigint_set64(bigint_t bigint, uint64_t value){
	bigint[0] = (value >> 56) & 0xff;
	bigint[1] = (value >> 48) & 0xff;
	bigint[2] = (value >> 40) & 0xff;
	bigint[3] = (value >> 32) & 0xff;
	bigint[4] = (value >> 24) & 0xff;
	bigint[5] = (value >> 16) & 0xff;
	bigint[6] = (value >>  8) & 0xff;
	bigint[7] = (value      ) & 0xff;
}

void bigint_set32(bigint_t bigint, uint32_t value){
	bigint[0] = (value >> 24) & 0xff;
	bigint[1] = (value >> 16) & 0xff;
	bigint[2] = (value >>  8) & 0xff;
	bigint[3] = (value 	) & 0xff;
}

void bigint_set16(bigint_t bigint, uint16_t value){
	bigint[0] = (value >>  8) & 0xff;
	bigint[1] = (value      ) & 0xff;
}

void bigint_set8(bigint_t bigint, uint8_t value){
	*bigint = value;
}

bigint_t bigint_create(uint32_t size){
	size = bigint_adjust_size(size);
	uint32_t * memory = (uint32_t*)malloc(size + 4);
	*memory = size;
	memset(memory + 1, 0, size);
	return (bigint_t)(memory + 1);
}

bigint_t bigint_make_copy(bigint_t bigint){
	__check_bigint(bigint);
	bigint_t copy = bigint_create(bigint_get_size(bigint));
	memcpy(copy, bigint, bigint_get_size(bigint));
	return copy;
}

bigint_t bigint_copy(bigint_t dst, bigint_t src){
	__check_bigint(dst); __check_bigint(src);
	if(bigint_get_size(dst) < bigint_get_size(src))
		return NULL;
	memset(dst, 0, bigint_get_size(dst));
	memcpy(dst + bigint_get_size(dst) - bigint_get_size(src), src, bigint_get_size(src));
	return dst;
}

bigint_t bigint_min_copy(bigint_t bigint){
	uint32_t size = bigint_get_size(bigint);
	bigint_skip_zero(&bigint, &size);
	bigint_t copy = bigint_create(size);
	memcpy(copy, bigint, size);
	return copy;
}

void bigint_hex_init(bigint_t bigint, const char* str){
	__check_bigint_void(bigint);
	uint32_t b_index, s_index;
	b_index = bigint_get_size(bigint);
	s_index = strlen(str);
	while(s_index > 1 && b_index){
		bigint[b_index - 1]  = hex2dec(str[s_index - 1]);
		bigint[b_index - 1] |= hex2dec(str[s_index - 2]) << 4;
		b_index--;
		s_index -= 2;
	}	
	if(b_index != 0 && s_index == 1){
		bigint[b_index - 1] = hex2dec(str[s_index - 1]);
	}
}

void bigint_oct_init(bigint_t bigint, const char* str){
	__check_bigint_void(bigint);
	assert(BIGINT_NOT_IMPLEMENTED);
}

void bigint_dec_init(bigint_t bigint, const char* str){
	__check_bigint_void(bigint);
	uint32_t len = strlen(str);
	while(len > 3){
		bigint_pmul(bigint, 10000);
		bigint_padd(bigint, (str[0] - '0') * 1000 + (str[1] - '0') * 100 + (str[2] - '0') * 10 + (str[3] - '0'));
		str += 4;
		len -= 4;
	}
	while(len){
		bigint_pmul(bigint, 10);
		bigint_padd(bigint, str[0] - '0');
		str++;
		len--;
	}
}

void bigint_bin_init(bigint_t bigint, const char* str){
	__check_bigint_void(bigint);	
	uint32_t len = strlen(str);
	uint32_t size = bigint_get_size(bigint);

	while((len >> 6) && size){
		len  -= 64;
		size  -= 8;
		bigint_set64(bigint + size, bigint_bitstr_to64(str + len, 64));
	}

	if(len && size){
		bigint_set64(bigint + size - 8, bigint_bitstr_to64(str, len));
	}
}

bigint_t bigint_str_init(bigint_t bigint, const char* str){
	__check_bigint(bigint);
	memset(bigint, 0, bigint_get_size(bigint));

	if(str == NULL || str[0] == 0 || (str[0] == '0' && str[1] == 0))
		return bigint;

	if(str[0] == '0'){ // hex/bin/oct
		if(str[1] == 'x' || str[1] == 'X'){
			bigint_hex_init(bigint, str + 2);
		}else if(str[1] == 'b' || str[1] == 'B'){
			bigint_bin_init(bigint, str + 2);
		}else{
			bigint_oct_init(bigint, str + 1);
		}
	}else{
		bigint_dec_init(bigint, str);
	}

	return bigint;
}

bigint_t bigint_num_init(bigint_t bigint, uint64_t num){
	__check_bigint(bigint);
	memset(bigint, 0, bigint_get_size(bigint));
	if(num){
		bigint_set64(bigint + bigint_get_size(bigint) - 8, num);
	}
	return bigint;
}

bigint_t bigint_from_str(const char* str){
	bigint_t bigint = bigint_create(bigint_byte_size(str));
	return bigint_str_init(bigint, str);
}

bigint_t bigint_from_num(uint64_t num){
	bigint_t bigint = bigint_create(BIGINT_MIN_SIZE);	
	return bigint_num_init(bigint, num);
}

bigint_t bigint_pmul(bigint_t bigint, uint16_t value){
	__check_bigint(bigint);

	uint64_t carry = 0;
	uint32_t i = bigint_get_size(bigint);
	
	if(value == 0){
		memset(bigint, 0, i);
		return bigint;
	}

	while(i > 0){
		carry += value * (uint64_t)bigint_get16(bigint + i - 2);
		bigint[i - 1] = carry 		& 0xff;
		bigint[i - 2] = (carry >> 8)	& 0xff;
		carry >>= 16;
		i -= 2;	
	}

	return bigint;
}

uint32_t bigint_pdiv(bigint_t bigint, uint32_t value){
	__check_bigint_uint(bigint);

	uint64_t carry = 0;
	uint64_t tmp;
	uint32_t i = bigint_get_size(bigint);
	bigint_skip_zero(&bigint, &i);	
	while(i > 0){
		carry <<= 32;
		tmp = (carry + bigint_get32(bigint)) / value;	
		carry = (carry + bigint_get32(bigint)) % value;
		bigint[0] = (tmp >> 24)	& 0xff;
		bigint[1] = (tmp >> 16)	& 0xff;
		bigint[2] = (tmp >>  8)	& 0xff;
		bigint[3] = (tmp      )	& 0xff;
		bigint  += 4;
		i 	-= 4;
	}
	return (uint32_t)carry;
}

uint32_t bigint_pmod(bigint_t bigint, uint32_t value){
	__check_bigint_uint(bigint);

	uint64_t carry = 0;
	uint16_t tmp;
	uint32_t i = bigint_get_size(bigint);
	bigint_skip_zero(&bigint, &i);	
	while(i > 0){
		carry = (carry + bigint_get32(bigint)) % value;
		bigint  += 4;
		i	-= 4;
	}
	return carry;
}

bigint_t bigint_padd(bigint_t bigint, uint64_t value){
	__check_bigint(bigint);
	uint32_t size = bigint_get_size(bigint) - 8;
	uint64_t carry = value;
	while(carry){
		value = bigint_get64(bigint + size);
		carry += value;
		bigint_set64(bigint + size, carry);
		carry = carry < value;
		size -= 8;
	}
	return bigint;
}

bigint_t bigint_pow(bigint_t bigint, uint64_t value){
	assert(BIGINT_NOT_IMPLEMENTED);
	return bigint;
}

bigint_t bigint_shl(bigint_t bigint, uint64_t shift){
	__check_bigint(bigint);	
	if(shift == 0)
		return bigint;
	// TODO: shift qwords
	assert(shift < 64);
	// shift &= 0x3f;
	// ----------------
	
	uint64_t carry = 0, tmp;
	uint32_t i = bigint_get_size(bigint) - 8;

	while(1){
		tmp = bigint_get64(bigint + i);
		bigint_set64(bigint + i, (tmp << shift) + carry);
		carry = tmp >> (64 - shift);
		if(i == 0)
			break;
		i -= 8;
	}
	
	return bigint;
}

bigint_t bigint_shr(bigint_t bigint, uint64_t shift){
	__check_bigint(bigint);
	if(shift == 0)
		return bigint;
	// TODO: shift qwords
	assert(shift < 64);
	// shift &= 0x3f
	// --------------

	uint64_t carry = 0, tmp;
	uint32_t i = 0;

	while(i < bigint_get_size(bigint)){
		tmp = bigint_get64(bigint + i);
		bigint_set64(bigint + i, (tmp >> shift) + carry);
		carry = tmp << (64 - shift);
		i += 8;
	}

	return bigint;
}

bigint_t bigint_add(bigint_t a, bigint_t b){
	__check_bigint(a);
	assert(b != NULL);

	uint32_t a_index = bigint_get_size(a) - 8, b_index = bigint_get_size(b) - 8, sticky;
	uint64_t carry, tmp;

	assert(a_index <= b_index);

	sticky = 0;

	while(1){
		tmp = bigint_get64(b + b_index);	
		carry = bigint_get64(a + a_index) + tmp + sticky;
		bigint_set64(a + a_index, carry);
		if(a_index == 0 || b_index == 0)
			break;
		sticky = (sticky && !carry) || (carry - sticky < tmp);
		a_index -= 8;
		b_index -= 8;
	}
	return a;
}

bigint_t bigint_mul(bigint_t a, bigint_t b){
	__check_bigint(a); __check_bigint(b);
	bigint_t partial, copy;
	uint32_t size = bigint_get_size(b);

	bigint_skip_zero16(&b, &size);
	if(size == 0){
		memset(a, 0, bigint_get_size(a));
		return a;
	}

	partial = bigint_create(bigint_get_size(a));
	copy    = bigint_make_copy(a);

	bigint_num_init(a, 0);

	while(1){
		size -= 2;	

		bigint_copy(partial, copy);
		bigint_pmul(partial, bigint_get16(b));
		bigint_add(a, partial);

		if(size == 0)
			break;		
		bigint_shl(a, 16);
		b += 2;
	}
	
	bigint_destroy(partial);
	bigint_destroy(copy);
	return a;		
}
uint32_t bigint_pcmp(bigint_t bigint, uint64_t value){
	if(bigint == NULL)
		return BIGINT_NEQUAL;
	uint32_t size = bigint_get_size(bigint);
	bigint_skip_zero(&bigint, &size);
	if(size == 0){
		return value == 0 ? BIGINT_EQUAL : BIGINT_LESS;	
	}
	while(size > 8){
		if(*(uint64_t*)bigint != 0)
			return BIGINT_GREATER;
		size   -= 8;
		bigint += 8;
	}
	return bigint_get64(bigint) > value ? BIGINT_GREATER : bigint_get64(bigint) < value ? BIGINT_LESS : BIGINT_EQUAL;
}

extern char* bigint_to_string(bigint_t bigint, uint32_t base){
	static char * bstr = NULL;
	static uint32_t bstr_size = 0;
	if(bigint == NULL){
		if(bstr != NULL){
			free(bstr);	
			bstr_size = 0;
		}
		return NULL;
	}
	if(bstr_size < (bigint_get_size(bigint) << 3) + 4){
		if(bstr_size != 0){
			free(bstr);
		}
		bstr_size = 4 + (bigint_get_size(bigint) << 3);
		bstr = (char*)malloc(bstr_size);
	}
	if(bigint_to_string_s(bigint, base, bstr, bstr_size))
		return NULL;
	return bstr;
}

uint32_t bigint_to_dec_string_s(bigint_t bigint, char * buffer, uint32_t bufsize){
	bigint_t copy = bigint_make_copy(bigint);		
	uint32_t i = bufsize;
	uint64_t carry;
	while(i > 1 && bigint_pcmp(copy, 0) != BIGINT_EQUAL){
		carry = bigint_pdiv(copy, 100);	
		buffer[i - 1] = (carry % 10) + '0';
		buffer[i - 2] = (carry / 10) + '0';
		i -= 2;
	}
	if(i){
		if(bigint_pcmp(copy, 0) != BIGINT_EQUAL){
			carry = bigint_pdiv(bigint, 10);
			buffer[i - 1] = carry;
			i--;
		}
	}
	if(buffer[i] == '0')
		i++;
	if(i){
		memmove(buffer, buffer + i, bufsize - i);
	}
	carry = bigint_pcmp(copy, 0) != BIGINT_EQUAL;
	bigint_destroy(copy);
	return (uint32_t)carry;
}

uint32_t bigint_to_hex_string_s(bigint_t bigint, char * buffer, uint32_t bufsize){
	uint32_t size = bigint_get_size(bigint);
	bigint_skip_zero8(&bigint, &size);
	while(bufsize > 1 && size){
		buffer[0] = (bigint[0] >> 4);	
		buffer[0] = buffer[0] < 10 ? buffer[0] + '0' : buffer[0] - 10 + 'A';
		buffer[1] = (bigint[0] & 0xf);
		buffer[1] = buffer[1] < 10 ? buffer[1] + '0' : buffer[1] - 10 + 'A';
		buffer += 2;
		bigint += 1;
		bufsize -= 2;
		size--;
	}	
	return size != 0;
}

uint32_t bigint_to_bin_string_s(bigint_t bigint, char * buffer, uint32_t bufsize){
	assert(bigint != NULL); assert(buffer != NULL);
	uint32_t size = bigint_get_size(bigint);
	bigint_skip_zero8(&bigint, &size);
	if((size << 3) > bufsize){
		return 1;
	}
	while((bufsize >> 3) && size){
		buffer[0] = ((bigint[0] >> 7) & 0x1) + '0';
		buffer[1] = ((bigint[0] >> 6) & 0x1) + '0';
		buffer[2] = ((bigint[0] >> 5) & 0x1) + '0';
		buffer[3] = ((bigint[0] >> 4) & 0x1) + '0';
		buffer[4] = ((bigint[0] >> 3) & 0x1) + '0';
		buffer[5] = ((bigint[0] >> 2) & 0x1) + '0';
		buffer[6] = ((bigint[0] >> 1) & 0x1) + '0';
		buffer[7] = ((bigint[0]     ) & 0x1) + '0';
		bufsize -= 8;
		buffer += 8;
		size--;	
		bigint++;
	}
	return 0;	
}

uint32_t bigint_to_string_s(bigint_t bigint, uint32_t base, char* buffer, uint32_t bufsize){
	__check_bigint_uint(bigint); assert(buffer != NULL);
	memset(buffer, 0, bufsize);
	switch(base){
		case 16:
			return bigint_to_hex_string_s(bigint, buffer, bufsize - 1);
		case 10:
			return bigint_to_dec_string_s(bigint, buffer, bufsize - 1);
		case  2:
			return bigint_to_bin_string_s(bigint, buffer, bufsize - 1);
		default:
			return (uint32_t)-1;
	}	
}

void bigint_skip_zero(bigint_t* bigint_ptr, uint32_t * size_ptr){
	__check_bigint_void(bigint_ptr); __check_bigint_void(*bigint_ptr);
	while(*size_ptr && !(*(uint64_t*)(*bigint_ptr) || *(uint64_t*)(*bigint_ptr + 8))){
		*size_ptr     -= 16;
		(*bigint_ptr) += 16;
	}
}

void bigint_skip_zero8(bigint_t * bigint_ptr, uint32_t * size_ptr){
	__check_bigint_void(bigint_ptr); __check_bigint_void(*bigint_ptr);
	while(*size_ptr && !(**bigint_ptr)){
		*size_ptr -= 1;
		*bigint_ptr += 1;
	}
}

void bigint_skip_zero16(bigint_t * bigint_ptr, uint32_t * size_ptr){
	__check_bigint_void(bigint_ptr); __check_bigint_void(*bigint_ptr);
	while(*size_ptr && !(*(uint16_t*)(*bigint_ptr))){
		(*size_ptr)   -= 2;
		(*bigint_ptr) += 2;
	}
}

void bigint_skip_zero32(bigint_t * bigint_ptr, uint32_t * size_ptr){
	__check_bigint_void(bigint_ptr); __check_bigint_void(*bigint_ptr);
	while(*size_ptr && !(*(uint32_t*)(*bigint_ptr))){
		(*size_ptr)   -= 4;
		(*bigint_ptr) += 4;
	}
}

uint32_t bigint_get_actual_size(bigint_t bigint){
	uint32_t size = bigint_get_size(bigint);
	while(size && !(*(uint64_t*)bigint || *(uint64_t*)(bigint + 8))){
		size   -= 16;
		bigint += 16;
	}

	return size;
}

uint32_t bigint_byte_size(const char* str){
	uint32_t size = strlen(str);
	if(str[0] == '0'){
		size--;
		if(str[1] == 'x' || str[1] == 'X'){
			size--;
			size = (size >> 1) + (size & 0x1);	
		}else{
			size = size + (size << 1);
			size = (size >> 3) + ((size & 0x7) && 1);
		}
	}else{
		size >>= 1;
	}
	return size;
}

uint32_t bigint_adjust_size(uint32_t size){
	return size < BIGINT_MIN_SIZE ? BIGINT_MIN_SIZE : (size & BIGINT_SIZE_MASK) << (1 && (size & BIGINT_INSZ_MASK));
}

uint64_t bigint_bitstr_to64(const char* bstr, uint32_t len){
	uint64_t result = 0;
	while(len > 3){
		result <<= 4;			
		result += ((bstr[0] - '0') << 3) | ((bstr[1] - '0') << 2) | ((bstr[2] - '0') << 1) | (bstr[3] - '0');
		len -= 4;
	}
	while(len){
		result <<= 1;
		result += (*bstr - '0');
		bstr++;
		len--;
	}
	return result;
}

