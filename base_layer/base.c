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
        DEBUG_ASSERT(0!=0, "memory arena cannot support partition size.");
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

GenId genid_make(i32 index, i32 generation){
    GenId result = 0;
    DEBUG_ASSERT(index >= 0 && index <= GENID_MAX_INDEX, "index value is out of bounds.");
    DEBUG_ASSERT(generation >= 0 && generation <= GENID_MAX_GENERATION, "generation value is out of bounds.");

    // shift generation up by 20 bit so its the last 12 bits in the integer. 
    result = (u32)(generation & GENID_GENERATION_MASK) << 20; // apply the mask anyways so there is no crash in release mode.

    // Or with the index to that the index values are the first 20 bits in the integer.
    result |= (u32)index & GENID_INDEX_MASK; // apply the mask anyways so there is no crash in release mode.
    return result;
}

i32 genid_get_index(GenId genid){
    return (i32)(genid & GENID_INDEX_MASK);
}

i32 genid_get_generation(GenId genid){
    return (i32)genid >> 20;
}

GenId genid_increment_generation(GenId genid){
    // adding (1<<20) effectively adds 1 to the generation slice of the integer.
    // if the generation was at 4095, adding 1 makes it 4096; which would
    // "overflow" out of the 32-bit uint, wrapping back to 0 naturally.

    i32 next_gen = (genid_get_generation(genid)+1) & GENID_GENERATION_MASK;
    return genid_make(genid_get_index(genid), next_gen);
}

GenId genid_increment_index(GenId genid){
    // Get the current index and add 1.
    // mask it so the index value stays within th 20 bit range; wrapping around to zero if it hits max index.
    // this preserves the existing generation bits from overflow corruption of the index value.

    u32 curr_gen = genid & ~GENID_INDEX_MASK; // Isolate the top 12 bits;
    u32 next_index = (genid + 1) & GENID_INDEX_MASK;
    return curr_gen | next_index;
}

void string_init(String* string, MemoryArena* arena, i32 size){
    MEMORY_ARENA_ALLOC_ARRAY(arena, string->chars, &string->size, size);
}