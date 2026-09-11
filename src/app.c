#include "platform.h"

#ifdef NDEBUG
#include <stdio.h>
#endif

#include "base_layer/base.h"
#include "base_layer/base_cpu.c"
#include "base_layer/base_math.c"
#include "base_layer/base_algorithms.c"
#include "base_layer/base_structures.c"
#include "renderer/gfx.c"
#include "renderer/gfx_app_types.c"
#include "fizx/fizx.c"
#include "fizx/fizx_draw.c"
#include "gameplay/entity.c"
#include "gameplay/player.c"

/**====================
    defines.
====================**//**/

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

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

typedef struct{
    EntityManager* entity_manager;    
} CollisionCallbackContext;

/**====================
    globals
====================**//**/

WindowContext* window_ctx;
GFX_State gfx_state;
MemoryArena gfx_memory;
EntityManager entity_manager;

f32 time_scale = 1.0f;

/**====================
    functions
====================**//**/

void enemy_body_on_enter_callback(FIZX_CollisionInfo info, void* user_data){
    CollisionCallbackContext* ctx = (CollisionCallbackContext*)user_data;
    GenId* enemy_gid = (GenId*)info.target_user_data; 
    
    if((info.source_layer & PHYSICS_LAYER_PLAYER) != 0){
        entity_deplete_health(ctx->entity_manager, *enemy_gid, 1);
    }    
}

void enemy_body_on_exit_callback(FIZX_CollisionInfo info, void* user_data){
    if((info.source_layer & PHYSICS_LAYER_PLAYER) != 0){
        platform_output_message("exit player\n");
    }
}

void app_update(MemoryArena* persistent, MemoryArena* transient, f32 delta_time){
    
    Vector2 mouse_world_position = gfx_get_mouse_world_position(&gfx_state);
        
    f32 camera_speed = 1.0f * delta_time * gfx_state.world_camera.orthographic_size;
    bool x = input_is_key_pressed(KEY_RIGHT);
    if(input_is_key_pressed(KEY_Q))     {gfx_state.world_camera.orthographic_size -= gfx_state.world_camera.orthographic_size * 1.0f * delta_time;}
    if(input_is_key_pressed(KEY_E))     {gfx_state.world_camera.orthographic_size += gfx_state.world_camera.orthographic_size * 1.0f * delta_time;}
    if(input_is_key_pressed(KEY_RIGHT)) {gfx_state.world_camera.position.x += camera_speed;}
    if(input_is_key_pressed(KEY_LEFT))  {gfx_state.world_camera.position.x -= camera_speed;}
    if(input_is_key_pressed(KEY_UP))    {gfx_state.world_camera.position.y += camera_speed;}
    if(input_is_key_pressed(KEY_DOWN))  {gfx_state.world_camera.position.y -= camera_speed;}
    
    if(input_is_key_pressed(KEY_SPACE)){
        time_scale = 0.0f;        
    }
    else if(input_is_key_pressed(KEY_F)) {
        time_scale = 0.1f;
    }
    else{
        time_scale = 1.0f;
    }
    

    for(i32 i = 0; i < entity_manager.entity_length; i++){
        Entity* entity = &entity_manager.entity[i];
        if(entity->physics_body_gid != 0){
            Transform2D transform2d;
            if(fizx_body_get_transform(&entity_manager.fizx_state, entity->physics_body_gid, &transform2d)){
                entity->transform = transform2d_to_transform3d(transform2d);
            }
        }
    }

    player_update(&entity_manager, mouse_world_position, delta_time);

    for(i32 i = 0; i < entity_manager.entity_length; i++){
        Entity* entity = &entity_manager.entity[i];
        if(!gfx_sprite_id_equals(entity->sprite_id, (GFX_SpriteId){0})){
            f32 z = entity->transform.position.z;
            gfx_sprite_set_transform(&gfx_state, entity->sprite_id, transform3d_to_matrix4x4(entity->transform));
            entity->transform.position.z = z;
        }
    }
    
    Vector2I result;
    platform_get_mouse_position(&result.x, &result.y);
}

GFX_State app_gfx_init(MemoryArena* persistent, MemoryArena* transient, WindowContext window_ctx){

    memory_arena_partition(persistent, &gfx_memory, MEGABYTE(2));

    /**
        font textures.
    **/
    GFX_FontTextureInitInfo font_texture_init_info ={
        .base_glyph_index = 32,
        .glyph_count = 128,
        .texture_height = 512,
        .texture_width = 512
    };
    MEMORY_ARENA_ALLOC_ARRAY(transient, font_texture_init_info.virtual_textures, &font_texture_init_info.virtual_textures_length, 2);
    for(i32 i = 0; i < font_texture_init_info.virtual_textures_length; i++){
        font_texture_init_info.virtual_textures[i] = i+1;
    }

    /**
        image textures.
    **/
    GFX_ImageTexturesInitInfo* image_textures_init_info;
    i32 image_textures_init_info_length;
    MEMORY_ARENA_ALLOC_ARRAY(transient, image_textures_init_info, &image_textures_init_info_length, 4);

    BOUNDS_CHECK(0, image_textures_init_info_length);
    image_textures_init_info[0] = (GFX_ImageTexturesInitInfo){.width = 512, .height = 512, .max_textures = 16};
    BOUNDS_CHECK(1, image_textures_init_info_length);
    image_textures_init_info[1] = (GFX_ImageTexturesInitInfo){.width = 360, .height = 162, .max_textures = 2};
    BOUNDS_CHECK(2, image_textures_init_info_length);
    image_textures_init_info[2] = (GFX_ImageTexturesInitInfo){.width = 640, .height = 360, .max_textures = 2};
    BOUNDS_CHECK(3, image_textures_init_info_length);
    image_textures_init_info[3] = (GFX_ImageTexturesInitInfo){.width = 16, .height = 16, .max_textures = 24};


    /**
        sprite layers.
    **/
    GFX_SpriteLayerCreateInfo* sprite_layer_create_infos;
    i32 sprite_layer_create_infos_length;
    MEMORY_ARENA_ALLOC_ARRAY(transient, sprite_layer_create_infos, &sprite_layer_create_infos_length, 2);

    BOUNDS_CHECK(0, sprite_layer_create_infos_length);
    sprite_layer_create_infos[0] = (GFX_SpriteLayerCreateInfo){.max_sprites = 512};
    BOUNDS_CHECK(1, sprite_layer_create_infos_length);
    sprite_layer_create_infos[1] = (GFX_SpriteLayerCreateInfo){.max_sprites = 512};

    /**
        context.
    **/
    GFX_StateInitInfo gfx_init_info = {
        .max_file_path_length = 256,
        .max_user_uniform_buffer_size_in_bytes = sizeof(Ubo),
        .max_user_storage_buffer_size_in_bytes = 4, // this should be 4 when not used for some reason idk.
        .max_virtual_textures = 1024,
        .font_textures_init_info = font_texture_init_info,
        .image_textures_init_infos = image_textures_init_info,
        .image_textures_init_infos_length = image_textures_init_info_length,
        .sprite_layer_create_infos = sprite_layer_create_infos,
        .sprite_layer_create_infos_length = sprite_layer_create_infos_length,
        .final_render_texture_width = 1920,
        .final_render_texture_height = 1080,
        .graphics_pipeline_shader_file_path = (String){.chars = "assets/shader.wgsl", .length = 18}
    };

    gfx_state_init(&gfx_state, gfx_init_info, persistent, transient, window_ctx, WINDOW_WIDTH, WINDOW_HEIGHT);
    return gfx_state;
}

void app_fixed_update(f32 delta_time){
    
}

void app_late_update(f32 delta_time){

    /**
        NOTE:
        this might have to be swapped for the final render target resolution, maybe idk.
    **/
    f32 aspect_ratio = platform_window_calc_aspect_ratio(*window_ctx);
    gfx_camera_update_projection_matrix(&gfx_state.world_camera, aspect_ratio);

    Ubo ubo = {
        .world_camera_matrix = 
            matrix4x4_mul(
                matrix4x4_mul(
                    gfx_state.world_camera.projection, 
                    gfx_state.world_camera.view
                ), 
            gfx_state.world_camera.model
        ),
        .screen_camera_matrix = 
            matrix4x4_mul(
                matrix4x4_mul(
                    gfx_state.world_camera.projection, 
                    gfx_state.world_camera.view
                ), 
            gfx_state.world_camera.model
        ),
        .world_camera_far_z = gfx_state.world_camera.far_z,
        .world_camera_near_z = gfx_state.world_camera.near_z,
    };

    gfx_write_to_user_uniform_buffer(&gfx_state, &ubo, sizeof(Ubo));
}

void app_main(){

    /**
        memory allocation.
    **/
    platform_init_persistent_memory(MEGABYTE(500));
    platform_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = platform_get_persistent_memory();
    MemoryArena* transient = platform_get_transient_memory();
    
    window_ctx = platform_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);

    input_init(persistent);

    gfx_orthographic_camera_init(&gfx_state.world_camera, (Vector3){.y = 5.0f, .z = -4.0f}, 0.01f, 100.0f, 22.0f);
    gfx_global_wireframe_thickness = 0.05f;
    gfx_state = app_gfx_init(persistent, transient, *window_ctx);
    
    String file_path = {0};
    string_init(&file_path, transient, 32);

    string_push_chars(&file_path, "assets/image.png", 16);
    gfx_virtual_texture_set_file_path(&gfx_state, file_path, 3);
    gfx_load_image_texture(&gfx_state, 3);
    
    string_clear(&file_path);
    
    string_push_chars(&file_path, "assets/sling_shot.png", 21);
    gfx_virtual_texture_set_file_path(&gfx_state, file_path, 4);
    gfx_load_image_texture(&gfx_state, 4);

    FIZX_DrawInfo fizx_draw_state = {
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
        .sprite_layer                   = SPRITE_LAYER_WORLD,
        .wireframe_thickness            = 0.005f,
        .material_idx                   = SPRITE_MATERIAL_DEBUG,
        .draw_body_shapes               = true,
        // .draw_bvh_leaves = true,
        // .draw_bvh_branches = true,
        // .draw_collision_info = true
    };

    Transform3D shape_transform = {.scale = VECTOR3_ONE};
    Circle circle = {.x = 0.0f, .y = 0.0f, .radius = 1.0f};
    Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
    FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 5.0f, .restitution = 0.0f};

    i32 entity_amount = 2048;
    i32 physics_body_amount = 128;
    entity_manager = (EntityManager){0};
    entity_manager_init(&entity_manager, persistent, entity_amount, physics_body_amount);
    Entity* entity;
        
    // GenId entity_shape_gid = fizx_circle_rigid_alloc(&entity_manager.fizx_state, circle, transform_to_transform2d(shape_transform), FIZX_ShapeBehaviour_Dynamic, material, dynamic_body_gid, true);
    
    // floor entity.
    GenId floor_gid = entity_manager_alloc_entity(&entity_manager);
    entity_manager_get_entity(entity_manager, floor_gid, &entity);
    {
        Transform3D entity_transform = {.scale = {.x = 100.0f, .y = 1.0f}};
        entity->is_physics_body = true;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager.fizx_state, transform3d_to_transform2d(entity_transform), false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager.fizx_state, entity->physics_body_gid, transform3d_to_transform2d(shape_transform), FIZX_ShapeBehaviour_Kinematic, &floor_gid, PHYSICS_LAYER_ENVIRONMENT, square, material, true);
    }
        
    // another dyanimc entity (piggy).
    GenId enemy_gid = entity_manager_alloc_entity(&entity_manager);
    entity_manager_get_entity(entity_manager, enemy_gid, &entity);
    {
        Transform3D entity_transform = {.position = {.x = -1.5f, .y = 5.0f}, .scale = VECTOR3_ONE};
        entity->is_physics_body = true;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager.fizx_state, transform3d_to_transform2d(entity_transform), true);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager.fizx_state, entity->physics_body_gid, transform3d_to_transform2d(shape_transform), FIZX_ShapeBehaviour_Dynamic, &enemy_gid, PHYSICS_LAYER_ENEMY, square, material, true);
    
        entity->is_health = true;
        entity->health = 2;
    
        fizx_shape_set_on_enter_callback(&entity_manager.fizx_state, enemy_body_on_enter_callback, entity_shape_gid);
        fizx_shape_set_on_exit_callback(&entity_manager.fizx_state, enemy_body_on_exit_callback, entity_shape_gid);
    }
    
    GenId sling_shot_gid = entity_manager_alloc_entity(&entity_manager);
    entity_manager_get_entity(entity_manager, sling_shot_gid, &entity);
    {
        entity->transform = (Transform3D){.position = {.y = 2.0f}, .scale = vector3_mul_val(VECTOR3_ONE, 3.0f)};
        Transform3D sprite_transform = {.position = {.z = 200.0f}, .scale = vector3_mul_val(VECTOR3_ONE, 1000.0f)};
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(&gfx_state, SPRITE_LAYER_WORLD, &success);
        gfx_sprite_init(
            &gfx_state, entity->sprite_id, transform3d_to_matrix4x4(sprite_transform), GFX_COLOUR_WHITE, (GFX_SpriteRegion){.width = 16, .height = 16}, GFX_ColourState_Tint,
            4, SPRITE_MATERIAL_IMAGE, true
        );
    
        entity_spawn_bird(&entity_manager, &gfx_state, vector3_add(entity->transform.position, (Vector3){.y = 0.5f}));
    }

    u128 prev_process_tick_in_mili  = 0;
    f32 previous_time_in_seconds    = 0.0f;
    f32 fixed_update_accumulator    = 0.0f;
    while(!window_ctx->is_destroyed){
        u128 process_tick = platform_get_proccess_tick();
        // micro to mili.
        u128 process_tick_in_mili = process_tick / 1000;
        u128 delta_tick_in_mili = process_tick_in_mili - prev_process_tick_in_mili;
        // mili to seconds.
        f32 delta_time = (f32)delta_tick_in_mili * 0.0001f;
        prev_process_tick_in_mili = process_tick_in_mili;

        delta_time *= time_scale;

        printf("%.5f", delta_time);

        platform_window_update(window_ctx);
        // fixed update.
        {
            fixed_update_accumulator += delta_time;
            if(fixed_update_accumulator > DELTA_TIME_ACCUMULATOR_SLOW_DOWN){
                fixed_update_accumulator = DELTA_TIME_ACCUMULATOR_SLOW_DOWN;
            }

            while(fixed_update_accumulator >= FIXED_DELTA_TIME){
                app_fixed_update(FIXED_DELTA_TIME);
                CollisionCallbackContext collision_callback_ctx = {.entity_manager = &entity_manager}; 
                fizx_state_fixed_update(&entity_manager.fizx_state, &collision_callback_ctx, FIXED_DELTA_TIME, 16);
                fixed_update_accumulator -= FIXED_DELTA_TIME;
            }
        }

        // update.
        {
            input_update();
            app_update(persistent, transient, delta_time);
        }

        // late update.
        {
            app_late_update(delta_time);
        }
    
        // final update.
        {
            fizx_state_draw(entity_manager.fizx_state, &gfx_state, fizx_draw_state, delta_time);
            entity_manager_debug_draw(entity_manager, &gfx_state, delta_time);
            gfx_state_draw(&gfx_state);
            transient->stride = 0;
        }
    }

    platform_window_context_free(window_ctx);
}