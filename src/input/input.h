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
    Key_None,
    Key_Backspace,
    Key_Tab,
    Key_Enter,
    Key_Escape,
    Key_Space,
    Key_PageUp,
    Key_PageDown,
    Key_End,
    Key_Home,
    Key_Left,
    Key_Up,
    Key_Right,
    Key_Down,
    Key_Select,
    Key_PrintScreen,
    Key_Insert,
    Key_Delete,
    Key_Help,
    Key_Pause,
    Key_Equals,
    Key_Minus,
    Key_LeftBracket,
    Key_RightBracket,
    Key_Backslash,
    Key_Semicolon,
    Key_Apostrophe,
    Key_BackTick,
    Key_Comma,
    Key_Period,
    Key_Slash,
    Key_Capslock,

    // Digit Keys (0-9)
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    // Letter Keys (A-Z)
    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    // NumPad keys
    Key_NP0,
    Key_NP1,
    Key_NP2,
    Key_NP3,
    Key_NP4,
    Key_NP5,
    Key_NP6,
    Key_NP7,
    Key_NP8,
    Key_NP9,

    // NumPad operators
    Key_NPMultiply,
    Key_NPAdd,
    Key_NPSeparator,
    Key_NPSubtract,
    Key_NPDecimal,
    Key_NPDivide,

    // Function keys
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,

    // Modifier Keys
    Key_LeftShift,
    Key_RightShift,
    Key_LeftControl,
    Key_RightControl,
    Key_LeftAlt,
    Key_RightAlt,

    Key_EnumSize
} Key;

typedef enum{
    MouseButton_Left,
    MouseButton_Right,
    MouseButton_Middle,

    MouseButton_EnumSize
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
void input_update(f32 delta_time);
void input_init(MemoryArena* arena, f32 latest_active_key_print_delay, f32 latest_active_key_print_rate);

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
f32 input_key_get_elapsed_pressed_time(Key key);

/*
    `returns`
    the character associated with the latest active key (E.g, 'A', 'b', '1', '@'), otherwise empty '' if there is no associated character.
*/
char input_get_latest_active_key_character();
i32 input_get_latest_active_key_print_count();
Key input_get_latest_active_key();

bool input_is_middle_mouse_down();
void input_set_middle_mouse_down();
void input_set_middle_mouse_up();

#endif