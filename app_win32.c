#include <assert.h>
#include <windows.h>
#include "app.h"
#include "stdint.h"
#include "base_layer/base.h"

/*
    TODO(nich s)

    - multi-threading.
    - fullscreen.
    - ClipCursor support (for multi monitor configs).
    - file i/o.
    - GetKeyboardLayout (for internatioal keyboard support).
    - WM_SET_CURSOR (for cursor visibility).
    - Blit speed improvements (BltBit)
*/

/*====================
    globals.
====================*//**/

MemoryArena persistent_memory;
MemoryArena transient_memory;

/*====================
    private functions.
====================*//**/

LRESULT main_window_callback(HWND window, UINT message, WPARAM  w_param, LPARAM l_param){
    LRESULT result = 0;
    switch(message){
        
        case WM_SIZE:{
            OutputDebugStringA("WM_SIZE\n");
        }break;
        
        case WM_DESTROY:{
            OutputDebugStringA("WM_DESTROY\n");
        }break;
        
        case WM_CLOSE:{
            OutputDebugStringA("WM_CLOSE\n");
        }break;
        
        case WM_ACTIVATEAPP:{
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            /*
                TODO(nich s)
                enter "sleep" state when no longer active.  
            */
        }break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:{

            UINT32 vk_code = (UINT32)w_param;
            int was_down = ((l_param & (1 << 30)) != 0)? 1 : 0;
            int is_down = ((w_param & (1 << 15)) == 0)? 1 : 0;

            if(vk_code == 'W'){
                OutputDebugStringA("W");
            }
            else if(vk_code == 'A'){
                OutputDebugStringA("A");                
            }
            OutputDebugStringA("\n");

        }break;
        
        case WM_PAINT:{
            PAINTSTRUCT paint;
            HDC device_ctx = BeginPaint(window, &paint);

            /*
                all painting occurs here...
            */

            LONG x = paint.rcPaint.left;
            LONG y = paint.rcPaint.top;
            LONG width = paint.rcPaint.right - paint.rcPaint.left;
            LONG height = paint.rcPaint.bottom - paint.rcPaint.top;
            PatBlt(device_ctx, x, y, width, height, BLACKNESS);
            EndPaint(window, &paint);
        }break;
        
        default:{
            result = DefWindowProc(window, message, w_param, l_param);
        }break;
    }

    return result;
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_code){
    app_main();
    WNDCLASSA window_class = {0};
    /*
        CS_OWNDC: have an individual device context for every window in the window_class.
        CS_HREDRAW: redraw the window on horizontal mutations (movement, resize, etc).
        CS_VREDRAW: redraw the window on vertical mutations (movement, resize, etc).
    */
    window_class.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;

    window_class.lpfnWndProc = main_window_callback;
    window_class.hInstance = instance;
    // window_class.hIcon;
    // window_class.hCursor;
    // window_class.hbrBackground;
    window_class.lpszClassName = "WindowClass";

    if(RegisterClass(&window_class)){
        HWND window_handle = CreateWindowExA(
            0,
            "WindowClass",
            "win32 app",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            0,
            instance,
            0
        );
        if(window_handle){
            MSG message;
            for(;;){
                /*
                    window handle is 0 here as we want to get messages from all active windows.
                */
                BOOL msg_result = GetMessage(&message, 0, 0, 0); 
                if(msg_result > 0){
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                    app_update_and_render(&persistent_memory, &transient_memory);
                    transient_memory.stride = 0;
                }
                else{
                    break;
                }
            }
        }
        else{
            /*
                TODO(nich s)
                log this error.  
            */
        }
    }
    else{
        /*
            TODO(nich s)
            log this error.    
        */
    }
}

/*====================
    public functions.
====================*//**/

void platform_output_message(char* msg){
    OutputDebugStringA(msg);
}

void* platform_alloc_memory(size_t size){
    /*
        `MEM_COMMIT` tells windows to begin actively tracking the page and virtual address space that has been allocated.
        an alternative is `MEM_RESERVE` if a page should be allocated but not actively tracked yet.
    */
    return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

void platform_free_memory(void* memory){
    /*
        `SIZE_T dwSize` is passed as 0 as magically windows remembers the size of the allocation for the pointer.
    */
    VirtualFree(memory, 0, MEM_RELEASE);
}

void platform_init_persistent_memory(size_t size){
    if(persistent_memory.ptr!=NULL){
        assert("platform persistent memory already init.");
    }
    persistent_memory.ptr = platform_alloc_memory(size);
    persistent_memory.size = size;
    persistent_memory.stride = 0;
}

void platform_init_transient_memory(size_t size){
    if(transient_memory.ptr!=NULL){
        assert("transient persistent memory already init.");
    }
    transient_memory.ptr = platform_alloc_memory(size);
    transient_memory.size = size;
    transient_memory.stride = 0;
}