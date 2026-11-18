

///
/// todo:
/// turn entities active and inactive.
///


///
/// @ Forward Declarations.
///


typedef struct GameState GameState;
typedef struct EntityManager EntityManager;
typedef struct Entity Entity;
typedef struct LevelManager LevelManager;
typedef struct DeferredLevelLoadContext DeferredLevelLoadContext;
typedef struct Particle Particle;
typedef struct ParticleEmitter ParticleEmitter;
typedef struct ParticleSystem ParticleSystem;
typedef struct ParticleEmitterConfig ParticleEmitterConfig;
typedef enum EntityTypeId EntityTypeId;

GenId level_manager_load_asset_file(LevelManager* level_manager, String file_path, MemoryArena* transient, Transform2D transform);
void level_manager_defer_load_level(LevelManager* level_manager, DeferredLevelLoadContext ctx);
bool entity_manager_get_entity(EntityManager manager, GenId entity_gid, Entity** out_entity);
bool entity_is_active_unsafe(EntityManager entity_manager, i32 entity_idx);
bool entity_is_active(EntityManager entity_manager, GenId entity_gid);
GenId entity_spawn(EntityManager* entity_manager, EntityTypeId type_id, Transform2D transform, GenId parent_gid, Entity** out_entity);
void entity_set_active_unsafe(EntityManager* entity_manager, i32 entity_idx,  bool is_active);
bool entity_set_active(EntityManager* entity_manager, GenId entity_gid, bool is_active);


///
/// @ Physics layers.
///


#define PHYSICS_LAYER_NONE                  0
#define PHYSICS_LAYER_ALL                   I32_MAX
#define PHYSICS_LAYER_GUIDELINE             (1 << 1)
#define PHYSICS_LAYER_ENVIRONMENT_TOP       (1 << 2)
#define PHYSICS_LAYER_ENVIRONMENT_MID       (1 << 3)
#define PHYSICS_LAYER_ENVIRONMENT_BOT       (1 << 4)
#define PHYSICS_LAYER_PLAYER                (1 << 5)


///
/// @ Virtual Texture ID.
///


#define VIRTUAL_TEXTURE_ID_FONT 1
#define VIRTUAL_TEXTURE_ID_GAME_BOARD 2


///
/// @ TextureView defines.
///


#define TEXTURE_VIEW_WALL           (GFX_TextureView){.top_left = {0, 0}, .bot_right = {128, 128}}
#define TEXTURE_VIEW_PLAYER         (GFX_TextureView){.top_left = {0, 0}, .bot_right = {128, 128}}
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

#define SPRITE_LAYER_WORLD_ENVIRONMENT_BOT      8
#define SPRITE_LAYER_WORLD_ENVIRONMENT_BOT_FX   7
#define SPRITE_LAYER_WORLD_ENVIRONMENT_MID      6
#define SPRITE_LAYER_WORLD_ENVIRONMENT_MID_FX   5
#define SPRITE_LAYER_WORLD_ENVIRONMENT_TOP      4
#define SPRITE_LAYER_WORLD_ENVIRONMENT_TOP_FX   3

#define SPRITE_LAYER_EDITOR_WORLD 2

#define SPRITE_LAYER_GAME_UI 1
#define SPRITE_LAYER_EDITOR_UI 0

#define SPRITE_MATERIAL_DEBUG 1
#define SPRITE_MATERIAL_IMAGE 2
#define SPRITE_MATERIAL_TEXT 3


///
/// @ Entity defines.
///


#define ENTITY_VISIBILITY_LAYER_NONE 0
#define ENTITY_VISIBILITY_LAYER_WORLD (1 << 1)
#define ENTITY_VISIBILITY_LAYER_EDITOR (1 << 2)



///
/// @ Player defines
///


#define PLAYER_PHYSICS_LAYER        (PHYSICS_LAYER_PLAYER)
#define PLAYER_PHYSICS_MASK         (PHYSICS_LAYER_ALL)
#define PLAYER_PHYSICS_BODY_CONFIG  (FIZX_BodyConfig){  \
    .layer = PLAYER_PHYSICS_LAYER,                      \
    .mask = PLAYER_PHYSICS_MASK,                        \
    .linear_drag = 22.4f,                               \
    .angular_drag = 1.0f                                \
}


///
/// @ Wall defines
///


#define WALL_PHYSICS_BODY_LAYER (PHYSICS_LAYER_ENVIRONMENT_MID | PHYSICS_LAYER_GUIDELINE)


///
/// @ Types
///


struct Particle{
    // the emitter this particle is attached to.
    ParticleEmitter* emitter;
    Transform2D transform;
    Vector2 linear_velocity;
    GFX_Colour colour;
    GFX_Colour fade_colour;
    f32 lifetime;
    f32 age;
    // note: this is in radians.
    f32 angular_velocity;
    u16 layer;
    u16 depth;
    i32 sibling_idx; // can be NIL.
    bool is_active;
    bool fade_over_lifetime;
};

struct ParticleEmitterConfig{
    GFX_Colour colour;
    GFX_Colour gradient_end_colour;
    GFX_Colour fade_colour;
    Vector2 min_scale;
    Vector2 max_scale;
    Vector2 direction;
    // `remarks`
    // this value is in anglular degrees.
    f32 direction_min_range;
    // `remarks`
    // this value is in anglular degrees.
    f32 direction_max_range;
    f32 min_linear_velocity;
    f32 max_linear_velocity;
    f32 min_lifetime;
    f32 max_lifetime;
    f32 min_angular_velocity;
    f32 max_angular_velocity;
    f32 radial_spawn_inner_ring;
    f32 radial_spawn_outer_ring;
    u16 layer;
    u16 depth;
    bool uniform_scale;
    bool fade_over_lifetime;
    bool use_gradient;
    bool radial_spawn;
    bool use_radial_spawn_direction;
};

struct ParticleSystem{
    GenIdAllocator gen_id_allocator;
    Particle* particle;
    i32 particle_length;
};

struct ParticleEmitter{
    ParticleEmitterConfig config;
    // the first allocated particle in the particle chain.
    GenId first_particle_gid;
    // the next particle to use when emitting.
    GenId next_particle_gid;
    ParticleSystem* system;
    bool is_init;
};


typedef void (*TimerTimeoutCallback)(void* user_data);
typedef void (*TimerTickCallback)(void* user_data, f32 current_tick, f32 delta_tick);

typedef struct{
    
    TimerTickCallback tick_callback;
    // this is deep copied into the internal data storage.
    void* tick_data;
    size_t tick_data_size;
    
    TimerTimeoutCallback timeout_callback;
    // this is deep copied into the internal data storage.
    void* timeout_data; 
    size_t timeout_data_size;
    
    f32 time_scale;
    f32 start_time;
    f32 end_time; 
    
} TimerContext;

typedef GenId TimerHandle;

typedef struct{
    GenIdAllocator gen_id_allocator;
    f32* time_scale;
    f32* previous_time;
    f32* current_time;
    f32* start_time;
    f32* end_time;
    // `remarks`
    // 1.0f = true, 0.0f = false.
    f32* is_ticking;
    // `remarks`
    // 1.0f = true, 0.0f = false.
    f32* has_started;
    f32* delta_time_scratch_space;

    TimerTickCallback* tick_callback;
    char* tick_data;
    i32 tick_data_length;
    i32 tick_data_element_size;
    
    TimerTimeoutCallback* timeout_callback;
    char* timeout_data;
    i32 timeout_data_length;
    i32 timeout_data_element_size;
    
    // the length of all backing arrays.
    i32 length;
    
    bool is_init;
} TimerManager;

typedef struct{
    Vector2 start_position;
    // the distance to the start position from the mouse world position.
    Vector2 mouse_to_start_dist;    
    // the amount of shake to apply to the direction.
    f32 shake_magnitude;
    // the amount of force to apply to the ball when launching in the desired direction.
    f32 impulse_magnitude;
} GuidelineState;

typedef struct{
    Matrix4x4 world_camera_matrix;
    Matrix4x4 ui_camera_matrix;
    f32 time;
    f32 world_camera_far_z;
    f32 world_camera_near_z;
    f32 padding_0;
} Ubo;

enum EntityTypeId{
    EntityTypeId_None                   = 0,
    EntityTypeId_LevelRoot              = 1,
    EntityTypeId_Wall                   = 2,
    EntityTypeId_Player                 = 3,
    // EntityTypeId_BallOne                = 4,
    // EntityTypeId_BallTwo                = 5,
    // EntityTypeId_BallThree              = 6,
    // EntityTypeId_BallFour               = 7,
    // EntityTypeId_BallFive               = 8,
    // EntityTypeId_BallSix                = 9,
    // EntityTypeId_BallSeven              = 10,
    // EntityTypeId_BallEight              = 11,
    // EntityTypeId_BallNine               = 12,
    // EntityTypeId_BallZero               = 13,
    // EntityTypeId_Pocket                 = 14,
    // EntityTypeId_BoostPad               = 15,
    // EntityTypeId_Wildwall               = 16,
    // EntityTypeId_WildwallHinge          = 17,
    // EntityTypeId_WildwallSegment        = 18,
    // EntityTypeId_Thwamper               = 19,
    // EntityTypeId_BoardTurnText          = 20,
    EntityTypeId_WorldCamera            = 21,
    EntityTypeId_UiCamera               = 22,
    EntityTypeId_Empty                  = 23,
    // EntityTypeId_ValidPlacementArea     = 24,
    // EntityTypeId_InvalidPlacementArea   = 25,
    // EntityTypeId_PocketInteriorWall     = 26
};

typedef struct{
    i32 clicked_entity_idx;
    bool hovering_ui_element;
    Vector2 clicked_entity_initial_position;
} GameMouseState;

typedef void (*TurnBasedEntityOnTurnEndCallback)(EntityManager* entity_manager, i32 entity_idx);

struct Entity{

    Transform2D global_transform;
    Transform2D local_transform;
    Transform2D sprite_local_transform;
    i32 physics_body_bidirectional_angular_constrain_idx;
    
    ParticleEmitter particle_emitter;
    
    // this is only applicable to the root entity that was loaded from a file.
    String serialisation_file_path;
    
    // specifically for the editor (for now...)
    i32 visibility_layer;
    
    String name;
    EntityTypeId type_id;
    GenId sprite_gid;
    GenId physics_body_gid;
    GenId game_clickable_area_gid;
    
    // the behvaioural state of the entity; implementation depends on entity type.
    i32 state;
    // the tag of the entity, for additional specification needs.
    i32 tag;
    
    i32 max_turns_until_turn_end_callback;
    i32 turns_until_turn_end_callback;
    i32 overlaps_pocket_interior_wall_count; // the amount of interior walls the entity is overlapping. 
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
    bool serialisation_disabled;
    bool is_particle_emitter;
    bool turn_based_logic_in_progress;

    void (*set_state_callback)(EntityManager* entity_manager, Entity* entity, i32 entity_idx, i32 state);
};

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
    i32 state;
    i32 tag;
} DeserialisedEntity;

struct EntityManager{
    
    GenIdAllocator gen_id_allocator;
    
    ParticleSystem particle_system;
    
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
    
    i32 visibility_layer;
    
    bool is_init;
};

typedef void (*DeferredLevelLoadCallback)(GameState* game_state, void* user_data);

struct DeferredLevelLoadContext{
    Transform2D transform;
    String file_path;
    DeferredLevelLoadCallback callback;
    void* callback_user_data;
};

struct LevelManager{
    EntityManager* entity_manager;

    String* world_root_entity_file_path;
    GenId world_root_entity_gid;
        
    // the queue of levels to load at an appropriate time (deffered for later).
    DeferredLevelLoadContext* deferred_level_load;
    i32 deferred_level_load_length;
    i32 deferred_level_load_count;    
    bool is_init;
};

typedef enum{
    // MenuStage_MainMenu,
    MenuStage_LevelSelector
} MenuStage;

typedef struct{
    MenuStage stage;
} MenuState;

typedef enum GameStage{
    GameStage_Menu,
    GameStage_Gameplay
} GameStage;

struct GameState{
    GameMouseState mouse_state;
    EntityManager entity_manager;
    LevelManager level_manager;
    MenuState menu_state;
    f32 time_scale;
    bool is_init;
    bool show_pause_menu;
    GameStage stage;
};

typedef struct {
    LevelManager* level_manager;
    DeferredLevelLoadContext deferred_level_load_ctx;
    String file_path;
    DeferredLevelLoadCallback callback;
    char* callback_user_data;
    i32 callback_user_data_length;
} LoadLevelButtonOnHoverContext;

typedef struct{
    GameState* game_state;
} CollisionCallbackContext;


///
/// @ defines
///


#define ASSET_DIRECTORY_PATH "../assets/"
#define ASSET_DIRECTORY_PATH_LENGTH 10
#define LEVEL_ASSET_DIRECTORY_PATH "../assets/levels/"
#define LEVEL_ASSET_DIRECTORY_PATH_LENGTH 17
#define ENTITY_ASSET_DIRECTORY_PATH "../assets/entities/"
#define ENTITY_ASSET_DIRECTORY_PATH_LENGTH 19

#define LEVEL_FILE_NATIVE_ENTITY_TOKEN 'N'
#define LEVEL_FILE_ASSET_ENTITY_TOKEN 'A'

#define ENTITY_NAME_LENGTH 32

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
/// globals.
///

static f32 fixed_update_accumulator = 0.0f;


///
/// @ ParticleSystem procedures.
///


void particle_system_init(ParticleSystem* system, MemoryArena* arena, i32 particle_amount){
    gen_id_allocator_init(&system->gen_id_allocator, arena, particle_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, system->particle, &system->particle_length, particle_amount);
}

void particle_system_update_and_draw(ParticleSystem* system, GFX_State* gfx_state, f32 delta_time){    
    for(i32 i = 0; i < system->particle_length; i++){
        Particle* particle = &system->particle[i];
        
        BOUNDS_CHECK(i, system->gen_id_allocator.length); 
        if(
            !system->gen_id_allocator.allocated[i] ||
            !particle->is_active
        ){
            continue;
        }

        // lifetime handling.
        particle->age = MIN(particle->age + delta_time, particle->lifetime);
        f32 age_ratio = particle->age / particle->lifetime;
        
        // transform handling.
        particle->transform.position = vector2_add(
            particle->transform.position,
            vector2_mul_val(particle->linear_velocity, delta_time)
        );
        
        particle->transform = transform2d_rotate(particle->transform, particle->angular_velocity * delta_time);
        
        // colour handling.
        GFX_Colour colour = particle->colour;
        if(particle->fade_over_lifetime){
            colour.r = particle->colour.r - ABS(particle->fade_colour.r - particle->colour.r) * age_ratio;
            colour.g = particle->colour.g - ABS(particle->fade_colour.g - particle->colour.g) * age_ratio;
            colour.b = particle->colour.b - ABS(particle->fade_colour.b - particle->colour.b) * age_ratio;
            colour.a = particle->colour.a - ABS(particle->fade_colour.a - particle->colour.a) * age_ratio;
        }
        else{
            colour = particle->colour;
        }
            
        // sprite drawing.
        GenId sprite_gid = gfx_one_frame_sprite_alloc(gfx_state);
        gfx_sprite_set_active(          gfx_state, sprite_gid);
        gfx_sprite_set_transform(       gfx_state, sprite_gid, particle->transform);
        gfx_sprite_set_material(        gfx_state, sprite_gid, SPRITE_MATERIAL_DEBUG);
        gfx_sprite_set_origin(          gfx_state, sprite_gid, GFX_SpriteOrigin_Center);
        gfx_sprite_set_colour(          gfx_state, sprite_gid, colour);
        gfx_sprite_set_colour_blend(    gfx_state, sprite_gid, GFX_ColourBlend_Override);
        gfx_sprite_set_layer(           gfx_state, sprite_gid, particle->layer);
        gfx_sprite_set_depth(           gfx_state, sprite_gid, particle->depth);

        // prepare for next step.
        if(particle->age >= particle->lifetime){
            particle->age = 0.0f;
            particle->is_active = false;
        }
    }
}

ParticleEmitter particle_system_create_emitter(ParticleSystem* system, ParticleEmitterConfig config, i32 max_particles){
    
    ParticleEmitter emitter = {0};
    emitter.config = config;
    
    i32 prev_particle_idx = 0;
    
    for(i32 i = 0; i < max_particles; i++){
        GenId particle_gid = gen_id_allocator_alloc(&system->gen_id_allocator);
        if(!particle_gid){
            break;
        }
        
        i32 curr_particle_idx = gen_id_get_index(particle_gid);
        
        BOUNDS_CHECK(curr_particle_idx, system->particle_length);
        Particle* curr_particle = &system->particle[curr_particle_idx];
        curr_particle->transform = TRANSFORM2D_IDENTITY;
        
        if(prev_particle_idx){
            
            BOUNDS_CHECK(curr_particle_idx, system->particle_length);
            Particle* prev_particle = &system->particle[prev_particle_idx];
            prev_particle->sibling_idx = curr_particle_idx;

            // if we have reached the end of the loop.
            if(i == (max_particles - 1)){
                // complete the circular loop.
                i32 first_particle_idx = gen_id_get_index(emitter.first_particle_gid);
                curr_particle->sibling_idx = first_particle_idx;
            }
        }
        else{
            // assign the first particle that was allocated.
            emitter.first_particle_gid = particle_gid;
            emitter.next_particle_gid = particle_gid;
        }
        
        prev_particle_idx = curr_particle_idx;
    }
    
    if(!emitter.first_particle_gid){
        return (ParticleEmitter){0};
    }
    
    emitter.system = system;
    emitter.is_init = true;
    return emitter;
}

bool particle_emitter_dealloc(ParticleEmitter* emitter){
    ASSERT(emitter->is_init, "emitter already deallocated");
    ASSERT(emitter->system, "emitter doesnt come from a particle system.");
    i32 first_idx = gen_id_allocator_is_gen_id_valid(&emitter->system->gen_id_allocator, emitter->first_particle_gid);
    if(!first_idx){
        return false;
    }
    
    i32 next_idx = first_idx;
    while(next_idx){
        gen_id_allocator_dealloc_unsafe(&emitter->system->gen_id_allocator, next_idx);
        BOUNDS_CHECK(next_idx, emitter->system->particle_length);
        Particle* particle = &emitter->system->particle[next_idx];
        next_idx = particle->sibling_idx;
        if(next_idx==first_idx){
            break;
        }
    }
    *emitter = (ParticleEmitter){0};    
    return true;
}

void particle_emitter_emit(ParticleEmitter* emitter, Vector2 position, i32 particle_amount){
    
    ASSERT(emitter->system, "particle emitter doesnt have a particle system.");
    i32 first_idx = gen_id_allocator_is_gen_id_valid(&emitter->system->gen_id_allocator, emitter->next_particle_gid);
    
    ASSERT(first_idx > 0, "attempted to emit particles from a stale emitter.");
    i32 next_idx = first_idx;
        
    i32 counter = 0;
    while(next_idx){
        
        BOUNDS_CHECK(next_idx, emitter->system->particle_length);
        Particle* particle = &emitter->system->particle[next_idx];
        
        // update particle with new emission values.
        particle->is_active             = true;
        
        // lifetime.
        particle->lifetime = f32_rand_range(
            emitter->config.min_lifetime,
            emitter->config.max_lifetime
        );
        
        // scale
        if(!emitter->config.uniform_scale){
            particle->transform.scale.x = f32_rand_range(
                emitter->config.min_scale.x,
                emitter->config.max_scale.x
            );
            
            particle->transform.scale.y = f32_rand_range(
                emitter->config.min_scale.y,
                emitter->config.max_scale.y
            );
        }
        else{
            f32 max = MAX(emitter->config.max_scale.x, emitter->config.max_scale.y);
            f32 min = MAX(emitter->config.min_scale.x, emitter->config.min_scale.y);
        
            f32 scale = f32_rand_range(min, max);
            particle->transform.scale = (Vector2){scale, scale};
        }
        
        // position.
        Vector2 radial_direction = {0};
        if(emitter->config.radial_spawn){
            // 6.2832 radians = 360 degrees 
            f32 radians = f32_rand_range(-3.1416f, 3.1416f);
            f32 ring_offset = f32_rand_range(
                emitter->config.radial_spawn_inner_ring,
                emitter->config.radial_spawn_outer_ring
            );
            radial_direction = vector2_rotate_by_radians(VECTOR2_UP, radians);
            Vector2 spawn_offset = vector2_mul_val(radial_direction, ring_offset);
            particle->transform.position = vector2_add(position, spawn_offset);            
        }
        else{
            particle->transform.position = position;
        }
        
        // linear velocity.
        f32 velocity = f32_rand_range(
            emitter->config.min_linear_velocity,
            emitter->config.max_linear_velocity 
        );
        
        Vector2 direction = emitter->config.use_radial_spawn_direction
        ? radial_direction
        : emitter->config.direction
        ;
        if(emitter->config.direction_min_range || emitter->config.direction_max_range){
            direction = vector2_rotate_by_radians(
                direction, 
                degrees_to_radians(
                    f32_rand_range(
                        emitter->config.direction_min_range, 
                        emitter->config.direction_max_range
                    )
                )
            );
        }
        
        particle->linear_velocity = vector2_mul_val(
            direction,
            velocity
        );
        
        // colour setting.
        if(emitter->config.use_gradient){        
            f32 gradient_ratio = f32_rand_range(0.0f, 1.0f);
            particle->colour = gfx_colour_lerp(emitter->config.colour, emitter->config.gradient_end_colour, gradient_ratio);
        }
        else{
            particle->colour = emitter->config.colour;
        }
        
        particle->fade_colour = emitter->config.fade_colour;
        particle->fade_over_lifetime = emitter->config.fade_over_lifetime;

        particle->layer = emitter->config.layer;
        particle->depth = emitter->config.depth;
        
        particle->angular_velocity = f32_rand_range(
            emitter->config.min_angular_velocity,
            emitter->config.max_angular_velocity
        );
        
        // prepare for next iteration.
        next_idx = particle->sibling_idx;
        counter += 1;
        
        if(counter >= particle_amount){
            emitter->next_particle_gid = emitter->system->gen_id_allocator.gen_ids[next_idx];
            break;
        }
    }
}


///
/// @ Visibility Codes
///


i32 create_entity_visibility_layer_code(
    bool world_layer_enabled,
    bool editor_layer_enabled
){
    i32 code = 0;

    if(world_layer_enabled){
        code |= ENTITY_VISIBILITY_LAYER_WORLD;
    }

    if(editor_layer_enabled){
        code |= ENTITY_VISIBILITY_LAYER_EDITOR;
    }
    
    return code;
}


///
/// @ Timer Manager.
///


bool timer_manager_init(
    TimerManager* manager, MemoryArena* arena, 
    i32 timer_amount, i32 timeout_data_element_size, i32 tick_data_element_size
){

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
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->end_time, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->start_time, &manager->length, timer_amount);

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->tick_callback, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->tick_data, &manager->tick_data_length, tick_data_element_size * timer_amount);
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->timeout_callback, &manager->length, timer_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->timeout_data, &manager->timeout_data_length, timeout_data_element_size * timer_amount);
    
    manager->timeout_data_element_size = timeout_data_element_size;
    manager->tick_data_element_size = tick_data_element_size;
    manager->is_init = true;

    return true;
}

/*
    `returns`
    a handle to the newly started timer; otherwise 0 if allocation was unsuccessful.
*/
TimerHandle timer_manager_timer_start(TimerManager* manager, TimerContext timer_ctx){
    
    TimerHandle handle = gen_id_allocator_alloc(&manager->gen_id_allocator);
    if(handle == 0){
        return handle;
    }

    i32 idx = gen_id_get_index(handle);

    if(timer_ctx.timeout_data_size > manager->timeout_data_element_size){
        ASSERT(false, "attempted to alloc timer timeout data that has a size greater than the allocated maximum size.");
    }
    else{ 
        // set_timer_timeout_data();
        size_t data_idx = idx * manager->timeout_data_element_size;
        BOUNDS_CHECK(data_idx, manager->timeout_data_length);
        COPY_MEMORY(manager->timeout_data + data_idx, timer_ctx.timeout_data, timer_ctx.timeout_data_size);
    }
    
    if(timer_ctx.tick_data_size > manager->tick_data_element_size){
        ASSERT(false, "attempted to alloc timer tick data that has a size greater than the allocated maximum size.");
    }
    else{ 
        // set_timer_tick_data();
        size_t data_idx = idx * manager->tick_data_element_size;
        BOUNDS_CHECK(data_idx, manager->tick_data_length);
        COPY_MEMORY(manager->tick_data + data_idx, timer_ctx.tick_data, timer_ctx.tick_data_size);
    }

    // set the timer state.
    manager->current_time[idx]      = timer_ctx.start_time;
    manager->previous_time[idx]     = timer_ctx.start_time;
    manager->time_scale[idx]        = timer_ctx.time_scale;
    manager->is_ticking[idx]        = TIMER_MANAGER_IS_TICKING_TRUE;
    manager->has_started[idx]       = TIMER_MANAGER_HAS_STARTED_TRUE;
    manager->timeout_callback[idx]  = timer_ctx.timeout_callback;
    manager->tick_callback[idx]     = timer_ctx.tick_callback;
    manager->end_time[idx]          = timer_ctx.end_time;
    manager->start_time[idx]        = timer_ctx.start_time;

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
    manager->timeout_callback[timer_idx]    = NULL;
    manager->tick_callback[timer_idx]       = NULL;
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
    simd_f32_add(manager->current_time, manager->current_time, manager->delta_time_scratch_space, manager->length, 0);
    simd_f32_clamp(manager->current_time, manager->start_time, manager->end_time, manager->current_time, manager->length, 0);
    
    for(i32 i = 0; i < manager->length; i++){
        if(manager->has_started[i] == TIMER_MANAGER_HAS_STARTED_FALSE){
            continue;
        }
        if(manager->is_ticking[i] == TIMER_MANAGER_IS_TICKING_FALSE){
            continue;
        }
        
        TimerTickCallback tick_callback = manager->tick_callback[i];
        if(tick_callback){
            i32 data_idx = i * manager->tick_data_element_size;
            BOUNDS_CHECK(data_idx, manager->tick_data_length);
            tick_callback(&manager->tick_data[data_idx], manager->current_time[i], manager->delta_time_scratch_space[i]);
        }

        if(manager->current_time[i] != manager->end_time[i]){
            continue;
        }
                
        TimerTimeoutCallback timeout_callback = manager->timeout_callback[i]; 
        if(timeout_callback){
            i32 data_idx = i * manager->timeout_data_element_size;
            BOUNDS_CHECK(data_idx, manager->timeout_data_length);
            timeout_callback(&manager->timeout_data[data_idx]);
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
bool timer_manager_timer_get_delta_tick_time(TimerManager manager, TimerHandle handle, f32* out_delta_tick){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, handle);
    if(!idx){
        return true;
    }
    BOUNDS_CHECK(idx, manager.length);
    *out_delta_tick = manager.delta_time_scratch_space[idx]; // ABS(manager.previous_time[idx] - manager.current_time[idx]);
    return false;
}

bool timer_manager_timer_get_current_tick(TimerManager manager, TimerHandle handle, f32* out_current_tick){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager.gen_id_allocator, handle);
    if(!idx){
        return false;
    }
    
    BOUNDS_CHECK(idx, manager.length);
    *out_current_tick = manager.current_time[idx];
    return true;
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
    
    if(entity->is_particle_emitter){
        particle_emitter_dealloc(&entity->particle_emitter);
    }
    
    string_clear(&entity->name);
    String name = entity->name;
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

void entity_manager_dealloc_entity_unsafe(EntityManager* manager, i32 entity_idx){
    // TODO:
    // may want to add functionality later so the user can deallocate an entity independently from its children.

    // NOTE:
    // the on_dealloc callback for this calls entity_manager_dealloc_entity_data_unsafe.
    bool preserve_sub_tree = false;
    if(!intrusive_list_remove_node_and_children(&manager->entity_hierarchy, entity_idx, manager)){
        ASSERT(false, "failed to remove entity from entity hierarchy.");
    }    
}

bool entity_manager_dealloc_entity(EntityManager* manager, GenId gid){
    i32 idx = gen_id_allocator_is_gen_id_valid(&manager->gen_id_allocator, gid);
    if(!idx){
        return false;
    }
    entity_manager_dealloc_entity_unsafe(manager, idx);
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
    i32 entity_amount, i32 physics_body_amount, i32 particle_amount,
    i32 timer_timeout_data_size, i32 timer_tick_data_size
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
    
    timer_manager_init(&manager->timer_manager, arena, entity_amount, timer_timeout_data_size, timer_tick_data_size);
    
    { // init_clickable_area_hierarchy();
    
        clickable_area_hierarchy_init(&manager->clickable_area_hierarchy, arena, entity_amount);
        MEMORY_ARENA_ALLOC_ARRAY(
            arena,
            manager->clickable_area_hierarchy_user_data_scratch_space, 
            &manager->clickable_area_hierarchy_user_data_scratch_space_length,
            entity_amount
        );
    }
    
    particle_system_init(&manager->particle_system, arena, particle_amount);
    
    manager->is_init = true;
}

bool entity_manager_get_world_camera(EntityManager entity_manager, Entity** out_entity){
    return entity_manager_get_entity(entity_manager, entity_manager.world_camera_entity_gid, out_entity);    
}

bool entity_manager_get_ui_camera(EntityManager entity_manager, Entity** out_entity){
    return entity_manager_get_entity(entity_manager, entity_manager.ui_camera_entity_gid, out_entity);    
}

void entity_manager_set_visibility_layer(EntityManager* entity_manager, i32 layer_code){        
    for(i32 i = 0; i < entity_manager->entity_length; i++){
        if(!entity_is_active_unsafe(*entity_manager, i)){
            continue;
        }
        
        Entity* entity = &entity_manager->entity[i];   
        if((entity->visibility_layer & layer_code) != 0){
            if(entity->is_sprite){
                gfx_sprite_set_active(entity_manager->gfx_state, entity->sprite_gid);
            }
        }
        else{        
            if(entity->is_sprite){
                gfx_sprite_set_inactive(entity_manager->gfx_state, entity->sprite_gid);
            }
        }    
    }
    
    entity_manager->visibility_layer = layer_code;
}


///
/// functions: Entity.
///


void entity_set_state_unsafe(EntityManager* entity_manager, i32 entity_idx, i32 state){
    BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
    Entity* entity = &entity_manager->entity[entity_idx];
    entity->state = state;
    if(entity->set_state_callback != NULL){
        entity->set_state_callback(entity_manager, entity, entity_idx, state);
    }
}

bool entity_set_state(EntityManager* entity_manager, GenId entity_gid, i32 state){
    i32 idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, entity_gid);
    if(!idx){
        return false;
    }
    
    entity_set_state_unsafe(entity_manager, idx, state);
    return true;
}

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

typedef struct{
    MenuStage stage_to_load_to;
} LoadMenuLevelCallbackContext;

void load_menu_level_callback(GameState* game_state, void* user_data){
    LoadMenuLevelCallbackContext* ctx = (LoadMenuLevelCallbackContext*)user_data;    
    game_state->stage = GameStage_Menu;
    game_state->menu_state.stage = ctx->stage_to_load_to; 
}

void load_gameplay_level_callback(GameState* game_state, void* user_data){
    game_state->stage = GameStage_Gameplay;
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
        u32 depth_layer = 0;
        
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

void entity_set_active_unsafe(EntityManager* entity_manager, i32 entity_idx,  bool is_active){
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
}

bool entity_set_active(EntityManager* entity_manager, GenId entity_gid, bool is_active){

    i32 entity_idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, entity_gid);
    if(!entity_idx){
        return false;
    }
    
    entity_set_active_unsafe(entity_manager, entity_idx, is_active);
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
/// @ Camera.
///


#define CAMERA_POSITIONAL_OFFSET (Vector3){.z = -1.0f}
#define WORLD_CAMERA_COORDINATE_SPACE GFX_CoordinateSpace_Cartesian
#define WORLD_CAMERA_DEFAULT_NEAR_Z 0.01f
#define WORLD_CAMERA_DEFAULT_FAR_Z 100.0f
#define WORLD_CAMERA_DEFAULT_ORTHOGRAPHIC_SIZE 20.0f
#define UI_CAMERA_COORDINATE_SPACE GFX_CoordinateSpace_Rasterised 
#define UI_CAMERA_DEFAULT_NEAR_Z 0.01f
#define UI_CAMERA_DEFAULT_FAR_Z 100.0f

GenId entity_spawn_world_camera(EntityManager* entity_manager, Transform2D transform, GenId parent_entity_gid, Entity** out_entity){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent_entity_gid);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity)){
        
        entity->type_id = EntityTypeId_WorldCamera;
        entity->visibility_layer = ENTITY_VISIBILITY_LAYER_WORLD;
        
        { // set_camera_settings();
        
            entity->camera_orthographic_size = WORLD_CAMERA_DEFAULT_ORTHOGRAPHIC_SIZE * MAX(transform.scale.x, transform.scale.y);
            entity->camera_near_z = WORLD_CAMERA_DEFAULT_NEAR_Z;
            entity->camera_far_z = WORLD_CAMERA_DEFAULT_FAR_Z;
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);        
        *out_entity = entity;
    }
    return entity_gid;
}


///
/// @ Wall.
///


void wall_on_enter_collision_callback(FIZX_CollisionInfo info, void* user_data){

}

GenId entity_spawn_wall(EntityManager* entity_manager, Transform2D transform, GenId parent, Entity** out_entity){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity))
    {
        entity->type_id = EntityTypeId_Wall;
        entity->visibility_layer = ENTITY_VISIBILITY_LAYER_WORLD;
        
        { // alloc_physics_body();
            entity->is_physics_body = true;
                        
            FIZX_BodyConfig body_config = {
                .layer = WALL_PHYSICS_BODY_LAYER,
                .mask = PHYSICS_LAYER_ALL
            };
            
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state, 
                TRANSFORM2D_IDENTITY, 
                body_config
            );
            
            Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;
            FIZX_Material material = {
                .static_friction = 0.75f, 
                .kinetic_friction = 0.5f, 
                .density = 22.6f, 
                .restitution = 0.224f, 
                .rotational_response = true
            };
            
            GenId shape_gid = fizx_rectangle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                shape_transform,
                FIZX_ShapeBehaviour_Kinematic,
                FIZX_CollisionCallbackBehaviour_All,
                &entity_gid,
                square,
                material
            );
            
            fizx_shape_set_on_enter_callback(&entity_manager->fizx_state, shape_gid, wall_on_enter_collision_callback);
            
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
            gfx_sprite_set_colour_blend(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourBlend_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_WORLD_ENVIRONMENT_TOP);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 2);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
        *out_entity = entity;
    }
    return entity_gid;
}


///
/// @ LevelRoot.
///


GenId entity_spawn_level_root(EntityManager* entity_manager, Transform2D transform, GenId parent, Entity** out_entity){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity))
    {
        entity->type_id = EntityTypeId_LevelRoot;
        entity->visibility_layer = ENTITY_VISIBILITY_LAYER_WORLD;    
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
        *out_entity = entity;
    }
    return entity_gid;
}


///
/// @ Player procedures.
///

#define PLAYER_MAX_VELOCITY 1.0f


void player_update(EntityManager* entity_manager, Entity* entity, f32 delta_time){
    
    Vector2 move_dir = {0};
    
    if(input_is_key_pressed(Key_W)){
        move_dir.y += 1.0f;
    }
    if(input_is_key_pressed(Key_A)){
        move_dir.x -= 1.0f;    
    }
    if(input_is_key_pressed(Key_S)){
        move_dir.y -= 1.0f;
    }
    if(input_is_key_pressed(Key_D)){
        move_dir.x += 1.0f;
    }

    move_dir = vector2_normalise(move_dir);

    Vector2 linear_velocity = {0};
    if(!fizx_body_get_linear_velocity(&entity_manager->fizx_state, entity->physics_body_gid, &linear_velocity)){
        ASSERT(false, "unable to get velocity");
    }

    f32 max_speed = 5.0f;
    f32 acceleration = 50.0f;

    Vector2 target_velocity = vector2_mul_val(move_dir, 5.0f);
    Vector2 error = vector2_sub(target_velocity, linear_velocity);
    f32 error_len = vector2_len(error);
    
    if(error_len > F32_EPSILON){
        bool has_input = (move_dir.x != 0.0f || move_dir.y != 0.0f);
        if(!has_input){
            return;
        }
        
        // use this when not doing things with linear drag; instead of the above if statement.
        // f32 rate = has_input ? params->acceleration : params->deceleration;
        
        f32 rate = acceleration;
        //largest velocity change allowed this frame.
        f32 max_dv = rate * delta_time;
        // never overshoot the target velocity.
        f32 dv_len = MIN(max_dv, error_len);
        
        Vector2 dv = vector2_mul_val(error, dv_len / error_len);
        
        f32 mass;
        if(fizx_body_get_mass(&entity_manager->fizx_state, entity->physics_body_gid, &mass)){
            fizx_body_impulse_linear_force(&entity_manager->fizx_state, entity->physics_body_gid, vector2_mul_val(dv, mass));
        }
        else{
            ASSERT(false, "failed to get mass.");
        }
        
    }
}

GenId entity_spawn_player(EntityManager* entity_manager, Transform2D transform, GenId parent, Entity** out_entity){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity))
    {
        entity->type_id = EntityTypeId_Player;
        entity->visibility_layer = ENTITY_VISIBILITY_LAYER_WORLD;
        
        { // alloc_physics_body();
            entity->is_physics_body = true;
            
            entity->physics_body_gid = fizx_body_alloc(
                &entity_manager->fizx_state, 
                TRANSFORM2D_IDENTITY,
                PLAYER_PHYSICS_BODY_CONFIG
            );
            
            Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
            Transform2D shape_transform = TRANSFORM2D_IDENTITY;
            FIZX_Material material = {
                .static_friction = 0.224f,
                .kinetic_friction = 0.125f,
                .density = 2.24f
            };
            
            GenId shape_gid = fizx_rectangle_rigid_alloc(
                &entity_manager->fizx_state,
                entity->physics_body_gid,
                shape_transform,
                FIZX_ShapeBehaviour_Dynamic,
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
            gfx_sprite_set_texture_view(    entity_manager->gfx_state, entity->sprite_gid, TEXTURE_VIEW_PLAYER);
            gfx_sprite_set_material(        entity_manager->gfx_state, entity->sprite_gid, SPRITE_MATERIAL_IMAGE);
            gfx_sprite_set_virtual_texture( entity_manager->gfx_state, entity->sprite_gid, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_sprite_set_origin(          entity_manager->gfx_state, entity->sprite_gid, GFX_SpriteOrigin_Center);
            gfx_sprite_set_colour(          entity_manager->gfx_state, entity->sprite_gid, (GFX_Colour){.r = 0.25f, .g = 1.0f, .b = 0.75f, .a = 1.0f});
            gfx_sprite_set_colour_blend(    entity_manager->gfx_state, entity->sprite_gid, GFX_ColourBlend_Tint);
            gfx_sprite_set_layer(           entity_manager->gfx_state, entity->sprite_gid, SPRITE_LAYER_WORLD_ENVIRONMENT_MID);
            gfx_sprite_set_depth(           entity_manager->gfx_state, entity->sprite_gid, 2);
        }
        
        entity_set_local_transform(entity_manager, entity_gid, transform);
        *out_entity = entity;
    }
    return entity_gid;
}


///
/// @ Point.
///


GenId entity_spawn_empty(EntityManager* entity_manager, Transform2D transform, GenId parent, Entity** out_entity){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    
    if(entity_manager_get_entity(*entity_manager, entity_gid, &entity))
    {
        entity->type_id = EntityTypeId_Empty;
        entity->visibility_layer = ENTITY_VISIBILITY_LAYER_WORLD;        
        entity_set_local_transform(entity_manager, entity_gid, transform);
        *out_entity = entity;
    }
    return entity_gid;
}

GenId entity_spawn(EntityManager* entity_manager, EntityTypeId type_id, Transform2D transform, GenId parent_gid, Entity** out_entity){
    Entity* entity = NULL;
    GenId entity_gid = {0};
    
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
            entity_gid = entity_spawn_level_root(entity_manager, transform, parent_gid, &entity);
        }break;
        case EntityTypeId_Wall:{
            entity_gid = entity_spawn_wall(entity_manager, transform, parent_gid, &entity);
        }break;
        case EntityTypeId_WorldCamera:{
            entity_gid = entity_spawn_world_camera(entity_manager, transform, parent_gid, &entity);
        }break;
        case EntityTypeId_Empty:{
            entity_gid = entity_spawn_empty(entity_manager, transform, parent_gid, &entity);
        }break;
        case EntityTypeId_Player:{
            entity_gid = entity_spawn_player(entity_manager, transform, parent_gid, &entity);
        }break;
    }
    
    if(entity){
        if(entity->is_sprite){
            if((entity->visibility_layer & entity_manager->visibility_layer) != 0){
                gfx_sprite_set_active(entity_manager->gfx_state, entity->sprite_gid);
            }
            else{
                gfx_sprite_set_inactive(entity_manager->gfx_state, entity->sprite_gid);        
            }    
        }
        
        *out_entity = entity;
    }
    
    return entity_gid;    
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
GenId level_manager_load_asset_file_recurssive(
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
                
                level_manager_load_asset_file_recurssive(level_manager, scratch_space, transient, entity_manager->deserialised_entity_count-1, root_parent_idx, root_transform);
            }break;
            case LEVEL_FILE_NATIVE_ENTITY_TOKEN: {

                // get the slot to load into.
                BOUNDS_CHECK(entity_manager->deserialised_entity_count, entity_manager->deserialised_entity_length);
                deserialised_entity = &entity_manager->deserialised_entity[entity_manager->deserialised_entity_count];

                // first scscanf for the name.
                // we do this as pattern matchin a blank name tag can yield no lines being read
                // as the first character in the line would be the delimitter ';'.
                lines_read = sscanf(
                    file_data,
                    "%[^;]%n;%n",
                    deserialised_entity->name.chars,
                    &deserialised_entity->name.count,
                    &bytes_consumed
                );
                                
                file_data = lines_read == 0
                ? file_data + 1 // increment past the ';' as the name of the entity is blank.
                : file_data + bytes_consumed
                ;

                deserialised_entity->spawn_transform = TRANSFORM2D_IDENTITY;
                lines_read = sscanf(
                    file_data,
                    "%d;%d;%d;%d;%f;%f;%f;%f;%f;%f;%n",
                    &deserialised_entity->entity_type_id,
                    &deserialised_entity->tag,
                    &deserialised_entity->parent_deserialised_entity_idx,
                    &deserialised_entity->state,
                    &deserialised_entity->spawn_transform.position.x,
                    &deserialised_entity->spawn_transform.position.y,
                    &deserialised_entity->spawn_transform.scale.x,
                    &deserialised_entity->spawn_transform.scale.y,
                    &deserialised_entity->spawn_transform.sine,
                    &deserialised_entity->spawn_transform.cosine,
                    &bytes_consumed
                );
        
                if(lines_read == 10){
            
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
                                        
                    Entity* entity;
                    // create the new deserialised entity.
                    deserialised_entity->entity_gid = entity_spawn(
                        entity_manager, 
                        deserialised_entity->entity_type_id, 
                        deserialised_entity->spawn_transform, 
                        parent_gid,
                        &entity
                    );
                    i32 entity_idx = gen_id_get_index(deserialised_entity->entity_gid);
                    
                    if(entity){
                        if(parent_idx == 0){
                            // set the root entity's serialisation file path to the file path we just loaded it from.
                            string_push(&entity->serialisation_file_path, file_path);
                        }
                        else{
                            entity_set_state_unsafe(entity_manager, entity_idx, deserialised_entity->state);
                            entity->tag = deserialised_entity->tag;
                        }
                        string_push(&entity->name, deserialised_entity->name);
                    }
                    else{
                        ASSERT(false, "failed to retrieve spawned entity from file.");
                        break;                            
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

GenId level_manager_load_asset_file(
    LevelManager* level_manager, String file_path, MemoryArena* transient, Transform2D transform 
){
    return level_manager_load_asset_file_recurssive(level_manager, file_path, transient, 0, 1, transform);
}


///
/// @ GameState
///


void game_state_init(GameState* game_state, MemoryArena* persistent, MemoryArena* transient, GFX_State* gfx_state){

    ASSERT(!game_state->is_init, "already init.");

    game_state->time_scale = 1.0f;

    i32 entity_amount = 2048;
    i32 physics_body_amount = 128;
    i32 particle_amount = 2048;
    i32 timeout_data_element_size = 512;
    i32 tick_data_element_size = 512;

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

    entity_manager_init(
        &game_state->entity_manager, persistent, gfx_state, 
        entity_amount, physics_body_amount, particle_amount, 
        timeout_data_element_size, tick_data_element_size 
    );
    
    level_manager_init(&game_state->level_manager, persistent, &game_state->entity_manager, 256);
    game_state->is_init = true;
}

void game_state_on_level_load(GameState* game_state){
    clickable_area_hierarchy_reset(&game_state->entity_manager.clickable_area_hierarchy);
    game_state->mouse_state.clicked_entity_idx = 0;
    
    // reset_entity_manager();
    game_state->entity_manager.world_camera_entity_gid = 0;
    game_state->entity_manager.ui_camera_entity_gid = 0;                
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
            entity_manager_dealloc_entity(entity_manager, level_manager->world_root_entity_gid);
            level_manager->world_root_entity_gid = level_manager_load_asset_file(level_manager, ctx->file_path, transient, ctx->transform);
            if(entity_manager_get_entity(*entity_manager, level_manager->world_root_entity_gid, &world_entity)){
                level_manager->world_root_entity_file_path = &world_entity->serialisation_file_path;
            
                game_state_on_level_load(game_state);
    
                if(ctx->callback){
                    ctx->callback(game_state, ctx->callback_user_data);
                }
            }
            else{
                ASSERT(false, "failed to set world root entity file path");
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

void game_state_update(GameState* game_state, MemoryArena* persistent, MemoryArena* transient, f32 delta_time, bool in_editor_mode){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_State* gfx_state = entity_manager->gfx_state;
    FIZX_DrawInfo* fizx_draw_state = &entity_manager->fizx_draw_state;
    TimerManager* timer_manager = &entity_manager->timer_manager;
    FIZX_State* fizx_state = &entity_manager->fizx_state;
    GameMouseState* mouse_state = &game_state->mouse_state;
    LevelManager* level_manager = &game_state->level_manager;

    // retrieve necessary data.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(gfx_state);

    { // input_handling();
    
        { // update fizx draw state.

            if(input_is_key_just_pressed(Key_PageUp)){
                fizx_draw_state->draw_bvh_branches = !fizx_draw_state->draw_bvh_branches;
            }
            if(input_is_key_just_pressed(Key_PageDown)){
                fizx_draw_state->draw_bvh_leaves = !fizx_draw_state->draw_bvh_leaves;
            }
            if(input_is_key_just_pressed(Key_Home)){
                fizx_draw_state->draw_body_shapes = !fizx_draw_state->draw_body_shapes;
            }
            if(input_is_key_just_pressed(Key_End)){
                fizx_draw_state->draw_collision_info = !fizx_draw_state->draw_collision_info;
            }
            
            if(input_is_key_just_pressed(Key_Escape)){
                game_state->show_pause_menu = !game_state->show_pause_menu; 
            }
        }

        if(input_is_key_just_pressed(Key_Tilde)){
            
            String file_path = string_duplicate(*game_state->level_manager.world_root_entity_file_path, transient);
            
            DeferredLevelLoadContext ctx = {
                .transform = TRANSFORM2D_IDENTITY,
                .file_path = file_path
            };
            
            level_manager_defer_load_level(&game_state->level_manager, ctx);
        }
    }
    
    { // time management.

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
                fizx_state_fixed_update(fizx_state, &collision_callback_ctx, FIXED_DELTA_TIME * 1.25f, 32);            
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
                            if(fizx_body_get_global_transform(fizx_state, entity->physics_body_gid, &transform)){
                                
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
                
            // entity updates.            
            switch(entity->type_id){
                case EntityTypeId_Player:{
                    player_update(entity_manager, entity, delta_time);
                }break;
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
    
    if(!in_editor_mode){
    
    }
    
    particle_system_update_and_draw(&entity_manager->particle_system, entity_manager->gfx_state, delta_time);
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
        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER },           \
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

                        // String menu_level_name = STRING_FROM_LITERAL("menu_level.scsv", transient);

                        for(i32 i = 0; i < file_count; i += 4){
                            GAME_GFX_CLAY_LEVEL_SELECT_BUTTON_CONTAINER(){
                                
                                i32 idx = i;
                                for(i32 j = 0; j < 4; j++){
                                    i32 idx = i + j;
                                    if(idx < file_count){
                                        String partial_file_path = file_paths[idx];
                                        // if(string_contents_equal(partial_file_path, menu_level_name)){
                                        //     continue;
                                        // }
                                        
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
                                            load_gameplay_level_callback,
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
            case GameStage_Gameplay:{
                 if(game_state->show_pause_menu){
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
                            load_gameplay_level_callback,
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
            }break; 
        }
    }

    
    gfx_clay_end_layout(gfx_state, delta_time, SPRITE_LAYER_GAME_UI, VIRTUAL_TEXTURE_ID_FONT, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
}