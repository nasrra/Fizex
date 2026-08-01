#include <string.h>

#define MemZero(ptr, size) memset(ptr, 0, size)
#define MemCopy(dst, src, size) memcpy(dst, src, size)

/*
    zeroes out an array. 
*/
#define ArrZero(array, length) MemZero(array, PtrArraySize(array, length))

#define Gigabyte(val) Megabyte(val) * 1024
#define Megabyte(val) Kilobyte(val) * 1024
#define Kilobyte(val) val * 1024

typedef struct{
    void* ptr;
    u64 capacity;
    u64 stride;
} Arena;

bool arena_init(Arena* arena, u64 capacity){
    arena->ptr = malloc(capacity);
    arena->capacity = capacity;
    arena->stride = 0;
    return true;
}

bool arena_push(Arena* arena, void* data, size_t size){
    u64 new_stride = arena->stride + size;
    if(new_stride > arena->capacity){
        return false;
    }
    MemCopy(arena->ptr+arena->stride, data, size);
    arena->stride = new_stride;
    return true;
}

void arena_clear(Arena* arena){
    arena->stride = 0;
}

void arena_clear_zeroed(Arena *arena){
    // MemZero(arena->ptr, arena->capacity);
    memset(arena->ptr, 0, arena->capacity); // test pattern
    arena->stride = 0;
}

void arena_free(Arena* arena){
    assert(arena != NULL);
    assert(arena->ptr != NULL);
    free(arena->ptr);
    *arena = (Arena){0};
}