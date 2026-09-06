#ifndef PLATFORM_H
#define PLATFORM_H

#include "base_layer/base.h"

/**
    A handle to a application window; including additional meta-data.
**/
typedef struct{
    void* win32_hwnd;
    void* win32_hdc;
    void* win32_hinstance;
    i32 height;
    i32 width;
    bool is_destroyed;
} WindowContext;

typedef struct{
    u8* pixel;
    i32 pixel_length;
    i32 width;
    i32 height;
} Image;

void platform_output_message(char* msg);
void* platform_alloc_memory(size_t size);
void platform_free_memory(void* memory);
void platform_init_persistent_memory(size_t size);
void platform_init_transient_memory(size_t size);
void platform_error_message_box(char* message);
/**
    creates an application window.

    `returns`
    a context structure as a handle to the newly created window.

    `remarks`
    the context struct should be freed with `platform_window_context_free(ctx)`, once the context's `has_closed` is set to `true`.
**/
WindowContext* platform_window_create(i32 width, i32 height);
void platform_window_update(WindowContext* ctx);
void platform_window_destroy(WindowContext* ctx);
void platform_window_context_free(WindowContext* ctx);
MemoryArena* platform_get_persistent_memory();
MemoryArena* platform_get_transient_memory();
/**
    reads and loads a file from disc into a memory-arena.
**/
bool platform_read_file(String file_path, MemoryArena* arena);
/**
    reads and loads a file from disc into memory.

    `remarks`
    this function dynamically allocates, call `platform_free_memory()` on the return pointer when no longer in use.
    
    `returns`
    a pointer to the loaded data; otherwise NULL in the case of a file read failure.
**/
void* platform_load_file(String file_path, size_t* out_buffer_size);
f32 platform_window_calc_aspect_ratio(WindowContext window_ctx);
u128 platform_get_system_tick();
u128 platform_get_proccess_tick();

void platform_print_msg(char* msg);

bool platform_load_image(Image* out_image, String file_path);
bool platform_free_image(Image* image);

#endif
