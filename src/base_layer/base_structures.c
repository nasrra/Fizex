/**====================
    types
====================**//**/

typedef struct{
    /**
        The amount of elements within a category.

        `remarks`
        Elements should be accessed by `category_index`.
    **/
    i32* category_stride;
    i32 category_stride_length;
    /**
        The starting indices for a sub category within the `overlap arrays`.

        `remarks`
        - `Elements` are arranged in a fixed-stride-like format.
        - `Entry` indices are layed out in a descending order; Example: (2,1,0) or (4,3,2,etc...).
        - The `stride` of each `entry` are the subcategories in a arithermatic series/triangular sum format - in ascending order.
            Note: this format removes duplicate entries when a storing a category overlaps with another.
        
        `example`
        (with three categories)
        index = [0], entry = 2, sub categories = 0,1,2
        index = [1], entry = 1, sub categories = 0,1
        index = [2], entry = 0, sub categories = 0    
    **/
    i32* sub_category_start_index;
    i32 sub_category_start_index_length;
    /**
        The count of valid elements after a sub category's start index within the `data` array.

        `remarks`
        - `Elements` are arranged in a fixed-stride-like format.
        - `Entry` indices are layed out in a descending order.
        - Example: (2,1,0) or (4,3,2,etc...).
        - The `stride` of each `entry` are the subcategories in a arithermatic series/triangular sum format - in ascending order.
            Note: this format removes duplicate entries when a storing a category overlaps with another.

        `example`
        (with three categories)
        index = [0], entry = 2, sub categories = 0,1,2
        index = [1], entry = 1, sub categories = 0,1
        index = [2], entry = 0, sub categories = 0    
    **/
    i32* sub_category_count;
    i32 sub_category_count_length;
    /**
        The triangular sum of the amount of categories the overlap data can be filtered into..
    **/
    i32 categories_triangular_sum;
    /**
        The maximum amount of elements this state instance can store.
    **/
    i32 max_elements;
    /**
        The element data.

        `remarks`
        Elements should be accessed via the calculated index provided by `categorised_leaf_overlaps_get_element_index(i32, i32, i32)`
        
        `example`        
        i32 index = categorised_leaf_overlaps_get_element_index(categoryIndex, subCategoryIndex, categoriesTriangularSum);
        i32 element = my_i32s[index];
    **/
    char* data;
    i32 data_length;
    bool is_init;
} CategorisedOverlapArray;

typedef struct {
    /**
        The Axis-Aligned Bounding-Boxes of all branches.
    **/
    Soa_Aabb aabb;
    /**
        The left leaf indices of all branches.
    **/
    i32* left_leaf_index;
    /**
        The right leaf indices of all branches.
    **/
    i32* right_leaf_index;
    /**
        The number of child branches (including the branch itself) of all branches.
        
        `remarks`
        E.g a branch that has three 4 children will have a subtree stride of 5; as the subtree size counts the branch as well.
    **/
    i32* subtree_stride; 
    /**
        The amount of leaves attatched of all branches.

        `remarks`
        Specifically, the amount of immediate leaves attatched to a branch; not counting children or parents.
    **/
    i32* leaf_count;
    /**
        The indices for the parent branch of all branches.
    **/
    i32* parent_index;
    /**
        The count of allocated entries from appending.
    **/
    i32 count;
    /**
        The length of all the backing arrays of this instance.
    **/
    i32 length;
    bool is_init;
} Soa_BvhBranch;

typedef struct{
    /**
        The Axis-Aligned Bounding-Boxes.
    **/
    Soa_Aabb aabb;
    /**
        The centroids of the Aabbs.
    **/
    Soa_Vector2 centroid;
    /**
        The user-defined categories of the leaves (used to filter overlap results).
    **/
    i32* category;
    /**    
        Gets the indices of branches that leaves are parented to.
        
        `remarks`
        Elements in this array should be valid after a Bounding Volume Hierarchy has been constructed.
    **/
    i32* branch_index;
    /**
        The count of allocated entries from appending.
    **/
    i32 count;
    /**
        The length of all the backing arrays of this instance.
    **/
    i32 length;
    bool is_init;
} Soa_BvhLeaf;

typedef struct{
    /**
        The index of the `owner` leaves of a query result.
        
        `remarks`
        Use a `query_result` integer to access elements.    
    **/
    i32* owner_leaf_index;
    i32 owner_leaf_index_length;
    /**
        The index of the `other` leaves of a query result.
        
        `remarks`
        Use a `query_result` integer to access elements.    
    **/
    i32* other_leaf_index;
    i32 other_lead_index_length;
    /**
        The index of the `owner` leaves of a query result.
        
        `remarks`
        Use a `query_result` integer to access elements.
    **/    
    Soa_Aabb owner_aabb;
    /**
        The index of the `other` leaves of a query result.
        
        `remarks`
        Use a `query_result` integer to access elements.        
    **/
    Soa_Aabb other_aabb;
    /**
        The count of allocated entries from appending.
    **/
    i32 count;
    /**
        The length of all the backing arrays of this instance.
    **/
    i32 length;
    bool is_init;
} Soa_BvhQueryResult;

typedef struct{
    /**
        The leaf indices of the `owner` of an overlap between to leaves.
        
        `remarks`
        Use a `overlap_index` integer to access elements.
    **/
    i32* owner_leaf_index;
    i32 owner_leaf_index_length;
    /**    
        The leaf indices of the `owner` of an overlap between to leaves.

        `remarks`
        Use a `overlap_index` integer to access elements.
    **/
    i32* other_leaf_index;
    i32 other_leaf_index_length;
    /**
        The count of allocated entries from appending.
    **/
    i32 count;
    /**
        The length of all the backing arrays of this instance.
    **/
    i32 length;
} Soa_BvhOverlap;

typedef struct {
    /**
        The indices of the `owner` leaf in the overlaps.
        
        `remarks`
        Elements should be accessed via `overlap_index`.
    **/
    i32* owner_leaf_index;
    /**
        The indices of the `other` leaf in the overlaps. 
        
        `remarks`
        Elements should be accessed via `overlap_index`.    
    **/
    i32* other_leaf_index;
    /**
        The length of elements in the spans of this instance.
    **/
    i32 length;
} BvhOverlapInfo;

typedef struct {
    /**
        The radix sort buffer used when sorting this leaf buffer.
    **/
    RadixSortBuffer radix_sort_buffer;
    /**
        The constructed branches from the inserted leaves.
        
        `remarks`
        Use a `branch_index` integer to access elements.
    **/
    Soa_BvhBranch branches;
    /**    
        The leaves to construct branches from.

        `remarks`
        Use a `leaf_index` integer to get access elements.
    **/
    Soa_BvhLeaf leaves;
    /**
        The morton codes for all leaf centroids.
        
        `remarks`
        `morton_leaf_ids` are vertically associated.    
    **/
    u32* morton_centroids;
    i32 morton_centroids_length;
    /**
        Used as an index for an element in `morton_centroids` to get its associated leaf data in `Leaves`.
        
        `remarks`
        `morton_centroids` are vertically associated.    
    **/
    i32* morton_leaf_ids;
    i32 morton_leaf_ids_length;
    bool is_init;
} BoundingVolumeHierarchy;

typedef struct{
    /**
        The amount of leaves within a category.
        
        `remarks`
        Elements should be accessed by `category_index`.
    **/
    i32* category_stride;
    i32 category_stride_length;
    /**
        The starting indices for a sub category within the `overlap arrays`.
        
        `remarks`
        - `Elements` are arranged in a fixed-stride-like format.
        - `Entry` indices are layed out in a descending order
        - Example: (2,1,0) or (4,3,2,etc...). 
        - The `stride` of each `entry` are the subcategories in a arithermatic series/triangular sum format - in ascending order.
        - Note: this format removes duplicate entries when a storing a category overlaps with another.

        `example`
        (with 3 categories): 
        index = [0], entry = 2, sub categories = 0,1,2
        index = [1], entry = 1, sub categories = 0,1
        index = [2], entry = 0, sub categories = 0
    **/
    i32* sub_category_start_index;
    i32 sub_category_start_index_length;
    /**
        The count of valid overlap elements after a sub category's start index within the `overlap arrays`.

        `remarks`
        - `Elements` are arranged in a fixed-stride-like format.
        - `Entry` indices are layed out in a descending order
        - Example: (2,1,0) or (4,3,2,etc...). 
        - The `stride` of each `entry` are the subcategories in a arithermatic series/triangular sum format - in ascending order.
        - Note: this format removes duplicate entries when a storing a category overlaps with another.
        
        `example`
        (with 3 categories): 
        index = [0], entry = 2, sub categories = 0,1,2
        index = [1], entry = 1, sub categories = 0,1
        index = [2], entry = 0, sub categories = 0
    **/
    i32* sub_category_count;
    i32 sub_category_count_length;    
    /**
        The indices of the `owner` leaf of a given overlap.
        `remarks`  
        - Elements are associated via index to `owner_leaf_index`
        - Elements should be accessed via the calculated index provided by <see cref="GetElementIndex(i32, i32, i32)"/>.

        `example`
        TODO: this is C# and should be translated into C.
        i32 index = GetElementIndex(categoryIndex, subCategoryIndex, categoriesTriangularSum);
        var element = myElements[index];
    **/
    i32* owner_leaf_index;
    i32 owner_leaf_index_length;
    /**
        The indices of the `other` leaf of a given overlap.
        
        `remarks`
        - Elements are associated via index to `owner_leaf_index`
        - Elements should be accessed via the calculated index provided by <see cref="GetElementIndex(i32, i32, i32)"/>.

        `example`
        TODO: this is C# and should be translated into C.
        i32 index = GetElementIndex(categoryIndex, subCategoryIndex, categoriesTriangularSum);
        var element = myElements[index];    
    **/
    i32* other_leaf_index;
    i32 other_leaf_index_length;
    /**
        The triangular sum of the amount of categories the overlap data can be filtered into.
    **/
    i32 categories_triangular_sum;
    /**
        The maximum amount of overlaps this state instance can store.
    **/
    i32 max_overlaps;
    bool is_init;
} BvhCategorisedLeafOverlaps;

/**====================
    defines
====================**//**/


/**
`summary`
    Gets the data that overlaps between two categories.

`returns`
    the data that overlaps between two categories.

`example`
    
    Data* data;
    i32 data_length;
    i32* sub_category_start_indices;
    i32 sub_category_start_indices_length;
    i32* sub_category_counts;
    i32 sub_category_counts_length;
    i32 main_category;
    i32 sub_category;
    i32 categories_triangular_sum;
    Data* overlaps;
    i32 overlaps_length;
    
    CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS(
        data, data_length,
        sub_category_start_indices, sub_category_start_indices_length,
        sub_category_counts, sub_category_counts_length,
        main_category, sub_category, categories_triangular_sum, overlaps, &overlaps_length
    );
**/
#define CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS(                                                                     \
    data, data_length,                                                                                              \
    sub_category_start_indices, sub_category_start_indices_length,                                                  \
    sub_category_counts, sub_category_counts_length,                                                                \
    main_category, sub_category, categories_triangular_sum, out_arr, out_arr_length                                 \
) do {                                                                                                              \
    i32 CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_element_index =                                                      \
        categorised_overlap_array_get_element_index((main_category), (sub_category), (categories_triangular_sum));  \
                                                                                                                    \
    BOUNDS_CHECK(CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_element_index, sub_category_start_indices_length);          \
    i32 CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_start_index                                                          \
        = array.SubCategoryStartIndices[CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_element_index];                      \
                                                                                                                    \
    BOUNDS_CHECK(CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_element_index, sub_category_counts_length);                 \
    i32 CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_count                                                                \
        = sub_category_counts[CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_element_index];                                \
                                                                                                                    \
    out_array = data + CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_start_index;                                          \
    *out_array_length = CATEGORISED_OVERLAP_ARRAY_GET_OVERLAPS_count;                                               \
} while(0)




/**====================
    functions: CategorisedOverlaps.
====================**//**/




void categorised_overlap_array_init(CategorisedOverlapArray* array, MemoryArena* arena, i32 category_count, u32 max_entries, i32 data_element_size){
    ASSERT(!array->is_init, "already init");
    array->categories_triangular_sum = calculate_triangular_sum(category_count);
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->category_stride, &array->category_stride_length, category_count);
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->sub_category_start_index, &array->sub_category_start_index_length, array->categories_triangular_sum);
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->sub_category_count, &array->sub_category_count_length, array->categories_triangular_sum);
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->data, &array->data_length, max_entries * data_element_size);
}


/**
    Calculates the starting indices for each sub category to write to in the data array's. 
**/
void categorised_overlaps_array_build_chunks_decomposed(i32* category_stride, i32 category_stride_length, i32* sub_category_start_index, i32 sub_category_start_index_length, i32 max_elements){
    // get the amount of categories this state instance can filter into.
    i32 category_amount = category_stride_length;
    // the start index of the sub category.
    i32 start_index = 0;
    // the index of the sub category to write the start index to.
    i32 write_index = 0;
    for(i32 category_index = category_amount-1; category_index >= 0; category_index--){ 
        for(i32 sub_category_index = 0; sub_category_index <= category_index; sub_category_index++){
            // set the start index in the overlap arrays.
            BOUNDS_CHECK(write_index, sub_category_start_index_length);
            sub_category_start_index[write_index] = start_index;
            write_index++;

            // add the stride/amount of overlaps that can possibly happen between these categories.
            BOUNDS_CHECK(category_index, category_stride_length);
            BOUNDS_CHECK(sub_category_index, category_stride_length);
            start_index += category_stride[category_index] * category_stride[sub_category_index];
            BOUNDS_CHECK(start_index, max_elements);
        }
    }
}

/**
    Calculates the starting indices for each sub category to write to in the data array's. 
**/
inline void categorised_overlaps_array_build_chunks(CategorisedOverlapArray* array){
    categorised_overlaps_array_build_chunks_decomposed(array->category_stride, array->category_stride_length, array->sub_category_start_index, array->sub_category_start_index_length, array->max_elements);
}

/**
    Gets the starting element index for a sub category within a state instances sub category arrays.
    
`parameters`
    `categoryIndex`: the category index.
    `subCategoryIndex`: the sub category index.

`returns`
    the starting element index in the sub category arrays.

**/
i32 categorised_overlaps_array_get_element_index(i32 category_index, i32 sub_category_index , i32 categories_triangular_sum){
    /**
        ensure that cat is always the maxmimum.
        Note: if the 'cat' was the min and the 'sub' was the max, the calculated
        index would always be incorrect (due to the formatting of the sub category arrays).    
    **/
    i32 cat = MAX(category_index, sub_category_index);
    i32 sub = MIN(category_index, sub_category_index);

    /**    
        this add one is very important, do not remove this EVER!!!!
        Note: this is because categoryIndex is zero indexed, where as 
        CalculateTriangularSum() is indexed by one.
    **/
    cat+=1;

    i32 offset = calculate_triangular_sum(cat);
    offset -= sub;
    return categories_triangular_sum - offset;        
}

/**
    Increments count of elements in a sub category.

    `parameters`
    `write_index`: output for the index that should now be written to with valid data.

    `returns`
    true, if the count was incremented; otherwise false.
**/
bool categorised_overlaps_array_increment_sub_category_count(
    i32* category_stride, i32 category_stride_length, 
    i32* sub_category_counts, i32 sub_category_counts_length,
    i32* sub_category_start_indices, i32 sub_category_start_indices_length,
    i32 categories_triangular_sum, i32 main_category, i32 sub_category, i32* out_write_index
){
    i32 element_index = categorised_overlaps_array_get_element_index(main_category, sub_category, categories_triangular_sum);
    
    BOUNDS_CHECK(element_index, sub_category_start_indices_length);
    i32 start_index = sub_category_start_indices[element_index];
    
    BOUNDS_CHECK(element_index, sub_category_counts_length);    
    i32* count = &sub_category_counts[element_index];

    BOUNDS_CHECK(main_category, category_stride_length);
    i32 category_stride_a = category_stride[main_category];

    BOUNDS_CHECK(sub_category, category_stride_length);
    i32 category_stride_b = category_stride[sub_category];
    
    if ((category_stride_a * category_stride_b) - 1 < *count){
        return false;
    }

    *out_write_index = start_index + *count;
    count+=1;
    return true;
}

/**
    Sets the count values in a `sub_category_counts` array to zero.
**/
inline void categorised_overlaps_array_clear_counts_decomposed(i32* sub_category_counts, i32 sub_category_counts_length){
    ZERO_MEMORY(sub_category_counts, sizeof(*sub_category_counts) * sub_category_counts_length);
}

inline void categorised_overlaps_array_clear_counts(CategorisedOverlapArray* array){
    categorised_overlaps_array_clear_counts_decomposed(array->sub_category_count, array->sub_category_count_length);
}

/** 
    Appends data to a pair of overlapping categories.

    `parameters`
    `data`: the data to copy into the array.
    `data_size`: the size of the data in bytes.
    `array`: the array instance to append to.

    `returns`
    true, if the data was successfuly appended; otherwise false.
**/ 
void categorised_overlaps_array_push(CategorisedOverlapArray* array, const void* data, size_t data_size, i32 main_category, i32 sub_category){
    i32 element_idx = categorised_overlaps_array_get_element_index(main_category, sub_category, array->categories_triangular_sum);
    BOUNDS_CHECK(element_idx, array->sub_category_start_index_length);
    i32 start_idx = array->sub_category_count[element_idx];
    BOUNDS_CHECK(element_idx, array->sub_category_count_length);
    i32* count = &array->sub_category_count[element_idx];

    // the amount of elements after the start_idx (inclusive) that is the overlapping data of 'main_category' and 'sub_category'.
    BOUNDS_CHECK(main_category, array->category_stride_length);
    BOUNDS_CHECK(sub_category, array->category_stride_length);
    i32 overlap_stride = (array->category_stride[main_category] * array->category_stride[sub_category]);
    BOUNDS_CHECK(*count, overlap_stride);

    i32 write_index = start_idx + *count;
    BOUNDS_CHECK(write_index, array->data_length);
    COPY_MEMORY(&array->data[write_index], data, data_size);
    *count++;
}




/**====================
    functions: Soa_BvhLeaf
====================**//**/




void soa_bvh_leaf_init(Soa_BvhLeaf* soa, MemoryArena* arena, i32 length){
    ASSERT(!soa->is_init, "already init");
    soa_aabb_init(&soa->aabb, arena, length);
    soa_vector2_init(&soa->centroid, arena, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->branch_index, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->category, &soa->length, length);
    soa->length = length;
    soa->is_init = true;
}

/** 
    Appends an entry into a soa at the soa instance's `count` index.    
**/ 
void soa_bvh_leaf_push(
    Soa_BvhLeaf* soa, 
    f32 leaf_min_x, f32 leaf_min_y, f32 leaf_max_x, f32 leaf_max_y,
    f32 leaf_centroid_x, f32 leaf_centroid_y, i32 leaf_category
){
    BOUNDS_CHECK(soa->count, soa->length);
    soa_aabb_push(&soa->aabb, leaf_min_x, leaf_min_y, leaf_max_x, leaf_max_y);
    soa_vector2_push(&soa->centroid, leaf_centroid_x, leaf_centroid_y);
    soa->category[soa->count] = leaf_category;
    soa->count += 1;
}

/**
    Sets a soa instance's `count` to zero.
**/
void soa_bvh_leaf_clear(Soa_BvhLeaf* soa){
    soa_aabb_reset_count(&soa->aabb);
    soa_vector2_reset_count(&soa->centroid);
    soa->count = 0;
}




/**====================
    functions: Soa_BvhBranch.
====================**//**/




void soa_bvh_branch_init(Soa_BvhBranch* soa, MemoryArena* arena, i32 length){
    ASSERT(!soa->is_init, "already init");
    soa_aabb_init(&soa->aabb, arena, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->left_leaf_index, &soa->length, length);    
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->right_leaf_index, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->subtree_stride, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->leaf_count, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->parent_index, &soa->length, length);
    soa->length = length;
    soa->is_init = true;
}

/** 
    Inserts an entry into a soa instance.    
**/ 
void soa_bvh_branch_insert(Soa_BvhBranch* soa, i32 element_index, f32 branch_min_x, f32 branch_min_y, f32 branch_max_x, 
    f32 branch_max_y, i32 branch_left_leaf_index, i32 branch_right_leaf_index, i32 branch_subtree_stride, i32 branch_leaf_count, i32 branch_parent_index
){
    BOUNDS_CHECK(element_index, soa->aabb.length);
    soa->aabb.min_x[element_index] = branch_min_x;
    soa->aabb.min_y[element_index] = branch_min_y;
    soa->aabb.max_x[element_index] = branch_max_x;
    soa->aabb.max_y[element_index] = branch_max_y;
    BOUNDS_CHECK(element_index, soa->length);
    soa->left_leaf_index[element_index] = branch_left_leaf_index;
    soa->right_leaf_index[element_index] = branch_right_leaf_index;
    soa->subtree_stride[element_index] = branch_subtree_stride;
    soa->leaf_count[element_index] = branch_leaf_count;       
    soa->parent_index[element_index] = branch_parent_index;
}

void soa_bvh_branch_clear(Soa_BvhBranch* soa){
    soa->count = 0;
}




/**====================
    functions: BvhCategorisedLeafOverlaps
====================**//**/




/**
    `parameters`
    `category_count`: the amount of categories the overlap data can be filtered into.
    `max_overlaps`: the maximum amount of overlap data that this instance can hold.
**/
void bvh_categorised_leaf_overlaps_init(BvhCategorisedLeafOverlaps* overlaps, MemoryArena* arena, i32 category_count, i32 max_overlaps){
    ASSERT(!overlaps->is_init, "already init.");
    i32 triangle_sum = calculate_triangular_sum(category_count);
    overlaps->categories_triangular_sum = triangle_sum;
    MEMORY_ARENA_ALLOC_ARRAY(arena, overlaps->category_stride, &overlaps->category_stride_length, category_count);
    MEMORY_ARENA_ALLOC_ARRAY(arena, overlaps->sub_category_start_index, &overlaps->sub_category_start_index_length, triangle_sum);
    MEMORY_ARENA_ALLOC_ARRAY(arena, overlaps->sub_category_count, &overlaps->sub_category_count_length, triangle_sum);
    MEMORY_ARENA_ALLOC_ARRAY(arena, overlaps->owner_leaf_index, &overlaps->owner_leaf_index_length, max_overlaps);
    MEMORY_ARENA_ALLOC_ARRAY(arena, overlaps->other_leaf_index, &overlaps->other_leaf_index_length, max_overlaps);
    overlaps->is_init = true;
}

/**
    Calculates the starting indices for each sub category to write to in the data array's.
**/
void bvh_categorised_leaf_overlaps_build_chunks(BvhCategorisedLeafOverlaps* state){
    categorised_overlaps_array_build_chunks_decomposed(
        state->category_stride, state->category_stride_length, 
        state->sub_category_start_index, state->sub_category_start_index_length,
        state->max_overlaps
    );
}

/**
`summary`
    Appends an overlap to an instance.

`parameters`
    `overlaps`: the instance to append to.
    `ownerLeafIndex`: the index of the leaf that is the `owner` of the overlap.
    `otherLeafIndex`: the index of the leaf that is the `other` of the overlap.
    `ownerCategory`: the category of the `owner` leaf.
    `otherCategory`: the category of the `other` leaf.

`returns`
    true, if the overlap was successfully appended; otherwise false.

**/
bool bvh_categorised_leaf_overlaps_push(BvhCategorisedLeafOverlaps* overlaps, i32 owner_leaf_index, i32 other_leaf_index, i32 owner_category, i32 other_category){
    i32 write_index = 0;
    bool success = categorised_overlaps_array_increment_sub_category_count(
        overlaps->category_stride, overlaps->category_stride_length, 
        overlaps->sub_category_count, overlaps->sub_category_count_length, 
        overlaps->sub_category_start_index, overlaps->sub_category_start_index_length, 
        overlaps->categories_triangular_sum, owner_category, other_category, &write_index
    );
    if(success){
        // write the data to the index.
        BOUNDS_CHECK(write_index, overlaps->owner_leaf_index_length);
        overlaps->owner_leaf_index[write_index] = owner_leaf_index;
        BOUNDS_CHECK(write_index, overlaps->other_leaf_index_length);
        overlaps->other_leaf_index[write_index] = other_leaf_index;
        return true;
    }
    return false;
}

/**
    Sets the count values in a overlaps instance to zero. 
**/
inline void bvh_categorised_leaf_overlaps_clear(BvhCategorisedLeafOverlaps* overlaps){
    ZERO_MEMORY(overlaps->sub_category_count, sizeof(*overlaps->sub_category_count) * overlaps->sub_category_count_length);
}

/** 
    
    Gets the overlap info between two categories.

    `returns`
    the data that overlaps between the two categories.
**/ 
BvhOverlapInfo bvh_categorised_leaf_overlaps_get_overlaps(BvhCategorisedLeafOverlaps overlaps, i32 main_category, i32 sub_category){
    i32 element_index = categorised_overlaps_array_get_element_index(main_category, sub_category, overlaps.categories_triangular_sum);
    
    BOUNDS_CHECK(element_index, overlaps.sub_category_start_index_length);
    i32 start_index = overlaps.sub_category_start_index[element_index];

    BOUNDS_CHECK(element_index, overlaps.sub_category_count_length);
    i32 count = overlaps.sub_category_count[element_index];

    BOUNDS_CHECK(start_index, overlaps.owner_leaf_index_length);
    i32* owner_ptr = overlaps.owner_leaf_index + start_index;

    BOUNDS_CHECK(start_index, overlaps.other_leaf_index_length);
    i32* other_ptr = overlaps.other_leaf_index + start_index;

    return (BvhOverlapInfo){
        .other_leaf_index = other_ptr,
        .owner_leaf_index = owner_ptr,
        .length = count
    };
}




/**====================
    functions: BoundingVolumeHierarchy 
====================**//**/




void bvh_init(BoundingVolumeHierarchy* bvh, MemoryArena* arena, i32 length){
    ASSERT(!bvh->is_init, "already init");
    // mapping each spatial pair onto one another (without duplicates) gives a length * 2 possible spatial pairs.
    i32 branches_length = length*2;
    soa_bvh_leaf_init(&bvh->leaves, arena, length);
    soa_bvh_branch_init(&bvh->branches, arena, branches_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, bvh->morton_centroids, &bvh->morton_centroids_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, bvh->morton_leaf_ids, &bvh->morton_leaf_ids_length, length);
    radix_sort_buffer_init(&bvh->radix_sort_buffer, arena, length);
    bvh->is_init = true;
}

/**
    Sets the count of the bounding volume hierarchy's internal arrays to zero.
    
    `parameters`
    `bvh`: the bvh to clear.
**/
void bvh_clear(BoundingVolumeHierarchy* bvh){
    bvh->leaves.count = 0;
    bvh->branches.count = 0;
}

/** 
    A recursive function that constructs branches from a given data set of leaves.
    
    `remarks`
    this is a destructive process on <paramref name="branches"/>, entries within the soa instance will be overwritten.

    `branches`: output soa instance for writing generated branches to.
    `leaf_index`: An array of leaf indices sorted so that neighbouring entries are neighbouring leaves (within close proximity) in world-space.
    `leaf_min_x`: the x-component of all leaves minimum vertices.
    `leaf_min_y`: the y-component of all leaves minimum vertices.
    `leaf_max_x`: the x-component of all leaves maximum vertices.
    `leaf_max_y`: the y-component of all leaves maximum vertices.
    `leaf_branch_index`: an array containing the branch indices that all leaves are parented to.
    `start`: the index to start at when processing the leaf indices.
    `length`: the total amount of leaf indices to process after `start`
    `parent_index`: the index of the branch that this newly constructed branch will be parented to.
    `write_index`: the index of the most recently written entry in `branches`.
    `aabb_min_x`: the x-component of the minimum vertex of the currently constructed branch.
    `aabb_min_y`: the y-component of the minimum vertex of the currently constructed branch.
    `aabb_max_x`: the x-component of the maximum vertex of the currently constructed branch.
    `aabb_max_y`: the y-component of the maximum vertex of the currently constructed branch.    
**/ 
void bvh_construct_branches_recurssive(
    Soa_BvhBranch* branches, 
    i32* leaf_index, i32 leaf_index_length, 
    f32* leaf_min_x, i32 leaf_min_x_length,
    f32* leaf_min_y, i32 leaf_min_y_length,
    f32* leaf_max_x, i32 leaf_max_x_length,
    f32* leaf_max_y, i32 leaf_max_y_length,
    i32* leaf_branch_index, i32 leaf_branch_index_length,
    i32 start, i32 length, i32 parent_index,
    i32* write_index, f32* aabb_min_x, f32* aabb_min_y, f32* aabb_max_x, f32* aabb_max_y
){
    // reserve space.
    i32 branch_index = *write_index+=1;

    /** 
        leaf.
    **/ 
    if(length <= 2){

        // build leaf aabb.
        BOUNDS_CHECK(start, leaf_index_length);
        i32 left_leaf_index = leaf_index[start];
        i32 right_leaf_index;
        i32 leaf_count;
        
        // combine the sibling leaf if there is one.
        if(length == 2){

            i32 index = start+1;
            BOUNDS_CHECK(index, leaf_index_length);
            right_leaf_index = leaf_index[start+1];
            
            BOUNDS_CHECK(right_leaf_index, leaf_min_x_length);
            BOUNDS_CHECK(right_leaf_index, leaf_min_y_length);
            BOUNDS_CHECK(right_leaf_index, leaf_max_x_length);
            BOUNDS_CHECK(right_leaf_index, leaf_max_y_length);
            aabb_combine_scalar(
                *aabb_min_x, *aabb_min_y, *aabb_max_x, *aabb_min_y, 
                leaf_min_x[right_leaf_index], leaf_min_y[right_leaf_index], leaf_max_x[right_leaf_index], leaf_max_y[right_leaf_index], 
                aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y
            );

            // set the leaf branch.
            BOUNDS_CHECK(right_leaf_index, leaf_branch_index_length);
            leaf_branch_index[right_leaf_index] = branch_index;
            
            leaf_count = 2;
        }
        else{
            leaf_count = 1;
            right_leaf_index = 0;
        }

        /** 
            insert the leaf.
            note that subtree size for leaves is always one as subtree size is inclusive of then entry; and a leaf is the final in a branch chain. 
        **/ 
        soa_bvh_branch_insert(branches, branch_index, *aabb_min_x, *aabb_min_y, *aabb_max_x, *aabb_max_y, left_leaf_index, right_leaf_index, 1, leaf_count, parent_index);
    }
    
    // internal branch.
    else{
        // split at the mid point.
        i32 mid = length/2;
        
        i32 left_start = start;
        i32 left_length = mid;
        f32 left_min_x = 0;
        f32 left_min_y = 0;
        f32 left_max_x = 0;
        f32 left_max_y = 0;

        i32 right_start = start + mid;
        i32 right_length = length - mid;
        f32 right_min_x = 0;
        f32 right_min_y = 0;
        f32 right_max_x = 0;
        f32 right_max_y = 0;

        /** 
            recurse (children are written contiguously after parent).       
        **/ 
        parent_index += 1;

        // left branch.
        bvh_construct_branches_recurssive(
            branches, 
            leaf_index, leaf_index_length, 
            leaf_min_x, leaf_min_x_length, 
            leaf_min_y, leaf_min_y_length, 
            leaf_max_x, leaf_max_x_length, 
            leaf_max_y, leaf_max_y_length, 
            leaf_branch_index, leaf_branch_index_length, 
            left_start, left_length, parent_index, write_index, 
            &left_min_x, &left_min_y, &left_max_x, &left_max_y
        );

        // right branch.
        bvh_construct_branches_recurssive(
            branches, 
            leaf_index, leaf_index_length, 
            leaf_min_x, leaf_min_x_length, 
            leaf_min_y, leaf_min_y_length, 
            leaf_max_x, leaf_max_x_length, 
            leaf_max_y, leaf_max_y_length, 
            leaf_branch_index, leaf_branch_index_length, 
            right_start, right_length, parent_index, write_index, 
            &right_min_x, &right_min_y, &right_max_x, &right_max_y
        );

        // get the aabb of both branches.
        aabb_combine_scalar(
            left_min_x, left_min_y, left_max_x, left_max_y,
            right_min_x, right_min_y, right_max_x, right_max_y,
            aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y            
        );

        // set the sub tree.
        i32 subtree_stride = *write_index - branch_index;

        // set the branch
        soa_bvh_branch_insert(branches, branch_index, *aabb_min_x, *aabb_min_y, *aabb_max_x, *aabb_max_y, 0, 0, subtree_stride, 0, parent_index);
    }
}

/**
    Constructs a tree of branches from the leaves store in a bvh instance.
**/
void bvh_construct_tree(BoundingVolumeHierarchy* bvh){
    ASSERT(bvh->is_init, "not init");
    soa_bvh_branch_clear(&bvh->branches);
    // get the spatial data for morton code calculations.
    f32 min_x = F32_MAX;
    f32 min_y = F32_MAX;
    f32 max_x = F32_MIN;
    f32 max_y = F32_MIN;
    for(i32 i = 0; i < bvh->leaves.count; i++){
        f32 cx = bvh->leaves.centroid.x[i];
        f32 cy = bvh->leaves.centroid.y[i];
        min_x = (cx < min_x) ? cx : min_x;
        max_x = (cx > max_x) ? cx : max_x;
        min_y = (cy < min_y) ? cy : min_y;
        max_y = (cy > max_y) ? cy : max_y;
    }
    f32 range_x = ABS(max_x - min_x);
    f32 range_y = ABS(max_y - min_y);

    //get the morton code for sorting each of the centroids.
    f32 scale_x = 0;
    f32 scale_y = 0;
    morton_code_calc_scale_factor(range_x, range_y, &scale_x, &scale_y);
    for(i32 i = 0; i < bvh->leaves.count; i++){
        BOUNDS_CHECK(i, bvh->morton_centroids_length);
        BOUNDS_CHECK(i, bvh->leaves.centroid.length);
        bvh->morton_centroids[i] = morton_code_calc_morton_code(bvh->leaves.centroid.x[i], bvh->leaves.centroid.y[i], min_x, min_y, scale_x, scale_y);
    }

    /** 
        TODO: (nich s)
        this should be done at i32ialisation.
    **/ 
    // reset leaf indices.
    for(i32 i = 0; i < bvh->leaves.count; i++){
        BOUNDS_CHECK(i, bvh->morton_leaf_ids_length);
        bvh->morton_leaf_ids[i] = i;
    }

    radix_buffer_sort_iasc(&bvh->radix_sort_buffer, bvh->morton_centroids, bvh->morton_centroids_length, bvh->morton_leaf_ids, bvh->morton_leaf_ids_length, bvh->leaves.count);

    i32 branch_count = 0;
    /** 
        TODO: (nich s)
        this will have to change to zero when we start enforcing Nils.
    **/ 
    i32 parent_index = -1;
    f32 aabb_min_x = 0;
    f32 aabb_min_y = 0;
    f32 aabb_max_x = 0;
    f32 aabb_max_y = 0;

    bvh_construct_branches_recurssive(
        &bvh->branches, 
        bvh->morton_leaf_ids, bvh->morton_leaf_ids_length, 
        bvh->leaves.aabb.min_x, bvh->leaves.length, 
        bvh->leaves.aabb.min_y, bvh->leaves.length, 
        bvh->leaves.aabb.max_x, bvh->leaves.length, 
        bvh->leaves.aabb.max_y, bvh->leaves.length,
        bvh->leaves.branch_index, bvh->leaves.length,
        0, bvh->leaves.count, parent_index, &branch_count, 
        &aabb_min_x, &aabb_min_y, &aabb_max_x, &aabb_max_y
    );

    /** 
        we set the branch count manually as the branches are inserted into the soa manually
        without using the push function; this is okay as branch insertion in Construct branches
        inserts branches in a 'subtree size' relative order for each branch; meaning, at the end of the 
        construction of all branches, the data is contiguous (no holes in the array entries).
    **/ 

    bvh->branches.count = branch_count;
}

/** 
    Finds any leaves that overlap with eachother within a set of constructed branches and leaves.
    
    `remarks`
    there are no duplicate elements in the output overlap data.

    `parameters`
    `branches`: the constructed tree of branches to query.
    `leaves`: the leaf data associated with the branches.
    `overlaps`: output for the overlap data.
**/ 
void bvh_get_overlaps(BoundingVolumeHierarchy bvh, BvhCategorisedLeafOverlaps* overlaps){
    // clear any garbage data.
    bvh_categorised_leaf_overlaps_clear(overlaps);

    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;    
    i32 other_leaf_idx;

    for(i32 owner_leaf_idx = 0; owner_leaf_idx < bvh.leaves.count; owner_leaf_idx++){
        BOUNDS_CHECK(owner_leaf_idx, bvh.leaves.aabb.length);
        min_x = bvh.leaves.aabb.min_x[owner_leaf_idx];
        min_y = bvh.leaves.aabb.min_y[owner_leaf_idx];
        max_x = bvh.leaves.aabb.max_x[owner_leaf_idx];
        max_y = bvh.leaves.aabb.max_y[owner_leaf_idx];

        i32 other_branch_idx = 0;
        while(other_branch_idx < bvh.branches.count){
            bool overlaps = aabb_overlaps_scalar(
                min_x, bvh.leaves.aabb.min_x[other_branch_idx],
                min_y, bvh.leaves.aabb.min_y[other_branch_idx],
                max_x, bvh.leaves.aabb.max_x[other_branch_idx],
                max_y, bvh.leaves.aabb.max_y[other_branch_idx]
            );
            
            if(!overlaps){
                // skip the entire subtree.
                BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
                other_branch_idx += bvh.branches.subtree_stride[other_branch_idx];
                continue;
            }

            BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
            i32 leaf_count = bvh.branches.leaf_count[other_branch_idx];
        }

        BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
        i32 leaf_count = bvh.branches.leaf_count[other_branch_idx];

        switch(leaf_count){
            default:{
                ASSERT(false, "invalid leaf count");
            }break;

            case 0:{
                // do nothing...
            }break;

            case 1:{
                // left leaf index should always be set to a leaf index for branches with leaf(s) attatched; it is the default leaf to set first.
                BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
                other_leaf_idx = bvh.branches.left_leaf_index[other_branch_idx];
                
                BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                bool leaf_overlaps_query_area = 
                    aabb_overlaps_scalar(
                        min_x, bvh.leaves.aabb.min_x[other_leaf_idx],
                        min_y, bvh.leaves.aabb.min_y[other_leaf_idx],
                        max_x, bvh.leaves.aabb.max_x[other_leaf_idx],
                        max_y, bvh.leaves.aabb.max_y[other_leaf_idx]
                    ); 
                
                if(owner_leaf_idx < other_leaf_idx && leaf_overlaps_query_area){
                    BOUNDS_CHECK(owner_leaf_idx, bvh.leaves.length);
                    BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                    bvh_categorised_leaf_overlaps_push(
                        overlaps, owner_leaf_idx, other_leaf_idx, bvh.leaves.category[owner_leaf_idx], bvh.leaves.category[other_leaf_idx]
                    );
                }
            }break;

            case 2:{
                { // left leaf.
                    BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
                    other_leaf_idx = bvh.branches.left_leaf_index[other_branch_idx];
                    
                    BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                    bool leaf_overlaps_query_area = 
                        aabb_overlaps_scalar(
                            min_x, bvh.leaves.aabb.min_x[other_leaf_idx],
                            min_y, bvh.leaves.aabb.min_y[other_leaf_idx],
                            max_x, bvh.leaves.aabb.max_x[other_leaf_idx],
                            max_y, bvh.leaves.aabb.max_y[other_leaf_idx]
                        ); 
                    
                    if(owner_leaf_idx < other_leaf_idx && leaf_overlaps_query_area){
                        BOUNDS_CHECK(owner_leaf_idx, bvh.leaves.length);
                        BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                        bvh_categorised_leaf_overlaps_push(
                            overlaps, owner_leaf_idx, other_leaf_idx, bvh.leaves.category[owner_leaf_idx], bvh.leaves.category[other_leaf_idx]
                        );
                    }
                }
                { // right leaf.
                    BOUNDS_CHECK(other_branch_idx, bvh.branches.length);
                    other_leaf_idx = bvh.branches.right_leaf_index[other_branch_idx];
                    
                    BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                    bool leaf_overlaps_query_area = 
                        aabb_overlaps_scalar(
                            min_x, bvh.leaves.aabb.min_x[other_leaf_idx],
                            min_y, bvh.leaves.aabb.min_y[other_leaf_idx],
                            max_x, bvh.leaves.aabb.max_x[other_leaf_idx],
                            max_y, bvh.leaves.aabb.max_y[other_leaf_idx]
                        ); 
                    
                    if(owner_leaf_idx < other_leaf_idx && leaf_overlaps_query_area){
                        BOUNDS_CHECK(owner_leaf_idx, bvh.leaves.length);
                        BOUNDS_CHECK(other_leaf_idx, bvh.leaves.length);
                        bvh_categorised_leaf_overlaps_push(
                            overlaps, owner_leaf_idx, other_leaf_idx, bvh.leaves.category[owner_leaf_idx], bvh.leaves.category[other_leaf_idx]
                        );
                    }
                }
            }break;
        }

        other_branch_idx += 1;
    }
}




/**====================
    functions: FixedStrideArray.
====================**//**/




/**
    `summary`
    Gets the element index of an entry's element in a fixed stride array. 

    `parameters`
    - `chunk_index`: the index of the `chunk` in the array.
    - `stride': the stride of each `chunk` in the array.
    - `chunk_element_index`: the index of the element in the `chunk`.

    `returns`
    the index in the fixed stride array to the entry's element.
**/
inline i32 fixed_stride_array_get_element_idx(i32 chunk_idx, i32 chunk_stride, i32 chunk_element_idx){
    return chunk_idx * chunk_stride + chunk_element_idx;
}

/**
    `summary`
    Appends a value to a destination array.

    `parameters`
    `entryIndex`: the index of the entry to append to.

    `example`
    Data* elements;
    i32 elements_length;
    i32* chunk_element_count;
    i32 chunk_element_count_length;
    i32 chunk_stride;
    i32 chunk_index;
    Data value;
    FIXED_STRIDE_ARRAY_PUSH(elements, elements_length, chunk_element_count, chunk_element_count_length, chunk_stride, chunk_index, value);
`returns`
**/
#define FIXED_STRIDE_ARRAY_PUSH(elements, elements_length, chunk_element_count, chunk_element_count_length, chunk_stride, chunk_index, value) do {      \
    BOUNDS_CHECK((chunk_index), (chunk_element_count_length));                                                                                          \
    i32* FIXED_STRIDE_ARRAY_PUSH_count = &(chunk_element_count)[(chunk_index)];                                                                         \
    i32 FIXED_STRIDE_ARRAY_PUSH_next = (*FIXED_STRIDE_ARRAY_PUSH_count) + 1;                                                                               \
    ASSERT(FIXED_STRIDE_ARRAY_PUSH_next <= (chunk_stride), "memory limit hit.");                                                                        \
    i32 FIXED_STRIDE_ARRAY_PUSH_element_index = fixed_stride_array_get_element_idx((chunk_index), (chunk_stride), *FIXED_STRIDE_ARRAY_PUSH_count);      \
    BOUNDS_CHECK(FIXED_STRIDE_ARRAY_PUSH_element_index, (elements_length));                                                                             \
    (elements)[FIXED_STRIDE_ARRAY_PUSH_element_index] = (value);                                                                                        \
    *FIXED_STRIDE_ARRAY_PUSH_count = FIXED_STRIDE_ARRAY_PUSH_next;                                                                                      \
} while(0)

/**
/// <summary>
///     Removes an element at a specified index from an array.
/// </summary>
/// <typeparam name="T`: </typeparam>
/// <param name="entryIndex`: the index of the entry to remove from.
/// <param name="elementIndex`: the index - relative to the entry - of the element to remove.
/// <returns>true, if the value was successfully removed; otherwise false.</returns>
[MethodImpl(MethodImplOptions.AggressiveInlining | MethodImplOptions.AggressiveOptimization)]
public static bool RemoveAt<T>(
    ref FixedStrideSwapbackArray<T> array, int stride, int entryIndex, int elementIndex
) where T : unmanaged{

    ref int count = ref array.EntryCounts[entryIndex];
    
    if(count == 0 || count < elementIndex){
        return false;
    }
    
    count--;
    
    // set the data to remove with the last entry.
    array.EntryElements[GetFixedStrideArrayElementIndex(entryIndex, stride, elementIndex)] = array.EntryElements[GetFixedStrideArrayElementIndex(entryIndex, stride, count)];
    
    return true;
}
**/