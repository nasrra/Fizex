#include "platform.h"

#ifdef NDEBUG
#include <stdio.h>
#endif

#include "base_layer/base.h"
#include "base_layer/base_cpu.c"
#include "base_layer/base_math.c"
#include "base_layer/base_algorithms.c"
#include "base_layer/base_structures.c"
#include "input/input.c"
#include "renderer/renderer.c"
#include "renderer/renderer_app_types.c"
#include "fizx/fizx.c"
#include "fizx/fizx_draw.c"
#include "gameplay/entity.c"

/**====================
    types.
====================**//**/

typedef struct{
    i32 num;
    char foo;
} Person;

/**====================
    defines.
====================**//**/

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

DEFINE_QUICKSORT_STRUCT(Person, i32, .num, quicksort_person);

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

Camera world_camera;
Camera screen_camera;
WindowContext* window_ctx;
RendererContext renderer_ctx;
MemoryArena renderer_memory;
EntityManager entity_manager;

/**====================
    functions
====================**//**/

void trigger_on_enter_callback(CollisionInfo info, void* user_data){
    platform_output_message("enter!\n");
}

void trigger_on_exit_callback(CollisionInfo info, void* user_data){
    platform_output_message("exit!\n");
}

void trigger_on_sustain_callback(CollisionInfo info, void* user_data){
    platform_output_message("sustain!\n");
}

void app_update(MemoryArena* persistent, MemoryArena* transient, f32 delta_time){
    entity_manager_update(&entity_manager, &renderer_ctx, delta_time);
}

RendererContext app_renderer_init(MemoryArena* persistent, MemoryArena* transient, WindowContext window_ctx){

    memory_arena_partition(persistent, &renderer_memory, MEGABYTE(2));

    /**
        font textures.
    **/
    FontTextureInitInfo font_texture_init_info ={
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
    ImageTexturesInitInfo* image_textures_init_info;
    i32 image_textures_init_info_length;
    MEMORY_ARENA_ALLOC_ARRAY(transient, image_textures_init_info, &image_textures_init_info_length, 3);

    BOUNDS_CHECK(0, image_textures_init_info_length);
    image_textures_init_info[0] = (ImageTexturesInitInfo){.width = 512, .height = 512, .max_textures = 16};
    BOUNDS_CHECK(1, image_textures_init_info_length);
    image_textures_init_info[1] = (ImageTexturesInitInfo){.width = 360, .height = 162, .max_textures = 2};
    BOUNDS_CHECK(2, image_textures_init_info_length);
    image_textures_init_info[2] = (ImageTexturesInitInfo){.width = 640, .height = 360, .max_textures = 2};

    /**
        sprite layers.
    **/
    SpriteLayerCreateInfo* sprite_layer_create_infos;
    i32 sprite_layer_create_infos_length;
    MEMORY_ARENA_ALLOC_ARRAY(transient, sprite_layer_create_infos, &sprite_layer_create_infos_length, 2);

    BOUNDS_CHECK(0, sprite_layer_create_infos_length);
    sprite_layer_create_infos[0] = (SpriteLayerCreateInfo){.max_sprites = 512};
    BOUNDS_CHECK(1, sprite_layer_create_infos_length);
    sprite_layer_create_infos[1] = (SpriteLayerCreateInfo){.max_sprites = 512};

    /**
        context.
    **/
    RendererContextInitInfo renderer_init_info = {
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

    renderer_renderer_ctx_init(&renderer_ctx, renderer_init_info, persistent, transient, window_ctx, WINDOW_WIDTH, WINDOW_HEIGHT);
    return renderer_ctx;
}

void app_fixed_update(f32 delta_time){

}

void app_late_update(f32 delta_time){

    /**
        NOTE:
        this might have to be swapped for the final render target resolution, maybe idk.
    **/
    f32 aspect_ratio = platform_window_calc_aspect_ratio(*window_ctx);
    renderer_camera_update_projection_matrix(&world_camera, aspect_ratio);

    Ubo ubo = {
        .world_camera_matrix = matrix4x4_mul(matrix4x4_mul(world_camera.projection, world_camera.view), world_camera.model),
        .screen_camera_matrix = matrix4x4_mul(matrix4x4_mul(world_camera.projection, world_camera.view), world_camera.model),
        .world_camera_far_z = world_camera.far_z,
        .world_camera_near_z = world_camera.near_z,
    };

    renderer_write_to_user_uniform_buffer(&renderer_ctx, &ubo, sizeof(Ubo));
}

void app_main(){

    /**
        memory allocation.
    **/
    platform_init_persistent_memory(MEGABYTE(500));
    platform_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = platform_get_persistent_memory();
    MemoryArena* transient = platform_get_transient_memory();

    String file_path = {0};
    string_init(&file_path, transient, 20);
    string_push_chars(&file_path, "assets/image.png", 16);

    window_ctx = platform_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);

    renderer_orthographic_camera_init(&world_camera, (Vector3){.z = -4.0f}, 0.01f, 100.0f, 44.0f);
    renderer_global_wireframe_thickness = 0.05f;
    renderer_ctx = app_renderer_init(persistent, transient, *window_ctx);
    renderer_virtual_texture_set_file_path(&renderer_ctx, file_path, 3);
    renderer_load_image_texture(&renderer_ctx, 3);

    FIZXDrawInfo fizx_draw_state = {
        .colour_dynamic_shape           = COLOUR_GREEN,
        .colour_passive_trigger_shape   = COLOUR_LIGHT_BLUE,
        .colour_kinematic_shape         = COLOUR_ORANGE,
        .colour_active_trigger_shape    = COLOUR_RED,
        .colour_aabb                    = COLOUR_LIGHT_BLUE,
        .colour_fallback_shape          = COLOUR_WHITE,
        .colour_inactive_physics_body   = COLOUR_BLACK,
        .colour_bvh_leaf                = COLOUR_WHITE,
        .colour_bvh_branch              = COLOUR_LIGHT_GREEN,
        .colour_contact_point           = COLOUR_RED,
        .colour_linear_velocity         = COLOUR_WHITE,
        .colour_global_position         = COLOUR_WHITE,
        .colour_centroid                = COLOUR_YELLOW,
        .colour_collision_other         = COLOUR_BLUE,
        .colour_collision_normal        = COLOUR_LIGHT_BLUE,
        .colour_center_of_mass          = COLOUR_ORANGE,
        .sprite_layer                   = SPRITE_LAYER_WORLD,
        .wireframe_thickness            = 0.005f,
        .material_idx                   = SPRITE_MATERIAL_DEBUG,
        .draw_body_shapes               = true,
        // .draw_bvh_leaves = true,
        .draw_bvh_branches = true,
        .draw_collision_info = true
    };

    Transform shape_transform = {.scale = VECTOR3_ONE};
    Rectangle shape = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
    Circle circle = {.x = 0.0f, .y = 0.0f, .radius = 1.0f};
    Material material = {.static_friction = 1.0f, .kinetic_friction = 1.0f, .density = 5.0f, .restitution = 0.5f};

    i32 entity_amount = 2048;
    i32 physics_body_amount = 128;
    entity_manager = (EntityManager){0};
    entity_manager_init(&entity_manager, persistent, entity_amount, physics_body_amount);
    GenId e = entity_manager_alloc_entity(&entity_manager);
    Entity* entity;
    entity_manager_get_entity(entity_manager, e, &entity);

    Transform trigger_body_transform = {.position = {.x = -10.0f, .y = 1.0f}, .scale = vector3_mul_val(VECTOR3_ONE, 3.0f), .rotation = quaternion_create_from_axis_angle(VECTOR3_FORWARD, 30.0f)};
    GenId trigger_body_gid = fizx_body_alloc(&entity_manager.fizx_state, transform_to_transform2d(trigger_body_transform), false);
    GenId trigger_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager.fizx_state, shape, transform_to_transform2d(shape_transform), ShapeBehaviour_Trigger, trigger_body_gid, material, false);
    // GenId trigger_shape_gid = fizx_circle_rigid_alloc(&entity_manager.fizx_state, circle, transform_to_transform2d(shape_transform), ShapeBehaviour_Kinematic, material, trigger_body_gid, false);
    shape_set_on_enter_callback(&entity_manager.fizx_state, trigger_on_enter_callback, trigger_shape_gid);
    shape_set_on_sustain_callback(&entity_manager.fizx_state, trigger_on_sustain_callback, trigger_shape_gid);
    shape_set_on_exit_callback(&entity_manager.fizx_state, trigger_on_exit_callback, trigger_shape_gid);
    
    Transform dynamic_body_transform = {.position = {.x = 0.1f, .y = 10.0f}, .scale = VECTOR3_ONE};
    entity->physics_body_gid = fizx_body_alloc(&entity_manager.fizx_state, transform_to_transform2d(dynamic_body_transform), true);
    GenId dynamic_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager.fizx_state, shape, transform_to_transform2d(shape_transform), ShapeBehaviour_Dynamic, entity->physics_body_gid, material, true);
    // GenId dynamic_shape_gid = fizx_circle_rigid_alloc(&entity_manager.fizx_state, circle, transform_to_transform2d(shape_transform), ShapeBehaviour_Dynamic, material, dynamic_body_gid, true);
    
    Transform kinematic_body_transform = {.position = {.y = -12.0f}, .scale = vector3_mul_val(VECTOR3_ONE, 3.0f), .rotation = QUATERNION_IDENTITY};
    GenId kinematic_body_gid = fizx_body_alloc(&entity_manager.fizx_state, transform_to_transform2d(kinematic_body_transform), false);
    GenId kinematic_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager.fizx_state, shape, transform_to_transform2d(shape_transform), ShapeBehaviour_Kinematic, kinematic_body_gid, material, false);
    
    Transform sprite_transform = {.position = {.x = 0.1f, .y = 0.0f, .z = 12.0f}, .scale = vector3_mul_val(VECTOR3_ONE, 10.0f)};
    bool success = false;
    entity->sprite_id = renderer_sprite_alloc(&renderer_ctx, SPRITE_LAYER_WORLD, &success);
    renderer_sprite_init(
        &renderer_ctx, entity->sprite_id, transform_to_matrix4x4(sprite_transform), COLOUR_WHITE, (SpriteRegion){.width = 512, .height = 512}, ColourState_Tint,
        3, SPRITE_MATERIAL_IMAGE, true
    );

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

        printf("%.5f", delta_time);

        // fixed update.
        {
            fixed_update_accumulator += delta_time;
            if(fixed_update_accumulator > DELTA_TIME_ACCUMULATOR_SLOW_DOWN){
                fixed_update_accumulator = DELTA_TIME_ACCUMULATOR_SLOW_DOWN;
            }

            while(fixed_update_accumulator >= FIXED_DELTA_TIME){
                app_fixed_update(FIXED_DELTA_TIME);
                fizx_state_fixed_update(&entity_manager.fizx_state, NULL, FIXED_DELTA_TIME, 16);
                fixed_update_accumulator -= FIXED_DELTA_TIME;
            }
        }

        fizx_state_draw(entity_manager.fizx_state, &renderer_ctx, fizx_draw_state, delta_time);
        // update.
        {
            app_update(persistent, transient, delta_time);
        }

        // late update.
        {
            app_late_update(delta_time);
        }

        // final update.
        {
            renderer_draw_renderer(&renderer_ctx);
            platform_window_update(window_ctx);
            transient->stride = 0;
        }
    }

    platform_window_context_free(window_ctx);
}