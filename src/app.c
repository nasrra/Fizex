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
#include "gameplay/editor.c"

/**====================
    defines.
====================**//**/

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

/**====================
    functions
====================**//**/

void app_main(){

    /**
        memory allocation.
    **/
    platform_init_persistent_memory(MEGABYTE(500));
    platform_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = platform_get_persistent_memory();
    MemoryArena* transient = platform_get_transient_memory();

    input_init(persistent);
    
    WindowContext* window_ctx = platform_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);
    GFX_State gfx_state = {0};
    { // init gfx

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
        MEMORY_ARENA_ALLOC_ARRAY(transient, sprite_layer_create_infos, &sprite_layer_create_infos_length, 3);
    
        BOUNDS_CHECK(0, sprite_layer_create_infos_length);
        sprite_layer_create_infos[0] = (GFX_SpriteLayerCreateInfo){.max_sprites = 512};
        BOUNDS_CHECK(1, sprite_layer_create_infos_length);
        sprite_layer_create_infos[1] = (GFX_SpriteLayerCreateInfo){.max_sprites = 512};
        BOUNDS_CHECK(1, sprite_layer_create_infos_length);
        sprite_layer_create_infos[2] = (GFX_SpriteLayerCreateInfo){.max_sprites = 512};
    
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
    
        gfx_state_init(&gfx_state, gfx_init_info, persistent, transient, window_ctx);
        gfx_orthographic_camera_init(&gfx_state.world_camera, GFX_CoordinateSpace_Cartesian, (Vector3){.y = 5.0f, .z = -4.0f}, 0.01f, 100.0f, 22.0f);
        gfx_orthographic_camera_init(&gfx_state.screen_camera, GFX_CoordinateSpace_Rasterised, (Vector3){.z = -1.0f}, 0.01f, 1028.0f, 1080.0f);
        gfx_global_wireframe_thickness = 0.05f;
        gfx_state.clay_game_ui_ctx = gfx_clay_create_context(MEGABYTE(8), WINDOW_WIDTH, WINDOW_HEIGHT);
        gfx_state.clay_editor_ui_ctx = gfx_clay_create_context(MEGABYTE(8), WINDOW_WIDTH, WINDOW_HEIGHT);
    }
    
    // create game state.
    GameState game_state = {0};
    game_state_init(&game_state, persistent, transient, &gfx_state);

    // create editor state.
    EditorState editor_state = {0};
    editor_state_init(&editor_state, &gfx_state);

    
    { // load assets.

        String file_path = {0};
        string_init(&file_path, transient, 48);

        { // load textures.

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

            string_clear(&file_path);
            string_push_chars(&file_path, "assets/fonts/PixelOperatorSC-Bold.ttf", 37);
            gfx_virtual_texture_set_file_path(&gfx_state, file_path, 1);
            gfx_load_font_texture(&gfx_state, transient, 1, 24, 32, 12);
    
        }    
        
        { // load level.
            string_clear(&file_path);
            string_push_chars(&file_path, "assets/saved.scsv", 17);
            load_lvl(&game_state.entity_manager, file_path);    
        }
        
    }
        
    // floor entity.
    Transform2D floor_transform = TRANSFORM2D_IDENTITY;
    GenId level_gid = entity_spawn_level_root(&game_state.entity_manager, (String){.chars = "level 0", .length = 7, .count = 7}, TRANSFORM2D_IDENTITY, 0);
        
        
    bool in_editor_mode = false;
    
    { // update loop
    
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

            { // app begin update.
                platform_window_update(window_ctx);
                input_update();
                if(input_is_key_just_pressed(KEY_F1)){
                    in_editor_mode = !in_editor_mode; 
                }
            }
                
            { // game update.
            
                game_state_update(&game_state, persistent, transient, delta_time, in_editor_mode);
                game_state_late_update(&game_state, delta_time);            
                game_state_draw(&game_state, delta_time);
            }
            
            if(in_editor_mode)
            { // editor update.

                editor_state_update(&editor_state, &game_state, transient, delta_time);
            }
            
            { // app end update.
                gfx_state_draw(&gfx_state);
                transient->stride = 0;        
            }
        }
    }
    
    { // clean up.
    
        platform_window_context_free(window_ctx);
    }
}