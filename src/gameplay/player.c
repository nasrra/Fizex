typedef struct{
    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
} GameMouseState;

typedef struct{
    // the entity_id of the entity to spawn. 
    EntityTypeId entity_to_spawn;    
    GenId entity_parent_gid;
} EditorMouseState;

typedef struct{
    bool is_editor_mode;
    EditorMouseState editor_mouse_state;
    GameMouseState mouse_state;
} GameState;
GameState game_state;

#define PLAYER_MOUSE_MAX_DRAW_RADIUS 2.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.5f

#define LEVEL_FILE_LINE_LENGTH 512 

void player_update(EntityManager* entity_manager, GFX_State* gfx_state, Vector2 mouse_world_position, f32 delta_time){
    
    if(!game_state.is_editor_mode){
        if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
            for(i32 e_idx = 0; e_idx < entity_manager->entity_length; e_idx++){
                Entity* entity = &entity_manager->entity[e_idx];
                Aabb world_aabb = aabb_translate(entity->clickable_aabb, entity->transform.position);
                if(aabb_overlaps_point(world_aabb, mouse_world_position)){
                    platform_output_message("clicked entity\n");
                    if(entity->is_physics_body){
                        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);
                    }
                    game_state.mouse_state.clicked_entity_initial_position = entity->transform.position;
                    game_state.mouse_state.clicked_entity_idx = e_idx;
                }
            }
        }
        
        Vector2 impulse_magnitude;
        
        if(game_state.mouse_state.clicked_entity_idx){
            Entity* entity = &entity_manager->entity[game_state.mouse_state.clicked_entity_idx];
            Vector2 position_diff = vector2_sub(game_state.mouse_state.clicked_entity_initial_position, mouse_world_position);
            
            position_diff = vector2_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
            impulse_magnitude = vector2_mul_val(position_diff, PLAYER_MOUSE_LAUNCH_FORCE); 
            
            Vector2 new_position = vector2_sub(game_state.mouse_state.clicked_entity_initial_position, position_diff);
                
            entity->transform.position = new_position;
            fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, new_position);
        }
        
        if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
            if(game_state.mouse_state.clicked_entity_idx > 0){
                Entity* entity = &entity_manager->entity[game_state.mouse_state.clicked_entity_idx];
                fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
                game_state.mouse_state.clicked_entity_idx = 0;
                fizx_body_clear_forces_and_velocities(&entity_manager->fizx_state, entity->physics_body_gid);
    
                fizx_body_impulse_force(&entity_manager->fizx_state,impulse_magnitude, entity->physics_body_gid);
            }
        }
    }
    else{
        if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
            Transform2D spawn_transform = TRANSFORM2D_IDENTITY;
            spawn_transform.position = mouse_world_position;
            
            if(game_state.editor_mouse_state.entity_parent_gid == 0){
                return;
            }
            
            switch(game_state.editor_mouse_state.entity_to_spawn){
                case EntityTypeId_RedBird:{
                    entity_spawn_red_bird(entity_manager, gfx_state, (String){.chars = "spawned red bird", .length = 16, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_YellowBird:{
                    entity_spawn_yellow_bird(entity_manager, gfx_state, (String){.chars = "spawned yellow bird", .length = 19, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_WoodBlock:{
                    entity_spawn_wood_block(entity_manager, gfx_state,  (String){.chars = "spawned wood block", .length = 18, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
                case EntityTypeId_LevelRoot:{
                    entity_spawn_level_root(entity_manager, gfx_state,  (String){.chars = "spawned level", .length = 13, .count = 16}, spawn_transform, game_state.editor_mouse_state.entity_parent_gid);
                }break;
            }
        }
    }
}

void editor_select_red_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_RedBird;
    }
}

void editor_select_yellow_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_YellowBird;
    }
}

void editor_select_wood_block_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = EntityTypeId_WoodBlock;
    }
}

void editor_on_hover_select_level_button(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_parent_gid = *((GenId*)user_data);
    }
}

void editor_save_level_entity_recurssive(
    EntityManager* entity_manager, 
    String file_path,
    String* entity_char_buffer, 
    char* char_scratch_space, i32 char_scratch_space_length, 
    i32 entity_idx, i32 parent_line_idx
){

    // loop through branch level.
    i32 first_idx = entity_idx;
    while(true){    
        BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
        Entity* entity = &entity_manager->entity[entity_idx];
        
        i32 written = 0;
        string_clear(entity_char_buffer);
        
        { // prepare entity data for serialisation.
            
            // entity name.
            string_push(entity_char_buffer, entity->name);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // entity type id.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%i", entity->type_id);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // parent idx.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%i", 0);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // transform position x.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.position.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform position y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.position.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform scale x.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.scale.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
        
            // transform scale y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.scale.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);
            
            // transform rotation.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->transform.rotation);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // new line.
            string_push_chars(entity_char_buffer, "\n", 1);
        }
    
        { // write entity data to file.
            platform_write_file(
                file_path, 
                entity_char_buffer->chars, 
                entity_char_buffer->count, 
                FileWriteType_Append
            );
        }
    
        BOUNDS_CHECK(entity_idx, entity_manager->entity_hierarchy.length);
        IntrusiveListNode* node = &entity_manager->entity_hierarchy.node[entity_idx];
        
        // go further into tree.
        if(node->first_child != 0){
            editor_save_level_entity_recurssive(
                entity_manager, file_path, entity_char_buffer, char_scratch_space, char_scratch_space_length, node->first_child, entity_idx
            );
        }
        
        // loop through branch level.
        entity_idx = node->next_sibling;
        if(entity_idx == first_idx){
            break;
        }
    }
}

void editor_on_hover_save_level_button(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        
        EntityManager* entity_manager = (EntityManager*)user_data;

        i32 level_idx = gen_id_get_index(game_state.editor_mouse_state.entity_parent_gid);
        
        String entity_char_buffer = {
            .chars = (char[LEVEL_FILE_LINE_LENGTH]){0},
            .length = LEVEL_FILE_LINE_LENGTH  
        };
        char* char_scratch_space = (char[64]){0};
        String file_path = {.chars = "assets/saved.scsv", .length = 17, .count = 17};
        platform_delete_file(file_path);
        
        editor_save_level_entity_recurssive(
            entity_manager, 
            file_path,
            &entity_char_buffer,
            char_scratch_space,
            64,
            level_idx,
            0
        );
    }
}

void gfx_clay_test_layout(EntityManager* manager){
    CLAY(CLAY_ID("Box"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_FIXED(512),
                .height = CLAY_SIZING_FIXED(1080)
            },
            .padding = CLAY_PADDING_ALL(24),
            .childGap = 12
        },
        .backgroundColor = { 10, 10, 20, 128},
    }){
        CLAY(CLAY_ID("Red Bird Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_red_bird_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Red Bird"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
        
        CLAY(CLAY_ID("Yellow Bird Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_yellow_bird_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Yellow Bird"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
 
        CLAY(CLAY_ID("Wood Block Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_select_wood_block_button_on_hover, NULL);
            CLAY_TEXT(CLAY_STRING("Wood Block"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
        
        for(i32 i = 1; i < manager->entity_hierarchy.root_index_count; i++){
            i32 idx = manager->entity_hierarchy.root_index[i];
            
            BOUNDS_CHECK(idx, manager->entity_length);
            Entity* entity = &manager->entity[idx];
            
            Clay_String name = {.length = entity->name.count, .chars = entity->name.chars};
            
            CLAY(CLAY_SIDI(name, idx), {
                .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
                .layout = {
                    .sizing = {
                        .width = CLAY_SIZING_PERCENT(0.2f),
                        .height = CLAY_SIZING_PERCENT(0.085f)
                    }
                },
                .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            }){
                Clay_OnHover(editor_on_hover_select_level_button, &manager->gen_id_allocator.gen_ids[idx]);
                CLAY_TEXT(name, { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255}});
            }
        }
        
        
        CLAY(CLAY_ID("Save Level Button"), {
            .backgroundColor = Clay_Hovered() ? (Clay_Color){255.0f,255.0f,255.0f,255.0f} : (Clay_Color){255.0f,255.0f,255.0f,200.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.2f),
                    .height = CLAY_SIZING_PERCENT(0.085f)
                }
            },
            .layout.childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
        }){
            Clay_OnHover(editor_on_hover_save_level_button, manager);
            CLAY_TEXT(CLAY_STRING("Save Level"), { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });
        }
    }
}