

/*
    TODO:
    - forbid the user from changing editor ui screens / alterning the clay layout
        to ensure that the selected_editor_ui_entity integrity is preserved.
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

typedef enum{
    Editor_ButtonDataStorageType_F32,
    Editor_ButtonDataStorageType_I32,
    Editor_ButtonDataStorageType_String
} Editor_ButtonDataStorageType;

typedef enum{
    Editor_Menu_EntitySpawner,  
    Editor_Menu_EntityInspector
} Editor_Menu;

typedef struct{
    // the type of data stored that the button is displaying.
    Editor_ButtonDataStorageType data_storage_type;
    // a pointer to the stored data that the button is displaying.
    void* data_storage;
    Clay_ElementId clay_element_id;
    // the game entity that this is apart of (zero if it isnt associated with an entity).
    i32 entity_idx;
} Editor_SelectedEditorUiEntity;

typedef struct{
    PolygonRectangle shape;
    u64 depth_layer;
}Editor_EntityClickableArea;

typedef struct{
    // the positional offset to apply to the selected game entity when dragging it around with the mouse. 
    Vector2 selected_game_entity_positional_offset;
    bool is_clicking_selected_game_entity;
} Editor_MouseStateMoveBehaviourState;

typedef struct{
    // the previous frame's positional difference (from entity to mouse).
    Vector2 previous_positional_difference;
} Editor_MouseStateRotateBehaviourState;

typedef struct{
    // the entity_id of the entity to spawn.
    EntityTypeId entity_to_spawn;
    GenId entity_to_spawn_parent_gid;

    // whether or not the selected entity was just set this update tick.
    Editor_MouseStateBehaviour previous_behaviour;
    Editor_MouseStateBehaviour current_behaviour;
    Editor_MouseStateMoveBehaviourState move_behaviour_state;
    Editor_MouseStateRotateBehaviourState rotate_behaviour_state;
        
    Editor_SelectedEditorUiEntity selected_editor_ui_entity;
    bool hovering_element;
} Editor_MouseState;

typedef struct{
    Editor_MouseState mouse_state;
    String f32_input_text_box_scratch_space;
    String string_input_text_box_scratch_space;
    GFX_State* gfx_state;
    
    Vector2 world_camera_position;
    f32 world_camera_orthographic_size;
    
    ClickableAreaHierarchy game_entity_clickable_area_hierarchy;
    GenId slected_game_entity_gid;
    Editor_Menu menu;
    bool is_typing;
    bool is_init;
} Editor_State;

typedef struct {
    // copied into 'dst'.
    Editor_SelectedEditorUiEntity src;
    // destination for 'src' data.
    Editor_State* editor_state;
    Clay_String input_text_box_label_string;
} Editor_InputTextBoxOnClickContext;

typedef struct{
    Editor_State* editor_state;
    GameState* game_state;
    MemoryArena* transient;
} SaveLevelButtonOnHoverContext;

typedef struct{
    Editor_State* editor_state;
    GenId level_root_entity_gid;
} SelectLevelButtonOnHoverContext;

typedef struct{
    Editor_State* editor_state;
    EntityTypeId entity_type_id;
} Editor_SpawnEntityButtonOnHoverContext;


///
/// @ Globals.
///


static i32 editor_gfx_clay_element_id = 0;


///
/// @ Defines.
///

#define EDITOR_WORLD_CAMERA_PROJECTION_TYPE GFX_ProjectionType_Orthographic
#define EDITOR_WORLD_CAMERA_COORDINATE_SPACE GFX_CoordinateSpace_Cartesian
#define EDITOR_WORLD_CAMERA_FAR_Z F32_MAX
#define EDITOR_WORLD_CAMERA_NEAR_Z F32_EPSILON

// the length of the clickable area struct's vertex arrays.
#define EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH 4

// max value of f32 is: 340,282,346,638,528,859,811,704,183,484,516,925,440
// which is 39 characters long (excluding commans).
#define EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH 39
#define EDITOR_STRING_INPUT_BUTTON_MAX_STRING_LENGTH 64
#define EDITOR_MAX_STRING_LENGTH 64

#define EDITOR_ENTTY_CLICKABLE_AREA_PASSIVE_COLOUR (GFX_Colour){.r = 0.25f, .g = 0.25f, .b = 0.25f, .a = 1.0f}
#define EDITOR_ENTTY_CLICKABLE_AREA_ACTIVE_COLOUR  (GFX_Colour){.r = 0.77f, .g = 0.77f, .b = 0.77f, .a = 1.0f}

#define EDITOR_MOUSE_BEHAVIOUR_NAVIGATE_KEYBINDING Key_Q
#define EDITOR_MOUSE_BEHAVIOUR_MOVE_KEYBINDING Key_W
#define EDITOR_MOUSE_BEHAVIOUR_SCALE_KEYBINDING Key_E
#define EDITOR_MOUSE_BEHAVIOUR_ROTATE_KEYBINDING Key_R

#define EDITOR_GFX_CLAY_WIDGET_COLOUR (Clay_Color){.r = 10, .g = 10, .b = 20, .a = 128}

#define EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG \
(Clay_TextElementConfig){                   \
    .fontSize = 1,                          \
    .lineHeight = 24,                       \
    .textColor = {                          \
        .r = 255,                           \
        .g = 255,                           \
        .b = 255,                           \
        .a = 255                            \
    }                                       \
}

#define EDITOR_GFX_CLAY_TYPING_TEXT_CONFIG  \
(Clay_TextElementConfig){                   \
    .fontSize = 1,                          \
    .lineHeight = 24,                       \
    .textColor = {                          \
        .r = 255,                           \
        .g = 150,                           \
        .b = 0,                             \
        .a = 255                            \
    }                                       \
}

#define EDITOR_GFX_CLAY_SIDE_PANEL()                                        \
CLAY(CLAY_SIDI(CLAY_STRING("Side Panel"), editor_gfx_clay_element_id++), {  \
    .layout = {                                                             \
        .sizing = {                                                         \
            .width = CLAY_SIZING_PERCENT(0.25),                             \
            .height = CLAY_SIZING_GROW(0)                                   \
        },                                                                  \
        .padding = CLAY_PADDING_ALL(6),                                     \
        .childGap = 4,                                                      \
        .layoutDirection = CLAY_TOP_TO_BOTTOM                               \
    },                                                                      \
    .backgroundColor = EDITOR_GFX_CLAY_WIDGET_COLOUR,                       \
})

#define EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(vertical_percent_size)             \
CLAY(                                                                           \
    CLAY_SIDI(CLAY_STRING("Button Container"), editor_gfx_clay_element_id++),   \
    {                                                                           \
        .layout = {                                                             \
            .sizing = {                                                         \
                CLAY_SIZING_GROW(0),                                            \
                CLAY_SIZING_PERCENT(vertical_percent_size)},                    \
            .padding = CLAY_PADDING_ALL(4),                                     \
            .childGap = 4,                                                      \
            .layoutDirection = CLAY_LEFT_TO_RIGHT                               \
        }, .backgroundColor = {100,100,100,100},                                \
    }                                                                           \
)

#define EDITOR_GFX_CLAY_BUTTON_IDLE_COLOUR (Clay_Color){100.0f,100.0f,100.0f,225.0f}
#define EDITOR_GFX_CLAY_BUTTON_HOVER_COLOUR (Clay_Color){155.0f,155.0f,155.0f,255.0f}
#define EDITOR_GFX_CLAY_BUTTON(clay_string_name, horizontal_percent_size, vertical_percent_size, on_hover_callback, on_hover_user_data)     \
CLAY(                                                                                                                                       \
    CLAY_SIDI(clay_string_name, editor_gfx_clay_element_id++),                                                                              \
    {                                                                                                                                       \
        .backgroundColor = Clay_Hovered() ? EDITOR_GFX_CLAY_BUTTON_HOVER_COLOUR : EDITOR_GFX_CLAY_BUTTON_IDLE_COLOUR,                       \
        .layout = {                                                                                                                         \
            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                                                                 \
            .sizing = {                                                                                                                     \
                CLAY_SIZING_PERCENT(horizontal_percent_size),                                                                               \
                CLAY_SIZING_PERCENT(vertical_percent_size)                                                                                  \
            },                                                                                                                              \
        }                                                                                                                                   \
    }                                                                                                                                       \
) {                                                                                                                                         \
    Clay_OnHover(on_hover_callback, on_hover_user_data);                                                                                    \
    CLAY_TEXT(clay_string_name, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);                                                                               \
}

#define GFX_CLAY_SPAWN_ENTITY_BUTTON(_clay_string_name, _horizontal_percent_size, _vertical_percent_size, _transient_arena, _editor_state, _entity_type_id)do{              \
    Editor_SpawnEntityButtonOnHoverContext GFX_CLAY_SPAWN_ENTITY_BUTTON_ctx = {.editor_state = _editor_state, .entity_type_id = _entity_type_id};                           \
    void* GFX_CLAY_SPAWN_ENTITY_BUTTON_ctx_ptr;                                                                                                                             \
    MEMORY_ARENA_PUSH_STRUCT(_transient_arena, GFX_CLAY_SPAWN_ENTITY_BUTTON_ctx, GFX_CLAY_SPAWN_ENTITY_BUTTON_ctx_ptr);                                                     \
    EDITOR_GFX_CLAY_BUTTON(_clay_string_name, _horizontal_percent_size, _vertical_percent_size, editor_spawn_entity_button_on_hover, GFX_CLAY_SPAWN_ENTITY_BUTTON_ctx_ptr); \
}while(0)                                                                                                                                                                   \

/*
    `typed example`
    GFX_CLAY_INPUT_TEXT_BOX(
        Clay_String clay_string_button_sid, 
        Clay_String clay_string_button_label, 
        f32 horizontal_percent_size, 
        f32 vertical_percent, 
        Editor_State* editor_state, 
        void* data_storage_ptr, 
        Editor_ButtonDataStorageType data_storage_type,
        Memory_Arena* transient_arena,
        i32 entity_idx
    );
    
    `parameters`
    `clay_string_button_sid`: the string id of the button clay ui element.
    `clay_string_button_label`: the contents of the textual label displayed ontop of the button rectangle.
    `horizontal_percent_size`: the horizontal percentage size of the button within the container widget.
    `vertical_percent_size`: the horizontal percentage size of the button within the container widget.
    `editor_state`: a pointer to the editor state to set the `selected_editor_ui_entity` and `is_typing` when clicked. 
    `data_storage`: a pointer to the data this button is assigned to / is displaying. 
    `data_storage_type`: the type of data stored.
    `transient_arena`: the transient arena to allocate the function capture onto.
    `entity_idx`: the index of the entity whom the data_storage is attached to; otherwise 0 if not an entity data_storage.
*/

#define GFX_CLAY_INPUT_TEXT_BOX(                                                                                                            \
    _clay_string_button_sid,                                                                                                                \
    _clay_string_button_label,                                                                                                              \
    _horizontal_percent_size,                                                                                                               \
    _vertical_percent_size,                                                                                                                 \
    _editor_state,                                                                                                                          \
    _data_storage,                                                                                                                          \
    _data_storage_type,                                                                                                                     \
    _transient_arena,                                                                                                                       \
    _entity_idx                                                                                                                             \
) do{                                                                                                                                       \
Clay_ElementId GFX_CLAY_INPUT_TEXT_BOX_clay_element_id = CLAY_SIDI(_clay_string_button_sid, editor_gfx_clay_element_id++);                  \
CLAY(                                                                                                                                       \
    GFX_CLAY_INPUT_TEXT_BOX_clay_element_id,                                                                                                \
    {                                                                                                                                       \
        .backgroundColor = Clay_Hovered() ? EDITOR_GFX_CLAY_BUTTON_HOVER_COLOUR : EDITOR_GFX_CLAY_BUTTON_IDLE_COLOUR,                       \
        .layout = {                                                                                                                         \
            .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER },                                                                 \
            .sizing = {                                                                                                                     \
                CLAY_SIZING_PERCENT(_horizontal_percent_size),                                                                              \
                CLAY_SIZING_PERCENT(_vertical_percent_size)                                                                                 \
            },                                                                                                                              \
        }                                                                                                                                   \
    }                                                                                                                                       \
) {                                                                                                                                         \
                                                                                                                                            \
    Editor_InputTextBoxOnClickContext GFX_CLAY_INPUT_TEXT_BOX_ctx = {                                                                       \
        .src = {                                                                                                                            \
            .data_storage_type = _data_storage_type,                                                                                        \
            .data_storage = _data_storage,                                                                                                  \
            .clay_element_id = GFX_CLAY_INPUT_TEXT_BOX_clay_element_id,                                                                     \
            .entity_idx = _entity_idx,                                                                                                      \
        },                                                                                                                                  \
        .editor_state = _editor_state,                                                                                                      \
        .input_text_box_label_string = _clay_string_button_label                                                                            \
    };                                                                                                                                      \
    void* GFX_CLAY_INPUT_TEXT_BOX_ctx_ptr;                                                                                                  \
    MEMORY_ARENA_PUSH_STRUCT(_transient_arena, GFX_CLAY_INPUT_TEXT_BOX_ctx, GFX_CLAY_INPUT_TEXT_BOX_ctx_ptr);                               \
    Clay_OnHover(editor_input_field_on_hover, GFX_CLAY_INPUT_TEXT_BOX_ctx_ptr);                                                             \
                                                                                                                                            \
    if(                                                                                                                                     \
        GFX_CLAY_INPUT_TEXT_BOX_clay_element_id.id       == editor_state->mouse_state.selected_editor_ui_entity.clay_element_id.id &&       \
        GFX_CLAY_INPUT_TEXT_BOX_clay_element_id.offset   == editor_state->mouse_state.selected_editor_ui_entity.clay_element_id.offset &&   \
        GFX_CLAY_INPUT_TEXT_BOX_clay_element_id.baseId   == editor_state->mouse_state.selected_editor_ui_entity.clay_element_id.baseId      \
    ){                                                                                                                                      \
                                                                                                                                            \
        String* GFX_CLAY_INPUT_TEXT_BOX_scratch_space_string;                                                                               \
        switch(_editor_state->mouse_state.selected_editor_ui_entity.data_storage_type){                                                     \
            case Editor_ButtonDataStorageType_F32:{                                                                                         \
                GFX_CLAY_INPUT_TEXT_BOX_scratch_space_string = &_editor_state->f32_input_text_box_scratch_space;                            \
            }break;                                                                                                                         \
            case Editor_ButtonDataStorageType_String:{                                                                                      \
                GFX_CLAY_INPUT_TEXT_BOX_scratch_space_string = &_editor_state->string_input_text_box_scratch_space;                         \
            }break;                                                                                                                         \
        }                                                                                                                                   \
                                                                                                                                            \
        Clay_String clay_scratch_space_string = {                                                                                           \
            .chars = (const char*)GFX_CLAY_INPUT_TEXT_BOX_scratch_space_string->chars,                                                      \
            .length = GFX_CLAY_INPUT_TEXT_BOX_scratch_space_string->count                                                                   \
        };                                                                                                                                  \
        CLAY_TEXT(clay_scratch_space_string, EDITOR_GFX_CLAY_TYPING_TEXT_CONFIG);                                                                  \
    }                                                                                                                                       \
    else{                                                                                                                                   \
        CLAY_TEXT(_clay_string_button_label, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);                                                                  \
    }                                                                                                                                       \
}                                                                                                                                           \
}while(0)


///
/// functions.
///


void editor_state_init(Editor_State* state, MemoryArena* arena, GFX_State* gfx_state, i32 entity_amount){
    ASSERT(!state->is_init, "already init.");
    state->mouse_state = (Editor_MouseState){0};
    
    string_init(&state->f32_input_text_box_scratch_space, arena, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
    string_init(&state->string_input_text_box_scratch_space, arena, EDITOR_STRING_INPUT_BUTTON_MAX_STRING_LENGTH);
    
    state->gfx_state = gfx_state;
    clickable_area_hierarchy_init(&state->game_entity_clickable_area_hierarchy, arena, entity_amount);    
    
    // set all the allocated flags to true so that the 'set_clickable_areas' loop doesnt have to.
    for(i32 i = 0; i < state->game_entity_clickable_area_hierarchy.gen_id_allocator.length; i++){
        state->game_entity_clickable_area_hierarchy.gen_id_allocator.allocated[i] = true;
    }
    
    state->is_init = true;
}

void editor_input_field_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_InputTextBoxOnClickContext* ctx = (Editor_InputTextBoxOnClickContext*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        ctx->editor_state->mouse_state.selected_editor_ui_entity = ctx->src;
        ctx->editor_state->is_typing = true;
        String label_string = {
            .chars = (char*)ctx->input_text_box_label_string.chars,
            .length = ctx->input_text_box_label_string.length,
            .count = ctx->input_text_box_label_string.length
        };
        
        String* typing_scratch_space;
        
        switch(ctx->src.data_storage_type){
            default: {ASSERT(false, "unknown data storage type.");}break;
            case Editor_ButtonDataStorageType_F32:{typing_scratch_space = &ctx->editor_state->f32_input_text_box_scratch_space;} break;
            case Editor_ButtonDataStorageType_String:{typing_scratch_space = &ctx->editor_state->string_input_text_box_scratch_space;} break;
        }

        string_clear(typing_scratch_space);
        string_push(typing_scratch_space, label_string);
    }
}

void editor_spawn_entity_button_on_hover(Clay_ElementId element_id, Clay_PointerData pointer_info, void* user_data){
    Editor_SpawnEntityButtonOnHoverContext* ctx = (Editor_SpawnEntityButtonOnHoverContext*)user_data;
    if(pointer_info.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME){
        ctx->editor_state->mouse_state.entity_to_spawn = ctx->entity_type_id;
    } 
}

/*
    `remarks`
    `file_path`: is a scratch space string, for writing to.
*/
void editor_save_level_entity_recurssive(
    EntityManager* entity_manager,
    String* file_path,
    String* entity_char_buffer,
    char* char_scratch_space, i32 char_scratch_space_length,
    i32 entity_idx, i32 parent_line_idx, i32 line_idx
){

    // this is the root entity of the tree, and the file name
    // should be the name of the root entity.
    if(line_idx == 0){
        
        BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
        Entity* entity = &entity_manager->entity[entity_idx];
    
        // this starts as a directory path, and gets the file name + extension added here.
        // for subsequent serialisation calls.
        // string_push(file_path, entity->name);
        string_push(file_path, entity->serialisation_file_path);
        
        if(!platform_delete_file(*file_path)){
            platform_output_message("[Warning]: failed to delete level file whilst serialising entity; as the file wasn't present previously.");
        }
    }

    // loop through branch level.
    i32 first_idx = entity_idx;
    while(true){
        BOUNDS_CHECK(entity_idx, entity_manager->entity_length);
        Entity* entity = &entity_manager->entity[entity_idx];
        
        if(entity->serialisation_disabled){
            break;
        }

        i32 written = 0;
        string_clear(entity_char_buffer);

        bool is_asset_entity =             
            line_idx > 0 && // dont serialise root entity as an asset file.
            entity->serialisation_file_path.count
        ;

        // asset entity.
        if(is_asset_entity){
        
            string_push_char(entity_char_buffer, LEVEL_FILE_ASSET_ENTITY_TOKEN);
            string_push_char(entity_char_buffer, ';');
            string_push(entity_char_buffer, entity->serialisation_file_path);
            string_push_chars(entity_char_buffer, ";\n", 2);
        }
        
        // native entity serialisation.
        else{

            // token
            string_push_char(entity_char_buffer, LEVEL_FILE_NATIVE_ENTITY_TOKEN);
            string_push_char(entity_char_buffer, ';');

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
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.position.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // transform position y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.position.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // transform scale x.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.scale.x);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // transform scale y.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.scale.y);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // transform sine.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.sine);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // transform cosine.
            written = snprintf(char_scratch_space, char_scratch_space_length, "%f", entity->local_transform.cosine);
            string_push_chars(entity_char_buffer, char_scratch_space, written);
            string_push_chars(entity_char_buffer, ";", 1);

            // new line.
            string_push_chars(entity_char_buffer, "\n", 1);
        }
                
        platform_write_file(
            *file_path,
            entity_char_buffer->chars,
            entity_char_buffer->count,
            FileWriteType_Append
        );

        BOUNDS_CHECK(entity_idx, entity_manager->entity_hierarchy.length);
        IntrusiveListNode* node = &entity_manager->entity_hierarchy.node[entity_idx];

        line_idx += 1;
        
        // go further into tree.
        if(node->first_child != 0 && !is_asset_entity){
            editor_save_level_entity_recurssive(
                entity_manager, file_path, entity_char_buffer, char_scratch_space, char_scratch_space_length, node->first_child, line_idx, line_idx
            );
        }

        // TODO: in release, dont do this!
        if(line_idx == 1){
            char* chars = (char*)platform_alloc_memory(platform_get_max_file_path_length());
            String dst_file_path = {.chars = chars, .length = platform_get_max_file_path_length()};
            string_push_chars(&dst_file_path, "../", 3);
            string_push(&dst_file_path, *file_path);
            platform_copy_file(dst_file_path, *file_path, false);
            platform_free_memory(chars);
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
        
        i32 char_scratch_space_length;
        char* char_scratch_space;
        MEMORY_ARENA_ALLOC_ARRAY(ctx->transient, char_scratch_space, &char_scratch_space_length, 512);
                
        // debug build path.
        String file_path_scratch_space;
        string_init(&file_path_scratch_space, ctx->transient, platform_get_max_file_path_length());
        
        editor_save_level_entity_recurssive(
            &ctx->game_state->entity_manager,
            &file_path_scratch_space,
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
    EDITOR_GFX_CLAY_SIDE_PANEL(){

        if(Clay_Hovered()){
            editor_state->mouse_state.hovering_element = true;
        }

        f32 button_width = 0.2f;
        f32 button_height = 1.0f;

        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
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
                EDITOR_GFX_CLAY_BUTTON(name, button_width, button_height, editor_on_hover_select_level_button, ctx_ptr);
            }
        }

        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Wall"), button_width, button_height, transient, editor_state, EntityTypeId_Wall);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Cue"), button_width, button_height, transient, editor_state, EntityTypeId_BallCue);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball One"), button_width, button_height, transient, editor_state, EntityTypeId_BallOne);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Two"), button_width, button_height, transient, editor_state, EntityTypeId_BallTwo);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Three"), button_width, button_height, transient, editor_state, EntityTypeId_BallThree);
        }
        
        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Four"), button_width, button_height, transient, editor_state, EntityTypeId_BallFour);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Five"), button_width, button_height, transient, editor_state, EntityTypeId_BallFive);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Six"), button_width, button_height, transient, editor_state, EntityTypeId_BallSix);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Seven"), button_width, button_height, transient, editor_state, EntityTypeId_BallSeven);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Eight"), button_width, button_height, transient, editor_state, EntityTypeId_BallEight);
        }

        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Nine"), button_width, button_height, transient, editor_state, EntityTypeId_BallNine);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Ball Zero"), button_width, button_height, transient, editor_state, EntityTypeId_BallZero);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Pocket"), button_width, button_height, transient, editor_state, EntityTypeId_Pocket);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Boost Pad"), button_width, button_height, transient, editor_state, EntityTypeId_BoostPad);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Windmill"), button_width, button_height, transient, editor_state, EntityTypeId_WindMill);
        }
        
        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("Thwamper"), button_width, button_height, transient, editor_state, EntityTypeId_Thwamper);
            GFX_CLAY_SPAWN_ENTITY_BUTTON(CLAY_STRING("World Camera"), button_width, button_height, transient, editor_state, EntityTypeId_WorldCamera);
        }

        EDITOR_GFX_CLAY_ROW_BUTTON_CONTAINER(0.1f){
            // for some reason this only works down here?
            // the save button user data gets corrupted for some reason when above the the earlier button container?
            SaveLevelButtonOnHoverContext ctx = {.editor_state = editor_state, .game_state = game_state, .transient = transient};
            SaveLevelButtonOnHoverContext* ctx_ptr;
            MEMORY_ARENA_PUSH_STRUCT(transient, ctx, ctx_ptr);
            EDITOR_GFX_CLAY_BUTTON(CLAY_STRING("Save Level"), button_width, button_height, editor_on_hover_save_level_button, ctx_ptr);
        }
    }
}

void gfx_clay_entity_inspector_layout(Editor_State* editor_state, GameState* game_state, MemoryArena* transient){

    // hoist invariance.
    EntityManager* entity_manager = &game_state->entity_manager;
    ClickableAreaHierarchy* clickable_area_hierarchy = &editor_state->game_entity_clickable_area_hierarchy;

    EDITOR_GFX_CLAY_SIDE_PANEL(){

        if(Clay_Hovered()){
            editor_state->mouse_state.hovering_element = true;
        }

        ClickableArea* clickable_area;
        if(clickable_area_hierarchy_get_selected_area(clickable_area_hierarchy, &clickable_area)){

            Entity* entity = (Entity*)clickable_area->user_data;

            char* chars;
                        
            i32 val_label_max_chars = 64;
            i32 string_label_max_chars = 64;
            
            
            // allocator_entity_name_strings();
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_STRING_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String entity_name_val_string = {.chars = (const char*) chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, string_label_max_chars);
            Clay_String entity_name_label_string = {.chars = (const char *) chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_STRING_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String serialisation_file_path_val_string = {.chars = (const char*) chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, string_label_max_chars);
            Clay_String serialisation_file_label_string = {.chars = (const char*) chars};
            
            // allocate_floating_point_strings();            
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String position_x_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String position_x_label_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String position_y_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String position_y_label_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String scale_x_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String scale_x_label_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String scale_y_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String scale_y_label_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String sine_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String sine_label_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH);
            Clay_String cosine_val_string = {.chars = (const char*)chars};
            
            MEMORY_ARENA_ALLOC_MEMORY(transient, chars, val_label_max_chars);
            Clay_String cosine_label_string = {.chars = (const char*)chars};

            f32 horizontal_percent_size = 0.75f;
            f32 vertical_percent_size = 0.05f;
            
            i32 entity_idx = clickable_area_hierarchy->selected_idx;

            { // draw_name_tag();
            
                { // draw_label();

                    entity_name_label_string.length = 11;
                    char* label_chars = (char*)entity_name_label_string.chars;
                    COPY_MEMORY(label_chars, "Entity Name", entity_name_label_string.length);
                    CLAY_TEXT(entity_name_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);                
                }
                
                { // draw_value();
                
                    entity_name_val_string.length = entity->name.count;
                    char* label_chars = (char*)entity_name_val_string.chars;
                    COPY_MEMORY(label_chars, entity->name.chars, entity_name_val_string.length);
                
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Name"),
                        entity_name_val_string,
                        horizontal_percent_size,
                        vertical_percent_size,
                        editor_state, 
                        (void*)(&entity->name), 
                        Editor_ButtonDataStorageType_String, 
                        transient,
                        entity_idx
                    );
                }            
            }

            { // draw_position_x();
            
                { // draw_label();
                    position_x_label_string.length = 10;
                    char* label_chars = (char*)position_x_label_string.chars;
                    COPY_MEMORY(label_chars, "Position X", position_x_label_string.length);
                    CLAY_TEXT(position_x_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
                
                { // draw_value();
                    position_x_val_string.length = 
                    snprintf((char*)position_x_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.position.x);
                    ASSERT(position_x_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Position X"), 
                        position_x_val_string, 
                        horizontal_percent_size, 
                        vertical_percent_size, 
                        editor_state, 
                        (void*)(&entity->local_transform.position.x), 
                        Editor_ButtonDataStorageType_F32, 
                        transient, 
                        entity_idx
                    );
                }
            
            }
             
            { // draw_position_y();
            
                { // draw_label();
                    position_y_label_string.length = 10;
                    char* label_chars = (char*)position_y_label_string.chars;
                    COPY_MEMORY(label_chars, "Position Y", position_y_label_string.length);
                    CLAY_TEXT(position_y_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
            
                { // draw_value();
                    position_y_val_string.length = 
                        snprintf((char*)position_y_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.position.y);
                    ASSERT(position_y_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Position Y"), 
                        position_y_val_string, 
                        horizontal_percent_size, 
                        vertical_percent_size, 
                        editor_state, 
                        (void*)(&entity->local_transform.position.y), 
                        Editor_ButtonDataStorageType_F32, 
                        transient, 
                        entity_idx
                    );
                }
            }
            
            { // draw_scale_x();
                
                { // draw_label();
                    scale_x_label_string.length = 7;
                    char* label_chars = (char*)scale_x_label_string.chars;
                    COPY_MEMORY(label_chars, "Scale X", scale_x_label_string.length);
                    CLAY_TEXT(scale_x_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
                
                { // draw_value();
                    scale_x_val_string.length = snprintf((char*)scale_x_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.scale.x);
                    ASSERT(scale_x_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Scale X"), 
                        scale_x_val_string, 
                        horizontal_percent_size, 
                        vertical_percent_size,
                        editor_state, 
                        (void*)(&entity->local_transform.scale.x), 
                        Editor_ButtonDataStorageType_F32, 
                        transient,
                        entity_idx
                    );
                }
            }            
        
            { // draw_scale_y();
                
                { // draw_label();
                    scale_y_label_string.length = 7;
                    char* label_chars = (char*)scale_y_label_string.chars;
                    COPY_MEMORY(label_chars, "Scale Y", scale_y_label_string.length);
                    CLAY_TEXT(scale_y_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
                
                { // draw_value();
                    scale_y_val_string.length = snprintf((char*)scale_y_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.scale.y);
                    ASSERT(scale_y_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Scale Y"), 
                        scale_y_val_string, 
                        horizontal_percent_size, 
                        vertical_percent_size,
                        editor_state, 
                        (void*)(&entity->local_transform.scale.y), 
                        Editor_ButtonDataStorageType_F32, 
                        transient, 
                        entity_idx
                    );
                }            
            }
            
            { // draw_sine();
            
                { // draw_label();
                    sine_label_string.length = 4;
                    char* label_chars = (char*)sine_label_string.chars;
                    COPY_MEMORY(label_chars, "Sine", sine_label_string.length);
                    CLAY_TEXT(sine_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
            
                { // draw_value();
                    sine_val_string.length = snprintf((char*)sine_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.sine);
                    ASSERT(sine_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Rotation"), 
                        sine_val_string, 
                        horizontal_percent_size,
                        vertical_percent_size,
                        editor_state, 
                        (void*)(&entity->local_transform.sine), 
                        Editor_ButtonDataStorageType_F32, 
                        transient, 
                        entity_idx
                    );
                }
            }
            
            { // draw_cosine();
                
                { // draw_label();
                    cosine_label_string.length = 6;
                    char* label_chars = (char*)cosine_label_string.chars;
                    COPY_MEMORY(label_chars, "Cosine", cosine_label_string.length);
                    CLAY_TEXT(cosine_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);
                }
                
                { // draw_value();

                    // cosine.
                    cosine_val_string.length = snprintf((char*)cosine_val_string.chars, EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH, "%f", entity->local_transform.cosine);
                    ASSERT(cosine_val_string.length > 0, "failed to encode float.");
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Entity Rotation"), 
                        cosine_val_string, 
                        horizontal_percent_size, 
                        vertical_percent_size, 
                        editor_state, 
                        (void*)(&entity->local_transform.cosine), 
                        Editor_ButtonDataStorageType_F32, 
                        transient, 
                        entity_idx
                    );
                }
            }
            
            { // draw_serialisation_file_path();
            
                { // draw_label();

                    serialisation_file_label_string.length = 23;
                    char* label_chars = (char*)serialisation_file_label_string.chars;
                    COPY_MEMORY(label_chars, "Serialisation File Path", serialisation_file_label_string.length);
                    CLAY_TEXT(serialisation_file_label_string, EDITOR_GFX_CLAY_DEFAULT_TEXT_CONFIG);                
                }
                
                { // draw_value();
                
                    serialisation_file_path_val_string.length = entity->serialisation_file_path.count;
                    char* label_chars = (char*)serialisation_file_path_val_string.chars;
                    COPY_MEMORY(label_chars, entity->serialisation_file_path.chars, serialisation_file_path_val_string.length);
                
                    GFX_CLAY_INPUT_TEXT_BOX(
                        CLAY_STRING("Serialisation File Path"),
                        serialisation_file_path_val_string,
                        horizontal_percent_size,
                        vertical_percent_size,
                        editor_state, 
                        (void*)(&entity->serialisation_file_path),
                        Editor_ButtonDataStorageType_String, 
                        transient,
                        entity_idx
                    );
                }            
            }
        }
    }
}


///
/// functions: Editor_State
///


void editor_state_update(Editor_State* editor_state, GameState* game_state, MemoryArena* transient, f32 delta_time){

    // hoisting invariance.
    Editor_MouseState*      editor_mouse_state = &editor_state->mouse_state;
    GFX_State*              editor_gfx_state = editor_state->gfx_state;
    EntityManager*          game_entity_manager = &game_state->entity_manager;
    GFX_State*              game_gfx_state = game_entity_manager->gfx_state;
    ClickableAreaHierarchy* clickable_area_hierarchy = &editor_state->game_entity_clickable_area_hierarchy;

    // retrieve necessary data.
    Vector2 mouse_world_position = gfx_get_mouse_world_position(game_gfx_state);

    /*
        clear from previous run.

        NOTE:
        you need to reset the global element_id as Clay SIDI on hover callbacks requires that
        elements retain their ID numbers, without this, its impossible for clay to properly handle OnHover callbacks.
    */
    editor_gfx_clay_element_id = 0;

    editor_state->mouse_state.hovering_element = false;

    { // ui drawing.

        Vector2I mouse_backbuffer_position;
        input_get_mouse_position(&mouse_backbuffer_position.x, &mouse_backbuffer_position.y);
        Clay_SetCurrentContext(editor_gfx_state->clay_editor_ui_ctx);
        gfx_clay_begin_layout(
            editor_gfx_state, (Vector2I){.x = editor_gfx_state->window_ctx->width, .y = editor_gfx_state->window_ctx->height}, mouse_backbuffer_position, delta_time,
            input_is_mouse_button_pressed(MouseButton_Left)
        );
        
        switch(editor_state->menu){
            case Editor_Menu_EntitySpawner:{
                gfx_clay_entity_spawner_layout(editor_state, game_state, transient);
            }break;
            case Editor_Menu_EntityInspector:{
                gfx_clay_entity_inspector_layout(editor_state, game_state, transient);
            }break;
        }
        gfx_clay_end_layout(editor_gfx_state, delta_time, SPRITE_LAYER_GAME_UI, VIRTUAL_TEXTURE_ID_FONT, SPRITE_MATERIAL_TEXT, SPRITE_MATERIAL_DEBUG);
    }

    { // set_clickable_area_hierarchy_areas();

        for(i32 idx = 1; idx < game_entity_manager->entity_length; idx++){
            u64 depth_layer = 0;
            PolygonRectangle rectangle = {0};
            void* user_data = NULL;
            
            // get_clickable_area();
            if(entity_manager_is_entity_allocated_unsafe(*game_entity_manager, idx)){
            
                Transform2D preferred_transform = {0};
                
                Entity* entity = &game_entity_manager->entity[idx];
                user_data = entity;

                // note that we infer this is a sprite; no need to check 'is_sprite' flag or 'sprite_gid'.
                if(gfx_sprite_get_transform(*game_gfx_state, entity->sprite_gid, &preferred_transform)){

                    GFX_SpriteOrigin sprite_origin;

                    // falback to entity transform if we failed to get the sprite origin.
                    if(!gfx_sprite_get_sprite_origin(*game_gfx_state, entity->sprite_gid, &sprite_origin)){
                        preferred_transform = entity->global_transform;
                        rectangle = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                    }

                    switch(sprite_origin){
                        default:
                            ASSERT(false, "unknown sprite origin");
                        case GFX_SpriteOrigin_Center:{
                            rectangle = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                        }break;
                        case GFX_SpriteOrigin_TopLeft:{
                            rectangle = transform2d_to_polygon_rectangle_top_left_origin(preferred_transform);
                        }break;
                    }
                    gfx_sprite_get_depth_layer(game_gfx_state, entity->sprite_gid, &depth_layer);
                }

                // is not a sprite.
                else{
                    preferred_transform = entity->global_transform;
                    rectangle = transform2d_to_polygon_rectangle_centered_origin(preferred_transform);
                }
            }
        
            { // set_clickable_area();
            
                clickable_area_hierarchy_set_clickable_area_user_data_unsafe(
                    clickable_area_hierarchy,
                    idx,
                    user_data
                );
                
                clickable_area_hierarchy_set_clickable_area_rectangle_shape_unsafe(
                    clickable_area_hierarchy,
                    idx,
                    rectangle.x,
                    rectangle.y
                );
            
                clickable_area_hierarchy_set_clickable_area_depth_layer_unsafe(
                    clickable_area_hierarchy,
                    idx,
                    depth_layer
                );
            }
        }
    }

    { // input_handling();

        if(editor_state->is_typing){
            
            Key key = input_get_latest_active_key();

            String* scratch_space;
            switch(editor_mouse_state->selected_editor_ui_entity.data_storage_type){
                case Editor_ButtonDataStorageType_String:{
                    scratch_space = &editor_state->string_input_text_box_scratch_space;
                }break;
                case Editor_ButtonDataStorageType_F32:{
                    scratch_space = &editor_state->f32_input_text_box_scratch_space;
                }break;
            }
            
            i32 count = input_get_latest_active_key_print_count();
            while(count){

            
                if(key == Key_Backspace){
                    string_pop(scratch_space);
                }
                else{
                    char typed_char = input_get_latest_active_key_character();
                    if(typed_char != '\0'){
                        string_push_chars(scratch_space, &typed_char, 1);
                    }
                }
                count-=1;
            }
                    
            if(
                input_is_mouse_button_just_pressed(MouseButton_Left) ||
                input_is_mouse_button_just_pressed(MouseButton_Right)
            ){
                
                // apply changes.
                if(input_is_mouse_button_just_pressed(MouseButton_Left)){
    
                    switch(editor_mouse_state->selected_editor_ui_entity.data_storage_type){
                        default: {ASSERT(false, "unknown data storage type.");}break;
                        case Editor_ButtonDataStorageType_F32:{
                            
                            char* null_terminated_scratch_space = (char[EDITOR_F32_INPUT_BUTTON_MAX_STRING_LENGTH]){0};
                            COPY_MEMORY(null_terminated_scratch_space, scratch_space->chars, scratch_space->count);
                            
                            sscanf(
                                null_terminated_scratch_space, "%f", 
                                (f32*)editor_mouse_state->selected_editor_ui_entity.data_storage
                            );
                        }break;
                        case Editor_ButtonDataStorageType_String:{
                            String* data_string = (String*)editor_mouse_state->selected_editor_ui_entity.data_storage;
                            string_clear(data_string);
                            string_push(data_string, *scratch_space);
                        }break;
                    }
                    
                    if(editor_mouse_state->selected_editor_ui_entity.entity_idx){
                    
                        // refresh the local transform because it might have been updated.
                        i32 entity_idx = editor_mouse_state->selected_editor_ui_entity.entity_idx;
                        BOUNDS_CHECK(entity_idx, game_entity_manager->entity_length);
                        Transform2D local_transform = game_entity_manager->entity[entity_idx].local_transform;
                        entity_set_local_transform_unsafe(game_entity_manager, entity_idx, local_transform);
                    }
                }

                editor_state->is_typing = false;
                editor_mouse_state->selected_editor_ui_entity = (Editor_SelectedEditorUiEntity){0};
                string_clear(scratch_space);
            }
        }
        else{
            if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_NAVIGATE_KEYBINDING)){
                editor_mouse_state->current_behaviour = Editor_MouseStateBehaviour_Navigate;
            }
            if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_MOVE_KEYBINDING)){
                editor_mouse_state->current_behaviour = Editor_MouseStateBehaviour_Move;
            }
            if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_SCALE_KEYBINDING)){
                editor_mouse_state->current_behaviour = Editor_MouseStateBehaviour_Scale;
            }
            if(input_is_key_just_pressed(EDITOR_MOUSE_BEHAVIOUR_ROTATE_KEYBINDING)){
                editor_mouse_state->current_behaviour = Editor_MouseStateBehaviour_Rotate;
            }
            if(input_is_key_just_pressed(Key_D)){
                entity_manager_dealloc_entity(game_entity_manager, editor_state->slected_game_entity_gid);
                editor_state->slected_game_entity_gid = 0;
            }
            
            
            { // camera_input();
        
                
                f32 camera_move_speed = 1.334f * delta_time * editor_state->world_camera_orthographic_size;
                f32 camera_zoom_speed = 0.156f * delta_time * editor_state->world_camera_orthographic_size;
                
                { // movement();

                    if(input_is_mouse_button_pressed(MouseButton_Middle)){
                        Vector2 movement = vector2_mul_val(gfx_get_mouse_delta_world_position(game_gfx_state), camera_move_speed);
                        editor_state->world_camera_position = vector2_sub(editor_state->world_camera_position, movement);
                    }
                }

                { // zoom();

                    editor_state->world_camera_orthographic_size -= input_get_mouse_scroll_wheel_delta_value() * camera_zoom_speed;
                }
            }
            
            if(!editor_state->mouse_state.hovering_element){
            
                switch(editor_mouse_state->current_behaviour){
                    case Editor_MouseStateBehaviour_Navigate:{

                        // select_entity();  
                        if(input_is_mouse_button_just_pressed(MouseButton_Left)){
                            clickable_area_hierarchy_update(
                                clickable_area_hierarchy,
                                mouse_world_position
                            );
                            
                            i32 selected_entity_idx = clickable_area_hierarchy->selected_idx;
                            if(clickable_area_hierarchy->selected_idx){
                                BOUNDS_CHECK(selected_entity_idx, game_entity_manager->gen_id_allocator.length);
                                editor_state->slected_game_entity_gid = game_entity_manager->gen_id_allocator.gen_ids[selected_entity_idx];
                            }
                        }
        
                        // handle entity spawning if we are not moving a selected entity around.
                        if(
                            !clickable_area_hierarchy->selected_idx &&
                            input_is_mouse_button_just_pressed(MouseButton_Left)
                        ){
        
                            // check if mouse is within clickable area.
                            // get entity sprite layer; fallback to using id's if both are the same.
        
                            Transform2D spawn_transform = TRANSFORM2D_IDENTITY;
                            spawn_transform.position = mouse_world_position; // we use the game state's gfx_state; just in case.
        
                            if(editor_state->mouse_state.entity_to_spawn_parent_gid == 0){
                                return;
                            }
        
                            entity_spawn(
                                game_entity_manager, 
                                editor_mouse_state->entity_to_spawn,
                                (String){.chars = "entity", .length = 6, .count = 6}, 
                                spawn_transform, 
                                editor_state->mouse_state.entity_to_spawn_parent_gid
                            );        
                        }
                    }break;
                    case Editor_MouseStateBehaviour_Move:{
                        
                        Entity* game_entity;
                        if(!entity_manager_get_entity(*game_entity_manager, editor_state->slected_game_entity_gid, &game_entity)){
                            // failed to retrieve selected entity data.
                            break;
                        }
    
                        i32 entity_idx = gen_id_get_index(editor_state->slected_game_entity_gid);
                        
                        // this can be 0 because of deletion.
                        if(!entity_idx){
                            break;
                        }
                        
                        ClickableArea* clickable_area;
                        clickable_area_hierarchy_get_clickable_area_unsafe(clickable_area_hierarchy, entity_idx, &clickable_area);
                                                        
                        // check if we have just clicked the entity.
                        if(input_is_mouse_button_just_pressed(MouseButton_Left)){
                            Vector2 normal;
                            f32 depth;
                            bool overlaps = 
                                polygon_overlaps_point_scalar(
                                    clickable_area->rectangle.x, clickable_area->rectangle.y, 4,
                                    mouse_world_position.x, mouse_world_position.y,
                                    &normal.x, &normal.y, &depth
                                );
                            
                            // store the on click information.
                            if(overlaps){
                                editor_mouse_state->move_behaviour_state.selected_game_entity_positional_offset 
                                    = vector2_sub(game_entity->global_transform.position, mouse_world_position);
                                editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity = true;
                            } 
                        }
                        else if(input_is_mouse_button_just_released(MouseButton_Left)){
                            editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity = false;
                        }
                        
                        // dont do anything if we are not clicking the entity at all.
                        if(!editor_mouse_state->move_behaviour_state.is_clicking_selected_game_entity){
                            break;
                        }
                        
                        // drag the selected entity with the mouse if we are clicking down.
                        if(input_is_mouse_button_pressed(MouseButton_Left)){                                            
                            Vector2 new_position = vector2_add(mouse_world_position, editor_mouse_state->move_behaviour_state.selected_game_entity_positional_offset);
                            Transform2D new_transform = game_entity->global_transform;
                            new_transform.position = new_position;
                            entity_set_global_transform_unsafe(game_entity_manager, entity_idx, new_transform);
                        }
                    }break;
                    case Editor_MouseStateBehaviour_Rotate:{
                        GenId game_entity_gid = editor_state->slected_game_entity_gid;
                        Entity* game_entity;
                        if(!entity_manager_get_entity(*game_entity_manager, game_entity_gid, &game_entity)){
                            // failed to retrieve selected entity.
                            break;
                        }
                        
                        i32 entity_idx = gen_id_get_index(game_entity_gid);
                        
                        Vector2 diff = vector2_sub(game_entity->global_transform.position, mouse_world_position);
                        
                        if(editor_mouse_state->previous_behaviour == Editor_MouseStateBehaviour_Rotate){
                            Vector2 prev_diff = editor_mouse_state->rotate_behaviour_state.previous_positional_difference;
                            f32 radians = vector2_radian_difference_scalar(diff.x, diff.y, prev_diff.x, prev_diff.y);
                            entity_set_local_transform_unsafe(game_entity_manager, entity_idx, transform2d_rotate(game_entity->local_transform, radians));
                        }
    
                        editor_mouse_state->rotate_behaviour_state.previous_positional_difference = diff;
                    }break;
                }
            }
        }
    }
        
    // { // set entity transforms; specifcally to update sprite or physics body data.

    //     for(i32 i = 0; i < game_entity_manager->entity_length; i++){
    //         BOUNDS_CHECK(i, game_entity_manager->gen_id_allocator.length);
    //         if(!game_entity_manager->gen_id_allocator.allocated[i]){
    //             continue;
    //         }
    //         GenId game_entity_gid = game_entity_manager->gen_id_allocator.gen_ids[i];
    //         Entity* game_entity = &game_entity_manager->entity[i];
    //         entity_set_transform(game_entity_manager, game_entity_gid, game_entity->transform);
    //     }
    // }
    
    // debug_gizmos();
    {
        i32 selected_game_entity_idx = -1;
        if(editor_state->slected_game_entity_gid){
            selected_game_entity_idx = gen_id_get_index(editor_state->slected_game_entity_gid);
        }
    
        for(i32 i = 1; i < game_entity_manager->entity_length; i++){
                        
            if(!entity_manager_is_entity_allocated_unsafe(*game_entity_manager, i)){
                continue;
            }
                        
            Entity* game_entity = &game_entity_manager->entity[i];
                        
            if(selected_game_entity_idx == i){
                switch(editor_mouse_state->current_behaviour){
                    case Editor_MouseStateBehaviour_Move:{
                        gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_LIGHT_BLUE, game_entity->global_transform.position, VECTOR2_UP, 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
                        gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_PINK, game_entity->global_transform.position, VECTOR2_RIGHT, 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
                        gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_ORANGE, game_entity->global_transform.position, transform2d_get_forward_angle_vector(game_entity->global_transform), 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
                    }break;
                    case Editor_MouseStateBehaviour_Rotate:{
                        gfx_draw_arrow(editor_gfx_state, GFX_COLOUR_ORANGE, game_entity->global_transform.position, transform2d_get_forward_angle_vector(game_entity->global_transform), 1.0f, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG);
                        gfx_draw_line(editor_gfx_state, GFX_COLOUR_ORANGE, game_entity->global_transform.position, mouse_world_position, SPRITE_LAYER_EDITOR_WORLD, 0, SPRITE_MATERIAL_DEBUG, 0.05f);
                    }
                }
            }
                
            ClickableArea* clickable_area;
            clickable_area_hierarchy_get_clickable_area_unsafe(clickable_area_hierarchy, i, &clickable_area);
            // draw the clickable area.
            i32 sprite_depth = 100;
            gfx_draw_wire_poly(
                editor_gfx_state,
                clickable_area->rectangle.x,
                clickable_area->rectangle.y,
                EDITOR_ENTITY_CLICKABLE_AREA_VERTEX_LENGTH,
                selected_game_entity_idx == i ? EDITOR_ENTTY_CLICKABLE_AREA_ACTIVE_COLOUR : EDITOR_ENTTY_CLICKABLE_AREA_PASSIVE_COLOUR,
                SPRITE_LAYER_EDITOR_WORLD,
                sprite_depth,
                SPRITE_MATERIAL_DEBUG
            );
        }
    }

    { // prepare_for_next_frame();
    
        editor_mouse_state->previous_behaviour = editor_mouse_state->current_behaviour;
    
    }
}