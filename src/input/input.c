// package input;

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
    KEY_EXECUTE,
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

/*========================================
    globals.
========================================*//**/

bool input_is_init;
bool* input_key_down_state;
InputState* input_curr_key_state;
InputState* input_prev_key_state;
bool* input_mouse_button_down_state;
InputState* input_curr_mouse_button_state;
InputState* input_prev_mouse_button_state;

void input_set_key_down(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    input_key_down_state[(size_t)key] = true;
}

void input_set_key_up(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    input_key_down_state[(size_t)key] = false;
}

void input_set_mouse_button_down(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    input_mouse_button_down_state[(size_t)button] = true;
}

void input_set_mouse_button_up(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    input_mouse_button_down_state[(size_t)button] = false;
}

bool input_is_key_pressed(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    return input_curr_key_state[index] == INPUT_STATE_PRESSED || INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_just_pressed(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    return input_curr_key_state[index] == INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_released(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    return input_curr_key_state[index] == INPUT_STATE_RELEASED || INPUT_STATE_JUST_RELEASED;
}

bool input_is_key_just_released(Key key){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    return input_curr_key_state[index] == INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_pressed(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    return input_curr_mouse_button_state[index] == INPUT_STATE_PRESSED || INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_just_pressed(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    return input_curr_mouse_button_state[index] == INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_released(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    return input_curr_mouse_button_state[index] == INPUT_STATE_RELEASED || INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_just_released(MouseButton button){
    DEBUG_ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    return input_curr_mouse_button_state[index] == INPUT_STATE_JUST_RELEASED;
}

void input_update(){
    { // validation.
        DEBUG_ASSERT(input_is_init, "input is not init");
    }

    /*
        keys.
    */
    for(size_t i = 0; i < (size_t)KEY_ENUM_SIZE; i++){
        InputState* last = &input_curr_key_state[i];
        InputState* next = &input_prev_key_state[i];
        switch(input_key_down_state[i]){
            case true:
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:
                        *last = INPUT_STATE_JUST_PRESSED;
                    break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:
                        *last = INPUT_STATE_PRESSED;
                    break;
                    default:
                        DEBUG_ASSERT(0!=0, "unknown input state");
                    break;
                }
            break;
            case false:
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:
                        *last = INPUT_STATE_RELEASED;
                    break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:
                        *last = INPUT_STATE_JUST_RELEASED;
                    break ;
                    default:
                        DEBUG_ASSERT(0!=0, "unknown input state");
                    break;
                }
            break;
        }
    }

    /*
        Mouse Buttons.
    */
    for(size_t i = 0; i < (size_t)MOUSE_BUTTON_ENUM_SIZE; i++){
        InputState* last = &input_curr_mouse_button_state[i];
        InputState* next = &input_prev_mouse_button_state[i];
        switch(input_mouse_button_down_state[i]){
            case true:
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:
                        *last = INPUT_STATE_JUST_PRESSED;
                    break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:
                        *last = INPUT_STATE_PRESSED;
                    break ;
                    default:
                        DEBUG_ASSERT(0!=0, "unknown input state");
                    break;
                }
            break;
            case false:
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:
                        *last = INPUT_STATE_RELEASED;
                    break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:
                        *last = INPUT_STATE_JUST_RELEASED;
                    break ;
                    default:
                        DEBUG_ASSERT(0!=0, "unknown input state");
                    break;
                }
            break;
        }
    }

    /*
        swap.
    */
    InputState* temp_key_state = input_curr_key_state;
    input_curr_key_state = input_prev_key_state;
    input_prev_key_state = temp_key_state;
    InputState* temp_mouse_button_state = input_curr_mouse_button_state;
    input_curr_mouse_button_state = input_prev_mouse_button_state;
    input_prev_mouse_button_state = temp_mouse_button_state;
}

void input_init(MemoryArena* arena){
    DEBUG_ASSERT(!input_is_init, "attempted to init an already init input system.");
    size_t temp;
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_key_down_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_key_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_key_state, &temp, (size_t)KEY_ENUM_SIZE);

    MEMORY_ARENA_ALLOC_ARRAY(arena, input_mouse_button_down_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_mouse_button_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_mouse_button_state, &temp, (size_t)KEY_ENUM_SIZE);
    input_is_init = true;
}