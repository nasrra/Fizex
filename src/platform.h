#ifndef PLATFORM_H
#define PLATFORM_H

typedef struct{
    void* win32_hwnd;
    void* win32_hdc;
    void* win32_hinstance;
} WindowCtx;

void platform_output_message(char* msg);
void* platform_alloc_memory(size_t size);
void platform_free_memory(void* memory);
void platform_init_persistent_memory(size_t size);
void platform_init_transient_memory(size_t size);
void platform_error_message_box(char* message);

#endif