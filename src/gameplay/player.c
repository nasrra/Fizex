typedef struct{
    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
} GameMouseState;

typedef struct{
    // the entity_id of the entity to spawn. 
    i32 entity_to_spawn;    
} EditorMouseState;

typedef struct{
    bool is_editor_mode;
    EditorMouseState editor_mouse_state;
    GameMouseState mouse_state;
} GameState;
GameState game_state;

#define PLAYER_MOUSE_MAX_DRAW_RADIUS 2.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.5f

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
            
            switch(game_state.editor_mouse_state.entity_to_spawn){
                case ENTITY_ID_RED_BIRD:{
                    entity_spawn_red_bird(entity_manager, gfx_state, spawn_transform);
                }break;
                case ENTITY_ID_YELLOW_BIRD:{
                    entity_spawn_yellow_bird(entity_manager, gfx_state, spawn_transform);
                }break;
                case ENTITY_ID_WOOD_BLOCK:{
                    entity_spawn_wood_block(entity_manager, gfx_state, spawn_transform);
                }break;
            }
        }
    }
}

void editor_select_red_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = ENTITY_ID_RED_BIRD;
    }
}

void editor_select_yellow_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = ENTITY_ID_YELLOW_BIRD;
    }
}

void editor_select_wood_block_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        game_state.editor_mouse_state.entity_to_spawn = ENTITY_ID_WOOD_BLOCK;
    }
}

void gfx_clay_test_layout(){
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
    }
}