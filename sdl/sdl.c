// #include <SDL3/SDL.h>

// /*========================================
//     types.
// ========================================*//**/

// typedef struct{
//     void* display;
//     void* window;
//     bool is_init;
// } X11Info;

// typedef struct{
//     void* display;
//     void* surface;
//     bool is_init;
// } WaylandInfo;

// typedef struct{
//     void* window;
//     bool is_init;
// } CocoaInfo;

// typedef struct{
//     void* hwnd;
//     void* hdc;
//     void* hinstance;
//     bool is_init;
// } Win32Info;

// typedef struct{
//     Win32Info win32_info;
//     CocoaInfo cocoa_info;
//     WaylandInfo wayland_info;
//     X11Info x11_info;
// } WindowManagerInfo;

// /*========================================
//     globals
// ========================================*//**/

// SDL_Window* sdl_window;
// bool sdl_should_close = false;
// WindowManagerInfo sdl_wminfo;

// /*========================================
//     X
// ========================================*//**/

// void sdl_init(const char* title, i32 width, i32 height){
//     DEBUG_ASSERT(SDL_Init(SDL_INIT_VIDEO), "failed to init sdl");
//     sdl_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_FULLSCREEN);
//     DEBUG_ASSERT(sdl_window!=NULL, "failed to create sdl window.");

//     SDL_PropertiesID props = SDL_GetWindowProperties(sdl_window);

//     { // win32.
//         sdl_wminfo.win32_info.hdc = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HDC_POINTER, 0);
//         sdl_wminfo.win32_info.hwnd = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, 0);
//         sdl_wminfo.win32_info.hinstance = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, 0);
//         sdl_wminfo.win32_info.is_init = sdl_wminfo.win32_info.hdc && sdl_wminfo.win32_info.hwnd && sdl_wminfo.win32_info.hinstance;
//     }

//     { // wayland.
//         sdl_wminfo.wayland_info.display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, 0);
//         sdl_wminfo.wayland_info.surface = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, 0);
//         sdl_wminfo.wayland_info.is_init = sdl_wminfo.wayland_info.display && sdl_wminfo.wayland_info.surface;
//     }

//     { // x11.
//         sdl_wminfo.x11_info.display = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, 0);
//         sdl_wminfo.x11_info.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
//         sdl_wminfo.x11_info.is_init = sdl_wminfo.x11_info.display && sdl_wminfo.x11_info.window; 
//     }

//     { // cocoa.
//         sdl_wminfo.cocoa_info.window = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, 0);
//         sdl_wminfo.cocoa_info.is_init = sdl_wminfo.cocoa_info.window && 1; 
//     }    
// }

// void sdl_update(){
//     SDL_Event event;
//     while(SDL_PollEvent(&event)){

//         if(event.type == SDL_EVENT_QUIT){
//             sdl_should_close = true;
//         }
//         /*
//             note:
//             event.key.key is the physical input of the device ('A' will always be where 'A' is).
//             event.key.scancode is the logical input of the device ('A' can change depending upon OS keyboard layout).
//         */
//         if(event.type == SDL_EVENT_KEY_DOWN){
//             switch(event.key.scancode){
//                 case SDL_SCANCODE_A: input_set_key_down(KEY_A); break;
//                 case SDL_SCANCODE_B: input_set_key_down(KEY_B); break;
//                 case SDL_SCANCODE_C: input_set_key_down(KEY_C); break;
//                 case SDL_SCANCODE_D: input_set_key_down(KEY_D); break;
//                 case SDL_SCANCODE_E: input_set_key_down(KEY_E); break;
//                 case SDL_SCANCODE_F: input_set_key_down(KEY_F); break;
//                 case SDL_SCANCODE_G: input_set_key_down(KEY_G); break;
//                 case SDL_SCANCODE_H: input_set_key_down(KEY_H); break;
//                 case SDL_SCANCODE_I: input_set_key_down(KEY_I); break;
//                 case SDL_SCANCODE_J: input_set_key_down(KEY_J); break;
//                 case SDL_SCANCODE_K: input_set_key_down(KEY_K); break;
//                 case SDL_SCANCODE_L: input_set_key_down(KEY_L); break;
//                 case SDL_SCANCODE_M: input_set_key_down(KEY_M); break;
//                 case SDL_SCANCODE_N: input_set_key_down(KEY_N); break;
//                 case SDL_SCANCODE_O: input_set_key_down(KEY_O); break;
//                 case SDL_SCANCODE_P: input_set_key_down(KEY_P); break;
//                 case SDL_SCANCODE_Q: input_set_key_down(KEY_Q); break;
//                 case SDL_SCANCODE_R: input_set_key_down(KEY_R); break;
//                 case SDL_SCANCODE_S: input_set_key_down(KEY_S); break;
//                 case SDL_SCANCODE_T: input_set_key_down(KEY_T); break;
//                 case SDL_SCANCODE_U: input_set_key_down(KEY_U); break;
//                 case SDL_SCANCODE_V: input_set_key_down(KEY_V); break;
//                 case SDL_SCANCODE_W: input_set_key_down(KEY_W); break;
//                 case SDL_SCANCODE_X: input_set_key_down(KEY_X); break;
//                 case SDL_SCANCODE_Y: input_set_key_down(KEY_Y); break;
//                 case SDL_SCANCODE_Z: input_set_key_down(KEY_Z); break;
//                 case SDL_SCANCODE_0: input_set_key_down(KEY_0); break;
//                 case SDL_SCANCODE_1: input_set_key_down(KEY_1); break;
//                 case SDL_SCANCODE_2: input_set_key_down(KEY_2); break;
//                 case SDL_SCANCODE_3: input_set_key_down(KEY_3); break;
//                 case SDL_SCANCODE_4: input_set_key_down(KEY_4); break;
//                 case SDL_SCANCODE_5: input_set_key_down(KEY_5); break;
//                 case SDL_SCANCODE_6: input_set_key_down(KEY_6); break;
//                 case SDL_SCANCODE_7: input_set_key_down(KEY_7); break;
//                 case SDL_SCANCODE_8: input_set_key_down(KEY_8); break;
//                 case SDL_SCANCODE_9: input_set_key_down(KEY_9); break;
//                 case SDL_SCANCODE_ESCAPE: input_set_key_down(KEY_ESCAPE); break;
//                 case SDL_SCANCODE_BACKSPACE: input_set_key_down(KEY_BACKSPACE); break;
//                 case SDL_SCANCODE_TAB: input_set_key_down(KEY_TAB); break;
//                 case SDL_SCANCODE_SPACE: input_set_key_down(KEY_SPACE); break;
//                 case SDL_SCANCODE_MINUS: input_set_key_down(KEY_MINUS); break;
//                 case SDL_SCANCODE_EQUALS: input_set_key_down(KEY_EQUALS); break;
//                 case SDL_SCANCODE_LEFTBRACKET: input_set_key_down(KEY_L_BRACKET); break;
//                 case SDL_SCANCODE_RIGHTBRACKET: input_set_key_down(KEY_R_BRACKET); break;
//                 case SDL_SCANCODE_BACKSLASH: input_set_key_down(KEY_BACKSLASH); break;
//                 case SDL_SCANCODE_SEMICOLON: input_set_key_down(KEY_SEMICOLON); break;
//                 case SDL_SCANCODE_APOSTROPHE: input_set_key_down(KEY_APOSTROPHE); break;
//                 case SDL_SCANCODE_GRAVE: input_set_key_down(KEY_BACK_TICK); break;
//                 case SDL_SCANCODE_COMMA: input_set_key_down(KEY_COMMA); break;
//                 case SDL_SCANCODE_PERIOD: input_set_key_down(KEY_PERIOD); break;
//                 case SDL_SCANCODE_SLASH: input_set_key_down(KEY_SLASH); break;
//                 case SDL_SCANCODE_CAPSLOCK: input_set_key_down(KEY_CAPSLOCK); break;                
//                 case SDL_SCANCODE_F1: input_set_key_down(KEY_F1); break;
//                 case SDL_SCANCODE_F2: input_set_key_down(KEY_F2); break;
//                 case SDL_SCANCODE_F3: input_set_key_down(KEY_F3); break;
//                 case SDL_SCANCODE_F4: input_set_key_down(KEY_F4); break;
//                 case SDL_SCANCODE_F5: input_set_key_down(KEY_F5); break;
//                 case SDL_SCANCODE_F6: input_set_key_down(KEY_F6); break;
//                 case SDL_SCANCODE_F7: input_set_key_down(KEY_F7); break;
//                 case SDL_SCANCODE_F8: input_set_key_down(KEY_F8); break;
//                 case SDL_SCANCODE_F9: input_set_key_down(KEY_F9); break;
//                 case SDL_SCANCODE_F10: input_set_key_down(KEY_F10); break;
//                 case SDL_SCANCODE_F11: input_set_key_down(KEY_F11); break;
//                 case SDL_SCANCODE_F12: input_set_key_down(KEY_F12); break;
//                 case SDL_SCANCODE_PRINTSCREEN: input_set_key_down(KEY_PRINT_SCREEN); break;
//                 case SDL_SCANCODE_INSERT: input_set_key_down(KEY_INSERT); break;
//                 case SDL_SCANCODE_HOME: input_set_key_down(KEY_HOME); break;
//                 case SDL_SCANCODE_PAGEUP: input_set_key_down(KEY_PAGE_UP); break;
//                 case SDL_SCANCODE_PAGEDOWN: input_set_key_down(KEY_PAGE_DOWN); break;
//                 case SDL_SCANCODE_DELETE: input_set_key_down(KEY_DELETE); break;
//                 case SDL_SCANCODE_END: input_set_key_down(KEY_END); break;
//                 case SDL_SCANCODE_RIGHT: input_set_key_down(KEY_RIGHT); break;
//                 case SDL_SCANCODE_LEFT: input_set_key_down(KEY_LEFT); break;
//                 case SDL_SCANCODE_DOWN: input_set_key_down(KEY_DOWN); break;
//                 case SDL_SCANCODE_UP: input_set_key_down(KEY_UP); break;
//                 case SDL_SCANCODE_LCTRL: input_set_key_down(KEY_L_CONTROL); break;
//                 case SDL_SCANCODE_LSHIFT: input_set_key_down(KEY_L_SHIFT); break;
//                 case SDL_SCANCODE_LALT: input_set_key_down(KEY_L_ALT); break;                
//                 case SDL_SCANCODE_RCTRL: input_set_key_down(KEY_R_CONTROL); break;
//                 case SDL_SCANCODE_RSHIFT: input_set_key_down(KEY_R_SHIFT); break;
//                 case SDL_SCANCODE_RALT: input_set_key_down(KEY_R_ALT); break;
//                 case SDL_SCANCODE_RETURN: input_set_key_down(KEY_ENTER); break;
//                 default: break;
//             }
//         }
//         else if(event.type == SDL_EVENT_KEY_UP){
//             switch (event.key.scancode)
//             {
//                 case SDL_SCANCODE_A: input_set_key_up(KEY_A); break;
//                 case SDL_SCANCODE_B: input_set_key_up(KEY_B); break;
//                 case SDL_SCANCODE_C: input_set_key_up(KEY_C); break;
//                 case SDL_SCANCODE_D: input_set_key_up(KEY_D); break;
//                 case SDL_SCANCODE_E: input_set_key_up(KEY_E); break;
//                 case SDL_SCANCODE_F: input_set_key_up(KEY_F); break;
//                 case SDL_SCANCODE_G: input_set_key_up(KEY_G); break;
//                 case SDL_SCANCODE_H: input_set_key_up(KEY_H); break;
//                 case SDL_SCANCODE_I: input_set_key_up(KEY_I); break;
//                 case SDL_SCANCODE_J: input_set_key_up(KEY_J); break;
//                 case SDL_SCANCODE_K: input_set_key_up(KEY_K); break;
//                 case SDL_SCANCODE_L: input_set_key_up(KEY_L); break;
//                 case SDL_SCANCODE_M: input_set_key_up(KEY_M); break;
//                 case SDL_SCANCODE_N: input_set_key_up(KEY_N); break;
//                 case SDL_SCANCODE_O: input_set_key_up(KEY_O); break;
//                 case SDL_SCANCODE_P: input_set_key_up(KEY_P); break;
//                 case SDL_SCANCODE_Q: input_set_key_up(KEY_Q); break;
//                 case SDL_SCANCODE_R: input_set_key_up(KEY_R); break;
//                 case SDL_SCANCODE_S: input_set_key_up(KEY_S); break;
//                 case SDL_SCANCODE_T: input_set_key_up(KEY_T); break;
//                 case SDL_SCANCODE_U: input_set_key_up(KEY_U); break;
//                 case SDL_SCANCODE_V: input_set_key_up(KEY_V); break;
//                 case SDL_SCANCODE_W: input_set_key_up(KEY_W); break;
//                 case SDL_SCANCODE_X: input_set_key_up(KEY_X); break;
//                 case SDL_SCANCODE_Y: input_set_key_up(KEY_Y); break;
//                 case SDL_SCANCODE_Z: input_set_key_up(KEY_Z); break;
//                 case SDL_SCANCODE_0: input_set_key_up(KEY_0); break;
//                 case SDL_SCANCODE_1: input_set_key_up(KEY_1); break;
//                 case SDL_SCANCODE_2: input_set_key_up(KEY_2); break;
//                 case SDL_SCANCODE_3: input_set_key_up(KEY_3); break;
//                 case SDL_SCANCODE_4: input_set_key_up(KEY_4); break;
//                 case SDL_SCANCODE_5: input_set_key_up(KEY_5); break;
//                 case SDL_SCANCODE_6: input_set_key_up(KEY_6); break;
//                 case SDL_SCANCODE_7: input_set_key_up(KEY_7); break;
//                 case SDL_SCANCODE_8: input_set_key_up(KEY_8); break;
//                 case SDL_SCANCODE_9: input_set_key_up(KEY_9); break;
//                 case SDL_SCANCODE_ESCAPE: input_set_key_up(KEY_ESCAPE); break;
//                 case SDL_SCANCODE_BACKSPACE: input_set_key_up(KEY_BACKSPACE); break;
//                 case SDL_SCANCODE_TAB: input_set_key_up(KEY_TAB); break;
//                 case SDL_SCANCODE_SPACE: input_set_key_up(KEY_SPACE); break;
//                 case SDL_SCANCODE_MINUS: input_set_key_up(KEY_MINUS); break;
//                 case SDL_SCANCODE_EQUALS: input_set_key_up(KEY_EQUALS); break;
//                 case SDL_SCANCODE_LEFTBRACKET: input_set_key_up(KEY_L_BRACKET); break;
//                 case SDL_SCANCODE_RIGHTBRACKET: input_set_key_up(KEY_R_BRACKET); break;
//                 case SDL_SCANCODE_BACKSLASH: input_set_key_up(KEY_BACKSLASH); break;
//                 case SDL_SCANCODE_SEMICOLON: input_set_key_up(KEY_SEMICOLON); break;
//                 case SDL_SCANCODE_APOSTROPHE: input_set_key_up(KEY_APOSTROPHE); break;
//                 case SDL_SCANCODE_GRAVE: input_set_key_up(KEY_BACK_TICK); break;
//                 case SDL_SCANCODE_COMMA: input_set_key_up(KEY_COMMA); break;
//                 case SDL_SCANCODE_PERIOD: input_set_key_up(KEY_PERIOD); break;
//                 case SDL_SCANCODE_SLASH: input_set_key_up(KEY_SLASH); break;
//                 case SDL_SCANCODE_CAPSLOCK: input_set_key_up(KEY_CAPSLOCK); break;                
//                 case SDL_SCANCODE_F1: input_set_key_up(KEY_F1); break;
//                 case SDL_SCANCODE_F2: input_set_key_up(KEY_F2); break;
//                 case SDL_SCANCODE_F3: input_set_key_up(KEY_F3); break;
//                 case SDL_SCANCODE_F4: input_set_key_up(KEY_F4); break;
//                 case SDL_SCANCODE_F5: input_set_key_up(KEY_F5); break;
//                 case SDL_SCANCODE_F6: input_set_key_up(KEY_F6); break;
//                 case SDL_SCANCODE_F7: input_set_key_up(KEY_F7); break;
//                 case SDL_SCANCODE_F8: input_set_key_up(KEY_F8); break;
//                 case SDL_SCANCODE_F9: input_set_key_up(KEY_F9); break;
//                 case SDL_SCANCODE_F10: input_set_key_up(KEY_F10); break;
//                 case SDL_SCANCODE_F11: input_set_key_up(KEY_F11); break;
//                 case SDL_SCANCODE_F12: input_set_key_up(KEY_F12); break;
//                 case SDL_SCANCODE_PRINTSCREEN: input_set_key_up(KEY_PRINT_SCREEN); break;
//                 case SDL_SCANCODE_INSERT: input_set_key_up(KEY_INSERT); break;
//                 case SDL_SCANCODE_HOME: input_set_key_up(KEY_HOME); break;
//                 case SDL_SCANCODE_PAGEUP: input_set_key_up(KEY_PAGE_UP); break;
//                 case SDL_SCANCODE_PAGEDOWN: input_set_key_up(KEY_PAGE_DOWN); break;
//                 case SDL_SCANCODE_DELETE: input_set_key_up(KEY_DELETE); break;
//                 case SDL_SCANCODE_END: input_set_key_up(KEY_END); break;
//                 case SDL_SCANCODE_RIGHT: input_set_key_up(KEY_RIGHT); break;
//                 case SDL_SCANCODE_LEFT: input_set_key_up(KEY_LEFT); break;
//                 case SDL_SCANCODE_DOWN: input_set_key_up(KEY_DOWN); break;
//                 case SDL_SCANCODE_UP: input_set_key_up(KEY_UP); break;
//                 case SDL_SCANCODE_LCTRL: input_set_key_up(KEY_L_CONTROL); break;
//                 case SDL_SCANCODE_LSHIFT: input_set_key_up(KEY_L_SHIFT); break;
//                 case SDL_SCANCODE_LALT: input_set_key_up(KEY_L_ALT); break;                
//                 case SDL_SCANCODE_RCTRL: input_set_key_up(KEY_R_CONTROL); break;
//                 case SDL_SCANCODE_RSHIFT: input_set_key_up(KEY_R_SHIFT); break;
//                 case SDL_SCANCODE_RALT: input_set_key_up(KEY_R_ALT); break;
//                 case SDL_SCANCODE_RETURN: input_set_key_up(KEY_ENTER); break;            
//                 default: break;

//             }
//         }

//     }
// }