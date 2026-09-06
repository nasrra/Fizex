typedef struct{
    Transform transform;
    SpriteId sprite_id;
    GenId physics_body_gid;
} Entity;

typedef struct{
    Entity* entity;
    i32 entity_length;
    GenIdAllocator gen_id_allocator;
    FIZXState fizx_state;
    bool is_init;
} EntityManager;

void entity_manager_init(EntityManager* manager, MemoryArena* arena, i32 entity_amount, i32 physics_body_amount){
    ASSERT(!manager->is_init, "already init.");
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->entity, &manager->entity_length, entity_amount);
    gen_id_allocator_init(&manager->gen_id_allocator, arena, physics_body_amount);
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4);
    manager->is_init = true;
}

GenId entity_manager_alloc_entity(EntityManager* manager){
    return gen_id_allocator_alloc(&manager->gen_id_allocator);
}

bool entity_manager_dealloc_entity(EntityManager* manager, GenId gid){
    if(!gen_id_allocator_dealloc(&manager->gen_id_allocator, gid)){
        return false;
    }
    
    i32 idx = gen_id_get_index(gid);
    BOUNDS_CHECK(idx, manager->entity_length);
    Entity* entity = &manager->entity[idx];
    
    if(entity->physics_body_gid != 0){
        fizx_body_dealloc(&manager->fizx_state, entity->physics_body_gid);
    }
    
    *entity = (Entity){0};
    
    return true;
}

bool entity_manager_get_entity(EntityManager manager, GenId entity_gid, Entity** out_entity){
    if(gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, entity_gid)){
        return false;
    }
    i32 idx = gen_id_get_index(entity_gid);
    BOUNDS_CHECK(idx, manager.entity_length);
    *out_entity = &manager.entity[idx];
    return true;
}

void entity_manager_update(EntityManager* manager, RendererContext* renderer_ctx, f32 delta_time){
    for(i32 i = 0; i < manager->entity_length; i++){
        Entity* entity = &manager->entity[i];       
        
        if(entity->physics_body_gid != 0){
            Transform2D transform2d;
            if(fizx_body_get_transform(&manager->fizx_state, entity->physics_body_gid, &transform2d)){
                entity->transform = transform2d_to_transform(transform2d);
            }
        }
        
        if(!renderer_sprite_id_equals(entity->sprite_id, (SpriteId){0})){
            renderer_sprite_set_transform(renderer_ctx, entity->sprite_id, transform_to_matrix4x4(entity->transform));
        }
    }
}
