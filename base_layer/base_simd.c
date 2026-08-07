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

typedef void (*simd_funcptr_f32)(const f32* lhs, const f32* rhs, f32* dst, i32 size);
typedef void (*simd_val_funcptr_f32)(const f32* lhs, const f32 rhs, f32* dst, i32 size);

typedef void (*simd_funcptr_f64)(const f64* lhs, const f64* rhs, f64* dst, i32 size);
typedef void (*simd_val_funcptr_f64)(const f64* lhs, const f64 rhs, f64* dst, i32 size);

typedef void (*simd_funcptr_i8)(const i8* lhs, const i8* rhs, i8* dst, i32 size);
typedef void (*simd_val_funcptr_i8)(const i8* lhs, const i8 rhs, i8* dst, i32 size);

typedef void (*simd_funcptr_i16)(const i16* lhs, const i16* rhs, i16* dst, i32 size);
typedef void (*simd_val_funcptr_i16)(const i16* lhs, const i16 rhs, i16* dst, i32 size);

typedef void (*simd_funcptr_i32)(const i32* lhs, const i32* rhs, i32* dst, i32 size);
typedef void (*simd_val_funcptr_i32)(const i32* lhs, const i32 rhs, i32* dst, i32 size);

typedef void (*simd_funcptr_i64)(const i64* lhs, const i64* rhs, i64* dst, i32 size);
typedef void (*simd_val_funcptr_i64)(const i64* lhs, const i64 rhs, i64* dst, i32 size);

typedef void (*simd_funcptr_u8)(const u8* lhs, const u8* rhs, u8* dst, i32 size);
typedef void (*simd_val_funcptr_u8)(const u8* lhs, const u8 rhs, u8* dst, i32 size);

typedef void (*simd_funcptr_u16)(const u16* lhs, const u16* rhs, u16* dst, i32 size);
typedef void (*simd_val_funcptr_u16)(const u16* lhs, const u16 rhs, u16* dst, i32 size);

typedef void (*simd_funcptr_u32)(const u32* lhs, const u32* rhs, u32* dst, i32 size);
typedef void (*simd_val_funcptr_u32)(const u32* lhs, const u32 rhs, u32* dst, i32 size);

typedef void (*simd_funcptr_u64)(const u64* lhs, const u64* rhs, u64* dst, i32 size);
typedef void (*simd_val_funcptr_u64)(const u64* lhs, const u64 rhs, u64* dst, i32 size);

/*========================================
    globals.
========================================*//**/

/*
    All funcptrs are lazy loaded when `simd_##type##_##op()` is first called.
*//**/

static simd_funcptr_f32 simd_add_funcptr_f32 = NULL;
static simd_funcptr_f32 simd_sub_funcptr_f32 = NULL;
static simd_funcptr_f32 simd_div_funcptr_f32 = NULL;
static simd_funcptr_f32 simd_mul_funcptr_f32 = NULL;
static simd_funcptr_f64 simd_add_funcptr_f64 = NULL;
static simd_funcptr_f64 simd_sub_funcptr_f64 = NULL;
static simd_funcptr_f64 simd_div_funcptr_f64 = NULL;
static simd_funcptr_f64 simd_mul_funcptr_f64 = NULL;

static simd_val_funcptr_f32 simd_add_val_funcptr_f32 = NULL;
static simd_val_funcptr_f32 simd_sub_val_funcptr_f32 = NULL;
static simd_val_funcptr_f32 simd_div_val_funcptr_f32 = NULL;
static simd_val_funcptr_f32 simd_mul_val_funcptr_f32 = NULL;
static simd_val_funcptr_f64 simd_add_val_funcptr_f64 = NULL;
static simd_val_funcptr_f64 simd_sub_val_funcptr_f64 = NULL;
static simd_val_funcptr_f64 simd_div_val_funcptr_f64 = NULL;
static simd_val_funcptr_f64 simd_mul_val_funcptr_f64 = NULL;

static simd_funcptr_i8  simd_add_funcptr_i8  = NULL;
static simd_funcptr_i8  simd_sub_funcptr_i8  = NULL;
static simd_funcptr_i8  simd_mul_funcptr_i8  = NULL;
static simd_funcptr_i16 simd_add_funcptr_i16 = NULL;
static simd_funcptr_i16 simd_sub_funcptr_i16 = NULL;
static simd_funcptr_i16 simd_mul_funcptr_i16 = NULL;
static simd_funcptr_i32 simd_add_funcptr_i32 = NULL;
static simd_funcptr_i32 simd_sub_funcptr_i32 = NULL;
static simd_funcptr_i32 simd_mul_funcptr_i32 = NULL;
static simd_funcptr_i64 simd_add_funcptr_i64 = NULL;
static simd_funcptr_i64 simd_sub_funcptr_i64 = NULL;
static simd_funcptr_i64 simd_mul_funcptr_i64 = NULL;

static simd_val_funcptr_i8  simd_add_val_funcptr_i8  = NULL;
static simd_val_funcptr_i8  simd_sub_val_funcptr_i8  = NULL;
static simd_val_funcptr_i8  simd_mul_val_funcptr_i8  = NULL;
static simd_val_funcptr_i16 simd_add_val_funcptr_i16 = NULL;
static simd_val_funcptr_i16 simd_sub_val_funcptr_i16 = NULL;
static simd_val_funcptr_i16 simd_mul_val_funcptr_i16 = NULL;
static simd_val_funcptr_i32 simd_add_val_funcptr_i32 = NULL;
static simd_val_funcptr_i32 simd_sub_val_funcptr_i32 = NULL;
static simd_val_funcptr_i32 simd_mul_val_funcptr_i32 = NULL;
static simd_val_funcptr_i64 simd_add_val_funcptr_i64 = NULL;
static simd_val_funcptr_i64 simd_sub_val_funcptr_i64 = NULL;
static simd_val_funcptr_i64 simd_mul_val_funcptr_i64 = NULL;

static simd_funcptr_u8  simd_add_funcptr_u8  = NULL;
static simd_funcptr_u8  simd_sub_funcptr_u8  = NULL;
static simd_funcptr_u8  simd_mul_funcptr_u8  = NULL;
static simd_funcptr_u16 simd_add_funcptr_u16 = NULL;
static simd_funcptr_u16 simd_sub_funcptr_u16 = NULL;
static simd_funcptr_u16 simd_mul_funcptr_u16 = NULL;
static simd_funcptr_u32 simd_add_funcptr_u32 = NULL;
static simd_funcptr_u32 simd_sub_funcptr_u32 = NULL;
static simd_funcptr_u32 simd_mul_funcptr_u32 = NULL;
static simd_funcptr_u64 simd_add_funcptr_u64 = NULL;
static simd_funcptr_u64 simd_sub_funcptr_u64 = NULL;
static simd_funcptr_u64 simd_mul_funcptr_u64 = NULL;

static simd_val_funcptr_u8  simd_add_val_funcptr_u8  = NULL;
static simd_val_funcptr_u8  simd_sub_val_funcptr_u8  = NULL;
static simd_val_funcptr_u8  simd_mul_val_funcptr_u8  = NULL;
static simd_val_funcptr_u16 simd_add_val_funcptr_u16 = NULL;
static simd_val_funcptr_u16 simd_sub_val_funcptr_u16 = NULL;
static simd_val_funcptr_u16 simd_mul_val_funcptr_u16 = NULL;
static simd_val_funcptr_u32 simd_add_val_funcptr_u32 = NULL;
static simd_val_funcptr_u32 simd_sub_val_funcptr_u32 = NULL;
static simd_val_funcptr_u32 simd_mul_val_funcptr_u32 = NULL;
static simd_val_funcptr_u64 simd_add_val_funcptr_u64 = NULL;
static simd_val_funcptr_u64 simd_sub_val_funcptr_u64 = NULL;
static simd_val_funcptr_u64 simd_mul_val_funcptr_u64 = NULL;

/*========================================
    defines.
========================================*//**/

#if COMPILER_GCC || COMPILER_CLANG
#   define SIMD_ATTR_SSE4 __attribute__((target("sse2,sse4.1")))
#   define SIMD_ATTR_AVX2 __attribute__((target("avx,avx2,avx512vl")))
#else
#   define SIMD_ATTR_SSE4
#   define SIMD_ATTR_AVX2
#endif

// 256 bits = 32 bytes.
#define SIMD_AVX_LANE_SIZE 32
// 128 bits = 16 bytes.
#define SIMD_SSE_LANE_SIZE 16

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
void simd_##op_name##_scalar_##type(const type* lhs, const type* rhs, type* dst, i32 size){ \
    for(i32 i = 0; i < size; i++){ \
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
void simd_##name##_val_scalar_##type(const type* lhs, const type rhs, type* dst, i32 size){ \
    for(i32 i = 0; i < size; i++){ \
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
#define SIMD_IMPL_FLT_SSE(type, vector_type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_SSE4 \
void simd_##func_name##_sse_##type(const type* lhs, const type* rhs, type* dst, i32 size){ \
    i32 i = 0; \
    i32 lanes = (SIMD_SSE_LANE_SIZE / (sizeof(type))); \
    i32 size_relative = size - lanes; \
    for(; i <= size_relative; size += lanes){ \
        vector_type v_lhs    = _mm_loadu_##simd_precision(&lhs[i]); \
        vector_type v_rhs    = _mm_loadu_##simd_precision(&rhs[i]); \
        vector_type result   = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
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
#define SIMD_IMPL_VAL_FLT_SSE(type, vector_type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_SSE4 \
void simd_##func_name##_val_sse_##type(const type* lhs, const type rhs, type* dst, i32 size){ \
    i32 i = 0; \
    i32 lanes = (SIMD_SSE_LANE_SIZE / (sizeof(type))); \
    i32 size_relative = size - lanes; \
    vector_type v_rhs = _mm_set1_##simd_precision(rhs); \
    for(; i <= size_relative; i+= lanes){ \
        vector_type v_lhs    = _mm_loadu_##simd_precision(&lhs[i]); \
        vector_type result   = _mm_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
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
#define SIMD_IMPL_FLT_AVX(type, vector_type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##func_name##_avx_##type(const type* lhs, const type* rhs, type* dst, i32 size){ \
    i32 i = 0; \
    i32 lanes = (SIMD_AVX_LANE_SIZE / (sizeof(type))); \
    i32 size_relative = size - lanes; \
    for(; i <= size_relative; i+= lanes){ \
        vector_type v_lhs    = _mm256_loadu_##simd_precision(&lhs[i]); \
        vector_type v_rhs    = _mm256_loadu_##simd_precision(&rhs[i]); \
        vector_type result   = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
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
#define SIMD_IMPL_VAL_FLT_AVX(type, vector_type, func_name, simd_op_name, scalar_op, simd_precision) \
SIMD_ATTR_AVX2 \
void simd_##func_name##_val_avx_##type(const type* lhs, const type rhs, type* dst, i32 size){ \
    i32 i = 0; \
    i32 lanes = (SIMD_AVX_LANE_SIZE / (sizeof(type))); \
    i32 size_relative = size - lanes; \
    vector_type v_rhs = _mm256_set1_##simd_precision(rhs); \
    for(; i <= size_relative; i+= lanes){ \
        vector_type v_lhs    = _mm256_loadu_##simd_precision(&lhs[i]); \
        vector_type result   = _mm256_##simd_op_name##_##simd_precision(v_lhs, v_rhs); \
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
void simd_##func_name##_sse_##type(const type* lhs, const type* rhs, type* dst, i32 size) { \
    i32 i = 0; \
    i32 lanes = SIMD_SSE_LANE_SIZE / sizeof(type); \
    i32 size_relative = size - lanes; \
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
void simd_##func_name##_val_sse_##type(const type* lhs, const type rhs, type* dst, i32 size) { \
    i32 i = 0; \
    i32 lanes = SIMD_SSE_LANE_SIZE / sizeof(type); \
    i32 size_relative = size - lanes; \
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
void simd_##func_name##_avx_##type(const type* lhs, const type* rhs, type* dst, i32 size) { \
    i32 i = 0; \
    i32 lanes = SIMD_AVX_LANE_SIZE / sizeof(type); \
    i32 size_relative = size - lanes; \
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
void simd_##func_name##_val_avx_##type(const type* lhs, const type rhs, type* dst, i32 size) { \
    i32 i = 0; \
    i32 lanes = SIMD_AVX_LANE_SIZE / sizeof(type); \
    i32 size_relative = size - lanes; \
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
void simd_##name##_##type(const type* lhs, const type* rhs, type* dst, i32 size){ \
    if(!simd_##name##_funcptr_##type){ \
        if(simd_is_avx_supported()){ \
            simd_##name##_funcptr_##type = simd_##name##_avx_##type; \
        } \
        else if(simd_is_sse_supported()){ \
            simd_##name##_funcptr_##type = simd_##name##_sse_##type; \
        } \
        else{ \
            simd_##name##_funcptr_##type = simd_##name##_scalar_##type; \
        } \
    } \
    simd_##name##_funcptr_##type(lhs,rhs,dst,size); \
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
void simd_##name##_val_##type(const type* lhs, const type rhs, type* dst, i32 size){ \
    if(!simd_##name##_val_funcptr_##type){ \
        if(simd_is_avx_supported()){ \
            simd_##name##_val_funcptr_##type = simd_##name##_val_avx_##type; \
        } \
        else if(simd_is_sse_supported()){ \
            simd_##name##_val_funcptr_##type = simd_##name##_val_sse_##type; \
        } \
        else{ \
            simd_##name##_val_funcptr_##type = simd_##name##_val_scalar_##type; \
        } \
    } \
    simd_##name##_val_funcptr_##type(lhs,rhs,dst,size); \
}

/*========================================
    procedures.
========================================*//**/

bool simd_is_avx_supported(){
#if ARCH_X64 || ARCH_X86
    int info[4] = {0};
#   if COMPILER_CL
        __cpuid(info, 7);
#   elif COMPILER_CLANG || COMPILER_GCC
        __cpuid_count(7, 0, info[0], info[1], info[2], info[3]);
#   else
#       error unknown compiler.
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

bool simd_is_sse_supported(){
#if ARCH_X64 || ARCH_X86
    int info[4] = {0};
#   if COMPILER_CL
        __cpuid(info, 1);
#   elif COMPILER_GCC || COMPILER_CLANG
        __cpuid(1, info[0], info[1], info[2], info[3]);
#   else
#       error unknown compiler.
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
SIMD_IMPL_FLT_SSE(     f32, __m128, add, add, +, ps)
SIMD_IMPL_FLT_AVX(     f32, __m256, add, add, +, ps)
SIMD_IMPL_DISPATCH(     f32, add)
SIMD_IMPL_SCALAR(       f32, sub, -)
SIMD_IMPL_FLT_SSE(     f32, __m128, sub, sub, -, ps)
SIMD_IMPL_FLT_AVX(     f32, __m256, sub, sub, -, ps)
SIMD_IMPL_DISPATCH(     f32, sub)
SIMD_IMPL_SCALAR(       f32, mul, *)
SIMD_IMPL_FLT_SSE(     f32, __m128, mul, mul, *, ps)
SIMD_IMPL_FLT_AVX(     f32, __m256, mul, mul, *, ps)
SIMD_IMPL_DISPATCH(     f32, mul)
SIMD_IMPL_SCALAR(       f32, div, /)
SIMD_IMPL_FLT_SSE(     f32, __m128, div, div, /, ps)
SIMD_IMPL_FLT_AVX(     f32, __m256, div, div, /, ps)
SIMD_IMPL_DISPATCH(     f32, div)
SIMD_IMPL_VAL_SCALAR(   f32, add, +)
SIMD_IMPL_VAL_FLT_SSE( f32, __m128, add, add, +, ps)
SIMD_IMPL_VAL_FLT_AVX( f32, __m256, add, add, +, ps)
SIMD_IMPL_VAL_DISPATCH( f32, add)
SIMD_IMPL_VAL_SCALAR(   f32, sub, -)
SIMD_IMPL_VAL_FLT_SSE( f32, __m128, sub, sub, -, ps)
SIMD_IMPL_VAL_FLT_AVX( f32, __m256, sub, sub, -, ps)
SIMD_IMPL_VAL_DISPATCH( f32, sub)
SIMD_IMPL_VAL_SCALAR(   f32, mul, *)
SIMD_IMPL_VAL_FLT_SSE( f32, __m128, mul, mul, *, ps)
SIMD_IMPL_VAL_FLT_AVX( f32, __m256, mul, mul, *, ps)
SIMD_IMPL_VAL_DISPATCH( f32, mul)
SIMD_IMPL_VAL_SCALAR(   f32, div, /)
SIMD_IMPL_VAL_FLT_SSE( f32, __m128, div, div, /, ps)
SIMD_IMPL_VAL_FLT_AVX( f32, __m256, div, div, /, ps)
SIMD_IMPL_VAL_DISPATCH( f32, div)

SIMD_IMPL_SCALAR(       f64, add, +)
SIMD_IMPL_FLT_SSE(     f64, __m128d, add, add, +, pd)
SIMD_IMPL_FLT_AVX(     f64, __m256d, add, add, +, pd)
SIMD_IMPL_DISPATCH(     f64, add)
SIMD_IMPL_SCALAR(       f64, sub, -)
SIMD_IMPL_FLT_SSE(     f64, __m128d, sub, sub, -, pd)
SIMD_IMPL_FLT_AVX(     f64, __m256d, sub, sub, -, pd)
SIMD_IMPL_DISPATCH(     f64, sub)
SIMD_IMPL_SCALAR(       f64, mul, *)
SIMD_IMPL_FLT_SSE(     f64, __m128d, mul, mul, *, pd)
SIMD_IMPL_FLT_AVX(     f64, __m256d, mul, mul, *, pd)
SIMD_IMPL_DISPATCH(     f64, mul)
SIMD_IMPL_SCALAR(       f64, div, /)
SIMD_IMPL_FLT_SSE(     f64, __m128d, div, div, /, pd)
SIMD_IMPL_FLT_AVX(     f64, __m256d, div, div, /, pd)
SIMD_IMPL_DISPATCH(     f64, div)
SIMD_IMPL_VAL_SCALAR(   f64, add, +)
SIMD_IMPL_VAL_FLT_SSE( f64, __m128d, add, add, +, pd)
SIMD_IMPL_VAL_FLT_AVX( f64, __m256d, add, add, +, pd)
SIMD_IMPL_VAL_DISPATCH( f64, add)
SIMD_IMPL_VAL_SCALAR(   f64, sub, -)
SIMD_IMPL_VAL_FLT_SSE( f64, __m128d, sub, sub, -, pd)
SIMD_IMPL_VAL_FLT_AVX( f64, __m256d, sub, sub, -, pd)
SIMD_IMPL_VAL_DISPATCH( f64, sub)
SIMD_IMPL_VAL_SCALAR(   f64, mul, *)
SIMD_IMPL_VAL_FLT_SSE( f64, __m128d, mul, mul, *, pd)
SIMD_IMPL_VAL_FLT_AVX( f64, __m256d, mul, mul, *, pd)
SIMD_IMPL_VAL_DISPATCH( f64, mul)
SIMD_IMPL_VAL_SCALAR(   f64, div, /)
SIMD_IMPL_VAL_FLT_SSE( f64, __m128d, div, div, /, pd)
SIMD_IMPL_VAL_FLT_AVX( f64, __m256d, div, div, /, pd)
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