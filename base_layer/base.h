#ifndef BASE_H
#define BASE_H

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

typedef struct{
    char* chars;
    i32 size;
} String;

typedef struct{
    void* ptr;
    u64 size;
    u64 stride;
} MemoryArena;

typedef u32 GenId;

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

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(*(arr)))
#define PTR_ARRAY_SIZE(ptr, length) (length * sizeof(*(ptr)))

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
#   define BOUNDS_CHECK(val, size)
#else
#   define BOUNDS_CHECK(val, size) do { \
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
#define ZERO_MEMORY(ptr, size) memset(ptr, 0, size)
#define COPY_MEMORY(dst, src, size) memcpy(dst, src, size)
/*
    zeroes out an array. 
*/
#define ARRAY_ZERO(array, length) ZERO_MEMORY(array, PTR_ARRAY_SIZE(array, length))
#define GIGABYTE(val) MEGABYTE(val) * 1024
#define MEGABYTE(val) KILOBYTE(val) * 1024
#define KILOBYTE(val) val * 1024

/*
    Example:

    MemoryArena arena;
    int* ints = (int[]){1,2,3};
    PUSH_ARRAY_MEMARENA(&arena, ints, 3).

        OR

    MemoryArena arena;
    int ints[] = {1,2,3};
    PUSH_ARRAY_MEMARENA(&arena, ints, 3).
*/
#define PUSH_ARRAY_MEMORY_ARENA(arena, data, array_size) do { \
    DEBUG_ASSERT(data != NULL, "attempted to push a nullptr onto a memory arena."); \
    size_t size_PUSH_ARRAY_MEMARENA = sizeof(*data) * (array_size); \
    size_t new_stride_PUSH_ARRAY_MEMARENA = (arena)->stride + size; \
    if(new_stride <= (arena)->size){ \
        void* dst = (arena)->ptr + (arena)->stride; \
        COPY_MEMORY(dst, data, size_PUSH_ARRAY_MEMARENA); \
        (arena)->stride = new_stride_PUSH_ARRAY_MEMARENA; \
    } \
    else{ \
        DEBUG_ASSERT(0!=0, "insufficient space to push data onto memory arena."); \
    } \
} while(0)

/*
    Example:

    MemoryArena arena;
    Data data = {0};
    PUSH_STRUCT_MEMARENA(&arena, data);
*/
#define PUSH_STRUCT_MEMORY_ARENA(arena, data) do { \
    size_t size_PUSH_STRUCT_MEMARENA = sizeof(data); \
    size_t new_stride_PUSH_STRUCT_MEMARENA = (arena)->stride + size; \
    if(new_stride_PUSH_STRUCT_MEMARENA <= (arena)->size){ \
        void* dst = (arena)->ptr + (arena)->stride; \
        COPY_MEMORY(dst, &(data), size_PUSH_STRUCT_MEMARENA); \
        (arena)->stride = new_stride; \
    } \
    else{ \
        DEBUG_ASSERT(0!=0, "insufficient space to push data onto memory arena."); \
    } \
} while(0)


/*
    Example:

    MemoryArena arena;
    Data* datas;
    i32 datas_size;
    ALLOC_ARRAY_MEMARENA(&arena, datas, &datas_size, 2);
*/
#define MEMORY_ARENA_ALLOC_ARRAY(arena, out_arr_ptr, out_arr_size, array_size) do { \
    size_t size_ALLOC_ARRAY_MEMARENA = sizeof(*out_arr_ptr) * (array_size); \
    size_t new_stride_ALLOC_ARRAY_MEMARENA = (arena)->stride + size_ALLOC_ARRAY_MEMARENA; \
    if(new_stride_ALLOC_ARRAY_MEMARENA <= (arena)->size){ \
        (out_arr_ptr) = (void*)((u8*)(arena)->ptr + (arena)->stride); \
        (arena)->stride = new_stride_ALLOC_ARRAY_MEMARENA; \
        *(out_arr_size) = array_size; \
    } \
    else{ \
        DEBUG_ASSERT(0!=0, "insufficient space to alloc array onto memory arena."); \
        *(out_arr_size) = 0; \
    } \
} while(0)

/*
    the bit-wise mask used to extract the generation value from the gen id.
*/
// binary: 0000 0000 0000 0000 0000 1111 1111 1111.
#define GENID_GENERATION_MASK 0xFFF
/*
    the bit-wise mask used to extract the index value from the gen id.
*/
// binary: 0000 0000 0000 1111 1111 1111 1111 1111.
#define GENID_INDEX_MASK 0xFFFFF
/*
    the total amount of unique generation values starting from zero.

    remarks:
    This is because the last 12 bits of a uint are used for the generational value.
*/
#define GENID_UNIQUE_GENERATIONS_COUNT (1<<12)
/*
    the max generation value a gen id can have.
    
    remarks:
    this is because the last 12 bits of a uint are used for the generational value.
*/
#define GENID_MAX_GENERATION (GENID_UNIQUE_GENERATIONS_COUNT - 1)
/*
    the total amount of unique index values starting from zero.
    
    remarks:
    this is because the first 20 bits of a uint are used for a indexing. 
*/
#define GENID_UNIQUE_INDICES_COUNT (1<<20)
/*
    the max index value a genId can have.
    
    remarks:
    this is because the first 20 bits of a uint are used for a indexing. 
*/
#define GENID_MAX_INDEX (GENID_UNIQUE_INDICES_COUNT - 1)

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
i32 rand_next_seed();
i32 rand_i32();
f32 rand_f32();
i32 srand_i32(i32 seed);
f32 srand_f32(i32 seed);

bool memory_arena_partition(MemoryArena* parent, MemoryArena* out_child, u64 size);
void clear_memory_arena(MemoryArena* arena);
void clear_zeroed_memory_arena(MemoryArena *arena);
void free_memory_arena(MemoryArena* arena);

/*
    Constructs a GenId.
    
    remarks:
    - `index` must be between 0 and 1,048,576. 
    - `generation` must be between 0 and 4,096.
*/
GenId genid_make(i32 index, i32 generation);
/*
    Increments the generational value of a gen id by one.

    remarks:
    the generational slice of the integer will be wrapped around automatically when exceeding its max value.
*/
GenId genid_increment_generation(GenId genid);
/*
    Increments the index value of a gen id by one.
*/
GenId genid_increment_index(GenId genid);
/*
    Gets the index value that is packed inside a gen id.
*/
i32 genid_get_index(GenId genid);
/*
    Gets the generation value that is packed inside of a gen id.
*/
i32 genid_get_generation(GenId genid);
/*
    Checks if two gen ids are equal. 
*/
bool genid_equals(GenId a, GenId b); 

void string_init(String* string, MemoryArena* arena, i32 size);

#endif