#ifndef __BIGINT_H__
#define __BIGINT_H__

# if defined(__cplusplus)
extern "C" {
# endif

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "platform.h"

#define BIGINT_NOT_IMPLEMENTED 		0

#define BIGINT_MIN_SIZE 		16
#define BIGINT_SIZE_MASK		0xfffffff0
#define BIGINT_INSZ_MASK		0xf
#define BIGINT_EQUAL			0
#define BIGINT_GREATER			1
#define BIGINT_LESS			2
#define BIGINT_NEQUAL			3



# if defined(BIGINT_NULL_ASSERT)
# define __check_bigint(B) assert((B) != NULL)
# define __check_bigint_void(B) assert((B) != NULL)
# define __check_bigint_uint(B) assert((B) != NULL)
# elif defined(BIGINT_NO_CHECK)
# define __check_bigint(B)
# define __check_bigint_void(B)
# define __check_bigint_uint(B)
# else
# define __check_bigint(B) if((B) == NULL) return NULL
# define __check_bigint_void(B) if((B) == NULL) return
# define __check_bigint_uint(B) if((B) == NULL) return 0
# endif


#define bigint_check_base(b)    ((b) == 2 || (b) == 10 || (b) == 16)

#define bigint_get_size(B) 	*(uint32_t*)((B) - 4)

#define bigint_destroy(B) 	free((B) - 4);

typedef uint8_t* bigint_t;

BIGINT_API uint64_t 			bigint_get64(bigint_t);
BIGINT_API uint32_t 			bigint_get32(bigint_t);
BIGINT_API uint16_t 			bigint_get16(bigint_t);
BIGINT_API uint8_t  			bigint_get8 (bigint_t);

BIGINT_API void 			bigint_set64(bigint_t, uint64_t);	
BIGINT_API void				bigint_set32(bigint_t, uint32_t);
BIGINT_API void	 	  		bigint_set16(bigint_t, uint16_t);
BIGINT_API void	 			bigint_set8 (bigint_t, uint8_t);
	
BIGINT_API  uint32_t 			bigint_adjust_size(uint32_t);

BIGINT_API bigint_t	 		bigint_create(uint32_t size);
BIGINT_API bigint_t			bigint_make_copy(bigint_t);
BIGINT_API bigint_t			bigint_copy(bigint_t, bigint_t);
BIGINT_API bigint_t			bigint_min_copy(bigint_t);
BIGINT_API bigint_t			bigint_str_init(bigint_t, const char*);
BIGINT_API bigint_t 			bigint_num_init(bigint_t, uint64_t);
BIGINT_API bigint_t 			bigint_from_str(const char*);
BIGINT_API bigint_t 			bigint_from_num(uint64_t);

BIGINT_API uint32_t			bigint_pdiv(bigint_t, uint32_t);
BIGINT_API uint32_t			bigint_pmod(bigint_t, uint32_t);
BIGINT_API bigint_t			bigint_pmul(bigint_t, uint16_t);
BIGINT_API bigint_t			bigint_padd(bigint_t, uint64_t);
BIGINT_API bigint_t			bigint_psub(bigint_t, uint64_t);
BIGINT_API bigint_t			bigint_pow(bigint_t, uint64_t); // not implemented (why?)
BIGINT_API bigint_t			bigint_shl(bigint_t, uint64_t);
BIGINT_API bigint_t			bigint_shr(bigint_t, uint64_t);

BIGINT_API bigint_t 			bigint_div(bigint_t, bigint_t); // not implemented
BIGINT_API bigint_t			bigint_mul(bigint_t, bigint_t);
BIGINT_API bigint_t			bigint_add(bigint_t, bigint_t);
BIGINT_API bigint_t			bigint_sub(bigint_t, bigint_t);

BIGINT_API uint32_t			bigint_pcmp(bigint_t, uint64_t);

BIGINT_API void				bigint_skip_zero  (bigint_t*, uint32_t*);
BIGINT_API void				bigint_skip_zero8 (bigint_t*, uint32_t*);
BIGINT_API void				bigint_skip_zero16(bigint_t*, uint32_t*);
BIGINT_API void				bigint_skip_zero32(bigint_t*, uint32_t*);
BIGINT_API void				bigint_skip_zero64(bigint_t*, uint32_t*);

BIGINT_API uint32_t			bigint_get_actual_size(bigint_t);

BIGINT_API uint32_t 			bigint_byte_size(const char*);

BIGINT_API uint64_t			bigint_bitstr_to64(const char*, uint32_t);
BIGINT_API extern char*			bigint_to_string(bigint_t, uint32_t);
BIGINT_API uint32_t			bigint_to_string_s(bigint_t, uint32_t, char*, uint32_t);

# if defined(__cplusplus)
}
# endif

#endif

