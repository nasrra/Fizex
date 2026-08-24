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

void intrusive_list_init(IntrusiveList* list, MemoryArena* arena, i32 length, bool preserve_root_order){
    ASSERT(!list->is_init, "already init");
    ASSERT(length >= 1, "length out of bounds");

    MEMORY_ARENA_ALLOC_ARRAY(arena, list->node, &list->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, list->root_index, &list->length, length);

    ARRAY_PUSH(list->root_index, list->length, &list->root_index_count, 0);
    list->preserve_root_order = preserve_root_order;
    
    list->is_init = true;
}

bool intrusive_list_add_root(IntrusiveList* list, i32 node_index){
    
    // node cannot be the Nil.
    if(node_index == 0){
        ASSERT(false, "nil element");
        return false;
    }

    BOUNDS_CHECK(node_index, list->length);
    IntrusiveListNode* node = &list->node[node_index];
    
    if(node->in_tree){
        return false;
    }

    node->root_dense_index = list->root_index_count;
    node->next_sibling = node_index;
    node->previous_sibling = node_index;
    node->in_tree = true;
    ARRAY_PUSH(list->root_index, list->length, &list->root_index_count, node_index);
    
    return true;
}

/**
    Adds a node to the tree.
    
    `remarks`
    - if `parent_index` is `0`, this will become a root node.

    `returns`
    true, if successfully added to the tree; otherwise false if already added.
**/
bool intrusive_list_add_branch(IntrusiveList* list, i32 node_index, i32 parent_index){
    // node cannot be nil.
    if(node_index == 0){
        ASSERT(false, "nil element");
        return false;
    }

    if(parent_index == 0){
        return intrusive_list_add_root(list, node_index);
    }

    BOUNDS_CHECK(node_index, list->length);
    IntrusiveListNode* node = &list->node[node_index];

    if(node->in_tree){
        return false;
    }

    IntrusiveListNode* parent = &list->node[parent_index];
    if(parent->in_tree == false){
        ASSERT(false, "parent is not in tree");
        return false;
    }
    node->parent = parent_index;
    // only set if it is pointing to the Nil.
    if(parent->first_child == 0){
        // node has no other siblings (as it is the first child).
        node->next_sibling = node_index;
        node->previous_sibling = node_index;
    }
    else{
        // get the last child.
        BOUNDS_CHECK(parent->first_child, list->length);
        i32 last_child_index = list->node[parent->first_child].previous_sibling;
        
        BOUNDS_CHECK(last_child_index, list->length);
        IntrusiveListNode* last_child = &list->node[last_child_index];

        // get the first child.
        i32 first_child_index = parent->first_child;

        BOUNDS_CHECK(first_child_index, list->length);
        IntrusiveListNode* first_child = &list->node[first_child_index];

        // connect last child to the new node.
        last_child->next_sibling = node_index;
        node->previous_sibling = last_child_index;
        node->next_sibling = first_child_index;
        first_child->previous_sibling = node_index;
    }

    node->in_tree = true;
    return true;
}

/**
    `returns`
    true, if successfully removed from the tree; otherwise false if already removed.
**/
bool intrusive_list_remove_node(IntrusiveList* list, i32 node_index){
    if(node_index == 0){
        ASSERT(false, "nil element");
        return false;
    }

    BOUNDS_CHECK(node_index, list->length);
    IntrusiveListNode* node = &list->node[node_index];

    if(node->in_tree == false){
        return false;
    }

    i32 parent_index = node->parent;
    i32 first_child_index = node->first_child;

    // deallocate from parent.
    if(parent_index != 0){
        node->parent = 0;
        BOUNDS_CHECK(parent_index, list->length);
        IntrusiveListNode* parent = &list->node[parent_index];

        // if this node doesnt have any children;
        if(node->first_child == 0){
            // nil the parents child.
            if(parent->first_child == node_index){
                parent->first_child = 0;
            }
        }
        else{
            
            // move the children to the parent.
            if(parent->first_child == node_index){
                parent->first_child = node->first_child;

                // dealloc from children by setting theirparent to this node's parent.
                BOUNDS_CHECK(node->first_child, list->length);
                IntrusiveListNode* child = &list->node[node->first_child];

                while(true){
                    child->parent = parent_index;
                    i32 next_sibling_index = child->next_sibling;

                    if(next_sibling_index == first_child_index){
                        break;
                    }

                    BOUNDS_CHECK(next_sibling_index, list->length);
                    *child = list->node[next_sibling_index];
                }
            }
            
            // append this node's children to it's parent's children.
            else{
                i32 parent_first_child_index = parent->first_child;
                BOUNDS_CHECK(parent_first_child_index, list->length);
                IntrusiveListNode* parent_first_child = &list->node[parent_first_child_index];

                i32 parent_last_child_index = parent_first_child->previous_sibling;
                BOUNDS_CHECK(parent_last_child_index, list->length);
                IntrusiveListNode* parent_last_child = &list->node[parent_last_child_index];                

                parent_last_child->next_sibling = node->first_child;

                i32 current_sibling_index = node->first_child;
                BOUNDS_CHECK(current_sibling_index, list->length);
                IntrusiveListNode* child = &list->node[current_sibling_index];
                child->previous_sibling = parent_last_child_index;

                while(true){
                    child->parent = parent_index;
                    i32 next_sibling_index = child->next_sibling;
                    if(next_sibling_index == first_child_index){
                        child->next_sibling = parent_first_child_index;
                        parent_first_child->previous_sibling = current_sibling_index;
                        break;
                    }
                    current_sibling_index = next_sibling_index;
                    BOUNDS_CHECK(next_sibling_index, list->length);
                    child = &list->node[next_sibling_index]; 
                } 

                // don't perform sibling deallocation at the end of this function.
                // as the re-ordering of siblings in the parent has aready done this.
                // goto End;
            }
        }
    }
    else{
        switch(list->preserve_root_order){
            /**
                ordererd removal: move all root node dense indices backward. 
            **/
            case true:{
                for(i32 i = node->root_dense_index+1; i < list->root_index_count; i++){
                    BOUNDS_CHECK(i, list->length);
                    IntrusiveListNode* next_root = &list->node[list->root_index[i]];
                    next_root->root_dense_index -= 1;
                }
                // remove the root index.
                ARRAY_ORDERED_REMOVE_AT(list->root_index, list->length, &list->root_index_count, node->root_dense_index);
            }break;
            /**
                un-ordererd removal: swap last with dealloc.                
            **/
            case false:{
                /**
                    remove the node from the roots array.
                    performing the dense index swap as well.                
                **/
                IntrusiveListNode* last_root = &list->node[list->root_index[list->root_index_count-1]];
                last_root->root_dense_index = node->root_dense_index;
                ARRAY_UNORDERED_REMOVE_AT(list->root_index, list->length, &list->root_index_count, node->root_dense_index);
                node->root_dense_index = 0;
            }break;
        }
        
        // dealloc from children by making them root nodes in the tree.
        if(first_child_index != 0){
            i32 current_sibling_index = first_child_index;
            BOUNDS_CHECK(current_sibling_index, list->length);
            IntrusiveListNode* child = &list->node[current_sibling_index];

            while(true){
                child->parent = 0;

                // add children to root stack array.
                ARRAY_PUSH(list->root_index, list->length, &list->root_index_count, current_sibling_index);
                child->root_dense_index = list->root_index_count;

                // children are now roots, so they should no longer be associated with thier siblings.
                i32 next_sibling_index = child->next_sibling;
                child->next_sibling = current_sibling_index;
                child->previous_sibling = current_sibling_index;

                if(next_sibling_index == first_child_index){
                    break;
                }
                // go to the next sibling of the child.
                current_sibling_index = next_sibling_index;
                BOUNDS_CHECK(current_sibling_index, list->length);
                child = &list->node[current_sibling_index];
            }

            // no need to deallocate from siblings, as this has already done that.
            goto End;
        }
    }

    // deallocate from siblings.
    if(node->next_sibling > 0 && node->previous_sibling > 0){
        i32 next_sibling_index = node->next_sibling;
        BOUNDS_CHECK(next_sibling_index, list->length);
        IntrusiveListNode* next_sibling = &list->node[next_sibling_index];
        next_sibling->previous_sibling = node->previous_sibling;

        i32 previous_sibling_index = node->previous_sibling;
        BOUNDS_CHECK(previous_sibling_index, list->length);
        IntrusiveListNode* previous_sibling = &list->node[previous_sibling_index];
        previous_sibling->next_sibling = node->next_sibling;
    }

    End:
    ZERO_STRUCT(node);
    return true;
}

/**
    this is an internal function for `intrusive_list_remove_node_and_children()`
    and shouldnt be used.
**/
void intrusive_list_remove_node_and_children_update_node_recursive(
    IntrusiveListNode* nodes, i32 nodes_length, i32 parent_index, i32 node_index, i32 parent_first_child_index
){
    BOUNDS_CHECK(node_index, nodes_length);
    IntrusiveListNode* node = &nodes[node_index];
    i32 first_child_index = node->first_child;
    i32 next_index = node->next_sibling;
    ZERO_STRUCT(node);

    if(first_child_index != 0){
        intrusive_list_remove_node_and_children_update_node_recursive(nodes, nodes_length, node_index, first_child_index, first_child_index);
    }
    if(next_index == parent_first_child_index){
        return;
    }
    else{
        intrusive_list_remove_node_and_children_update_node_recursive(nodes, nodes_length, parent_index, next_index, parent_first_child_index);
    }
}
/**
    `returns`
    true, if successfully removed from the tree; otherwise false if already removed.
**/
bool intrusive_list_remove_node_and_children(IntrusiveList* list, i32 node_index){
    // node cannot be nil.
    if(node_index == 0){
        ASSERT(false, "nil element");
        return false;
    }
    
    BOUNDS_CHECK(node_index, list->length);
    IntrusiveListNode* node = &list->node[node_index];
    if(node->in_tree == false){
        return false;
    }

    i32 parent_index = node->parent;
    i32 first_child_index = node->first_child;

    // dealloc all children.
    if(node->first_child != 0){
        BOUNDS_CHECK(node->first_child, list->length);
        IntrusiveListNode* child = &list->node[node->first_child];
        intrusive_list_remove_node_and_children_update_node_recursive(list->node, list->length, node_index, node->first_child, node->first_child);
    } 

    // dealloc from parent
    if(parent_index!=0){
        BOUNDS_CHECK(parent_index, list->length);
        IntrusiveListNode* parent = &list->node[parent_index];
        if(parent->first_child == node_index){
            parent->first_child = 0;
        }
    }
    else{
        switch(list->preserve_root_order){
            case true:{
                // move all root node dense indices backward; reflecting the ordered removal.
                for(i32 i = node->root_dense_index+1; i < list->root_index_count; i++){
                    BOUNDS_CHECK(i, list->length);
                    IntrusiveListNode* next_root = &list->node[list->root_index[i]];
                    next_root->root_dense_index -= 1;
                }
                // remove the root index.
                ARRAY_ORDERED_REMOVE_AT(list->root_index, list->length, &list->root_index_count, node->root_dense_index);
            } break;
            case false:{
                /**
                    remove the node from the roots array.
                    performing the dense index swap as well.
                **/
                IntrusiveListNode* last_root = &list->node[list->root_index[list->root_index_count-1]];
                last_root->root_dense_index = node->root_dense_index;
                ARRAY_UNORDERED_REMOVE_AT(list->root_index, list->length, &list->root_index_count, node->root_dense_index);
                node->root_dense_index = 0;
            } break;
        }
    }

    // deallocate from siblings.
    i32 next_sibling_index = node->next_sibling;
    i32 previous_sibling_index = node->previous_sibling;
    
    if(next_sibling_index > 0 && previous_sibling_index > 0){
        
        BOUNDS_CHECK(next_sibling_index, list->length);
        IntrusiveListNode* next_sibling = &list->node[next_sibling_index];
        next_sibling->previous_sibling = previous_sibling_index;

        BOUNDS_CHECK(previous_sibling_index, list->length);
        IntrusiveListNode* previous_sibling = &list->node[previous_sibling_index];
        previous_sibling->next_sibling = next_sibling_index;
    }

    // deallocate.
    ZERO_STRUCT(node);
    return true;
}

/**
    Sends a root node to the front of the <c><see cref="IntrusiveList.RootIndices"/></c> buffer.

    `remarks`
    The 'front' in this context is index `1` NOT `0` as the collection stores a `Nil` element.

**/
void intrusive_list_send_root_front(IntrusiveList* list, i32 root_index){
    ASSERT(root_index < 0, "invalid element");
    ASSERT(root_index == 0, "nil element");
    /**    
        shift all the node root indices - before the root index to send to the front - 
        forwards to reflect their new element positions after the ordered insertion. 
    **/
    for(i32 i = root_index; i > 0; i--){
        BOUNDS_CHECK(root_index, list->length);
        list->node[list->root_index[i]].root_dense_index += 1;
    }

    // set the root node's root index to 1 (which is the front).
    BOUNDS_CHECK(root_index, list->length);
    i32 node_index = list->root_index[root_index];
    BOUNDS_CHECK(node_index, list->length);
    list->node[node_index].root_dense_index = 1;

    // send the root node to the front of the root list.
    ARRAY_ORDERED_REMOVE_AT(list->root_index, list->length, &list->root_index_count, root_index);
    ARRAY_ORDERED_INSERT(list->root_index, list->length, &list->root_index_count, 1, node_index);
}

bool intrusive_list_is_node_in_tree(IntrusiveList* list, i32 node_index){
    BOUNDS_CHECK(node_index, list->length); 
    return list->node[node_index].in_tree;
}

bool intrusive_list_is_node_root(IntrusiveList* list, i32 node_index){
    BOUNDS_CHECK(node_index, list->length);
    return list->node[node_index].parent == 0;
}

/**
    Gets the root node of a node within an intrusive list.
**/
IntrusiveListNode* intrusive_list_get_node_root(IntrusiveList* list, i32 node_index){
    BOUNDS_CHECK(node_index, list->length);
    IntrusiveListNode* node = &list->node[node_index];
    while(node->parent != 0){
        node = &list->node[node->parent];
    }
    return node;
}