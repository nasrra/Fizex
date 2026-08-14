#include "platform.h"
#include "base_layer/base.h"
#include "base_layer/base_simd.c"
#include "base_layer/base_math.c"
#include "input/input.c"
#include "renderer/renderer.c"

typedef struct{
    i32 num;
} Person;

DEFINE_QUICKSORT_STRUCT(Person, i32, .num, quicksort_person);

void app_main(){
    platform_init_persistent_memory(MEGABYTE(4));    
    platform_init_transient_memory(MEGABYTE(1));    
    i32 asd;
    asd = 12;
    i32 nums[4] = {0};
    i32* nums_ptr = nums;
    i32 nums_length = 4;
    i32 nums_count = 0;
    ARRAY_PUSH(nums_ptr, nums_length, &nums_count, 12);
    ARRAY_PUSH(nums_ptr, nums_length, &nums_count, 33);
    ARRAY_PUSH(nums_ptr, nums_length, &nums_count, 34);
    ARRAY_PUSH(nums_ptr, nums_length, &nums_count, 132);

}

GenId a;
GenId b;

void app_update_and_render(MemoryArena* persistent, MemoryArena* transient){

    Soa_Aabb soa = {0};
    init_soa_aabb(&soa, transient, 3);
    append_soa_aabb(&soa, -1.0f, -1.0f, 2.0f, 2.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 3.0f, 3.0f);
    append_soa_aabb(&soa, -1.0f, -1.0f, 4.0f, 4.0f);

    f32* nums = (f32[]){2.0f, 1.0f, 9.0f, 11.0f, 16.0f, -33.3f, -120.0f, 99.0f};
    f32* num_0 = &nums[0];
    f32* num_1 = &nums[1];
    f32* num_2 = &nums[2];
    f32* num_3 = &nums[3];
    f32* num_4 = &nums[4];
    f32* num_5 = &nums[5];
    f32* num_6 = &nums[6];
    f32* num_7 = &nums[7];

    quicksort_f32_dsc(nums, 8);
    quicksort_f32_asc(nums, 8);


    Person* persons = (Person[]){
        (Person){.num = 99}, 
        (Person){.num = 2}, 
        (Person){.num = -32}, 
    };
    Person* person_0 = &persons[0];
    Person* person_1 = &persons[1];
    Person* person_2 = &persons[2];
    quicksort_person_asc(persons, 3);
    quicksort_person_dsc(persons, 3);

    
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