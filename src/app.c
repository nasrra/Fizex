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

#define WINDOW_WIDTH 1280 / 2
#define WINDOW_HEIGHT 720 / 2

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

/**====================
    functions
====================**//**/

void app_update(MemoryArena* persistent, MemoryArena* transient, f32 delta_time){
    Soa_Aabb soa = {0};
    soa_aabb_init(&soa, transient, 3);
    soa_aabb_push(&soa, -1.0f, -1.0f, 2.0f, 2.0f);
    soa_aabb_push(&soa, -1.0f, -1.0f, 3.0f, 3.0f);
    soa_aabb_push(&soa, -1.0f, -1.0f, 4.0f, 4.0f);

    f32* nums = (f32[]){2.0f, 1.0f, 9.0f, 11.0f, 16.0f, -33.3f, -120.0f, 99.0f};
    f32* num_0 = &nums[0];
    f32* num_1 = &nums[1];
    f32* num_2 = &nums[2];
    f32* num_3 = &nums[3];
    f32* num_4 = &nums[4];
    f32* num_5 = &nums[5];
    f32* num_6 = &nums[6];
    f32* num_7 = &nums[7];

    quicksort_f32_dsc(nums, 8);
    quicksort_f32_asc(nums, 8);

    Person* persons = (Person[]){
        (Person){.num = 99},
        (Person){.num = 2},
        (Person){.num = -32},
    };
    Person* person_0 = &persons[0];
    Person* person_1 = &persons[1];
    Person* person_2 = &persons[2];
    quicksort_person_asc(persons, 3);
    quicksort_person_dsc(persons, 3);


    f32* centroids_x;
    i32 centroids_x_size;
    f32* centroids_y;
    i32 centroids_y_size;

    MEMORY_ARENA_ALLOC_ARRAY(transient, centroids_x, &centroids_x_size, 3);
    MEMORY_ARENA_ALLOC_ARRAY(transient, centroids_y, &centroids_y_size, 3);
    soa_aabb_calculate_centroids(&soa, centroids_x, centroids_y);

    i32 i_a = rand_i32();
    i32 i_b = rand_i32();
    i32 i_c = rand_i32();
    i32 i_d = rand_i32();
    i32 i_e = rand_i32();
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
    sprite_layer_create_infos[0] = (SpriteLayerCreateInfo){.max_sprites = 256};
    BOUNDS_CHECK(1, sprite_layer_create_infos_length);
    sprite_layer_create_infos[1] = (SpriteLayerCreateInfo){.max_sprites = 256};

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
    platform_init_persistent_memory(MEGABYTE(144));
    platform_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = platform_get_persistent_memory();
    MemoryArena* transient = platform_get_transient_memory();
    window_ctx = platform_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);

    renderer_orthographic_camera_init(&world_camera, (Vector3){.z = -4.0f}, 0.01f, 100.0f, 12.0f);
    renderer_global_wireframe_thickness = 0.05f;
    renderer_ctx = app_renderer_init(persistent, transient, *window_ctx);

    FIZXState fizx_state = {0};
    fizx_state_init(&fizx_state, persistent, 5, 4);
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
        .draw_bvh_leaves = true,
        .draw_bvh_branches = false
    };

    Transform dynamic_body_transform = {.position = {.x = 0.1f, .y = 5.0f}, .scale = VECTOR3_ONE};
    Transform kinematic_body_transform = {.scale = vector3_mul_val(VECTOR3_ONE, 3.0f), .rotation = quaternion_create_from_axis_angle(VECTOR3_FORWARD, 45.0f)};
    Transform shape_transform = {.scale = VECTOR3_ONE};
    Rectangle shape = {.x = -0.0f, .y = 0.0f, .width = 1.0f, .height = 1.0f};
    Material material = {.static_friction = 1.0f, .kinetic_friction = 1.0f, .density = 5.0f, .restitution = 0.5f};

    GenId dynamic_body_gid = body_alloc(&fizx_state, transform_to_transform2d(dynamic_body_transform), true);
    GenId dynamic_shape_gid = fizx_rectangle_rigid_alloc(&fizx_state, shape, transform_to_transform2d(shape_transform), ShapeBehaviour_Dynamic, dynamic_body_gid, material, true);

    GenId kinematic_body_gid = body_alloc(&fizx_state, transform_to_transform2d(kinematic_body_transform), false);
    GenId kinematic_shape_gid = fizx_rectangle_rigid_alloc(&fizx_state, shape, transform_to_transform2d(shape_transform), ShapeBehaviour_Kinematic, kinematic_body_gid, material, true);

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
                fizx_state_fixed_update(&fizx_state, FIXED_DELTA_TIME, 16);
                fixed_update_accumulator -= FIXED_DELTA_TIME;
            }
        }

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
            fizx_state_draw(fizx_state, &renderer_ctx, fizx_draw_state, delta_time);
            renderer_draw_renderer(&renderer_ctx);
            platform_window_update(window_ctx);
            transient->stride = 0;
        }
        platform_output_message("update! \n");
    }

    platform_window_context_free(window_ctx);
}