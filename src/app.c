#include "platform.h"

#include "base_layer/base.h"
#include "base_layer/base_simd.c"
#include "base_layer/base_math.c"
#include "input/input.c"
#include "renderer/renderer.c"
#include "renderer/renderer_app_types.c"

typedef struct{
    i32 num;
} Person;

DEFINE_QUICKSORT_STRUCT(Person, i32, .num, quicksort_person);

RendererContext renderer_ctx;
MemoryArena renderer_memory;

GenId a;
GenId b;

/**====================
    globals
====================**//**/

Camera world_camera;
Camera screen_camera;
WindowContext* window_ctx;

/**====================
    functions
====================**//**/

void app_update(MemoryArena* persistent, MemoryArena* transient){

    Soa_Aabb soa = {0};
    init_soa_aabb(&soa, transient, 3);
    append_soa_aabb(&soa, -1.0f, -1.0f, 2.0f, 2.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 3.0f, 3.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 4.0f, 4.0f);

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
    calculate_centroids_soa_aabb(&soa, centroids_x, centroids_y);

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
    
    renderer_renderer_ctx_init(&renderer_ctx, renderer_init_info, persistent, transient, window_ctx, 1920, 1080);
    return renderer_ctx;
}

void app_late_update(){

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

    renderer_draw_line(&renderer_ctx, COLOUR_BLUE, (Vector3){0}, (Vector3){.x = 2}, SPRITE_LAYER_WORLD, SPRITE_MATERIAL_DEBUG, 0.01f);
}

void app_main(){
    
    /**
        memory allocation.
    **/
    platform_init_persistent_memory(MEGABYTE(144));
    platform_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = platform_get_persistent_memory();
    MemoryArena* transient = platform_get_transient_memory();
    Person person = (Person){.num = 256};
    window_ctx = platform_window_create(1920, 1080);
    
    renderer_ctx = app_renderer_init(persistent, transient, *window_ctx);

    renderer_orthographic_camera_init(&world_camera, (Vector3){.z = -4.0f}, 0.01f, 100.0f, 10.0f);
    // renderer_perspective_camera_init(&world_camera, (Vector3){.z = -5.0f}, 0.01f, 1024.0f, to_radians(45.0f));
    // CameraSystem.InitPerspectiveCamera(ref CameraSystem.GetCamera(CameraId.World), new(){Z = -5}, 0.01f, 1024, Math.ToRadians(45));

    bool success = false;
    SpriteId sprite_id = renderer_sprite_alloc(&renderer_ctx, SPRITE_LAYER_WORLD, &success);
    Transform transform = TRANSFORM_IDENTITY;
    transform.scale = (Vector3){.x = 1, .y = 1, .z = 100};
    // renderer_sprite_init(&renderer_ctx, sprite_id, matrix4x4_from_transform(transform), COLOUR_ORANGE, (SpriteRegion){0}, ColourState_Override, 1, SPRITE_MATERIAL_DEBUG, true);

    while(!window_ctx->is_destroyed){
        app_update(persistent, transient);
        app_late_update();
        renderer_draw_renderer(&renderer_ctx);
        platform_window_update(window_ctx);
        transient->stride = 0;
    }

    platform_window_context_free(window_ctx);
}

