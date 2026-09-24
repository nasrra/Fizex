


///
/// types.
///




typedef enum{
    EntityTypeId_None = 0,
    EntityTypeId_LevelRoot = 1,
    EntityTypeId_RedBird = 2,
    EntityTypeId_YellowBird = 3,
    EntityTypeId_WoodBlock = 4,
    EntityTypeId_InvisibleWall = 5,
    EntityTypeId_Pig = 6
} EntityTypeId;

typedef struct{
    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
} GameMouseState;

typedef struct{
    // the entity_id of the entity to spawn. 
    EntityTypeId entity_to_spawn;    
    GenId entity_parent_gid;
} EditorMouseState;

typedef struct{
    bool is_editor_mode;
    EditorMouseState editor_mouse_state;
    GameMouseState mouse_state;
    i32 alive_enemies;
} GameState;

typedef struct{
    String name;
    Transform2D transform;
    GFX_SpriteId sprite_id;
    f32 sprite_depth;
    GenId physics_body_gid;
    Aabb clickable_aabb;
    EntityTypeId type_id;
    i32 health;
    bool is_health;
    bool is_clickable;
    bool is_physics_body;
    bool is_invincible;
} Entity;

typedef void (*TimerTimeout)(void* user_data);

typedef struct{
    GenIdAllocator gen_id_allocator;
    f32* time_scale;
    f32* previous_time;
    f32* current_time;
    // `remarks`
    // 1.0f = true, 0.0f = false.
    f32* is_ticking;
    // `remarks`
    // 1.0f = true, 0.0f = false.
    f32* has_started;
    f32* delta_time_scratch_space;
    char* timeout_data;
    TimerTimeout* timeout_function;
    i32 timeout_data_length;
    i32 timeout_data_element_size;
    // the length of all backing arrays.
    i32 length;
    bool is_init;
} TimerManager;

typedef GenId TimerHandle;

typedef struct{
    // the transform to spawn the entity at.
    Transform2D spawn_transform;
    /*
        the gid of the spawned entity.
        
        `remarks`
        this field should be populated with valid data after the entity has been allocated in the entity manager.
    */
    GenId entity_gid;
    // the id of entity to spawn's type. 
    i32 entity_type_id;
    // the index of the entity's parent in the deserialised entity list. 
    i32 parent_deserialised_entity_idx;
} DeserialisedEntity;

typedef struct{
    Entity* entity;
    i32 entity_length;
    DeserialisedEntity* deserialised_entity;
    i32 deserialised_entity_length;
    i32 deserialised_entity_count;
    GenIdAllocator gen_id_allocator;
    FIZX_State fizx_state;
    GFX_State* gfx_state;
    TimerManager timer_manager;
    IntrusiveList entity_hierarchy;
    bool is_init;
} EntityManager;

typedef struct{
    EntityManager* entity_manager;
} CollisionCallbackContext;




///
/// Globals.
///




GameState game_state;




///
/// Definitions.
///




#define ENTITY_NAME_LENGTH 32
#define PLAYER_MOUSE_MAX_DRAW_RADIUS 2.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.5f
#define LEVEL_FILE_LINE_LENGTH 512
// timer manager's `is_ticking` value that indicates a boolean `true`.
#define TIMER_MANAGER_IS_TICKING_TRUE 1.0f
// timer manager's `is_ticking` value that indicates a boolean `false`.
#define TIMER_MANAGER_IS_TICKING_FALSE 0.0f
// timer manager's `has_started` value that indicates a boolean `true`.
#define TIMER_MANAGER_HAS_STARTED_TRUE 1.0f
// timer manager's `has_started` value that indicates a boolean `false`.
#define TIMER_MANAGER_HAS_STARTED_FALSE 0.0f




///
/// Physics layers.
///

#define PHYSICS_LAYER_ALL I32_MAX
#define PHYSICS_LAYER_PLAYER (1 << 1)
#define PHYSICS_LAYER_ENEMY (1 << 2)
#define PHYSICS_LAYER_ENVIRONMENT (1 << 3)

///
/// Virtual Texture ID.
///
#define VIRTUAL_TEXTURE_ID_TEST_SHEET 3
#define VIRTUAL_TEXTURE_ID_SLING_SHOT 4
#define VIRTUAL_TEXTURE_ID_WOOD_BLOCK 6

/// 
/// Texture Views.
///
#define GFX_SPRITE_REGION_PIG_HEALTHY (GFX_SpriteRegion){.top_left = {692, 855}, .bot_right = {740, 901}}
#define GFX_SPRITE_REGION_PIG_HURT (GFX_SpriteRegion){.top_left = {692, 902}, .bot_right = {740, 948}}
#define GFX_SPRITE_REGION_PIG_CRITICAL (GFX_SpriteRegion){.top_left = {752, 846}, .bot_right = {800, 892}}




///
/// Timer Manager.
///




bool timer_manager_init(TimerManager* manager, MemoryArena* arena, i32 timer_amount, i32 timeout_data_element_size){
    if(manager->is_init){
        ASSERT(false, "already init.");
        return false;
    }
    
    gen_id_allocator_init(&manager->gen_id_allocator, arena, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->time_scale, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->previous_time, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->current_time, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->is_ticking, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->has_started, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->delta_time_scratch_space, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->timeout_function, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->timeout_data, &manager->timeout_data_length, timeout_data_element_size * timer_amount);
    manager->timeout_data_element_size = timeout_data_element_size;
    manager->is_init = true;
    
    return true;
}

/*
    `returns`
    a handle to the newly started timer; otherwise 0 if allocation was unsuccessful.
*/
TimerHandle timer_manager_timer_start(TimerManager* manager, f32 start_time, f32 time_scale, TimerTimeout timer_timeout_function, void* timeout_data, size_t timeout_data_size){
    TimerHandle handle = gen_id_allocator_alloc(&manager->gen_id_allocator);
    if(handle == 0){
        return handle;
    }
    
    if(timeout_data_size > manager->timeout_data_element_size){
        ASSERT(false, "attempted to alloc timer data that has a size greater than the allocated maximum size.");
        return handle;
    }
    
    i32 idx = gen_id_get_index(handle);
    
    // set timer callback data.
    size_t timeout_data_idx = idx * manager->timeout_data_element_size;
    BOUNDS_CHECK(timeout_data_idx, manager->timeout_data_length);
    COPY_MEMORY(manager->timeout_data + timeout_data_idx, timeout_data, timeout_data_size);
    
    // set the timer state.
    manager->current_time[idx] = start_time;
    manager->previous_time[idx] = 0.0f;
    manager->is_ticking[idx] = TIMER_MANAGER_IS_TICKING_TRUE;
    manager->has_started[idx] = TIMER_MANAGER_HAS_STARTED_TRUE;
    manager->timeout_function[idx] = timer_timeout_function;
    
    return handle;
}

/*
    `returns`
    true, if the timer was successfully stopped; otherwise false, if it was already stopped.
*/
bool timer_manager_timer_stop_unsafe(TimerManager* manager, i32 timer_idx){
    BOUNDS_CHECK(timer_idx, manager->length);
    if(manager->has_started[timer_idx] == TIMER_MANAGER_HAS_STARTED_FALSE){
        return false;
    }
    manager->has_started[timer_idx]         = TIMER_MANAGER_HAS_STARTED_FALSE;
    manager->is_ticking[timer_idx]          = TIMER_MANAGER_IS_TICKING_FALSE;
    manager->current_time[timer_idx]        = 0.0f;
    manager->previous_time[timer_idx]       = 0.0f;
    manager->timeout_function[timer_idx]    = NULL; 
    gen_id_allocator_dealloc_unsafe(&manager->gen_id_allocator, timer_idx);
    return true;
}

/*
    `returns`
    true, if the timer was successfully stopped; otherwise false, if the `handle` is invalid.
*/
bool timer_manager_timer_stop(TimerManager* manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager->gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    return timer_manager_timer_stop_unsafe(manager, idx);
}

/*
    `returns`
    true, if the timer was successfully paused; otherwise false, if the `handle` is invalid.
*/
bool timer_manager_timer_pause(TimerManager* manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager->gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager->length);
    if(manager->has_started[idx] == TIMER_MANAGER_HAS_STARTED_FALSE){
        return false;
    }
    manager->is_ticking[idx] = TIMER_MANAGER_IS_TICKING_FALSE;
    return true;
}

/*
    `returns`
    true, if the timer was successfully resumed; otherwise false, if the `handle` is invalid.
*/
bool timer_manager_timer_resume(TimerManager* manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager->gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager->length);
    if(manager->has_started[idx] == TIMER_MANAGER_HAS_STARTED_FALSE){
        return false;
    }    
    manager->is_ticking[idx] = TIMER_MANAGER_IS_TICKING_TRUE;
    return true;
}

void timer_manager_update(TimerManager* manager, f32 delta_time){
    COPY_MEMORY(manager->previous_time, manager->current_time, manager->length * sizeof(f32));
    simd_f32_store_val(manager->delta_time_scratch_space, manager->length, delta_time);
    simd_f32_mul(manager->delta_time_scratch_space, manager->delta_time_scratch_space, manager->time_scale, manager->length, 0);
    simd_f32_mul(manager->delta_time_scratch_space, manager->delta_time_scratch_space, manager->is_ticking, manager->length, 0);
    simd_f32_mul(manager->delta_time_scratch_space, manager->delta_time_scratch_space, manager->has_started, manager->length, 0);
    simd_f32_sub_val_clamped_sse(manager->current_time, delta_time, manager->current_time, 0.0f, manager->length);
    for(i32 i = 0; i < manager->length; i++){
        if(manager->has_started[i] == TIMER_MANAGER_HAS_STARTED_FALSE){
            continue;
        }
        if(manager->is_ticking[i] == TIMER_MANAGER_IS_TICKING_FALSE){
            continue;
        }
        if(manager->current_time[i] != 0.0f){
            continue;
        }
        if(manager->previous_time[i] <= manager->current_time[i]){
            continue;
        }
        if(manager->timeout_function[i] != NULL){
            i32 timeout_data_idx = i * manager->timeout_data_element_size;
            BOUNDS_CHECK(timeout_data_idx, manager->timeout_data_length);
            manager->timeout_function[i](&manager->timeout_data[timeout_data_idx]);
        }
        timer_manager_timer_stop_unsafe(manager, i);
    }
}

bool timer_manager_timer_has_started(TimerManager manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager.length);
    return manager.has_started[idx] == TIMER_MANAGER_HAS_STARTED_TRUE;
}

/*
    `returns`
    true if the timer is ticking; otherwise false if it isnt or the handle is invalid.
*/
bool timer_manager_timer_is_ticking(TimerManager manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager.length);
    if(manager.has_started[idx] == TIMER_MANAGER_HAS_STARTED_FALSE){
        return false;
    }  
    return manager.is_ticking[idx] == TIMER_MANAGER_IS_TICKING_TRUE;
}

/*
    `returns`
    true, if the timer's time scale was successfully set; otherwise false.
*/
bool timer_manager_timer_set_time_scale(TimerManager* manager, TimerHandle handle, f32 time_scale){
    if(gen_id_allocator_is_gen_id_invalid(&manager->gen_id_allocator, handle)){
        return false;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager->length);
    manager->time_scale[idx] = time_scale;
    return true;
}

/*
    `returns`
    the delta time between updates that the timer ticked by; otherwise 0 if the timer didn't tick or the handle is invalid.
*/
f32 timer_manager_timer_get_delta_tick_time(TimerManager manager, TimerHandle handle){
    if(gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, handle)){
        return 0.0f;
    }
    i32 idx = gen_id_get_index(handle);
    BOUNDS_CHECK(idx, manager.length);
    return ABS(manager.previous_time[idx] - manager.current_time[idx]);
}

inline bool timer_manager_is_timer_handle_invalid(TimerManager manager, TimerHandle handle){
    return gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, handle);
}





GenId entity_manager_alloc_entity(EntityManager* manager, GenId parent){
    GenId gid = gen_id_allocator_alloc(&manager->gen_id_allocator);
    if(gid == 0){
        return gid;
    }
    
    i32 idx = gen_id_get_index(gid);
    
    if(parent == 0){
        if(!intrusive_list_add_root(&manager->entity_hierarchy, idx)){
            ASSERT(false, "failed to insert entity into entity hierarchy.");
            gen_id_allocator_dealloc(&manager->gen_id_allocator, gid);
            gid = 0;
        }
    }
    else{
        i32 parent_idx = gen_id_get_index(parent);
        if(!intrusive_list_add_branch(&manager->entity_hierarchy, idx, parent_idx)){
            ASSERT(false, "failed to insert entity into entity hierarchy.");
            gen_id_allocator_dealloc(&manager->gen_id_allocator, gid);
            gid = 0;
        }
    }
    
    return gid;
}

void entity_manager_dealloc_entity_data_unsafe(EntityManager* manager, i32 idx){
    BOUNDS_CHECK(idx, manager->entity_length);
    Entity* entity = &manager->entity[idx];

    if(entity->physics_body_gid != 0){
        fizx_body_dealloc(&manager->fizx_state, entity->physics_body_gid);
    }
    
    if(!gfx_sprite_id_equals(entity->sprite_id, (GFX_SpriteId){0})){
        gfx_dealloc_sprite(manager->gfx_state, entity->sprite_id);
    }

    *entity = (Entity){0};
}

void entity_on_entity_hierarchy_dealloc(IntrusiveList* list, i32 node_idx, void* user_data){
    EntityManager* manager = (EntityManager*)user_data;
    entity_manager_dealloc_entity_data_unsafe(manager, node_idx);
}

bool entity_manager_dealloc_entity(EntityManager* manager, GenId gid){
    if(!gen_id_allocator_dealloc(&manager->gen_id_allocator, gid)){
        return false;
    }
    i32 idx = gen_id_get_index(gid);

    // TODO:
    // may want to add functionality later so the user can deallocate an entity independently from its children.
    
    // NOTE:
    // the on_dealloc callback for this calls entity_manager_dealloc_entity_data_unsafe.
    if(!intrusive_list_remove_node_and_children(&manager->entity_hierarchy, idx, manager)){
        ASSERT(false, "failed to remove entity from entity hierarchy.");
    }
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
#if 0
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
#endif
}

GenId entity_spawn_red_bird(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);
         
        entity->type_id = EntityTypeId_RedBird; 
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 2.0f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);

        entity->is_clickable = true;
        entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};

        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.top_left = {863, 797}, .bot_right = {863 + 45, 797 + 45}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_TEST_SHEET, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}

GenId entity_spawn_yellow_bird(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        entity->type_id = EntityTypeId_YellowBird; 
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 1.0f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);

        entity->is_clickable = true;
        entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};

        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.top_left = {629, 879}, .bot_right = {629 + 58, 879 + 53}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_TEST_SHEET, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}

GenId entity_spawn_wood_block(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);

        entity->type_id = EntityTypeId_WoodBlock;
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 22.6f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_ENVIRONMENT, square, material, true);
        
        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.bot_right = {.x = 150, .y = 150}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_WOOD_BLOCK, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}


GenId entity_spawn_invisible_wall(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {        
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        entity->type_id = EntityTypeId_InvisibleWall;
        
        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 1.0f, .kinetic_friction = 1.0f, .density = 22.6f, .restitution = 0.0f};
        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->is_physics_body = true;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(
            &entity_manager->fizx_state, 
            entity->physics_body_gid, 
            shape_transform, 
            FIZX_ShapeBehaviour_Kinematic, 
            &entity_gid, 
            PHYSICS_LAYER_ENVIRONMENT, 
            square, 
            material, 
            true
        );
    }
    return entity_gid;
}

GenId entity_spawn_level_root(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        entity->type_id = EntityTypeId_LevelRoot;
        string_push(&entity->name, name); 
    }
    return entity_gid;
}

typedef struct{
    EntityManager* entity_manager;
    GenId entity_gid;
} PigInvincibleTimerTimeoutContext;

void pig_invincible_timer_timeout(void* user_data){
    PigInvincibleTimerTimeoutContext* ctx = (PigInvincibleTimerTimeoutContext*)user_data;
    Entity* entity;
    if(!entity_manager_get_entity(*ctx->entity_manager, ctx->entity_gid, &entity)){
        return;
    }
    entity->is_invincible = false;
}

void pig_fizx_shape_on_enter_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    GenId* entity_gid = (GenId*)info.target_user_data;

    if((info.source_layer & PHYSICS_LAYER_PLAYER) == 0){
        return;
    }
    
    Entity* entity;
    if(!entity_manager_get_entity(*ctx->entity_manager, *entity_gid, &entity)){
        ASSERT(false, "failed to get entity.");
    }
    if(entity->is_invincible){
        return;
    }

    ASSERT(entity->is_health, "entity doesnt use health.");
    entity->health -= 1;
    
    if(entity->health <= 0){
        entity_manager_dealloc_entity(ctx->entity_manager, *entity_gid);
        game_state.alive_enemies-=1;
        if(game_state.alive_enemies <= 0){
            platform_output_message("WIN!");
        }
    }
    else if(entity->health <= 1){
        gfx_sprite_set_region(ctx->entity_manager->gfx_state, entity->sprite_id, GFX_SPRITE_REGION_PIG_CRITICAL);
    }
    else if(entity->health <= 2){
        gfx_sprite_set_region(ctx->entity_manager->gfx_state, entity->sprite_id, GFX_SPRITE_REGION_PIG_HURT);        
    }
    
    PigInvincibleTimerTimeoutContext timeout_data = {
        .entity_manager = ctx->entity_manager,
        .entity_gid = *entity_gid
    };
    
    entity->is_invincible = true;
    timer_manager_timer_start(&ctx->entity_manager->timer_manager, 0.675f, 1.0f, pig_invincible_timer_timeout, &timeout_data, sizeof(timeout_data));
}

GenId entity_spawn_pig(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        entity->type_id = EntityTypeId_Pig; 
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 1.0f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_ENEMY, square, material, true);
        fizx_shape_set_on_enter_callback(&entity_manager->fizx_state, pig_fizx_shape_on_enter_callback, entity_shape_gid);

        entity->is_health = true;
        entity->health = 3;

        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = GFX_SPRITE_REGION_PIG_HEALTHY;
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_TEST_SHEET, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    game_state.alive_enemies+=1;
    return entity_gid;
}

void load_lvl(EntityManager* entity_manager, GFX_State* gfx, String file_path){
    /*
        .scsv are .csv files that are separated with ';' instead of ','

        a level file is as follows:

        entity_id ; entity_position_x ; entity_position_y ; entity_scale_x ; entity_scale_y ; entity_rotation_radians ;
    */

    size_t buffer_size;
    void* raw_file = platform_load_file(file_path, &buffer_size);
    char* file_data = (char*)raw_file;

    DeserialisedEntity* deserialised_entity;
    i32 lines_read = 1;
    i32 bytes_consumed = 0;

    entity_manager->deserialised_entity_count = 1; // skip the NIL.

    while(true){
        entity_manager->deserialised_entity_count += 1;
        if(entity_manager->deserialised_entity_count > entity_manager->deserialised_entity_length){
            ASSERT(false, "cannot store all required deserialised entities from level file.");
            break;
        }

        BOUNDS_CHECK(entity_manager->deserialised_entity_count, entity_manager->deserialised_entity_length);
        deserialised_entity = &entity_manager->deserialised_entity[entity_manager->deserialised_entity_count-1];
    
        // NOTE:
        // this can corrupt the stack, as sscanf doesnt bounds check at all for name strings.
        // be very careful here, if the program nukes itself when loadiing saved data, this is probs why.
        // have fun champ :)
        String name = {.chars =  (char[ENTITY_NAME_LENGTH]){0}, .length = ENTITY_NAME_LENGTH};
        deserialised_entity->spawn_transform = TRANSFORM2D_IDENTITY;
        lines_read = sscanf(
            file_data,
            "%[^;]%n;%i;%i;%f;%f;%f;%f;%f;%n",
            name.chars,
            &name.count,
            &deserialised_entity->entity_type_id,
            &deserialised_entity->parent_deserialised_entity_idx,
            &deserialised_entity->spawn_transform.position.x,
            &deserialised_entity->spawn_transform.position.y,
            &deserialised_entity->spawn_transform.scale.x,
            &deserialised_entity->spawn_transform.scale.y,
            &deserialised_entity->spawn_transform.rotation,
            &bytes_consumed
        );

        // NOTE:
        // deserialsied entities should be in the order (from 0 to length) parent->child.
        // such that entities at the end can be parented to ones near the start.
        //
        // EXAMPLE:
        // [0] is the NIL.
        // [1] parent_deserialised_entity_idx = 0;
        // [2] parent_deserialised_entity_idx = 1; // references the line above.
        // [3] parent_deserialised_entity_idx = 4; // this is not okay as it is a later in the list and hasnt been loaded yet, also its out of bounds.
        i32 parent_idx = deserialised_entity->parent_deserialised_entity_idx;
        BOUNDS_CHECK(parent_idx, entity_manager->deserialised_entity_count);
        GenId parent_gid = parent_idx <= 0
        ? 0
        : entity_manager->deserialised_entity[parent_idx].entity_gid;

        if(lines_read == 8){
            switch(deserialised_entity->entity_type_id){
                case EntityTypeId_RedBird:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_red_bird(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                case EntityTypeId_YellowBird:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_yellow_bird(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                case EntityTypeId_WoodBlock:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_wood_block(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                case EntityTypeId_InvisibleWall:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_invisible_wall(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                case EntityTypeId_Pig:{
                    deserialised_entity->entity_gid
                        = entity_spawn_pig(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                case EntityTypeId_LevelRoot:{
                    deserialised_entity->entity_gid
                        = entity_spawn_level_root(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_gid);
                }break;
                default:{
                    ASSERT(false, "unknown entity type id.");
                }break;
            }
            file_data += bytes_consumed;
        }
        else{
            break;
        }        
    }
    platform_free_memory(raw_file);
}

void entity_manager_init(
    EntityManager* manager, MemoryArena* arena, GFX_State* gfx_state, 
    i32 entity_amount, i32 physics_body_amount, i32 timer_timeout_data_size
){
    ASSERT(!manager->is_init, "already init.");
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->entity, &manager->entity_length, entity_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->deserialised_entity, &manager->deserialised_entity_length, entity_amount);
    manager->deserialised_entity_count = 0;
    
    for(i32 i = 0; i < entity_amount; i++){
        Entity* entity = &manager->entity[i];
        string_init(&entity->name, arena, ENTITY_NAME_LENGTH);
    }
    
    gen_id_allocator_init(&manager->gen_id_allocator, arena, physics_body_amount);
    intrusive_list_init(&manager->entity_hierarchy, arena, entity_amount, false);
    manager->entity_hierarchy.on_dealloc_callback = entity_on_entity_hierarchy_dealloc;
    manager->gfx_state = gfx_state;
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4, sizeof(GenId));
    timer_manager_init(&manager->timer_manager, arena, entity_amount, timer_timeout_data_size);
    manager->is_init = true;
}

void player_update(EntityManager* entity_manager, GFX_State* gfx_state, Vector2 mouse_world_position, f32 delta_time){
    
    if(!game_state.is_editor_mode){
        if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
            for(i32 e_idx = 0; e_idx < entity_manager->entity_length; e_idx++){
                Entity* entity = &entity_manager->entity[e_idx];
                Aabb world_aabb = aabb_translate(entity->clickable_aabb, entity->transform.position);
                if(aabb_overlaps_point(world_aabb, mouse_world_position)){
                    platform_output_message("clicked entity\n");
                    if(entity->is_physics_body){
                        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);
                    }
                    game_state.mouse_state.clicked_entity_initial_position = entity->transform.position;
                    game_state.mouse_state.clicked_entity_idx = e_idx;
                }
            }
        }
        
        Vector2 impulse_magnitude;
        
        if(game_state.mouse_state.clicked_entity_idx){
            Entity* entity = &entity_manager->entity[game_state.mouse_state.clicked_entity_idx];
            Vector2 position_diff = vector2_sub(game_state.mouse_state.clicked_entity_initial_position, mouse_world_position);
            
            position_diff = vector2_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
            impulse_magnitude = vector2_mul_val(position_diff, PLAYER_MOUSE_LAUNCH_FORCE); 
            
            Vector2 new_position = vector2_sub(game_state.mouse_state.clicked_entity_initial_position, position_diff);
                
            entity->transform.position = new_position;
            fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, new_position);
        }
        
        if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
            if(game_state.mouse_state.clicked_entity_idx > 0){
                Entity* entity = &entity_manager->entity[game_state.mouse_state.clicked_entity_idx];
                fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
                game_state.mouse_state.clicked_entity_idx = 0;
                fizx_body_clear_forces_and_velocities(&entity_manager->fizx_state, entity->physics_body_gid);
    
                fizx_body_impulse_force(&entity_manager->fizx_state,impulse_magnitude, entity->physics_body_gid);
            }
        }
    }
    else{
        if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
            Transform2D spawn_transform = TRANSFORM2D_IDENTITY;
            spawn_transform.position = mouse_world_position;
            
            if(game_state.editor_mouse_state.entity_parent_gid == 0){
                return;
            }
            
            switch(game_state.editor_mouse_state.entity_to_spawn){
                case EntityTypeId_RedBird:{
                    entity_spawn_red_bird(entity_manager, gfx_state, (String){.chars = "spawned red bird", .length = 16, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_YellowBird:{
                    entity_spawn_yellow_bird(entity_manager, gfx_state, (String){.chars = "spawned yellow bird", .length = 19, .count = 19}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_WoodBlock:{
                    entity_spawn_wood_block(entity_manager, gfx_state, (String){.chars = "spawned wood block", .length = 18, .count = 18}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_LevelRoot:{
                    entity_spawn_level_root(entity_manager, gfx_state, (String){.chars = "spawned level", .length = 16, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_Pig:{
                    entity_spawn_pig(entity_manager, gfx_state, (String){.chars = "spawned pig", .length = 11, .count = 11}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
            }
        }
    }
}

void editor_select_red_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_RedBird;
    }
}

void editor_select_yellow_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_YellowBird;
    }
}

void editor_select_wood_block_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_WoodBlock;
    }
}

void editor_select_pig_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_Pig;
    }
}

void editor_on_hover_select_level_button(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_parent_gid = *((GenId*)user_data);
    }
}

void editor_save_level_entity_recurssive(
    EntityManager* entity_manager, 
    String file_path,
    String* entity_char_buffer, 
    char* char_scratch_space, i32 char_scratch_space_length, 
    i32 entity_idx, i32 parent_line_idx, i32 line_idx
){

    // loop through branch level.
    i32 first_idx = entity_idx;
    while(true){    
        BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
        Entity* entity = &entity_manager->entity[entity_idx];
        
        i32 written = 0;
        string_clear(entity_char_buffer);
        
        { // prepare entity data for serialisation.
            
            // entity name.
            string_push(entity_char_buffer, entity->name);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // entity type id.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%i", entity->type_id);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // parent idx.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%i", parent_line_idx);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // transform position x.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.position.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform position y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.position.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform scale x.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.scale.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform scale y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.scale.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // transform rotation.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.rotation);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // new line.
            string_push_chars(entity_char_buffer, "\n", 1);
        }
    
        { // write entity data to file.
            platform_write_file(
                file_path, 
                entity_char_buffer->chars, 
                entity_char_buffer->count, 
                FileWriteType_Append
            );
        }
    
        BOUNDS_CHECK(entity_idx, entity_manager->entity_hierarchy.length);
        IntrusiveListNode* node = &entity_manager->entity_hierarchy.node[entity_idx];
        
        line_idx += 1;
        // go further into tree.
        if(node->first_child != 0){
            editor_save_level_entity_recurssive(
                entity_manager, file_path, entity_char_buffer, char_scratch_space, char_scratch_space_length, node->first_child, line_idx, line_idx 
            );
        }
        
        // loop through branch level.
        entity_idx = node->next_sibling;
        if(entity_idx == first_idx){
            break;
        }
    }
}

void editor_on_hover_save_level_button(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        
        EntityManager* entity_manager = (EntityManager*)user_data;

        i32 level_idx = gen_id_get_index(game_state.editor_mouse_state.entity_parent_gid);
        
        String entity_char_buffer = {
            .chars = (char[LEVEL_FILE_LINE_LENGTH]){0},
            .length = LEVEL_FILE_LINE_LENGTH  
        };
        char* char_scratch_space = (char[64]){0};
        String file_path = {.chars = "assets/saved.scsv", .length = 17, .count = 17};
        platform_delete_file(file_path);
        
        editor_save_level_entity_recurssive(
            entity_manager, 
            file_path,
            &entity_char_buffer,
            char_scratch_space,
            64,
            level_idx,
            0,
            0
        );
    }
}

void gfx_clay_test_layout(EntityManager* manager){
    CLAY(CLAY_ID("Box"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_FIXED(512),
                .height = CLAY_SIZING_FIXED(1080)
            },
            .padding = CLAY_PADDING_ALL(24),
            .childGap = 12
        },
        .backgroundColor = { 10, 10, 20, 128},
    }){
        CLAY(CLAY_ID("Red Bird Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_red_bird_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Red Bird"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
        
        CLAY(CLAY_ID("Yellow Bird Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_yellow_bird_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Yellow Bird"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
 
        CLAY(CLAY_ID("Wood Block Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_wood_block_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Wood Block"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
        
        CLAY(CLAY_ID("Pig Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_pig_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Pig"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
        
        for(i32 i = 1; i < manager->entity_hierarchy.root_index_count; i++){
            i32 idx = manager->entity_hierarchy.root_index[i];
            
            BOUNDS_CHECK(idx, manager->entity_length);
            Entity* entity = &manager->entity[idx];
            
            Clay_String name = {.length = entity->name.count, .chars = entity->name.chars};
            
            CLAY(CLAY_SIDI(name, idx), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_PERCENT(0.2f),
                        .height = CLAY_SIZING_PERCENT(0.085f)
                    }
                },
                .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            }){
                Clay_OnHover(editor_on_hover_select_level_button, &manager->gen_id_allocator.gen_ids[idx]);
                CLAY_TEXT(name, { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255}});
            }
        }
        
        
        CLAY(CLAY_ID("Save Level Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_on_hover_save_level_button, manager);
            CLAY_TEXT(CLAY_STRING("Save Level"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
    }
}