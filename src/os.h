#ifndef OS_H
#define OS_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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

typedef enum{
    FileWriteType_Truncate, // 0
    FileWriteType_Append  // 1
} FileWriteType;

void os_output_message(char* msg);
void* os_alloc_memory(size_t size);
void os_free_memory(void* memory);
void os_init_persistent_memory(size_t size);
void os_init_transient_memory(size_t size);
void os_error_message_box(char* message);
/**
    creates an application window.

    `returns`
    a context structure as a handle to the newly created window.

    `remarks`
    the context struct should be freed with `os_window_context_free(ctx)`, once the context's `has_closed` is set to `true`.
**/
WindowContext* os_window_create(i32 width, i32 height);
void os_window_update(WindowContext* ctx);
void os_window_destroy(WindowContext* ctx);
void os_window_context_free(WindowContext* ctx);
MemoryArena* os_get_persistent_memory();
MemoryArena* os_get_transient_memory();
/**
    reads and loads a file from disc into a memory-arena.
**/
bool os_read_file(String file_path, MemoryArena* arena);
/**
    reads and loads a file from disc into memory.

    `remarks`
    this function dynamically allocates, call `os_free_memory()` on the return pointer when no longer in use.

    `returns`
    a pointer to the loaded data; otherwise NULL in the case of a file read failure.
**/
void* os_load_file(String file_path, size_t* out_buffer_size);
i32 os_write_file(String file_path, void* data, size_t data_size, FileWriteType write_type);
bool os_delete_file(String file_path);
bool os_copy_file(String dst_file_path, String src_file_path, bool fail_if_exists);

f32 os_window_calc_aspect_ratio(WindowContext window_ctx);
u128 os_get_system_tick();
u128 os_get_proccess_tick();

void os_print_msg(char* msg);

bool os_load_image(Image* out_image, String file_path);
bool os_free_image(Image* image);

i32 os_get_max_file_path_length();

#endif
