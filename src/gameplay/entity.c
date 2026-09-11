typedef struct{
    Transform2D transform;
    GFX_SpriteId sprite_id;
    f32 sprite_depth;
    GenId physics_body_gid;
    Aabb clickable_aabb;
    i32 health;
    bool is_health;
    bool is_clickable;
    bool is_physics_body;
} Entity;

typedef struct{
    Entity* entity;
    i32 entity_length;
    GenIdAllocator gen_id_allocator;
    FIZX_State fizx_state;
    bool is_init;
} EntityManager;

#define PHYSICS_LAYER_ALL I32_MAX
#define PHYSICS_LAYER_PLAYER (1 << 1) 
#define PHYSICS_LAYER_ENEMY (1 << 2) 
#define PHYSICS_LAYER_ENVIRONMENT (1 << 3) 

void entity_manager_init(EntityManager* manager, MemoryArena* arena, i32 entity_amount, i32 physics_body_amount){
    ASSERT(!manager->is_init, "already init.");
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->entity, &manager->entity_length, entity_amount);
    gen_id_allocator_init(&manager->gen_id_allocator, arena, physics_body_amount);
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4, sizeof(GenId));
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

void entity_manager_debug_draw(EntityManager manager, GFX_State* gfx, f32 delta_time){
    for(i32 i = 0; i < manager.entity_length; i++){
        Entity* entity = &manager.entity[i];
        if(entity->is_clickable){
            Rectangle shape = {
                .x = entity->transform.position.x  + entity->clickable_aabb.min_x,
                .y = entity->transform.position.y  + entity->clickable_aabb.max_y,
                .width = entity->clickable_aabb.max_x - entity->clickable_aabb.min_x,
                .height = entity->clickable_aabb.max_y - entity->clickable_aabb.min_y
            };            
            gfx_draw_wire_rect(gfx, shape , GFX_COLOUR_WHITE, 0.0f, SPRITE_LAYER_WORLD, SPRITE_MATERIAL_DEBUG);
        }
    }
}

void entity_deplete_health(EntityManager* manager, GenId entity_gid, i32 amount){
    Entity* entity;
    if(!entity_manager_get_entity(*manager, entity_gid, &entity)){
        ASSERT(false, "failed to get entity.");
    }
    
    ASSERT(entity->is_health, "entity doesnt use health.");
    entity->health -= amount;
    if(entity->health <= 0){
        entity_manager_dealloc_entity(manager, entity_gid);
    }
}

void entity_spawn_bird(EntityManager* entity_manager, GFX_State* gfx_ctx, Vector2 position){
    // clickable entity (angry bird).
    GenId player_gid = entity_manager_alloc_entity(entity_manager);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, player_gid, &entity);    
    {
        entity->transform = TRANSFORM2D_IDENTITY;
        entity->transform.position = position;
        entity->transform.scale = vector2_mul_val(entity->transform.scale, 1.0f); 
        entity->is_physics_body = true;
        
        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 5.0f, .restitution = 0.0f};
        
        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, true);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &player_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);
    
        entity->is_clickable = true;
        entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};
   
        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, transform2d_to_matrix4x4(sprite_transform), GFX_COLOUR_WHITE, (GFX_SpriteRegion){.width = 512, .height = 512}, GFX_ColourState_Tint,
            3, SPRITE_MATERIAL_IMAGE, true
        );
    }
}