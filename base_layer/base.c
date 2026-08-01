#include <assert.h>
#include <immintrin.h> // Supported by MSVC, GCC, and Clang
#include <stdint.h>
#include <float.h>

/*========================================
    types.
========================================*//**/

typedef int bool;
#define false 0;
#define true 1;
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
    #define DebugAssert(val, msg)
#else
    #define DebugAssert(val, msg) do { \
        assert(val, msg); \
    } while(0)
#endif
#ifdef NDEBUG
    #define BoundsCheck(val)
#else
    #define BoundsCheck(val, len) do { \
        assert(val >= 0 && val < len); \
    } while(0)
#endif