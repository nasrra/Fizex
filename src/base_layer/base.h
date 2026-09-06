#ifndef BASE_H
#define BASE_H

#include <stdlib.h>
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
typedef long long u128;

typedef struct{
    char* chars;
    i32 length;
    i32 count;
} String;

typedef struct{
    void* ptr;
    u64 size;
    u64 stride;
} MemoryArena;

typedef u32 GenId;

typedef struct{
    GenId* gen_ids;
    bool* allocated;
    i32 length;
    i32* free_slots_stack;
    i32 free_slots_stack_count;
    bool is_init;
} GenIdAllocator;

/*========================================
    defines.
========================================*//**/

#define false 0
#define true 1

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

#ifdef PLATFORM_H
#   define ABORT(msg) do { platform_error_message_box(msg); abort(); } while(0)
#else
#   define ABORT(msg) do { abort(); } while(0)
#endif

#define PANIC(condition, msg) do { if((condition)==false){ABORT(msg);} } while(0)

#ifdef NDEBUG
#define ASSERT(condition, msg)
#define BOUNDS_CHECK(val, length)
#define NIL_BOUNDS_CHECK(val, length)
#else
#define ASSERT(condition, msg) (assert(msg && (condition)))
#define BOUNDS_CHECK(val, length) do { \
        ASSERT(val >= 0 && val < length, "Index Out Of Bounds."); \
    } while(0)
#define NIL_BOUNDS_CHECK(val, length) do { \
        ASSERT(val > 0 && val < length, "Index Out Of Bounds."); \
    } while(0)
#endif

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
#define ZERO_STRUCT(ptr) memset(ptr, 0, sizeof(*(ptr)))
#define COPY_MEMORY(DST, SRC, SIZE_IN_BYTES) memcpy(DST, SRC, SIZE_IN_BYTES)
/*
    zeroes out an array.
*/
#define GIGABYTE(val) MEGABYTE(val) * 1024
#define MEGABYTE(val) KILOBYTE(val) * 1024
#define KILOBYTE(val) val * 1024

/**
    arrays.
**/
#define PTR_ARRAY_SIZE(ptr, length) (length * sizeof(*(ptr)))

#define ARRAY_ZERO(array, length) ZERO_MEMORY(array, PTR_ARRAY_SIZE(array, length))
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(*(arr)))
/**
    example:

    i32* array = (i32[1]){};
    i32 length = 1;
    i32 count = 0;
    i32 value = 22;
    ARRAY_PUSH(array, length, &count, value);
**/
#define ARRAY_PUSH(array, array_length, array_count, value) do {    \
    BOUNDS_CHECK(*array_count, array_length);                       \
    array[*array_count] = value;                                    \
    *array_count += 1;                                              \
} while (0)
/**
    example:

    i32* array = (i32[1]){1};
    i32 length = 1;
    i32 count = 1;
    i32 out_value;
    ARRAY_POP(array, length, &count, &out_value);
**/
#define ARRAY_POP(array, array_length, array_count, out_value) do {         \
    *array_count -= 1;                                                      \
    BOUNDS_CHECK(*array_count, array_length);                               \
    if(*array_count < 0){                                                   \
        *array_count += 1;                                                  \
    }                                                                       \
    else{                                                                   \
        *out_value = array[*array_count];                                    \
    }                                                                       \
} while (0)
/**
    `remarks`
    when removing a value from an array; all elements after the removed slot are shifted towards element zero.

    `example`

    i32* nums = (i32[]){1,2,3,4};
    i32 nums_length = 4;
    i32 nums_count = 4;
    ARRAY_ORDERED_REMOVE_AT(nums, nums_length, &nums_count, 2); // results in {1,2,4,4}
**/
#define ARRAY_ORDERED_REMOVE_AT(arr_ptr, arr_length, arr_count, element_index) do {                                                         \
    BOUNDS_CHECK((element_index), arr_length);                                                                                              \
    BOUNDS_CHECK((element_index), *(arr_count));                                                                                            \
    COPY_MEMORY((arr_ptr) + (element_index), (arr_ptr) + (element_index) + 1, sizeof(*arr_ptr) * (*(arr_count) - ((element_index) + 1)));   \
    *arr_count -= 1;                                                                                                                        \
} while(0)
/**
    `remarks`
    When removing a value from the buffer; the data of the last element in the buffer is copied into the element that is being removed.
**/
#define ARRAY_UNORDERED_REMOVE_AT(arr_ptr, arr_length, arr_count, element_index) do{    \
    BOUNDS_CHECK((element_index), (arr_length));                                        \
    BOUNDS_CHECK((element_index), *(arr_count));                                        \
    *(arr_count) -= 1;                                                                  \
    (arr_ptr)[(element_index)] = (arr_ptr)[*(arr_count)];                               \
} while(0)
/**
    `remarks`
    When inserting a value into an element, any data the was previously in that element is
    shifted forward - away from element zero - including all elements after the inserted element.
**/
#define ARRAY_ORDERED_INSERT(arr_ptr, arr_length, arr_count, element_index, value) do{                                                       \
    BOUNDS_CHECK((element_index), (arr_length));                                                                                            \
    BOUNDS_CHECK((element_index), *(arr_count));                                                                                            \
    BOUNDS_CHECK(*(arr_count), (arr_length));                                                                                               \
    COPY_MEMORY((arr_ptr) + (element_index) + 1, (arr_ptr) + (element_index), sizeof(*arr_ptr) * (*(arr_count) - ((element_index) - 1)));   \
    arr_ptr[element_index] = value;                                                                                                         \
} while(0)
/**
`remarks`
    When inserting a value into an element, any data the was previously in that element is sent forward - away from element zero.
**/
#define ARRAY_UNORDERED_INSERT(arr_ptr, arr_length, arr_count, element_index, value) do{    \
    BOUNDS_CHECK((element_index), (arr_length));                                            \
    BOUNDS_CHECK((element_index), *(arr_count));                                            \
    BOUNDS_CHECK(*(arr_count), (arr_length));                                               \
    arr_ptr[*arr_count] = arr_ptr[element_index];                                           \
    *arr_count += 1;                                                                        \
    arr_ptr[element_index] = value;                                                         \
} while(0)

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
#define MEMORY_ARENA_PUSH_ARRAY(arena, data, array_size) do {                                   \
    DEBUG_ASSERT(data != NULL, "attempted to push a nullptr onto a memory arena.");             \
    size_t MEMORY_ARENA_PUSH_ARRAY_size = sizeof(*data) * (array_size);                         \
    size_t MEMORY_ARENA_PUSH_ARRAY_new_stride = (arena)->stride + MEMORY_ARENA_PUSH_ARRAY_size; \
    if(MEMORY_ARENA_PUSH_ARRAY_new_stride <= (arena)->size){                                    \
        void* dst = (void*)((u8*)(arena)->ptr + (arena)->stride);                               \
        COPY_MEMORY(dst, data, MEMORY_ARENA_PUSH_ARRAY_size);                                   \
        (arena)->stride = MEMORY_ARENA_PUSH_ARRAY_new_stride;                                   \
    }                                                                                           \
    else{                                                                                       \
        PANIC(false, "insufficient space to push data onto memory arena.");                     \
    }                                                                                           \
} while(0)

/*
    Example:

    MemoryArena arena;
    Data data = {0};
    PUSH_STRUCT_MEMARENA(&arena, data);
*/
#define MEMORY_ARENA_PUSH_STRUCT(arena, data) do {                                                  \
                                                                                                    \
    size_t MEMORY_ARENA_PUSH_STRUCT_size = sizeof(data);                                            \
    size_t MEMORY_ARENA_PUSH_STRUCT_new_stride = (arena)->stride + MEMORY_ARENA_PUSH_STRUCT_size;   \
    if(MEMORY_ARENA_PUSH_STRUCT_new_stride <= (arena)->size){                                       \
        void* dst = (void*)((u8*)(arena)->ptr + (arena)->stride);                                   \
        COPY_MEMORY(dst, &(data), MEMORY_ARENA_PUSH_STRUCT_size);                                   \
        (arena)->stride = MEMORY_ARENA_PUSH_STRUCT_new_stride;                                      \
    }                                                                                               \
    else{                                                                                           \
        PANIC(false, "insufficient space to push data onto memory arena.");                         \
    }                                                                                               \
} while(0)


/*
    Example:

    MemoryArena arena;
    Data* datas;
    i32 datas_size;
    ALLOC_ARRAY_MEMARENA(&arena, datas, &datas_size, 2);
*/
#define MEMORY_ARENA_ALLOC_ARRAY(arena, out_arr_ptr, out_arr_length, array_length) do {             \
                                                                                                    \
    size_t MEMORY_ARENA_ALLOC_ARRAY_size = sizeof(*out_arr_ptr) * (array_length);                   \
    size_t MEMORY_ARENA_ALLOC_ARRAY_new_stride = (arena)->stride + MEMORY_ARENA_ALLOC_ARRAY_size;   \
    if(MEMORY_ARENA_ALLOC_ARRAY_new_stride <= (arena)->size){                                       \
        (out_arr_ptr) = (void*)((u8*)(arena)->ptr + (arena)->stride);                               \
        (arena)->stride = MEMORY_ARENA_ALLOC_ARRAY_new_stride;                                      \
        *(out_arr_length) = array_length;                                                           \
    }                                                                                               \
    else{                                                                                           \
        PANIC(false, "insufficient space to push data onto memory arena.");                         \
        *(out_arr_length) = 0;                                                                      \
    }                                                                                               \
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

#define DEFINE_QUICKSORT_STRUCT(STRUCT_TYPE, MEMBER_TYPE, MEMBER_ACCESSOR, FUNCTION_NAME)   \
                                                                                            \
static void FUNCTION_NAME##_recursion_asc(STRUCT_TYPE* array_ptr, i32 low, i32 high){       \
    if(low >= high){                                                                        \
        return;                                                                             \
    }                                                                                       \
    MEMBER_TYPE pivot = array_ptr[high] MEMBER_ACCESSOR;                                    \
    i32 i = low;                                                                            \
    for(i32 j = low; j < high; j++){                                                        \
        if(array_ptr[j] MEMBER_ACCESSOR <= pivot){                                          \
            STRUCT_TYPE temp = array_ptr[i];                                                \
            array_ptr[i] = array_ptr[j];                                                    \
            array_ptr[j] = temp;                                                            \
            i++;                                                                            \
        }                                                                                   \
    }                                                                                       \
    STRUCT_TYPE temp = array_ptr[i];                                                        \
    array_ptr[i] = array_ptr[high];                                                         \
    array_ptr[high] = temp;                                                                 \
    if(i > low){FUNCTION_NAME##_recursion_asc(array_ptr, low, i - 1);}                      \
    if(i < high){FUNCTION_NAME##_recursion_asc(array_ptr, i + 1, high);}                    \
}                                                                                           \
                                                                                            \
static void FUNCTION_NAME##_asc(STRUCT_TYPE* array_ptr, i32 array_length){                  \
    i32 high = array_length-1;                                                              \
    if(high > 0 && high < array_length){                                                    \
        FUNCTION_NAME##_recursion_asc(array_ptr, 0, array_length-1);                        \
    }                                                                                       \
}                                                                                           \
                                                                                            \
static void FUNCTION_NAME##_recursion_dsc(STRUCT_TYPE* array_ptr, i32 low, i32 high){       \
    if(low >= high){                                                                        \
        return;                                                                             \
    }                                                                                       \
    MEMBER_TYPE pivot = array_ptr[high] MEMBER_ACCESSOR;                                    \
    i32 i = low;                                                                            \
    for(i32 j = low; j < high; j++){                                                        \
        if(array_ptr[j] MEMBER_ACCESSOR >= pivot){                                          \
            STRUCT_TYPE temp = array_ptr[i];                                                \
            array_ptr[i] = array_ptr[j];                                                    \
            array_ptr[j] = temp;                                                            \
            i++;                                                                            \
        }                                                                                   \
    }                                                                                       \
    STRUCT_TYPE temp = array_ptr[i];                                                        \
    array_ptr[i] = array_ptr[high];                                                         \
    array_ptr[high] = temp;                                                                 \
    if(i > low){FUNCTION_NAME##_recursion_dsc(array_ptr, low, i - 1);}                      \
    if(i < high){FUNCTION_NAME##_recursion_dsc(array_ptr, i + 1, high);}                    \
}                                                                                           \
                                                                                            \
static void FUNCTION_NAME##_dsc(STRUCT_TYPE* array_ptr, i32 array_length){                  \
    i32 high = array_length-1;                                                              \
    if(high > 0 && high < array_length){                                                    \
        FUNCTION_NAME##_recursion_dsc(array_ptr, 0, array_length-1);                        \
    }                                                                                       \
}

#define DEFINE_QUICKSORT(TYPE, FUNCTION_NAME)                                   \
                                                                                \
static void FUNCTION_NAME##_recursion_asc(TYPE* array_ptr, i32 low, i32 high){  \
    if(low >= high){                                                            \
        return;                                                                 \
    }                                                                           \
    TYPE pivot = array_ptr[high];                                               \
    i32 i = low;                                                                \
    for(i32 j = low; j < high; j++){                                            \
        if(array_ptr[j] <= pivot){                                              \
            TYPE temp = array_ptr[i];                                           \
            array_ptr[i] = array_ptr[j];                                        \
            array_ptr[j] = temp;                                                \
            i++;                                                                \
        }                                                                       \
    }                                                                           \
    TYPE temp = array_ptr[i];                                                   \
    array_ptr[i] = array_ptr[high];                                             \
    array_ptr[high] = temp;                                                     \
    if(i > low){FUNCTION_NAME##_recursion_asc(array_ptr, low, i - 1);}          \
    if(i < high){FUNCTION_NAME##_recursion_asc(array_ptr, i + 1, high);}        \
}                                                                               \
                                                                                \
static void FUNCTION_NAME##_recursion_dsc(TYPE* array_ptr, i32 low, i32 high){  \
    if(low >= high){                                                            \
        return;                                                                 \
    }                                                                           \
    TYPE pivot = array_ptr[high];                                               \
    i32 i = low;                                                                \
    for(i32 j = low; j < high; j++){                                            \
        if(array_ptr[j] >= pivot){                                              \
            TYPE temp = array_ptr[i];                                           \
            array_ptr[i] = array_ptr[j];                                        \
            array_ptr[j] = temp;                                                \
            i++;                                                                \
        }                                                                       \
    }                                                                           \
    TYPE temp = array_ptr[i];                                                   \
    array_ptr[i] = array_ptr[high];                                             \
    array_ptr[high] = temp;                                                     \
    if(i > low){FUNCTION_NAME##_recursion_dsc(array_ptr, low, i - 1);}          \
    if(i < high){FUNCTION_NAME##_recursion_dsc(array_ptr, i + 1, high);}        \
}                                                                               \
                                                                                \
static void FUNCTION_NAME##_asc(TYPE* array_ptr, i32 array_length){             \
    i32 high = array_length-1;                                                  \
    if(high > 0 && high < array_length){                                        \
        FUNCTION_NAME##_recursion_asc(array_ptr, 0, array_length-1);            \
    }                                                                           \
}                                                                               \
                                                                                \
static void FUNCTION_NAME##_dsc(TYPE* array_ptr, i32 array_length){             \
    i32 high = array_length-1;                                                  \
    if(high > 0 && high < array_length){                                        \
        FUNCTION_NAME##_recursion_dsc(array_ptr, 0, array_length-1);            \
    }                                                                           \
}

#define GEN_ID_ALLOCATOR_MIN_LENGTH 2
#define GEN_ID_ALLOCATOR_MAX_LENGTH GENID_UNIQUE_INDICES_COUNT

/*========================================
    globals.
========================================*//**/
/*
    base_rand_seed is lazy init.
*/
i32 base_rand_state;
bool base_rand_initial_state_set;
void * (*base_panic_pre_abort_funcptr) (char* msg);

/*========================================
    functions
========================================*//**/

DEFINE_QUICKSORT(i8,  quicksort_i8)
DEFINE_QUICKSORT(i16, quicksort_i16)
DEFINE_QUICKSORT(i32, quicksort_i32)
DEFINE_QUICKSORT(i64, quicksort_i64)
DEFINE_QUICKSORT(i8,  quicksort_u8)
DEFINE_QUICKSORT(i16, quicksort_u16)
DEFINE_QUICKSORT(i32, quicksort_u32)
DEFINE_QUICKSORT(i64, quicksort_u64)
DEFINE_QUICKSORT(f32, quicksort_f32)
DEFINE_QUICKSORT(f64, quicksort_f64)

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
GenId gen_id_make(i32 index, i32 generation);
/*
    Increments the generational value of a gen id by one.

    remarks:
    the generational slice of the integer will be wrapped around automatically when exceeding its max value.
*/
GenId gen_id_increment_generation(GenId gen_id);
/*
    Increments the index value of a gen id by one.
*/
GenId gen_id_increment_index(GenId gen_id);
/*
    Gets the index value that is packed inside a gen id.
*/
i32 gen_id_get_index(GenId gen_id);
/*
    Gets the generation value that is packed inside of a gen id.
*/
i32 gen_id_get_generation(GenId gen_id);
/*
    Checks if two gen ids are equal.
*/
bool gen_id_equals(GenId a, GenId b);

void string_init(String* string, MemoryArena* arena, i32 size);
void string_clear(String* string);
void string_push_chars(String* string, char* chars, i32 chars_length);
void string_push(String* dst, String src);

void gen_id_allocator_init(GenIdAllocator* allocator, MemoryArena* arena, i32 length);
/**
    Allocates an gen id from a allocator instance.

    `returns`
    the newly allocated GenId; otherwise (GenId){0} if there are no slots available.
**/
GenId gen_id_allocator_alloc(GenIdAllocator* allocator);
/**
    Deallocates an gen id from a allocator instance.

    `remarks`
    stale gen id checks are not enforced; the id will always run through the deallocation procedure.
**/
void gen_id_allocator_dealloc_unsafe(GenIdAllocator* allocator, i32 index);
/**
    Deallocates an gen id from a allocator instance.
**/
bool gen_id_allocator_dealloc(GenIdAllocator* allocator, GenId gen_id);
bool gen_id_allocator_is_gen_id_invalid(GenIdAllocator* allocator, GenId gen_id);

#endif