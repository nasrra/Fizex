#if ARCH_X86 || ARCH_X64
#   include <immintrin.h> 
#   if COMPILER_CL
#       include <intrin.h>
#   elif COMPILER_GCC || COMPILER_CLANG
#       include <cpuid.h>
#   endif
#elif
/*
    (todo):
    fallback for ARM and APPLE SILICON.
*/  
#   error not implemented for cpu architecture.
#endif

/*========================================
    types.
========================================*//**/

typedef void (*simd_f32_funcptr)(const f32* lhs, const f32* rhs, f32* dst, size_t size);
typedef void (*simd_f32_val_funcptr)(const f32* lhs, const f32 rhs, f32* dst, size_t size);

typedef void (*simd_f64_funcptr)(const f64* lhs, const f64* rhs, f64* dst, size_t size);
typedef void (*simd_f64_val_funcptr)(const f64* lhs, const f64 rhs, f64* dst, size_t size);

typedef void (*simd_i8_funcptr)(const i8* lhs, const i8* rhs, i8* dst, size_t size);
typedef void (*simd_i8_val_funcptr)(const i8* lhs, const i8 rhs, i8* dst, size_t size);

typedef void (*simd_i16_funcptr)(const i16* lhs, const i16* rhs, i16* dst, size_t size);
typedef void (*simd_i16_val_funcptr)(const i16* lhs, const i16 rhs, i16* dst, size_t size);

typedef void (*simd_i32_funcptr)(const i32* lhs, const i32* rhs, i32* dst, size_t size);
typedef void (*simd_i32_val_funcptr)(const i32* lhs, const i32 rhs, i32* dst, size_t size);

typedef void (*simd_i64_funcptr)(const i64* lhs, const i64* rhs, i64* dst, size_t size);
typedef void (*simd_i64_val_funcptr)(const i64* lhs, const i64 rhs, i64* dst, size_t size);

typedef void (*simd_u8_funcptr)(const u8* lhs, const u8* rhs, u8* dst, size_t size);
typedef void (*simd_u8_val_funcptr)(const u8* lhs, const u8 rhs, u8* dst, size_t size);

typedef void (*simd_u16_funcptr)(const u16* lhs, const u16* rhs, u16* dst, size_t size);
typedef void (*simd_u16_val_funcptr)(const u16* lhs, const u16 rhs, u16* dst, size_t size);

typedef void (*simd_u32_funcptr)(const u32* lhs, const u32* rhs, u32* dst, size_t size);
typedef void (*simd_u32_val_funcptr)(const u32* lhs, const u32 rhs, u32* dst, size_t size);

typedef void (*simd_u64_funcptr)(const u64* lhs, const u64* rhs, u64* dst, size_t size);
typedef void (*simd_u64_val_funcptr)(const u64* lhs, const u64 rhs, u64* dst, size_t size);

/*========================================
    globals.
========================================*//**/

/*
    All funcptrs are lazy loaded when `simd_##type##_##op()` is first called.
*//**/

static simd_f32_funcptr simd_f32_add_funcptr = NULL;
static simd_f32_funcptr simd_f32_sub_funcptr = NULL;
static simd_f32_funcptr simd_f32_div_funcptr = NULL;
static simd_f32_funcptr simd_f32_mul_funcptr = NULL;
static simd_f64_funcptr simd_f64_add_funcptr = NULL;
static simd_f64_funcptr simd_f64_sub_funcptr = NULL;
static simd_f64_funcptr simd_f64_div_funcptr = NULL;
static simd_f64_funcptr simd_f64_mul_funcptr = NULL;

static simd_f32_val_funcptr simd_f32_add_val_funcptr = NULL;
static simd_f32_val_funcptr simd_f32_sub_val_funcptr = NULL;
static simd_f32_val_funcptr simd_f32_div_val_funcptr = NULL;
static simd_f32_val_funcptr simd_f32_mul_val_funcptr = NULL;
static simd_f64_val_funcptr simd_f64_add_val_funcptr = NULL;
static simd_f64_val_funcptr simd_f64_sub_val_funcptr = NULL;
static simd_f64_val_funcptr simd_f64_div_val_funcptr = NULL;
static simd_f64_val_funcptr simd_f64_mul_val_funcptr = NULL;

static simd_i8_funcptr  simd_i8_add_funcptr = NULL;
static simd_i8_funcptr  simd_i8_sub_funcptr = NULL;
static simd_i8_funcptr  simd_i8_mul_funcptr = NULL;
static simd_i16_funcptr simd_i16_add_funcptr = NULL;
static simd_i16_funcptr simd_i16_sub_funcptr = NULL;
static simd_i16_funcptr simd_i16_mul_funcptr = NULL;
static simd_i32_funcptr simd_i32_add_funcptr = NULL;
static simd_i32_funcptr simd_i32_sub_funcptr = NULL;
static simd_i32_funcptr simd_i32_mul_funcptr = NULL;
static simd_i64_funcptr simd_i64_add_funcptr = NULL;
static simd_i64_funcptr simd_i64_sub_funcptr = NULL;
static simd_i64_funcptr simd_i64_mul_funcptr = NULL;

static simd_i8_val_funcptr simd_i8_add_val_funcptr = NULL;
static simd_i8_val_funcptr simd_i8_sub_val_funcptr = NULL;
static simd_i8_val_funcptr simd_i8_mul_val_funcptr = NULL;
static simd_i16_val_funcptr simd_i16_add_val_funcptr = NULL;
static simd_i16_val_funcptr simd_i16_sub_val_funcptr = NULL;
static simd_i16_val_funcptr simd_i16_mul_val_funcptr = NULL;
static simd_i32_val_funcptr simd_i32_add_val_funcptr = NULL;
static simd_i32_val_funcptr simd_i32_sub_val_funcptr = NULL;
static simd_i32_val_funcptr simd_i32_mul_val_funcptr = NULL;
static simd_i64_val_funcptr simd_i64_add_val_funcptr = NULL;
static simd_i64_val_funcptr simd_i64_sub_val_funcptr = NULL;
static simd_i64_val_funcptr simd_i64_mul_val_funcptr = NULL;

static simd_u8_funcptr simd_u8_add_funcptr = NULL;
static simd_u8_funcptr simd_u8_sub_funcptr = NULL;
static simd_u8_funcptr simd_u8_mul_funcptr = NULL;
static simd_u16_funcptr simd_u16_add_funcptr = NULL;
static simd_u16_funcptr simd_u16_sub_funcptr = NULL;
static simd_u16_funcptr simd_u16_mul_funcptr = NULL;
static simd_u32_funcptr simd_u32_add_funcptr = NULL;
static simd_u32_funcptr simd_u32_sub_funcptr = NULL;
static simd_u32_funcptr simd_u32_mul_funcptr = NULL;
static simd_u64_funcptr simd_u64_add_funcptr = NULL;
static simd_u64_funcptr simd_u64_sub_funcptr = NULL;
static simd_u64_funcptr simd_u64_mul_funcptr = NULL;

static simd_u8_val_funcptr simd_u8_add_val_funcptr = NULL;
static simd_u8_val_funcptr simd_u8_sub_val_funcptr = NULL;
static simd_u8_val_funcptr simd_u8_mul_val_funcptr = NULL;
static simd_u16_val_funcptr simd_u16_add_val_funcptr = NULL;
static simd_u16_val_funcptr simd_u16_sub_val_funcptr = NULL;
static simd_u16_val_funcptr simd_u16_mul_val_funcptr = NULL;
static simd_u32_val_funcptr simd_u32_add_val_funcptr = NULL;
static simd_u32_val_funcptr simd_u32_sub_val_funcptr = NULL;
static simd_u32_val_funcptr simd_u32_mul_val_funcptr = NULL;
static simd_u64_val_funcptr simd_u64_add_val_funcptr = NULL;
static simd_u64_val_funcptr simd_u64_sub_val_funcptr = NULL;
static simd_u64_val_funcptr simd_u64_mul_val_funcptr = NULL;

/*========================================
    defines.
========================================*//**/

#if COMPILER_GCC || COMPILER_CLANG
#   define SIMD_ATTR_SSE4 __attribute__((target("sse2,sse4.1")))
#   define SIMD_ATTR_AVX2 __attribute__((target("avx,avx2,avx512vl")))
#else
#   define SIMD_ATTR_SSE2
#   define SIMD_ATTR_AVX2
#endif

// 256 bits = 32 bytes.
#define SIMD_AVX2_LANE_SIZE 32
// 128 bits = 16 bytes.
#define SIMD_SSE2_LANE_SIZE 16

/*========================================
    generators.
========================================*//**/

/*
    Generates a function to perform an operation 
    between two array's elements; using a scalar loop.

    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_SCALAR(type, op_name, op) \
void simd_##type##_##op_name##_scalar(const type* lhs, const type* rhs, type* dst, size_t size){ \
    for(size_t i = 0; i < size; i++){ \
        dst[i] = lhs[i] op rhs[i]; \
    } \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using a scalar loop.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_VAL_SCALAR(type, name, operator) \
void simd_##type##_##name##_val_scalar(const type* lhs, const type rhs, type* dst, size_t size){ \
    for(size_t i = 0; i < size; i++){ \
        dst[i] = lhs[i] operator rhs; \
    } \
}

/*
    Generates a function to perform an operation 
    between two array's of floating-point values elements; 
    using the cpu's SSE registers.

    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_FLT_SSE2(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_SSE4 \
void simd_##type##_##func_name##_sse2(const type* lhs, const type* rhs, type* dst, size_t size){ \
    int i = 0; \
    int lanes = (16 / (sizeof(type))); \
    int size_relative = size - lanes; \
    for(; i <= size_relative; size += lanes){ \
        __m128 v_lhs    = _mm_loadu_##simd_precision(&lhs[i]); \
        __m128 v_rhs    = _mm_loadu_##simd_precision(&rhs[i]); \
        __m128 result   = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm_storeu_##simd_precision(&dst[i], result); \
    } \
    for(; i < size; i++){ \
        dst[i] = lhs[i] scalar_op rhs[i]; \
    } \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's SSE registers.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_VAL_FLT_SSE2(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##type##_##func_name##_val_sse2(const type* lhs, const type rhs, type* dst, size_t size){ \
    int i = 0; \
    int lanes = (32 / (sizeof(type))); \
    int size_relative = size - lanes; \
    __m128 v_rhs = _mm_set1_##simd_precision(rhs); \
    for(; i <= size_relative; i+= lanes){ \
        __m128 v_lhs    = _mm_loadu_##simd_precision(&lhs[i]); \
        __m128 result   = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm_storeu_##simd_precision(&dst[i], result); \
    } \
    for(; i < size; i++){ \
        dst[i] = lhs[i] scalar_op rhs; \
    } \
}

/*
    Generates a function to perform an operation 
    between two array's of floating-point values elements; 
    using the cpu's AVX registers.

    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_FLT_AVX2(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##type##_##func_name##_avx2(const type* lhs, const type* rhs, type* dst, size_t size){ \
    int i = 0; \
    int lanes = (32 / (sizeof(type))); \
    int size_relative = size - lanes; \
    for(; i <= size_relative; i+= lanes){ \
        __m256 v_lhs    = _mm256_loadu_##simd_precision(&lhs[i]); \
        __m256 v_rhs    = _mm256_loadu_##simd_precision(&rhs[i]); \
        __m256 result   = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm256_storeu_##simd_precision(&dst[i], result); \
    } \
    for(; i < size; i++){ \
        dst[i] = lhs[i] scalar_op rhs[i]; \
    } \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's AVX registers.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_VAL_FLT_AVX2(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##type##_##func_name##_val_avx2(const type* lhs, const type rhs, type* dst, size_t size){ \
    int i = 0; \
    int lanes = (32 / (sizeof(type))); \
    int size_relative = size - lanes; \
    __m256 v_rhs = _mm256_set1_##simd_precision(rhs); \
    for(; i <= size_relative; i+= lanes){ \
        __m256 v_lhs    = _mm256_loadu_##simd_precision(&lhs[i]); \
        __m256 result   = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm256_storeu_##simd_precision(&dst[i], result); \
    } \
    for(; i < size; i++){ \
        dst[i] = lhs[i] scalar_op rhs; \
    } \
}

/*
    Generates a function to perform an operation 
    between two array's of integer values elements; 
    using the cpu's SSE registers.

    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_INT_SSE(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_SSE4 \
void simd_##type##_##func_name##_sse2(const type* lhs, const type* rhs, type* dst, size_t size) { \
    int i = 0; \
    int lanes = sizeof(__m128i) / sizeof(type); \
    int size_relative = size - lanes; \
    for (; i <= size_relative; i += lanes) { \
        __m128i v_lhs   = _mm_loadu_si128((const __m128i*)&lhs[i]); \
        __m128i v_rhs   = _mm_loadu_si128((const __m128i*)&rhs[i]); \
        __m128i result  = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm_storeu_si128((__m128i*)&dst[i], result); \
    } \
    for (; i < size; i++) { \
        dst[i] = lhs[i] scalar_op rhs[i]; \
    } \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's SSE registers.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_VAL_INT_SSE(type, func_name, simd_op_name, scalar_op, simd_precision, set1_suffix) \
SIMD_ATTR_SSE4 \
void simd_##type##_##func_name##_val_sse2(const type* lhs, const type rhs, type* dst, size_t size) { \
    int i = 0; \
    int lanes = sizeof(__m128i) / sizeof(type); \
    int size_relative = size - lanes; \
    __m128i v_rhs = _mm_set1_##simd_precision##set1_suffix(rhs); \
    for (; i <= size_relative; i += lanes) { \
        __m128i v_lhs   = _mm_loadu_si128((const __m128i*)&lhs[i]); \
        __m128i result  = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm_storeu_si128((__m128i*)&dst[i], result); \
    } \
    for (; i < size; i++) { \
        dst[i] = lhs[i] scalar_op rhs; \
    } \
}

/*
    Generates a function to perform an operation 
    between two array's of integer values elements; 
    using the cpu's AVX registers.

    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_INT_AVX(type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##type##_##func_name##_avx2(const type* lhs, const type* rhs, type* dst, size_t size) { \
    int i = 0; \
    int lanes = sizeof(__m256i) / sizeof(type); \
    int size_relative = size - lanes; \
    for (; i <= size_relative; i += lanes) { \
        __m256i v_lhs   = _mm256_loadu_si256((const __m256i*)&lhs[i]); \
        __m256i v_rhs   = _mm256_loadu_si256((const __m256i*)&rhs[i]); \
        __m256i result  = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm256_storeu_si256((__m256i*)&dst[i], result); \
    } \
    for (; i < size; i++) { \
        dst[i] = lhs[i] scalar_op rhs[i]; \
    } \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's AVX registers.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `op_name`: the name of the operator.
    `op`: the operator to apply to each element in the loop. 
*/
#define SIMD_IMPL_VAL_INT_AVX(type, func_name, simd_op_name, scalar_op, simd_precision, set1_suffix) \
SIMD_ATTR_AVX2 \
void simd_##type##_##func_name##_val_avx2(const type* lhs, const type rhs, type* dst, size_t size) { \
    int i = 0; \
    int lanes = sizeof(__m256i) / sizeof(type); \
    int size_relative = size - lanes; \
    __m256i v_rhs = _mm256_set1_##simd_precision##set1_suffix(rhs); \
    for (; i <= size_relative; i += lanes) { \
        __m256i v_lhs   = _mm256_loadu_si256((const __m256i*)&lhs[i]); \
        __m256i result  = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
        _mm256_storeu_si256((__m256i*)&dst[i], result); \
    } \
    for (; i < size; i++) { \
        dst[i] = lhs[i] scalar_op rhs; \
    } \
}

/*
    Generates a dispatch to perform an operation
    between two array's.
    
    Dynamically choosing between previously generated
    SSE and AVX procedures

    Remarks:
    fallsback to scalar loop.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `name`: the name of the operator.
*/
#define SIMD_IMPL_DISPATCH(type, name) \
void simd_##type##_##name(const type* lhs, const type* rhs, type* dst, size_t size){ \
    if(!simd_##type##_##name##_funcptr){ \
        if(is_avx2_supported()){ \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_avx2; \
        } \
        else if(is_sse2_supported()){ \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_sse2; \
        } \
        else{ \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_scalar; \
        } \
    } \
    simd_##type##_##name##_funcptr(lhs,rhs,dst,size); \
}

/*
    Generates a dispatch to perform an operation
    between an array's elements and a value of `type`.
    
    Dynamically choosing between previously generated
    SSE and AVX procedures

    Remarks:
    fallsback to scalar loop.
    
    Parameters:
    `type`: the type of the `lhs` and `rhs`
    `name`: the name of the operator.
*/
#define SIMD_IMPL_VAL_DISPATCH(type, name) \
void simd_##type##_##name##_val(const type* lhs, const type rhs, type* dst, size_t size){ \
    if(!simd_##type##_##name##_val_funcptr){ \
        if(is_avx2_supported()){ \
            simd_##type##_##name##_val_funcptr = simd_##type##_##name##_val_avx2; \
        } \
        else if(is_sse2_supported()){ \
            simd_##type##_##name##_val_funcptr = simd_##type##_##name##_val_sse2; \
        } \
        else{ \
            simd_##type##_##name##_val_funcptr = simd_##type##_##name##_val_scalar; \
        } \
    } \
    simd_##type##_##name##_val_funcptr(lhs,rhs,dst,size); \
}

/*========================================
    procedures.
========================================*//**/

bool is_avx2_supported(){
#if ARCH_X64 || ARCH_X86
    int info[4] = {0};
#   if COMPILER_CL
    __cpuid(info, 7);
#   elif COMPILER_CLANG || COMPILER_GCC
    __cpuid_count(7, 0, info[0], info[1], info[2], info[3]);
#   else
#   error unknown compiler.
#   endif
    return (info[1] & (1 << 5)) != 0; // Bit 5 of EBX is AVX2
#else
    /*
        (todo):
        fallback for ARM and APPLE SILICON.
    */  
    return false;
#endif
}

bool is_sse2_supported(){
#if ARCH_X64 || ARCH_X86
    int info[4] = {0};
#   if COMPILER_CL
    __cpuid(info, 1);
#   elif COMPILER_GCC || COMPILER_CLANG
    __cpuid(1, info[0], info[1], info[2], info[3]);
#   else
#   error unknown compiler.
#   endif
    return (info[3] & (1 << 26)) != 0; // Bit 26 of EDX is SSE2
#else
    /*
        (todo):
        fallback for ARM and APPLE SILICON.
    */ 
    return false;
#endif
}

/*========================================
    floats
========================================*//**/

SIMD_IMPL_SCALAR(       f32, add, +)
SIMD_IMPL_FLT_SSE2(     f32, add, add, +, ps)
SIMD_IMPL_FLT_AVX2(     f32, add, add, +, ps)
SIMD_IMPL_DISPATCH(     f32, add)
SIMD_IMPL_SCALAR(       f32, sub, -)
SIMD_IMPL_FLT_SSE2(     f32, sub, sub, -, ps)
SIMD_IMPL_FLT_AVX2(     f32, sub, sub, -, ps)
SIMD_IMPL_DISPATCH(     f32, sub)
SIMD_IMPL_SCALAR(       f32, mul, *)
SIMD_IMPL_FLT_SSE2(     f32, mul, mul, *, ps)
SIMD_IMPL_FLT_AVX2(     f32, mul, mul, *, ps)
SIMD_IMPL_DISPATCH(     f32, mul)
SIMD_IMPL_SCALAR(       f32, div, /)
SIMD_IMPL_FLT_SSE2(     f32, div, div, /, ps)
SIMD_IMPL_FLT_AVX2(     f32, div, div, /, ps)
SIMD_IMPL_DISPATCH(     f32, div)
SIMD_IMPL_VAL_SCALAR(   f32, add, +)
SIMD_IMPL_VAL_FLT_SSE2( f32, add, add, +, ps)
SIMD_IMPL_VAL_FLT_AVX2( f32, add, add, +, ps)
SIMD_IMPL_VAL_DISPATCH( f32, add)
SIMD_IMPL_VAL_SCALAR(   f32, sub, -)
SIMD_IMPL_VAL_FLT_SSE2( f32, sub, sub, -, ps)
SIMD_IMPL_VAL_FLT_AVX2( f32, sub, sub, -, ps)
SIMD_IMPL_VAL_DISPATCH( f32, sub)
SIMD_IMPL_VAL_SCALAR(   f32, mul, *)
SIMD_IMPL_VAL_FLT_SSE2( f32, mul, mul, *, ps)
SIMD_IMPL_VAL_FLT_AVX2( f32, mul, mul, *, ps)
SIMD_IMPL_VAL_DISPATCH( f32, mul)
SIMD_IMPL_VAL_SCALAR(   f32, div, /)
SIMD_IMPL_VAL_FLT_SSE2( f32, div, div, /, ps)
SIMD_IMPL_VAL_FLT_AVX2( f32, div, div, /, ps)
SIMD_IMPL_VAL_DISPATCH( f32, div)

SIMD_IMPL_SCALAR(       f64, add, +)
SIMD_IMPL_FLT_SSE2(     f64, add, add, +, pd)
SIMD_IMPL_FLT_AVX2(     f64, add, add, +, pd)
SIMD_IMPL_DISPATCH(     f64, add)
SIMD_IMPL_SCALAR(       f64, sub, -)
SIMD_IMPL_FLT_SSE2(     f64, sub, sub, -, pd)
SIMD_IMPL_FLT_AVX2(     f64, sub, sub, -, pd)
SIMD_IMPL_DISPATCH(     f64, sub)
SIMD_IMPL_SCALAR(       f64, mul, *)
SIMD_IMPL_FLT_SSE2(     f64, mul, mul, *, pd)
SIMD_IMPL_FLT_AVX2(     f64, mul, mul, *, pd)
SIMD_IMPL_DISPATCH(     f64, mul)
SIMD_IMPL_SCALAR(       f64, div, /)
SIMD_IMPL_FLT_SSE2(     f64, div, div, /, pd)
SIMD_IMPL_FLT_AVX2(     f64, div, div, /, pd)
SIMD_IMPL_DISPATCH(     f64, div)
SIMD_IMPL_VAL_SCALAR(   f64, add, +)
SIMD_IMPL_VAL_FLT_SSE2( f64, add, add, +, pd)
SIMD_IMPL_VAL_FLT_AVX2( f64, add, add, +, pd)
SIMD_IMPL_VAL_DISPATCH( f64, add)
SIMD_IMPL_VAL_SCALAR(   f64, sub, -)
SIMD_IMPL_VAL_FLT_SSE2( f64, sub, sub, -, pd)
SIMD_IMPL_VAL_FLT_AVX2( f64, sub, sub, -, pd)
SIMD_IMPL_VAL_DISPATCH( f64, sub)
SIMD_IMPL_VAL_SCALAR(   f64, mul, *)
SIMD_IMPL_VAL_FLT_SSE2( f64, mul, mul, *, pd)
SIMD_IMPL_VAL_FLT_AVX2( f64, mul, mul, *, pd)
SIMD_IMPL_VAL_DISPATCH( f64, mul)
SIMD_IMPL_VAL_SCALAR(   f64, div, /)
SIMD_IMPL_VAL_FLT_SSE2( f64, div, div, /, pd)
SIMD_IMPL_VAL_FLT_AVX2( f64, div, div, /, pd)
SIMD_IMPL_VAL_DISPATCH( f64, div)


/*========================================
    ints.
========================================*//**/

SIMD_IMPL_SCALAR(       i8, add, +)
SIMD_IMPL_INT_SSE(      i8, add, add, +, epi8)
SIMD_IMPL_INT_AVX(      i8, add, add, +, epi8)
SIMD_IMPL_DISPATCH(     i8, add)
SIMD_IMPL_SCALAR(       i8, sub, -)
SIMD_IMPL_INT_SSE(      i8, sub, sub, -, epi8)
SIMD_IMPL_INT_AVX(      i8, sub, sub, -, epi8)
SIMD_IMPL_DISPATCH(     i8, sub)
SIMD_IMPL_VAL_SCALAR(   i8, add, +)
SIMD_IMPL_VAL_INT_SSE(  i8, add, add, +, epi8, )
SIMD_IMPL_VAL_INT_AVX(  i8, add, add, +, epi8, )
SIMD_IMPL_VAL_DISPATCH( i8, add)
SIMD_IMPL_VAL_SCALAR(   i8, sub, -)
SIMD_IMPL_VAL_INT_SSE(  i8, sub, sub, -, epi8, )
SIMD_IMPL_VAL_INT_AVX(  i8, sub, sub, -, epi8, )
SIMD_IMPL_VAL_DISPATCH( i8, sub)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       i16, add, +)
SIMD_IMPL_INT_SSE(      i16, add, add, +, epi16)
SIMD_IMPL_INT_AVX(      i16, add, add, +, epi16)
SIMD_IMPL_DISPATCH(     i16, add)
SIMD_IMPL_SCALAR(       i16, sub, -)
SIMD_IMPL_INT_SSE(      i16, sub, sub, -, epi16)
SIMD_IMPL_INT_AVX(      i16, sub, sub, -, epi16)
SIMD_IMPL_DISPATCH(     i16, sub)
SIMD_IMPL_SCALAR(       i16, mul, *)
SIMD_IMPL_INT_SSE(      i16, mul, mullo, *, epi16)
SIMD_IMPL_INT_AVX(      i16, mul, mullo, *, epi16)
SIMD_IMPL_DISPATCH(     i16, mul)
SIMD_IMPL_VAL_SCALAR(   i16, add, +)
SIMD_IMPL_VAL_INT_SSE(  i16, add, add, +, epi16, )
SIMD_IMPL_VAL_INT_AVX(  i16, add, add, +, epi16, )
SIMD_IMPL_VAL_DISPATCH( i16, add)
SIMD_IMPL_VAL_SCALAR(   i16, sub, -)
SIMD_IMPL_VAL_INT_SSE(  i16, sub, sub, -, epi16, )
SIMD_IMPL_VAL_INT_AVX(  i16, sub, sub, -, epi16, )
SIMD_IMPL_VAL_DISPATCH( i16, sub)
SIMD_IMPL_VAL_SCALAR(   i16, mul, *)
SIMD_IMPL_VAL_INT_SSE(  i16, mul, mullo, *, epi16, )
SIMD_IMPL_VAL_INT_AVX(  i16, mul, mullo, *, epi16, )
SIMD_IMPL_VAL_DISPATCH( i16, mul)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       i32, add, +)
SIMD_IMPL_INT_SSE(      i32, add, add, +, epi32)
SIMD_IMPL_INT_AVX(      i32, add, add, +, epi32)
SIMD_IMPL_DISPATCH(     i32, add)
SIMD_IMPL_SCALAR(       i32, sub, -)
SIMD_IMPL_INT_SSE(      i32, sub, sub, -, epi32)
SIMD_IMPL_INT_AVX(      i32, sub, sub, -, epi32)
SIMD_IMPL_DISPATCH(     i32, sub)
SIMD_IMPL_SCALAR(       i32, mul, *)
SIMD_IMPL_INT_SSE(      i32, mul, mullo, *, epi32)
SIMD_IMPL_INT_AVX(      i32, mul, mullo, *, epi32)
SIMD_IMPL_DISPATCH(     i32, mul)
SIMD_IMPL_VAL_SCALAR(   i32, add, +)
SIMD_IMPL_VAL_INT_SSE(  i32, add, add, +, epi32, )
SIMD_IMPL_VAL_INT_AVX(  i32, add, add, +, epi32, )
SIMD_IMPL_VAL_DISPATCH( i32, add)
SIMD_IMPL_VAL_SCALAR(   i32, sub, -)
SIMD_IMPL_VAL_INT_SSE(  i32, sub, sub, -, epi32, )
SIMD_IMPL_VAL_INT_AVX(  i32, sub, sub, -, epi32, )
SIMD_IMPL_VAL_DISPATCH( i32, sub)
SIMD_IMPL_VAL_SCALAR(   i32, mul, *)
SIMD_IMPL_VAL_INT_SSE(  i32, mul, mullo, *, epi32, )
SIMD_IMPL_VAL_INT_AVX(  i32, mul, mullo, *, epi32, )
SIMD_IMPL_VAL_DISPATCH( i32, mul)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       i64, add, +)
SIMD_IMPL_INT_SSE(      i64, add, add, +, epi64)
SIMD_IMPL_INT_AVX(      i64, add, add, +, epi64)
SIMD_IMPL_DISPATCH(     i64, add)
SIMD_IMPL_SCALAR(       i64, sub, -)
SIMD_IMPL_INT_SSE(      i64, sub, sub, -, epi64)
SIMD_IMPL_INT_AVX(      i64, sub, sub, -, epi64)
SIMD_IMPL_DISPATCH(     i64, sub)
SIMD_IMPL_VAL_SCALAR(   i64, add, +)
SIMD_IMPL_VAL_INT_SSE(  i64, add, add, +, epi64, x)
SIMD_IMPL_VAL_INT_AVX(  i64, add, add, +, epi64, x)
SIMD_IMPL_VAL_DISPATCH( i64, add)
SIMD_IMPL_VAL_SCALAR(   i64, sub, -)
SIMD_IMPL_VAL_INT_SSE(  i64, sub, sub, -, epi64, x)
SIMD_IMPL_VAL_INT_AVX(  i64, sub, sub, -, epi64, x)
SIMD_IMPL_VAL_DISPATCH( i64, sub)
/*
    TODO: implement AVX512 for simd mul instructions. 
*/

SIMD_IMPL_SCALAR(       u8, add, +)
SIMD_IMPL_INT_SSE(      u8, add, add, +, epi8)
SIMD_IMPL_INT_AVX(      u8, add, add, +, epi8)
SIMD_IMPL_DISPATCH(     u8, add)
SIMD_IMPL_SCALAR(       u8, sub, -)
SIMD_IMPL_INT_SSE(      u8, sub, sub, -, epi8)
SIMD_IMPL_INT_AVX(      u8, sub, sub, -, epi8)
SIMD_IMPL_DISPATCH(     u8, sub)
SIMD_IMPL_VAL_SCALAR(   u8, add, +)
SIMD_IMPL_VAL_INT_SSE(  u8, add, add, +, epi8, )
SIMD_IMPL_VAL_INT_AVX(  u8, add, add, +, epi8, )
SIMD_IMPL_VAL_DISPATCH( u8, add)
SIMD_IMPL_VAL_SCALAR(   u8, sub, -)
SIMD_IMPL_VAL_INT_SSE(  u8, sub, sub, -, epi8, )
SIMD_IMPL_VAL_INT_AVX(  u8, sub, sub, -, epi8, )
SIMD_IMPL_VAL_DISPATCH( u8, sub)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       u16, add, +)
SIMD_IMPL_INT_SSE(      u16, add, add, +, epi16)
SIMD_IMPL_INT_AVX(      u16, add, add, +, epi16)
SIMD_IMPL_DISPATCH(     u16, add)
SIMD_IMPL_SCALAR(       u16, sub, -)
SIMD_IMPL_INT_SSE(      u16, sub, sub, -, epi16)
SIMD_IMPL_INT_AVX(      u16, sub, sub, -, epi16)
SIMD_IMPL_DISPATCH(     u16, sub)
SIMD_IMPL_SCALAR(       u16, mul, *)
SIMD_IMPL_INT_SSE(      u16, mul, mullo, *, epi16)
SIMD_IMPL_INT_AVX(      u16, mul, mullo, *, epi16)
SIMD_IMPL_DISPATCH(     u16, mul)
SIMD_IMPL_VAL_SCALAR(   u16, add, +)
SIMD_IMPL_VAL_INT_SSE(  u16, add, add, +, epi16, )
SIMD_IMPL_VAL_INT_AVX(  u16, add, add, +, epi16, )
SIMD_IMPL_VAL_DISPATCH( u16, add)
SIMD_IMPL_VAL_SCALAR(   u16, sub, -)
SIMD_IMPL_VAL_INT_SSE(  u16, sub, sub, -, epi16, )
SIMD_IMPL_VAL_INT_AVX(  u16, sub, sub, -, epi16, )
SIMD_IMPL_VAL_DISPATCH( u16, sub)
SIMD_IMPL_VAL_SCALAR(   u16, mul, *)
SIMD_IMPL_VAL_INT_SSE(  u16, mul, mullo, *, epi16, )
SIMD_IMPL_VAL_INT_AVX(  u16, mul, mullo, *, epi16, )
SIMD_IMPL_VAL_DISPATCH( u16, mul)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       u32, add, +)
SIMD_IMPL_INT_SSE(      u32, add, add, +, epi32)
SIMD_IMPL_INT_AVX(      u32, add, add, +, epi32)
SIMD_IMPL_DISPATCH(     u32, add)
SIMD_IMPL_SCALAR(       u32, sub, -)
SIMD_IMPL_INT_SSE(      u32, sub, sub, -, epi32)
SIMD_IMPL_INT_AVX(      u32, sub, sub, -, epi32)
SIMD_IMPL_DISPATCH(     u32, sub)
SIMD_IMPL_SCALAR(       u32, mul, *)
SIMD_IMPL_INT_SSE(      u32, mul, mullo, *, epi32)
SIMD_IMPL_INT_AVX(      u32, mul, mullo, *, epi32)
SIMD_IMPL_DISPATCH(     u32, mul)
SIMD_IMPL_VAL_SCALAR(   u32, add, +)
SIMD_IMPL_VAL_INT_SSE(  u32, add, add, +, epi32, )
SIMD_IMPL_VAL_INT_AVX(  u32, add, add, +, epi32, )
SIMD_IMPL_VAL_DISPATCH( u32, add)
SIMD_IMPL_VAL_SCALAR(   u32, sub, -)
SIMD_IMPL_VAL_INT_SSE(  u32, sub, sub, -, epi32, )
SIMD_IMPL_VAL_INT_AVX(  u32, sub, sub, -, epi32, )
SIMD_IMPL_VAL_DISPATCH( u32, sub)
SIMD_IMPL_VAL_SCALAR(   u32, mul, *)
SIMD_IMPL_VAL_INT_SSE(  u32, mul, mullo, *, epi32, )
SIMD_IMPL_VAL_INT_AVX(  u32, mul, mullo, *, epi32, )
SIMD_IMPL_VAL_DISPATCH( u32, mul)
/*
    x64/x86 doesnt have simd div instructions for integers. 
*/

SIMD_IMPL_SCALAR(       u64, add, +)
SIMD_IMPL_INT_SSE(      u64, add, add, +, epi64)
SIMD_IMPL_INT_AVX(      u64, add, add, +, epi64)
SIMD_IMPL_DISPATCH(     u64, add)
SIMD_IMPL_SCALAR(       u64, sub, -)
SIMD_IMPL_INT_SSE(      u64, sub, sub, -, epi64)
SIMD_IMPL_INT_AVX(      u64, sub, sub, -, epi64)
SIMD_IMPL_DISPATCH(     u64, sub)
SIMD_IMPL_VAL_SCALAR(   u64, add, +)
SIMD_IMPL_VAL_INT_SSE(  u64, add, add, +, epi64, x)
SIMD_IMPL_VAL_INT_AVX(  u64, add, add, +, epi64, x)
SIMD_IMPL_VAL_DISPATCH( u64, add)
SIMD_IMPL_VAL_SCALAR(   u64, sub, -)
SIMD_IMPL_VAL_INT_SSE(  u64, sub, sub, -, epi64, x)
SIMD_IMPL_VAL_INT_AVX(  u64, sub, sub, -, epi64, x)
SIMD_IMPL_VAL_DISPATCH( u64, sub)
/*
    TODO: implement AVX512 for simd mul instructions. 
*/