///
/// todo:
/// bird damage per km/h or speed lol, do it.
///



///
/// types.
///



typedef struct{
    Matrix4x4 world_camera_matrix;
    Matrix4x4 screen_camera_matrix;
    f32 time;
    f32 world_camera_far_z;
    f32 world_camera_near_z;
    f32 padding_0;
} Ubo;

typedef enum{
    EntityTypeId_None           = 0,
    EntityTypeId_LevelRoot      = 1,
    EntityTypeId_Wall           = 2,
    EntityTypeId_BallCue        = 3,
    EntityTypeId_BallOne        = 4,
    EntityTypeId_BallTwo        = 5,
    EntityTypeId_BallThree      = 6,
    EntityTypeId_BallFour       = 7,
    EntityTypeId_BallFive       = 8,
    EntityTypeId_BallSix        = 9,
    EntityTypeId_BallSeven      = 10,
    EntityTypeId_BallEight      = 11,
    EntityTypeId_BallNine       = 12,
    EntityTypeId_BallZero       = 13,
    EntityTypeId_Pocket         = 14
} EntityTypeId;

typedef struct{
    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
} GameMouseState;

typedef struct{
    String name;
    Transform2D transform;
    Transform2D sprite_local_transform;
    GenId sprite_gid;
    GenId physics_body_gid;
    Aabb clickable_aabb;
    EntityTypeId type_id;
    f32 health;
    bool is_sprite;
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
    FIZX_DrawInfo fizx_draw_state;
    GFX_State* gfx_state;
    TimerManager timer_manager;
    IntrusiveList entity_hierarchy;
    i32 alive_enemies;
    bool is_init;
} EntityManager;

typedef struct{
    GameMouseState mouse_state;
    EntityManager entity_manager;
    f32 time_scale;
    bool is_init;
} GameState;

typedef struct{
    EntityManager* entity_manager;
} CollisionCallbackContext;




///
/// Definitions.
///

#define BALL_FIZX_SHAPE_MATERIAL (FIZX_Material) {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 1.7f, .restitution = 0.334f, .rotational_response = true}
#define BALL_FIZX_BODY_LINEAR_DRAG 0.5f
#define BALL_FIZX_BODY_ANGULAR_DRAG 0.5f

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
/**
    The amount of time in miliseconds that each fixed update should move forwards by.
**/
#define FIXED_DELTA_TIME 0.01666666666666666666666666666667f
/**
    The amount of time that has to be store in the fixed update accumulator
    (in milliseconds) before slowing down the game; avoiding the "spiral of death".
    note that the value is not greater than or equal to the FixedDt * 2,
    this is so that two fixed update steps are never called at a single time.
**/
#define DELTA_TIME_ACCUMULATOR_SLOW_DOWN 0.0333147881012903f

///
/// Physics layers.
///

#define PHYSICS_LAYER_ALL I32_MAX
#define PHYSICS_LAYER_BALL (1 << 1)
#define PHYSICS_LAYER_ENVIRONMENT (1 << 2)
#define PHYSICS_LAYER_RAYCAST (1 << 3)

///
/// Virtual Texture ID.
///
#define VIRTUAL_TEXTURE_ID_FONT 1
#define VIRTUAL_TEXTURE_ID_GAME_BOARD 2

///
/// Texture Views.
///
#define TEXTURE_VIEW_WALL       (GFX_TextureView){.top_left = {0, 0}, .bot_right = {128, 128}}
#define TEXTURE_VIEW_BALL_CUE   (GFX_TextureView){.top_left = {128, 0}, .bot_right = {256, 128}}
#define TEXTURE_VIEW_BALL_ONE   (GFX_TextureView){.top_left = {256, 0}, .bot_right = {384, 128}}
#define TEXTURE_VIEW_BALL_TWO   (GFX_TextureView){.top_left = {384, 0}, .bot_right = {512, 128}}
#define TEXTURE_VIEW_BALL_THREE (GFX_TextureView){.top_left = {512, 0}, .bot_right = {640, 128}}
#define TEXTURE_VIEW_BALL_FOUR  (GFX_TextureView){.top_left = {640, 0}, .bot_right = {768, 128}}
#define TEXTURE_VIEW_BALL_FIVE  (GFX_TextureView){.top_left = {768, 0}, .bot_right = {896, 128}}
#define TEXTURE_VIEW_BALL_SIX   (GFX_TextureView){.top_left = {896, 0}, .bot_right = {1024, 128}}
#define TEXTURE_VIEW_BALL_SEVEN (GFX_TextureView){.top_left = {1024, 0}, .bot_right = {1152, 128}}
#define TEXTURE_VIEW_BALL_EIGHT (GFX_TextureView){.top_left = {1152, 0}, .bot_right = {1280, 128}}
#define TEXTURE_VIEW_BALL_NINE  (GFX_TextureView){.top_left = {1280, 0}, .bot_right = {1408, 128}}
#define TEXTURE_VIEW_BALL_ZERO  (GFX_TextureView){.top_left = {1408, 0}, .bot_right = {1536, 128}}
#define TEXTURE_VIEW_POCKET     (GFX_TextureView){.top_left = {128, 0}, .bot_right = {256, 128}}

#define FONT_HEIGHT_IN_PIXELS 48

#define SPRITE_LAYER_GAME_WORLD 3
#define SPRITE_LAYER_EDITOR_WORLD 2
#define SPRITE_LAYER_GAME_UI 1
#define SPRITE_LAYER_EDITOR_UI 0

#define SPRITE_MATERIAL_DEBUG 1
#define SPRITE_MATERIAL_IMAGE 2
#define SPRITE_MATERIAL_TEXT 3



///
/// globals.
///




static f32 fixed_update_accumulator = 0.0f;




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
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, handle);
    if(!idx){
        return false;
    }
    return timer_manager_timer_stop_unsafe(manager, idx);
}

/*
    `returns`
    true, if the timer was successfully paused; otherwise false, if the `handle` is invalid.
*/
bool timer_manager_timer_pause(TimerManager* manager, TimerHandle handle){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, handle);
    if(!idx){
        return false;
    }
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
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, handle);
    if(!idx){
        return false;
    }
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
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, handle);
    if(!idx){
        return false;
    }
    BOUNDS_CHECK(idx, manager.length);
    return manager.has_started[idx] == TIMER_MANAGER_HAS_STARTED_TRUE;
}

/*
    `returns`
    true if the timer is ticking; otherwise false if it isnt or the handle is invalid.
*/
bool timer_manager_timer_is_ticking(TimerManager manager, TimerHandle handle){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, handle);
    if(!idx){
        return false;
    }
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
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, handle);
    if(!idx){
        return false;
    }
    BOUNDS_CHECK(idx, manager->length);
    manager->time_scale[idx] = time_scale;
    return true;
}

/*
    `returns`
    the delta time between updates that the timer ticked by; otherwise 0 if the timer didn't tick or the handle is invalid.
*/
f32 timer_manager_timer_get_delta_tick_time(TimerManager manager, TimerHandle handle){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, handle);
    if(!idx){
        return 0.0f;
    }
    BOUNDS_CHECK(idx, manager.length);
    return ABS(manager.previous_time[idx] - manager.current_time[idx]);
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

    if(entity->physics_body_gid){
        fizx_body_dealloc(&manager->fizx_state, entity->physics_body_gid);
    }

    if(entity->sprite_gid){
        gfx_dealloc_sprite(manager->gfx_state, entity->sprite_gid);
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
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, entity_gid);
    if(!idx){
        return false;
    }
    BOUNDS_CHECK(idx, manager.entity_length);
    *out_entity = &manager.entity[idx];
    return true;
}

bool entity_manager_is_entity_allocated_unsafe(EntityManager manager, i32 entity_idx){
    BOUNDS_CHECK(entity_idx, manager.gen_id_allocator.length);
    return manager.gen_id_allocator.allocated[entity_idx];
}

void entity_manager_debug_draw(EntityManager manager, f32 delta_time){
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
            gfx_draw_wire_rect(entity_manager.gfx_state, shape , GFX_COLOUR_WHITE, 0.0f, SPRITE_LAYER_GAME_WORLD, SPRITE_MATERIAL_DEBUG);
        }
    }
#endif
}

void pocket_fizx_shape_on_sustain_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    GenId* source_entity_gid = (GenId*)info.source_user_data;
    
    if((info.source_layer & PHYSICS_LAYER_BALL) == 0){
        return;
    }
    
    if(info.depth >= 0.85f){
        entity_manager_dealloc_entity(ctx->entity_manager, *source_entity_gid); 
        platform_output_message("foo!");
    }    
}

GenId entity_spawn_pocket(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        
        entity->type_id = EntityTypeId_Pocket;
        string_push(&entity->name, name);
        entity->transform = transform;
        
        { // physics_body_alloc();
        
            entity->is_physics_body = true;
            Circle physics_shape = {.radius = 0.55f};
            Transform2D physics_shape_transform = TRANSFORM2D_IDENTITY;
            f32 linear_drag = 0.0f;
            f32 angular_drag = 0.0f;
            bool gravity_affected = false;
            void* shape_user_data = &entity_gid;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                entity->transform,
                linear_drag,
                angular_drag,
                gravity_affected
            );
            GenId shape_gid = fizx_circle_collider_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                physics_shape_transform,
                FIZX_ShapeBehaviour_Trigger,
                shape_user_data,
                PHYSICS_LAYER_ENVIRONMENT,
                physics_shape
            );
            fizx_shape_set_on_sustain_callback(
                &entity_manager->fizx_state,
                shape_gid,
                pocket_fizx_shape_on_sustain_callback
            );
        }
        
        { // sprite_alloc();
            
            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, TEXTURE_VIEW_POCKET);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, (GFX_Colour){.r = 0.34f, .g = 0.34f, .b = 0.34f, .a = 1.0f});
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 1);
        }           
    }
    return entity_gid;
}

void raycast_on_sustain_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    Circle shape;
    Vector2 contact_point = {.x = info.first_contact_point_x, .y = info.first_contact_point_y};
    Vector2 normal = {.x = info.normal_x, .y = info.normal_y};
    Vector2 end = vector2_add(contact_point, normal); 
    gfx_draw_line(ctx->entity_manager->gfx_state, GFX_COLOUR_WHITE, contact_point, end, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG, 0.05f);
    if(info.two_contact_points){
        shape =  (Circle){.x = info.first_contact_point_x, .y = info.first_contact_point_y, .radius = 0.33f}; 
        gfx_draw_wire_circle(ctx->entity_manager->gfx_state, shape, GFX_COLOUR_BLUE, SPRITE_LAYER_EDITOR_WORLD, 1, SPRITE_MATERIAL_DEBUG);
        shape = (Circle){.x = info.second_contact_point_x, .y = info.second_contact_point_y, .radius = 0.33f}; 
        gfx_draw_wire_circle(ctx->entity_manager->gfx_state, shape, GFX_COLOUR_ORANGE, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
    }
    else{
        shape =  (Circle){.x = info.first_contact_point_x, .y = info.first_contact_point_y, .radius = 0.33f}; 
        gfx_draw_wire_circle(ctx->entity_manager->gfx_state, shape, GFX_COLOUR_ORANGE, SPRITE_LAYER_GAME_WORLD, 0, SPRITE_MATERIAL_DEBUG);
    }
}

GenId entity_spawn_ball_cue(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_push(&entity->name, name);
        entity->type_id = EntityTypeId_BallCue;
        entity->transform = transform;

        { // physics_body_alloc();

            entity->is_physics_body = true;
            Circle circle = {.radius = 0.55f};
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state, 
                entity->transform, 
                BALL_FIZX_BODY_LINEAR_DRAG, 
                BALL_FIZX_BODY_ANGULAR_DRAG, 
                false
            );
            fizx_circle_rigid_alloc(
                &entity_manager->fizx_state, 
                entity->physics_body_gid, 
                shape_transform, 
                FIZX_ShapeBehaviour_Dynamic, 
                &entity_gid, 
                PHYSICS_LAYER_BALL, 
                circle, 
                BALL_FIZX_SHAPE_MATERIAL 
            );
            GenId raycast = fizx_line_collider_alloc(
                &entity_manager->fizx_state, 
                entity->physics_body_gid, 
                shape_transform, 
                FIZX_ShapeBehaviour_Trigger, 
                &entity_gid, 
                PHYSICS_LAYER_RAYCAST, 
                (Vector2){0},
                VECTOR2_UP,
                2.0f
            );
            fizx_shape_set_on_sustain_callback(&entity_manager->fizx_state, raycast, raycast_on_sustain_callback);
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);    
        }

        { // set_is_clickable();
        
            entity->is_clickable = true;
            entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};        
        }

        { // sprite_alloc();

            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, TEXTURE_VIEW_BALL_CUE);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 0);
        }
    }
    return entity_gid;
}

GenId entity_spawn_wall(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);

        entity->type_id = EntityTypeId_Wall;
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 22.6f, .restitution = 1.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        f32 linear_drag = 0.75f;
        f32 angular_drag = 0.75f;
        bool gravity_effected = false;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, linear_drag, angular_drag, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(
            &entity_manager->fizx_state, 
            entity->physics_body_gid, 
            shape_transform, 
            FIZX_ShapeBehaviour_Kinematic, 
            &entity_gid, 
            PHYSICS_LAYER_ENVIRONMENT, 
            square, 
            material
        );

        entity->is_sprite = true;
        entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
        entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
        gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
        gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, TEXTURE_VIEW_WALL);
        gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
        gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
        gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
        gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
        gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
        gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
        gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 2);
    }
    return entity_gid;
}

GenId entity_spawn_level_root(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        entity->type_id = EntityTypeId_LevelRoot;
        string_push(&entity->name, name);
    }
    return entity_gid;
}

#if 0
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

    i32 source_body_idx = fizx_shape_get_parent_unsafe(ctx->entity_manager->fizx_state, info.source_entity_idx);
    if(!source_body_idx){
        ASSERT(false, "failed to retrieve body idx.");
        return;
    }

    Vector2 velocity = fizx_body_get_linear_velocity_unsafe(ctx->entity_manager->fizx_state, source_body_idx);
    f32 magnitude = vector2_len(velocity);
    f32 damage = magnitude / ((PLAYER_MOUSE_LAUNCH_FORCE * 0.5f) * PLAYER_MOUSE_MAX_DRAW_RADIUS);
    damage = CLAMP(damage, 0.0f, 1.0f);

    ASSERT(entity->is_health, "entity doesnt use health.");
    entity->health -= damage;

    if(entity->health <= 0.0f){
        entity_manager_dealloc_entity(ctx->entity_manager, *entity_gid);
        ctx->entity_manager->alive_enemies-=1;
        if(ctx->entity_manager->alive_enemies <= 0){
            platform_output_message("WIN!");
        }
    }
    else if(entity->health <= 1.0f){
        gfx_sprite_set_texture_view(ctx->entity_manager->gfx_state, entity->sprite_gid, GFX_TEXTURE_VIEW_PIG_CRITICAL);
    }
    else if(entity->health <= 2.0f){
        gfx_sprite_set_texture_view(ctx->entity_manager->gfx_state, entity->sprite_gid, GFX_TEXTURE_VIEW_PIG_HURT);
    }

    PigInvincibleTimerTimeoutContext timeout_data = {
        .entity_manager = ctx->entity_manager,
        .entity_gid = *entity_gid
    };

    char* chars = (char[256]){0};
    // i32 written = snprintf(chars, 256, "%f", entity->health);
    i32 written = snprintf(chars, 256, "%f", damage);
    chars[written] = '\n';
    platform_output_message(chars);

    entity->is_invincible = true;
    timer_manager_timer_start(&ctx->entity_manager->timer_manager, 0.675f, 1.0f, pig_invincible_timer_timeout, &timeout_data, sizeof(timeout_data));
}
#endif

GenId entity_spawn_number_ball(EntityManager* entity_manager, String name, Transform2D transform, GenId parent, i32 ball_number){
    
    GFX_TextureView texture_view;
    EntityTypeId type_id;
    switch(ball_number){
        default:{
            texture_view = (GFX_TextureView){0}; 
            type_id = EntityTypeId_None;
            ASSERT(false, "invalid ball number.");
        }break;
        case 0:{
            texture_view = TEXTURE_VIEW_BALL_ZERO;
            type_id = EntityTypeId_BallZero;
        }break;
        case 1:{
            texture_view = TEXTURE_VIEW_BALL_ONE;
            type_id = EntityTypeId_BallOne;
        }break;
        case 2:{
            texture_view = TEXTURE_VIEW_BALL_TWO;
            type_id = EntityTypeId_BallTwo;
        }break;
        case 3:{
            texture_view = TEXTURE_VIEW_BALL_THREE;
            type_id = EntityTypeId_BallThree;
        }break;
        case 4:{
            texture_view = TEXTURE_VIEW_BALL_FOUR;
            type_id = EntityTypeId_BallFour;
        }break;
        case 5:{
            texture_view = TEXTURE_VIEW_BALL_FIVE;
            type_id = EntityTypeId_BallFive;
        }break;
        case 6:{
            texture_view = TEXTURE_VIEW_BALL_SIX;
            type_id = EntityTypeId_BallSix;
        }break;
        case 7:{
            texture_view = TEXTURE_VIEW_BALL_SEVEN;
            type_id = EntityTypeId_BallSeven;
        }break;
        case 8:{
            texture_view = TEXTURE_VIEW_BALL_EIGHT;
            type_id = EntityTypeId_BallEight;
        }break;
        case 9:{
            texture_view = TEXTURE_VIEW_BALL_NINE;
            type_id = EntityTypeId_BallNine;
        }break;
    }
    
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);

        entity->type_id = type_id;
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        Circle circle = {.radius = 0.55f};
        
        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(
            &entity_manager->fizx_state, 
            entity->transform, 
            BALL_FIZX_BODY_LINEAR_DRAG, 
            BALL_FIZX_BODY_ANGULAR_DRAG,
            false
        );
        
        GenId entity_shape_gid = fizx_circle_rigid_alloc(
            &entity_manager->fizx_state, 
            entity->physics_body_gid, 
            shape_transform, 
            FIZX_ShapeBehaviour_Dynamic, 
            &entity_gid, 
            PHYSICS_LAYER_BALL, 
            circle, 
            BALL_FIZX_SHAPE_MATERIAL 
        );

        entity->is_health = true;
        entity->health = 3;

        entity->is_sprite = true;
        entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
        entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
        gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
        gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, texture_view);
        gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
        gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
        gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
        gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
        gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
        gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
        gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 0);
    }
    entity_manager->alive_enemies+=1;
    return entity_gid;
}

GenId entity_spawn_ball_one(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 1);
}

GenId entity_spawn_ball_two(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 2);
}

GenId entity_spawn_ball_three(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 3);
}

GenId entity_spawn_ball_four(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 4);
}

GenId entity_spawn_ball_five(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 5);
}

GenId entity_spawn_ball_six(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 6);
}

GenId entity_spawn_ball_seven(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 7);
}

GenId entity_spawn_ball_eight(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 8);
}

GenId entity_spawn_ball_nine(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 9);
}

GenId entity_spawn_ball_zero(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_number_ball(entity_manager, name, transform, parent, 0);
}

GenId entity_spawn(EntityManager* entity_manager, EntityTypeId type_id, String name, Transform2D transform, GenId parent_gid){
    switch(type_id){
        default:{
            ASSERT(false, "unknown entity type id.");
            return (GenId){0};
        }break;
        case EntityTypeId_LevelRoot:{
            return entity_spawn_level_root(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_Wall:{
            return entity_spawn_wall(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallCue:{
            return entity_spawn_ball_cue(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallOne:{
            return entity_spawn_ball_one(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallTwo:{
            return entity_spawn_ball_two(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallThree:{
            return entity_spawn_ball_three(entity_manager, name, transform, parent_gid);
        }break;                
        case EntityTypeId_BallFour:{
            return entity_spawn_ball_four(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallFive:{
            return entity_spawn_ball_five(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallSix:{
            return entity_spawn_ball_six(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallSeven:{
            return entity_spawn_ball_seven(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallEight:{
            return entity_spawn_ball_eight(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallNine:{
            return entity_spawn_ball_nine(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_BallZero:{
            return entity_spawn_ball_zero(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_Pocket:{
            return entity_spawn_pocket(entity_manager, name, transform, parent_gid);
        }break;
    }
}

void load_lvl(EntityManager* entity_manager, String file_path){
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
            "%[^;]%n;%i;%i;%f;%f;%f;%f;%f;%f;%n",
            name.chars,
            &name.count,
            &deserialised_entity->entity_type_id,
            &deserialised_entity->parent_deserialised_entity_idx,
            &deserialised_entity->spawn_transform.position.x,
            &deserialised_entity->spawn_transform.position.y,
            &deserialised_entity->spawn_transform.scale.x,
            &deserialised_entity->spawn_transform.scale.y,
            &deserialised_entity->spawn_transform.sine,
            &deserialised_entity->spawn_transform.cosine,
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

        if(lines_read == 9){
            deserialised_entity->entity_gid = entity_spawn(entity_manager, deserialised_entity->entity_type_id, name, deserialised_entity->spawn_transform, parent_gid);
        }

        if(lines_read == 9){
            file_data += bytes_consumed + 1; // add 1 to account for the new line character.
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

    gen_id_allocator_init(&manager->gen_id_allocator, arena, entity_amount);
    intrusive_list_init(&manager->entity_hierarchy, arena, entity_amount, false);
    manager->entity_hierarchy.on_dealloc_callback = entity_on_entity_hierarchy_dealloc;
    manager->gfx_state = gfx_state;
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4, sizeof(GenId));
    timer_manager_init(&manager->timer_manager, arena, entity_amount, timer_timeout_data_size);
    manager->is_init = true;
}

void game_state_init(GameState* game_state, MemoryArena* persistent, MemoryArena* transient, GFX_State* gfx_state){

    ASSERT(!game_state->is_init, "already init.");

    game_state->time_scale = 1.0f;

    i32 entity_amount = 2048;
    i32 physics_body_amount = 128;
    i32 timeout_data_element_size = 512;

    game_state->entity_manager.fizx_draw_state = (FIZX_DrawInfo){
        .colour_dynamic_shape           = GFX_COLOUR_GREEN,
        .colour_passive_trigger_shape   = GFX_COLOUR_LIGHT_BLUE,
        .colour_kinematic_shape         = GFX_COLOUR_ORANGE,
        .colour_active_trigger_shape    = GFX_COLOUR_RED,
        .colour_aabb                    = GFX_COLOUR_LIGHT_BLUE,
        .colour_fallback_shape          = GFX_COLOUR_WHITE,
        .colour_inactive_entity         = GFX_COLOUR_BLACK,
        .colour_bvh_leaf                = GFX_COLOUR_WHITE,
        .colour_bvh_branch              = GFX_COLOUR_LIGHT_GREEN,
        .colour_contact_point           = GFX_COLOUR_RED,
        .colour_linear_velocity         = GFX_COLOUR_WHITE,
        .colour_global_position         = GFX_COLOUR_WHITE,
        .colour_centroid                = GFX_COLOUR_YELLOW,
        .colour_collision_other         = GFX_COLOUR_BLUE,
        .colour_collision_normal        = GFX_COLOUR_LIGHT_BLUE,
        .colour_center_of_mass          = GFX_COLOUR_ORANGE,
        .sprite_layer                   = SPRITE_LAYER_GAME_WORLD,
        .wireframe_thickness            = 0.005f,
        .material_idx                   = SPRITE_MATERIAL_DEBUG,
        .draw_body_shapes               = true
        // .draw_collision_info            = true
    };

    entity_manager_init(&game_state->entity_manager, persistent, gfx_state, entity_amount, physics_body_amount, timeout_data_element_size);
    game_state->is_init = true;
}

void game_state_preupdate(GameState* game_state, f32 delta_time){
    EntityManager* entity_manager = &game_state->entity_manager;
    for(i32 i = 0; i < entity_manager->entity_length; i++){
        Entity* entity = &entity_manager->entity[i];
        if(!entity->is_physics_body){
            continue;
        }
        fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, entity->transform.position);
    }
}

void game_state_update(GameState* game_state, MemoryArena* persistent, MemoryArena* transient, f32 delta_time, bool in_editor_mode){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;
    FIZX_DrawInfo* fizx_draw_state = &entity_manager->fizx_draw_state;
    TimerManager* timer_manager = &entity_manager->timer_manager;
    FIZX_State* fizx_state = &entity_manager->fizx_state;

    // retrieve necessary data.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(gfx_state);

    { // update fizx draw state.

        // if(input_is_key_just_pressed(Key_1)){
        //     fizx_draw_state->draw_bvh_branches = !fizx_draw_state->draw_bvh_branches;
        // }
        // if(input_is_key_just_pressed(Key_2)){
        //     fizx_draw_state->draw_bvh_leaves = !fizx_draw_state->draw_bvh_leaves;
        // }
        if(input_is_key_just_pressed(Key_3)){
            fizx_draw_state->draw_body_shapes = !fizx_draw_state->draw_body_shapes;
        }
        // if(input_is_key_just_pressed(Key_4)){
        //     fizx_draw_state->draw_collision_info = !fizx_draw_state->draw_collision_info;
        // }
    }

    { // time management.

        if(input_is_key_pressed(Key_Space)){
            game_state->time_scale = 0.0f;
        }
        else if(input_is_key_pressed(Key_F)) {
            game_state->time_scale = 0.1f;
        }
        else{
            game_state->time_scale = 1.0f;
        }

        delta_time *= game_state->time_scale;
        timer_manager_update(timer_manager, delta_time);
    }


    { // fixed update.

        fixed_update_accumulator += delta_time;
        if(fixed_update_accumulator > DELTA_TIME_ACCUMULATOR_SLOW_DOWN){
            fixed_update_accumulator = DELTA_TIME_ACCUMULATOR_SLOW_DOWN;
        }

        while(fixed_update_accumulator >= FIXED_DELTA_TIME){

            CollisionCallbackContext collision_callback_ctx = {.entity_manager = entity_manager};
            if(!in_editor_mode){
                fizx_state_fixed_update(fizx_state, &collision_callback_ctx, FIXED_DELTA_TIME, 32);
            }
            fixed_update_accumulator -= FIXED_DELTA_TIME;
        }
    }

    { // move camera.
        gfx_state->world_camera.orthographic_size -= gfx_state->world_camera.orthographic_size * (f32)input_get_mouse_scroll_wheel_delta_value() * delta_time * 0.1f;
    }

    if(!in_editor_mode)
    { // player game update.

        if(input_is_mouse_button_just_pressed(MouseButton_Left)){
            for(i32 e_idx = 0; e_idx < entity_manager->entity_length; e_idx++){
                Entity* entity = &entity_manager->entity[e_idx];
                Aabb world_aabb = aabb_translate(entity->clickable_aabb, entity->transform.position);
                if(aabb_overlaps_point(world_aabb, mouse_world_position)){
                    platform_output_message("clicked entity\n");
                    if(entity->is_physics_body){
                        fizx_body_set_active(fizx_state, entity->physics_body_gid, false);
                    }
                    game_state->mouse_state.clicked_entity_initial_position = entity->transform.position;
                    game_state->mouse_state.clicked_entity_idx = e_idx;
                }
            }
        }

        Vector2 impulse_magnitude;

        if(game_state->mouse_state.clicked_entity_idx){
            i32 entity_idx = game_state->mouse_state.clicked_entity_idx;
            BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
            Entity* entity = &entity_manager->entity[entity_idx];
            Vector2 position_diff = vector2_sub(game_state->mouse_state.clicked_entity_initial_position, mouse_world_position);

            position_diff = vector2_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
            impulse_magnitude = vector2_mul_val(position_diff, PLAYER_MOUSE_LAUNCH_FORCE);

            Vector2 new_position = vector2_sub(game_state->mouse_state.clicked_entity_initial_position, position_diff);

            entity->transform.position = new_position;
            fizx_body_set_global_position(fizx_state, entity->physics_body_gid, new_position);
        }

        if(input_is_mouse_button_just_released(MouseButton_Left)){
            if(game_state->mouse_state.clicked_entity_idx > 0){
                i32 entity_idx = game_state->mouse_state.clicked_entity_idx;
                BOUNDS_CHECK(entity_idx, game_state->entity_manager.entity_length);
                Entity* entity = &entity_manager->entity[entity_idx];
                fizx_body_set_active(fizx_state, entity->physics_body_gid, true);
                game_state->mouse_state.clicked_entity_idx = 0;
                fizx_body_clear_forces_and_velocities(fizx_state, entity->physics_body_gid);
                fizx_body_impulse_force(fizx_state,impulse_magnitude, entity->physics_body_gid);
            }
        }
    }

    {
        for(i32 i = 0; i < entity_manager->entity_length; i++){
            Entity* entity = &entity_manager->entity[i];

            if(entity->is_physics_body){
                if(fizx_body_is_active(fizx_state, entity->physics_body_gid)){
                    Transform2D transform;
                    if(fizx_body_get_transform(fizx_state, entity->physics_body_gid, &transform)){
                        entity->transform.position = transform.position;
                        entity->transform.cosine = transform.cosine;
                        entity->transform.sine = transform.sine;
                    }
                }
            }

            if(entity->is_sprite){
                Transform2D sprite_transform = transform2d_transform(entity->sprite_local_transform, entity->transform);
                gfx_sprite_set_transform(gfx_state, entity->sprite_gid, sprite_transform);
            }
        }
    }
}

void game_state_late_update(GameState* game_state, f32 delta_time){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;

    /**
        NOTE:
        this might have to be swapped for the final render target resolution, maybe idk.
    **/
    f32 aspect_ratio = platform_window_calc_aspect_ratio(*gfx_state->window_ctx);
    gfx_camera_update_projection_matrix(&gfx_state->world_camera, aspect_ratio);
    gfx_camera_update_projection_matrix(&gfx_state->screen_camera, aspect_ratio);

    Ubo ubo = {
        .world_camera_matrix =
            matrix4x4_mul(
                matrix4x4_mul(
                    gfx_state->world_camera.projection,
                    gfx_state->world_camera.view
                ),
            gfx_state->world_camera.model
        ),
        .screen_camera_matrix =
            matrix4x4_mul(
                matrix4x4_mul(
                    gfx_state->screen_camera.projection,
                    gfx_state->screen_camera.view
                ),
            gfx_state->screen_camera.model
        ),
        .world_camera_far_z     = gfx_state->world_camera.far_z,
        .world_camera_near_z    = gfx_state->world_camera.near_z,
    };

    gfx_write_to_user_uniform_buffer(gfx_state, &ubo, sizeof(Ubo));
}

void game_state_draw(GameState* game_state, f32 delta_time){

    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;
    FIZX_State* fizx_state = &entity_manager->fizx_state;
    FIZX_DrawInfo* fizx_draw_state = &entity_manager->fizx_draw_state;

    fizx_state_draw(*fizx_state, gfx_state, *fizx_draw_state, delta_time);

    entity_manager_debug_draw(*entity_manager, delta_time);

    Vector2I mouse_backbuffer_position;
    input_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);

    Clay_SetCurrentContext(gfx_state->clay_game_ui_ctx);
    gfx_clay_begin_layout(
        gfx_state, (Vector2I){.x = gfx_state->window_ctx->width, .y = gfx_state->window_ctx->height}, mouse_backbuffer_position, delta_time,
        input_is_mouse_button_pressed(MouseButton_Left)
    );
    // gfx_clay_test_layout(&entity_manager);
    gfx_clay_end_layout(gfx_state, delta_time, SPRITE_LAYER_GAME_UI, VIRTUAL_TEXTURE_ID_FONT, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
}




///
/// functions: Entity.
///




bool entity_set_position(EntityManager* manager, GenId entity_gid, Vector2 position){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, entity_gid);
    if(!idx){
        return false;
    }
    
    BOUNDS_CHECK(idx, manager->entity_length);
    Entity* entity = &manager->entity[idx];
    
    entity->transform.position = position;
    
    if(entity->is_sprite){
        gfx_sprite_set_position(manager->gfx_state, entity->sprite_gid, position);
    }
    
    if(entity->is_physics_body){
        // TODO: may need to check this in the future, havent tested it.
        fizx_body_set_global_position(&manager->fizx_state, entity->physics_body_gid, position);
    }

    return true;
}

bool entity_set_transform(EntityManager* manager, GenId entity_gid, Transform2D transform){
    Entity* entity;
    if(!entity_manager_get_entity(*manager, entity_gid, &entity)){
        return false;
    }
    entity->transform = transform;
    
    if(entity->is_sprite){
        gfx_sprite_set_transform(manager->gfx_state, entity->sprite_gid, transform);
    }
    
    if(entity->is_physics_body){
        fizx_body_set_global_transform(&manager->fizx_state, entity->physics_body_gid, transform);
    }
    return true;
}