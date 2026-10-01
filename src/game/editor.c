/*
    TODO:
    - forbid the user from changing editor ui screens / alterning the clay layout
        to ensure that the selected_ui_entity integrity is preserved.
*/




///
/// types.
///




typedef enum{
    Editor_MouseStateBehaviour_Navigate, // general purpose: used for navigation, selection and spawning entities.
    Editor_MouseStateBehaviour_Move,
    Editor_MouseStateBehaviour_Scale,
    Editor_MouseStateBehaviour_Rotate
} Editor_MouseStateBehaviour;

typedef struct{
    u64 depth_layer;
    GenId gid;
} Editor_SelectedGameEntity;

typedef struct{
    PolygonRectangle shape;
    u64 depth_layer;
}Editor_EntityClickableArea;

// note that this is per-frame data; and should be considered stale
// beyond the frame boundary. hence the name suffix: immediate mode.
typedef struct{
    Editor_EntityClickableArea clickable_area;
} Editor_EntityImmediateModeData;

typedef struct{
    Editor_EntityImmediateModeData* entity;
    bool* entity_is_valid;
    i32 entity_length;
    i32 entity_is_valid_length;
    bool is_init;
} Editor_EntityManager;

typedef struct{
    // the positional offset to apply to the selected game entity when dragging it around with the mouse. 
    Vector2 selected_game_entity_positional_offset;
    bool is_clicking_selected_game_entity;
} Editor_MouseStateMoveBehaviourState;

typedef struct{
    // the entity_id of the entity to spawn.
    EntityTypeId entity_to_spawn;
    GenId entity_to_spawn_parent_gid;

    // whether or not the selected entity was just set this update tick.
    Editor_MouseStateBehaviour behaviour;
    Editor_SelectedGameEntity first_hit_game_entity;
    Editor_SelectedGameEntity selected_game_entity;
    Editor_SelectedGameEntity* previously_selected_game_entity;
    Editor_MouseStateMoveBehaviourState move_behaviour_state;
    i32 previously_selected_game_entity_length;
    i32 previously_selected_game_entity_count;
    Clay_ElementId selected_ui_entity;

    bool hovering_element;
} Editor_MouseState;

typedef struct{
    Editor_MouseState mouse_state;
    String ui_input_scratch_space;
    GFX_State* gfx_state;
    Editor_EntityManager entity_manager;
    bool is_init;
} Editor_State;

typedef struct{
    Editor_State* editor_state;
    GameState* game_state;
} SaveLevelButtonOnHoverContext;

typedef struct{
    Editor_State* editor_state;
    GenId level_root_entity_gid;
} SelectLevelButtonOnHoverContext;




///
/// globals.
///




static i32 gfx_clay_element_id = 0;




///
/// defines.
///




// the length of the clickable area struct's vertex arrays.
#define EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH 4

#define EDITOR_MAX_STRING_LENGTH 64

#define EDITOR_ENTTY_CLICKABLE_AREA_PASSIVE_COLOUR (GFX_Colour){.r = 0.25f, .g = 0.25f, .b = 0.25f, .a = 1.0f}
#define EDITOR_ENTTY_CLICKABLE_AREA_ACTIVE_COLOUR  (GFX_Colour){.r = 0.5f, .g = 0.5f, .b = 0.5f, .a = 1.0f}

#define EDITOR_MOUSE_BEHAVIOUR_NAVIGATE_KEYBINDING KEY_Q
#define EDITOR_MOUSE_BEHAVIOUR_MOVE_KEYBINDING KEY_W
#define EDITOR_MOUSE_BEHAVIOUR_SCALE_KEYBINDING KEY_E
#define EDITOR_MOUSE_BEHAVIOUR_ROTATE_KEYBINDING KEY_R

#define GFX_CLAY_WIDGET_COLOUR (Clay_Color){.r = 10, .g = 10, .b = 20, .a = 128}

#define GFX_CLAY_TEXT_CONFIG    \
(Clay_TextElementConfig){       \
    .fontSize = 1,              \
    .lineHeight = 24,           \
    .textColor = {              \
        .r = 255,               \
        .g = 255,               \
        .b = 255,               \
        .a = 255                \
    }                           \
}

#define GFX_CLAY_SIDE_PANEL()                                           \
CLAY(CLAY_SIDI(CLAY_STRING("Side Panel"), gfx_clay_element_id++), {     \
    .layout = {                                                         \
        .sizing = {                                                     \
            .width = CLAY_SIZING_PERCENT(0.25),                         \
            .height = CLAY_SIZING_GROW(0)                               \
        },                                                              \
        .padding = CLAY_PADDING_ALL(6),                                 \
        .childGap = 4,                                                  \
        .layoutDirection = CLAY_TOP_TO_BOTTOM                           \
    },                                                                  \
    .backgroundColor = GFX_CLAY_WIDGET_COLOUR,                          \
})

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

#define GFX_CLAY_BUTTON_IDLE_COLOUR (Clay_Color){100.0f,100.0f,100.0f,225.0f}
#define GFX_CLAY_BUTTON_HOVER_COLOUR (Clay_Color){155.0f,155.0f,155.0f,255.0f}
#define GFX_CLAY_BUTTON(clay_string_name, horizontal_percent_size, vertical_percent_size, on_hover_callback, on_hover_user_data)    \
CLAY(                                                                                                                               \
    CLAY_SIDI(clay_string_name, gfx_clay_element_id++),                                                                             \
    {                                                                                                                               \
        .backgroundColor = Clay_Hovered() ? GFX_CLAY_BUTTON_HOVER_COLOUR : GFX_CLAY_BUTTON_IDLE_COLOUR,                             \
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
    CLAY_TEXT(clay_string_name, GFX_CLAY_TEXT_CONFIG);                                                                              \
}

#define GFX_CLAY_INPUT_TEXT_BOX(clay_string_button_sid, clay_string_button_label, horizontal_percent_size, vertical_percent_size, editor_state_ptr) do{ \
Clay_ElementId GFX_CLAY_INPUT_TEXT_BOX_element_id = CLAY_SIDI(clay_string_button_sid, gfx_clay_element_id++);                                           \
CLAY(                                                                                                                                                   \
    GFX_CLAY_INPUT_TEXT_BOX_element_id,                                                                                                                 \
    {                                                                                                                                                   \
        .backgroundColor = Clay_Hovered() ? GFX_CLAY_BUTTON_HOVER_COLOUR : GFX_CLAY_BUTTON_IDLE_COLOUR,                                                 \
        .layout = {                                                                                                                                     \
            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                                                                             \
            .sizing = {                                                                                                                                 \
                CLAY_SIZING_PERCENT(horizontal_percent_size),                                                                                           \
                CLAY_SIZING_PERCENT(vertical_percent_size)                                                                                              \
            },                                                                                                                                          \
        }                                                                                                                                               \
    }                                                                                                                                                   \
) {                                                                                                                                                     \
    Clay_OnHover(editor_input_field_on_hover, editor_state_ptr);                                                                                        \
    if(                                                                                                                                                 \
        GFX_CLAY_INPUT_TEXT_BOX_element_id.id == editor_state_ptr->mouse_state.selected_ui_entity.id &&                                                 \
        GFX_CLAY_INPUT_TEXT_BOX_element_id.offset == editor_state_ptr->mouse_state.selected_ui_entity.offset &&                                         \
        GFX_CLAY_INPUT_TEXT_BOX_element_id.baseId == editor_state_ptr->mouse_state.selected_ui_entity.baseId                                            \
    ){                                                                                                                                                  \
                                                                                                                                                        \
        Clay_String scratch_space_string = {                                                                                                            \
            .chars = (const char*)editor_state_ptr->ui_input_scratch_space.chars,                                                                       \
            .length = editor_state_ptr->ui_input_scratch_space.count                                                                                    \
        };                                                                                                                                              \
        CLAY_TEXT(scratch_space_string, GFX_CLAY_TEXT_CONFIG);                                                                                          \
    }                                                                                                                                                   \
    else{                                                                                                                                               \
        CLAY_TEXT(clay_string_button_label, GFX_CLAY_TEXT_CONFIG);                                                                                      \
    }                                                                                                                                                   \
}                                                                                                                                                       \
}while(0)



///
/// functions.
///




void editor_entity_manager_init(Editor_EntityManager* entity_manager, MemoryArena* arena, i32 entity_amount){
    MEMORY_ARENA_ALLOC_ARRAY(arena, entity_manager->entity, &entity_manager->entity_length, entity_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, entity_manager->entity_is_valid, &entity_manager->entity_is_valid_length, entity_amount);
    entity_manager->is_init = true;
}

void editor_state_init(Editor_State* state, MemoryArena* arena, GFX_State* gfx_state, i32 entity_amount){
    ASSERT(!state->is_init, "already init.");
    state->mouse_state = (Editor_MouseState){0};
    MEMORY_ARENA_ALLOC_ARRAY(
        arena, state->mouse_state.previously_selected_game_entity, &state->mouse_state.previously_selected_game_entity_length, entity_amount
    );
    string_init(&state->ui_input_scratch_space, arena, EDITOR_MAX_STRING_LENGTH);
    state->mouse_state.previously_selected_game_entity_count = 0;
    state->gfx_state = gfx_state;
    editor_entity_manager_init(&state->entity_manager, arena, entity_amount);
    state->is_init = true;
}

void editor_input_field_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_State* editor_state = (Editor_State*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.selected_ui_entity = element_id;
    }
}

void editor_select_red_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_State* editor_state = (Editor_State*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_RedBird;
    }
}

void editor_select_yellow_bird_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_State* editor_state = (Editor_State*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_YellowBird;
    }
}

void editor_select_wood_block_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_State* editor_state = (Editor_State*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        editor_state->mouse_state.entity_to_spawn = EntityTypeId_WoodBlock;
    }
}

void editor_select_pig_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_State* editor_state = (Editor_State*)user_data;
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

        i32 level_idx = gen_id_get_index(ctx->editor_state->mouse_state.entity_to_spawn_parent_gid);

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
        ctx->editor_state->mouse_state.entity_to_spawn_parent_gid = ctx->level_root_entity_gid;
    }
}

void gfx_clay_entity_spawner_layout(Editor_State* editor_state, GameState* game_state, MemoryArena* transient){

    // hoisting invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    GFX_CLAY_SIDE_PANEL(){

        if(Clay_Hovered()){
            editor_state->mouse_state.hovering_element = true;
        }

        f32 button_width = 0.2f;
        f32 button_height = 1.0f;

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
                SelectLevelButtonOnHoverContext* ctx_ptr;
                MEMORY_ARENA_PUSH_STRUCT(transient, ctx, ctx_ptr);
                GFX_CLAY_BUTTON(name, button_width, button_height, editor_on_hover_select_level_button, ctx_ptr);
            }
        }

        GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_BUTTON(CLAY_STRING("Red Bird"), button_width, button_height, editor_select_red_bird_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Yellow Bird"), button_width, button_height, editor_select_yellow_bird_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Wood Block"), button_width, button_height, editor_select_wood_block_button_on_hover, editor_state);
            GFX_CLAY_BUTTON(CLAY_STRING("Pig"), button_width, button_height, editor_select_pig_button_on_hover, editor_state);
        }

        GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            // for some reason this only works down here?
            // the save button user data gets corrupted for some reason when above the the earlier button container?
            SaveLevelButtonOnHoverContext ctx = {.editor_state = editor_state, .game_state = game_state};
            SaveLevelButtonOnHoverContext* ctx_ptr;
            MEMORY_ARENA_PUSH_STRUCT(transient, ctx, ctx_ptr);
            GFX_CLAY_BUTTON(CLAY_STRING("Save Level"), button_width, button_height, editor_on_hover_save_level_button, ctx_ptr);
        }
    }
}

void gfx_clay_entity_inspector_layout(Editor_State* editor_state, GameState* game_state, MemoryArena* transient){

    // hoist invariance.
    EntityManager* entity_manager = &game_state->entity_manager;


    GFX_CLAY_SIDE_PANEL(){

        if(Clay_Hovered()){
            editor_state->mouse_state.hovering_element = true;
        }

        GenId entity_gid = editor_state->mouse_state.selected_game_entity.gid;
        if(entity_gid != 0){
            i32 idx = gen_id_allocator_is_gen_id_valid(&entity_manager->gen_id_allocator, entity_gid);
            if(idx){

                i32 max_chars = 35; // +1 for null terminator, as f32 is a max 34 char string.
                char* chars;
                MEMORY_ARENA_ALLOC_MEMORY(transient, chars, max_chars);
                Clay_String position_x_string = {.chars = (const char*)chars};
                MEMORY_ARENA_ALLOC_MEMORY(transient, chars, max_chars);
                Clay_String position_y_string = {.chars = (const char*)chars};
                MEMORY_ARENA_ALLOC_MEMORY(transient, chars, max_chars);
                Clay_String scale_x_string = {.chars = (const char*)chars};
                MEMORY_ARENA_ALLOC_MEMORY(transient, chars, max_chars);
                Clay_String scale_y_string = {.chars = (const char*)chars};
                MEMORY_ARENA_ALLOC_MEMORY(transient, chars, max_chars);
                Clay_String rotation_string = {.chars = (const char*)chars};

                BOUNDS_CHECK(idx, entity_manager->entity_length);
                Entity* entity = &entity_manager->entity[idx];

                // position-x.
                position_x_string.length = snprintf((char*)position_x_string.chars, max_chars, "%f", entity->transform.position.x);
                ASSERT(position_x_string.length > 0, "failed to encode float.");
                GFX_CLAY_INPUT_TEXT_BOX(CLAY_STRING("Entity Position X"), position_x_string, 0.75f, 0.05f, editor_state);

                // #if 0
                // // position-x.
                // position_x_string.length = snprintf((char*)position_x_string.chars, max_chars, "%f", entity->transform.position.x);
                // ASSERT(position_x_string.length > 0, "failed to encode float.");
                // CLAY_TEXT(position_x_string, GFX_CLAY_TEXT_CONFIG);

                // // position-y.
                // position_y_string.length = snprintf((char*)position_y_string.chars, max_chars, "%f", entity->transform.position.y);
                // ASSERT(position_y_string.length > 0, "failed to encode float.");
                // CLAY_TEXT(position_y_string, GFX_CLAY_TEXT_CONFIG);

                // // scale-x.
                // scale_x_string.length = snprintf((char*)scale_x_string.chars, max_chars, "%f", entity->transform.scale.x);
                // ASSERT(scale_x_string.length > 0, "failed to encode float.");
                // CLAY_TEXT(scale_x_string, GFX_CLAY_TEXT_CONFIG);

                // // scale-y.
                // scale_y_string.length = snprintf((char*)scale_y_string.chars, max_chars, "%f", entity->transform.scale.y);
                // ASSERT(scale_y_string.length > 0, "failed to encode float.");
                // CLAY_TEXT(scale_y_string, GFX_CLAY_TEXT_CONFIG);

                // // rotation.
                // rotation_string.length = snprintf((char*)rotation_string.chars, max_chars, "%f", entity->transform.rotation);
                // ASSERT(rotation_string.length > 0, "failed to encode float.");
                // CLAY_TEXT(rotation_string, GFX_CLAY_TEXT_CONFIG);
                // #endif
            }
        }
    }
}




///
/// functions: Editor_MouseState
///




bool editor_selected_game_entity_equals(Editor_SelectedGameEntity a, Editor_SelectedGameEntity b){
    return a.gid == b.gid && a.depth_layer == b.depth_layer;
}

bool editor_mouse_state_has_selected_game_entity(Editor_MouseState* mouse_state){
    return !editor_selected_game_entity_equals(mouse_state->selected_game_entity, (Editor_SelectedGameEntity){0});
}

void editor_mouse_state_set_selected_game_entity(Editor_MouseState* mouse_state, Editor_SelectedGameEntity entity){
    mouse_state->selected_game_entity = entity;
}




///
/// functions: Editor_State
///




void editor_state_update(Editor_State* editor_state, GameState* game_state, MemoryArena* transient, f32 delta_time){

    // hoisting invariance.
    Editor_EntityManager*   editor_entity_manager = &editor_state->entity_manager;
    Editor_MouseState*      editor_mouse_state = &editor_state->mouse_state;
    GFX_State*              editor_gfx_state = editor_state->gfx_state;

    EntityManager*          game_entity_manager = &game_state->entity_manager;
    GFX_State*              game_gfx_state = game_entity_manager->gfx_state;

    // retrieve necessary data.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(game_gfx_state);


    /*
        clear from pervious run.

        NOTE:
        you need to reset the global element_id as Clay SIDI on hover callbacks requires that
        elements retain their ID numbers, without this, its impossible for clay to properly handle OnHover callbacks.
    */
    gfx_clay_element_id = 0;

    editor_state->mouse_state.hovering_element = false;

    { // ui drawing.

        Vector2I mouse_backbuffer_position;
        input_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);
        Clay_SetCurrentContext(editor_gfx_state->clay_editor_ui_ctx);
        gfx_clay_begin_layout(
            editor_gfx_state, (Vector2I){.x = editor_gfx_state->window_ctx->width, .y = editor_gfx_state->window_ctx->height}, mouse_backbuffer_position, delta_time,
            input_is_mouse_button_pressed(MOUSE_BUTTON_LEFT)
        );
        gfx_clay_entity_spawner_layout(editor_state, game_state, transient);
        // gfx_clay_entity_inspector_layout(editor_state, game_state, transient);
        gfx_clay_end_layout(editor_gfx_state, delta_time, SPRITE_LAYER_GAME_UI, VIRTUAL_TEXTURE_ID_FONT, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
    }

    { // retrieve_entity_immeidate_mode_data();

        // clear previous tick's stale data.
        ZERO_MEMORY(editor_entity_manager->entity_is_valid, sizeof(bool) * editor_entity_manager->entity_is_valid_length);

        for(i32 idx = 0; idx < game_entity_manager->entity_length; idx++){
            if(!entity_manager_is_entity_allocated_unsafe(*game_entity_manager, idx)){
                continue;
            }
            BOUNDS_CHECK(idx, editor_entity_manager->entity_length);
            editor_state->entity_manager.entity_is_valid[idx] = true;
            Editor_EntityImmediateModeData* editor_entity = &editor_entity_manager->entity[idx];
            Entity* game_entity = &game_entity_manager->entity[idx];

            { // get_clickable_area();

                editor_entity->clickable_area = (Editor_EntityClickableArea){0};
                Transform2D preferred_transform = {0};
                // note that we infer this is a sprite; no need to check 'is_sprite' flag or 'sprite_gid'.
                if(gfx_sprite_get_transform(*game_gfx_state, game_entity->sprite_gid, &preferred_transform)){

                    GFX_SpriteOrigin sprite_origin;

                    // falback to entity transform if we failed to get the sprite origin.
                    if(!gfx_sprite_get_sprite_origin(*game_gfx_state, game_entity->sprite_gid, &sprite_origin)){
                        preferred_transform = game_entity->transform;
                        editor_entity->clickable_area.shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                    }

                    switch(sprite_origin){
                        default:
                            ASSERT(false, "unknown sprite origin");
                        case GFX_SpriteOrigin_Center:{
                            editor_entity->clickable_area.shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                        }break;
                        case GFX_SpriteOrigin_TopLeft:{
                            editor_entity->clickable_area.shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                        }break;
                    }
                    gfx_sprite_get_depth_layer(game_gfx_state, game_entity->sprite_gid, &editor_entity->clickable_area.depth_layer);
                }

                // is not a sprite.
                else{
                    preferred_transform = game_entity->transform;
                    editor_entity->clickable_area.shape = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                }
            }
        }
    }


    { // input_handling();

        if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_NAVIGATE_KEYBINDING)){
            editor_mouse_state->behaviour = Editor_MouseStateBehaviour_Navigate;
        }
        if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_MOVE_KEYBINDING)){
            editor_mouse_state->behaviour = Editor_MouseStateBehaviour_Move;
        }
        if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_SCALE_KEYBINDING)){
            editor_mouse_state->behaviour = Editor_MouseStateBehaviour_Scale;
        }
        if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_ROTATE_KEYBINDING)){
            editor_mouse_state->behaviour = Editor_MouseStateBehaviour_Rotate;
        }

        switch(editor_mouse_state->behaviour){
            case Editor_MouseStateBehaviour_Navigate:{

                { // entity_selecting();

                    if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){

                        /*
                            this is here in the very astronomically low case that  entities are deallocated enough times
                            an reallocated, stacking ontop of eachother (whilst clicking them); which could cause a buffer overflow.
                            DO NOT REMOVE THIS!!!
                        */
                        if(editor_mouse_state->previously_selected_game_entity_count == editor_mouse_state->previously_selected_game_entity_length){
                            editor_mouse_state->previously_selected_game_entity_count = 0;
                        }

                        if(editor_mouse_state->selected_game_entity.gid != 0){
                            ARRAY_PUSH(
                                editor_mouse_state->previously_selected_game_entity,
                                editor_mouse_state->previously_selected_game_entity_length,
                                &editor_mouse_state->previously_selected_game_entity_count,
                                editor_mouse_state->selected_game_entity
                            );
                        }
                        editor_mouse_state->first_hit_game_entity           = (Editor_SelectedGameEntity){.gid = GENID_MAX, .depth_layer = U64_MAX};
                        editor_mouse_state->selected_game_entity            = (Editor_SelectedGameEntity){0};
                    }

                    // only attempt selction of a game entity if the mouse isnt on a editor
                    // ui widget, the left button has just been pressed, and we havent already selected an entity.
                    if(
                        !editor_state->mouse_state.hovering_element &&
                        input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT) &&
                        !editor_mouse_state_has_selected_game_entity(editor_mouse_state)
                    ){

                        for(i32 idx = 0; idx < editor_entity_manager->entity_length; idx++){

                            if(!editor_entity_manager->entity_is_valid[idx]){
                                continue;
                            }

                            Editor_EntityImmediateModeData* entity = &editor_entity_manager->entity[idx];

                            Vector2 normal;
                            f32 depth;
                            bool overlaps = polygon_overlaps_point_scalar(
                                entity->clickable_area.shape.x, entity->clickable_area.shape.y,
                                EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH,
                                mouse_world_position.x, mouse_world_position.y,
                                &normal.x, &normal.y, &depth
                            );

                            if(!overlaps){
                                continue;
                            }

                            // get the game entity vertically associated with this editor entity.
                            BOUNDS_CHECK(idx, game_entity_manager->gen_id_allocator.length);
                            GenId current_gid = game_entity_manager->gen_id_allocator.gen_ids[idx];

                            if(entity->clickable_area.depth_layer <= editor_mouse_state->first_hit_game_entity.depth_layer){
                                Editor_SelectedGameEntity first_hit_game_entity = {.gid = current_gid, .depth_layer = entity->clickable_area.depth_layer};
                                editor_mouse_state->first_hit_game_entity = first_hit_game_entity;
                            }

                            // loop through all of the previously selected entities
                            // and skip this one if it was previously selected.
                            bool is_previously_selected = false;
                            for(i32 j = 0; j < editor_mouse_state->previously_selected_game_entity_count; j++){
                                if(current_gid == editor_mouse_state->previously_selected_game_entity[j].gid){
                                    is_previously_selected = true;
                                    break;
                                }
                            }
                            if(is_previously_selected){
                                continue;
                            }

                            if(entity->clickable_area.depth_layer >= editor_mouse_state->selected_game_entity.depth_layer){
                                Editor_SelectedGameEntity selected_game_entity = {.gid = current_gid, .depth_layer = entity->clickable_area.depth_layer};
                                editor_mouse_state_set_selected_game_entity(editor_mouse_state, selected_game_entity);
                            }
                        }

                        // if the mouse didnt find anything to select.
                        if(!editor_mouse_state_has_selected_game_entity(editor_mouse_state)){

                            // if the mouse did click on something.
                            if(!editor_selected_game_entity_equals(editor_mouse_state->first_hit_game_entity, (Editor_SelectedGameEntity){.gid = GENID_MAX, .depth_layer = U64_MAX})){
                                // go to the top of the stack.
                                editor_mouse_state_set_selected_game_entity(editor_mouse_state, editor_mouse_state->first_hit_game_entity);
                            }
                            // if the mouse didn't click on anything, clear the stack entirely.
                            editor_mouse_state->previously_selected_game_entity_count = 0;
                        }
                    }
                }


                // handle entity spawning if we are not moving a selected entity around.
                if(
                    !editor_mouse_state_has_selected_game_entity(editor_mouse_state) &&
                    input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)
                ){
                    if(editor_state->mouse_state.hovering_element==true){
                        return;
                    }

                    // check if mouse is within clickable area.
                    // get entity sprite layer; fallback to using id's if both are the same.

                    Transform2D spawn_transform = TRANSFORM2D_IDENTITY;
                    spawn_transform.position = mouse_world_position; // we use the game state's gfx_state; just in case.

                    if(editor_state->mouse_state.entity_to_spawn_parent_gid == 0){
                        return;
                    }

                    switch(editor_state->mouse_state.entity_to_spawn){
                        case EntityTypeId_RedBird:{
                            entity_spawn_red_bird(
                                game_entity_manager,
                                (String){.chars = "spawned red bird", .length = 16, .count = 16},
                                spawn_transform,
                                editor_state->mouse_state.entity_to_spawn_parent_gid
                            );
                        }break;
                        case EntityTypeId_YellowBird:{
                            entity_spawn_yellow_bird(
                                game_entity_manager,
                                (String){.chars = "spawned yellow bird", .length = 19, .count = 19},
                                spawn_transform,
                                editor_state->mouse_state.entity_to_spawn_parent_gid
                            );
                        }break;
                        case EntityTypeId_WoodBlock:{
                            entity_spawn_wood_block(
                                game_entity_manager,
                                (String){.chars = "spawned wood block", .length = 18, .count = 18},
                                spawn_transform,
                                editor_state->mouse_state.entity_to_spawn_parent_gid
                            );
                        }break;
                        case EntityTypeId_LevelRoot:{
                            entity_spawn_level_root(
                                game_entity_manager,
                                (String){.chars = "spawned level", .length = 16, .count = 16},
                                spawn_transform,
                                editor_state->mouse_state.entity_to_spawn_parent_gid
                            );
                        }break;
                        case EntityTypeId_Pig:{
                            entity_spawn_pig(
                                game_entity_manager,
                                (String){.chars = "spawned pig", .length = 11, .count = 11},
                                spawn_transform,
                                editor_state->mouse_state.entity_to_spawn_parent_gid);
                        }break;
                    }
                }
            }break;
            case Editor_MouseStateBehaviour_Move:{
                
                Entity* game_entity;
                if(!entity_manager_get_entity(*game_entity_manager, editor_mouse_state->selected_game_entity.gid, &game_entity)){
                    // failed to retrieve selected entity data.
                    break;
                }
                
                gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_LIGHT_BLUE, game_entity->transform.position, VECTOR2_UP, 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
                gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_PINK, game_entity->transform.position, VECTOR2_RIGHT, 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);                
                
                i32 entity_idx = gen_id_get_index(editor_mouse_state->selected_game_entity.gid);
                BOUNDS_CHECK(entity_idx, editor_entity_manager->entity_length);
                Editor_EntityImmediateModeData* editor_entity = &editor_entity_manager->entity[entity_idx];

                // check if we have just clicked the entity.
                if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
                    Vector2 normal;
                    f32 depth;
                    bool overlaps = 
                        polygon_overlaps_point_scalar(
                            editor_entity->clickable_area.shape.x, editor_entity->clickable_area.shape.y,
                            EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH,
                            mouse_world_position.x, mouse_world_position.y,
                            &normal.x, &normal.y, &depth
                        );
                    
                    // store the on click information.
                    if(overlaps){
                        editor_mouse_state->move_behaviour_state.selected_game_entity_positional_offset 
                            = vector2_sub(game_entity->transform.position, mouse_world_position);
                        editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity = true;
                    } 
                }
                else if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
                    editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity = false;
                }
                
                // dont do anything if we are not clicking the entity at all.
                if(!editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity){
                    break;
                }
                
                
                // drag the selected entity with the mouse if we are clicking down.
                if(input_is_mouse_button_pressed(MOUSE_BUTTON_LEFT)){                                            
                    Vector2 new_position = vector2_add(mouse_world_position, editor_mouse_state->move_behaviour_state.selected_game_entity_positional_offset);
                    game_entity->transform.position = new_position;                    
                }
            }break;
        }
        
        
        // debug_gizmos();
        {
        
            // note:
            // this is fine, as state isnt retained; this data is immediate mode.
            i32 selected_game_entity_idx = -1;
            if(editor_mouse_state_has_selected_game_entity(editor_mouse_state)){
                selected_game_entity_idx = gen_id_get_index(editor_mouse_state->selected_game_entity.gid);
            }
            
        
            for(i32 i = 0; i < editor_entity_manager->entity_length; i++){
                BOUNDS_CHECK(i, editor_entity_manager->entity_is_valid_length);
                if(!editor_entity_manager->entity_is_valid[i]){
                    continue;
                }
                
                Editor_EntityImmediateModeData* editor_entity = &editor_entity_manager->entity[i];
                
                // draw the clickable area.                
                i32 sprite_depth = 100;
                gfx_draw_wire_poly(
                    editor_gfx_state,
                    editor_entity->clickable_area.shape.x,
                    editor_entity->clickable_area.shape.y,
                    EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH,
                    selected_game_entity_idx == i ? EDITOR_ENTTY_CLICKABLE_AREA_ACTIVE_COLOUR : EDITOR_ENTTY_CLICKABLE_AREA_PASSIVE_COLOUR,
                    SPRITE_LAYER_EDITOR_WORLD,
                    sprite_depth,
                    SPRITE_MATERIAL_DEBUG
                );
            
            }
        }
    }
}