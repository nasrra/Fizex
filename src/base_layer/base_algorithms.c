/**====================
    types.
====================**//**/

typedef struct{
    /**
        The u32 converted values for sorting.
    **/
    u32* translated_value;
    i32 translated_value_length;
    /**
        The array for temporary values when reordering the translated values during each radix pass.
    **/
    u32* value_scratch_space;
    u32 value_scratch_space_length;
    /**
        The array for temporary indices when reordering indices alongside the values during each radix pass.
    **/
    i32* index_scratch_space;
    i32 index_scratch_space_length;
    /**
        A histogram array.

        `remarks`
        Always 256 elements long.
    **/
    i32* byte_count;
    i32 byte_count_length;
    bool is_init;
} RadixSortBuffer;

/**====================
    defines
====================**//**/

#define MORTON_CODE_EXPAND_BITS_MASK_1 0x00FF00FF
#define MORTON_CODE_EXPAND_BITS_MASK_2 0x0F0F0F0F
#define MORTON_CODE_EXPAND_BITS_MASK_3 0x33333333
#define MORTON_CODE_EXPAND_BITS_MASK_4 0x55555555

/**====================
    functions.
====================**//**/

void radix_sort_buffer_init(RadixSortBuffer* buffer, MemoryArena* arena, i32 length){
    ASSERT(!buffer->is_init, "already init");
    MEMORY_ARENA_ALLOC_ARRAY(arena, buffer->translated_value, &buffer->translated_value_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, buffer->value_scratch_space, &buffer->value_scratch_space_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, buffer->index_scratch_space, &buffer->index_scratch_space_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, buffer->byte_count, &buffer->byte_count_length, 256); // count must always be 256 as radix operates on 8-bit/byte chunks.
    buffer->is_init = true;
}

/**
    Spreads 16 bits into 32 bits by inserting a 0 between each bit.

    `returns`
    `value` with expanded bits.
**/
u32 morton_code_expand_bits(u32 value){
    /**
        ----------------------------------------------
        Example:
        ----------------------------------------------
        value    = 00000000 00000000 00000000 01010101
        expanded = 00000000 00000000 00010001 00010001
        ----------------------------------------------

        note: bmi2 is used here instead of SIMD morton codes as
        bit shifting is the biggest operation here, which is a 'horizontal'
        problem. Bits in the same memory location need to be shifted from right to left.
        SIMD is good for 'vertical' problems, like array to array math, where each 'lane'
        of the Vector is operted on together.
    **/
    if(cpu_is_bmi2_supported()){
        return _pdep_u32(value, 0x55555555);
    }
    else{
        value = (value | (value << 8)) & MORTON_CODE_EXPAND_BITS_MASK_1;
        value = (value | (value << 4)) & MORTON_CODE_EXPAND_BITS_MASK_2;
        value = (value | (value << 2)) & MORTON_CODE_EXPAND_BITS_MASK_3;
        value = (value | (value << 1)) & MORTON_CODE_EXPAND_BITS_MASK_4;
        return value;
    }
}

/**
    Calculates the normalization factors required to map a pair of 32-bit values (x,y)
    into the 16-bit integer range [0, 65535] for Morton encoding.

    `remarks`
    This calculates the scale values used to transform a floating-poi32 value
    into a discrete 65536 x 65536 grid. This ensures that the bit-interleaving
    process utilizes the maximum precision available in a 32-bit Morton code.

    `parameters`
    - `range_x`: The range of the highest to lowest value in the `x` dataset/collection.
    - `range_y`: The range of the highest to lowest value in the `y` dataset/collection.
    - `scale_x`: output for the calculated x-scaling factor for calculating a morton code.
    - `scale_y`: output for the calculated y-scaling factor for calcualting a morton code.
**/
void morton_code_calc_scale_factor(f32 range_x, f32 range_y, f32* out_scale_x, f32* out_scale_y){

#if 0
    // calculate the range.
    float width = maxX - minX;
    float height = maxY - minY;
#endif

    // calculate the scales to fit in a 16 bit-range (2^16 - 1 = 65535)
    // this is done as a morton code is two 16-bit numbers interleaved together to form a 32 bit number.
    *out_scale_x = range_x > 0 ? 65535.0f / range_x : 0;
    *out_scale_y = range_y > 0 ? 65535.0f / range_y : 0;
}

/**
    Interleaves the bits of a pair of 32-bit values (x,y) into a 1-dimensional 32-bit Morton code.

    `remarks`
    - `x` should never be lower than `min_x`.
    - `y` should never be lower than `min_y`.

    `parameters`
    - `x`: the x-value of the 32-bit pair.
    - `y`: the y-value of the 32-bit pair.
    - `min_x`: the minimum x-value in the pair's dataset/collection.
    - `min_y`: the minimum x-value in the pair's dataset/collection.
    - `scale_x`: the scaling factor for the pair's dataset/collection to keep the x value within a 16-bit range.
    - `scale_y`: the scaling factor for the pair's dataset/collection to keep the y value within a 16-bit range.
**/
u32 morton_code_calc_morton_code(f32 x, f32 y, f32 min_x, f32 min_y, f32 scale_x, f32 scale_y){
    /**
        normalise coordinates to [0, 65535] (16 bit range.)
        this is done as a morton code is two 16-bit numbers interleaved together to form a 32 bit number.
    **/
    u32 ux = (u32)((x - min_x) * scale_x);
    u32 uy = (u32)((y - min_y) * scale_y);
    /**
        shift 'y' coordinate bit values to the left; so that the 'x' coordinate bit values dont overwrite it.
        this creates the morton code by interleaving the two 16 bit numbers.

        ------------------------------------------------------------------
        Example:
        ------------------------------------------------------------------
        ux                           = 00000000 00000000 00000000 01010101
        uy                           = 00000000 00000000 00000000 01010101
        ------------------------------------------------------------------
        expanded ux                  = 00000000 00000000 00010001 00010001
        expanded uy                  = 00000000 00000000 00010001 00010001
        ------------------------------------------------------------------
        expanded uy << 1             = 00000000 00000000 00100010 00100010
        ------------------------------------------------------------------
        (expanded uy << 1) | exp ux  = 00000000 00000000 00110010 00110011
        ------------------------------------------------------------------
    **/
    return morton_code_expand_bits(uy) << 1 | morton_code_expand_bits(ux);
}


/**
    Sorts a System.Span of uints in ascending order using the Radix Sort Algorithm.

    `remarks`
    This implementation processes 32-bit integers in four 8-bit (1 byte) passes.
    The following arrays must have a length at least equal to `length`:
    - `values`
    - `values_scratch_space`

    `parameters`
    `values`: the System.Span of uints to be sorted. Contains the final sorted values.
    `values_scratch_space`: temporary System.Span for reordering values during each pass.
    `byte_count`: a histogram System.Span, must be at least 256 elements long.
    `length`: the total number of elements to process.
**/
void radix_sort_asc(
    u32* values, i32 values_length,
    u32* values_scratch_space, i32 values_scratch_space_length,
    i32* byte_count, i32 byte_count_length,
    i32 length
){

    // Use pointers or references to swap which buffer is "source" and "destination"
    u32* src = values;
    i32 src_length = values_length;
    u32* dst = values_scratch_space;
    i32 dst_length = values_scratch_space_length;

    /**
        perform the radix sort on the units (LSD approach)
        Use 8-bit chunks (buckets of 256) for efficiency.
    **/
    for(i32 shift = 0; shift < 32; shift += 8){

        // reset the frequency of counts for this 8-bit chunk.
        ZERO_MEMORY(byte_count, sizeof(*byte_count) * byte_count_length);

        for(i32 i = 0; i < length; i++){
            /**
                Shift the target 8-bit chunk (byte) to the far right of the 32-bit integer.
                'shift' moves in increments of 8 (0, 8, 16, 24) to isolate each byte in the uint.
                Apply a bit mask of 0xFF (binary 11111111) to zero out everythin except those bottom 8 bits.
                This results in a 'bucket' index between 0 and 255, matching our count array.
            **/
            BOUNDS_CHECK(i, src_length);
            i32 byte_count_index = (i32)((src[i] >> shift) & 0xFF);

            BOUNDS_CHECK(byte_count_index, byte_count_length);
            byte_count[byte_count_index]+=1;
        }

        /**
            compute prefix sum (cumulative count) - ascending order.
            this tells us exactly which index each bucket starts at in the temp array.
        **/
        i32 start_idx = 0;
        for(i32 i = 0; i < 256; i++){
            BOUNDS_CHECK(i, byte_count_length);
            i32 c = byte_count[i];
            byte_count[i] = start_idx;
            start_idx += c;
        }

        // shuffle from src to dst.
        for(i32 i = 0; i < length; i++){
            BOUNDS_CHECK(i, src_length);
            i32 bucket = (int)((src[i] >> shift) & 0xFF);

            BOUNDS_CHECK(bucket, byte_count_length);
            i32 byte_count_index = byte_count[bucket]+=1;

            BOUNDS_CHECK(byte_count_index, dst_length);
            dst[byte_count_index] = src[i];
        }

        /**
            TOGGLE: Swap src and dst for the next pass
            Pass 0: val -> tmp
            Pass 1: tmp -> val
            Pass 2: val -> tmp
            Pass 3: tmp -> val
            Because we swapped 4 times (a 32-bit u32 is an even number), the final result
            is already back in the 'values' System.Span! No CopyTo needed.
        **/
        u32* swap_ptr = src;
        i32 swap_length = src_length;

        src = dst;
        src_length = dst_length;

        dst = swap_ptr;
        dst_length = swap_length;
    }
}

/**
    Sorts an array  of u32 in ascending order using the Radix Sort Algorithm.

    `remarks`
    This implementation processes 32-bit integers in four 8-bit (1 byte) passes .
    The following arrays must have a length at least equal to `length`:
    - `indices`
    - `indices_scratch_space`
    - `values`
    - `values_scratch_space`

    `parameters`
    `values`: the System.Span of uints to be sorted. Contains the final sorted values.
    `values_scratch_space`: temporary System.Span for reordering values during each pass.
    `indices`: the associated index System.Span to be reordered alongside the values.
    `indices_scratch_space`: temporary System.Span for reordering indices during each pass.
    `byte_count`: a histogram System.Span, must be at least 256 elements long.
    `length`: the total number of elements to process.

**/
void radix_sort_iasc(
    u32* values, i32 values_length,
    u32* values_scratch_space, i32 values_scratch_space_length,
    i32* indices, i32 indices_length,
    i32* indices_scratch_space, i32 indices_scratch_space_length,
    i32* byte_count, i32 byte_count_length,
    i32 length
){
    // use pointers or references to swap which buffer is "source" and "destination"
    u32* src_values = values;
    i32 src_values_length = values_length;
    u32* dst_values = values_scratch_space;
    i32 dst_values_length = values_scratch_space_length;
    i32* src_indices = indices;
    i32 src_indices_length = indices_length;
    i32* dst_indices = indices_scratch_space;
    i32 dst_indices_length = indices_scratch_space_length;

    /**
        perform the radix sort on the units (LSD approach)
        Use 8-bit chunks (buckets of 256) for efficiency.
    **/
    for(i32 shift = 0; shift < 32; shift += 8){

        // reset the frequency of counts for this 8-bit chunk.
        ZERO_MEMORY(byte_count, sizeof(*byte_count) * byte_count_length);

        for(i32 i = 0; i < length; i++){
            /**
                Shift the target 8-bit chunk (byte) to the far right of the 32-bit integer.
                'shift' moves in increments of 8 (0, 8, 16, 24) to isolate each byte in the uint.
                Apply a bit mask of 0xFF (binary 11111111) to zero out everything except those bottom 8 bits.
                This results in a 'bucket' index between 0 and 255, matching our count array.
            **/
            BOUNDS_CHECK(i, src_values_length);
            i32 byte_count_index = (i32)((src_values[i] >> shift) & 0xFF);
            BOUNDS_CHECK(byte_count_index, byte_count_length);
            byte_count[byte_count_index]++;
        }

        /**
            compute prefix sum (cumulative count) - ascending order.
            this tells us exactly which index each bucket starts at in the temp array.
        **/
        i32 start_idx = 0;
        for(i32 i = 0; i < 256; i++){
            BOUNDS_CHECK(i, byte_count_length);
            i32 c = byte_count[i];
            byte_count[i] = start_idx;
            start_idx += c;
        }

        // shuffle from src to dst.
        for(i32 i = 0; i < length; i++){
            BOUNDS_CHECK(i, src_values_length);
            i32 bucket = (int)((src_values[i] >> shift) & 0xFF);
            BOUNDS_CHECK(bucket, byte_count_length);
            i32 swap_index = byte_count[bucket]++;
            BOUNDS_CHECK(swap_index, dst_values_length);
            BOUNDS_CHECK(swap_index, dst_indices_length);
            BOUNDS_CHECK(i, src_values_length);
            BOUNDS_CHECK(i, src_indices_length);
            dst_values[swap_index] = src_values[i];
            dst_indices[swap_index] = src_indices[i];
        }

        /**
            TOGGLE: Swap src and dst for the next pass
            Pass 0: val -> tmp
            Pass 1: tmp -> val
            Pass 2: val -> tmp
            Pass 3: tmp -> val
            Because we swapped 4 times (a 32-bit u32 is an even number), the final result
            is already back in the 'values' System.Span! No CopyTo needed.

        **/

        u32* swap_values        = src_values;
        i32 swap_values_length  = src_values_length;
        src_values              = dst_values;
        src_values_length       = dst_values_length;
        dst_values              = swap_values;
        dst_values_length       = swap_values_length;

        u32* swap_indices       = src_indices;
        i32 swap_indices_length = src_indices_length;
        src_indices             = dst_indices;
        src_indices_length      = dst_indices_length;
        dst_indices             = swap_indices;
        dst_indices_length      = swap_indices_length;
    }
}

/**
    Sorts an array of uints in descending order using the Radix Sort Algorithm.

    `remarks`
    This implementation processes 32-bit integers in four 8-bit (1 byte) passes .
    The following arrays must have a length at least equal to `length`:
    - `values`
    - `temp`

    `parameters`
    `values`: the System.Span of uints to be sorted. Contains the final sorted values.
    `temp`: temporary System.Span for reordering values during each pass.
    `byteCount`: a histogram System.Span, must be at least 256 elements long.
    `length`: the total number of elements to process.
**/
void radix_sort_dsc(
    u32* values, i32 values_length,
    u32* values_scratch_space, i32 values_scratch_space_length,
    i32* byte_count, i32 byte_count_length,
    i32 length
){
    // Use pointers or references to swap which buffer is "source" and "destination"
    u32* src = values;
    i32 src_length = values_length;
    u32* dst = values_scratch_space;
    i32 dst_length = values_scratch_space_length;

    /**
        perform the radix sort on the units (LSD approach)
        Use 8-bit chunks (buckets of 256) for efficiency.
    **/
    for(i32 shift = 0; shift < 32; shift += 8){
        // reset the frequency of counts for this 8-bit chunk.
        ZERO_MEMORY(byte_count, sizeof(*byte_count) * byte_count_length);

        // count the occurences of each 8-value (0-255).
        for(i32 i = 0; i < length; i++){
            /**
                Shift the target 8-bit chunk (byte) to the far right of the 32-bit integer.
                'shift' moves in increments of 8 (0, 8, 16, 24) to isolate each byte in the uint.
                Apply a bit mask of 0xFF (binary 11111111) to zero out everythin except those bottom 8 bits.
                This results in a 'bucket' index between 0 and 255, matching our count array.
            **/
            BOUNDS_CHECK(i, src_length);
            i32 byte_count_index = (int)((src[i] >> shift) & 0xFF);
            BOUNDS_CHECK(byte_count_index, byte_count_length);
            byte_count[byte_count_index] += 1;
        }

        /**
            compute prefix sum (cumulative count) - descending order.
            this tells us exactly which index each bucket starts at in the temp array.
        **/
        i32 start_index = 0;
        for(i32 i = 255; i >= 0; i--){
            BOUNDS_CHECK(i, byte_count_length);
            i32 c = byte_count[i];
            byte_count[i] = start_index;
            start_index += c;
        }

        // shuffle from src to dst.
        for(i32 i = 0; i < length; i++){

            BOUNDS_CHECK(i, src_length);
            i32 bucket = (i32)((src[i] >> shift) & 0xFF);
            BOUNDS_CHECK(bucket, byte_count_length);
            i32 dst_index = byte_count[bucket]+=1;
            BOUNDS_CHECK(dst_index, dst_length);
            dst[dst_index] = src[i];
        }

        /**
            TOGGLE: Swap src and dst for the next pass
            Pass 0: val -> tmp
            Pass 1: tmp -> val
            Pass 2: val -> tmp
            Pass 3: tmp -> val
            Because we swapped 4 times (a 32-bit u32 is an even number), the final result
            is already back in the 'values' System.Span! No CopyTo needed.
        **/
        u32* swap = src;
        i32 swap_length = src_length;
        src = dst;
        src_length = dst_length;
        dst = swap;
        dst_length = swap_length;
    }
}

/**
    Sorts a System.Span of uints in descending order using the Radix Sort Algorithm.

    `remarks`
    This implementation processes 32-bit integers in four 8-bit (1 byte) passes .
    The following arrays must have a length at least equal to `length`:
    - `indices`
    - `tempIndices`
    - `values`
    - `tempValues`

    `parameters`
    `values`: the array of uints to be sorted. Contains the final sorted values.
    `tempValues`: temporary array for reordering values during each pass.
    `indices`: the associated index array to be reordered alongside the values.
    `tempIndices`: temporary array for reordering indices during each pass.
    `byteCount`: a histogram array, must be at least 256 elements long.
    `length`: the total number of elements to process.

**/
void radix_sort_idsc(
    u32* values, i32 values_length,
    u32* values_scratch_space, i32 values_scratch_space_length,
    i32* indices, i32 indices_length,
    i32* indices_scratch_space, i32 indices_scratch_space_length,
    i32* byte_count, i32 byte_count_length,
    i32 length
){
    // Use pointers or references to swap which buffer is "source" and "destination"
    u32* src_values         = values;
    i32 src_values_length   = values_length;
    u32* dst_values         = values_scratch_space;
    i32 dst_values_length   = values_scratch_space_length;
    i32* src_indices        = indices;
    i32 src_indices_length  = indices_length;
    i32* dst_indices        = indices_scratch_space;
    i32 dst_indices_length  = indices_scratch_space_length;

    /**
        perform the radix sort on the units (LSD approach)
        Use 8-bit chunks (buckets of 256) for efficiency.
    **/
    for (i32 shift = 0; shift < 32; shift += 8){
        // reset the frequency of counts for this 8-bit chunk.
        ZERO_MEMORY(byte_count, sizeof(*byte_count) * byte_count_length);

        // count the occurences of each 8-value (0-255).
        for (i32 i = 0; i < length; i++){
            /**
                Shift the target 8-bit chunk (byte) to the far right of the 32-bit integer.
                'shift' moves in increments of 8 (0, 8, 16, 24) to isolate each byte in the uint.
                Apply a bit mask of 0xFF (binary 11111111) to zero out everythin except those bottom 8 bits.
                This results in a 'bucket' index between 0 and 255, matching our count array.
            **/
            BOUNDS_CHECK(i, src_values_length);
            i32 byte_count_index = (int)((src_values[i] >> shift) & 0xFF);
            BOUNDS_CHECK(byte_count_index, byte_count_length);
            byte_count[byte_count_index] += 1;

            /**
                compute prefix sum (cumulative count) - ascending order.
                this tells us exactly which index each bucket starts at in the temp array.
            **/
            i32 start_idx = 0;
            for(i32 i = 255; i >= 0; i--){
                BOUNDS_CHECK(i, byte_count_length);
                i32 c = byte_count[i];
                byte_count[i] = start_idx;
                start_idx += c;
            }

            // Shuffle from src to dst
            for (i32 i = 0; i < length; i++){
                BOUNDS_CHECK(i, src_values_length);
                i32 bucket = (int)((src_values[i] >> shift) & 0xFF);
                BOUNDS_CHECK(bucket, byte_count_length);
                i32 swap_index = byte_count[bucket] += 1;
                BOUNDS_CHECK(swap_index, dst_values_length);
                BOUNDS_CHECK(swap_index, dst_indices_length);
                BOUNDS_CHECK(i, src_values_length);
                BOUNDS_CHECK(i, src_indices_length);
                dst_values[swap_index] = src_values[i];
                dst_indices[swap_index] = src_indices[i];
            }

            /**
                TOGGLE: Swap src and dst for the next pass
                Pass 0: val -> tmp
                Pass 1: tmp -> val
                Pass 2: val -> tmp
                Pass 3: tmp -> val
                Because we swapped 4 times (a 32-bit u32 is an even number), the final result
                is already back in the 'values' array! No CopyTo needed.
            **/
            u32* swap_values        = src_values;
            i32 swap_values_length  = src_values_length;
            src_values              = dst_values;
            src_values_length       = dst_values_length;
            dst_values              = swap_values;
            dst_values_length       = swap_values_length;


            u32* swap_indices        = src_indices;
            i32 swap_indices_length  = src_indices_length;
            src_indices              = dst_indices;
            src_indices_length       = dst_indices_length;
            dst_indices              = swap_indices;
            dst_indices_length       = swap_indices_length;
        }
    }
}




/**====================
    functions: RadixBuffer.
====================**//**/




void radix_buffer_sort_iasc(
    RadixSortBuffer* buffer,
    u32* value, i32 value_length,
    i32* index, i32 index_length,
    i32 length
){
    radix_sort_iasc(
        value, value_length,
        buffer->value_scratch_space, buffer->value_scratch_space_length,
        index, index_length,
        buffer->index_scratch_space, buffer->index_scratch_space_length,
        buffer->byte_count, buffer->byte_count_length,
        length
    );
}