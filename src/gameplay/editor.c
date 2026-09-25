



///
/// types.
///




typedef struct{
    // the entity_id of the entity to spawn. 
    EntityTypeId entity_to_spawn;    
    GenId entity_parent_gid;
    bool hovering_element;
} EditorMouseState;

typedef struct{
    EditorMouseState mouse_state;
    GFX_State* gfx_state;
    bool is_init;
} EditorState;

typedef struct{
    EditorState* editor_state;
    GameState* game_state;
} SaveLevelButtonOnHoverContext;

typedef struct{
    EditorState* editor_state;
    GenId level_root_entity_gid;
} SelectLevelButtonOnHoverContext;



///
/// globals.
///




static i32 gfx_clay_element_id = 0;




///
/// defines.
///




#define GFX_CLAY_ROW_BUTTON_CONTAINER(vertical_percent_size)            \
CLAY(                                                                   \
    CLAY_SIDI(CLAY_STRING("Button Container"), gfx_clay_element_id++),  \
    {                                                                   \
        .layout = {                                                     \
            .sizing = {                                                 \
                CLAY_SIZING_GROW(0),                                    \
                CLAY_SIZING_PERCENT(vertical_percent_size)},            \
            .padding = CLAY_PADDING_ALL(4),                             \
            .childGap = 4,                                              \
            .layoutDirection = CLAY_LEFT_TO_RIGHT                       \
        }, .backgroundColor = {100,100,100,100},                        \
    }                                                                   \
)

#define GFX_CLAY_BUTTON(clay_string_name, horizontal_percent_size, vertical_percent_size, on_hover_callback, on_hover_user_data)    \
CLAY(                                                                                                                               \
    CLAY_SIDI(clay_string_name, gfx_clay_element_id++),                                                                             \
    {                                                                                                                               \
        .backgroundColor = Clay_Hovered() ? (Clay_Color){155.0f,155.0f,155.0f,255.0f} : (Clay_Color){100.0f,100.0f,100.0f,225.0f},  \
        .layout = {                                                                                                                 \
            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                                                         \
            .sizing = {                                                                                                             \
                CLAY_SIZING_PERCENT(horizontal_percent_size),                                                                       \
                CLAY_SIZING_PERCENT(vertical_percent_size)                                                                          \
            },                                                                                                                      \
        }                                                                                                                           \
    }                                                                                                                               \
) {                                                                                                                                 \
    Clay_OnHover(on_hover_callback, on_hover_user_data);                                                                            \
    CLAY_TEXT(clay_string_name, { .fontSize = 1, .lineHeight = 24, .textColor = {255, 255, 255, 255} });                            \
}




///
/// functions.
///




void editor_state_init(EditorState* state, GFX_State* gfx_state){
    ASSERT(!state->is_init, "already init.");
    state->mouse_state = (EditorMouseState){0};
    state->gfx_state = gfx_state;
    state->is_init = true;
}

void editor_select_red_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    EditorState* editor_state = (EditorState*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_RedBird;
    }
}

void editor_select_yellow_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    EditorState* editor_state = (EditorState*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_YellowBird;
    }
}

void editor_select_wood_block_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    EditorState* editor_state = (EditorState*)user_data;    
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_WoodBlock;
    }
}

void editor_select_pig_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    EditorState* editor_state = (EditorState*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_Pig;
    }
}

void editor_save_level_entity_recurssive(
    EntityManager* entity_manager, 
    String file_path,
    String* entity_char_buffer, 
    char* char_scratch_space, i32 char_scratch_space_length, 
    i32 entity_idx, i32 parent_line_idx, i32 line_idx
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
            written = snprintf(char_scratch_space, char_scratch_space_length, "%i", parent_line_idx);
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
        
        line_idx += 1;
        // go further into tree.
        if(node->first_child != 0){
            editor_save_level_entity_recurssive(
                entity_manager, file_path, entity_char_buffer, char_scratch_space, char_scratch_space_length, node->first_child, line_idx, line_idx 
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
        
        SaveLevelButtonOnHoverContext* ctx = (SaveLevelButtonOnHoverContext*)user_data;

        i32 level_idx = gen_id_get_index(ctx->editor_state->mouse_state.entity_parent_gid);
        
        String entity_char_buffer = {
            .chars = (char[LEVEL_FILE_LINE_LENGTH]){0},
            .length = LEVEL_FILE_LINE_LENGTH  
        };
        char* char_scratch_space = (char[64]){0};
        String file_path = {.chars = "assets/saved.scsv", .length = 17, .count = 17};
        platform_delete_file(file_path);
        
        editor_save_level_entity_recurssive(
            &ctx->game_state->entity_manager, 
            file_path,
            &entity_char_buffer,
            char_scratch_space,
            64,
            level_idx,
            0,
            0
        );
    }
}

void editor_on_hover_select_level_button(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    SelectLevelButtonOnHoverContext* ctx = (SelectLevelButtonOnHoverContext*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        ctx->editor_state->mouse_state.entity_parent_gid = ctx->level_root_entity_gid;
    }
}

void gfx_clay_test_layout(EditorState* editor_state, GameState* game_state, MemoryArena* transient){
    
    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    
    CLAY(CLAY_ID("Box"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_FIXED(512),
                .height = CLAY_SIZING_FIXED(1080)
            },
            .padding = CLAY_PADDING_ALL(6),
            .childGap = 4,
            .layoutDirection = CLAY_TOP_TO_BOTTOM
        },
        .backgroundColor = { 10, 10, 20, 128},
    }){
        if(Clay_Hovered()){
            editor_state->mouse_state.hovering_element = true;
        }
        
        f32 button_width = 0.2f;
        f32 button_height = 1.0f;

        GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_BUTTON(CLAY_STRING("Red Bird"), button_width, button_height, editor_select_red_bird_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Yellow Bird"), button_width, button_height, editor_select_yellow_bird_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Wood Block"), button_width, button_height, editor_select_wood_block_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Pig"), button_width, button_height, editor_select_pig_button_on_hover, editor_state);
            SaveLevelButtonOnHoverContext ctx = {.editor_state = editor_state, .game_state = game_state};
            void* ctx_ptr;
            MEMORY_ARENA_PUSH_STRUCT(transient, ctx, ctx_ptr);
            GFX_CLAY_BUTTON(CLAY_STRING("Save Level"), button_width, button_height, editor_on_hover_save_level_button, ctx_ptr);
        }
        
        GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            for(i32 i = 1; i < entity_manager->entity_hierarchy.root_index_count; i++){
                i32 idx = entity_manager->entity_hierarchy.root_index[i];
                BOUNDS_CHECK(idx, entity_manager->entity_length);
                Entity* entity = &entity_manager->entity[idx];
                Clay_String name = {.length = entity->name.count, .chars = entity->name.chars};
                SelectLevelButtonOnHoverContext ctx = {
                    .editor_state = editor_state, 
                    .level_root_entity_gid = entity_manager->gen_id_allocator.gen_ids[idx]
                };
                void* ctx_ptr;
                MEMORY_ARENA_PUSH_STRUCT(transient, ctx, ctx_ptr);
                
                GFX_CLAY_BUTTON(name, button_width, button_height, editor_on_hover_select_level_button, ctx_ptr);
            }
        }        
    }
}

void editor_state_update(EditorState* editor_state, GameState* game_state, MemoryArena* transient, f32 delta_time){
    
    /*
        clear from pervious run.
   
        NOTE: 
        you need to reset the global element_id as Clay SIDI on hover callbacks requires that
        elements retain their ID numbers, without this, its impossible for clay to properly handle OnHover callbacks. 
    */
    gfx_clay_element_id = 0;
    
    editor_state->mouse_state.hovering_element = false;
    
    // hoisting invariance.
    GFX_State* gfx_state = editor_state->gfx_state;
    EntityManager* entity_manager = &game_state->entity_manager;
        
    Vector2I mouse_backbuffer_position;
    platform_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);
    Clay_SetCurrentContext(gfx_state->clay_editor_ui_ctx);
    gfx_clay_begin_layout(
        gfx_state, (Vector2I){.x = gfx_state->window_ctx->width, .y = gfx_state->window_ctx->height}, mouse_backbuffer_position, delta_time, 
        input_is_mouse_button_pressed(MOUSE_BUTTON_LEFT)
    );
    gfx_clay_test_layout(editor_state, game_state, transient);
    gfx_clay_end_layout(gfx_state, delta_time, SPRITE_LAYER_UI, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
    
    if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
        if(editor_state->mouse_state.hovering_element==true){
            return;
        }
        
        Transform2D spawn_transform = TRANSFORM2D_IDENTITY;
        spawn_transform.position = gfx_get_mouse_world_position(entity_manager->gfx_state); // we use the game state's gfx_state; just in case.
        
        if(editor_state->mouse_state.entity_parent_gid == 0){
            return;
        }
        
        switch(editor_state->mouse_state.entity_to_spawn){
            case EntityTypeId_RedBird:{
                entity_spawn_red_bird(
                    entity_manager, 
                    (String){.chars = "spawned red bird", .length = 16, .count = 16}, 
                    spawn_transform, 
                    editor_state->mouse_state.entity_parent_gid
                );
            }break;
            case EntityTypeId_YellowBird:{
                entity_spawn_yellow_bird(
                    entity_manager, 
                    (String){.chars = "spawned yellow bird", .length = 19, .count = 19}, 
                    spawn_transform, 
                    editor_state->mouse_state.entity_parent_gid
                );
            }break;
            case EntityTypeId_WoodBlock:{
                entity_spawn_wood_block(
                    entity_manager, 
                    (String){.chars = "spawned wood block", .length = 18, .count = 18}, 
                    spawn_transform, 
                    editor_state->mouse_state.entity_parent_gid
                );
            }break;
            case EntityTypeId_LevelRoot:{
                entity_spawn_level_root(
                    entity_manager, 
                    (String){.chars = "spawned level", .length = 16, .count = 16}, 
                    spawn_transform, 
                    editor_state->mouse_state.entity_parent_gid
                );
            }break;
            case EntityTypeId_Pig:{
                entity_spawn_pig(
                    entity_manager, 
                    (String){.chars = "spawned pig", .length = 11, .count = 11}, 
                    spawn_transform, 
                    editor_state->mouse_state.entity_parent_gid);
            }break;
        }
    }
}