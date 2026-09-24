/*
    NOTES:
    - x64/x86 doesnt have SIMD div instructions for integers.
    
    TODO:
    - AVX512 implementation.
    - NEON implementation.
*/

#if ARCH_X86 || ARCH_X64
#   include <immintrin.h>
#   if COMPILER_CL
#       include <intrin.h>
#   elif COMPILER_GCC || COMPILER_CLANG
#       include <cpuid.h>
#   endif
#elif
/*
    (TODO):
    fallback for ARM and APPLE SILICON.
*/
#   error not implemented for cpu architecture.
#endif

/*========================================
    types.
========================================*//**/

typedef void (*simd_f32_log_op_funcptr)(f32* dst, const f32* lhs, const f32* rhs, i32 length, i32 start_idx);
typedef void (*simd_f32_val_log_op_funcptr)(f32* dst, const f32* lhs, const f32 rhs, i32 length, i32 start_idx);

typedef void (*simd_f64_log_op_funcptr)(f64* dst, const f64* lhs, const f64* rhs, i32 length, i32 start_idx);
typedef void (*simd_f64_val_log_op_funcptr)(f64* dst, const f64* lhs, const f64 rhs, i32 length, i32 start_idx);

typedef void (*simd_i8_log_op_funcptr)(i8* dst, const i8* lhs, const i8* rhs, i32 length, i32 start_idx);
typedef void (*simd_i8_val_log_op_funcptr)(i8* dst, const i8* lhs, const i8 rhs, i32 length, i32 start_idx);

typedef void (*simd_i16_log_op_funcptr)(i16* dst, const i16* lhs, const i16* rhs, i32 length, i32 start_idx);
typedef void (*simd_i16_val_log_op_funcptr)(i16* dst, const i16* lhs, const i16 rhs, i32 length, i32 start_idx);

typedef void (*simd_i32_log_op_funcptr)(i32* dst, const i32* lhs, const i32* rhs, i32 length, i32 start_idx);
typedef void (*simd_i32_val_log_op_funcptr)(i32* dst, const i32* lhs, const i32 rhs, i32 length, i32 start_idx);

typedef void (*simd_i64_log_op_funcptr)(i64* dst, const i64* lhs, const i64* rhs, i32 length, i32 start_idx);
typedef void (*simd_i64_val_log_op_funcptr)(i64* dst, const i64* lhs, const i64 rhs, i32 length, i32 start_idx);

typedef void (*simd_u8_log_op_funcptr)(u8* dst, const u8* lhs, const u8* rhs, i32 length, i32 start_idx);
typedef void (*simd_u8_val_log_op_funcptr)(u8* dst, const u8* lhs, const u8 rhs, i32 length, i32 start_idx);

typedef void (*simd_u16_log_op_funcptr)(u16* dst, const u16* lhs, const u16* rhs, i32 length, i32 start_idx);
typedef void (*simd_u16_val_log_op_funcptr)(u16* dst, const u16* lhs, const u16 rhs, i32 length, i32 start_idx);

typedef void (*simd_u32_log_op_funcptr)(u32* dst, const u32* lhs, const u32* rhs, i32 length, i32 start_idx);
typedef void (*simd_u32_val_log_op_funcptr)(u32* dst, const u32* lhs, const u32 rhs, i32 length, i32 start_idx);

typedef void (*simd_u64_log_op_funcptr)(u64* dst, const u64* lhs, const u64* rhs, i32 length, i32 start_idx);
typedef void (*simd_u64_log_op_val_funcptr)(u64* dst, const u64* lhs, const u64 rhs, i32 length, i32 start_idx);

/*========================================
    globals.
========================================*//**/

/*
    All funcptrs are lazy loaded when `simd_##type##_##op()` is first called.
*//**/

// floating point log ops.
static simd_f32_log_op_funcptr simd_f32_add_funcptr = NULL;
static simd_f32_log_op_funcptr simd_f32_sub_funcptr = NULL;
static simd_f32_log_op_funcptr simd_f32_div_funcptr = NULL;
static simd_f32_log_op_funcptr simd_f32_mul_funcptr = NULL;
static simd_f64_log_op_funcptr simd_f64_add_funcptr = NULL;
static simd_f64_log_op_funcptr simd_f64_sub_funcptr = NULL;
static simd_f64_log_op_funcptr simd_f64_div_funcptr = NULL;
static simd_f64_log_op_funcptr simd_f64_mul_funcptr = NULL;

// floating point val log ops. 
static simd_f32_val_log_op_funcptr simd_f32_add_val_funcptr = NULL;
static simd_f32_val_log_op_funcptr simd_f32_sub_val_funcptr = NULL;
static simd_f32_val_log_op_funcptr simd_f32_div_val_funcptr = NULL;
static simd_f32_val_log_op_funcptr simd_f32_mul_val_funcptr = NULL;
static simd_f64_val_log_op_funcptr simd_f64_add_val_funcptr = NULL;
static simd_f64_val_log_op_funcptr simd_f64_sub_val_funcptr = NULL;
static simd_f64_val_log_op_funcptr simd_f64_div_val_funcptr = NULL;
static simd_f64_val_log_op_funcptr simd_f64_mul_val_funcptr = NULL;

// 
static simd_i8_log_op_funcptr  simd_i8_add_funcptr  = NULL;
static simd_i8_log_op_funcptr  simd_i8_sub_funcptr  = NULL;
static simd_i8_log_op_funcptr  simd_i8_mul_funcptr  = NULL;
static simd_i16_log_op_funcptr simd_i16_add_funcptr = NULL;
static simd_i16_log_op_funcptr simd_i16_sub_funcptr = NULL;
static simd_i16_log_op_funcptr simd_i16_mul_funcptr = NULL;
static simd_i32_log_op_funcptr simd_i32_add_funcptr = NULL;
static simd_i32_log_op_funcptr simd_i32_sub_funcptr = NULL;
static simd_i32_log_op_funcptr simd_i32_mul_funcptr = NULL;
static simd_i64_log_op_funcptr simd_i64_add_funcptr = NULL;
static simd_i64_log_op_funcptr simd_i64_sub_funcptr = NULL;
static simd_i64_log_op_funcptr simd_i64_mul_funcptr = NULL;

static simd_i8_val_log_op_funcptr  simd_i8_add_val_funcptr  = NULL;
static simd_i8_val_log_op_funcptr  simd_i8_sub_val_funcptr  = NULL;
static simd_i8_val_log_op_funcptr  simd_i8_mul_val_funcptr  = NULL;
static simd_i16_val_log_op_funcptr simd_i16_add_val_funcptr = NULL;
static simd_i16_val_log_op_funcptr simd_i16_sub_val_funcptr = NULL;
static simd_i16_val_log_op_funcptr simd_i16_mul_val_funcptr = NULL;
static simd_i32_val_log_op_funcptr simd_i32_add_val_funcptr = NULL;
static simd_i32_val_log_op_funcptr simd_i32_sub_val_funcptr = NULL;
static simd_i32_val_log_op_funcptr simd_i32_mul_val_funcptr = NULL;
static simd_i64_val_log_op_funcptr simd_i64_add_val_funcptr = NULL;
static simd_i64_val_log_op_funcptr simd_i64_sub_val_funcptr = NULL;
static simd_i64_val_log_op_funcptr simd_i64_mul_val_funcptr = NULL;

static simd_u8_log_op_funcptr  simd_u8_add_funcptr  = NULL;
static simd_u8_log_op_funcptr  simd_u8_sub_funcptr  = NULL;
static simd_u8_log_op_funcptr  simd_u8_mul_funcptr  = NULL;
static simd_u16_log_op_funcptr simd_u16_add_funcptr = NULL;
static simd_u16_log_op_funcptr simd_u16_sub_funcptr = NULL;
static simd_u16_log_op_funcptr simd_u16_mul_funcptr = NULL;
static simd_u32_log_op_funcptr simd_u32_add_funcptr = NULL;
static simd_u32_log_op_funcptr simd_u32_sub_funcptr = NULL;
static simd_u32_log_op_funcptr simd_u32_mul_funcptr = NULL;
static simd_u64_log_op_funcptr simd_u64_add_funcptr = NULL;
static simd_u64_log_op_funcptr simd_u64_sub_funcptr = NULL;
static simd_u64_log_op_funcptr simd_u64_mul_funcptr = NULL;

static simd_u8_val_log_op_funcptr  simd_u8_add_val_funcptr  = NULL;
static simd_u8_val_log_op_funcptr  simd_u8_sub_val_funcptr  = NULL;
static simd_u8_val_log_op_funcptr  simd_u8_mul_val_funcptr  = NULL;
static simd_u16_val_log_op_funcptr simd_u16_add_val_funcptr = NULL;
static simd_u16_val_log_op_funcptr simd_u16_sub_val_funcptr = NULL;
static simd_u16_val_log_op_funcptr simd_u16_mul_val_funcptr = NULL;
static simd_u32_val_log_op_funcptr simd_u32_add_val_funcptr = NULL;
static simd_u32_val_log_op_funcptr simd_u32_sub_val_funcptr = NULL;
static simd_u32_val_log_op_funcptr simd_u32_mul_val_funcptr = NULL;
static simd_u64_log_op_val_funcptr simd_u64_add_val_funcptr = NULL;
static simd_u64_log_op_val_funcptr simd_u64_sub_val_funcptr = NULL;
static simd_u64_log_op_val_funcptr simd_u64_mul_val_funcptr = NULL;

static void (*simd_f32_store_val_funcptr) (f32* dst, i32 dst_length, f32 value);
static void (*simd_f64_store_val_funcptr) (f64* dst, i32 dst_length, f64 value);
static void (*simd_i8_store_val_funcptr)  (i8* dst,  i32 dst_length, i8 value);
static void (*simd_i16_store_val_funcptr) (i16* dst, i32 dst_length, i16 value);
static void (*simd_i32_store_val_funcptr) (i32* dst, i32 dst_length, i32 value);
static void (*simd_i64_store_val_funcptr) (i64* dst, i32 dst_length, i64 value);
static void (*simd_u8_store_val_funcptr)  (u8* dst,  i32 dst_length, u8 value);
static void (*simd_u16_store_val_funcptr) (u16* dst, i32 dst_length, u16 value);
static void (*simd_u32_store_val_funcptr) (u32* dst, i32 dst_length, u32 value);
static void (*simd_u64_store_val_funcptr) (u64* dst, i32 dst_length, u64 value);


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
    `type`: the type of the `dst`, `lhs` and `rhs`.
    `func_name`: the name tag for the function, formatted: simd_##type##_##func_name##_val_scalar. 
    `log_op`: the operator to apply to each element in the loop: (E.g, +, -, /, *);
*/
#define SIMD_DEFINE_LOG_OP_VAL_SCALAR(type, func_name, log_op)                                                      \
void simd_##type##_##func_name##_val_scalar(type* dst, const type* lhs, const type rhs, i32 length, i32 start_idx){ \
    i32 i = start_idx;                                                                                              \
    for(;i < length; i++){                                                                                          \
        dst[i] = lhs[i] log_op rhs;                                                                                 \
    }                                                                                                               \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's SSE registers.

    `parameters`:
    `type`: the type of the `lhs` and `rhs`
    `func_name`: the name tag in the function, formated like: simd_##type##_##func_name##_val_avx
    `log_op_name`: the name of the simd logical operator to perform: (E.g, add, sub, mullo, mul, div)
    `mem_op_suffix`: the suffix to use for memory opertations: (E.g, si128, si256, pd, ps)
    `cast_type`: the type to cast to when simd storing.
    `precision`: the simd precision suffix for `set` and logical operations: (E.g, pd, ps, epi8, epi16, epi32, epi64). 
    `set1_suffix`: should be character `x` for 64 bit numbers; otherwise blank space ` `. 
*/
#define SIMD_DEFINE_LOG_OP_VAL_SSE(type, func_name, log_op_name, mem_op_suffix, precision, cast_type, vector_type_suffix, set1_suffix)  \
SIMD_ATTR_SSE4                                                                                                                          \
void simd_##type##_##func_name##_val_sse(type* dst, const type* lhs, const type rhs, i32 length, i32 start_idx) {                       \
    i32 i = start_idx;                                                                                                                  \
    i32 lanes = SIMD_SSE_LANE_SIZE / sizeof(type);                                                                                      \
    i32 length_relative = length - lanes;                                                                                               \
    __m128##vector_type_suffix v_rhs = _mm_set1_##precision##set1_suffix(rhs);                                                          \
    for (; i <= length_relative; i += lanes) {                                                                                          \
        __m128##vector_type_suffix v_lhs   = _mm_loadu_##mem_op_suffix((const cast_type *)&lhs[i]);                                     \
        __m128##vector_type_suffix result  = _mm_##log_op_name##_##precision(v_lhs, v_rhs);                                             \
        _mm_storeu_##mem_op_suffix((cast_type *)&dst[i], result);                                                                       \
    }                                                                                                                                   \
    simd_##type##_##func_name##_val_scalar(dst, lhs, rhs, length, i);                                                                   \
}

/*
    Generates a function to perform an operation
    between an array's elements and a value of `type`;
    using the cpu's AVX registers.

    `parameters`:
    `type`: the type of the `lhs` and `rhs`
    `func_name`: the name tag in the function, formated like: simd_##type##_##func_name##_val_avx
    `log_op_name`: the name of the simd logical operator to perform: (E.g, add, sub, mullo, mul, div)
    `mem_op_suffix`: the suffix to use for memory opertations: (E.g, si128, si256, pd, ps)
    `cast_type`: the type to cast to when simd storing.
    `precision`: the simd precision suffix for `set` and logical operations: (E.g, pd, ps, epi8, epi16, epi32, epi64). 
    `set1_suffix`: should be character `x` for 64 bit numbers; otherwise blank space ` `. 
*/
#define SIMD_DEFINE_LOG_OP_VAL_AVX(type, func_name, log_op_name, mem_op_suffix, precision, cast_type, vector_type_suffix, set1_suffix)  \
SIMD_ATTR_AVX2                                                                                                                          \
void simd_##type##_##func_name##_val_avx(type* dst, const type* lhs, const type rhs, i32 length, i32 start_idx) {                       \
    BOUNDS_CHECK(start_idx, length);                                                                                                    \
    i32 i = start_idx;                                                                                                                  \
    i32 lanes = SIMD_AVX_LANE_SIZE / sizeof(type);                                                                                      \
    i32 length_relative = length - lanes;                                                                                               \
    __m256##vector_type_suffix v_rhs = _mm256_set1_##precision##set1_suffix(rhs);                                                       \
    for (; i <= length_relative; i += lanes) {                                                                                          \
        __m256##vector_type_suffix v_lhs   = _mm256_loadu_##mem_op_suffix((const cast_type *)&lhs[i]);                                  \
        __m256##vector_type_suffix result  = _mm256_##log_op_name##_##precision(v_lhs, v_rhs);                                          \
        _mm256_storeu_##mem_op_suffix((cast_type *)&dst[i], result);                                                                    \
    }                                                                                                                                   \
    simd_##type##_##func_name##_val_scalar(dst, lhs, rhs, length, i);                                                                   \
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
#define SIMD_DEFINE_LOG_OP_VAL_DISPATCH(type, func_name)                                                    \
void simd_##type##_##func_name##_val(type* dst, const type* lhs, const type rhs, i32 length, i32 start_idx){\
    if(!simd_##type##_##func_name##_val_funcptr){                                                           \
        if(simd_is_avx_supported()){                                                                        \
            simd_##type##_##func_name##_val_funcptr = simd_##type##_##func_name##_val_avx;                  \
        }                                                                                                   \
        else if(simd_is_sse_supported()){                                                                   \
            simd_##type##_##func_name##_val_funcptr = simd_##type##_##func_name##_val_sse;                  \
        }                                                                                                   \
        else{                                                                                               \
            simd_##type##_##func_name##_val_funcptr = simd_##type##_##func_name##_val_scalar;               \
        }                                                                                                   \
    }                                                                                                       \
    simd_##type##_##func_name##_val_funcptr(dst, lhs, rhs, length, start_idx);                              \
}

/*
    Generates a function to perform an operation
    between two array's elements; using a scalar loop.

    Parameters:
    `type`: the type of the `dst`, `lhs` and `rhs`.
    `func_name`: the name tag for the function, formatted: simd_##type##_##func_name##_val_scalar. 
    `log_op`: the operator to apply to each element in the loop: (E.g, +, -, /, *);
*/
#define SIMD_DEFINE_LOG_OP_SCALAR(type, func_name, log_op)                                                          \
void simd_##type##_##func_name##_scalar(type* dst, const type* lhs, const type* rhs, i32 length, i32 start_idx){    \
    i32 i = start_idx;                                                                                              \
    for(;i < length; i++){                                                                                          \
        dst[i] = lhs[i] log_op rhs[i];                                                                              \
    }                                                                                                               \
}

/*
    Generates a function to perform an operation
    between an two array's elements of `type`;
    using the cpu's SSE registers.

    `parameters`:
    `type`: the type of the `lhs` and `rhs`
    `func_name`: the name tag in the function, formated like: simd_##type##_##func_name##_val_avx
    `log_op_name`: the name of the simd logical operator to perform: (E.g, add, sub, mullo, mul, div)
    `mem_op_suffix`: the suffix to use for memory opertations: (E.g, si128, si256, pd, ps)
    `cast_type`: the type to cast to when simd storing.
    `precision`: the simd precision suffix for `set` and logical operations: (E.g, pd, ps, epi8, epi16, epi32, epi64). 
    `set1_suffix`: should be character `x` for 64 bit numbers; otherwise blank space ` `. 
*/
#define SIMD_DEFINE_LOG_OP_SSE(type, func_name, log_op_name, mem_op_suffix, precision, cast_type, vector_type_suffix, set1_suffix)  \
SIMD_ATTR_SSE4                                                                                                                      \
void simd_##type##_##func_name##_sse(type* dst, const type* lhs, const type* rhs, i32 length, i32 start_idx) {                      \
    i32 i = start_idx;                                                                                                              \
    i32 lanes = SIMD_SSE_LANE_SIZE / sizeof(type);                                                                                  \
    i32 length_relative = length - lanes;                                                                                           \
    for (; i <= length_relative; i += lanes) {                                                                                      \
        __m128##vector_type_suffix v_lhs    = _mm_loadu_##mem_op_suffix((const cast_type *)&lhs[i]);                                \
        __m128##vector_type_suffix v_rhs    = _mm_loadu_##mem_op_suffix((const cast_type *)&rhs[i]);                                \
        __m128##vector_type_suffix result   = _mm_##log_op_name##_##precision(v_lhs, v_rhs);                                        \
        _mm_storeu_##mem_op_suffix(( cast_type *)&dst[i], result);                                                                  \
    }                                                                                                                               \
    simd_##type##_##func_name##_scalar(dst, lhs, rhs, length, i);                                                                   \
}

/*
    Generates a function to perform an operation
    between an two array's elements of `type`;
    using the cpu's AVX registers.

    `parameters`:
    `type`: the type of the `lhs` and `rhs`
    `func_name`: the name tag in the function, formated like: simd_##type##_##func_name##_val_avx
    `log_op_name`: the name of the simd logical operator to perform: (E.g, add, sub, mullo, mul, div)
    `mem_op_suffix`: the suffix to use for memory opertations: (E.g, si128, si256, pd, ps)
    `cast_type`: the type to cast to when simd storing.
    `precision`: the simd precision suffix for `set` and logical operations: (E.g, pd, ps, epi8, epi16, epi32, epi64). 
    `set1_suffix`: should be character `x` for 64 bit numbers; otherwise blank space ` `. 
*/
#define SIMD_DEFINE_LOG_OP_AVX(type, func_name, log_op_name, mem_op_suffix, precision, cast_type, vector_type_suffix, set1_suffix)  \
SIMD_ATTR_AVX2                                                                                                                      \
void simd_##type##_##func_name##_avx(type* dst, const type* lhs, const type* rhs, i32 length, i32 start_idx) {                      \
    i32 i = start_idx;                                                                                                              \
    i32 lanes = SIMD_AVX_LANE_SIZE / sizeof(type);                                                                                  \
    i32 length_relative = length - lanes;                                                                                           \
    for (; i <= length_relative; i += lanes) {                                                                                      \
        __m256##vector_type_suffix v_lhs    = _mm256_loadu_##mem_op_suffix((const cast_type *)&lhs[i]);                             \
        __m256##vector_type_suffix v_rhs    = _mm256_loadu_##mem_op_suffix((const cast_type *)&rhs[i]);                             \
        __m256##vector_type_suffix result   = _mm256_##log_op_name##_##precision(v_lhs, v_rhs);                                     \
        _mm256_storeu_##mem_op_suffix(( cast_type *)&dst[i], result);                                                               \
    }                                                                                                                               \
    simd_##type##_##func_name##_scalar(dst, lhs, rhs, length, i);                                                                   \
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
#define SIMD_DEFINE_LOG_OP_DISPATCH(type, name)                                                     \
void simd_##type##_##name##(type* dst, const type* lhs, const type* rhs, i32 legth, i32 start_idx){ \
    if(!simd_##type##_##name##_funcptr){                                                            \
        if(simd_is_avx_supported()){                                                                \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_avx;                            \
        }                                                                                           \
        else if(simd_is_sse_supported()){                                                           \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_sse;                            \
        }                                                                                           \
        else{                                                                                       \
            simd_##type##_##name##_funcptr = simd_##type##_##name##_scalar;                         \
        }                                                                                           \
    }                                                                                               \
    simd_##type##_##name##_funcptr(dst, lhs, rhs, legth, start_idx);                                \
}

#define SIMD_DEFINE_STORE_VAL_SCALAR(type)                                  \
void simd_##type##_store_val_scalar(type* dst, i32 dst_length, type value){ \
    for(i32 i = 0; i < dst_length; i++){                                    \
        dst[i] = value;                                                     \
    }                                                                       \
}

#define SIMD_DEFINE_STORE_VAL_SSE(type, cast_type, set_suffix, store_suffix, vector_type_suffix)    \
void simd_##type##_store_val_sse(type* dst, i32 dst_length, type value){                            \
    __m128##vector_type_suffix v = _mm_set1_##set_suffix(value);                                    \
    i32 i = 0;                                                                                      \
    i32 size = SIMD_SSE_LANE_SIZE / sizeof(type);                                                   \
    for(; i + size <= dst_length; i += size){                                                       \
        _mm_storeu_##store_suffix((cast_type*)&dst[i], v);                                          \
    }                                                                                               \
    for(; i < dst_length; i++){                                                                     \
        dst[i] = value;                                                                             \
    }                                                                                               \
}

#define SIMD_DEFINE_STORE_VAL_AVX(type, cast_type, set_suffix, store_suffix, vector_type_suffix)    \
void simd_##type##_store_val_avx(type* dst, i32 dst_length, type value){                            \
    __m256##vector_type_suffix v = _mm256_set1_##set_suffix(value);                                 \
    i32 i = 0;                                                                                      \
    i32 size = SIMD_AVX_LANE_SIZE / sizeof(type);                                                   \
    for(; i + size <= dst_length; i += size){                                                       \
        _mm256_storeu_##store_suffix((cast_type*)&dst[i], v);                                       \
    }                                                                                               \
    for(; i < dst_length; i++){                                                                     \
        dst[i] = value;                                                                             \
    }                                                                                               \
}

#define SIMD_DEFINE_STORE_VAL_DISPATCH(type)                                    \
void simd_##type##_store_val(type* dst, i32 dst_length, type value){            \
    if(!simd_##type##_store_val_funcptr){                                       \
        if(simd_is_avx_supported()){                                            \
            simd_##type##_store_val_funcptr = simd_##type##_store_val_avx;      \
        }                                                                       \
        else if(simd_is_sse_supported()){                                       \
            simd_##type##_store_val_funcptr = simd_##type##_store_val_sse;      \
        }                                                                       \
        else{                                                                   \
            simd_##type##_store_val_funcptr = simd_##type##_store_val_scalar;   \
        }                                                                       \
    }                                                                           \
    simd_##type##_store_val_funcptr(dst, dst_length, value);                    \
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

bool cpu_is_bmi2_supported(){
    int cpu_info[4];
    __cpuidex(cpu_info, 7, 0);
    //EBX bit 8 = BMI2
    return (cpu_info[1] & (1<<8)) != 0;
}

///
/// f32
///

SIMD_DEFINE_STORE_VAL_SCALAR(f32);
SIMD_DEFINE_STORE_VAL_SSE(f32, f32, ps, ps, );
SIMD_DEFINE_STORE_VAL_AVX(f32, f32, ps, ps, );
SIMD_DEFINE_STORE_VAL_DISPATCH(f32);

SIMD_DEFINE_LOG_OP_SCALAR       (f32, add, +);
SIMD_DEFINE_LOG_OP_SSE          (f32, add, add, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_AVX          (f32, add, add, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_DISPATCH     (f32, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f32, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f32, add, add, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f32, add, add, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f32, add);

SIMD_DEFINE_LOG_OP_SCALAR       (f32, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (f32, sub, sub, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_AVX          (f32, sub, sub, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_DISPATCH     (f32, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f32, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f32, sub, sub, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f32, sub, sub, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f32, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (f32, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (f32, mul, mul, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_AVX          (f32, mul, mul, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_DISPATCH     (f32, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f32, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f32, mul, mul, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f32, mul, mul, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f32, mul);

SIMD_DEFINE_LOG_OP_SCALAR       (f32, div, /);
SIMD_DEFINE_LOG_OP_SSE          (f32, div, div, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_AVX          (f32, div, div, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_DISPATCH     (f32, div);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f32, div, /);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f32, div, div, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f32, div, div, ps, ps, f32, , );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f32, div);




///
/// f64
///




SIMD_DEFINE_STORE_VAL_SCALAR(f64);
SIMD_DEFINE_STORE_VAL_SSE(f64, f64, pd, pd, d);
SIMD_DEFINE_STORE_VAL_AVX(f64, f64, pd, pd, d);
SIMD_DEFINE_STORE_VAL_DISPATCH(f64);

SIMD_DEFINE_LOG_OP_SCALAR       (f64, add, +);
SIMD_DEFINE_LOG_OP_SSE          (f64, add, add, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_AVX          (f64, add, add, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_DISPATCH     (f64, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f64, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f64, add, add, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f64, add, add, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f64, add);

SIMD_DEFINE_LOG_OP_SCALAR       (f64, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (f64, sub, sub, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_AVX          (f64, sub, sub, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_DISPATCH     (f64, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f64, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f64, sub, sub, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f64, sub, sub, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f64, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (f64, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (f64, mul, mul, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_AVX          (f64, mul, mul, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_DISPATCH     (f64, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f64, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f64, mul, mul, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f64, mul, mul, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f64, mul);

SIMD_DEFINE_LOG_OP_SCALAR       (f64, div, /);
SIMD_DEFINE_LOG_OP_SSE          (f64, div, div, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_AVX          (f64, div, div, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_DISPATCH     (f64, div);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (f64, div, /);
SIMD_DEFINE_LOG_OP_VAL_SSE      (f64, div, div, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (f64, div, div, pd, pd, f64, d, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (f64, div);





///
/// i8
///
/*
    NOTE:
    simd doesnt have multiplication for 8bit integers.
*/



SIMD_DEFINE_STORE_VAL_SCALAR(i8);
SIMD_DEFINE_STORE_VAL_SSE(i8, __m128i, epi8, si128, i);
SIMD_DEFINE_STORE_VAL_AVX(i8, __m256i, epi8, si256, i);
SIMD_DEFINE_STORE_VAL_DISPATCH(i8);

SIMD_DEFINE_LOG_OP_SCALAR       (i8, add, +);
SIMD_DEFINE_LOG_OP_SSE          (i8, add, add, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i8, add, add, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i8, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i8, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i8, add, add, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i8, add, add, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i8, add);

SIMD_DEFINE_LOG_OP_SCALAR       (i8, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (i8, sub, sub, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i8, sub, sub, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i8, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i8, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i8, sub, sub, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i8, sub, sub, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i8, sub);




///
/// i16
///




SIMD_DEFINE_STORE_VAL_SCALAR(i16);
SIMD_DEFINE_STORE_VAL_SSE(i16, __m128i, epi16, si128, i);
SIMD_DEFINE_STORE_VAL_AVX(i16, __m256i, epi16, si256, i);
SIMD_DEFINE_STORE_VAL_DISPATCH(i16);

SIMD_DEFINE_LOG_OP_SCALAR       (i16, add, +);
SIMD_DEFINE_LOG_OP_SSE          (i16, add, add, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i16, add, add, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i16, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i16, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i16, add, add, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i16, add, add, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i16, add);

SIMD_DEFINE_LOG_OP_SCALAR       (i16, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (i16, sub, sub, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i16, sub, sub, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i16, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i16, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i16, sub, sub, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i16, sub, sub, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i16, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (i16, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (i16, mul, mullo, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i16, mul, mullo, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i16, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i16, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i16, mul, mullo, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i16, mul, mullo, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i16, mul);




///
/// i32
///


SIMD_DEFINE_STORE_VAL_SCALAR(i32);
SIMD_DEFINE_STORE_VAL_SSE(i32, __m128i, epi32, si128, i);
SIMD_DEFINE_STORE_VAL_AVX(i32, __m256i, epi32, si256, i);
SIMD_DEFINE_STORE_VAL_DISPATCH(i32);

SIMD_DEFINE_LOG_OP_SCALAR       (i32, add, +);
SIMD_DEFINE_LOG_OP_SSE          (i32, add, add, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i32, add, add, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i32, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i32, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i32, add, add, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i32, add, add, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i32, add);

SIMD_DEFINE_LOG_OP_SCALAR       (i32, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (i32, sub, sub, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i32, sub, sub, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i32, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i32, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i32, sub, sub, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i32, sub, sub, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i32, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (i32, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (i32, mul, mullo, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (i32, mul, mullo, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (i32, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i32, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i32, mul, mullo, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (i32, mul, mullo, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i32, mul);




///
/// i64
///




SIMD_DEFINE_STORE_VAL_SCALAR(i64);
SIMD_DEFINE_STORE_VAL_SSE(i64, __m128i, epi64x, si128, i);
SIMD_DEFINE_STORE_VAL_AVX(i64, __m256i, epi64x, si256, i);
SIMD_DEFINE_STORE_VAL_DISPATCH(i64);

SIMD_DEFINE_LOG_OP_SCALAR       (i64, add, +);
SIMD_DEFINE_LOG_OP_SSE          (i64, add, add, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_AVX          (i64, add, add, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_DISPATCH     (i64, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i64, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i64, add, add, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_VAL_AVX      (i64, add, add, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i64, add);

SIMD_DEFINE_LOG_OP_SCALAR       (i64, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (i64, sub, sub, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_AVX          (i64, sub, sub, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_DISPATCH     (i64, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (i64, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (i64, sub, sub, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_VAL_AVX      (i64, sub, sub, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (i64, sub);




///
/// u8
///



SIMD_DEFINE_LOG_OP_SCALAR       (u8, add, +);
SIMD_DEFINE_LOG_OP_SSE          (u8, add, add, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u8, add, add, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u8, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u8, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u8, add, add, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u8, add, add, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u8, add);

SIMD_DEFINE_LOG_OP_SCALAR       (u8, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (u8, sub, sub, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u8, sub, sub, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u8, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u8, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u8, sub, sub, si128, epi8, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u8, sub, sub, si256, epi8, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u8, sub);




///
/// u16
///




SIMD_DEFINE_LOG_OP_SCALAR       (u16, add, +);
SIMD_DEFINE_LOG_OP_SSE          (u16, add, add, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u16, add, add, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u16, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u16, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u16, add, add, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u16, add, add, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u16, add);

SIMD_DEFINE_LOG_OP_SCALAR       (u16, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (u16, sub, sub, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u16, sub, sub, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u16, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u16, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u16, sub, sub, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u16, sub, sub, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u16, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (u16, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (u16, mul, mullo, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u16, mul, mullo, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u16, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u16, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u16, mul, mullo, si128, epi16, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u16, mul, mullo, si256, epi16, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u16, mul);




///
/// u32
///




SIMD_DEFINE_LOG_OP_SCALAR       (u32, add, +);
SIMD_DEFINE_LOG_OP_SSE          (u32, add, add, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u32, add, add, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u32, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u32, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u32, add, add, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u32, add, add, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u32, add);

SIMD_DEFINE_LOG_OP_SCALAR       (u32, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (u32, sub, sub, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u32, sub, sub, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u32, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u32, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u32, sub, sub, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u32, sub, sub, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u32, sub);

SIMD_DEFINE_LOG_OP_SCALAR       (u32, mul, *);
SIMD_DEFINE_LOG_OP_SSE          (u32, mul, mullo, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_AVX          (u32, mul, mullo, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_DISPATCH     (u32, mul);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u32, mul, *);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u32, mul, mullo, si128, epi32, __m128i, i, );
SIMD_DEFINE_LOG_OP_VAL_AVX      (u32, mul, mullo, si256, epi32, __m256i, i, );
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u32, mul);




///
/// u64
///




SIMD_DEFINE_LOG_OP_SCALAR       (u64, add, +);
SIMD_DEFINE_LOG_OP_SSE          (u64, add, add, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_AVX          (u64, add, add, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_DISPATCH     (u64, add);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u64, add, +);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u64, add, add, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_VAL_AVX      (u64, add, add, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u64, add);

SIMD_DEFINE_LOG_OP_SCALAR       (u64, sub, -);
SIMD_DEFINE_LOG_OP_SSE          (u64, sub, sub, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_AVX          (u64, sub, sub, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_DISPATCH     (u64, sub);

SIMD_DEFINE_LOG_OP_VAL_SCALAR   (u64, sub, -);
SIMD_DEFINE_LOG_OP_VAL_SSE      (u64, sub, sub, si128, epi64, __m128i, i, x);
SIMD_DEFINE_LOG_OP_VAL_AVX      (u64, sub, sub, si256, epi64, __m256i, i, x);
SIMD_DEFINE_LOG_OP_VAL_DISPATCH (u64, sub);

void simd_f32_sub_val_clamped_sse(f32* lhs, f32 rhs, f32* dst, f32 clamp_value, i32 length){
    i32 i = 0;
    __m128 clamp = _mm_set1_ps(clamp_value);
    __m128 v_rhs = _mm_set1_ps(rhs);
    i32 increment = SIMD_SSE_LANE_SIZE / (sizeof(f32));
    for(; i + increment <= length; i+= increment){
        __m128 v_lhs = _mm_loadu_ps(&lhs[i]);
        __m128 diff = _mm_sub_ps(v_lhs, v_rhs);
        __m128 clamped = _mm_max_ps(diff, clamp);
        _mm_storeu_ps(&dst[i], clamped);
    }

    // scalar tail.
    for(; i < length; i++){
        f32 d = lhs[i] - rhs;
        dst[i] = d < clamp_value ? clamp_value : d;
    }
}