#ifndef __BIGINT_PLATFORM_H__
#define __BIGINT_PLATFORM_H__

# if defined(_WIN32) || defined(__NT__) || defined(__WINDOWS__) || defined(_WINDOWS)
# 	define BIGINT_PLATFORM_WINDOWS
# elif defined(__LINUX__) || defined(__linux__) 
# 	define BIGINT_PLATFORM_LINUX
# elif defined(__UNIX__) || defined(__unix__)
# 	define BIGINT_PLATFORM_UNIX
# elif defined(__CYGWIN__) || defined(__CYGWIN32__)
# 	define BIGINT_PLATFORM_CYGWIN
# elif defined(__MACH__)
# 	define BIGINT_PLATFORM_MACH
# elif defined(__FreeBSD__)
# 	define BIGINT_PLATFORM_FREEBSD
# elif defined(__NetBSD__)
# 	define BIGINT_PLATFORM_NETBSD
# elif defined(__OpenBSD__)
# 	define BIGINT_PLATFORM_OPENBSD
# endif

#ifndef BIGINT_DEPRECATED
# 	if (__GNUC__ >= 4) 
#		define BIGINT_DEPRECATED __attribute__((deprecated))
#	else
#		define BIGINT_DEPRECATED
#	endif
#endif

#ifndef BIGINT_UNUSED
#	ifdef __GNUC__
#		define BIGINT_UNUSED __attribute__((unused))
#	else
#		define BIGINT_UNUSED
#	endif
#endif

#ifndef BIGINT_API
#	if defined(__WIN32__) || defined(__WINRT__)
#		ifdef __BORLANDC__
#			ifdef BUILD_BIGINT
#				define BIGINT_API
#			else
#				define BIGINT_API    __declspec(dllimport)
#			endif
#		else
#   			define BIGINT_API __declspec(dllexport)
#		endif
#	elif defined(__OS2__)
#		ifdef BUILD_BIGINT
#			define BIGINT_API    __declspec(dllexport)
#		else
#			define BIGINT_API
# 		endif
#	else
#		if defined(__GNUC__) && __GNUC__ >= 4
#			define BIGINT_API __attribute__ ((visibility("default")))
#		else
#   			define BIGINT_API
#		endif
#	endif
#endif

#ifndef BIGINT_INLINE
	#if defined(__GNUC__)
		#define BIGINT_INLINE __inline__
	#elif defined(_MSC_VER) || defined(__BORLANDC__) || \
      		defined(__DMC__) || defined(__SC__) || \
      		defined(__WATCOMC__) || defined(__LCC__) || \
      		defined(__DECC) || defined(__CC_ARM)
			#define BIGINT_INLINE __inline
			#ifndef __inline__
				#define __inline__ __inline
			#endif
	#else
		#define BIGINT_INLINE inline
		#ifndef __inline__
			#define __inline__ inline
		#endif
	#endif
#endif 

#ifndef BIGINT_CONST_INLINE
#	if defined(_MSC_VER)
#		define BIGINT_CONST_INLINE __forceinline
#	elif ( (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__) )
#		define BIGINT_CONST_INLINE __attribute__((always_inline)) static __inline__
#	else
#		define BIGINT_CONST_INLINE static BIGINT_INLINE
#	endif
#endif 

#if !defined(__MACH__)
#	ifndef NULL
#		ifdef __cplusplus
#			define NULL 0
#		else
#			define NULL ((void *)0)
#		endif
#	endif /* NULL */
#endif

#endif
