

///
/// todo:
/// turn entities active and inactive.
///


///
/// @ Forward Declarations.
///


struct GameState;


///
/// types.
///


typedef enum{

    // the stage where the player can direct the cue ball.
    BoardTurn_Player,
    
    // the state where the balls move around on the board.
    BoardTurn_Ball,
    
    // the state where the turn based entities get their turn.
    BoardTurn_TurnBased,
    
    // the state when all subsequent turns have completed.
    BoardTurn_Completed
    
} BoardTurn;

typedef enum{  
    BoardStage_Playing,
    BoardStage_Win,
    BoardStage_Lose
} BoardStage;

typedef struct{
    GenId cue_ball_entity_gid;
    GenId* number_ball;
    // the amount of numbered balls still alive on the board.
    i32 number_ball_count;
    i32 number_ball_length;
    
    GenId* turn_based_entity;
    i32 turn_based_entity_length;
    i32 turn_based_entity_count;
    
    BoardTurn turn;
    BoardStage stage;
    
    String turn_text;
    String turn_text_scratch_space;
    i32 turn_number;
    bool display_turn_text;
    bool show_pause_menu;
    bool cue_ball_is_pocketed;
    
    bool is_init;
} BoardState;

typedef struct{
    Matrix4x4 world_camera_matrix;
    Matrix4x4 screen_camera_matrix;
    f32 time;
    f32 world_camera_far_z;
    f32 world_camera_near_z;
    f32 padding_0;
} Ubo;

typedef enum{
    EntityTypeId_None                   = 0,
    EntityTypeId_LevelRoot              = 1,
    EntityTypeId_Wall                   = 2,
    EntityTypeId_BallCue                = 3,
    EntityTypeId_BallOne                = 4,
    EntityTypeId_BallTwo                = 5,
    EntityTypeId_BallThree              = 6,
    EntityTypeId_BallFour               = 7,
    EntityTypeId_BallFive               = 8,
    EntityTypeId_BallSix                = 9,
    EntityTypeId_BallSeven              = 10,
    EntityTypeId_BallEight              = 11,
    EntityTypeId_BallNine               = 12,
    EntityTypeId_BallZero               = 13,
    EntityTypeId_Pocket                 = 14,
    EntityTypeId_BoostPad               = 15,
    EntityTypeId_WindMill               = 16,
    EntityTypeId_WindMillHinge          = 17,
    EntityTypeId_WindMillBlade          = 18,
    EntityTypeId_Thwamper               = 19,
    EntityTypeId_BoardTurnText          = 20,
    EntityTypeId_WorldCamera            = 21,
    EntityTypeId_UiCamera               = 22    
} EntityTypeId;

typedef enum{
    GameStage_Menu,
    GameStage_Board
} GameStage;

typedef struct{
    ClickableAreaHierarchy clickable_entity_hierarchy;

    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
    GenId guideline_physics_body_gid;
    GenId guideline_physics_shape_gid;
    // the end point in world space for the guideline to draw.
    Vector2 guideline_end_point;

    bool hovering_ui_element;

} GameMouseState;

typedef struct GameState GameState;
typedef void (*TurnBasedEntityOnTurnEndCallback)(GameState* game_state, i32 entity_idx);

typedef struct{
    Transform2D global_transform;
    Transform2D local_transform;
    Transform2D sprite_local_transform;
    
    // this is only applicable to the root entity that was loaded from a file.
    String serialisation_file_path;
    
    String name;
    Aabb clickable_aabb;
    EntityTypeId type_id;
    GenId sprite_gid;
    GenId physics_body_gid;
    GenId game_clickable_area_gid;
    i32 state;
    i32 max_turns_until_turn_end_callback;
    i32 turns_until_turn_end_callback; 
    f32 health;
    f32 camera_orthographic_size;
    f32 camera_perspective_fov;
    f32 camera_far_z;
    f32 camera_near_z;
    TurnBasedEntityOnTurnEndCallback on_turn_end_callback;
    bool is_sprite;
    bool is_health;
    bool is_clickable_in_game;
    bool is_physics_body;
    bool is_invincible;
    bool boost_depleted;
    bool serialisation_disabled;
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
    TimerTimeout* timeout_function;
    char* timeout_data;
    i32 timeout_data_length;
    i32 timeout_data_element_size;
    // the length of all backing arrays.
    i32 length;
    bool is_init;
} TimerManager;

typedef GenId TimerHandle;

typedef struct{
    String name;
    String serialisation_file_path;
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
    
    GenIdAllocator gen_id_allocator;
    
    Entity* entity;
    i32 entity_length;
    
    DeserialisedEntity* deserialised_entity;
    i32 deserialised_entity_length;
    i32 deserialised_entity_count;
    
    GFX_State* gfx_state;
    
    FIZX_State fizx_state;
    FIZX_DrawInfo fizx_draw_state;
        
    TimerManager timer_manager;
    
    IntrusiveList entity_hierarchy;

    ClickableAreaHierarchy clickable_area_hierarchy;
    GenId* clickable_area_hierarchy_user_data_scratch_space;
    i32 clickable_area_hierarchy_user_data_scratch_space_length;
    
    GenId world_camera_entity_gid;
    GenId ui_camera_entity_gid;
    
    bool is_init;

} EntityManager;

typedef enum{
    // loads the level file as the world root entity.
    LevelLoadType_World,
    // loads the level file as an entity and parents to the world root entity.
    LevelLoadType_WorldEntity,
    // loads the level file as the ui root entity.
    LevelLoadType_Ui,
    // loads the level file as an entity and parents to the ui root entity.
    LevelLoadType_UiEntity
} LevelLoadType;

typedef void (*DeferredLevelLoadCallback)(GameState* game_state, void* user_data);

typedef struct{
    Transform2D transform;
    String file_path;
    LevelLoadType load_type;
    DeferredLevelLoadCallback callback;
    void* callback_user_data;
} DeferredLevelLoadContext;

typedef struct{
    EntityManager* entity_manager;

    String* world_root_entity_file_path;
    // the gen-id of the entity that is the parent of all entities within the world level.
    GenId world_root_entity_gid;
    
    String ui_root_entity_file_path;
    // the gen-id of the entity that is the parent of all entities within the ui level.
    GenId ui_root_entity_gid;
    
    // the queue of levels to load at an appropriate time (deffered for later).
    DeferredLevelLoadContext* deferred_level_load;
    i32 deferred_level_load_length;
    i32 deferred_level_load_count;    
    bool is_init;
} LevelManager;

typedef enum{
    // MenuStage_MainMenu,
    MenuStage_LevelSelector
} MenuStage;

typedef struct{
    MenuStage stage;
} MenuState;

typedef struct GameState{
    GameMouseState mouse_state;
    EntityManager entity_manager;
    LevelManager level_manager;
    BoardState board_state;
    MenuState menu_state;
    f32 time_scale;
    bool is_init;
    GameStage stage;
} GameState;

typedef struct {
    LevelManager* level_manager;
    DeferredLevelLoadContext deferred_level_load_ctx;
    String file_path;
    LevelLoadType load_type;
    DeferredLevelLoadCallback callback;
    char* callback_user_data;
    i32 callback_user_data_length;
} LoadLevelButtonOnHoverContext;

typedef struct{
    GameState* game_state;
} CollisionCallbackContext;

typedef struct{
    GenId ball_cue_physics_gid;
    EntityManager* entity_manager;
} BallCueFragmentationTimerTimeoutContext;

typedef struct{
    GenId boost_pad_entity_gid;
    EntityManager* entity_manager;
} BoostPadBoostDepletedTimerTimeoutContext;


///
/// @ defines
///


#define BOARD_STATE_MAX_BALLS 10

#define ASSET_DIRECTORY_PATH "../assets/"
#define ASSET_DIRECTORY_PATH_LENGTH 10
#define LEVEL_ASSET_DIRECTORY_PATH "../assets/levels/"
#define LEVEL_ASSET_DIRECTORY_PATH_LENGTH 17
#define ENTITY_ASSET_DIRECTORY_PATH "../assets/entities/"
#define ENTITY_ASSET_DIRECTORY_PATH_LENGTH 19

#define LEVEL_FILE_NATIVE_ENTITY_TOKEN 'N'
#define LEVEL_FILE_ASSET_ENTITY_TOKEN 'A'

#define BALL_FIZX_SHAPE_MATERIAL (FIZX_Material) {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 1.85f, .restitution = 0.45f, .rotational_response = true}
#define BALL_FIZX_BODY_LINEAR_DRAG 1.1f
#define BALL_FIZX_BODY_ANGULAR_DRAG 1.35f

#define ENTITY_NAME_LENGTH 32
#define PLAYER_MOUSE_MAX_DRAW_RADIUS 3.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.0f

#define LEVEL_FILE_LINE_LENGTH 2048
     
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
#define PHYSICS_LAYER_BALL_NUMBER       (1 << 1)
#define PHYSICS_LAYER_ENVIRONMENT_TOP   (1 << 2)
#define PHYSICS_LAYER_ENVIRONMENT_MID   (1 << 3)
#define PHYSICS_LAYER_ENVIRONMENT_BOT   (1 << 4)
#define PHYSICS_LAYER_RAYCAST           (1 << 5)
#define PHYSICS_LAYER_GUIDELINE         (1 << 6)
#define PHYSICS_LAYER_BALL_CUE          (1 << 7)
#define PHYSICS_LAYER_BALL (PHYSICS_LAYER_BALL_CUE | PHYSICS_LAYER_BALL_NUMBER)

#define BALL_CUE_DEFAULT_PHYSICS_LAYER_MASK (PHYSICS_LAYER_BALL_CUE | PHYSICS_LAYER_BALL_NUMBER | PHYSICS_LAYER_ENVIRONMENT_MID | PHYSICS_LAYER_ENVIRONMENT_BOT)
#define BALL_CUE_FRAGMENTATION_PHYSICS_LAYER_MASK (PHYSICS_LAYER_BALL_NUMBER | PHYSICS_LAYER_ENVIRONMENT_MID) 


///
/// Virtual Texture ID.
///


#define VIRTUAL_TEXTURE_ID_FONT 1
#define VIRTUAL_TEXTURE_ID_GAME_BOARD 2


///
/// @ texture views.
///


#define TEXTURE_VIEW_WALL           (GFX_TextureView){.top_left = {0, 0}, .bot_right = {128, 128}}
#define TEXTURE_VIEW_BALL_CUE       (GFX_TextureView){.top_left = {1664, 0}, .bot_right = {1792, 128}}
#define TEXTURE_VIEW_BALL_ONE       (GFX_TextureView){.top_left = {256, 0}, .bot_right = {384, 128}}
#define TEXTURE_VIEW_BALL_TWO       (GFX_TextureView){.top_left = {384, 0}, .bot_right = {512, 128}}
#define TEXTURE_VIEW_BALL_THREE     (GFX_TextureView){.top_left = {512, 0}, .bot_right = {640, 128}}
#define TEXTURE_VIEW_BALL_FOUR      (GFX_TextureView){.top_left = {640, 0}, .bot_right = {768, 128}}
#define TEXTURE_VIEW_BALL_FIVE      (GFX_TextureView){.top_left = {768, 0}, .bot_right = {896, 128}}
#define TEXTURE_VIEW_BALL_SIX       (GFX_TextureView){.top_left = {896, 0}, .bot_right = {1024, 128}}
#define TEXTURE_VIEW_BALL_SEVEN     (GFX_TextureView){.top_left = {1024, 0}, .bot_right = {1152, 128}}
#define TEXTURE_VIEW_BALL_EIGHT     (GFX_TextureView){.top_left = {1152, 0}, .bot_right = {1280, 128}}
#define TEXTURE_VIEW_BALL_NINE      (GFX_TextureView){.top_left = {1280, 0}, .bot_right = {1408, 128}}
#define TEXTURE_VIEW_BALL_ZERO      (GFX_TextureView){.top_left = {1408, 0}, .bot_right = {1536, 128}}
#define TEXTURE_VIEW_BOOST_PAD      (GFX_TextureView){.top_left = {1536, 0}, .bot_right = {1664, 128}}
#define TEXTURE_VIEW_POCKET         (GFX_TextureView){.top_left = {128, 0}, .bot_right = {256, 128}}

#define FONT_HEIGHT_IN_PIXELS 48

#define SPRITE_LAYER_GAME_WORLD 3
#define SPRITE_LAYER_EDITOR_WORLD 2
#define SPRITE_LAYER_GAME_UI 1
#define SPRITE_LAYER_EDITOR_UI 0

#define SPRITE_MATERIAL_DEBUG 1
#define SPRITE_MATERIAL_IMAGE 2
#define SPRITE_MATERIAL_TEXT 3


///
/// forward declarations.
///


GenId level_manager_load_level_file(LevelManager* level_manager, String file_path, MemoryArena* transient, Transform2D transform);
void level_manager_defer_load_level(LevelManager* level_manager, DeferredLevelLoadContext ctx);


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
    `remarks`
    `timeout_data` is deep copied into the internal data storage.
    `duration`: the duration - in seconds - of the timer before timing out.

    `returns`
    a handle to the newly started timer; otherwise 0 if allocation was unsuccessful.
*/
TimerHandle timer_manager_timer_start(TimerManager* manager, f32 duration, f32 time_scale, TimerTimeout timer_timeout_function, void* timeout_data, size_t timeout_data_size){
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
    manager->current_time[idx] = duration;
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


///
/// @ BoardState
///


typedef struct{
    BoardState* board_state;
} BoardStateDisplayTurnHUDTimerContext;

void board_state_display_turn_hud_timeout(void* user_data){
    BoardStateDisplayTurnHUDTimerContext* ctx = (BoardStateDisplayTurnHUDTimerContext*)user_data;
    ctx->board_state->display_turn_text = false;
}

void board_state_init(BoardState* state, MemoryArena* arena, i32 max_turn_based_entities){
    if(state->is_init){
        ASSERT(false, "already init.");
        return;
    }
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, state->number_ball, &state->number_ball_length, BOARD_STATE_MAX_BALLS);
    state->number_ball_count = 0;
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, state->turn_based_entity, &state->turn_based_entity_length, max_turn_based_entities);
    state->turn_based_entity_count = 0;
    
    string_init(&state->turn_text, arena, 64);
    string_init(&state->turn_text_scratch_space, arena, 64);
    
    state->is_init = true;
}

void board_state_reset(BoardState* state){
    state->cue_ball_entity_gid = 0;
    state->turn = BoardTurn_Player;
    state->stage = BoardStage_Playing;
    state->number_ball_count = 0;
    state->turn_based_entity_count = 0;
    state->turn_number = 0;
    state->display_turn_text = false;
    state->show_pause_menu = false;
    state->cue_ball_is_pocketed = false;
}


///
/// @ EntityManager
///


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
    
    if(entity->is_physics_body){
        fizx_body_dealloc(&manager->fizx_state, entity->physics_body_gid);
    }

    if(entity->sprite_gid){
        bool success = gfx_sprite_dealloc(manager->gfx_state, entity->sprite_gid);
        ASSERT(success, "f");
    }
    
    if(entity->is_clickable_in_game){
        clickable_area_hierarchy_dealloc_clickable_area(&manager->clickable_area_hierarchy, entity->game_clickable_area_gid);
    }
    
    String name = entity->name;
    string_clear(&name);
    String serialisation_file_path = entity->serialisation_file_path;
    string_clear(&serialisation_file_path);
    
    *entity = (Entity){0};
    
    // ensure that the name tag isnt fucked with.
    entity->name = name;
    entity->serialisation_file_path = serialisation_file_path;
    
    gen_id_allocator_dealloc_unsafe(&manager->gen_id_allocator, idx);
}

void entity_on_entity_hierarchy_dealloc(IntrusiveList* list, i32 node_idx, void* user_data){
    EntityManager* manager = (EntityManager*)user_data;
    entity_manager_dealloc_entity_data_unsafe(manager, node_idx);
}

bool entity_manager_dealloc_entity(EntityManager* manager, GenId gid){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, gid);
    if(!idx){
        return false;
    }

    // TODO:
    // may want to add functionality later so the user can deallocate an entity independently from its children.

    // NOTE:
    // the on_dealloc callback for this calls entity_manager_dealloc_entity_data_unsafe.
    bool preserve_sub_tree = false;
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

void entity_manager_init(
    EntityManager* manager, MemoryArena* arena, GFX_State* gfx_state,
    i32 entity_amount, i32 physics_body_amount, i32 timer_timeout_data_size
){
    ASSERT(!manager->is_init, "already init.");

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->entity, &manager->entity_length, entity_amount);
    for(i32 i = 0; i < manager->entity_length; i++){
        Entity* entity = &manager->entity[i];
        string_init(&entity->name, arena, ENTITY_NAME_LENGTH);
        string_init(&entity->serialisation_file_path, arena, os_get_max_file_path_length());
    }
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->deserialised_entity, &manager->deserialised_entity_length, entity_amount);
    manager->deserialised_entity_count = 0;
    for(i32 i = 0; i < manager->deserialised_entity_length; i++){
        DeserialisedEntity* deserialised_entity = &manager->deserialised_entity[i];
        string_init(&deserialised_entity->name, arena, ENTITY_NAME_LENGTH);
        string_init(&deserialised_entity->serialisation_file_path, arena, os_get_max_file_path_length());
    }

    gen_id_allocator_init(&manager->gen_id_allocator, arena, entity_amount);
    
    intrusive_list_init(&manager->entity_hierarchy, arena, entity_amount, false);
    
    manager->entity_hierarchy.on_dealloc_callback = entity_on_entity_hierarchy_dealloc;
    
    manager->gfx_state = gfx_state;
    
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4, sizeof(GenId));
    
    timer_manager_init(&manager->timer_manager, arena, entity_amount, timer_timeout_data_size);
    
    { // init_clickable_area_hierarchy();
    
        clickable_area_hierarchy_init(&manager->clickable_area_hierarchy, arena, entity_amount);
        MEMORY_ARENA_ALLOC_ARRAY(
            arena,
            manager->clickable_area_hierarchy_user_data_scratch_space, 
            &manager->clickable_area_hierarchy_user_data_scratch_space_length,
            entity_amount
        );
    }
    
    manager->is_init = true;
}

bool entity_manager_get_world_camera(EntityManager entity_manager, Entity** out_entity){
    return entity_manager_get_entity(entity_manager, entity_manager.world_camera_entity_gid, out_entity);    
}

bool entity_manager_get_ui_camera(EntityManager entity_manager, Entity** out_entity){
    return entity_manager_get_entity(entity_manager, entity_manager.ui_camera_entity_gid, out_entity);    
}


///
/// functions: Entity.
///


void entity_set_local_transform_unsafe_ext(EntityManager* manager, i32 entity_idx, Transform2D transform, bool update_components_transform){

    BOUNDS_CHECK(entity_idx, manager->entity_length);
    Entity* entity = &manager->entity[entity_idx];
    
    entity->local_transform = transform;
    BOUNDS_CHECK(entity_idx, manager->entity_hierarchy.length);
    IntrusiveListNode* node = &manager->entity_hierarchy.node[entity_idx];

    { // recalculate_global_transform();
        if(node->parent){
            BOUNDS_CHECK(node->parent, manager->entity_length);
            Entity* parent_entity = &manager->entity[node->parent];
            entity->global_transform = transform2d_transform(entity->local_transform, parent_entity->global_transform);
        }
        else{
            entity->global_transform = transform;
        }
    }
    
    i32 child_idx = node->first_child;    
    while(child_idx){

        BOUNDS_CHECK(child_idx, manager->entity_length);
        Entity* child_entity = &manager->entity[child_idx];
        
        entity_set_local_transform_unsafe_ext(manager, child_idx, child_entity->local_transform, update_components_transform);

        BOUNDS_CHECK(child_idx, manager->entity_hierarchy.length);
        IntrusiveListNode* child_node = &manager->entity_hierarchy.node[child_idx];
        
        child_idx = child_node->next_sibling;
        if(child_idx == node->first_child){
            break;
        }
    }
    
    if(!update_components_transform){
        return;
    }
    
    // after calculate the global transform.

    if(entity->is_sprite){
        Transform2D sprite_transform = transform2d_transform(entity->sprite_local_transform, entity->global_transform);
        if(gfx_gen_id_is_sprite_string(*manager->gfx_state, entity->sprite_gid)){
            gfx_sprite_string_set_transform(manager->gfx_state, entity->sprite_gid, sprite_transform);            
        }
        else{
            gfx_sprite_set_transform(manager->gfx_state, entity->sprite_gid, sprite_transform);
        }
    }

    if(entity->is_physics_body){
        fizx_body_set_global_transform(&manager->fizx_state, entity->physics_body_gid, entity->global_transform);
    }
}

void entity_set_local_transform_unsafe(EntityManager* manager, i32 entity_idx, Transform2D transform){
    bool update_components_transform = true;
    entity_set_local_transform_unsafe_ext(manager, entity_idx, transform, update_components_transform);
}

bool entity_set_local_transform(EntityManager* manager, GenId entity_gid, Transform2D transform){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, entity_gid); 
    if(!idx){
        return false;
    }
    
    entity_set_local_transform_unsafe(manager, idx, transform);
        
    return true;
}

void entity_set_global_transform_unsafe_ext(EntityManager* manager, i32 entity_idx, Transform2D transform, bool update_components_transform){
    BOUNDS_CHECK(entity_idx, manager->entity_length);
    Entity* entity = &manager->entity[entity_idx];
    
    entity->global_transform = transform;
    
    IntrusiveListNode* node = &manager->entity_hierarchy.node[entity_idx];
     
    { // recalculate_local_transform();
        
        if(node->parent){
            BOUNDS_CHECK(node->parent, manager->entity_length);
            Entity* parent_entity = &manager->entity[node->parent];
            
            Transform2D result;
            result.scale = vector2_div(
                entity->global_transform.scale,
                parent_entity->global_transform.scale 
            );
            
            result.cosine = 
                entity->global_transform.cosine * parent_entity->global_transform.cosine + 
                entity->global_transform.sine * parent_entity->global_transform.sine;
                
            result.sine = 
                entity->global_transform.sine * parent_entity->global_transform.cosine -
                entity->global_transform.cosine * parent_entity->global_transform.sine;
            
            { // calculate_result_position();
            
                Vector2 delta_pos = vector2_sub(
                    entity->global_transform.position,
                    parent_entity->global_transform.position 
                );
                
                // inverse rotation: rotate by -parent_angle (transpose of the rotation matrix).
                Vector2 rotated_pos;
                rotated_pos.x = 
                    delta_pos.x * parent_entity->global_transform.cosine +
                    delta_pos.y * parent_entity->global_transform.sine;
                rotated_pos.y = 
                    -delta_pos.x * parent_entity->global_transform.sine +
                    delta_pos.y * parent_entity->global_transform.cosine;
                
                if(vector2_equals(parent_entity->global_transform.scale, (Vector2){0})){
                    result.position = (Vector2){0};
                }
                else{
                    // inverse scale.
                    result.position = vector2_div(rotated_pos, parent_entity->global_transform.scale);                
                }
            }
                         
            entity->local_transform = result;
        }
        else{
            entity->local_transform = transform;
        }
    }
    
    i32 child_idx = node->first_child;
    while(child_idx){

        BOUNDS_CHECK(child_idx, manager->entity_length);
        Entity* child_entity = &manager->entity[child_idx];
        
        entity_set_local_transform_unsafe_ext(manager, child_idx, child_entity->local_transform, update_components_transform);    

        BOUNDS_CHECK(child_idx, manager->entity_hierarchy.length);
        IntrusiveListNode* child_node = &manager->entity_hierarchy.node[child_idx];
        
        child_idx = child_node->next_sibling;
        if(child_idx == node->first_child){
            break;
        }
    }
    
    if(!update_components_transform){
        return;
    }
    
    if(entity->is_sprite){
        gfx_sprite_set_transform(manager->gfx_state, entity->sprite_gid, transform2d_transform(entity->sprite_local_transform, entity->global_transform));
    }

    if(entity->is_physics_body){
        fizx_body_set_global_transform(&manager->fizx_state, entity->physics_body_gid, entity->global_transform);
    }
}

void entity_set_global_transform_unsafe(EntityManager* manager, i32 entity_idx, Transform2D transform){
    bool update_components_transform = true;
    entity_set_global_transform_unsafe_ext(manager, entity_idx, transform, update_components_transform);
}

bool entity_set_global_transform(EntityManager* entity_manager, GenId entity_gid, Transform2D global_transform){
    i32 idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, entity_gid);
    if(!idx){
        return false;
    }
    entity_set_global_transform_unsafe(entity_manager, idx, global_transform);
    return true;
}

bool entity_set_parent_unsafe(EntityManager* entity_manager, i32 child_idx, i32 parent_idx){

    if(!intrusive_list_set_node_parent(&entity_manager->entity_hierarchy, child_idx, parent_idx)){
        return false;
    }

    BOUNDS_CHECK(child_idx, entity_manager->entity_length);
    Entity* child_entity = &entity_manager->entity[child_idx];
    Transform2D global_transform = child_entity->global_transform;
    
    entity_set_global_transform_unsafe(entity_manager, child_idx, global_transform);
    
    return true;
}

bool entity_set_parent(EntityManager* entity_manager, GenId child_gid, GenId parent_gid){
    
    i32 child_idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, child_gid);
    if(!child_idx){
        return false;
    }
    
    i32 parent_idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, parent_gid);
    if(!parent_idx){
        return false;
    }
    
    return entity_set_parent_unsafe(entity_manager, child_idx, parent_idx);
}


///
/// @ Level Load Callbacks.
///


typedef struct {
    GameState* game_state;
}LoadBoardLevelCallbackContext;

void load_level_callback(GameState* game_state, void* user_data){
    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    BoardState* board_state = &game_state->board_state;

    clickable_area_hierarchy_reset(&game_state->entity_manager.clickable_area_hierarchy);
    game_state->mouse_state.clicked_entity_idx = 0;
    board_state_reset(board_state);
    
    // reset_entity_manager();
    entity_manager->world_camera_entity_gid = 0;
    entity_manager->ui_camera_entity_gid = 0;    
}

typedef struct{
    MenuStage stage_to_load_to;
} LoadMenuLevelCallbackContext;

void load_menu_level_callback(GameState* game_state, void* user_data){
    LoadMenuLevelCallbackContext* ctx = (LoadMenuLevelCallbackContext*)user_data;
    
    load_level_callback(game_state, user_data);
    game_state->stage = GameStage_Menu;
    game_state->menu_state.stage = ctx->stage_to_load_to; 
}

void load_board_level_callback(GameState* game_state, void* user_data){    
    
    load_level_callback(game_state, user_data);
    
    // hoisting invariance.
    BoardState* board_state = &game_state->board_state;
    GenIdAllocator* gid_allocator = &game_state->entity_manager.gen_id_allocator;
    
    game_state->stage = GameStage_Board;    
    
    for(i32 i = 0; i < gid_allocator->length; i++){
    
        if(!gid_allocator->allocated[i]){
            continue;
        }
        
        GenId entity_gid = gid_allocator->gen_ids[i];
        Entity* entity;
        
        
        if(entity_manager_get_entity(game_state->entity_manager, entity_gid, &entity)){
            
            { // check_if_turn_based();
                if(entity->on_turn_end_callback){
                    ARRAY_PUSH(board_state->turn_based_entity, board_state->turn_based_entity_length, &board_state->turn_based_entity_count, entity_gid);    
                }
            }
                        
            { // check_if_ball();
            
                switch(entity->type_id){
                    case EntityTypeId_BallOne:
                    case EntityTypeId_BallTwo:
                    case EntityTypeId_BallThree:
                    case EntityTypeId_BallFour: 
                    case EntityTypeId_BallFive: 
                    case EntityTypeId_BallSix:  
                    case EntityTypeId_BallSeven: 
                    case EntityTypeId_BallEight: 
                    case EntityTypeId_BallNine:  
                    case EntityTypeId_BallZero:{
                        ARRAY_PUSH(board_state->number_ball, board_state->number_ball_length, &board_state->number_ball_count, entity_gid);
                    }break;
                    case EntityTypeId_BallCue:{
                        board_state->cue_ball_entity_gid = entity_gid;
                    }break;
                    case EntityTypeId_WorldCamera:
                        game_state->entity_manager.world_camera_entity_gid = entity_gid;
                    break;
                }
            }
        }
        else{
            ASSERT(false, "gid is allocated but failed to get entity.");
        }
    }
}


///
/// @ Entity.
///


void generic_entity_update(Entity* entity, EntityManager* entity_manager){
    
    ClickableAreaHierarchy* clickable_area_hierarchy = &entity_manager->clickable_area_hierarchy;
    GFX_State* gfx_state = entity_manager->gfx_state;
    
    if(entity->is_clickable_in_game)
    { // recalculate_clickable_area();
        
        Transform2D preferred_transform = {0};
        PolygonRectangle clickable_area_shape = {0};
        u64 depth_layer = 0;
        
        // note that we infer this is a sprite; no need to check 'is_sprite' flag or 'sprite_gid'.
        if(gfx_sprite_get_transform(*gfx_state, entity->sprite_gid, &preferred_transform)){

            GFX_SpriteOrigin sprite_origin;

            // falback to entity transform if we failed to get the sprite origin.
            if(!gfx_sprite_get_sprite_origin(*gfx_state, entity->sprite_gid, &sprite_origin)){
                preferred_transform = entity->global_transform;
                clickable_area_shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
            }

            switch(sprite_origin){
                default:
                    ASSERT(false, "unknown sprite origin");
                case GFX_SpriteOrigin_Center:{
                    clickable_area_shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                }break;
                case GFX_SpriteOrigin_TopLeft:{
                    clickable_area_shape = transform2d_to_polygon_rectangle_top_left_origin(preferred_transform);
                }break;
            }
            gfx_sprite_get_depth_layer(gfx_state, entity->sprite_gid, &depth_layer);
        }

        // is not a sprite.
        else{
            preferred_transform = entity->global_transform;
            clickable_area_shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);        
        }
        
        clickable_area_hierarchy_set_clickable_area_depth_layer(
            clickable_area_hierarchy,
            entity->game_clickable_area_gid,
            depth_layer
        );
        
        // draw_clickable_wireframe();
        // gfx_draw_wire_poly(
        //     gfx_state, 
        //     clickable_area_shape.x,
        //     clickable_area_shape.y,
        //     4, 
        //     GFX_COLOUR_RED, 
        //     SPRITE_LAYER_EDITOR_WORLD, 
        //     0, 
        //     SPRITE_MATERIAL_DEBUG
        // );

        clickable_area_hierarchy_set_clickable_area_rectangle_shape(
            clickable_area_hierarchy,
            entity->game_clickable_area_gid,
            clickable_area_shape.x,
            clickable_area_shape.y
        );
    }
}

bool entity_set_active(EntityManager* entity_manager, GenId entity_gid, bool is_active){

    i32 entity_idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, entity_gid);
    if(!entity_idx){
        return false;
    }

    BOUNDS_CHECK(entity_idx, entity_manager->entity_hierarchy.length);
    entity_manager->entity_hierarchy.node[entity_idx].is_active = is_active;

    BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
    Entity* entity = &entity_manager->entity[entity_idx];
    
    if(entity->is_physics_body){
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, is_active);
    }
    
    if(entity->is_sprite){
        switch(is_active){
            case true:{
                gfx_sprite_set_active(entity_manager->gfx_state, entity->sprite_gid);
            }break;
            case false:{
                gfx_sprite_set_inactive(entity_manager->gfx_state, entity->sprite_gid);
            }break;
        }
    }
    
    return true;
}

inline bool entity_is_active_unsafe(EntityManager entity_manager, i32 entity_idx){
    BOUNDS_CHECK(entity_idx, entity_manager.entity_hierarchy.length);
    return entity_manager.entity_hierarchy.node[entity_idx].is_active;
}

bool entity_is_active(EntityManager entity_manager, GenId entity_gid){
    i32 idx = gen_id_allocator_is_gen_id_valid(&entity_manager.gen_id_allocator, entity_gid);
    if(!idx){
        return false;
    }
    return entity_is_active_unsafe(entity_manager, idx);
}


///
/// @ BoardTurnText.
///


GenId entity_spawn_board_turn_text(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_Thwamper;
        string_push(&entity->name, name);
        
        { // sprite_alloc();


            String str = {
                .chars = "text",
                .length = 4,
                .count = 4
            };
            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_one_frame_sprite_chain_alloc(entity_manager->gfx_state, str.length);
            // order matters here: text->transform.
            gfx_sprite_string_set_active(       entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_string_set_text(         entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_FONT, str);
            gfx_sprite_string_set_transform(    entity_manager->gfx_state, entity->sprite_gid, transform);
            gfx_sprite_string_set_material(     entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_TEXT);
            gfx_sprite_string_set_layer(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_UI);
            gfx_sprite_string_set_depth(        entity_manager->gfx_state, entity->sprite_gid, 0);
            gfx_sprite_string_set_colour(       entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
            gfx_sprite_string_set_colour_state( entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ Camera.
///


#define CAMERA_POSITIONAL_OFFSET (Vector3){.z = -1.0f}
#define WORLD_CAMERA_COORDINATE_SPACE GFX_CoordinateSpace_Cartesian
#define WORLD_CAMERA_DEFAULT_NEAR_Z 0.01f
#define WORLD_CAMERA_DEFAULT_FAR_Z 100.0f
#define WORLD_CAMERA_DEFAULT_ORTHOGRAPHIC_SIZE 22.0f
#define UI_CAMERA_COORDINATE_SPACE GFX_CoordinateSpace_Rasterised 
#define UI_CAMERA_DEFAULT_NEAR_Z 0.01f
#define UI_CAMERA_DEFAULT_FAR_Z 100.0f

GenId entity_spawn_world_camera(EntityManager* entity_manager, String name, Transform2D transform, GenId parent_entity_gid){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent_entity_gid);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_WorldCamera;
        string_push(&entity->name, name);
        
        { // set_camera_settings();
        
            entity->camera_orthographic_size = WORLD_CAMERA_DEFAULT_ORTHOGRAPHIC_SIZE * MAX(transform.scale.x, transform.scale.y);
            entity->camera_near_z = WORLD_CAMERA_DEFAULT_NEAR_Z;
            entity->camera_far_z = WORLD_CAMERA_DEFAULT_FAR_Z;
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ Thwamper.
///


typedef enum{
    ThwamperState_Idle,
    ThwamperState_SmackDown
} ThwamperState;

typedef struct{
    EntityManager* entity_manager;
    GenId thwamper_entity_gid;
} ThwamperIdleStateTimeoutContext;

#define THWAMPER_IDLE_TIME 0.5f
#define THWAMPER_MAX_TURNS_UNTIL_TURN_END_CALLBACK 3
#define THWAMPER_LAUNCH_FORCE 7.0f

#define THWAMPER_TEXTURE_VIEW           (GFX_TextureView){.top_left = {1792, 0}, .bot_right = {1920, 128}}
#define THWAMPER_DEFAULT_PHYSICS_LAYER  PHYSICS_LAYER_ENVIRONMENT_MID
#define THWAMPER_DEFAULT_PHYSICS_MASK   PHYSICS_LAYER_ALL
#define THWAMPER_PHYSICS_MATERIAL       (FIZX_Material){.static_friction = 0.75f, .kinetic_friction = 0.65f, .density = 10.0f, .restitution = 0.084f}

void thwamper_smackdown_area_query_callback(FIZX_State* state, FIZX_QueryCollisionInfo info, void* user_data){
    i32 thwamper_body_idx = *(i32*)user_data;
    i32 body_idx = fizx_shape_get_parent_unsafe(*state, info.target_entity_idx);
    if(body_idx == thwamper_body_idx){
        return;
    }
    
    Vector2 force = vector2_mul_val((Vector2){.x = info.normal_x, .y = info.normal_y}, THWAMPER_LAUNCH_FORCE);
    fizx_body_impulse_linear_force_unsafe(state, body_idx, force);
}

void thwamper_on_turn_end_callback(GameState* game_state, i32 entity_idx){
    os_output_message("thwamp! thwamp! thwamp!\n");
    BOUNDS_CHECK(entity_idx, game_state->entity_manager.entity_length);
    Entity* entity = &game_state->entity_manager.entity[entity_idx];
    i32 body_idx = gen_id_get_index(entity->physics_body_gid);
    Circle area = {.x = entity->global_transform.position.x, .y = entity->global_transform.position.y, 2.0f};
    fizx_state_query_circle_area(&game_state->entity_manager.fizx_state, thwamper_smackdown_area_query_callback, &body_idx, PHYSICS_LAYER_BALL, area);
    gfx_draw_wire_circle(game_state->entity_manager.gfx_state, area, GFX_COLOUR_WHITE, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);    
}

GenId entity_spawn_thwamper(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_Thwamper;
        string_push(&entity->name, name);
        
        { // physics_body_alloc();

            entity->is_physics_body = true;
            f32 linear_drag = 0.0f;
            f32 angular_drag = 0.0f;
            bool is_gravity_affected = false;
            
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                linear_drag,
                angular_drag,
                THWAMPER_DEFAULT_PHYSICS_LAYER,
                THWAMPER_DEFAULT_PHYSICS_MASK,
                is_gravity_affected
            );

            Circle physics_shape = {.radius = 0.5f};
            Transform2D physics_shape_transform = TRANSFORM2D_IDENTITY;
            void* shape_user_data = &entity_gid;
                        
            GenId shape_gid = fizx_circle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                physics_shape_transform,
                FIZX_ShapeBehaviour_Kinematic,
                FIZX_CollisionCallbackBehaviour_All,
                shape_user_data,
                physics_shape,
                THWAMPER_PHYSICS_MATERIAL
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
        }

        { // sprite_alloc();

            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, THWAMPER_TEXTURE_VIEW);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 1);
        }
        
        { // turn_based();
            entity->max_turns_until_turn_end_callback = THWAMPER_MAX_TURNS_UNTIL_TURN_END_CALLBACK;
            entity->turns_until_turn_end_callback = THWAMPER_MAX_TURNS_UNTIL_TURN_END_CALLBACK;
            entity->on_turn_end_callback = thwamper_on_turn_end_callback;
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}

///
/// @ WindMillBlade.
///


#define WINDMILL_BLADE_TEXTURE_VIEW      (GFX_TextureView){.top_left = {0, 0}, .bot_right = {128, 128}}
#define WINDMILL_BLADE_PHYSICS_MATERIAL (FIZX_Material){.static_friction = 0.75f, .kinetic_friction = 0.65f, .density = 10.0f, .restitution = 0.084f}

GenId entity_spawn_windmill_blade(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_WindMillBlade;
        string_push(&entity->name, name);
        
        entity->serialisation_disabled = true;

        f32 width = 3.0f;
        f32 height = 0.1f;

        { // physics_shape_alloc();

            Entity* parent_entity;            
            if(
                entity_manager_get_entity(*entity_manager, parent, &parent_entity) &&
                parent_entity->is_physics_body                
            ){

                Rectangle physics_shape = {.x = 0.0f, .y = 0.0f, .width = width, .height = height};
                Transform2D physics_shape_transform = transform;
                void* shape_user_data = &entity_gid;

                GenId shape_gid = fizx_rectangle_rigid_alloc(
                    &entity_manager->fizx_state,
                    parent_entity->physics_body_gid,
                    physics_shape_transform,
                    FIZX_ShapeBehaviour_Kinematic,
                    FIZX_CollisionCallbackBehaviour_All,
                    shape_user_data,
                    physics_shape,
                    WINDMILL_BLADE_PHYSICS_MATERIAL
                );
            }
            else{
                ASSERT(false, "parent doesnt have a physics body.");
            }
        }        
    
        { // sprite_alloc();

            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_local_transform.scale.x = width;
            entity->sprite_local_transform.scale.y = height;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, WINDMILL_BLADE_TEXTURE_VIEW);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_TopLeft);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, (GFX_Colour){.r = 0.34f, .g = 0.34f, .b = 0.34f, .a = 1.0f});
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 1);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    
    return entity_gid;
}


///
/// @ WindMillHinge.
///


#define WINDMILL_HINGE_DEFAULT_PHYSICS_LAYER  PHYSICS_LAYER_ENVIRONMENT_MID
#define WINDMILL_HINGE_DEFAULT_PHYSICS_MASK   PHYSICS_LAYER_ALL 

void windmill_hinge_update(Entity* entity, i32 entity_idx, GameState* game_state, f32 delta_time){    
    // Transform2D rotated_transform = transform2d_rotate(entity->global_transform, to_radians(66.67f) * delta_time);
    // entity_set_global_transform_unsafe(&game_state->entity_manager, entity_idx, rotated_transform);
}

GenId entity_spawn_windmill_hinge(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
        
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){

        entity->type_id = EntityTypeId_WindMillHinge;
        string_push(&entity->name, name);
        
        entity->serialisation_disabled = true;
        
        { // physics_body();
        
            entity->is_physics_body = true;
            f32 linear_drag = 0.0f;
            f32 angular_drag = 0.0f;
            bool is_gravity_affected = false;
            void* shape_user_data = &entity_gid;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                linear_drag,
                angular_drag,
                WINDMILL_HINGE_DEFAULT_PHYSICS_LAYER,
                WINDMILL_HINGE_DEFAULT_PHYSICS_MASK,
                is_gravity_affected
            );
                
            // start spinning
            fizx_body_impulse_angular_force(&entity_manager->fizx_state, entity->physics_body_gid, to_radians(66.64f));
        }

        String blade_name = {.chars = "wind_mill_blade", .count = 15, .length = 15};
        Entity* blade_entity;
        
        { // forward_blade();

            GenId blade_gid = entity_spawn_windmill_blade(entity_manager, blade_name, transform2d_rotate(TRANSFORM2D_IDENTITY, degrees_to_radians(0)), entity_gid);
            if(!entity_manager_get_entity(*entity_manager, blade_gid, &blade_entity)){
                entity_manager_dealloc_entity(entity_manager, entity_gid);
                return 0;
            }
            blade_entity = NULL;
        }
        
        { // down_blade();

            GenId blade_gid = entity_spawn_windmill_blade(entity_manager, blade_name, transform2d_rotate(TRANSFORM2D_IDENTITY, degrees_to_radians(90)), entity_gid);
            if(!entity_manager_get_entity(*entity_manager, blade_gid, &blade_entity)){
                entity_manager_dealloc_entity(entity_manager, entity_gid);
                return 0;
            }
            blade_entity = NULL;
        }
        
        { // backward_blade();

            GenId blade_gid = entity_spawn_windmill_blade(entity_manager, blade_name, transform2d_rotate(TRANSFORM2D_IDENTITY, degrees_to_radians(180)), entity_gid);
            if(!entity_manager_get_entity(*entity_manager, blade_gid, &blade_entity)){
                entity_manager_dealloc_entity(entity_manager, entity_gid);
                return 0;
            }
            blade_entity = NULL;
        }

        { // up_blade();
        
            GenId blade_gid = entity_spawn_windmill_blade(entity_manager, blade_name, transform2d_rotate(TRANSFORM2D_IDENTITY, degrees_to_radians(270)), entity_gid);
            if(!entity_manager_get_entity(*entity_manager, blade_gid, &blade_entity)){
                entity_manager_dealloc_entity(entity_manager, entity_gid);
                return 0;
            }
            blade_entity = NULL;
        }
        
        // set the fizx body active here as the blades allocate their shapes into the hinge's body.
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
    
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ WindMill.
///


#define WINDMILL_BODY_TEXTURE_VIEW      (GFX_TextureView){.top_left = {128, 0}, .bot_right = {256, 128}}
#define WINDMILL_DEFAULT_PHYSICS_LAYER  PHYSICS_LAYER_ENVIRONMENT_MID
#define WINDMILL_DEFAULT_PHYSICS_MASK   PHYSICS_LAYER_ALL 
#define WINDMILL_PHYSICS_MATERIAL (FIZX_Material){.static_friction = 0.75f, .kinetic_friction = 0.65f, .density = 10.0f, .restitution = 0.167f}

GenId entity_spawn_windmill(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_WindMill;
        string_push(&entity->name, name);
                
        { // physics_body_alloc();

            entity->is_physics_body = true;
            f32 linear_drag = 0.0f;
            f32 angular_drag = 0.0f;
            bool is_gravity_affected = false;

            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                linear_drag,
                angular_drag,
                WINDMILL_DEFAULT_PHYSICS_LAYER,
                WINDMILL_DEFAULT_PHYSICS_MASK,
                is_gravity_affected
            );
            
            Circle physics_shape = {.radius = 0.5f};
            Transform2D physics_shape_transform = TRANSFORM2D_IDENTITY;
            void* shape_user_data = &entity_gid;
            
            GenId shape_gid = fizx_circle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                physics_shape_transform,
                FIZX_ShapeBehaviour_Kinematic,
                FIZX_CollisionCallbackBehaviour_All,
                shape_user_data,
                physics_shape,
                WINDMILL_PHYSICS_MATERIAL
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
        }
                
        { // sprite_alloc();

            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, WINDMILL_BODY_TEXTURE_VIEW);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, (GFX_Colour){.r = 0.34f, .g = 0.34f, .b = 0.34f, .a = 1.0f});
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 1);
        }

        String hinge_name = {.chars = "wind_mill_hinge", .count = 15, .length = 15};
        if(!entity_spawn_windmill_hinge(entity_manager, hinge_name, TRANSFORM2D_IDENTITY, entity_gid)){
            entity_manager_dealloc_entity(entity_manager, entity_gid);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ Pocket.
///


void pocket_fizx_shape_on_sustain_callback(FIZX_CollisionInfo info, void* user_data){
    
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    GenId source_entity_gid = *(GenId*)info.source_user_data;

    if((info.source_layer & PHYSICS_LAYER_BALL) == 0){
        return;
    }

    if(info.depth >= 0.85f){
        Entity* entity;
        if(!entity_manager_get_entity(ctx->game_state->entity_manager, source_entity_gid, &entity)){
            return;
        }
        
        switch(entity->type_id){
            case EntityTypeId_BallCue:{
                ctx->game_state->board_state.cue_ball_is_pocketed = true;
            }
            case EntityTypeId_BallEight:
            case EntityTypeId_BallOne:
            case EntityTypeId_BallTwo:
            case EntityTypeId_BallThree:
            case EntityTypeId_BallFour:
            case EntityTypeId_BallFive:
            case EntityTypeId_BallSix:
            case EntityTypeId_BallSeven:
            case EntityTypeId_BallNine:
            case EntityTypeId_BallZero:{
                entity_set_active(&ctx->game_state->entity_manager, source_entity_gid, false);
            }break;
        }
                
    }
}

GenId entity_spawn_pocket(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){

        entity->type_id = EntityTypeId_Pocket;
        string_push(&entity->name, name);

        { // physics_body_alloc();

            entity->is_physics_body = true;
            f32 linear_drag = 0.0f;
            f32 angular_drag = 0.0f;
            bool is_gravity_affected = false;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                linear_drag,
                angular_drag,
                PHYSICS_LAYER_ENVIRONMENT_MID,
                PHYSICS_LAYER_ALL,
                is_gravity_affected
            );

            Circle physics_shape = {.radius = 0.55f};
            Transform2D physics_shape_transform = TRANSFORM2D_IDENTITY;
            void* shape_user_data = &entity_gid;
            
            GenId shape_gid = fizx_circle_collider_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                physics_shape_transform,
                FIZX_ShapeBehaviour_Trigger,
                FIZX_CollisionCallbackBehaviour_All,
                shape_user_data,
                physics_shape
            );
            
            fizx_shape_set_on_sustain_callback(
                &entity_manager->fizx_state,
                shape_gid,
                pocket_fizx_shape_on_sustain_callback
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
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
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ BallCue.
///


GenId entity_spawn_ball_cue(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    i32 entity_idx = gen_id_get_index(entity_gid);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_push(&entity->name, name);
        entity->type_id = EntityTypeId_BallCue;

        { // physics_body_alloc();

            entity->is_physics_body = true;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                BALL_FIZX_BODY_LINEAR_DRAG,
                BALL_FIZX_BODY_ANGULAR_DRAG,
                PHYSICS_LAYER_BALL_CUE,
                BALL_CUE_DEFAULT_PHYSICS_LAYER_MASK,
                false
            );

            Circle circle = {.radius = 0.55f};
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;
            fizx_circle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                shape_transform,
                FIZX_ShapeBehaviour_Dynamic,
                FIZX_CollisionCallbackBehaviour_All,
                &entity_gid,
                circle,
                BALL_FIZX_SHAPE_MATERIAL
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
        }

        { // set_is_clickable_in_game();

            entity->is_clickable_in_game = true;
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
        
        { // clickable_area_alloc();
            
            // init_user_data();
            BOUNDS_CHECK(entity_idx, entity_manager->clickable_area_hierarchy_user_data_scratch_space_length);
            GenId* user_data = &entity_manager->clickable_area_hierarchy_user_data_scratch_space[entity_idx]; 
            COPY_MEMORY(user_data, &entity_gid, sizeof(entity_gid));
            
            // alloc_clickable_area();
            entity->game_clickable_area_gid = clickable_area_hierarchy_alloc_clickable_area(&entity_manager->clickable_area_hierarchy);
            
            // set_user_data();
            clickable_area_hierarchy_set_clickable_area_user_data(
                &entity_manager->clickable_area_hierarchy, 
                entity->game_clickable_area_gid, 
                entity
            );
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}

void ball_cue_fragment_timer_timeout(void* user_data){
    BallCueFragmentationTimerTimeoutContext* ctx = (BallCueFragmentationTimerTimeoutContext*)user_data;
    fizx_body_set_mask(&ctx->entity_manager->fizx_state, ctx->ball_cue_physics_gid, BALL_CUE_DEFAULT_PHYSICS_LAYER_MASK);
}

void ball_cue_update(Entity* entity, GameState* game_state, f32 delta_time){
    // os_output_message("update cue ball!\n");
    if(input_is_key_just_pressed(Key_A)){

        if(entity->is_physics_body){            
            fizx_body_set_mask(&game_state->entity_manager.fizx_state, entity->physics_body_gid, BALL_CUE_FRAGMENTATION_PHYSICS_LAYER_MASK);
            BallCueFragmentationTimerTimeoutContext ctx = {
                .ball_cue_physics_gid = entity->physics_body_gid,
                .entity_manager = &game_state->entity_manager
            };
            
            timer_manager_timer_start(&game_state->entity_manager.timer_manager, 0.334f, 1.0f, ball_cue_fragment_timer_timeout, &ctx, sizeof(ctx));
            
            #if 0
            
            Vector2 velocity = fizx_body_get_linear_velocity(game_state->entity_manager.fizx_state, entity->physics_body_gid);
            Vector2 direction = vector2_normalise(velocity);
            f32 velocity_len = vector2_len(velocity);
            
            
            { // inner_left_ball_fragment();

                Vector2 new_direction = vector2_rotate(direction, -0.334f);     
                Vector2 impulse_force = vector2_mul_val(new_direction, velocity_len);
                
                GenId fragment_gid = entity_spawn_ball_cue(
                    &game_state->entity_manager, 
                    (String){.chars = "spawned cue ball", .count = 16, .length = 16}, 
                    entity->transform, 
                    game_state->level_manager.world_root_entity_gid
                );
                
                Entity* fragment_entity;
                if(entity_manager_get_entity(game_state->entity_manager, fragment_gid, &fragment_entity)){
                    fizx_body_set_mask(&game_state->entity_manager.fizx_state, fragment_entity->physics_body_gid, BALL_CUE_FRAGMENTATION_PHYSICS_LAYER_MASK);
                    fizx_body_impulse_force(&game_state->entity_manager.fizx_state, fragment_entity->physics_body_gid, impulse_force);
                }
            }
            
            { // inner_right_ball_fragment();

                Vector2 new_direction = vector2_rotate(direction, 0.334f);     
                Vector2 impulse_force = vector2_mul_val(new_direction, velocity_len);
                
                GenId fragment_gid = entity_spawn_ball_cue(
                    &game_state->entity_manager, 
                    (String){.chars = "spawned cue ball", .count = 16, .length = 16}, 
                    entity->transform, 
                    game_state->level_manager.world_root_entity_gid
                );
                
                Entity* fragment_entity;
                if(entity_manager_get_entity(game_state->entity_manager, fragment_gid, &fragment_entity)){
                    fizx_body_set_mask(&game_state->entity_manager.fizx_state, fragment_entity->physics_body_gid, BALL_CUE_FRAGMENTATION_PHYSICS_LAYER_MASK);
                    fizx_body_impulse_force(&game_state->entity_manager.fizx_state, fragment_entity->physics_body_gid, impulse_force);
                }
            }
            #endif
            
            // Vector2 outter_left_direction = vector2_rotate(direction, -0.99f);            
            // Transform2D outter_left_transform = entity->transform;
            // outter_left_transform.position = vector2_sub(outter_left_transform.position, outter_left_direction);
            // entity_spawn_ball_cue(
            //     &game_state->entity_manager, 
            //     (String){.chars = "spawned cue ball", .count = 16, .length = 16}, 
            //     outter_left_transform, 
            //     game_state->level_manager.world_root_entity_gid
            // );
            
            
            // Vector2 inner_left_direction = vector2_rotate(direction, -0.167f);
        }
    }
}


///
/// @ Wall.
///


GenId entity_spawn_wall(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);

        entity->type_id = EntityTypeId_Wall;
        
        { // alloc_physics_body();
            entity->is_physics_body = true;
            
            f32 linear_drag = 0.75f;
            f32 angular_drag = 0.75f;
            bool gravity_effected = false;
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state, 
                TRANSFORM2D_IDENTITY, 
                linear_drag, 
                angular_drag, 
                PHYSICS_LAYER_ENVIRONMENT_MID,
                PHYSICS_LAYER_ALL,
                false
            );
            
            Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;
            FIZX_Material material = {
                .static_friction = 0.75f, 
                .kinetic_friction = 0.5f, 
                .density = 22.6f, 
                .restitution = 1.0f, 
                .rotational_response = true
            };
            fizx_rectangle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                shape_transform,
                FIZX_ShapeBehaviour_Kinematic,
                FIZX_CollisionCallbackBehaviour_All,
                &entity_gid,
                square,
                material
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);     
        }        

        { // alloc_sprite();
        
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
    
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ LevelRoot.
///


GenId entity_spawn_level_root(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        entity->type_id = EntityTypeId_LevelRoot;
        string_push(&entity->name, name);
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}


///
/// @ NumberBall.
///


GenId entity_spawn_game_ball(EntityManager* entity_manager, String name, Transform2D transform, GenId parent, i32 ball_number){

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

        { // alloc_physics_body();
            entity->is_physics_body = true;
    
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                BALL_FIZX_BODY_LINEAR_DRAG,
                BALL_FIZX_BODY_ANGULAR_DRAG,
                PHYSICS_LAYER_BALL_NUMBER,
                PHYSICS_LAYER_ALL,
                false
            );
            
            Circle circle = {.radius = 0.55f};    
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;    
            GenId entity_shape_gid = fizx_circle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                shape_transform,
                FIZX_ShapeBehaviour_Dynamic,
                FIZX_CollisionCallbackBehaviour_All,
                &entity_gid,
                circle,
                BALL_FIZX_SHAPE_MATERIAL
            );
            
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
        }
        
        { // alloc_sprite();
        
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
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
    return entity_gid;
}

GenId entity_spawn_ball_one(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 1);
}

GenId entity_spawn_ball_two(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 2);
}

GenId entity_spawn_ball_three(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 3);
}

GenId entity_spawn_ball_four(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 4);
}

GenId entity_spawn_ball_five(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 5);
}

GenId entity_spawn_ball_six(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 6);
}

GenId entity_spawn_ball_seven(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 7);
}

GenId entity_spawn_ball_eight(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 8);
}

GenId entity_spawn_ball_nine(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 9);
}

GenId entity_spawn_ball_zero(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    return entity_spawn_game_ball(entity_manager, name, transform, parent, 0);
}


///
/// @ BoostPad.
///


void boost_pad_boost_depleted_timeout(void* user_data){
    BoostPadBoostDepletedTimerTimeoutContext* ctx = (BoostPadBoostDepletedTimerTimeoutContext*)user_data;
    Entity* entity;
    if(!entity_manager_get_entity(*ctx->entity_manager, ctx->boost_pad_entity_gid, &entity)){
        return;
    }
    entity->boost_depleted = false;
}

void boost_pad_on_sustain_collision_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    GenId* boost_pad_entity_gid = (GenId*)info.target_user_data;
    GenId* ball_entity_gid = (GenId*)info.source_user_data;
    
    if((info.source_layer & PHYSICS_LAYER_BALL) == 0){
        return;
    }
    
    if(info.depth <= 0.334f){
        return;
    }
    
    Entity* boost_pad_entity;
    if(!entity_manager_get_entity(ctx->game_state->entity_manager, *boost_pad_entity_gid, &boost_pad_entity)){
        return;
    }
    
    if(boost_pad_entity->boost_depleted){
        return;
    }
    
    Entity* ball_entity;
    if(!entity_manager_get_entity(ctx->game_state->entity_manager, *ball_entity_gid, &ball_entity)){
        return;
    }
    
    Vector2 forward_direction = transform2d_get_forward_angle_vector(boost_pad_entity->global_transform);    
    f32 impulse_force = 5.5f;
    Vector2 impulse = vector2_mul_val(forward_direction, impulse_force);
    
    if(ball_entity->is_physics_body){
        fizx_body_impulse_linear_force(&ctx->game_state->entity_manager.fizx_state, ball_entity->physics_body_gid, impulse);
        BoostPadBoostDepletedTimerTimeoutContext timer_ctx = {
            .entity_manager = &ctx->game_state->entity_manager,
            .boost_pad_entity_gid = *boost_pad_entity_gid
        };
        timer_manager_timer_start(
            &ctx->game_state->entity_manager.timer_manager, 
            1.0f, 
            1.0f, 
            boost_pad_boost_depleted_timeout,
            &timer_ctx, 
            sizeof(timer_ctx)
        );
    }
}

GenId entity_spawn_boost_pad(EntityManager* entity_manager, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        entity->type_id = EntityTypeId_BoostPad;
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        f32 angular_drag = 0.0f;
        f32 linear_drag = 0.0f;
        
        { // alloc_physics_body();
        
            entity->is_physics_body = true;
            
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state,
                TRANSFORM2D_IDENTITY,
                linear_drag,
                angular_drag,
                PHYSICS_LAYER_ENVIRONMENT_BOT,
                PHYSICS_LAYER_ALL,
                false
            );
            
            Rectangle rect = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
            GenId shape_gid = fizx_rectangle_collider_alloc(
                &entity_manager->fizx_state, entity->physics_body_gid, 
                TRANSFORM2D_IDENTITY, 
                FIZX_ShapeBehaviour_Trigger, 
                FIZX_CollisionCallbackBehaviour_All, 
                &entity_gid, 
                rect
            );
            
            fizx_shape_set_on_sustain_callback(&entity_manager->fizx_state, shape_gid, boost_pad_on_sustain_collision_callback);
        
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
        }
        
        { // alloc_sprite();
        
            entity->is_sprite = true;
            entity->sprite_local_transform = TRANSFORM2D_IDENTITY;
            entity->sprite_gid = gfx_sprite_alloc(entity_manager->gfx_state);
            gfx_sprite_set_active(          entity_manager->gfx_state, entity->sprite_gid);
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, TEXTURE_VIEW_BOOST_PAD);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, GFX_COLOUR_WHITE);
            gfx_sprite_set_colour_state(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourState_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_GAME_WORLD);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 3);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
    }
        
    return entity_gid;
}

GenId entity_spawn(EntityManager* entity_manager, EntityTypeId type_id, String name, Transform2D transform, GenId parent_gid){
    switch(type_id){
        default:{
            ASSERT(false, "unknown entity type id.");
            return (GenId){0};
        }break;
        case EntityTypeId_None:{
            os_output_message("[Warning]: editor attempted to spawn an entity without a entity type id; (EntityTypeId_None).\n");
            return 0;
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
        case EntityTypeId_BoostPad:{
            return entity_spawn_boost_pad(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_WindMill:{
            return entity_spawn_windmill(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_Thwamper:{
            return entity_spawn_thwamper(entity_manager, name, transform, parent_gid);
        }break;
        case EntityTypeId_WorldCamera:{
            return entity_spawn_world_camera(entity_manager, name, transform, parent_gid);
        }break;
    }
}


///
/// functions: LevelManager.
///



void level_manager_init(
    LevelManager* level_manager, MemoryArena* arena, 
    EntityManager* entity_manager, i32 deferred_level_load_length
){
    
    ASSERT(!level_manager->is_init, "level manager is already init.");
    *level_manager = (LevelManager){0};
    
    i32 file_path_length = os_get_max_file_path_length();
    string_init(&level_manager->ui_root_entity_file_path, arena, file_path_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, level_manager->deferred_level_load, &level_manager->deferred_level_load_length, deferred_level_load_length);
    for(i32 i = 0; i < deferred_level_load_length; i++){
        DeferredLevelLoadContext* ctx = &level_manager->deferred_level_load[i];
        string_init(&ctx->file_path, arena, file_path_length);
    }
    
    level_manager->entity_manager = entity_manager;
    level_manager->is_init = false;
}

void level_manager_defer_load_level(LevelManager* level_manager, DeferredLevelLoadContext ctx){
    // get ctx.
    i32 idx = level_manager->deferred_level_load_count;
    BOUNDS_CHECK(idx, level_manager->deferred_level_load_length);
    DeferredLevelLoadContext* internal_ctx = &level_manager->deferred_level_load[idx];
    
    *internal_ctx = ctx;            
    level_manager->deferred_level_load_count++;
}

/*
    `parameters`:
    `parent_idx_offset`: this should be set to `0` when calling this outside of the function; i.e, just always set it to `0`.
    `root_parent_idx`: the index of the current root entity's parent in the deserialised entity array; this should be set to `1` when first calling the recursion/this function.
    `root_transform`: the transform override for the loaded asset root entity.
*/
GenId level_manager_load_level_file_recurssive(
    LevelManager* level_manager, String file_path, MemoryArena* transient, 
    i32 parent_idx_offset, i32 root_parent_idx, Transform2D root_transform
){
    
    EntityManager* entity_manager = level_manager->entity_manager;

    /*
        .scsv are .csv files that are separated with ';' instead of ','

        a level file is as follows:

        entity_id ; entity_position_x ; entity_position_y ; entity_scale_x ; entity_scale_y ; entity_rotation_radians ;
    */

    size_t buffer_size;
    void* raw_file = os_load_file(file_path, &buffer_size);
    char* file_data = (char*)raw_file;

    DeserialisedEntity* deserialised_entity;
    i32 lines_read = 1;
    i32 bytes_consumed = 0;
    
    // only reset the deserialised entity storage on the first call so that
    // any asset entity entries in the level file dont reset the shared deserialised entity storage.
    // otherwise the returned GenID of this function would be the root of the last entity asset file that was loaded
    // rather than the parent of all of them, i.e, the level we want to load.
    ASSERT(parent_idx_offset >= 0, "invalid offset");
    if(parent_idx_offset <= 0){
        // zero the first two deserialised entities.
        // this is to preserve the NIL as well as the first loaded entity.
        // specifically for the first loaded entity, in the case that the 'while'
        // loop fails on the first iteration, this ensures that the later returning of 
        // 'entity_manager->deserialised_entity[1].entity_gid' is zero when the level cannot be loaded at all.
        ZERO_MEMORY(entity_manager->deserialised_entity, 2);
    
        // skip the NIL
        // this is because the level files (.scsv) ordering defines what is the parent of an entity.
        // for instance, an entity on line 12 can specify that it is the a child of the entity on line 3.
        // 0 means that it doesnt have a parent at all. 
        entity_manager->deserialised_entity_count = 1;
    }
    
    while(true){
    
        // ensure that we can load the entity.
        i32 next_count = entity_manager->deserialised_entity_count + 1;
        if(next_count > entity_manager->deserialised_entity_length){
            ASSERT(false, "cannot store all required deserialised entities from level file.");
            break;
        }

        // get the entity token.
        char token;
        lines_read = sscanf(
            file_data,
            "%c;",
            &token
        );
        
        // go forward by two bytes:
        // one for the token, one for the ';' delimiter.
        file_data += 2;
        
        if(lines_read != 1){
            break;
        }
        
        switch(token){
            default :{ASSERT(false, "unknown entity file line token.");}break;
            case LEVEL_FILE_ASSET_ENTITY_TOKEN:{
                String scratch_space = {0};
                string_init(&scratch_space, transient, os_get_max_file_path_length());
                
                lines_read = sscanf(
                    file_data,
                    "%[^;]%n;%f;%f;%f;%f;%f;%f;%n",
                    scratch_space.chars,
                    &scratch_space.count,
                    &root_transform.position.x,
                    &root_transform.position.y,
                    &root_transform.scale.x,
                    &root_transform.scale.y,
                    &root_transform.sine,
                    &root_transform.cosine,
                    &bytes_consumed
                );
                
                if(lines_read != 7){
                    ASSERT(false, "mangled entity serialisation found.");
                    break;
                }
                
                level_manager_load_level_file_recurssive(level_manager, scratch_space, transient, entity_manager->deserialised_entity_count-1, root_parent_idx, root_transform);
            }break;
            case LEVEL_FILE_NATIVE_ENTITY_TOKEN: {

                // get the slot to load into.
                BOUNDS_CHECK(entity_manager->deserialised_entity_count, entity_manager->deserialised_entity_length);
                deserialised_entity = &entity_manager->deserialised_entity[entity_manager->deserialised_entity_count];

                deserialised_entity->spawn_transform = TRANSFORM2D_IDENTITY;
                lines_read = sscanf(
                    file_data,
                    "%[^;]%n;%i;%i;%f;%f;%f;%f;%f;%f;%n",
                    deserialised_entity->name.chars,
                    &deserialised_entity->name.count,
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
        
                if(lines_read == 9){
            
                    // NOTE:
                    // deserialsied entities should be in the order (from 0 to length) parent->child.
                    // such that entities at the end can be parented to ones near the start.
                    //
                    // EXAMPLE:
                    // [0] is the NIL.
                    // [1] parent_deserialised_entity_idx = 0; // the root.
                    // [2] parent_deserialised_entity_idx = 1; // references the line above.
                    // [3] parent_deserialised_entity_idx = 4; // this is not okay as it is a later in the list and hasnt been loaded yet, also its out of bounds.
                    i32 parent_idx = deserialised_entity->parent_deserialised_entity_idx; // note that parenting is by line in the level file.
                    BOUNDS_CHECK(parent_idx, entity_manager->deserialised_entity_count);
                    
                    GenId parent_gid;
                    if(parent_idx <= 0){
                        
                        // override the transform whenever it is a root (both for the level root and neseted asset file root).
                        deserialised_entity->spawn_transform = root_transform;

                        // this it is an asset entity file's root entity (parent_idx_offset > 0), 
                        // it should be parented to the previous entity tree's root. 
                        // if not, it shouldnt be parented to anything.
                        
                        parent_gid = parent_idx_offset > 0
                        ? entity_manager->deserialised_entity[root_parent_idx].entity_gid
                        : 0;
                        
                        // this is the parent of the next asset entity's root.
                        root_parent_idx = entity_manager->deserialised_entity_count;
                    }
                    else{
                                                
                        // this isa native entity within the asset entity file.
                        parent_gid = entity_manager->deserialised_entity[parent_idx + parent_idx_offset].entity_gid;
                    }
                                        
                    // create the new deserialised entity.
                    deserialised_entity->entity_gid = entity_spawn(
                        entity_manager, 
                        deserialised_entity->entity_type_id, 
                        deserialised_entity->name, 
                        deserialised_entity->spawn_transform, 
                        parent_gid
                    );
           
                    // root entity from file.
                    if(parent_idx == 0){
                        Entity* root_entity;
                        if(entity_manager_get_entity(*entity_manager, deserialised_entity->entity_gid, &root_entity)){
                            // set the root entity's serialisation file path to the file path we just loaded it from.
                            string_push(&root_entity->serialisation_file_path, file_path);
                        }
                        else{
                            ASSERT(false, "failed to retrieve root entity.");
                            break;
                        }
                    }
                    
                    entity_manager->deserialised_entity_count += 1;
                }    
                else{
                    ASSERT(false, "mangled entity serialisation found.");
                    break;
                }

            }break;
             
        }        
        
        file_data += bytes_consumed + 1; // add 1 to account for the subsequent '\n'.
    
    }
    os_free_memory(raw_file);
    
    // return the root entity.
    BOUNDS_CHECK(1, entity_manager->deserialised_entity_length);
    return entity_manager->deserialised_entity[1].entity_gid;  
}

GenId level_manager_load_level_file(
    LevelManager* level_manager, String file_path, MemoryArena* transient, Transform2D transform 
){
    return level_manager_load_level_file_recurssive(level_manager, file_path, transient, 0, 1, transform);
}


///
/// @ GameState
///


void guideline_collider_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    ctx->game_state->mouse_state.guideline_end_point = (Vector2){info.first_contact_point_x, info.first_contact_point_y};
    Vector2 end = {info.first_contact_point_x + info.normal_x, info.first_contact_point_y + info.normal_y};
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
        .sprite_layer                   = SPRITE_LAYER_EDITOR_WORLD,
        .wireframe_thickness            = 0.005f,
        .material_idx                   = SPRITE_MATERIAL_DEBUG
        // .draw_body_shapes               = true,
        // .draw_bvh_branches              = true,
        // .draw_bvh_leaves                = true
        // .draw_collision_info            = true
    };

    entity_manager_init(&game_state->entity_manager, persistent, gfx_state, entity_amount, physics_body_amount, timeout_data_element_size);
    
    game_state->mouse_state.guideline_physics_body_gid = fizx_body_alloc(
        &game_state->entity_manager.fizx_state, 
        TRANSFORM2D_IDENTITY, 
        0.0f, 
        0.0f, 
        PHYSICS_LAYER_GUIDELINE,
        PHYSICS_LAYER_BALL_NUMBER | PHYSICS_LAYER_ENVIRONMENT_MID,
        false
    );
    
    i32 x = 0;
    game_state->mouse_state.guideline_physics_shape_gid = fizx_line_collider_alloc(
        &game_state->entity_manager.fizx_state, 
        game_state->mouse_state.guideline_physics_body_gid, 
        TRANSFORM2D_IDENTITY,
        FIZX_ShapeBehaviour_Trigger,
        FIZX_CollisionCallbackBehaviour_Closest,
        &x,
        (Vector2){0}, 
        (Vector2){0}, 
        0.0f
    );
    
    fizx_shape_set_on_enter_callback(&game_state->entity_manager.fizx_state, game_state->mouse_state.guideline_physics_shape_gid, guideline_collider_callback);
    fizx_shape_set_on_sustain_callback(&game_state->entity_manager.fizx_state, game_state->mouse_state.guideline_physics_shape_gid, guideline_collider_callback);
    
    fizx_body_set_active(
        &game_state->entity_manager.fizx_state,
        game_state->mouse_state.guideline_physics_body_gid,
        false
    );
    
    level_manager_init(&game_state->level_manager, persistent, &game_state->entity_manager, 256);
        
    board_state_init(&game_state->board_state, persistent, entity_amount);
    
    game_state->is_init = true;
}

void game_state_preupdate(GameState* game_state, f32 delta_time, MemoryArena* transient){

    // hoist invariance.
    LevelManager* level_manager = &game_state->level_manager;
    EntityManager* entity_manager = &game_state->entity_manager;

    { // level_manager_update();
    
        Entity* world_entity;
        for(i32 i = 0; i < level_manager->deferred_level_load_count; i++){
            BOUNDS_CHECK(i, level_manager->deferred_level_load_length);
            DeferredLevelLoadContext* ctx = &level_manager->deferred_level_load[i];
            switch(ctx->load_type){
                default:{
                    ASSERT(false, "undefined level load type.");
                }break;
                case LevelLoadType_WorldEntity:{
                    // spawn entity.
                    GenId entity_gid = level_manager_load_level_file(level_manager, ctx->file_path, transient, ctx->transform);
                    // add to world tree.
                    entity_set_parent(entity_manager, entity_gid, level_manager->world_root_entity_gid);
                }break;
                case LevelLoadType_World:{
                    entity_manager_dealloc_entity(entity_manager, level_manager->world_root_entity_gid);
                    level_manager->world_root_entity_gid = level_manager_load_level_file(level_manager, ctx->file_path, transient, ctx->transform);
                    if(entity_manager_get_entity(*entity_manager, level_manager->world_root_entity_gid, &world_entity)){
                        level_manager->world_root_entity_file_path = &world_entity->serialisation_file_path;
                    }
                    else{
                        ASSERT(false, "failed to set world root entity file path");
                    }
                }break;
                case LevelLoadType_Ui:{
                    entity_manager_dealloc_entity(entity_manager, level_manager->ui_root_entity_gid);
                    level_manager->ui_root_entity_gid = level_manager_load_level_file(level_manager, ctx->file_path, transient, ctx->transform);
                    if(entity_manager_get_entity(*entity_manager, level_manager->world_root_entity_gid, &world_entity)){
                        level_manager->world_root_entity_file_path = &world_entity->serialisation_file_path;
                    }
                    else{
                        ASSERT(false, "failed to set world root entity file path");
                    }
                }break;
            }
            if(ctx->callback){
                ctx->callback(game_state, ctx->callback_user_data);
            }
        }
        level_manager->deferred_level_load_count = 0;
    }

    for(i32 i = 0; i < entity_manager->entity_length; i++){
        Entity* entity = &entity_manager->entity[i];
        if(!entity->is_physics_body){
            continue;
        }
        fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, entity->global_transform.position);
    }
}

void foo(void* user_data){
    i32 x = *(i32*)user_data;
    os_output_message("loaded level!\n");
}

void game_state_update(GameState* game_state, MemoryArena* persistent, MemoryArena* transient, f32 delta_time, bool in_editor_mode){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;
    FIZX_DrawInfo* fizx_draw_state = &entity_manager->fizx_draw_state;
    TimerManager* timer_manager = &entity_manager->timer_manager;
    FIZX_State* fizx_state = &entity_manager->fizx_state;
    BoardState* board_state = &game_state->board_state;
    GameMouseState* mouse_state = &game_state->mouse_state;
    LevelManager* level_manager = &game_state->level_manager;

    // retrieve necessary data.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(gfx_state);

    { // input_handling();
    
        { // update fizx draw state.

            if(input_is_key_just_pressed(Key_1)){
                // fizx_draw_state->draw_bvh_branches = !fizx_draw_state->draw_bvh_branches;
            }
            if(input_is_key_just_pressed(Key_2)){
                // fizx_draw_state->draw_bvh_leaves = !fizx_draw_state->draw_bvh_leaves;
            }
            if(input_is_key_just_pressed(Key_3)){
                fizx_draw_state->draw_body_shapes = !fizx_draw_state->draw_body_shapes;
            }
            if(input_is_key_just_pressed(Key_4)){
                // fizx_draw_state->draw_collision_info = !fizx_draw_state->draw_collision_info;
            }
            
            if(input_is_key_just_pressed(Key_Escape)){
                board_state->show_pause_menu = !board_state->show_pause_menu;
            }
        }

        if(input_is_key_just_pressed(Key_Tilde)){
            
            String file_path = string_duplicate(*game_state->level_manager.world_root_entity_file_path, transient);
            
            DeferredLevelLoadContext ctx = {
                .transform = TRANSFORM2D_IDENTITY,
                .file_path = file_path,
                .load_type = LevelLoadType_World,
                .callback = load_board_level_callback
            };
            level_manager_defer_load_level(&game_state->level_manager, ctx);
        }
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

            CollisionCallbackContext collision_callback_ctx = {.game_state = game_state};
            if(!in_editor_mode){
                fizx_state_fixed_update(fizx_state, &collision_callback_ctx, FIXED_DELTA_TIME, 32);            
            }

            { // set_entity_positions_to_fizx_positions();
            
                // note that for this loop we don't set the entity component transforms
                // they are set at the end by updating the global transform of the root entity
                // which cascades down to all other entities in the tree. This is done as
                // setting the global transform of an entity is 'order' sensitive; relative to how the tree is structured.
                //
                // Example (transform update order is left to right, i.e, the start to the end of loop):
                //
                //  - this is bad as the child physics component transform is lost (when 'update_components_transform' = true).
                //      new_parent_global -> current_child_local
                //
                //  - this is would 'half' work (when 'update_components_transform' = true) as child physics transform isnt lost but the parent's is.
                //      new_child_global -> current_parent_local
                //
                // the entity allocation system doesnt guarantee that a parent will come after its child in the collection.
                // thus we have to set the transforms of the entities - without updating the components - in order to properly
                // set the new transform data. componets are then updated by updating the transform of the level root entity; 
                // which then cascades down to all other entities in the level.  
            
                for(i32 i = 0; i < entity_manager->entity_length; i++){
                    Entity* entity = &entity_manager->entity[i];
        
                    if(entity->is_physics_body){
                        if(fizx_body_is_active(fizx_state, entity->physics_body_gid)){
                            Transform2D transform;
                            if(fizx_body_get_transform(fizx_state, entity->physics_body_gid, &transform)){
                                
                                transform.scale = entity->global_transform.scale;
                                bool update_components_transform = false;
                                entity_set_global_transform_unsafe_ext(entity_manager, i, transform, update_components_transform);   
                            }
                        }
                    }        
                }
                
                Entity* world_root_entity;
                if(entity_manager_get_entity(*entity_manager, level_manager->world_root_entity_gid, &world_root_entity)){
                    i32 idx = gen_id_get_index(level_manager->world_root_entity_gid);
                    if(idx){
                        entity_set_global_transform_unsafe(entity_manager, idx, world_root_entity->global_transform);
                    }
                }
            }
            
            { // evaluate_board_state();
    
                bool balls_are_moving = false;
                                
                Entity* cue_ball_entity;
                if(entity_manager_get_entity(*entity_manager, board_state->cue_ball_entity_gid, &cue_ball_entity)){
                    Vector2 linear_velocity;
                    if(fizx_body_get_linear_velocity(*fizx_state, cue_ball_entity->physics_body_gid, &linear_velocity)){                
                        if(vector2_len_sqrd(linear_velocity)){
                            balls_are_moving = true;
                        }
                    } 
                }
                
                for(i32 i = 0; i < board_state->number_ball_count; i++){
                    
                    Entity* number_ball_entity;
                    BOUNDS_CHECK(i, board_state->number_ball_length);
                    GenId number_ball_entity_gid = board_state->number_ball[i];
                    
                    if(entity_manager_get_entity(*entity_manager, number_ball_entity_gid, &number_ball_entity)){
                        Vector2 linear_velocity;
                        if(fizx_body_get_linear_velocity(*fizx_state, number_ball_entity->physics_body_gid, &linear_velocity)){
                            if(vector2_len_sqrd(linear_velocity)){
                                balls_are_moving = true;
                                break;
                            }

                        } 
                    }
                }
                
                if(balls_are_moving){
                    switch(board_state->turn){
                        case BoardTurn_Player:{
                            board_state->turn = BoardTurn_Ball;
                            os_output_message("ball turn\n");
                        }break;
                    }
                }else{
                    switch(board_state->turn){
                        case BoardTurn_Ball:{
                            board_state->turn = BoardTurn_TurnBased;
            
                            for(i32 i = 0; i < board_state->turn_based_entity_length; i++){
                                Entity* turn_based_entity;
                                GenId turn_based_entity_gid = board_state->turn_based_entity[i];
                                if(entity_manager_get_entity(*entity_manager, turn_based_entity_gid, &turn_based_entity)){
                                    if(turn_based_entity->turns_until_turn_end_callback == 0){
                                        turn_based_entity->on_turn_end_callback(game_state, gen_id_get_index(turn_based_entity_gid));
                                        turn_based_entity->turns_until_turn_end_callback = turn_based_entity->max_turns_until_turn_end_callback;
                                    }
                                    else{
                                        turn_based_entity->turns_until_turn_end_callback -= 1;
                                    }
                                }
                            }
                            os_output_message("turn based.\n");
                        }break;
                        case BoardTurn_TurnBased:{
                            board_state->turn = BoardTurn_Player;
                            
                            { // increment_turn_text();
                            
                                board_state->turn_number += 1;
                                string_clear(&board_state->turn_text_scratch_space);
                                board_state->turn_text_scratch_space.count = snprintf(
                                    board_state->turn_text_scratch_space.chars, 
                                    board_state->turn_text_scratch_space.length, 
                                    "%i", 
                                    board_state->turn_number
                                );
                                string_clear(&board_state->turn_text);
                                string_push_chars(&board_state->turn_text, "Turn: ", 6);
                                string_push(&board_state->turn_text, board_state->turn_text_scratch_space);
                            }
                            
                            bool eight_ball_present = false;
                            bool eight_ball_pocketed = false;
                            bool active_balls = 0;
                            for(i32 i = 0; i < board_state->number_ball_count; i++){
                                GenId ball_entity_gid = board_state->number_ball[i];
                                Entity* entity;
                                if(entity_manager_get_entity(*entity_manager, ball_entity_gid, &entity)){

                                    if(entity->type_id == EntityTypeId_BallEight){
                                        eight_ball_present = true; 
                                    }                                    
                                    if(!entity_is_active(*entity_manager, ball_entity_gid)){
                                        if(entity->type_id == EntityTypeId_BallEight){
                                            eight_ball_pocketed = true;
                                        }
                                        continue;
                                    }
                                    active_balls++;              
                                }
                            }
                            
                            if(board_state->cue_ball_is_pocketed){
                                board_state->stage = BoardStage_Lose;
                            }
                            else if(eight_ball_pocketed && eight_ball_present){
                                board_state->stage = active_balls > 1
                                ? BoardStage_Lose
                                : BoardStage_Win
                                ;
                            }
                            else{
                                if(active_balls == 0){
                                    board_state->stage = BoardStage_Win; 
                                }
                                else{
                        
                                    // start_displaying_turn_text();
                            
                                    board_state->display_turn_text = true;
                                    BoardStateDisplayTurnHUDTimerContext timer_ctx = {
                                        .board_state = board_state
                                    };
                                    timer_manager_timer_start(
                                        timer_manager, 
                                        1.0f, 
                                        1.0f, 
                                        board_state_display_turn_hud_timeout, 
                                        &timer_ctx, 
                                        sizeof(timer_ctx)
                                    );                        
                                
                                    os_output_message("player turn\n");
                                }
                            }
                        }break;
                    }
                }        
            }
            
            fixed_update_accumulator -= FIXED_DELTA_TIME;
        }
    }
        
    { // entity_updates.
    
        for(i32 i = 0; i < game_state->entity_manager.entity_length; i++){
            if(
                !entity_manager_is_entity_allocated_unsafe(game_state->entity_manager, i) ||
                !entity_is_active_unsafe(game_state->entity_manager, i)
            ){
                continue;
            }
                        
            Entity* entity = &game_state->entity_manager.entity[i];
            
            generic_entity_update(entity, entity_manager);
            
            switch(entity->type_id){
                case EntityTypeId_BallCue:{ball_cue_update(entity, game_state, delta_time);}break;
                case EntityTypeId_WindMillHinge:{windmill_hinge_update(entity, i, game_state, delta_time);}break;
            }
        }
    }
}

void game_state_late_update(GameState* game_state, f32 delta_time, bool in_editor_mode){    
    
    // hoist invariance.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(game_state->entity_manager.gfx_state);
    EntityManager* entity_manager = &game_state->entity_manager;
    ClickableAreaHierarchy* clickable_area_hierarchy = &entity_manager->clickable_area_hierarchy;
    GameMouseState* mouse_state = &game_state->mouse_state;
    FIZX_State* fizx_state = &entity_manager->fizx_state;
    BoardState* board_state = &game_state->board_state;
    
    if(!in_editor_mode){

        if(
            !mouse_state->hovering_ui_element &&
            board_state->stage == BoardStage_Playing
        ){
            Entity* clicked_entity = NULL;
            ClickableArea* area;
            // select_entity();  
            if(input_is_mouse_button_just_pressed(MouseButton_Left)){
                clickable_area_hierarchy_update(
                    clickable_area_hierarchy,
                    mouse_world_position
                );
                if(clickable_area_hierarchy_get_selected_area(clickable_area_hierarchy, &area)){
                    mouse_state->clicked_entity_initial_position = ((Entity*)area->user_data)->global_transform.position;
                }
            }
            
            if(clickable_area_hierarchy_get_selected_area(clickable_area_hierarchy, &area)){
                clicked_entity = (Entity*)area->user_data;
                os_output_message("farts\n");        
            }
            
            if(
                clicked_entity &&
                board_state->turn == BoardTurn_Player
            ){            
                // get directions.
                Vector2 position_diff = vector2_sub(mouse_state->clicked_entity_initial_position, mouse_world_position);
                position_diff = vector2_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
                f32 length = vector2_len(position_diff);
                Vector2 launch_direction = vector2_normalise(position_diff);
                
                // calculate impulse to apply to clicked entity.
                Vector2 impulse_magnitude = vector2_mul_val(position_diff, PLAYER_MOUSE_LAUNCH_FORCE);
    
                // apply impulse when button is released.            
                if(input_is_mouse_button_just_released(MouseButton_Left)){
                    fizx_body_clear_forces_and_velocities(fizx_state, clicked_entity->physics_body_gid);
                    fizx_body_impulse_linear_force(fizx_state, clicked_entity->physics_body_gid, impulse_magnitude);
                }
    
                { // draw_guideline();
                    gfx_draw_line(
                        entity_manager->gfx_state,
                        GFX_COLOUR_WHITE,
                        mouse_state->clicked_entity_initial_position,
                        mouse_state->guideline_end_point, 
                        SPRITE_LAYER_EDITOR_WORLD, 
                        0, 
                        SPRITE_MATERIAL_DEBUG, 
                        0.05f
                    );
        
                    fizx_body_set_active(
                        &entity_manager->fizx_state,
                        mouse_state->guideline_physics_body_gid,
                        true
                    );        
                    fizx_body_set_global_position(
                        &entity_manager->fizx_state, 
                        mouse_state->guideline_physics_body_gid, 
                        mouse_state->clicked_entity_initial_position
                    );
                    fizx_line_set_direction(
                        &entity_manager->fizx_state, 
                        mouse_state->guideline_physics_shape_gid, 
                        launch_direction, 
                        100.0f
                    );       
                }
                
                { // draw_launch_cursour();
                    Vector2 new_position = vector2_sub(mouse_state->clicked_entity_initial_position, position_diff);
                    Circle launch_cursour_shape = {.x = new_position.x, .y = new_position.y, .radius = 0.1f};
                    gfx_draw_wire_circle(
                        entity_manager->gfx_state, 
                        launch_cursour_shape, 
                        GFX_COLOUR_YELLOW, 
                        SPRITE_LAYER_EDITOR_WORLD, 
                        0, 
                        SPRITE_MATERIAL_DEBUG
                    );
                }      
            }
            else{    
                fizx_body_set_active(
                    &game_state->entity_manager.fizx_state,
                    game_state->mouse_state.guideline_physics_body_gid,
                    false
                );  
            }
        
            // deslect_entity();
            // note: this is done after the clcked entity code so that a clicked entity
            // can execute 'input_on_mouse_button_released' code paths. 
            if(input_is_mouse_button_just_released(MouseButton_Left)){
                // clickable_area_hierarchy_update(
                //     clickable_area_hierarchy,
                //     mouse_world_position
                // );
                clickable_area_hierarchy_reset(clickable_area_hierarchy); 
            }
        }
    }
}

#define GFX_CLAY_BG_COLOUR (Clay_Color){.r = 10, .g = 10, .b = 20, .a = 100}

static game_gfx_clay_element_id;

#define GFX_CLAY_BOARD_TURN_TEXT_CONTAINER()                                    \
CLAY(CLAY_SIDI(CLAY_STRING("Board Turn Text"), game_gfx_clay_element_id++), {   \
    .layout = {                                                                 \
        .sizing = {                                                             \
            .width = CLAY_SIZING_PERCENT(0.5f),                                 \
            .height = CLAY_SIZING_PERCENT(0.2f)                                 \
        },                                                                      \
        .layoutDirection = CLAY_TOP_TO_BOTTOM,                                  \
        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }          \
    },                                                                          \
    .backgroundColor = GFX_CLAY_BG_COLOUR                                       \
})

#define GFX_CLAY_PAUSE_MENU_CONTAINER()                                                 \
CLAY(CLAY_SIDI(CLAY_STRING("Pause Menu Container"), game_gfx_clay_element_id++), {      \
    .layout = {                                                                         \
        .sizing = {                                                                     \
            .width = CLAY_SIZING_PERCENT(0.75f),                                        \
            .height = CLAY_SIZING_PERCENT(0.5f)                                         \
        },                                                                              \
        .layoutDirection = CLAY_TOP_TO_BOTTOM,                                          \
        .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                 \
        .childGap = 16                                                                  \
    },                                                                                  \
    .floating = {                                                                       \
        .zIndex = 1,                                                                    \
        .attachPoints = {                                                               \
            .element = CLAY_ATTACH_POINT_CENTER_CENTER,                                 \
            .parent = CLAY_ATTACH_POINT_CENTER_CENTER                                   \
        },                                                                              \
        .attachTo = CLAY_ATTACH_TO_PARENT                                               \
    },                                                                                  \
    .backgroundColor = GFX_CLAY_BG_COLOUR                                               \
})

#define GAME_GFX_CLAY_TEXT_CONFIG   \
(Clay_TextElementConfig){           \
    .fontSize = 1,                  \
    .lineHeight = 24,               \
    .textColor = {                  \
        .r = 255,                   \
        .g = 255,                   \
        .b = 255,                   \
        .a = 255                    \
    }                               \
}

#define GAME_GFX_CLAY_SUB_HEADER_TEXT_CONFIG    \
(Clay_TextElementConfig){                       \
    .fontSize = 2,                              \
    .lineHeight = 24,                           \
    .textColor = {                              \
        .r = 255,                               \
        .g = 255,                               \
        .b = 255,                               \
        .a = 255                                \
    }                                           \
}

#define GAME_GFX_CLAY_HEADER_TEXT_CONFIG    \
(Clay_TextElementConfig){                   \
    .fontSize = 3,                          \
    .lineHeight = 24,                       \
    .textColor = {                          \
        .r = 255,                           \
        .g = 255,                           \
        .b = 255,                           \
        .a = 255                            \
    }                                       \
}

#define GAME_GFX_CLAY_BUTTON_IDLE_COLOUR (Clay_Color){100.0f,100.0f,100.0f,225.0f}
#define GAME_GFX_CLAY_BUTTON_HOVER_COLOUR (Clay_Color){155.0f,155.0f,155.0f,255.0f}
#define GAME_GFX_CLAY_BUTTON(clay_string_name, horizontal_percent_size, vertical_percent_size, on_hover_callback, on_hover_user_data)   \
CLAY(                                                                                                                                   \
    CLAY_SIDI(clay_string_name, game_gfx_clay_element_id++),                                                                          \
    {                                                                                                                                   \
        .backgroundColor = Clay_Hovered() ? GAME_GFX_CLAY_BUTTON_HOVER_COLOUR : GAME_GFX_CLAY_BUTTON_IDLE_COLOUR,                       \
        .layout = {                                                                                                                     \
            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                                                             \
            .sizing = {                                                                                                                 \
                CLAY_SIZING_PERCENT(horizontal_percent_size),                                                                           \
                CLAY_SIZING_PERCENT(vertical_percent_size)                                                                              \
            },                                                                                                                          \
        }                                                                                                                               \
    }                                                                                                                                   \
) {                                                                                                                                     \
    Clay_OnHover(on_hover_callback, on_hover_user_data);                                                                                \
    CLAY_TEXT(clay_string_name, GAME_GFX_CLAY_TEXT_CONFIG);                                                                           \
}

/*
    `typed example`
    GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
        Clay_String _clay_string_name, 
        f32 _horizontal_percent_size, 
        f32 _vertical_percent, 
        MemoryArena* _transient_arena,
        LevelManager* _level_manager,
        char* _file_path_chars,
        i32 _file_path_length,
        DeferredLevelLoadCallback _level_load_callback,
        char* _level_load_callback_user_data,
    );
    
    `parameters`
    `_clay_string_name`: the display label of the button.
    `_horizontal_percent_size`: the horizontal percentage size of the button within the container widget.
    `_vertical_percent_size`: the horizontal percentage size of the button within the container widget.
    `_transient_arena`: the transient arena to allocate the function capture onto.
    `_level_manager`: the level manager to push the generated defer load level onto.                        
    `_file_path_chars`: the file path to the level asset to load
    `_file_path_length`: the length of the file path to the asset to load.
    `_level_load_callback`: the callback for when the level is loaded. 
    `_level_load_callback_user_data`: the user_data to supply to the callback.
*/
#define GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(                                          \
    _clay_string_name,                                                                  \
    _horizontal_percent_size,                                                           \
    _vertical_percent_size,                                                             \
    _transient_arena,                                                                   \
    _level_manager,                                                                     \
    _file_path_chars,                                                                   \
    _file_path_length,                                                                  \
    _level_load_callback,                                                               \
    _level_load_callback_user_data                                                      \
) do {                                                                                  \
                                                                                        \
    LoadLevelButtonOnHoverContext __ggclwlb_ctx = {0};                                  \
    string_init(&__ggclwlb_ctx.file_path, _transient_arena, _file_path_length);         \
    string_push_chars(&__ggclwlb_ctx.file_path, _file_path_chars, _file_path_length);   \
    __ggclwlb_ctx.load_type = LevelLoadType_World;                                      \
    __ggclwlb_ctx.callback = _level_load_callback;                                      \
    __ggclwlb_ctx.callback_user_data = (char*)_level_load_callback_user_data;           \
    __ggclwlb_ctx.level_manager = _level_manager;                                       \
                                                                                        \
    void* GAME__ggclwlb_ptr;                                                            \
    MEMORY_ARENA_PUSH_STRUCT(_transient_arena, __ggclwlb_ctx, GAME__ggclwlb_ptr);       \
    GAME_GFX_CLAY_BUTTON(                                                               \
        _clay_string_name,                                                              \
        (f32)_horizontal_percent_size,                                                  \
        (f32)_vertical_percent_size,                                                    \
        load_world_level_button_on_hover_callback,                                      \
        GAME__ggclwlb_ptr                                                               \
    );                                                                                  \
}while(0)

#define GAME_GFX_CLAY_LEVEL_SELECT_BUTTON_CONTAINER()                                       \
                                                                                            \
CLAY(CLAY_SIDI(CLAY_STRING("Level Select Button Container"), game_gfx_clay_element_id++), { \
    .backgroundColor = (Clay_Color){0.0f,0.0f,0.0f,200.0f},                                 \
    .layout = {                                                                             \
        .sizing = {                                                                         \
            .width = CLAY_SIZING_PERCENT(0.95f),                                            \
            .height = CLAY_SIZING_PERCENT(0.25f)                                            \
        },                                                                                  \
        .layoutDirection = CLAY_LEFT_TO_RIGHT,                                              \
        .padding = CLAY_PADDING_ALL(16),                                                    \
        .childGap = 16,                                                                     \
    },                                                                                      \
})


///
/// @ Button On Hover Callback
///


void load_world_level_button_on_hover_callback(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    LoadLevelButtonOnHoverContext* ctx = (LoadLevelButtonOnHoverContext*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        DeferredLevelLoadContext level_load_ctx = {
            .transform = TRANSFORM2D_IDENTITY,
            .file_path = ctx->file_path,
            .load_type = ctx->load_type,
            .callback = ctx->callback,
            .callback_user_data = ctx->callback_user_data
        };
        level_manager_defer_load_level(ctx->level_manager, level_load_ctx);
    }    
}


void game_state_draw(GameState* game_state, MemoryArena* transient, f32 delta_time, bool in_editor_mode){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;
    FIZX_State* fizx_state = &entity_manager->fizx_state;
    FIZX_DrawInfo* fizx_draw_state = &entity_manager->fizx_draw_state;
    BoardState* board_state = &game_state->board_state;
    GameMouseState* mouse_state = &game_state->mouse_state;
    LevelManager* level_manager = &game_state->level_manager;

    /*
        clear from previous run.

        NOTE:
        you need to reset the global element_id as Clay SIDI on hover callbacks requires that
        elements retain their ID numbers, without this, its impossible for clay to properly handle OnHover callbacks.
    */
    game_gfx_clay_element_id = 0;

    mouse_state->hovering_ui_element = false;

    fizx_state_draw(*fizx_state, gfx_state, *fizx_draw_state, delta_time);

    Vector2I mouse_backbuffer_position;
    if(in_editor_mode){
        mouse_backbuffer_position.x = I32_MAX;
        mouse_backbuffer_position.y = I32_MAX;
    }
    else{
        input_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);
    }
    

    { // ui();

        Clay_SetCurrentContext(gfx_state->clay_game_ui_ctx);
        gfx_clay_begin_layout(
            gfx_state, (Vector2I){.x = gfx_state->window_ctx->width, .y = gfx_state->window_ctx->height}, 
            mouse_backbuffer_position, delta_time, input_is_mouse_button_pressed(MouseButton_Left)
        );

        switch(game_state->stage){

            case GameStage_Menu:{
                CLAY(CLAY_SIDI(CLAY_STRING("Left Panel"), game_gfx_clay_element_id++), {
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_PERCENT(0.1f),
                            .height = CLAY_SIZING_GROW(0)                                 
                        },                                                                
                    },                                                                    
                });
                
                CLAY(CLAY_SIDI(CLAY_STRING("Center Panel"), game_gfx_clay_element_id++), {
                    .backgroundColor = GAME_GFX_CLAY_BUTTON_IDLE_COLOUR,
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_PERCENT(0.8f),
                            .height = CLAY_SIZING_GROW(0)
                        },                           
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                        .padding = CLAY_PADDING_ALL(16) 
                    },
                }){

                    CLAY(CLAY_SIDI(CLAY_STRING("Level Select Widget Container"), game_gfx_clay_element_id++), {
                        .backgroundColor =  (Clay_Color){0.0f,0.0f,0.0f,200.0f},
                        .layout = {                                                           
                            .sizing = {                                                       
                                .width = CLAY_SIZING_GROW(0),                                 
                                .height = CLAY_SIZING_GROW(0)
                            },                                                                
                            .layoutDirection = CLAY_TOP_TO_BOTTOM,
                            .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                            .childGap = 16
                        },
                    }){
                    
                        CLAY_TEXT(CLAY_STRING("Level Select"), GAME_GFX_CLAY_HEADER_TEXT_CONFIG);   
                        
                        i32 file_count;
                        os_get_all_file_paths(STRING_FROM_LITERAL("../assets/levels/*", transient), NULL, &file_count);
                        
                        String* file_paths = NULL;
                        i32 file_paths_length = 0;
                        MEMORY_ARENA_ALLOC_ARRAY(transient, file_paths, &file_paths_length, file_count);
                        for(i32 i = 0; i < file_paths_length; i++){
                            string_init(&file_paths[i], transient, os_get_max_file_path_length()); 
                        }
                        
                        os_get_all_file_paths(STRING_FROM_LITERAL("../assets/levels/*", transient), file_paths, &file_count);
                        
                        f32 load_world_button_horizontal_percent = 0.25f;
                        f32 load_world_button_vertical_percent = 1.0f;

                        for(i32 i = 0; i < file_count; i += 4){
                            GAME_GFX_CLAY_LEVEL_SELECT_BUTTON_CONTAINER(){
                                
                                i32 idx = i;
                                for(i32 j = 0; j < 4; j++){
                                    i32 idx = i + j;
                                    if(idx < file_count){
                                        String partial_file_path = file_paths[idx];
                                        String full_file_path;
                                        string_init(&full_file_path, transient, os_get_max_file_path_length());
                                        STRING_PUSH_LITERAL(&full_file_path, LEVEL_ASSET_DIRECTORY_PATH);
                                        string_push(&full_file_path, partial_file_path);
                                        
                                        // remove the file extension from the partial_file_path, making it formatted
                                        // as the label for the load world level button.
                                        partial_file_path.count -= 5;
                                        
                                        GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                            gfx_string_to_clay_string(partial_file_path),
                                            load_world_button_horizontal_percent,
                                            load_world_button_vertical_percent,
                                            transient,
                                            level_manager,
                                            full_file_path.chars,
                                            full_file_path.count,
                                            load_board_level_callback,
                                            NULL
                                        );
                                    }
                                }
                            }
                        }
                    }
                }
                
                CLAY(CLAY_SIDI(CLAY_STRING("Right Panel"), game_gfx_clay_element_id++), {
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_PERCENT(0.1f),                                 
                            .height = CLAY_SIZING_GROW(0)                                 
                        },                                                                
                    },                      
                });          
            }break;
            case GameStage_Board:{
                                
                CLAY(CLAY_SIDI(CLAY_STRING("Padding Container"), game_gfx_clay_element_id++), {
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_GROW(0),
                            .height = CLAY_SIZING_GROW(0)                                 
                        },                                                                
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,                            
                    },                                                                    
                });
                
                CLAY(CLAY_SIDI(CLAY_STRING("Center Panel"), game_gfx_clay_element_id++), {
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_GROW(0),                                 
                            .height = CLAY_SIZING_GROW(0)
                        },                                                                
                        .layoutDirection = CLAY_TOP_TO_BOTTOM,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },
                    },
                }){
                    switch(board_state->stage){
                        default: {ASSERT(false, "undefined state");} break;
                        case BoardStage_Playing:{
                            if(board_state->show_pause_menu){
                                GFX_CLAY_PAUSE_MENU_CONTAINER(){
                                    
                                    CLAY_TEXT(CLAY_STRING("Pause Menu!"), GAME_GFX_CLAY_SUB_HEADER_TEXT_CONFIG);
                                    
                                    LoadMenuLevelCallbackContext* ctx;
                                    String menu_level_file_path = STRING_FROM_LITERAL("../assets/levels/menu_level.scsv", transient);
                                    f32 horizontal_size = 0.75f;
                                    f32 vertical_size = 0.25f;
                                                                        
                                    MEMORY_ARENA_ALLOC_STRUCT(transient, ctx);
                                    ctx->stage_to_load_to = MenuStage_LevelSelector;
                                    
                                    GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                        CLAY_STRING("Restart"),
                                        horizontal_size,
                                        vertical_size,
                                        transient,
                                        &game_state->level_manager,
                                        game_state->level_manager.world_root_entity_file_path->chars,
                                        game_state->level_manager.world_root_entity_file_path->count,
                                        load_board_level_callback,
                                        NULL
                                    );
                                    
                                    GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                        CLAY_STRING("Return to Level Select"),
                                        horizontal_size,
                                        vertical_size,
                                        transient,
                                        &game_state->level_manager,
                                        menu_level_file_path.chars,
                                        menu_level_file_path.count,
                                        load_menu_level_callback,
                                        ctx
                                    );
                                }
                            }

                            if(board_state->display_turn_text){
                                GFX_CLAY_BOARD_TURN_TEXT_CONTAINER(){
                                    Clay_String clay_string = {
                                        .length = board_state->turn_text.count,
                                        .chars = (const char*)board_state->turn_text.chars
                                    };
                                    CLAY_TEXT(clay_string, GAME_GFX_CLAY_TEXT_CONFIG);
                                }
                            }

                        }break;
                        case BoardStage_Lose:{
                            GFX_CLAY_BOARD_TURN_TEXT_CONTAINER(){
                                CLAY_TEXT(CLAY_STRING("You Lose!"), GAME_GFX_CLAY_TEXT_CONFIG);                
                                f32 horizontal_size = 0.75f;
                                f32 vertical_size = 0.25f;
                                GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                    CLAY_STRING("Retry"),
                                    horizontal_size,
                                    vertical_size,
                                    transient,
                                    &game_state->level_manager,
                                    game_state->level_manager.world_root_entity_file_path->chars,
                                    game_state->level_manager.world_root_entity_file_path->count,
                                    load_board_level_callback,
                                    NULL
                                );
                                
                                LoadMenuLevelCallbackContext* ctx;
                                String menu_level_file_path = STRING_FROM_LITERAL("../assets/levels/menu_level.scsv", transient);
                                MEMORY_ARENA_ALLOC_STRUCT(transient, ctx);
                                ctx->stage_to_load_to = MenuStage_LevelSelector;
                                GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                    CLAY_STRING("Return to Level Select"),
                                    horizontal_size,
                                    vertical_size,
                                    transient,
                                    &game_state->level_manager,
                                    menu_level_file_path.chars,
                                    menu_level_file_path.count,
                                    load_menu_level_callback,
                                    ctx
                                );
                            }
                        }break;
                        
                        case BoardStage_Win:{
                            GFX_CLAY_BOARD_TURN_TEXT_CONTAINER(){
                                CLAY_TEXT(CLAY_STRING("You Win!"), GAME_GFX_CLAY_TEXT_CONFIG);
                                f32 horizontal_size = 0.75f;
                                f32 vertical_size = 0.25f;
                                GAME_GFX_CLAY_LOAD_WORLD_LEVEL_BUTTON(
                                    CLAY_STRING("Play Again"),
                                    horizontal_size,
                                    vertical_size,
                                    transient,
                                    &game_state->level_manager,
                                    game_state->level_manager.world_root_entity_file_path->chars,
                                    game_state->level_manager.world_root_entity_file_path->count,
                                    load_board_level_callback,
                                    NULL
                                );
                            }
                        }break;
                    }
                }
                
                CLAY(CLAY_SIDI(CLAY_STRING("Right Panel"), game_gfx_clay_element_id++), {
                    .layout = {                                                           
                        .sizing = {                                                       
                            .width = CLAY_SIZING_GROW(0),                                 
                            .height = CLAY_SIZING_GROW(0)                                 
                        },                                                                
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER },                            
                    },                      
                }){
                    CLAY(CLAY_SIDI(CLAY_STRING("dummy"), game_gfx_clay_element_id++), {
                        .layout = {                                                           
                            .sizing = {                                                       
                                .width = CLAY_SIZING_PERCENT(0.5f),                                 
                                .height = CLAY_SIZING_GROW(0)                                 
                            },                                                                
                        }                      
                    });
                    GFX_CLAY_BOARD_TURN_TEXT_CONTAINER(){
                        if(Clay_Hovered()){
                            mouse_state->hovering_ui_element = true;
                        }
                    
                        switch(board_state->turn){
                            case BoardTurn_Player:{
                                CLAY_TEXT(CLAY_STRING("Player Turn"), GAME_GFX_CLAY_TEXT_CONFIG);
                            }break;
                            case BoardTurn_Ball:{
                                CLAY_TEXT(CLAY_STRING("Ball Turn"), GAME_GFX_CLAY_TEXT_CONFIG);
                            }break;
                            case BoardTurn_TurnBased:{
                                CLAY_TEXT(CLAY_STRING("Turn Based Turn"), GAME_GFX_CLAY_TEXT_CONFIG);
                            }break;
                        }
                    }
                }
            
#if 0
                switch(board_state->turn){
                    case BoardTurn_Player:{
                        CLAY_TEXT(CLAY_STRING("PLAYER TURN"), GAME_GFX_CLAY_TEXT_CONFIG);
                    }break;
                    case BoardTurn_Ball:{
                        CLAY_TEXT(CLAY_STRING("BALL TURN"), GAME_GFX_CLAY_TEXT_CONFIG);
                    }break;
                    case BoardTurn_TurnBased:{
                        CLAY_TEXT(CLAY_STRING("TURN BASED TURN"), GAME_GFX_CLAY_TEXT_CONFIG);
                    }break;
                }
#endif
            }break;        
        }


    }

    
    gfx_clay_end_layout(gfx_state, delta_time, SPRITE_LAYER_GAME_UI, VIRTUAL_TEXTURE_ID_FONT, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
}