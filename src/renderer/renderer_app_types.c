typedef struct{
    Matrix4x4 world_camera_matrix;
    Matrix4x4 screen_camera_matrix;
    f32 time;
    f32 world_camera_far_z;
    f32 world_camera_near_z;
    f32 padding_0;
} Ubo;

#define FONT_HEIGHT_IN_PIXELS 48

#define SPRITE_LAYER_WORLD 0
#define SPRITE_LAYER_UI 1

#define SPRITE_MATERIAL_DEBUG 1
#define SPRITE_MATERIAL_IMAGE 2
#define SPRITE_MATERIAL_TEXT 3