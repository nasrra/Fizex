#include <assert.h>
/*
    Supported by MSVC, GCC, and Clang
*/
#include <immintrin.h>
#include <stdint.h>
#include <float.h>
/*
    used in:
    - base_memory
    - base_linux
*/
#include <string.h>
#include <time.h>
#include <stdio.h>

/*========================================
    types.
========================================*//**/

typedef int8_t bool;
#define false 0
#define true 1
typedef float f32;
typedef double f64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/*========================================
    defines.
========================================*//**/

/*
    Context Cracking.
*//**/

#if defined(__clang__)
#   define COMPILER_CLANG 1
#   if defined(_WIN32)
#       define OS_WINDOWS 1
#   elif defined(__gnu_linux__)
#       define OS_LINUX 1
#   elif defined(__APPLE__) && defined(__MACH__)
#       define OS_MAC 1
#   else
#       error missing OS detection.
#   endif
#   if defined(__amd64__)
#       define ARCH_X64 1
#   elif defined(__i386__)
#       define ARCH_X86 1
#   elif defined(__arm__)
#       define ARCH_ARM
#   elif defined(__aarch64__)
#       define ARCH_ARM64 1
#   else
#       error missing ARCH detection.
#   endif
#elif defined(__GNUC__)
#   define COMPILER_GCC 1
#   if defined(_WIN32)
#       define OS_WINDOWS 1
#   elif defined(__gnu_linux__)
#       define OS_LINUX 1
#   elif defined(__APPLE__) && defined(__MACH__)
#       define OS_MAC 1
#   else
#       error missing OS detection.
#   endif
#   if defined(__amd64__)
#       define ARCH_X64 1
#   elif defined(__i386__)
#       define ARCH_X86 1
#   elif defined(__arm__)
#       define ARCH_ARM
#   elif defined(__aarch64__)
#       define ARCH_ARM64 1
#   else
#       error missing ARCH detection.
#   endif
/*
    make sure to keep this last as windows msvc would always be present regardless of another compiler being used.
    microsoft is awesome :)))
*/
#elif defined(_MSC_VER) 
#   define COMPILER_CL 1
#   if defined(_WIN32)
#       define OS_WINDOWS 1
#   else
#       error missing OS detection.
#   endif
#   if defined(_M_AMD64)
#       define ARCH_X64 1
#   elif defined(_M_I86)
#       define ARCH_X86 1
#   elif defined(_M_ARM)
#       define ARCH_ARM 1
#   else
#       error missing ARCH detection.
#   endif
#endif
#if !defined(COMPILER_CLANG)
#   define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
#   define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CL)
#   define COMPILER_C 0
#endif
#if !defined(OS_WINDOWS)
#   define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
#   define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#   define OS_MAC 0
#endif
#if !defined(ARCH_X64)
#   define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#   define ARCH_X86 0
#endif
#if !defined(ARCH_X86)
#   define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
#   define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#   define ARCH_ARM64 0
#endif

#define ArrayLength(arr) (sizeof(arr)/sizeof(*(arr)))
#define PtrArraySize(ptr, length) (length * sizeof(*(ptr)))

#ifdef NDEBUG
#   define DEBUG_ASSERT(val, msg)
#else
#   if OS_WINDOWS
#        define DEBUG_ASSERT(val, msg) \
            ((void)((val) || (_wassert(L##msg, _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)))
#   elif OS_LINUX
#       define DEBUG_ASSERT(val, msg) \
            ((void)((val) || (__assert_fail(msg, __FILE__, __LINE__, __func__), 0)))
#   elif OS_MAC
#       define DEBUG_ASSERT(val, msg) \
           ((void)((val) || (__assert_rtn(__func__, __FILE__, __LINE__, msg), 0)))
#   else
    // fallback.
#       define DEBUG_ASSERT(val, msg) do { \
            assert(val); \
        } while(0)
#   endif
#endif

#ifdef NDEBUG
#   define BND_CHCK(val, size)
#else
#   define BNDS_CHCK(val, size) do { \
        assert(val >= 0 && val < size); \
    } while(0)
#endif

#define foo FLT_MIN

/* 
    Minimum of signed integral types.  
*/
#define I8_MIN  (-128)
#define I16_MIN (-32767-1)
#define I32_MIN (-2147483647-1)
#define I64_MIN (-__INT64_C(9223372036854775807)-1)

/* 
    Maximum of signed integral types.  
*/
#define I8_MAX  (127)
#define I16_MAX (32767)
#define I32_MAX (2147483647)
#define I64_MAX (__INT64_C(9223372036854775807))

/* 
    Maximum of unsigned integral types.  
*/
#define U8_MAX  (255)
#define U16_MAX (65535)
#define U32_MAX (4294967295U)
#define U64_MAX (__UINT64_C(18446744073709551615))

/*
    maximum of floating-point types.
*/
#define F32_MAX 3.40282347e+38F
#define F64_MAX 1.7976931348623157e+308

/*
    minimum of floating-point types.
*/
#define F32_MIN 1.17549435e-38F
#define F64_MIN 2.2250738585072014e-308

/*
    epsilon of floating-point types.
*/
#define F32_EPSILON 1.19209290e-7F
#define F64_EPSILON 2.2204460492503131e-16

/*========================================
    globals.
========================================*//**/
/*
    base_rand_seed is lazy init.
*/
i32 base_rand_state;
bool base_rand_initial_state_set;

/*========================================
    functions
========================================*//**/

/*
    sets the global random seed to a new seed.

    returns:
    the newl set seed. 
*/
i32 rand_next_seed(){
    // get the current calender time.
    time_t raw_time;
    time(&raw_time);
    struct tm *t = localtime(&raw_time);

    i32 second  = t->tm_sec;
    i32 minute  = t->tm_min;
    i32 hour    = t->tm_hour;
    i32 day     = t->tm_mday;
    i32 month   = t->tm_mon+1; // +1 for 1-12; not 0-11
    i32 year    = t->tm_year+1980; // years since 1980.

    i32 result; 
    result += second;
    result += minute;
    result += hour;
    result += day;
    result += month;
    result += year;
    base_rand_state = result;
    return result;
}

i32 rand_i32(){
    
    // lazy init the first seed.
    if(base_rand_initial_state_set == false){
        rand_next_seed();
        base_rand_initial_state_set = true;
    }
   
    i32 result = base_rand_state;
	result ^= result << 13;
	result ^= result >> 17;
	result ^= result << 5;
    base_rand_state = result;
	return result;
}

f32 rand_f32(){
    f32 r = (f32)rand_i32();
    return r * r;
}

i32 srand_i32(i32 seed){
    base_rand_state = seed;
    base_rand_initial_state_set = true;
    return rand_i32();
}

f32 srand_f32(i32 seed){
    base_rand_state = seed;
    base_rand_initial_state_set = true;
    return rand_f32();
}