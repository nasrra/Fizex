#include "input.h"



///
/// globals.
///




bool input_is_init;
bool* input_key_down_state;
InputState* input_curr_key_state;
InputState* input_prev_key_state;
bool* input_mouse_button_down_state;
InputState* input_curr_mouse_button_state;
InputState* input_prev_mouse_button_state;

i32 input_mouse_position_x;
i32 input_mouse_position_y;
i32 input_mouse_delta_position_x;
i32 input_mouse_delta_position_y;
i32 input_mouse_previous_delta_position_x;
i32 input_mouse_previous_delta_position_y;

i32 input_mouse_scroll_wheel_value;
i32 input_mouse_scroll_wheel_delta_value;
i32 input_mouse_scroll_wheel_previous_delta_value;




///
/// functions.
///




void input_set_key_up(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    input_key_down_state[(size_t)key] = false;
}

void input_set_key_down(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    input_key_down_state[(size_t)key] = true;
}

void input_set_mouse_button_down(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    input_mouse_button_down_state[(size_t)button] = true;
}

void input_set_mouse_button_up(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    input_mouse_button_down_state[(size_t)button] = false;
}

bool input_is_key_pressed(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_PRESSED || state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_just_pressed(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_released(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_RELEASED || state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_key_just_released(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, KEY_ENUM_SIZE);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_pressed(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_PRESSED || state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_just_pressed(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_released(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_RELEASED || state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_just_released(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MOUSE_BUTTON_ENUM_SIZE);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_JUST_RELEASED;
}

void input_update(){

    { // validation.
        ASSERT(input_is_init, "input is not init");
    }
    
    { // handle_delta_values();

        if(input_mouse_previous_delta_position_x == input_mouse_delta_position_x){
            input_mouse_previous_delta_position_x = 0;
            input_mouse_delta_position_x = 0;
        }
        else{
            input_mouse_previous_delta_position_x = input_mouse_delta_position_x;
        }
    
        if(input_mouse_previous_delta_position_y == input_mouse_delta_position_y){
            input_mouse_previous_delta_position_y = 0;
            input_mouse_delta_position_y = 0;
        }
        else{
            input_mouse_previous_delta_position_y = input_mouse_delta_position_y;
        }
    
        if(input_mouse_scroll_wheel_previous_delta_value == input_mouse_scroll_wheel_delta_value){
            input_mouse_scroll_wheel_delta_value = 0;
            input_mouse_scroll_wheel_previous_delta_value = 0;
        }
        else{
            input_mouse_scroll_wheel_previous_delta_value = input_mouse_scroll_wheel_delta_value;
        }
    }

    /*
        keys.
    */
    for(size_t i = 0; i < (size_t)KEY_ENUM_SIZE; i++){
        InputState* last = &input_curr_key_state[i];
        InputState* next = &input_prev_key_state[i];
        switch(input_key_down_state[i]){
            case true:{
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:{
                        *next = INPUT_STATE_JUST_PRESSED;
                    }break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:{
                        *next = INPUT_STATE_PRESSED;
                    }break;
                    default:{
                        ASSERT(0!=0, "unknown input state");
                    }break;
                }
            }break;
            case false:{
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:{
                        *next = INPUT_STATE_RELEASED;
                    }break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:{
                        *next = INPUT_STATE_JUST_RELEASED;
                    }break;
                    default:{
                        ASSERT(0!=0, "unknown input state");
                    }break;
                }
            }break;
            default:{
                ASSERT(false, "inavlid input key down state.");
            }break;
        }
    }

    /*
        Mouse Buttons.
    */
    for(size_t i = 0; i < (size_t)MOUSE_BUTTON_ENUM_SIZE; i++){
        InputState* last = &input_curr_mouse_button_state[i];
        InputState* next = &input_prev_mouse_button_state[i];
        switch(input_mouse_button_down_state[i]){
            case true:{
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:{
                        *next = INPUT_STATE_JUST_PRESSED;
                    }break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:{
                        *next = INPUT_STATE_PRESSED;
                    }break;
                    default:{
                        ASSERT(0!=0, "unknown input state");
                    }break;
                }
            }break;
            case false:{
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:{
                        *next = INPUT_STATE_RELEASED;
                    }break;
                    case INPUT_STATE_PRESSED:
                    case INPUT_STATE_JUST_PRESSED:{
                        *next = INPUT_STATE_JUST_RELEASED;
                    }break;
                    default:{
                        ASSERT(0!=0, "unknown input state");
                    }break;
                }
            }break;
            default:{
                ASSERT(false, "inavlid input key down state.");
            }break;
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
    ASSERT(!input_is_init, "attempted to init an already init input system.");
    size_t temp;
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_key_down_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_key_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_key_state, &temp, (size_t)KEY_ENUM_SIZE);

    MEMORY_ARENA_ALLOC_ARRAY(arena, input_mouse_button_down_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_mouse_button_state, &temp, (size_t)KEY_ENUM_SIZE);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_mouse_button_state, &temp, (size_t)KEY_ENUM_SIZE);
    input_is_init = true;
}


///
/// mouse position.
///


void input_get_mouse_position(i32* out_pos_x, i32* out_pos_y){
    *out_pos_x = input_mouse_position_x;
    *out_pos_y = input_mouse_position_y;
}

void input_get_mouse_delta_position(i32* out_delta_x, i32* out_delta_y){
    *out_delta_x = input_mouse_delta_position_x;
    *out_delta_y = input_mouse_delta_position_y;
}

void input_set_mouse_position(i32 pos_x, i32 pos_y){
    input_mouse_delta_position_x = pos_x - input_mouse_position_x;
    input_mouse_delta_position_y = pos_y - input_mouse_position_y;
    input_mouse_position_x = pos_x;
    input_mouse_position_y = pos_y;
}


///
/// mouse wheel.
///


i32 input_get_mouse_scoll_wheel_value(){
    return input_mouse_scroll_wheel_value;
}

i32 input_get_mouse_scroll_wheel_delta_value(){
    return input_mouse_scroll_wheel_delta_value;
}

void input_set_mouse_scroll_wheel_value(i32 value){
    input_mouse_scroll_wheel_delta_value = value - input_mouse_scroll_wheel_value;
    input_mouse_scroll_wheel_value = value;
}

void input_increment_mouse_scroll_wheel_value(i32 scroll_amount){
    input_set_mouse_scroll_wheel_value(input_get_mouse_scoll_wheel_value() + scroll_amount);
}
