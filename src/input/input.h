#ifndef INPUT_H
#define INPUT_H

#include "../base_layer/base.h"

typedef enum {
    INPUT_STATE_RELEASED,
    INPUT_STATE_PRESSED,
    INPUT_STATE_JUST_RELEASED,
    INPUT_STATE_JUST_PRESSED,

    INPUT_STATE_ENUM_SIZE
} InputState;

typedef enum {
    // Basic Keys
    KEY_NONE,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_ENTER,
    KEY_ESCAPE,
    KEY_SPACE,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_END,
    KEY_HOME,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_SELECT,
    KEY_PRINT_SCREEN,
    KEY_INSERT,
    KEY_DELETE,
    KEY_HELP,
    KEY_PAUSE,
    KEY_EQUALS,
    KEY_MINUS,
    KEY_L_BRACKET,
    KEY_R_BRACKET,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_APOSTROPHE,
    KEY_BACK_TICK,
    KEY_COMMA,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_CAPSLOCK,

    // Digit Keys (0-9)
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    // Letter Keys (A-Z)
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    // NumPad keys
    KEY_NP_0,
    KEY_NP_1,
    KEY_NP_2,
    KEY_NP_3,
    KEY_NP_4,
    KEY_NP_5,
    KEY_NP_6,
    KEY_NP_7,
    KEY_NP_8,
    KEY_NP_9,

    // NumPad operators
    KEY_NP_MULTIPLY,
    KEY_NP_ADD,
    KEY_NP_SEPARATOR,
    KEY_NP_SUBTRACT,
    KEY_NP_DECIMAL,
    KEY_NP_DIVIDE,

    // Function keys
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,
    KEY_F20,
    KEY_F21,
    KEY_F22,
    KEY_F23,
    KEY_F24,

    // Modifier Keys
    KEY_L_SHIFT,
    KEY_R_SHIFT,
    KEY_L_CONTROL,
    KEY_R_CONTROL,
    KEY_L_ALT,
    KEY_R_ALT,

    KEY_ENUM_SIZE
} Key;

typedef enum{
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,

    MOUSE_BUTTON_ENUM_SIZE
} MouseButton;

typedef enum {
    // Dpad buttons
    GAMEPAD_BUTTON_DPAD_NORTH,
    GAMEPAD_BUTTON_DPAD_EAST,
    GAMEPAD_BUTTON_DPAD_SOUTH,
    GAMEPAD_BUTTON_DPAD_WEST,

    // Face buttons (X, Y, B, A, etc.)
    GAMEPAD_BUTTON_FACE_NORTH,
    GAMEPAD_BUTTON_FACE_EAST,
    GAMEPAD_BUTTON_FACE_SOUTH,
    GAMEPAD_BUTTON_FACE_WEST,

    // Shoulders
    GAMEPAD_BUTTON_SHOULDER_RIGHT,
    GAMEPAD_BUTTON_SHOULDER_LEFT,

    // Triggers
    GAMEPAD_BUTTON_TRIGGER_RIGHT,
    GAMEPAD_BUTTON_TRIGGER_LEFT,

    // Special
    GAMEPAD_BUTTON_START,
    GAMEPAD_BUTTON_MENU,

    // Thumbsticks
    GAMEPAD_BUTTON_LEFT_THUMBSTICK,
    GAMEPAD_BUTTON_RIGHT_THUMBSTICK,

    GAMEPAD_BUTTON_ENUM_SIZE
} GamePadButton;

void input_set_key_up(Key key);
void input_set_key_down(Key key);
void input_set_mouse_button_down(MouseButton button);
void input_set_mouse_button_up(MouseButton button);
bool input_is_key_pressed(Key key);
bool input_is_key_just_pressed(Key key);
bool input_is_key_released(Key key);
bool input_is_key_just_released(Key key);
bool input_is_mouse_button_pressed(MouseButton button);
bool input_is_mouse_button_just_pressed(MouseButton button);
bool input_is_mouse_button_released(MouseButton button);
bool input_is_mouse_button_just_released(MouseButton button);
void input_update();
void input_init(MemoryArena* arena);

input_mouse_get_position_relative(
    f32 dst_rect_x, f32 dst_rect_y, f32 dst_rect_width, f32 dst_rect_height,
    i32 dst_resolution_width, i32 dst_resolution_height,
    i32* out_mouse_x, i32* out_mouse_y
);
void input_set_mouse_position(i32 pos_x, i32 pos_y);
void input_get_mouse_position(i32* out_pos_x, i32* out_pos_y);
void input_get_mouse_delta_position(i32* out_delta_x, i32* out_delta_y);

i32 input_get_mouse_scoll_wheel_value();
i32 input_get_mouse_scroll_wheel_delta_value();
void input_set_mouse_scroll_wheel_value(i32 value);
void input_increment_mouse_scroll_wheel_value(i32 scroll_amount);

#endif