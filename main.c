#include "base_layer/base.c"
#include "base_layer/base_memory.c"
#include "base_layer/base_simd.c"
#include "base_layer/base_math.c"

typedef struct{
    char a;
    char b;
    char c;
    char d;
} Person;

int main(void){
    MemoryArena arena1;
    MemoryArena arena2;
    init_memory_arena(&arena1, Kilobyte(1));
    init_memory_arena(&arena2, Kilobyte(1));

    Soa_Aabb soa = {0};
    init_soa_aabb(&soa, &arena1, 3);
    append_soa_aabb(&soa, -1.0f, -1.0f, 2.0f, 2.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 3.0f, 3.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 4.0f, 4.0f);

    f32* centroids_x;
    size_t centroids_x_size;
    f32* centroids_y;
    size_t centroids_y_size;

    ALLOC_ARRAY_MEMORY_ARENA(&arena1, centroids_x, &centroids_x_size, 3);
    ALLOC_ARRAY_MEMORY_ARENA(&arena1, centroids_y, &centroids_y_size, 3);
    calculate_centroids_soa_aabb(&soa, centroids_x, centroids_y);
}