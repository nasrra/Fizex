#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include "app.h"
#include "stdint.h"
#include "os.h"

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
long long win32_global_process_start_time;

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
            os_window_destroy(ctx);
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
                case '0':           {input_set_key_down(Key_0);}break;
                case '1':           {input_set_key_down(Key_1);}break;
                case '2':           {input_set_key_down(Key_2);}break;
                case '3':           {input_set_key_down(Key_3);}break;
                case '4':           {input_set_key_down(Key_4);}break;
                case '5':           {input_set_key_down(Key_5);}break;
                case '6':           {input_set_key_down(Key_6);}break;
                case '7':           {input_set_key_down(Key_7);}break;
                case '8':           {input_set_key_down(Key_8);}break;
                case '9':           {input_set_key_down(Key_9);}break;
                
                case 'A':           {input_set_key_down(Key_A);}break;
                case 'B':           {input_set_key_down(Key_B);}break;
                case 'C':           {input_set_key_down(Key_C);}break;
                case 'D':           {input_set_key_down(Key_D);}break;
                case 'E':           {input_set_key_down(Key_E);}break;
                case 'F':           {input_set_key_down(Key_F);}break;
                case 'G':           {input_set_key_down(Key_G);}break;
                case 'H':           {input_set_key_down(Key_H);}break;
                case 'I':           {input_set_key_down(Key_I);}break;
                case 'J':           {input_set_key_down(Key_J);}break;
                case 'K':           {input_set_key_down(Key_K);}break;
                case 'L':           {input_set_key_down(Key_L);}break;
                case 'M':           {input_set_key_down(Key_M);}break;
                case 'N':           {input_set_key_down(Key_N);}break;
                case 'O':           {input_set_key_down(Key_O);}break;
                case 'P':           {input_set_key_down(Key_P);}break;
                case 'Q':           {input_set_key_down(Key_Q);}break;
                case 'R':           {input_set_key_down(Key_R);}break;
                case 'S':           {input_set_key_down(Key_S);}break;
                case 'T':           {input_set_key_down(Key_T);}break;
                case 'U':           {input_set_key_down(Key_U);}break;
                case 'V':           {input_set_key_down(Key_V);}break;
                case 'W':           {input_set_key_down(Key_W);}break;
                case 'X':           {input_set_key_down(Key_X);}break;
                case 'Y':           {input_set_key_down(Key_Y);}break;
                case 'Z':           {input_set_key_down(Key_Z);}break;

                case VK_RIGHT:      {input_set_key_down(Key_Right);}break;
                case VK_LEFT:       {input_set_key_down(Key_Left);}break;
                case VK_UP:         {input_set_key_down(Key_Up);}break;
                case VK_DOWN:       {input_set_key_down(Key_Down);}break;
                case VK_SPACE:      {input_set_key_down(Key_Space);}break;

                case VK_F1:         {input_set_key_down(Key_F1);}break;
                case VK_F2:         {input_set_key_down(Key_F2);}break;
                case VK_F3:         {input_set_key_down(Key_F3);}break;
                case VK_F4:         {input_set_key_down(Key_F4);}break;
                case VK_F5:         {input_set_key_down(Key_F5);}break;
                case VK_F6:         {input_set_key_down(Key_F6);}break;
                case VK_F7:         {input_set_key_down(Key_F7);}break;
                case VK_F8:         {input_set_key_down(Key_F8);}break;
                case VK_F9:         {input_set_key_down(Key_F9);}break;
                case VK_F10:        {input_set_key_down(Key_F10);}break;
                case VK_F11:        {input_set_key_down(Key_F11);}break;
                case VK_F12:        {input_set_key_down(Key_F12);}break;

                case VK_OEM_PERIOD: {input_set_key_down(Key_Period);}break;
                case VK_BACK:       {input_set_key_down(Key_Backspace);}break;
                case VK_OEM_MINUS:  {input_set_key_down(Key_Minus);}break;
                case VK_OEM_3:      {input_set_key_down(Key_Tilde);}break;
                case VK_SHIFT:      {input_set_key_down(Key_LeftShift); input_set_key_down(Key_RightShift);}break; // note: VK_LSHIFT and VK_RSHIFT dont work on my keyboard; only VK_SHIFT does.
                case VK_ESCAPE:     {input_set_key_down(Key_Escape);} break;
            }
        }break;
        case WM_SYSKEYUP:
        case WM_KEYUP:{
            UINT32 vk_code = (UINT32)w_param;
            // int was_down = ((l_param & (1 << 30)) != 0)? 1 : 0;
            // int is_down = ((w_param & (1 << 15)) == 0)? 1 : 0;
            switch(vk_code){
                case '0':           {input_set_key_up(Key_0);}break;
                case '1':           {input_set_key_up(Key_1);}break;
                case '2':           {input_set_key_up(Key_2);}break;
                case '3':           {input_set_key_up(Key_3);}break;
                case '4':           {input_set_key_up(Key_4);}break;
                case '5':           {input_set_key_up(Key_5);}break;
                case '6':           {input_set_key_up(Key_6);}break;
                case '7':           {input_set_key_up(Key_7);}break;
                case '8':           {input_set_key_up(Key_8);}break;
                case '9':           {input_set_key_up(Key_9);}break;
                
                case 'A':           {input_set_key_up(Key_A);}break;
                case 'B':           {input_set_key_up(Key_B);}break;
                case 'C':           {input_set_key_up(Key_C);}break;
                case 'D':           {input_set_key_up(Key_D);}break;
                case 'E':           {input_set_key_up(Key_E);}break;
                case 'F':           {input_set_key_up(Key_F);}break;
                case 'G':           {input_set_key_up(Key_G);}break;
                case 'H':           {input_set_key_up(Key_H);}break;
                case 'I':           {input_set_key_up(Key_I);}break;
                case 'J':           {input_set_key_up(Key_J);}break;
                case 'K':           {input_set_key_up(Key_K);}break;
                case 'L':           {input_set_key_up(Key_L);}break;
                case 'M':           {input_set_key_up(Key_M);}break;
                case 'N':           {input_set_key_up(Key_N);}break;
                case 'O':           {input_set_key_up(Key_O);}break;
                case 'P':           {input_set_key_up(Key_P);}break;
                case 'Q':           {input_set_key_up(Key_Q);}break;
                case 'R':           {input_set_key_up(Key_R);}break;
                case 'S':           {input_set_key_up(Key_S);}break;
                case 'T':           {input_set_key_up(Key_T);}break;
                case 'U':           {input_set_key_up(Key_U);}break;
                case 'V':           {input_set_key_up(Key_V);}break;
                case 'W':           {input_set_key_up(Key_W);}break;
                case 'X':           {input_set_key_up(Key_X);}break;
                case 'Y':           {input_set_key_up(Key_Y);}break;
                case 'Z':           {input_set_key_up(Key_Z);}break;                
                
                case VK_RIGHT:      {input_set_key_up(Key_Right);}break;
                case VK_LEFT:       {input_set_key_up(Key_Left);}break;
                case VK_UP:         {input_set_key_up(Key_Up);}break;
                case VK_DOWN:       {input_set_key_up(Key_Down);}break;
                case VK_SPACE:      {input_set_key_up(Key_Space);}break;

                case VK_F1:         {input_set_key_up(Key_F1);}break;
                case VK_F2:         {input_set_key_up(Key_F2);}break;
                case VK_F3:         {input_set_key_up(Key_F3);}break;
                case VK_F4:         {input_set_key_up(Key_F4);}break;
                case VK_F5:         {input_set_key_up(Key_F5);}break;
                case VK_F6:         {input_set_key_up(Key_F6);}break;
                case VK_F7:         {input_set_key_up(Key_F7);}break;
                case VK_F8:         {input_set_key_up(Key_F8);}break;
                case VK_F9:         {input_set_key_up(Key_F9);}break;
                case VK_F10:        {input_set_key_up(Key_F10);}break;
                case VK_F11:        {input_set_key_up(Key_F11);}break;
                case VK_F12:        {input_set_key_up(Key_F12);}break;

                case VK_OEM_PERIOD: {input_set_key_up(Key_Period);}break;
                case VK_BACK:       {input_set_key_up(Key_Backspace);}break;
                case VK_OEM_MINUS:  {input_set_key_up(Key_Minus);}break;
                case VK_OEM_3:      {input_set_key_up(Key_Tilde);}break;
                case VK_ESCAPE:     {input_set_key_up(Key_Escape);} break;
                case VK_SHIFT:      {input_set_key_up(Key_LeftShift); input_set_key_up(Key_RightShift);}break; // note: VK_LSHIFT and VK_RSHIFT dont work on my keyboard; only VK_SHIFT does.
            }
        }break;
        case WM_LBUTTONDOWN:{input_set_mouse_button_down(MouseButton_Left);}break;
        case WM_LBUTTONUP:{input_set_mouse_button_up(MouseButton_Left);}break;
        case WM_RBUTTONDOWN:{input_set_mouse_button_down(MouseButton_Right);}break;
        case WM_RBUTTONUP:{input_set_mouse_button_up(MouseButton_Right);}break;
        case WM_MOUSEMOVE:{
            i32 mouse_x = GET_X_LPARAM(l_param);
            i32 mouse_y = GET_Y_LPARAM(l_param);
            input_set_mouse_position(mouse_x, mouse_y);
        }break;
        case WM_MOUSEWHEEL:{
            input_increment_mouse_scroll_wheel_value((i32)GET_WHEEL_DELTA_WPARAM(w_param));
        }break;
        case WM_MBUTTONDOWN:{
            input_set_mouse_button_down(MouseButton_Middle);
        }break;
        case WM_MBUTTONUP:{
            input_set_mouse_button_up(MouseButton_Middle);
        }break;
        default:{
            result = DefWindowProc(window, message, w_param, l_param);
        }break;
    }

    return result;
}

long long os_get_system_tick(){
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return (long long)time.QuadPart;
}

long long os_get_proccess_tick(){
    return os_get_system_tick() - win32_global_process_start_time;
}

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int show_code){
    win32_global_process_start_time = os_get_system_tick();
    app_main();
    return(0);
}

/*====================
    public functions.
====================*//**/

void os_output_message(char* msg){
    OutputDebugStringA(msg);
}

void* os_alloc_memory(size_t size){
    /*
        `MEM_COMMIT` tells windows to begin actively tracking the page and virtual address space that has been allocated.
        an alternative is `MEM_RESERVE` if a page should be allocated but not actively tracked yet.
    */
    return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

void os_free_memory(void* memory){
    /*
        `SIZE_T dwSize` is passed as 0 as magically windows remembers the size of the allocation for the pointer.
    */
    VirtualFree(memory, 0, MEM_RELEASE);
}

void os_init_persistent_memory(size_t size){
    if(persistent_memory.ptr!=NULL){
        assert("platform persistent memory already init.");
    }
    persistent_memory.ptr = os_alloc_memory(size);
    persistent_memory.size = size;
    persistent_memory.stride = 0;
}

void os_init_transient_memory(size_t size){
    if(transient_memory.ptr!=NULL){
        assert("transient persistent memory already init.");
    }
    transient_memory.ptr = os_alloc_memory(size);
    transient_memory.size = size;
    transient_memory.stride = 0;
}

void os_error_message_box(char* msg){
    MessageBox(0, msg, "win32app", MB_OK | MB_ICONERROR);
}

MemoryArena* os_get_persistent_memory(){
    return &persistent_memory;
}

MemoryArena* os_get_transient_memory(){
    return &transient_memory;
}

WindowContext* os_window_create(i32 width, i32 height){
    WindowContext* ctx = (WindowContext*)os_alloc_memory(sizeof(WindowContext));

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

void os_window_update(WindowContext* ctx){
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

void os_window_destroy(WindowContext* ctx){
    ASSERT(ctx->win32_hwnd != (HWND){0}, "window context doesnt have an init win32 window handle.");
    ASSERT(ctx->win32_hdc != (HDC){0}, "window context doesnt have an init win32 device context handle.");
    ASSERT(ctx->win32_hinstance != (HINSTANCE){0}, "window context doesnt have an init win32 instance handle");
    ReleaseDC(ctx->win32_hwnd, ctx->win32_hdc);
    DestroyWindow(ctx->win32_hwnd);
    ctx->is_destroyed = true;
}

void os_window_context_free(WindowContext* ctx){
    os_free_memory(ctx);
}

bool os_load_file_into_memory_arena(String file_path, MemoryArena* arena){

    /**
        convert to null terminated string.
    **/
    char* nt_file_path = NULL;
    i32 nt_file_path_length = 0;
    string_to_ntstring(file_path, &transient_memory, &nt_file_path, &nt_file_path_length);
    
    /**
        open file.
    **/
    HANDLE file = CreateFileA(nt_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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

void* os_load_file(String file_path, size_t* out_buffer_size){

    /**
        convert to null terminated string.
    **/
    char* nt_file_path = NULL;
    i32 nt_file_path_length = 0;
    string_to_ntstring(file_path, &transient_memory, &nt_file_path, &nt_file_path_length);

    /**
        open file.
    **/
    HANDLE file = CreateFileA(nt_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
    void* buffer = os_alloc_memory(bytes_to_read+1); // add +1 for the NULL terminator.

    /**
        read file into buffer.
    **/
    DWORD bytes_read = 0;
    size_t total_bytes_read = 0;
    while(bytes_to_read > 0){
        BOOL read_success = ReadFile(file, (u8*)buffer + total_bytes_read, (DWORD)bytes_to_read, &bytes_read, NULL);
        if(!read_success){
            ASSERT(false, "failed to read file contents.");
            os_free_memory(buffer);
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

i32 os_write_file(String file_path, void* data, size_t data_size, FileWriteType write_type){
    /**
        convert to null terminated string.
    **/
    char* nt_file_path = NULL;
    i32 nt_file_path_length = 0;
    string_to_ntstring(file_path, &transient_memory, &nt_file_path, &nt_file_path_length);
    
    HANDLE h;
    switch(write_type){
        case FileWriteType_Truncate:{
            h = CreateFileA(
                nt_file_path,
                GENERIC_WRITE,              // we want to write.
                0,                          // no sharing while we write.
                NULL,                       // default security attributes.
                CREATE_ALWAYS,              // overwrite if it exists, create if it doesnt.
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
        }break;
        case FileWriteType_Append:{
            h = CreateFileA(
                nt_file_path,
                FILE_APPEND_DATA,           // request append-specific access.
                0,                          // no sharing while we write.
                NULL,                       // default security attributes.
                OPEN_ALWAYS,                // open if it exits, create if it doesnt.
                FILE_ATTRIBUTE_NORMAL,
                NULL
            );
        }break;
        default:{
            ASSERT(false, "unknown file write type.");
            return 0;
        }break;
    }

    if(h == INVALID_HANDLE_VALUE){
        ASSERT(false, "failed to open/create file to write to.");
        return 0;
    }

    DWORD bytes_written = 0;
    BOOL ok = WriteFile(
        h,
        data,
        (DWORD)data_size,
        &bytes_written,
        NULL // no overlapped I/O.
    );

    CloseHandle(h);

    return ok && (bytes_written == data_size);
}

bool os_delete_file(String file_path){
    /**
        convert to null terminated string.
    **/
    char* nt_file_path = NULL;
    i32 nt_file_path_length = 0;
    string_to_ntstring(file_path, &transient_memory, &nt_file_path, &nt_file_path_length);    

    if(DeleteFileA(nt_file_path)){
        return true;
    }
    return false;
}

f32 os_window_calc_aspect_ratio(WindowContext ctx){
    return (f32)ctx.width / (f32)ctx.height;
}

void os_print_msg(char* msg){
    OutputDebugStringA(msg);
}

bool os_load_image(Image* out_image, String file_path){
    ASSERT(out_image->pixel == NULL, "image already init.");
    i32 comp;
    // 4 channels for RGBA always being output.
    i32 desired_channels = 4;
    size_t buffer_size = (file_path.length + 1) * sizeof(char);
    void* buffer = os_alloc_memory(buffer_size);
    ZERO_MEMORY(buffer, buffer_size);
    COPY_MEMORY(buffer, file_path.chars, file_path.length);
    u8* ptr = stbi_load((char*)buffer, &out_image->width, &out_image->height, &comp, desired_channels);
    os_free_memory(buffer);
    if(ptr == NULL){
        ASSERT(false, "failed to load image.");
        return false;
    }
    out_image->pixel = ptr;
    out_image->pixel_length = desired_channels * out_image->width * out_image->height * sizeof(u8);
    return true;
}

bool os_free_image(Image* image){
    if(image->pixel == NULL){
        return false;
    }

    stbi_image_free(image->pixel);
    // zero out image once free;
    *image = (Image){0};
    return true;
}

bool os_copy_file(String dst_file_path, String src_file_path, bool fail_if_exists){

    char* nt_src_file_path = NULL;
    i32 nt_src_file_path_length = 0;
    string_to_ntstring(src_file_path, &transient_memory, &nt_src_file_path, &nt_src_file_path_length);    

    char* nt_dst_file_path = NULL;
    i32 nt_dst_file_path_length = 0;
    string_to_ntstring(dst_file_path, &transient_memory, &nt_dst_file_path, &nt_dst_file_path_length);    
    
    BOOL success = CopyFile(
      nt_src_file_path,
      nt_dst_file_path,
      fail_if_exists // bFailIfExists
    );
    
    return success;
}

i32 os_get_max_file_path_length(){
    return MAX_PATH; 
}

/*
    `remarks`:
    `in_file_path`: is only written to when supplied with a valid pointer; passing NULL will do nothing. 
    `out_file_path_length`: always returns the amount of file paths that are required to be written; to the `in_file_path`.
*/
void os_get_all_file_paths(String directory_path, String* in_file_path, i32* out_file_path_length){
    WIN32_FIND_DATAA find_data;
    HANDLE handle;
    
    char* nt_directory_path;
    i32 nt_directory_path_length;
    string_to_ntstring(directory_path, &transient_memory, &nt_directory_path, &nt_directory_path_length);

    // example:
    // // "*" matches everything; use "*.txt" to filter by extension
    // handle = FindFirstFileA("C:\\MyFolder\\*", &findData);
    handle = FindFirstFileA(nt_directory_path, &find_data);
    
    *out_file_path_length = 0;
    
    if(handle == INVALID_HANDLE_VALUE){
        ASSERT(false, "invalid file path.");
        return;
    }
    
    while(true){
        // skip dot files ('.') and sub directories ('..').
        if(
            strcmp(find_data.cFileName, ".") != 0 &&
            strcmp(find_data.cFileName, "..") != 0 &&
            !(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        ){
            if(in_file_path){
                string_push_ntstring(&in_file_path[*out_file_path_length], find_data.cFileName);
            }
            *out_file_path_length += 1;
        }
        
        if(!FindNextFileA(handle, &find_data)){
            break;
        }
    }

    FindClose(handle);
}