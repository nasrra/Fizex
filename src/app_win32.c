#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include "app.h"
#include "stdint.h"
#include "platform.h"
#include "base_layer/base.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendors/stb/stb_image.h"

/**

STB_IMAGE NOTE:

It is  important to note the differences between Windows and Linux.

STB Image on linux and macos work fine with utf8 file paths as the fopen() function
was updated at the kernel level to handle ut8's.

However; because microsoft is sooo awesome; you have to use wfopen() to open a file with a UTF8 path.
So make sure when building the windows DLL to do:

Define this in the header file:

    #define STBI_WINDOWS_UTF8  // <-- CRITICAL FOR WINDOWS UTF-8 SUPPORT
    #define STB_IMAGE_IMPLEMENTATION

    OR:

        use the build command flag: /DSTBI_WINDOWS_UTF8=1
**/


/*
    TODO(nich s)

    - multi-threading.
    - fullscreen.
    - ClipCursor support (for multi monitor configs).
    - file i/o.
    - GetKeyboardLayout (for internatioal keyboard support).
    - WM_SET_CURSOR (for cursor visibility).
    - Blit speed improvements (BltBit)
    - swap out of WinMain to have consistent update loop timing.
*/

/*====================
    globals.
====================*//**/

MemoryArena persistent_memory;
MemoryArena transient_memory;
u128 win32_global_process_start_time;
i32 mouse_x;
i32 mouse_y;


/*====================
    private functions.
====================*//**/

LRESULT main_window_callback(HWND window, UINT message, WPARAM  w_param, LPARAM l_param){

    WindowContext* ctx = (WindowContext*)GetWindowLongPtr(window, GWLP_USERDATA);

    LRESULT result = 0;
    switch(message){

        case WM_CREATE:{
            // extract the 'ctx' pointer passed during CreateWindow()
            CREATESTRUCT* create_struct = (CREATESTRUCT*)l_param;
            ctx = (WindowContext*)create_struct->lpCreateParams;
            // store it in the window's user data for future messages.
            SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)ctx);
        }break;

        case WM_SIZE:{
            OutputDebugStringA("WM_SIZE\n");
        }break;

        case WM_DESTROY:{
            OutputDebugStringA("WM_DESTROY\n");
        }break;

        case WM_CLOSE:{
            OutputDebugStringA("WM_CLOSE\n");
            platform_window_destroy(ctx);
        }break;

        case WM_ACTIVATEAPP:{
            OutputDebugStringA("WM_ACTIVATEAPP\n");
            /*
                TODO(nich s)
                enter "sleep" state when no longer active.
            */
        }break;
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:{
            UINT32 vk_code = (UINT32)w_param;
            switch(vk_code){
                case 'A':           {input_set_key_down(KEY_A);}break;
                case 'D':           {input_set_key_down(KEY_D);}break;
                case 'E':           {input_set_key_down(KEY_E);}break;
                case 'F':           {input_set_key_down(KEY_F);}break;
                case 'Q':           {input_set_key_down(KEY_Q);}break;
                case 'S':           {input_set_key_down(KEY_S);}break;
                case 'W':           {input_set_key_down(KEY_W);}break;
                case VK_RIGHT:      {input_set_key_down(KEY_RIGHT);}break;
                case VK_LEFT:       {input_set_key_down(KEY_LEFT);}break;
                case VK_UP:         {input_set_key_down(KEY_UP);}break;
                case VK_DOWN:       {input_set_key_down(KEY_DOWN);}break;
                case VK_SPACE:      {input_set_key_down(KEY_SPACE);}break;
            }                    
        }break;
        case WM_SYSKEYUP:
        case WM_KEYUP:{
            UINT32 vk_code = (UINT32)w_param;
            // int was_down = ((l_param & (1 << 30)) != 0)? 1 : 0;
            // int is_down = ((w_param & (1 << 15)) == 0)? 1 : 0;
            switch(vk_code){
                case 'A':           {input_set_key_up(KEY_A);}break;
                case 'D':           {input_set_key_up(KEY_D);}break;
                case 'E':           {input_set_key_up(KEY_E);}break;
                case 'F':           {input_set_key_up(KEY_F);}break;
                case 'Q':           {input_set_key_up(KEY_Q);}break;
                case 'S':           {input_set_key_up(KEY_S);}break;
                case 'W':           {input_set_key_up(KEY_W);}break;
                case VK_RIGHT:      {input_set_key_up(KEY_RIGHT);}break;
                case VK_LEFT:       {input_set_key_up(KEY_LEFT);}break;
                case VK_UP:         {input_set_key_up(KEY_UP);}break;
                case VK_DOWN:       {input_set_key_up(KEY_DOWN);}break;
                case VK_SPACE:      {input_set_key_up(KEY_SPACE);}break;
            }
        }break;
        case WM_LBUTTONDOWN:{input_set_mouse_button_down(MOUSE_BUTTON_LEFT);}break;
        case WM_LBUTTONUP:{input_set_mouse_button_up(MOUSE_BUTTON_LEFT);}break;
        case WM_RBUTTONDOWN:{input_set_mouse_button_down(MOUSE_BUTTON_RIGHT);}break;
        case WM_RBUTTONUP:{input_set_mouse_button_up(MOUSE_BUTTON_RIGHT);}break;
        case WM_MOUSEMOVE:{
            mouse_x = GET_X_LPARAM(l_param);
            mouse_y = GET_Y_LPARAM(l_param);
        };break;

        // case WM_PAINT:{
        //     PAINTSTRUCT paint;
        //     HDC device_ctx = BeginPaint(window, &paint);

        //     /*
        //         all painting occurs here...
        //     */
        //     LONG x = paint.rcPaint.left;
        //     LONG y = paint.rcPaint.top;
        //     LONG width = paint.rcPaint.right - paint.rcPaint.left;
        //     LONG height = paint.rcPaint.bottom - paint.rcPaint.top;
        //     PatBlt(device_ctx, x, y, width, height, BLACKNESS);
        //     EndPaint(window, &paint);
        // }break;

        default:{
            result = DefWindowProc(window, message, w_param, l_param);
        }break;
    }

    return result;
}

u128 platform_get_system_tick(){
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (u128)time.QuadPart;
}

u128 platform_get_proccess_tick(){
    return platform_get_system_tick() - win32_global_process_start_time;
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_code){
    win32_global_process_start_time = platform_get_system_tick();
    app_main();
    return(0);
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

void platform_error_message_box(char* msg){
    MessageBox(0, msg, "win32app", MB_OK | MB_ICONERROR);
}

MemoryArena* platform_get_persistent_memory(){
    return &persistent_memory;
}

MemoryArena* platform_get_transient_memory(){
    return &transient_memory;
}

WindowContext* platform_window_create(i32 width, i32 height){
    WindowContext* ctx = (WindowContext*)platform_alloc_memory(sizeof(WindowContext));

    HWND window_handle = NULL;
    HINSTANCE instance = GetModuleHandle(0);

    char* class_name = "WindowClass";

    WNDCLASSA window_class = {
        /*
            CS_OWNDC: have an individual device context for every window in the window_class.
            CS_HREDRAW: redraw the window on horizontal mutations (movement, resize, etc).
            CS_VREDRAW: redraw the window on vertical mutations (movement, resize, etc).
        */
        .style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW,
        .lpfnWndProc = main_window_callback,
        .hInstance = instance,
        .lpszClassName = class_name
    };

    if(RegisterClass(&window_class)){
        RECT window_rect = {0, 0, width, height};
        DWORD window_style = WS_OVERLAPPEDWINDOW;
        /**
            expands window_rect to the size needed so the *client area* ends up being `width` x `height`.
            this is done because the specified resolution used for creating a window includes the toolbar and borders
            to be within the requested size. 
        **/
        AdjustWindowRectEx(&window_rect, window_style, FALSE, 0);
        i32 adjusted_width = window_rect.right - window_rect.left;
        i32 adjusted_height = window_rect.bottom - window_rect.top;
    
        window_handle = CreateWindowEx(
            0,
            class_name,
            "win32 app",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            adjusted_width,
            adjusted_height,
            NULL,
            0,
            instance,
            ctx
        );
        if(!window_handle){
            PANIC(false, "failed to create win32 window.");
        }
    }
    else{
        PANIC(false, "failed to register win32 window class.");
    }

    ctx->win32_hinstance = instance;
    ctx->win32_hwnd = window_handle;
    ctx->win32_hdc = GetDC(window_handle);
    ctx->width = width;
    ctx->height = height;
    return ctx;
}

void platform_window_update(WindowContext* ctx){
    ASSERT(ctx->win32_hwnd != (HWND){0}, "window context doesnt have an init win32 window handle.");
    ASSERT(ctx->win32_hdc != (HDC){0}, "window context doesnt have an init win32 device context handle.");
    ASSERT(ctx->win32_hinstance != (HINSTANCE){0}, "window context doesnt have an init win32 instance handle");
    MSG message;
    if(PeekMessageA(&message, NULL, 0, 0, PM_NOREMOVE)){
        while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

void platform_window_destroy(WindowContext* ctx){
    ASSERT(ctx->win32_hwnd != (HWND){0}, "window context doesnt have an init win32 window handle.");
    ASSERT(ctx->win32_hdc != (HDC){0}, "window context doesnt have an init win32 device context handle.");
    ASSERT(ctx->win32_hinstance != (HINSTANCE){0}, "window context doesnt have an init win32 instance handle");
    ReleaseDC(ctx->win32_hwnd, ctx->win32_hdc);
    DestroyWindow(ctx->win32_hwnd);
    ctx->is_destroyed = true;
}

void platform_window_context_free(WindowContext* ctx){
    platform_free_memory(ctx);
}

bool platform_load_file_into_memory_arena(String file_path, MemoryArena* arena){

    /**
        convert to null terminated string.
    **/
    char* null_terminated_file_path;
    i32 null_terminated_file_path_length;
    MEMORY_ARENA_ALLOC_ARRAY(&transient_memory, null_terminated_file_path, &null_terminated_file_path_length, file_path.length+1);
    COPY_MEMORY(null_terminated_file_path, file_path.chars, file_path.length);
    null_terminated_file_path[null_terminated_file_path_length] = '\0';

    /**
        open file.
    **/
    HANDLE file = CreateFileA(null_terminated_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file == INVALID_HANDLE_VALUE){
        ASSERT(false, "failed to open file.");
        return false;
    }

    /**
        Get file size.
    **/
    LARGE_INTEGER file_size;
    if(!GetFileSizeEx(file, &file_size)){
        ASSERT(false, "failed to retrieve file size.");
        return false;
    }
    size_t bytes_to_read = (size_t)file_size.QuadPart;
    if(bytes_to_read > (arena->size - arena->stride)){
        ASSERT(false, "buffer is too small to store file contents.");
        return false;
    }

    /**
        read file into buffer.
    **/
    DWORD bytes_read = 0;
    while(bytes_to_read > 0){
        BOOL read_success = ReadFile(file, ((u8*)arena->ptr)+arena->stride, (DWORD)bytes_to_read, &bytes_read, NULL);
        if(!read_success){
            ASSERT(false, "failed to read file contents.");
            return false;
        }
        bytes_to_read -= (size_t)bytes_read;
    }
    arena->stride += (size_t)bytes_read;

    /**
        complete.
    **/
    CloseHandle(file);
    return true;
}

void* platform_load_file(String file_path, size_t* out_buffer_size){

    /**
        convert to null terminated string.
    **/
    char* null_terminated_file_path;
    i32 null_terminated_file_path_length;
    MEMORY_ARENA_ALLOC_ARRAY(&transient_memory, null_terminated_file_path, &null_terminated_file_path_length, file_path.length+1);
    COPY_MEMORY(null_terminated_file_path, file_path.chars, file_path.length);
    null_terminated_file_path[null_terminated_file_path_length] = '\0';

    /**
        open file.
    **/
    HANDLE file = CreateFileA(null_terminated_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(file == INVALID_HANDLE_VALUE){
        ASSERT(false, "failed to open file.");
        return NULL;
    }

    /**
        Get file size.
    **/
    LARGE_INTEGER file_size;
    if(!GetFileSizeEx(file, &file_size)){
        ASSERT(false, "failed to retrieve file size.");
        return NULL;
    }
    size_t bytes_to_read = (size_t)file_size.QuadPart;
    void* buffer = platform_alloc_memory(bytes_to_read+1); // add +1 for the NULL terminator.

    /**
        read file into buffer.
    **/
    DWORD bytes_read = 0;
    size_t total_bytes_read = 0;
    while(bytes_to_read > 0){
        BOOL read_success = ReadFile(file, (u8*)buffer + total_bytes_read, (DWORD)bytes_to_read, &bytes_read, NULL);
        if(!read_success){
            ASSERT(false, "failed to read file contents.");
            platform_free_memory(buffer);
            return NULL;
        }
        total_bytes_read += (size_t)bytes_read;
        bytes_to_read -= (size_t)bytes_read;
    }

    *out_buffer_size = total_bytes_read;
    u8* u8_buffer = (u8*)buffer;
    u8_buffer[total_bytes_read] = '\0';

    /**
        complete.
    **/
    CloseHandle(file);
    return buffer;
}

f32 platform_window_calc_aspect_ratio(WindowContext ctx){
    return (f32)ctx.width / (f32)ctx.height;
}

void platform_print_msg(char* msg){
    OutputDebugStringA(msg);
}

bool platform_load_image(Image* out_image, String file_path){
    ASSERT(out_image->pixel == NULL, "image already init.");
    i32 comp;
    // 4 channels for RGBA always being output.
    i32 desired_channels = 4;
    size_t buffer_size = (file_path.length + 1) * sizeof(char);
    void* buffer = platform_alloc_memory(buffer_size);
    ZERO_MEMORY(buffer, buffer_size);
    COPY_MEMORY(buffer, file_path.chars, file_path.length);
    u8* ptr = stbi_load((char*)buffer, &out_image->width, &out_image->height, &comp, desired_channels);
    platform_free_memory(buffer);
    if(ptr == NULL){
        ASSERT(false, "failed to load image.");
        return false;
    }
    out_image->pixel = ptr;
    out_image->pixel_length = desired_channels * out_image->width * out_image->height * sizeof(u8);
    return true;
}

bool platform_free_image(Image* image){
    if(image->pixel == NULL){
        return false;
    }
    
    stbi_image_free(image->pixel);
    // zero out image once free;
    *image = (Image){0};
    return true; 
}

void platform_get_mouse_position(int* out_x, int* out_y){
    *out_x = mouse_x;
    *out_y = mouse_y;
}
