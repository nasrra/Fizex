#include "base.h"

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

    // Use the stack address of a local variable as entropy noise
    i32 stack_noise; 
    i32 result = (i32)(uintptr_t)&stack_noise;
    
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

bool memory_arena_partition(MemoryArena* parent, MemoryArena* out_child, u64 size){
    u64 new_stride = parent->stride + size;
    if(new_stride >= parent->size){
        ASSERT(0!=0, "memory arena cannot support partition size.");
        return false;
    }

    parent->stride = new_stride;
    out_child->ptr = (u8*)(parent->ptr) + new_stride;
    out_child->size = size;
    out_child->stride = 0;
    
    return true;
}

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

GenId gen_id_make(i32 index, i32 generation){
    GenId result = 0;
    ASSERT(index >= 0 && index <= GENID_MAX_INDEX, "index value is out of bounds.");
    ASSERT(generation >= 0 && generation <= GENID_MAX_GENERATION, "generation value is out of bounds.");

    // shift generation up by 20 bit so its the last 12 bits in the integer. 
    result = (u32)(generation & GENID_GENERATION_MASK) << 20; // apply the mask anyways so there is no crash in release mode.

    // Or with the index to that the index values are the first 20 bits in the integer.
    result |= (u32)index & GENID_INDEX_MASK; // apply the mask anyways so there is no crash in release mode.
    return result;
}

i32 gen_id_get_index(GenId gen_id){
    return (i32)(gen_id & GENID_INDEX_MASK);
}

i32 gen_id_get_generation(GenId gen_id){
    return (i32)gen_id >> 20;
}

GenId gen_id_increment_generation(GenId gen_id){
    // adding (1<<20) effectively adds 1 to the generation slice of the integer.
    // if the generation was at 4095, adding 1 makes it 4096; which would
    // "overflow" out of the 32-bit uint, wrapping back to 0 naturally.

    i32 next_gen = (gen_id_get_generation(gen_id)+1) & GENID_GENERATION_MASK;
    return gen_id_make(gen_id_get_index(gen_id), next_gen);
}

GenId gen_id_increment_index(GenId gen_id){
    // Get the current index and add 1.
    // mask it so the index value stays within th 20 bit range; wrapping around to zero if it hits max index.
    // this preserves the existing generation bits from overflow corruption of the index value.

    u32 curr_gen = gen_id & ~GENID_INDEX_MASK; // Isolate the top 12 bits;
    u32 next_index = (gen_id + 1) & GENID_INDEX_MASK;
    return curr_gen | next_index;
}

void string_init(String* string, MemoryArena* arena, i32 length){
    MEMORY_ARENA_ALLOC_ARRAY(arena, string->chars, &string->length, length);
    string->count = 0;
}

void string_clear(String* string){
    ZERO_MEMORY(string->chars, string->length * sizeof(char));
    string->count = 0;
}

void string_push_chars(String* string, char* chars, i32 chars_length){
    i32 available_space =  (string->length - string->count);
    ASSERT(available_space >= 0, "string to small to hold characters");
    available_space = available_space < chars_length ? available_space : chars_length; 
    for(i32 i = 0; i < available_space; i++){
        string->chars[string->count + i] = chars[i];
    }
    string->count += available_space;
}

void string_push(String* dst, String src){
    string_push_chars(dst, src.chars, src.count);
}

void gen_id_allocator_init(GenIdAllocator* allocator, MemoryArena* arena, i32 length){
    ASSERT(!allocator->is_init, "already initialised.");
    ASSERT(length <= GEN_ID_ALLOCATOR_MAX_LENGTH && length >= GEN_ID_ALLOCATOR_MIN_LENGTH, "length out of bounds");
    MEMORY_ARENA_ALLOC_ARRAY(arena, allocator->gen_ids, &allocator->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, allocator->allocated, &allocator->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, allocator->free_slots_stack, &allocator->length, length);

    // set the indices for the gen ids.
    for(u32 i = 1; i < (u32)length; i++){
        allocator->gen_ids[i] = i;
    }

    ARRAY_PUSH(allocator->free_slots_stack, length, &allocator->free_slots_stack_count, 1);
    allocator->is_init = true;
}

GenId gen_id_allocator_alloc(GenIdAllocator* allocator){

    if(allocator->free_slots_stack_count == 0){
        /**
            TODO: LogError("Memory Limit Hit", stackDepth: 2);
        **/
        return (GenId){0};
    }

    // get the next available slot to allocate in.
    i32 slot;
    ARRAY_POP(allocator->free_slots_stack, allocator->length, &allocator->free_slots_stack_count, &slot);

    // check if its neighbour can be allocated as well.
    i32 next_slot = slot + 1;
    if(next_slot > 0 && next_slot < allocator->length){
        // add to the stack if it is also free.
        if (allocator->allocated[next_slot] == false){
            ARRAY_PUSH(allocator->free_slots_stack, allocator->length, &allocator->free_slots_stack_count, next_slot);
        }
    }

    // update the gen index with the newly allocate data.
    allocator->allocated[slot] = true;
    return allocator->gen_ids[slot];
}

void gen_id_allocator_dealloc_unsafe(GenIdAllocator* allocator, i32 index){
    BOUNDS_CHECK(index, allocator->length);
    // increment the generation so that any gen indices pointing to this data are invalidated (making them stale pointers).
    allocator->gen_ids[index] = gen_id_increment_generation(allocator->gen_ids[index]);

    // deallocate the entity.
    allocator->allocated[index] = false;
    ARRAY_PUSH(allocator->free_slots_stack, allocator->length, &allocator->free_slots_stack_count, index);
}

bool gen_id_allocator_dealloc(GenIdAllocator* allocator, GenId gen_id){
    i32 index = gen_id_get_index(gen_id);

    // do nothing if the gen index is stale.
    BOUNDS_CHECK(index, allocator->length);
    if(allocator->gen_ids[index] != gen_id){
        return false;
    }

    gen_id_allocator_dealloc_unsafe(allocator, index);
    return true;
}

bool gen_id_allocator_is_gen_id_invalid(GenIdAllocator* allocator, GenId gen_id){
    i32 index = gen_id_get_index(gen_id);
    BOUNDS_CHECK(index, allocator->length);
    return gen_id == (GenId){0} || allocator->gen_ids[index] != gen_id;
}
