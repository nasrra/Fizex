#include "os.h"
#include "base_layer/base.h"
#include "base_layer/base_cpu.c"
#include "base_layer/base_math.c"
#include "base_layer/base_algorithms.c"
#include "base_layer/base_structures.c"
#include "input/input.h"
#include "renderer/gfx.c"
#include "fizx/fizx.c"
#include "fizx/fizx_draw.c"
#include "game/gameplay.c"
#include "game/editor.c"

/**====================
    defines.
====================**//**/

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define GFX_DEFAULT_WIREFRAME_THICKNESS 0.05f


///
/// @ functions
///


void app_main(){

    /**
        memory allocation.
    **/
    os_init_persistent_memory(MEGABYTE(500));
    os_init_transient_memory(MEGABYTE(4));
    MemoryArena* persistent = os_get_persistent_memory();
    MemoryArena* transient = os_get_transient_memory();

    IntrusiveList intrsuive_list = {0};
    intrusive_list_init(&intrsuive_list, transient, 16, false);
    intrusive_list_add_branch(&intrsuive_list, 1, 0);
    intrusive_list_add_branch(&intrsuive_list, 2, 1);
    intrusive_list_add_branch(&intrsuive_list, 3, 2);
    intrusive_list_add_branch(&intrsuive_list, 4, 0);
    intrusive_list_add_branch(&intrsuive_list, 5, 4);
    intrusive_list_add_branch(&intrsuive_list, 6, 4);
    intrusive_list_set_node_parent(&intrsuive_list, 4, 1);

    f32 latest_active_key_print_delay = 0.25f;
    f32 latest_active_key_print_rate = 0.05f;
    input_init(persistent, latest_active_key_print_delay, latest_active_key_print_rate);

    WindowContext* window_ctx = os_window_create(WINDOW_WIDTH, WINDOW_HEIGHT);
    GFX_State gfx_state = {0};
    { // init gfx

        // font textures.
        GFX_FontTextureInitInfo font_texture_init_info ={
            .max_glyphs = 256,
            .texture_height = 512,
            .texture_width = 512
        };
        MEMORY_ARENA_ALLOC_ARRAY(transient, font_texture_init_info.virtual_textures, &font_texture_init_info.virtual_textures_length, 2);
        for(i32 i = 0; i < font_texture_init_info.virtual_textures_length; i++){
            font_texture_init_info.virtual_textures[i] = i+1;
        }

        // image textures.
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
        image_textures_init_info[3] = (GFX_ImageTexturesInitInfo){.width = 1028, .height = 1028, .max_textures = 3};
        BOUNDS_CHECK(4, image_textures_init_info_length);
        image_textures_init_info[4] = (GFX_ImageTexturesInitInfo){.width = 2048, .height = 2048, .max_textures = 4};

        // context.
        GFX_StateInitInfo gfx_init_info = {
            .max_file_path_length = os_get_max_file_path_length(),
            .max_user_uniform_buffer_size_in_bytes = sizeof(Ubo),
            .max_user_storage_buffer_size_in_bytes = 4, // this should be 4 when not used for some reason idk.
            .max_virtual_textures = 1024,
            .font_textures_init_info = font_texture_init_info,
            .image_textures_init_infos = image_textures_init_info,
            .image_textures_init_infos_length = image_textures_init_info_length,
            .final_render_texture_width = WINDOW_WIDTH,
            .final_render_texture_height = WINDOW_HEIGHT,
            .graphics_pipeline_shader_file_path = STRING_FROM_LITERAL("../assets/shader.wgsl", transient),
            .max_sprites = 1024,
            .draw_wireframe_thickness = GFX_DEFAULT_WIREFRAME_THICKNESS,
            .draw_circle_vertex_count = 24,
            .draw_arrow_prong_max_length = 0.5f
        };

        gfx_state_init(&gfx_state, gfx_init_info, persistent, transient, window_ctx);
        gfx_orthographic_camera_init(
            &gfx_state.world_camera, 
            WORLD_CAMERA_COORDINATE_SPACE, 
            CAMERA_POSITIONAL_OFFSET, 
            WORLD_CAMERA_DEFAULT_NEAR_Z, 
            WORLD_CAMERA_DEFAULT_FAR_Z, 
            WORLD_CAMERA_DEFAULT_ORTHOGRAPHIC_SIZE
        );
        
        gfx_orthographic_camera_init(
            &gfx_state.screen_camera, 
            UI_CAMERA_COORDINATE_SPACE, 
            CAMERA_POSITIONAL_OFFSET, 
            UI_CAMERA_DEFAULT_NEAR_Z,
            UI_CAMERA_DEFAULT_FAR_Z,
            WINDOW_HEIGHT
        );
        
        gfx_state.clay_game_ui_ctx = gfx_clay_create_context(MEGABYTE(8), WINDOW_WIDTH, WINDOW_HEIGHT);
        gfx_state.clay_editor_ui_ctx = gfx_clay_create_context(MEGABYTE(8), WINDOW_WIDTH, WINDOW_HEIGHT);
    }

    // create game state.
    GameState game_state = {0};
    game_state_init(&game_state, persistent, transient, &gfx_state);

    // create editor state.
    Editor_State editor_state = {0};
    editor_state_init(&editor_state, persistent, &gfx_state, game_state.entity_manager.entity_length);

    { // load assets.

        String file_path = {0};
        string_init(&file_path, transient, os_get_max_file_path_length());

        { // load textures.
            string_clear(&file_path);
            STRING_PUSH_LITERAL(&file_path, "../assets/sprites/GameBoard.png");
            gfx_virtual_texture_set_file_path(&gfx_state, file_path, VIRTUAL_TEXTURE_ID_GAME_BOARD);
            gfx_load_image_texture(&gfx_state, VIRTUAL_TEXTURE_ID_GAME_BOARD);

            string_clear(&file_path);
            STRING_PUSH_LITERAL(&file_path, "../assets/fonts/PixelOperatorSC-Bold.ttf");
            gfx_virtual_texture_set_file_path(&gfx_state, file_path, VIRTUAL_TEXTURE_ID_FONT);
            gfx_load_font_texture(&gfx_state, transient, VIRTUAL_TEXTURE_ID_FONT, 24, 32, 12);
        }

        { // load level.
            // string_clear(&file_path);
            // string_push_chars(&file_path,    "../assets/levels/level_001.scsv", 31);
            // LoadBoardLevelCallbackContext load_board_level_callback_ctx = {
            //     .game_state = &game_state
            // };
            
            // Transform2D transform = TRANSFORM2D_IDENTITY;
            // transform.scale.x = 1.2f;
            
            // DeferredLevelLoadContext dll_ctx = {
            //     .transform = transform,
            //     .file_path = file_path,
            //     .load_type = LevelLoadType_World,
            //     .callback = load_board_level_callback,
            //     .callback_user_data = &load_board_level_callback_ctx
            // };
        }
    }    

    bool was_in_editor_mode = false;
    bool in_editor_mode = false;
    Key last_editor_key_pressed = Key_None;

    { // update_loop();

        u128 prev_process_tick_in_mili  = 0;
        f32 previous_time_in_seconds    = 0.0f;
        f32 fixed_update_accumulator    = 0.0f;
        while(!window_ctx->is_destroyed){
            u128 process_tick = os_get_proccess_tick();
            // micro to mili.
            u128 process_tick_in_mili = process_tick / 1000;
            u128 delta_tick_in_mili = process_tick_in_mili - prev_process_tick_in_mili;
            // mili to seconds.
            f32 delta_time = (f32)delta_tick_in_mili * 0.0001f;
            prev_process_tick_in_mili = process_tick_in_mili;

            { // app begin update.
                os_window_update(window_ctx);
                input_update(delta_time);
                if(input_is_key_just_pressed(Key_F1)){
                    if(last_editor_key_pressed == Key_F1){
                        in_editor_mode = false;
                        last_editor_key_pressed = Key_None;
                    }
                    else{
                        in_editor_mode = true;
                        editor_state.menu = Editor_Menu_LevelManagement;
                        last_editor_key_pressed = Key_F1;
                    }
                }
                else if(input_is_key_just_pressed(Key_F2)){
                    if(last_editor_key_pressed == Key_F2){
                        in_editor_mode = false;
                        last_editor_key_pressed = Key_None;
                    }
                    else{
                        in_editor_mode = true;
                        editor_state.menu = Editor_Menu_NativeEntitySpawner;
                        last_editor_key_pressed = Key_F2;
                    }
                }
                else if(input_is_key_just_pressed(Key_F3)){
                    if(last_editor_key_pressed == Key_F3){
                        in_editor_mode = false;
                        last_editor_key_pressed = Key_None;
                    }
                    else{
                        in_editor_mode = true;
                        editor_state.menu = Editor_Menu_AssetEntitySpawner;
                        last_editor_key_pressed = Key_F3;
                    }
                }
                else if(input_is_key_just_pressed(Key_F4)){
                    if(last_editor_key_pressed == Key_F4){
                        in_editor_mode = false;
                        last_editor_key_pressed = Key_None;
                    }
                    else{
                        in_editor_mode = true;
                        editor_state.menu = Editor_Menu_EntityInspector;
                        last_editor_key_pressed = Key_F4;
                    }
                }
                else if(input_is_key_just_pressed(Key_F5)){
                    if(last_editor_key_pressed == Key_F5){
                        in_editor_mode = false;
                        last_editor_key_pressed = Key_None;
                    }
                    else{
                        in_editor_mode = true;
                        editor_state.menu = Editor_Menu_EntityTreeView;
                        last_editor_key_pressed = Key_F5;
                    }
                }
            }

            { // game update.

                game_state_preupdate(&game_state, delta_time, transient);
                game_state_update(&game_state, persistent, transient, delta_time, in_editor_mode);
                game_state_late_update(&game_state, delta_time, in_editor_mode);
            }


            { // app end update.
            
                { // choose_camera();
                
                    EntityManager* entity_manager = &game_state.entity_manager;
                    
                    // hoisting invariance.
                    if(in_editor_mode){
                        
                        // when first entering the editor state, the editor camera should 
                        // mimic the game camera position and scale.
                        if(!was_in_editor_mode){
                            Entity* world_camera_entity;
                            if(entity_manager_get_world_camera(*entity_manager, &world_camera_entity)){
                                editor_state.world_camera_position            = world_camera_entity->global_transform.position;
                                editor_state.world_camera_orthographic_size   = world_camera_entity->camera_orthographic_size;
                            }
                        }
                        
                        gfx_orthographic_camera_init(
                            &gfx_state.world_camera, 
                            EDITOR_WORLD_CAMERA_COORDINATE_SPACE,
                            vector3_add(
                                vector2_to_vector3(editor_state.world_camera_position), 
                                CAMERA_POSITIONAL_OFFSET
                            ), 
                            EDITOR_WORLD_CAMERA_NEAR_Z, 
                            EDITOR_WORLD_CAMERA_FAR_Z, 
                            editor_state.world_camera_orthographic_size
                        );
                    }
                    else{                        
                        Entity* world_camera_entity;
                        if(entity_manager_get_world_camera(game_state.entity_manager, &world_camera_entity)){
                            
                            Vector2 scale = world_camera_entity->global_transform.scale;
                            gfx_orthographic_camera_init(
                                &gfx_state.world_camera, 
                                WORLD_CAMERA_COORDINATE_SPACE, 
                                vector3_add(
                                    vector2_to_vector3(world_camera_entity->global_transform.position), 
                                    CAMERA_POSITIONAL_OFFSET
                                ), 
                                world_camera_entity->camera_near_z, 
                                world_camera_entity->camera_far_z, 
                                world_camera_entity->camera_orthographic_size
                            );

                        }                        
                    }
                    
                    { // uploading_ubo();

                        /**
                            NOTE:
                            this might have to be swapped for the final render target resolution, maybe idk.
                        **/
                        f32 aspect_ratio = os_window_calc_aspect_ratio(*gfx_state.window_ctx);
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
                            .world_camera_far_z     = gfx_state.world_camera.far_z,
                            .world_camera_near_z    = gfx_state.world_camera.near_z,            
                        };
                       
                        gfx_write_to_user_uniform_buffer(&gfx_state, &ubo, sizeof(Ubo));
                    
                    }
                }
            }

            if(in_editor_mode)
            { // editor update.

                editor_state_update(&editor_state, &game_state, transient, delta_time);
            }

            { // draw();
            
                game_state_draw(&game_state, transient, delta_time, in_editor_mode);
                gfx_state_draw(&gfx_state);
            }
            
            // prepare_for_next_frame();
            {
                transient->stride = 0;
                was_in_editor_mode = in_editor_mode;
            }
        }
    }

    { // clean up.

        os_window_context_free(window_ctx);
    }
}