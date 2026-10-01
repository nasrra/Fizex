#include "input.h"



///
/// globals.
///




bool input_is_init;
bool* input_key_down_state;
InputState* input_curr_key_state;
InputState* input_prev_key_state;
// the total amount of elapsed time since the intial press of the input character.
f32* input_key_elapsed_pressed_time;
// the internal counter to check against when determining whether to print the held input character or not. 
i32* input_key_pressed_print_rate_counter;
// the internal counter to check against when determining whether to print the held input character or not. 
i32* input_key_previous_print_rate_counter;
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

// this the most recently pressed key.
// note that this only account for that singular key, there is no queue to pull from.
// for example: 
//      pressing '1' then '2', will store Key_2, 
//      but when '2' is let go (but '1' is still pressed) this will store Key_None.

Key input_latest_active_key;
// the amount of time elapsed for the print interval.
f32 input_latest_active_key_print_elapsed_time;
// the amount of prints that have occured this frame of the latest active key.
i32 input_latest_active_key_print_count;
// the amount of time between print intervals of the latest active key.
f32 input_latest_active_key_print_rate;
// the amount of time before printing the latest active key.
f32 input_latest_active_key_print_delay;


///
/// defines
///




// #define INPUT_EMPTY_CHAR ''




///
/// functions.
///




void input_set_key_up(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    input_key_down_state[(size_t)key] = false;
    if(input_latest_active_key == key){
        // clear stale data.
        input_latest_active_key = Key_None;
    }
}

void input_set_key_down(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    input_key_down_state[(size_t)key] = true;
    input_latest_active_key = key;
}

void input_set_mouse_button_down(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    input_mouse_button_down_state[(size_t)button] = true;
}

void input_set_mouse_button_up(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    input_mouse_button_down_state[(size_t)button] = false;
}

bool input_is_key_pressed(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_PRESSED || state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_just_pressed(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_key_released(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_RELEASED || state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_key_just_released(Key key){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)key;
    BOUNDS_CHECK(index, Key_EnumSize);
    InputState state = input_curr_key_state[index];
    return state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_pressed(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_PRESSED || state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_just_pressed(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_JUST_PRESSED;
}

bool input_is_mouse_button_released(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_RELEASED || state == INPUT_STATE_JUST_RELEASED;
}

bool input_is_mouse_button_just_released(MouseButton button){
    ASSERT(input_is_init, "input is not init");
    size_t index = (size_t)button;
    BOUNDS_CHECK(index, MouseButton_EnumSize);
    InputState state = input_curr_mouse_button_state[index];
    return state == INPUT_STATE_JUST_RELEASED;
}

void input_update(f32 delta_time){

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
    for(size_t i = 0; i < (size_t)Key_EnumSize; i++){
        InputState* last = &input_curr_key_state[i];
        InputState* next = &input_prev_key_state[i];
        switch(input_key_down_state[i]){
            case true:{
                switch(*last){
                    case INPUT_STATE_RELEASED:
                    case INPUT_STATE_JUST_RELEASED:{
                        *next = INPUT_STATE_JUST_PRESSED;
                        // should print on just pressed.
                        if(input_latest_active_key == (Key)i){
                            input_latest_active_key_print_count = 1;
                            input_latest_active_key_print_elapsed_time = 0;
                        }
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
    for(size_t i = 0; i < (size_t)MouseButton_EnumSize; i++){
        InputState* last = &input_curr_mouse_button_state[i];
        InputState* next = &input_prev_mouse_button_state[i];
        f32* pressed_time = &input_key_elapsed_pressed_time[i];
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
                        *pressed_time += delta_time;
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
                        *pressed_time = 0;
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

    { // handle_latest_active_key();
        
        
        if(input_latest_active_key != Key_None){
        
            if(input_latest_active_key_print_elapsed_time > 0){
                input_latest_active_key_print_count = 0; 
            }
            if(input_latest_active_key_print_elapsed_time >= input_latest_active_key_print_delay){            
                f32 theshold = input_latest_active_key_print_delay + input_latest_active_key_print_rate;
                while((input_latest_active_key_print_elapsed_time - theshold) >= 0){
                    input_latest_active_key_print_elapsed_time -= input_latest_active_key_print_rate;
                    input_latest_active_key_print_count += 1;
                }        
            }
            
            input_latest_active_key_print_elapsed_time += delta_time;
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

void input_init(MemoryArena* arena, f32 latest_active_key_print_delay, f32 latest_active_key_print_rate){
    ASSERT(!input_is_init, "attempted to init an already init input system.");
    size_t temp;
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_key_down_state, &temp, (size_t)Key_EnumSize);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_key_state, &temp, (size_t)Key_EnumSize);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_key_state, &temp, (size_t)Key_EnumSize);

    MEMORY_ARENA_ALLOC_ARRAY(arena, input_mouse_button_down_state, &temp, (size_t)Key_EnumSize);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_curr_mouse_button_state, &temp, (size_t)Key_EnumSize);
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_prev_mouse_button_state, &temp, (size_t)Key_EnumSize);
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, input_key_elapsed_pressed_time, &temp, (size_t)Key_EnumSize);

    input_latest_active_key_print_rate = latest_active_key_print_rate;
    input_latest_active_key_print_delay = latest_active_key_print_delay;

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

f32 input_key_get_elapsed_pressed_time(Key key){
    return input_key_elapsed_pressed_time[(size_t)key];
}




///
/// key to character handling.
///




char input_get_latest_active_key_character(){
    bool shift = input_key_down_state[(size_t)Key_LeftShift] || input_key_down_state[(size_t)Key_RightShift];
    if(input_key_down_state[(size_t)Key_LeftShift]){
        i32 x = 0;
    }
    bool capslock = input_key_down_state[(size_t)Key_Capslock];
    switch(input_latest_active_key){
        default:
        case Key_None:   return '\0';
        case Key_1:      return shift ? '!' : '1';
        case Key_2:      return shift ? '@' : '2';
        case Key_3:      return shift ? '#' : '3';
        case Key_4:      return shift ? '$' : '4';
        case Key_5:      return shift ? '%' : '5';
        case Key_6:      return shift ? '^' : '6';
        case Key_7:      return shift ? '&' : '7';
        case Key_8:      return shift ? '*' : '8';
        case Key_9:      return shift ? '(' : '9';
        case Key_0:      return shift ? ')' : '0';
        case Key_Period: return shift ? '>' : '.';
    }
}

Key input_get_latest_active_key(){
    return input_latest_active_key;
}

i32 input_get_latest_active_key_print_count(){
    return input_latest_active_key_print_count;
}