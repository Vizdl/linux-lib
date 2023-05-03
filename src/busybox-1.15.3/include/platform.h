/* vi: set sw=4 ts=4: */
/*
   Copyright 2006, Bernhard Reutner-Fischer

   Licensed under the GPL v2 or later, see the file LICENSE in this tarball.
*/
#ifndef	BB_PLATFORM_H
#define BB_PLATFORM_H 1

/* Convenience macros to test the version of gcc. */
#undef __GNUC_PREREQ
#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
		((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

/* __restrict is known in EGCS 1.2 and above. */
#if !__GNUC_PREREQ(2,92)
# ifndef __restrict
#  define __restrict
# endif
#endif

/* Define macros for some gcc attributes.  This permits us to use the
   macros freely, and know that they will come into play for the
   version of gcc in which they are supported.  */

#if !__GNUC_PREREQ(2,7)
# ifndef __attribute__
#  define __attribute__(x)
# endif
#endif

#undef inline
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 199901L
/* it's a keyword */
#elif __GNUC_PREREQ(2,7)
# define inline __inline__
#else
# define inline
#endif

#ifndef __const
# define __const const
#endif

#define UNUSED_PARAM __attribute__ ((__unused__))
#define NORETURN __attribute__ ((__noreturn__))
#define PACKED __attribute__ ((__packed__))
#define ALIGNED(m) __attribute__ ((__aligned__(m)))

/* __NO_INLINE__: some gcc's do not honor inlining! :( */
#if __GNUC_PREREQ(3,0) && !defined(__NO_INLINE__)
# define ALWAYS_INLINE __attribute__ ((always_inline)) inline
/* I've seen a toolchain where I needed __noinline__ instead of noinline */
# define NOINLINE      __attribute__((__noinline__))
# if !ENABLE_WERROR
#  define DEPRECATED __attribute__ ((__deprecated__))
#  define UNUSED_PARAM_RESULT __attribute__ ((warn_unused_result))
# else
#  define DEPRECATED
#  define UNUSED_PARAM_RESULT
# endif
#else
# define ALWAYS_INLINE inline
# define NOINLINE
# define DEPRECATED
# define UNUSED_PARAM_RESULT
#endif

/* -fwhole-program makes all symbols local. The attribute externally_visible
   forces a symbol global.  */
#if __GNUC_PREREQ(4,1)
# define EXTERNALLY_VISIBLE __attribute__(( visibility("default") ))
//__attribute__ ((__externally_visible__))
#else
# define EXTERNALLY_VISIBLE
#endif

/* We use __extension__ in some places to suppress -pedantic warnings
   about GCC extensions.  This feature didn't work properly before
   gcc 2.8.  */
#if !__GNUC_PREREQ(2,8)
# ifndef __extension__
#  define __extension__
# endif
#endif

/* gcc-2.95 had no va_copy but only __va_copy. */
#if !__GNUC_PREREQ(3,0)
# include <stdarg.h>
# if !defined va_copy && defined __va_copy
#  define va_copy(d,s) __va_copy((d),(s))
# endif
#endif

/* FAST_FUNC is a qualifier which (possibly) makes function call faster
 * and/or smaller by using modified ABI. It is usually only needed
 * on non-static, busybox internal functions. Recent versions of gcc
 * optimize statics automatically. FAST_FUNC on static is required
 * only if you need to match a function pointer's type */
#if __GNUC_PREREQ(3,0) && defined(i386) /* || defined(__x86_64__)? */
/* stdcall makes callee to pop arguments from stack, not caller */
# define FAST_FUNC __attribute__((regparm(3),stdcall))
/* #elif ... - add your favorite arch today! */
#else
# define FAST_FUNC
#endif

/* Make all declarations hidden (-fvisibility flag only affects definitions) */
/* (don't include system headers after this until corresponding pop!) */
#if __GNUC_PREREQ(4,1)
# define PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN _Pragma("GCC visibility push(hidden)")
# define POP_SAVED_FUNCTION_VISIBILITY              _Pragma("GCC visibility pop")
#else
# define PUSH_AND_SET_FUNCTION_VISIBILITY_TO_HIDDEN
# define POP_SAVED_FUNCTION_VISIBILITY
#endif

/* ---- Endian Detection ------------------------------------ */

#if defined(__digital__) && defined(__unix__)
# include <sex.h>
# define __BIG_ENDIAN__ (BYTE_ORDER == BIG_ENDIAN)
# define __BYTE_ORDER BYTE_ORDER
#elif defined __FreeBSD__
char *strchrnul(const char *s, int c);
# include <sys/resource.h>	/* rlimit */
# include <machine/endian.h>
# define bswap_64 __bswap64
# define bswap_32 __bswap32
# define bswap_16 __bswap16
# define __BIG_ENDIAN__ (_BYTE_ORDER == _BIG_ENDIAN)
#elif !defined __APPLE__
# include <byteswap.h>
# include <endian.h>
#endif

#if defined(__BIG_ENDIAN__) && __BIG_ENDIAN__
# define BB_BIG_ENDIAN 1
# define BB_LITTLE_ENDIAN 0
#elif __BYTE_ORDER == __BIG_ENDIAN
# define BB_BIG_ENDIAN 1
# define BB_LITTLE_ENDIAN 0
#elif __BYTE_ORDER == __LITTLE_ENDIAN
# define BB_BIG_ENDIAN 0
# define BB_LITTLE_ENDIAN 1
#else
# error "Can't determine endiannes"
#endif

/* SWAP_LEnn means "convert CPU<->little_endian by swapping bytes" */
#if BB_BIG_ENDIAN
# define SWAP_BE16(x) (x)
# define SWAP_BE32(x) (x)
# define SWAP_BE64(x) (x)
# define SWAP_LE16(x) bswap_16(x)
# define SWAP_LE32(x) bswap_32(x)
# define SWAP_LE64(x) bswap_64(x)
#else
# define SWAP_BE16(x) bswap_16(x)
# define SWAP_BE32(x) bswap_32(x)
# define SWAP_BE64(x) bswap_64(x)
# define SWAP_LE16(x) (x)
# define SWAP_LE32(x) (x)
# define SWAP_LE64(x) (x)
#endif

/* ---- Unaligned access ------------------------------------ */

/* NB: unaligned parameter should be a pointer, aligned one -
 * a lvalue. This makes it more likely to not swap them by mistake
 */
#if defined(i386) || defined(__x86_64__)
# define move_from_unaligned_int(v, intp) ((v) = *(int*)(intp))
# define move_from_unaligned16(v, u16p) ((v) = *(uint16_t*)(u16p))
# define move_from_unaligned32(v, u32p) ((v) = *(uint32_t*)(u32p))
# define move_to_unaligned32(u32p, v)   (*(uint32_t*)(u32p) = (v))
/* #elif ... - add your favorite arch today! */
#else
/* performs reasonably well (gcc usually inlines memcpy here) */
# define move_from_unaligned_int(v, intp) (memcpy(&(v), (intp), sizeof(int)))
# define move_from_unaligned16(v, u16p) (memcpy(&(v), (u16p), 2))
# define move_from_unaligned32(v, u32p) (memcpy(&(v), (u32p), 4))
# define move_to_unaligned32(u32p, v) do { \
	uint32_t __t = (v); \
	memcpy((u32p), &__t, 4); \
} while (0)
#endif

/* ---- Networking ------------------------------------------ */

#ifndef __APPLE__
# include <arpa/inet.h>
# if !defined(__socklen_t_defined) && !defined(_SOCKLEN_T_DECLARED)
typedef int socklen_t;
# endif
#else
# include <netinet/in.h>
#endif

/* ---- Compiler dependent settings ------------------------- */

#if (defined __digital__ && defined __unix__) \
 || defined __APPLE__ || defined __FreeBSD__
# undef HAVE_MNTENT_H
# undef HAVE_SYS_STATFS_H
#else
# define HAVE_MNTENT_H 1
# define HAVE_SYS_STATFS_H 1
#endif

/*----- Kernel versioning ------------------------------------*/

#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

/* ---- Miscellaneous --------------------------------------- */

#if defined(__GNU_LIBRARY__) && __GNU_LIBRARY__ < 5 && \
	!defined(__dietlibc__) && \
	!defined(_NEWLIB_VERSION) && \
	!(defined __digital__ && defined __unix__)
# error "Sorry, this libc version is not supported :("
#endif

/* Don't perpetuate e2fsck crap into the headers.  Clean up e2fsck instead. */

#if defined __GLIBC__ || defined __UCLIBC__ \
 || defined __dietlibc__ || defined _NEWLIB_VERSION
# include <features.h>
# define HAVE_FEATURES_H
# include <stdint.h>
# define HAVE_STDINT_H
#elif !defined __APPLE__
/* Largest integral types. */
# if BB_BIG_ENDIAN
/* Looks BROKEN! */
typedef long                intmax_t;
typedef unsigned long       uintmax_t;
# else
__extension__
typedef long long           intmax_t;
__extension__
typedef unsigned long long  uintmax_t;
# endif
#endif

/* Size-saving "small" ints (arch-dependent) */
#if defined(i386) || defined(__x86_64__) || defined(__mips__) || defined(__cris__)
/* add other arches which benefit from this... */
typedef signed char smallint;
typedef unsigned char smalluint;
#else
/* for arches where byte accesses generate larger code: */
typedef int smallint;
typedef unsigned smalluint;
#endif

/* ISO C Standard:  7.16  Boolean type and values  <stdbool.h> */
#if (defined __digital__ && defined __unix__)
/* old system without (proper) C99 support */
# define bool smalluint
#else
/* modern system, so use it */
# include <stdbool.h>
#endif

/* Try to defeat gcc's alignment of "char message[]"-like data */
#if 1 /* if needed: !defined(arch1) && !defined(arch2) */
# define ALIGN1 __attribute__((aligned(1)))
# define ALIGN2 __attribute__((aligned(2)))
#else
/* Arches which MUST have 2 or 4 byte alignment for everything are here */
# define ALIGN1
# define ALIGN2
#endif


/* uclibc does not implement daemon() for no-mmu systems.
 * For 0.9.29 and svn, __ARCH_USE_MMU__ indicates no-mmu reliably.
 * For earlier versions there is no reliable way to check if we are building
 * for a mmu-less system.
 */
#if ENABLE_NOMMU || \
    (defined __UCLIBC__ && __UCLIBC_MAJOR__ >= 0 && __UCLIBC_MINOR__ >= 9 && \
    __UCLIBC_SUBLEVEL__ > 28 && !defined __ARCH_USE_MMU__)
# define BB_MMU 0
# define USE_FOR_NOMMU(...) __VA_ARGS__
# define USE_FOR_MMU(...)
#else
# define BB_MMU 1
# define USE_FOR_NOMMU(...)
# define USE_FOR_MMU(...) __VA_ARGS__
#endif

/* Platforms that haven't got dprintf need to implement fdprintf() in
 * libbb.  This would require a platform.c.  It's not going to be cleaned
 * out of the tree, so stop saying it should be. */
#if !defined(__dietlibc__)
/* Needed for: glibc */
/* Not needed for: dietlibc */
/* Others: ?? (add as needed) */
# define fdprintf dprintf
#endif

#if defined(__dietlibc__)
static ALWAYS_INLINE char* strchrnul(const char *s, char c)
{
	while (*s && *s != c) ++s;
	return (char*)s;
}
#endif

/* Don't use lchown with glibc older than 2.1.x */
#if defined(__GLIBC__) && __GLIBC__ <= 2 && __GLIBC_MINOR__ < 1
# define lchown chown
#endif

#if defined(__digital__) && defined(__unix__)

# include <standards.h>
# include <inttypes.h>
# define PRIu32 "u"
/* use legacy setpgrp(pid_t,pid_t) for now.  move to platform.c */
# define bb_setpgrp() do { pid_t __me = getpid(); setpgrp(__me,__me); } while (0)
# if !defined ADJ_OFFSET_SINGLESHOT && defined MOD_CLKA && defined MOD_OFFSET
#  define ADJ_OFFSET_SINGLESHOT (MOD_CLKA | MOD_OFFSET)
# endif
# if !defined ADJ_FREQUENCY && defined MOD_FREQUENCY
#  define ADJ_FREQUENCY MOD_FREQUENCY
# endif
# if !defined ADJ_TIMECONST && defined MOD_TIMECONST
#  define ADJ_TIMECONST MOD_TIMECONST
# endif
# if !defined ADJ_TICK && defined MOD_CLKB
#  define ADJ_TICK MOD_CLKB
# endif

#else

# define bb_setpgrp() setpgrp()

#endif


#endif
