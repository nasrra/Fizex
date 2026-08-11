#include "base_layer/base.h"
#include "platform.h"
#include "base_layer/base_simd.c"
#include "base_layer/base_math.c"
#include "input/input.c"
#include "renderer/renderer.c"

typedef struct{
    char a;
    char b;
    char c;
    char d;
} Person;

void app_main(){
    platform_init_persistent_memory(MEGABYTE(4));    
    platform_init_transient_memory(MEGABYTE(1));    
}

GenId a;
GenId b;

void app_update_and_render(MemoryArena* persistent, MemoryArena* transient){
 
    Soa_Aabb soa = {0};
    init_soa_aabb(&soa, transient, 3);
    append_soa_aabb(&soa, -1.0f, -1.0f, 2.0f, 2.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 3.0f, 3.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 4.0f, 4.0f);

    f32* centroids_x;
    i32 centroids_x_size;
    f32* centroids_y;
    i32 centroids_y_size;

    MEMORY_ARENA_ALLOC_ARRAY(transient, centroids_x, &centroids_x_size, 3);
    MEMORY_ARENA_ALLOC_ARRAY(transient, centroids_y, &centroids_y_size, 3);
    calculate_centroids_soa_aabb(&soa, centroids_x, centroids_y);

    i32 i_a = rand_i32();
    i32 i_b = rand_i32();
    i32 i_c = rand_i32();
    i32 i_d = rand_i32();
    i32 i_e = rand_i32();

}