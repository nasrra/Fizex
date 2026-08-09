#define MemZero(ptr, size) memset(ptr, 0, size)
#define COPY_MEMORY(dst, src, size) memcpy(dst, src, size)

/*
    zeroes out an array. 
*/
#define ArrZero(array, length) MemZero(array, PtrArraySize(array, length))

#define Gigabyte(val) Megabyte(val) * 1024
#define Megabyte(val) Kilobyte(val) * 1024
#define Kilobyte(val) val * 1024

typedef struct{
    void* ptr;
    u64 size;
    u64 stride;
} MemoryArena;

bool memory_arena_partition(MemoryArena* parent, MemoryArena* out_child, u64 size){
    u64 new_stride = parent->stride + size;
    if(new_stride >= parent->size){
        DEBUG_ASSERT(0!=0, "memory arena cannot support partition size.");
        return false;
    }

    parent->stride = new_stride;
    out_child->ptr = (u8*)(parent->ptr) + new_stride;
    out_child->size = size;
    out_child->stride = 0;
    
    return true;
}

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

void clear_memory_arena(MemoryArena* arena){
    arena->stride = 0;
}

void clear_zeroed_memory_arena(MemoryArena *arena){
    // MemZero(arena->ptr, arena->size);
    memset(arena->ptr, 0, arena->size); // test pattern
    arena->stride = 0;
}

void free_memory_arena(MemoryArena* arena){
    assert(arena != NULL);
    assert(arena->ptr != NULL);
    free(arena->ptr);
    *arena = (MemoryArena){0};
}