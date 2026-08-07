
// /*========================================
//     includes.
// ========================================*//**/
// /*
//     you may need to install wayland dev packages:
//         Debian: 
//             sudo apt install libwayland-dev wayland-protocols
//         Fedora: 
//             sudo dnf install wayland-devel wayland-protocols-devel
//         Arch: 
//             sudo pacman -S wayland wayland-protocols

//     to get the xdbg shell.c file use the command (from root):
//         wayland-scanner private-code usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml xdg-shell-protocol.c
// */
// #include <wayland-client.h>
// #include <fcntl.h>
// #include <sys/mman.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include "xdg-shell-client-protocol.h"

// /*========================================
//     types.
// ========================================*//**/

// typedef struct{
//     /*
//         a wayland surface is the container that holds the pixel data for the compoistor to form an image.
//     */
//     struct wl_surface* surface;
//     /*
//         remarks:
//         this is the same as `surface` but represented through xdbg.
//     */
//     struct xdg_surface* x_surface;
//     /*
//         this is the  buffer sent to wayland to form the image.
//     */
//     struct wl_buffer* buffer;
//     u8* pixels;
//     u32 width;
//     u32 height;
// } WaylandWindow;

// /*========================================
//     globals.
// ========================================*//**/

// struct wl_compositor* linux_wayland_compositor;
// struct wl_shm* linux_wayland_shared_memory;
// struct xdg_wm_base* linux_xdg_shell;
// struct xdg_toplevel* linux_xdg_toplevel;

// /*========================================
//     functions.
// ========================================*//**/

// /*
//     tells us when globals no longer exist within the wayland registry.
//     i.e. confirmation from the server that a global has been removed.
// */
// void linux_wayland_reg_global_remove(void* data, struct wl_registry* reg, u32 name){

// }

// void linux_xdb_draw(WaylandWindow* window){
//     memset(window->pixels, 255, window->width * window->height * 4);
//     wl_surface_attach(window->surface, window->buffer, 0, 0);
//     // 'damage' tells the wayland server what region of the buffer you want to draw.
//     wl_surface_damage_buffer(window->surface, 0, 0, window->width, window->height);
//     // commit the window surface to the wayland server to be composited.
//     wl_surface_commit(window->surface);
// }

// void linux_xdg_surface_configure(void* data, struct xdg_surface* surface, u32 serial){
    
//     WaylandWindow* window = (WaylandWindow*)data;
//     xdg_surface_ack_configure(surface, serial);

//     linux_xdb_draw(window);
    
// }

// void linux_xdg_top_configure(void* data, struct xdg_toplevel* top, i32 width, i32 height, struct wl_array* array){

// }

// void linux_xdg_top_close(void* data, struct xdg_toplevel* top){

// }

// void linux_xdg_shell_ping(void* data, struct xdg_wm_base* shell, u32 serial){
//     xdg_wm_base_pong(shell, serial);
// }

// /*
//     returns:
//     the file descriptor of the allocated page of memory.
// */
// i32 linux_wayland_alloc_shared_memory(size_t size){
//     /*
//         create a random name as the kernel requires a page file with a unique name;
//         otherwise if two files have the same name, they may share the same shared memory.
//     */
//     char name[64];
//     name[0] = '/';
//     for(size_t i = 1; i < 63; i++){
//         name[i] = (char)rand_i32();
//     }

//     /*
//         read write, create, exclusive.
//     */
//     int open_flags = O_RDWR | O_CREAT | O_EXCL; 
//     /*
//         read and write for both user and others.
//     */
//     mode_t open_mode = S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH;
//     i32 fd = shm_open(name, open_flags, open_mode);
//     shm_unlink(name);
//     /*
//         clear the file of any garbage data.
//     */
//     ftruncate(fd, size);

//     return fd;
// }

// void linux_wayland_window_set_size(WaylandWindow* window, u32 width, u32 height){
    
//     // clean up resources if they exist.
//     if(window->buffer){
//         wl_buffer_destroy(window->buffer);
//         window->buffer = NULL;
//     }

//     if(window->pixels){
//         size_t buffer_size = window->width * window->height * 4; // *4 for RGBA.
//         munmap(window->pixels, buffer_size);
//         window->pixels = NULL;
//     }

//     size_t buffer_size = width * height * 4; // *4 for RGBA.    
//     i32 fd = linux_wayland_alloc_shared_memory(buffer_size);
//     struct wl_shm_pool* pool = wl_shm_create_pool(linux_wayland_shared_memory, fd, buffer_size);
//     i32 buffer_stride = width * 4; // 4 bytes per pixels (RGBA)

//     // set and create new values.
//     window->pixels = mmap(NULL, buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     window->buffer = wl_shm_pool_create_buffer(pool, 0, width, height, buffer_stride, WL_SHM_FORMAT_ARGB8888);
//     window->width = width;
//     window->height = height;

//     // procedure cleanup.
//     wl_shm_pool_destroy(pool);
//     close(fd);
// }

// /*
//     this function defines what globals will be needed in the wayland registry.
//     i.e. things that are consistent between the wayland client and the server.

//     parameters:
//     `global_id`: the id assigned to the global in the way land registry.
//     `global_name`: the name of the global that the server is attempting to send to us.
// */
// void linux_wayland_reg_global(void* data, struct wl_registry* registry, u32 global_id, const char* global_name, u32 version){
//     /*
//         check if the name of the global is that of the compositor.
//     */
//     if(!strcmp(global_name, wl_compositor_interface.name)){
//         linux_wayland_compositor = wl_registry_bind(registry, global_id, &wl_compositor_interface, 4);
//     }
//     else if(!strcmp(global_name, wl_shm_interface.name)){
//         linux_wayland_shared_memory = wl_registry_bind(registry, global_id, &wl_shm_interface, 1);
//     }
//     else if(!strcmp(global_name, xdg_wm_base_interface.name)){
//         linux_xdg_shell = wl_registry_bind(registry, global_id, &xdg_wm_base_interface, 1);
//         struct xdg_wm_base_listener xdg_wm_base_listener = {.ping = linux_xdg_shell_ping};
//         xdg_wm_base_add_listener(linux_xdg_shell, &xdg_wm_base_listener, 0);
//     }
// }

// bool linux_wayland_init(){
    
//     // look for an active wayland server.
//     struct wl_display* disp = wl_display_connect(0);
//     if(!disp){
//         return false;
//     }

//     // init registry.
//     struct wl_registry_listener listener = {0};
//     listener.global = linux_wayland_reg_global;
//     listener.global_remove = linux_wayland_reg_global_remove;
//     struct wl_registry* reg = wl_display_get_registry(disp);
//     wl_registry_add_listener(reg, &listener, 0);

//     // block until fully setup.
//     wl_display_roundtrip(disp);

//     WaylandWindow window = {0};
//     window.surface = wl_compositor_create_surface(linux_wayland_compositor);
//     linux_wayland_window_set_size(&window, 1280, 720);

//     // block until fully setup.
//     wl_display_roundtrip(disp);

//     struct xdg_surface_listener x_surface_listener = {.configure = linux_xdg_surface_configure};
//     window.x_surface = xdg_wm_base_get_xdg_surface(linux_xdg_shell, window.surface);
//     xdg_surface_add_listener(window.x_surface, &x_surface_listener, &window);

//     // block until fully setup.
//     wl_display_roundtrip(disp);

//     struct xdg_toplevel_listener xdg_toplevel_listener = {.configure = linux_xdg_top_configure, .close = linux_xdg_top_close};
//     linux_xdg_toplevel = xdg_surface_get_toplevel(window.x_surface);
//     xdg_toplevel_add_listener(linux_xdg_toplevel, &xdg_toplevel_listener, 0);
//     xdg_toplevel_set_title(linux_xdg_toplevel, "wayland_client");
//     wl_surface_commit(window.surface);

//     // block until fully setup.
//     wl_display_roundtrip(disp);

//     while(wl_display_dispatch(disp));


//     // clean up.
//     xdg_toplevel_destroy(linux_xdg_toplevel);
//     wl_surface_destroy(window.surface);
//     wl_display_disconnect(disp);

//     return true;
// }

