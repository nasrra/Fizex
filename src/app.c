#include "platform.h"

#include "base_layer/base.h"
#include "base_layer/base_cpu.c"
#include "base_layer/base_math.c"
#include "base_layer/base_algorithms.c"
#include "base_layer/base_structures.c"
#include "renderer/gfx.c"
#include "renderer/gfx_app_types.c"
#include "fizx/fizx.c"
#include "fizx/fizx_draw.c"
#include "gameplay/gameplay.c"

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
                entity->transform = transform2d;
            }
        }
    }

    player_update(&entity_manager, &gfx_state, mouse_world_position, delta_time);

    for(i32 i = 0; i < entity_manager.entity_length; i++){
        Entity* entity = &entity_manager.entity[i];
        if(!gfx_sprite_id_equals(entity->sprite_id, (GFX_SpriteId){0})){
            gfx_sprite_set_transform(&gfx_state, entity->sprite_id, entity->transform, entity->sprite_depth);
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
        .max_glyphs = 256,
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
    MEMORY_ARENA_ALLOC_ARRAY(transient, image_textures_init_info, &image_textures_init_info_length, 5);

    BOUNDS_CHECK(0, image_textures_init_info_length);
    image_textures_init_info[0] = (GFX_ImageTexturesInitInfo){.width = 150, .height = 150, .max_textures = 16};
    BOUNDS_CHECK(1, image_textures_init_info_length);
    image_textures_init_info[1] = (GFX_ImageTexturesInitInfo){.width = 360, .height = 162, .max_textures = 2};
    BOUNDS_CHECK(2, image_textures_init_info_length);
    image_textures_init_info[2] = (GFX_ImageTexturesInitInfo){.width = 640, .height = 360, .max_textures = 2};
    BOUNDS_CHECK(3, image_textures_init_info_length);
    image_textures_init_info[3] = (GFX_ImageTexturesInitInfo){.width = 16, .height = 16, .max_textures = 24};
    BOUNDS_CHECK(4, image_textures_init_info_length);
    image_textures_init_info[4] = (GFX_ImageTexturesInitInfo){.width = 1028, .height = 1028, .max_textures = 4};


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
    gfx_camera_update_projection_matrix(&gfx_state.screen_camera, aspect_ratio);

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
                    gfx_state.screen_camera.projection,
                    gfx_state.screen_camera.view
                ),
            gfx_state.screen_camera.model
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

    gfx_clay_init(MEGABYTE(8), WINDOW_WIDTH, WINDOW_HEIGHT);

    window_ctx = platform_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);

    input_init(persistent);

    gfx_orthographic_camera_init(&gfx_state.world_camera, GFX_CoordinateSpace_Cartesian, (Vector3){.y = 5.0f, .z = -4.0f}, 0.01f, 100.0f, 22.0f);
    gfx_orthographic_camera_init(&gfx_state.screen_camera, GFX_CoordinateSpace_Rasterised, (Vector3){.z = -1.0f}, 0.01f, 1028.0f, 1080.0f);
    gfx_global_wireframe_thickness = 0.05f;
    gfx_state = app_gfx_init(persistent, transient, *window_ctx);

    String file_path = {0};
    string_init(&file_path, transient, 48);

    string_clear(&file_path);
    string_push_chars(&file_path, "assets/sprites/test-sheet.png", 29);
    gfx_virtual_texture_set_file_path(&gfx_state, file_path, VIRTUAL_TEXTURE_ID_TEST_SHEET);
    gfx_load_image_texture(&gfx_state, VIRTUAL_TEXTURE_ID_TEST_SHEET);

    string_clear(&file_path);
    string_push_chars(&file_path, "assets/sprites/sling_shot.png", 29);
    gfx_virtual_texture_set_file_path(&gfx_state, file_path, VIRTUAL_TEXTURE_ID_SLING_SHOT);
    gfx_load_image_texture(&gfx_state, VIRTUAL_TEXTURE_ID_SLING_SHOT);

    string_clear(&file_path);
    string_push_chars(&file_path, "assets/sprites/wood block.png", 29);
    gfx_virtual_texture_set_file_path(&gfx_state, file_path, VIRTUAL_TEXTURE_ID_WOOD_BLOCK);
    gfx_load_image_texture(&gfx_state, VIRTUAL_TEXTURE_ID_WOOD_BLOCK);

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

    i32 entity_amount = 2048;
    i32 physics_body_amount = 128;
    entity_manager = (EntityManager){0};
    i32 timeout_data_element_size = 512;
    entity_manager_init(&entity_manager, persistent, &gfx_state, entity_amount, physics_body_amount, timeout_data_element_size);
    
    // GenId entity_shape_gid = fizx_circle_rigid_alloc(&entity_manager.fizx_state, circle, transform_to_transform2d(shape_transform), FIZX_ShapeBehaviour_Dynamic, material, dynamic_body_gid, true);

    // floor entity.
    Transform2D floor_transform = TRANSFORM2D_IDENTITY;
    GenId level_gid = entity_spawn_level_root(&entity_manager, &gfx_state, (String){.chars = "level 0", .length = 7, .count = 7}, TRANSFORM2D_IDENTITY, 0);

    // floor_transform.scale = (Vector2){.x = 100.0f, .y = 1.0f};
    // entity_spawn_invisible_wall(&entity_manager, &gfx_state, (String){.chars = "floor", .length = 5, .count = 5}, floor_transform, level_gid);
    
    // GenId sling_shot_gid = entity_manager_alloc_entity(&entity_manager, level_gid);
    // entity_manager_get_entity(entity_manager, sling_shot_gid, &entity);
    // {
    //     entity->transform = TRANSFORM2D_IDENTITY;
    //     entity->transform.scale = vector2_mul_val(entity->transform.scale, 3.0f);
    //     entity->transform.position = (Vector2){.y = 2.0f};
    //     entity->sprite_depth = 10.0f;
    //     Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
    //     sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 10.0f);
    //     bool success = false;
    //     entity->sprite_id = gfx_sprite_alloc(&gfx_state, SPRITE_LAYER_WORLD, &success);
    //     GFX_SpriteRegion region = {.bot_right = {.x = 16, .y = 16}};
    //     gfx_sprite_init(
    //         &gfx_state, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
    //         GFX_SpriteOrigin_Center, 4, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
    //     );

    //     Transform2D bird_transform = TRANSFORM2D_IDENTITY;
    //     bird_transform.position = vector2_add(entity->transform.position, (Vector2){.y = 0.5f});
    //     bird_transform.scale = VECTOR2_ONE;

    //     entity_spawn_yellow_bird(&entity_manager, &gfx_state, bird_transform, level_gid);
    // }

    { // level load.
        string_clear(&file_path);
        string_push_chars(&file_path, "assets/saved.scsv", 17);
        load_lvl(&entity_manager, &gfx_state, file_path);
    }

    { // font render.

        // load font.
        string_clear(&file_path);
        string_push_chars(&file_path, "assets/fonts/PixelOperatorSC-Bold.ttf", 37);
        gfx_virtual_texture_set_file_path(&gfx_state, file_path, 1);
        gfx_load_font_texture(&gfx_state, transient, 1, 24, 32, 12);
    }

    // entity_manager_dealloc_entity(&entity_manager, level_gid);

    // entity_spawn_pig(&entity_manager, &gfx_state, (String){0}, TRANSFORM2D_IDENTITY, 0);
    
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
                fizx_state_fixed_update(&entity_manager.fizx_state, &collision_callback_ctx, FIXED_DELTA_TIME, 32);
                fixed_update_accumulator -= FIXED_DELTA_TIME;
            }
        }

        // update.
        {
            input_update();
            timer_manager_update(&entity_manager.timer_manager, delta_time);
            
            if(input_is_key_just_pressed(KEY_1)){
                fizx_draw_state.draw_bvh_branches = !fizx_draw_state.draw_bvh_branches;
            }
            if(input_is_key_just_pressed(KEY_2)){
                fizx_draw_state.draw_bvh_leaves = !fizx_draw_state.draw_bvh_leaves;
            }
            if(input_is_key_just_pressed(KEY_3)){
                fizx_draw_state.draw_body_shapes = !fizx_draw_state.draw_body_shapes;
            }
            if(input_is_key_just_pressed(KEY_F1)){
                game_state.is_editor_mode = !game_state.is_editor_mode; 
            }
            app_update(persistent, transient, delta_time);

        }

        // late update.
        {
            app_late_update(delta_time);
        }

        // final update.
        {
            Vector2I mouse_backbuffer_position;
            platform_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);
            fizx_state_draw(entity_manager.fizx_state, &gfx_state, fizx_draw_state, delta_time);
            entity_manager_debug_draw(entity_manager, &gfx_state, delta_time);
            gfx_clay_begin_layout(
                &gfx_state, (Vector2I){.x = WINDOW_WIDTH, .y = WINDOW_HEIGHT}, mouse_backbuffer_position, delta_time, 
                input_is_mouse_button_pressed(MOUSE_BUTTON_LEFT)
            );
            if(game_state.is_editor_mode){
                gfx_clay_test_layout(&entity_manager);
            }
            gfx_clay_end_layout(&gfx_state, delta_time, SPRITE_LAYER_UI, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
            gfx_state_draw(&gfx_state);
            transient->stride = 0;
        }
    }

    platform_window_context_free(window_ctx);
}