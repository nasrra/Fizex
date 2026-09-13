/*
    Disable MSVC's implicit cast warnings as Nic Barker wrote this and I trust him.
*/
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
    #define CLAY_IMPLEMENTATION
    #include "vendors/clay/clay.h"
#pragma warning(pop)


const Clay_Color COLOR_LIGHT = {224, 215, 210, 255};
const Clay_Color COLOR_RED = {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = {225, 138, 50, 255};

void gfxclay_handle_errors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    switch(errorData.errorType) {
        default:{ASSERT(false, errorData.errorText.chars);}break;
    }
}

// Example measure text function
static inline Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    // Clay_TextElementConfig contains members such as fontId, fontSize, letterSpacing etc
    // Note: Clay_String->chars is not guaranteed to be null terminated
    return (Clay_Dimensions) {
            .width = (f32)(text.length * config->fontSize), // <- this will only work for monospace fonts, see the renderers/ directory for more advanced text measurement
            .height = config->fontSize
    };
}

// Re-useable components are just normal functions
void SidebarItemComponent() {

    // Layout config is just a struct that can be declared statically, or inline
    Clay_ElementDeclaration sidebarItemConfig = {
        .layout = {
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
        },
        .backgroundColor = COLOR_ORANGE
    };

    // CLAY(id, sidebarItemConfig) {
    //     // children go here...
    // }
}

void gfxclay_init(size_t clay_arena_size, i32 screen_height, i32 screen_width){
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(clay_arena_size, platform_alloc_memory(clay_arena_size));
    Clay_Initialize(
        arena, 
        (Clay_Dimensions){(f32)screen_width, (f32)screen_height}, 
        (Clay_ErrorHandler){gfxclay_handle_errors}
    );
}


GFX_Colour gfxclay_clay_to_gfx_colour(Clay_Color clay_colour){
    clay_colour.r /= 255.0f;
    clay_colour.g /= 255.0f;
    clay_colour.b /= 255.0f;
    clay_colour.a /= 255.0f;
    return (GFX_Colour){
        .r = clay_colour.r,
        .g = clay_colour.g,
        .b = clay_colour.b,
        .a = clay_colour.a
    };
}

Rectangle gfxclay_clay_to_rectangle(Clay_BoundingBox box){
    return(Rectangle){
        .x = box.x,
        .y = -box.y,
        .width = box.width - box.x,
        .height = box.height - box.y
    };
}

void gfxclay_test_layout(){
    CLAY(CLAY_ID("Box"), {
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_GROW(0)
            },
            .padding = CLAY_PADDING_ALL(24),
        },
        .backgroundColor = { 10, 10, 20, 128},
    }){
        CLAY(CLAY_ID("ChildA"), {
            .backgroundColor = {255.0f,255.0f,255.0f,255.0f},
            .layout = {
                .sizing = {
                    .width = CLAY_SIZING_GROW(0),
                    .height = CLAY_SIZING_GROW(0)
                }
            }
        }){}
    }
}

void gfxclay_update(GFX_State* gfx_state, Vector2I screen_resolution, Vector2I mouse_screen_position, f32 delta_time, bool is_mouse_down){
    Clay_SetLayoutDimensions((Clay_Dimensions) {(f32)screen_resolution.x, (f32)screen_resolution.y});
    Clay_SetPointerState((Clay_Vector2) {(f32)mouse_screen_position.x, (f32)mouse_screen_position.y}, is_mouse_down);
    // Clay_UpdateScrollContainers(true, (Clay_Vector2) { mouseWheelX, mouseWheelY }, deltaTime);

    // All clay layouts are declared between Clay_BeginLayout and Clay_EndLayout
    Clay_BeginLayout();
    {
        gfxclay_test_layout();
    }
    Clay_RenderCommandArray renderCommands = Clay_EndLayout(delta_time);

    // More comprehensive rendering examples can be found in the renderers/ directory
    for (int i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];

        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                GFX_Colour colour = gfxclay_clay_to_gfx_colour(renderCommand->renderData.rectangle.backgroundColor);
                Rectangle rect = gfxclay_clay_to_rectangle(renderCommand->boundingBox);
                gfx_draw_fill_rect(gfx_state, rect, colour, (f32)i+1, SPRITE_LAYER_UI, SPRITE_MATERIAL_DEBUG);

            }break;
            default:{
                ASSERT(false, "attempted to use unimplemented clay feature!");
            }break;
        }
    }
}