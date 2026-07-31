#include <assert.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int bool;
#define false 0;
#define true 1;

#ifdef NDEBUG
    #define BoundsCheck(val)
#else
    #define BoundsCheck(val, len) do { \
        assert(val >= 0 && val < len); \
    } while(0)
#endif

#define ArrayLength(arr) (sizeof(arr)/sizeof(*(arr)))
#define PtrArraySize(ptr, length) (length * sizeof(*(ptr)))