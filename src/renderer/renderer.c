#include "vendors/webgpu/webgpu.h"

/*====================
    types.
====================*//**/

typedef enum{
    CameraProjectionType_Orthographic,
    CameraProjectionType_Perspective
} CameraProjectionType;

/**
    `remarks`
    member values shouldn't be directly modified. 
**/
typedef struct{
    Matrix4x4 projection;
    Matrix4x4 view;
    Matrix4x4 model;
    Vector3 position;
    f32 orthographic_size;
    f32 perspective_fov;
    f32 far_z;
    f32 near_z;
    CameraProjectionType projection_type;
    bool is_init;
} Camera;

typedef struct{
    f32 x;
    f32 y;
    u32 width;
    u32 height;
} DestinationRectangle;

typedef struct{
    WGPUSurfaceTexture ptr;
    WGPUTextureView view;
    WGPUExtent3D extents;
} SurfaceTexture;

typedef struct{
    WGPUBuffer host;
    WGPUBuffer device;
    /*
        The length of the `host` and `device` buffers respectively.
    */
    u32 length_in_bytes;
    /*
        The count of the `host` and `device` buffers respectively.
    */
    u32 count_in_bytes;
} RenderBuffer;

typedef struct{
    Vector3 position;
    Vector2 uv;
} Vertex;

typedef struct {
    bool request_ended;
    bool is_valid;
} BufferMapAsyncCallbackCtx;

typedef struct{
    WGPUTexture ptr;
    WGPUTextureView view;
    /*
        `remarks`:
        This is a copied value from texture creation and wont change the underlying texture extents.    
    */
    WGPUExtent3D extents;
    bool is_init;
} Texture;

typedef struct{
    WGPUTexture texture;
    /*
        `remarks`:
        this is a copied value from texture creation and wont change the underlying texture extents.
    */
    WGPUTextureView view;
    WGPUExtent3D extents;
    /*
        `remarks`
        this is a stack-array.
    */
    i32* free_layer_indices_stack;
    i32 free_layer_indices_stack_length;
    bool is_init;
} TextureArray;

typedef struct{
    /*
        The amount of pixels to move the glyph so it is at the expected position along the origin line.
    */
    Vector2 offset;
    /*
        The amount of pixels to move before drawing the next glyph.
    */
    Vector2 advance;
    /*
        The coordinates of the top left point of this glyph in its generated texture atlas.
    */
    Vector2 texture_coords;
    /*
        The size of this glyph's quad in its generated texture atlas.
    */
    Vector2 size;
} Glyph;

typedef struct{
    u32 base_glyph_index;
    u32 max_glyph_height_in_pixels;
    Glyph* glyphs;
    i32 glyphs_length;
} FontData;

typedef enum{
    VirtualTextureType_Image,
    VirtualTextureType_Font
} VirtualTextureType;

/*
    The maximum amount of virtual textures a shader can store.
    
    remarks:
    This is because a virtual texture's size is 16 bytes and the default max size of a WebGPU uniform buffer is 64Kib;
    so only 4096 unique texture ids can be stored; which is more than enough for most games.
*/
typedef struct{
    i32 shader_texture_array_binding;
    i32 texture_array_layer_index;
    i32 is_loaded;
    i32 padding_0;
} DeviceVirtualTexture;

typedef struct{
    String file_path;
    FontData font_data;
    VirtualTextureType texture_type;
    bool is_init;
} HostVirtualTexture;

typedef struct{
    /*
        remarks:
        - Contains a NIL element.
        - Elements are vertically associated with `host_virtual_textures`.
    */
    DeviceVirtualTexture* device_virtual_textures;
    i32 device_virtual_textures_length;
    /*
        remarks:
        - Contains a NIL element.
        - Elements are vertically associated with `device_virtual_textures`.
    */
    HostVirtualTexture* host_virtual_textures;
    i32 host_virtual_textures_length;
    TextureArray* texture_arrays;
    i32 texture_arrays_length;
    RenderBuffer device_virtual_texture_buffer;
    bool is_init;
} VirtualTextureManager;

typedef struct{
    f32 x;
    f32 y;
    f32 height;
    f32 width;
} SpriteRegion;

typedef struct{
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} Colour;

typedef enum{
    SpriteState_Deallocated = 0,
    SpriteState_Inactive = 1,
    SpriteState_Active = 2
} SpriteState;

/*
    WGSL requires that the total size of a struct must be a multiple of its largest member's alignment

    Remember to look at alignments and sizes when changing this: 
    https://www.w3.org/TR/WGSL/#alignment-and-size
*/
typedef struct{
    Matrix4x4 transform;
    SpriteRegion region;
    Colour colour;
    SpriteState state;
    i32 virtual_texture;
    i32 material; 
    i32 colour_state;
    i32 layer;
    i32 _padding_0;
    i32 _padding_1;
    i32 _padding_2;
} DeviceSprite;

typedef struct{
    /*
        The positional offset to place this of this glyph within the text/sentence.
    */
    Vector2 glyph_offset;
    /*
        The size - in pixels - of the glyph rectangle on the texture.
    */
    Vector2 glyph_quad_length;
    i32 next_in_chain;
    bool is_first_in_chain;
} HostSprite;

typedef struct{
    i32 max_sprites;
    /*
        remarks:
        this is a stack array.
    */
    i32* free_sprite_indices;
    i32 free_sprite_indices_length;
    i32 free_sprite_indices_count;
} SpriteLayer;

typedef struct{
    /*
        remarks:
        mutating this value will not change the actual windows extents; this is a copied value from initialisation.
    */
    WGPUSurface surface;
    WGPUExtent3D window_extents;
    bool is_init;
} WindowSurface;

typedef struct{
    WGPURenderPipeline render_pipeline;
    WGPUPipelineLayout pipeline_layout;
    WGPUSampler non_filter_sampler;
    WGPUBindGroupLayout bind_group_layout_0;
    WGPUBindGroupLayout bind_group_layout_1;
    WGPUBindGroupLayout bind_group_layout_2;
    WGPUBindGroup bind_group_0;
    WGPUBindGroup bind_group_1;
    WGPUBindGroup bind_group_2;
    bool is_init;
} GraphicsPipeline;

typedef struct{
    WGPURenderPipeline render_pipeline;
    WGPUSampler sampler;
    WGPUBindGroupLayout bind_group_layout;
    WGPUBindGroup bind_group;
    bool is_init;
} BlitPipeline;

typedef enum{
    ColourState_Tint,
    ColourState_Override
} ColourState;

typedef enum{
    SpriteType_Solo,
    SpriteType_Chain
} SpriteType;

typedef struct{
    GenId gen_id;
    /*
        remarks:
        Indicates the layer this sprite ID came from and should return to.
    */
    i32 layer;
} SpriteId;


typedef struct{
    i32 max_sprites;
} SpriteLayerCreateInfo;

typedef struct{
    /*
        the width of the texture to write the glyph data to.
    */
    u32 texture_width;
    /*
        the height of the texture to write the glyph data to.
    */
    u32 texture_height;
    u32 base_glyph_index;
    i32 glyph_count;
    /*
        the indices of the virtual textures to initialise as font textures.
    */
    i32* virtual_textures;
    i32 virtual_textures_length;
} FontTextureInitInfo;

typedef struct{
    u32 width;
    u32 height;
    /*
        the maximum amount of textures of this type that can be loaded at a time.
    */
    u32 max_textures;
} ImageTexturesInitInfo;

typedef struct{
    /*
        remarks:
        The order of the textures determines their binding value within the shader.
        E.g, texture index 0 = @binding(0), texture index 1 = @binding(1), etc ... 
    */
    ImageTexturesInitInfo* image_textures_init_infos;
    i32 image_textures_init_infos_length;
    FontTextureInitInfo font_textures_init_info;
    /*
        remarks:
        The order of the layers determines the order that they are drawn.
        E.g, layer index 4 will be above layers 3, 2, 1, etc, ... 
    */
    SpriteLayerCreateInfo* sprite_layer_create_infos;
    i32 sprite_layer_create_infos_length;
    i32 max_file_path_length;
    i32 max_virtual_textures;
    u32 max_user_uniform_buffer_size_in_bytes;
    u32 max_user_storage_buffer_size_in_bytes;
    u32 final_render_texture_width;
    u32 final_render_texture_height;
    String graphics_pipeline_shader_file_path;
} RendererContextInitInfo;

typedef struct {
    /*
        remarks:
        - contains a `NIL` element.
        - elements vertically align with `host_sprites` and `sprite_generations`.
    */
    DeviceSprite* device_sprites;
    i32 device_sprites_length;
    /*
        remarks:
        - contains a `NIL` element.
        - elements vertically align with `device_sprites` and `sprite_generations`.
    */
    HostSprite* host_sprites;
    i32 host_sprites_length;
    /*
        remarks:
        - contains a `NIL` element.
        - elements vertically align with `host_sprites` and `device_sprites`.
    */
    i32* sprite_generations;
    i32 sprite_generations_length;
    /*
        remarks:
        this is a stack-array.
    */
    SpriteId* one_frame_sprites_stack;
    i32 one_frame_sprites_stack_length;
    i32 one_frame_sprites_stack_count;
    DeviceSprite* device_sprites_scratch_space;
    i32 device_sprites_scratch_space_length;
    SpriteLayer* sprite_layers;
    i32 sprite_layers_length;
    RenderBuffer sprite_buffer;
    bool is_init;
} SpriteManager;

typedef struct{
    WGPUInstance instance;
    WGPUAdapter adapter;
    WGPUDevice device;
    GraphicsPipeline graphics_pipeline;
    BlitPipeline blit_pipeline;
    RenderBuffer vertex_buffer;
    RenderBuffer index_buffer;
    RenderBuffer user_uniform_buffer;
    RenderBuffer user_storage_buffer;
    VirtualTextureManager virtual_texture_manager;
    SpriteManager sprite_manager;
    WindowSurface window_surface;
    /*
        remarks:
        note that web-gpu has its depth range set from 0 (near) to 1 (far).
    */
    Texture depth_texture;
    Texture final_render_texture;
    /*
        the destination rectangle for renderering the final render texture onto the back-buffer.
    */
    DestinationRectangle destination_rectangle;
    bool is_init;
} RendererContext;

/*====================
    defines
====================*//**/

#define COLOUR_RED      (Colour){.r = 1.0f, .a = 1.0f}
#define COLOUR_GREEN    (Colour){.g = 1.0f, .a = 1.0f}
#define COLOUR_BLUE     (Colour){.b = 1.0f, .a = 1.0f}
#define COLOUR_WHITE    (Colour){.r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f}
#define COLOUR_ORANGE   (Colour){.r = 0.5f, .g = 0.5f, .a = 1.0f}
#define COLOUR_YELLOW   (Colour){.r = 1.0f, .g = 1.0f, .a = 1.0f}
#define COLOUR_PINK     (Colour){.r = 1.0f, .g = 0.5f, .b 0.5f, .a = 0.5f}
#define COLOUR_BLACK    (Colour){.a = 1.0f}
#define COLOUR_PURPLE   (Colour){.r = 0.5f, .b = 0.5f, .a = 1.0f}

/*
    The amount of entries in @group(0).
*/
#define BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT 2
/*
    The binding of the sampler object in @group(0).
*/
#define BLIT_PIPELINE_SAMPLER_BINDING 0
/*
    The binding of the texture object in @group(0).
*/
#define BLIT_PIPELINE_TEXTURE_BINDING 1

/*
    The maximum amount of virtual textures a shader can store.
    
    Remarks:
    This is because a virtual texture's size is 16 bytes and the default max size of a WebGPU uniform buffer is 64Kib;
    so only 4096 unique texture ids can be stored; which is more than enough for most games.
*/
#define DEVICE_VIRTUAL_TEXTURE_MAX_AMOUNT 4096

/*
    The value of a vertex shader location is the same as the respective WGSL attribute `@location(g)` in the shader.
*/
#define SHADER_VERTEX_LOCATION_POSITION 0
/*
    The value of a vertex shader location is the same as the respective WGSL attribute `@location(g)` in the shader.
*/
#define SHADER_VERTEX_LOCATION_UV 1
/*
    The value of a vertex shader location is the same as the respective WGSL attribute `@location(g)` in the shader.
*/
#define SHADER_VERTEX_LOCATION_SPRITE_INDEX 2

/*
    The value of a shader group is the same as the respective WGSL attribute `@group(g)` in the shader.</para>
*/
#define SHADER_GROUP_BUFFERS 0
/*
    The value of a shader group is the same as the respective WGSL attribute `@group(g)` in the shader.</para>
*/
#define SHADER_GROUP_TEXTURE_ARRAYS 1
/*
    The value of a shader group is the same as the respective WGSL attribute `@group(g)` in the shader.</para>
*/
#define SHADER_GROUP_UTILITIES 2

/*
    The value of a shader binding is the same as the respective WGSL attribute @binding(b) in the shader.
*/
#define SHADER_BINDING_USER_UNIFORM 0
/*
    The value of a shader binding is the same as the respective WGSL attribute @binding(b) in the shader.
*/
#define SHADER_BINDING_USER_STORAGE 1
/*
    The value of a shader binding is the same as the respective WGSL attribute @binding(b) in the shader.
*/
#define SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM 2
/*
    The value of a shader binding is the same as the respective WGSL attribute @binding(b) in the shader.
*/
#define SHADER_BINDING_SPRITE_STORAGE 3
/*
    The value of a shader binding is the same as the respective WGSL attribute @binding(b) in the shader.
*/
#define SHADER_BINDING_NON_FILTER_SAMPLER 0

/*
    The amount of bindings a shader group has.
*/
#define SHADER_BUFFERS_GROUP_BINDING_COUNT 4
/*
    The amount of bindings a shader group has.
*/
#define SHADER_UTILITIES_GROUP_BINDING_COUNT 1

/*
    The index where font textures are stored within the `TextureArrays` array.
*/
#define VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX 1
/*
    The index where the first image textures are stored within the `TextureArrays` array.
*/
#define VIRTUAL_TEXTURE_MANAGER_IMAGE_TEXTURE_ARRAY_START_INDEX \
            VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX + 1
/*
    The maximum amount of sprites a shader can store.

    remarks:
    This is because a sprite's size is 80 bytes and the default max size of a WebGPU SSBO is 128Mb;
    so only 1,677,721 unique sprites can be stored; which is more than enough for most games.
*/
#define DEVICE_SPRITE_MAX_AMOUNT 1677721

#define VERTEX_SHADER_ENTRY_POINT "vs_main"
#define FRAGMENT_SHADER_ENTRY_POINT "fs_main"

#define DEFINE_SPRITE_SETTER_FUNCTION(MEMBER_NAME, MEMBER_TYPE)                                                 \
                                                                                                                \
void renderer_sprite_set_##MEMBER_NAME##_unsafe(RendererContext* ctx, i32 sprite_index, MEMBER_TYPE MEMBER_NAME){   \
    SpriteManager* sprite_manager = &ctx->sprite_manager;                                                       \
    BOUNDS_CHECK(sprite_index, sprite_manager->device_sprites_length);                                          \
    sprite_manager->device_sprites[sprite_index].##MEMBER_NAME = MEMBER_NAME;                                   \
}                                                                                                               \
                                                                                                                \
bool renderer_sprite_set_##MEMBER_NAME(RendererContext* ctx, SpriteId sprite_id, MEMBER_TYPE MEMBER_NAME){          \
    i32 index = gen_id_get_index(sprite_id.gen_id);                                                             \
    i32 generation = gen_id_get_generation(sprite_id.gen_id);                                                   \
    BOUNDS_CHECK(index, ctx->sprite_manager.sprite_generations_length);                                         \
    if(ctx->sprite_manager.sprite_generations[index] != generation){                                            \
        return false;                                                                                           \
    }                                                                                                           \
    renderer_sprite_set_##MEMBER_NAME##_unsafe(ctx, index, MEMBER_NAME);                                        \
    return true;                                                                                                \
}

#define DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(MEMBER_NAME, MEMBER_TYPE)                                               \
                                                                                                                    \
void renderer_sprite_chain_set_##MEMBER_NAME##_unsafe(RendererContext* ctx, i32 sprite_index, MEMBER_TYPE MEMBER_NAME){ \
    i32 first_index = sprite_index;                                                                                 \
    i32 index = first_index;                                                                                        \
    while(true){                                                                                                    \
        BOUNDS_CHECK(index, ctx->sprite_manager.host_sprites_length);                                               \
        HostSprite* host = &ctx->sprite_manager.host_sprites[index];                                                \
        renderer_sprite_set_##MEMBER_NAME##_unsafe(ctx, index, MEMBER_NAME);                                        \
        index = host->next_in_chain;                                                                                \
        if(index == first_index){                                                                                   \
            break;                                                                                                  \
        }                                                                                                           \
    }                                                                                                               \
}                                                                                                                   \
                                                                                                                    \
bool renderer_sprite_chain_set_##MEMBER_NAME(RendererContext* ctx, SpriteId sprite_id, MEMBER_TYPE MEMBER_NAME){        \
    i32 first_index = gen_id_get_index(sprite_id.gen_id);                                                           \
    i32 generation = gen_id_get_generation(sprite_id.gen_id);                                                       \
                                                                                                                    \
    {                                                                                                               \
        ASSERT(ctx->sprite_manager.is_init == true, "sprite manager not init.");                                    \
        BOUNDS_CHECK(first_index, ctx->sprite_manager.sprite_generations_length);                                   \
        if(ctx->sprite_manager.sprite_generations[first_index] != generation){                                      \
            ASSERT(false, "stale sprite_id.");                                                                      \
            return false;                                                                                           \
        }                                                                                                           \
        BOUNDS_CHECK(first_index, ctx->sprite_manager.host_sprites_length);                                         \
        if(renderer_sprite_is_chain_sprite(ctx->sprite_manager.host_sprites[first_index]) == true){                 \
            ASSERT(false, "sprite is not within a sprite-chain.");                                                  \
            return false;                                                                                           \
        }                                                                                                           \
    }                                                                                                               \
                                                                                                                    \
    renderer_sprite_set_##MEMBER_NAME##_unsafe(ctx, first_index, MEMBER_NAME);                                      \
    return true;                                                                                                    \
}

#define BLIT_SHADER_CODE \
"struct VertexOutput {\n" \
"    @builtin(position) position : vec4<f32>,\n" \
"    @location(0) uv : vec2<f32>,\n" \
"}\n" \
"\n" \
"@vertex\n" \
"fn vs_main(@builtin(vertex_index) vertexIndex : u32) -> VertexOutput {\n" \
"    // Traditional full-screen quad using 4 vertices\n" \
"    var pos = array<vec2<f32>, 4>(\n" \
"        vec2<f32>(-1.0,  1.0), // Top-Left\n" \
"        vec2<f32>(-1.0, -1.0), // Bottom-Left\n" \
"        vec2<f32>( 1.0,  1.0), // Top-Right\n" \
"        vec2<f32>( 1.0, -1.0)  // Bottom-Right\n" \
"    );\n" \
"    var uv = array<vec2<f32>, 4>(\n" \
"        vec2<f32>(0.0, 0.0),\n" \
"        vec2<f32>(0.0, 1.0),\n" \
"        vec2<f32>(1.0, 0.0),\n" \
"        vec2<f32>(1.0, 1.0)\n" \
"    );\n" \
"\n" \
"    var output : VertexOutput;\n" \
"    output.position = vec4<f32>(pos[vertexIndex], 0.0, 1.0);\n" \
"    output.uv = uv[vertexIndex];\n" \
"    return output;\n" \
"}\n" \
"\n" \
"@group(0) @binding(0) var textureSampler : sampler;\n" \
"@group(0) @binding(1) var sourceTexture : texture_2d<f32>;\n" \
"\n" \
"@fragment\n" \
"fn fs_main(@location(0) uv : vec2<f32>) -> @location(0) vec4<f32> {\n" \
"    return textureSample(sourceTexture, textureSampler, uv);\n" \
"}\n"

/**
    Sorts sprites by their Z translation value. Note that this is specically so that transparent objects 
    are sorted correctly as the depth buffer freaks out with tranparency. However, this will only work when the camera is 
    facing down Z+ (e.g, camera position: {0,0,-3} and looking at position {0,0,0}). 
    
    remarks:
    As the camera is not expected to rotate or move from its fixed position; this is okay.
**/
DEFINE_QUICKSORT_STRUCT(DeviceSprite, f32, .transform.m[14], quicksort_device_sprite);

/*====================
    functions.
====================*//**/

inline bool renderer_sprite_is_chain_sprite(HostSprite sprite){
    return sprite.next_in_chain > 0;
}

inline bool renderer_sprite_is_first_in_chain(HostSprite sprite){
    return renderer_sprite_is_chain_sprite(sprite) && sprite.is_first_in_chain;
}

DEFINE_SPRITE_SETTER_FUNCTION(transform, Matrix4x4);
DEFINE_SPRITE_SETTER_FUNCTION(material, i32);
DEFINE_SPRITE_SETTER_FUNCTION(region, SpriteRegion);
DEFINE_SPRITE_SETTER_FUNCTION(virtual_texture, i32);
DEFINE_SPRITE_SETTER_FUNCTION(state, SpriteState);
DEFINE_SPRITE_SETTER_FUNCTION(colour, Colour);
DEFINE_SPRITE_SETTER_FUNCTION(colour_state, ColourState);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(transform, Matrix4x4);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(material, i32);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(region, SpriteRegion);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(virtual_texture, i32);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(state, SpriteState);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(colour, Colour);
DEFINE_SPRITE_CHAIN_SETTER_FUNCTION(colour_state, ColourState);

void renderer_font_data_init(FontData* font_data, MemoryArena* arena, i32 glyph_count, u32 base_glyph_index){
    ASSERT(glyph_count > 1, "font data should be init with a glyph count greater than one to account for the Nil element.");
    glyph_count = CLAMP(glyph_count, 1, I32_MAX);
    font_data->base_glyph_index = base_glyph_index;
    MEMORY_ARENA_ALLOC_ARRAY(arena, font_data->glyphs, &font_data->glyphs_length, glyph_count);
}

void renderer_request_adapter_callback(
    WGPURequestAdapterStatus status, WGPUAdapter adapter, WGPUStringView message, 
    WGPU_NULLABLE void* user_data_1, WGPU_NULLABLE void* user_data_2
){
    WGPUAdapter* out_adapter = (WGPUAdapter*)user_data_1;
    status == WGPURequestAdapterStatus_Success ? (*out_adapter = adapter) : (*out_adapter = NULL);
}

WGPUAdapter renderer_request_adapter(WGPUInstance instance){

    WGPURequestAdapterOptions options = {0};
    options.powerPreference = WGPUPowerPreference_HighPerformance; // default to best gpu.
#if OS_WINDOWS || OS_LINUX
    options.backendType = WGPUBackendType_Vulkan;
#elif OS_MACOS
    options.backendType = WGPUBackendType_Metal;
#else
    #error unrecognised OS;
#endif

    WGPUAdapter wgpu_adapter = {0};
    WGPURequestAdapterCallbackInfo callback_info = {0};
    callback_info.callback = renderer_request_adapter_callback;
    callback_info.userdata1 = &wgpu_adapter;

    wgpuInstanceRequestAdapter(instance, &options, callback_info);
    ASSERT(wgpu_adapter!=NULL, "failed to retrieve adapter.");
    
    return wgpu_adapter;
}

void renderer_adapter_get_limits(WGPUAdapter adapter, WGPULimits* out_limits){
    /*
        reference here whilst looking at this code:
        https://www.w3.org/TR/webgpu/#limits
    */
    
    WGPULimits supported = {0};
    wgpuAdapterGetLimits(adapter, &supported);
    supported.nextInChain = NULL;

    WGPULimits required = {0};

    /**
        undefined means there is no limit.
        limits have to be initialise this way as WebGPU is not ZII      >:( not cool
 
        the default capabilities for a device can be found here:
            https://www.w3.org/TR/webgpu/#limit-default
        note that every adapter is guaranteed to support the default or better:
            https://www.w3.org/TR/webgpu/#limit-default
    **/
    required.maxTextureDimension1D                      = WGPU_LIMIT_U32_UNDEFINED;
    required.maxTextureDimension2D                      = WGPU_LIMIT_U32_UNDEFINED;
    required.maxTextureDimension3D                      = WGPU_LIMIT_U32_UNDEFINED;
    required.maxTextureArrayLayers                      = WGPU_LIMIT_U32_UNDEFINED;
    required.maxBindGroups                              = WGPU_LIMIT_U32_UNDEFINED;
    required.maxBindGroupsPlusVertexBuffers             = WGPU_LIMIT_U32_UNDEFINED;
    required.maxBindingsPerBindGroup                    = WGPU_LIMIT_U32_UNDEFINED;
    required.maxDynamicUniformBuffersPerPipelineLayout  = WGPU_LIMIT_U32_UNDEFINED;
    required.maxDynamicStorageBuffersPerPipelineLayout  = WGPU_LIMIT_U32_UNDEFINED;
    required.maxSampledTexturesPerShaderStage           = WGPU_LIMIT_U32_UNDEFINED;
    required.maxSamplersPerShaderStage                  = WGPU_LIMIT_U32_UNDEFINED;
    required.maxStorageBuffersPerShaderStage            = WGPU_LIMIT_U32_UNDEFINED;
    required.maxStorageTexturesPerShaderStage           = WGPU_LIMIT_U32_UNDEFINED;
    required.maxUniformBuffersPerShaderStage            = WGPU_LIMIT_U32_UNDEFINED;
    required.maxUniformBufferBindingSize                = WGPU_LIMIT_U32_UNDEFINED;
    required.maxStorageBufferBindingSize                = WGPU_LIMIT_U32_UNDEFINED;
    required.maxVertexBuffers                           = WGPU_LIMIT_U32_UNDEFINED;
    required.maxVertexAttributes                        = WGPU_LIMIT_U32_UNDEFINED;
    required.maxVertexBufferArrayStride                 = WGPU_LIMIT_U32_UNDEFINED;
    required.maxInterStageShaderVariables               = WGPU_LIMIT_U32_UNDEFINED;
    required.maxColorAttachments                        = WGPU_LIMIT_U32_UNDEFINED;
    required.maxColorAttachmentBytesPerSample           = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeWorkgroupStorageSize             = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeInvocationsPerWorkgroup          = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeWorkgroupSizeX                   = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeWorkgroupSizeY                   = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeWorkgroupSizeZ                   = WGPU_LIMIT_U32_UNDEFINED;
    required.maxComputeWorkgroupsPerDimension           = WGPU_LIMIT_U32_UNDEFINED;
    required.maxUniformBufferBindingSize                = WGPU_LIMIT_U64_UNDEFINED;
    required.maxStorageBufferBindingSize                = WGPU_LIMIT_U64_UNDEFINED;
    required.maxBufferSize                              = WGPU_LIMIT_U64_UNDEFINED;

    /*
        actual requirements.
    */
    required.maxVertexAttributes = 3;
    required.maxVertexBuffers = 1;
    // 256 is the default web gpu size.
    required.maxBufferSize = MEGABYTE(256);
    // maximum stride is between two vonsecutivve vertice (to make triangles) in the vertex buffer.
    required.maxVertexBufferArrayStride = (u32)sizeof(Vertex);
    /**
        These two limits are different because they are 'minimum' limits, they are the only ones we may forward from the adapter's 
        supported limits as it may cause issuesit they remain undefined (not supported by the adapter).
    **/
    required.minUniformBufferOffsetAlignment = supported.minUniformBufferOffsetAlignment;
    required.minStorageBufferOffsetAlignment = supported.minStorageBufferOffsetAlignment;
    // note that 4 is the guaranteed standard for WebGPU.
    required.maxBindGroups = 4;
    required.maxUniformBuffersPerShaderStage = 2;
    // 64Kib as defined as the default uniform buffer size by WebGPU. 
    required.maxUniformBufferBindingSize = KILOBYTE(64);
    // 128Mb is defined as the default storage buffer size by WebGPU.
    required.maxStorageBufferBindingSize = MEGABYTE(128);
    // set the max required height of a texture; in pixels.
    required.maxTextureDimension1D = 2160;
    // set the max required width of a texture; in pixels.
    required.maxTextureDimension2D = 4096;
    required.maxTextureArrayLayers = 256;
    
    *out_limits = required;
}

void renderer_adapter_request_device_callback(
    WGPURequestDeviceStatus status, WGPUDevice device, WGPUStringView message, 
    WGPU_NULLABLE void* user_data_1, WGPU_NULLABLE void* user_data_2
){
    WGPUDevice* out_device = (WGPUDevice*)user_data_1;
    status == WGPURequestDeviceStatus_Success ? (*out_device = device) : (*out_device = NULL);
}

WGPUDevice renderer_request_device(WGPUAdapter adapter){
    
    WGPULimits limits = {0};
    renderer_adapter_get_limits(adapter, &limits);
    
    WGPUAdapterInfo adapter_info = {0};
    wgpuAdapterGetInfo(adapter, &adapter_info);

    WGPUDeviceDescriptor desc = {0};
    desc.label = adapter_info.device;
    desc.requiredLimits = &limits;

    WGPUDevice wgpu_device = {0};
    WGPURequestDeviceCallbackInfo callback_info = {0};
    callback_info.callback = renderer_adapter_request_device_callback;
    callback_info.userdata1 = &wgpu_device; 
    
    wgpuAdapterRequestDevice(adapter, &desc, callback_info);
    ASSERT(wgpu_device!=NULL, "Failed to get the gpu device");

    return wgpu_device;
}

void renderer_buffer_init(RenderBuffer* buffer, WGPUDevice device, WGPUBufferUsage host_usage, WGPUBufferUsage device_usage, u32 array_length, u32 element_length_in_bytes){

    *buffer = (RenderBuffer){0};

    buffer->length_in_bytes = element_length_in_bytes * array_length;
    /*
        create the host staging buffer.
    */
    WGPUBufferDescriptor host_desc = {0};
    host_desc.size = buffer->length_in_bytes;
    host_desc.usage = host_usage;
    host_desc.mappedAtCreation = false;
    buffer->host = wgpuDeviceCreateBuffer(device, &host_desc);
    /*
        create the device local buffer
    */
    WGPUBufferDescriptor device_desc = {0};
    device_desc.size = buffer->length_in_bytes;
    device_desc.usage = device_usage;
    buffer->device = wgpuDeviceCreateBuffer(device, &device_desc);
}

void renderer_texture_array_init(TextureArray* array, WGPUDevice device, MemoryArena* arena, WGPUTextureFormat format, u32 width, u32 height, u32 layer_count){
    
    { // validation steps.
        ASSERT(array->is_init==false, "attempted to init an already init texture array.");
    }

    // create the texture.
    WGPUTextureDescriptor text_desc = {0};
    text_desc.size.width = width;
    text_desc.size.height = height;
    text_desc.size.depthOrArrayLayers = layer_count;
    text_desc.mipLevelCount = 1;
    text_desc.sampleCount = 1;
    text_desc.dimension = WGPUTextureDimension_2D;
    text_desc.format = format;
    text_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
    text_desc.viewFormatCount = 0;
    array->texture = wgpuDeviceCreateTexture(device, &text_desc);

    // create the view.
    WGPUTextureViewDescriptor view_desc = {0};
    view_desc.format = format;
    view_desc.dimension = WGPUTextureViewDimension_2DArray;
    view_desc.baseMipLevel = 0;
    view_desc.mipLevelCount = 1;
    view_desc.baseArrayLayer = 0;
    view_desc.arrayLayerCount = layer_count;    
    view_desc.aspect = WGPUTextureAspect_All;
    array->view = wgpuTextureCreateView(array->texture, &view_desc);

    // create the free texture indices for all textures.
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->free_layer_indices_stack, &array->free_layer_indices_stack_length, layer_count);
    for(i32 i = 0; i < array->free_layer_indices_stack_length; i++){
        array->free_layer_indices_stack[i] = i;
    }

    array->extents = text_desc.size;
    array->is_init = true;
}

void renderer_virtual_texture_manager_init(
    VirtualTextureManager* manager, MemoryArena* arena, WGPUDevice device, ImageTexturesInitInfo* image_infos, i32 image_infos_length,
    FontTextureInitInfo font_info, i32 max_virtual_textures, i32 file_path_max_chars 
){
    { // validation.
        ASSERT(!manager->is_init, "virtual texture manager is already init.");
        ASSERT(device, "cannot init virtual texture manager with a null gpu device.");
        ASSERT(max_virtual_textures >= 2, "virtual texture manager should not be initialised with less than 2 virtual textures.");
        ASSERT(max_virtual_textures <= DEVICE_VIRTUAL_TEXTURE_MAX_AMOUNT, "max virtual textures exceeds 4096.");
    }

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->device_virtual_textures, &manager->device_virtual_textures_length, max_virtual_textures);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->host_virtual_textures, &manager->host_virtual_textures_length, max_virtual_textures);
    
    // initialise virtual textures.
    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    renderer_buffer_init(&manager->device_virtual_texture_buffer, device, host_usage, device_usage, max_virtual_textures, sizeof(DeviceVirtualTexture));
    for(i32 i = 0; i < manager->host_virtual_textures_length; i++){
        BOUNDS_CHECK(i,manager->host_virtual_textures_length);
        string_init(&manager->host_virtual_textures[i].file_path, arena, file_path_max_chars);
    }

    // initialise font virtual textures.
    for(i32 i = 0; i < font_info.virtual_textures_length; i++){
        i32 virtual_texture = font_info.virtual_textures[i];

        BOUNDS_CHECK(virtual_texture, manager->host_virtual_textures_length);
        HostVirtualTexture* host = &manager->host_virtual_textures[virtual_texture];

        renderer_font_data_init(&host->font_data, arena, font_info.glyph_count, font_info.base_glyph_index);
        host->texture_type = VirtualTextureType_Font;
    }

    /*
        initialise texture arrays.
        +1 for the nil entry and the font texture array.
    */
    i32 texture_array_count = image_infos_length + VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX + 1;
    i32 write_index = 0;
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->texture_arrays, &manager->texture_arrays_length, texture_array_count);
    
    // init the nil.
    WGPUTextureFormat nil_texture_format = WGPUTextureFormat_R8Unorm; // the format should be the least taxing on VRAM storage. 
    BOUNDS_CHECK(write_index, manager->texture_arrays_length);
    renderer_texture_array_init(&manager->texture_arrays[write_index], device, arena, nil_texture_format, 1, 1, 1);

    // init the font texture.
    write_index = VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX;
    /*
        Font textures are an array of bytes ranging from 0-256 for one channel ('red' - relative to the gpu - as it is a one channel value).
        These values should be normalised in the shader; converting 0-256 to 0-1.
    */
    WGPUTextureFormat font_texture_format = WGPUTextureFormat_R8Unorm;
    BOUNDS_CHECK(write_index, manager->texture_arrays_length);
    renderer_texture_array_init(
        &manager->texture_arrays[write_index], device, arena, font_texture_format, 
        font_info.texture_width, font_info.texture_height, font_info.virtual_textures_length
    );

    // image textures.
    /*
        Image textures are an array of four bytes for each channel (red, green, blue, alpha) ranging from 0-256.
        These values should be normalised in the shader; converting 0-256 to 0-1.
    */
    write_index = VIRTUAL_TEXTURE_MANAGER_IMAGE_TEXTURE_ARRAY_START_INDEX;
    WGPUTextureFormat image_texture_format = WGPUTextureFormat_RGBA8Unorm;
    for(i32 i = 0; i < image_infos_length; i++){
        ImageTexturesInitInfo* create_info = &image_infos[i];
        BOUNDS_CHECK(write_index, manager->texture_arrays_length);
        renderer_texture_array_init(
            &manager->texture_arrays[write_index], device, arena, image_texture_format, create_info->width, create_info->height, create_info->max_textures
        );
        write_index++;
    }
    manager->is_init = true;
}

void renderer_sprite_manager_init(SpriteManager* manager, WGPUDevice device, MemoryArena* arena, SpriteLayerCreateInfo* layer_infos, i32 layer_infos_length){
    
    { // validation step.
        ASSERT(!manager->is_init, "attempted to init an already init sprite manager.");
        ASSERT(layer_infos, "layer_infos is NULL.");
    }

    i32 max_sprites = 0;
    for(i32 i = 0; i < layer_infos_length; i++){
        ASSERT(layer_infos[i].max_sprites > 0, "should not init sprite layer with less than 1 sprite; as a NIL element is required.");
        max_sprites += layer_infos[i].max_sprites;
    }

    ASSERT(max_sprites<DEVICE_SPRITE_MAX_AMOUNT, "attempted to init sprite manager with a sprite amount greater than the max device sprite amount.");
    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Storage;
    renderer_buffer_init(&manager->sprite_buffer, device, host_usage, device_usage, max_sprites, sizeof(DeviceSprite));

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->device_sprites, &manager->device_sprites_length, max_sprites);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->host_sprites, &manager->host_sprites_length, max_sprites);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->sprite_generations, &manager->sprite_generations_length, max_sprites);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->device_sprites_scratch_space, &manager->device_sprites_scratch_space_length, max_sprites);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->sprite_layers, &manager->sprite_layers_length, layer_infos_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->one_frame_sprites_stack, &manager->one_frame_sprites_stack_length, max_sprites);

    i32 free_index = 1; // exclude the nil.
    for(i32 i = 0; i < manager->sprite_layers_length; i++){
        SpriteLayer* layer = &manager->sprite_layers[i];

        BOUNDS_CHECK(i, layer_infos_length);
        SpriteLayerCreateInfo* create_info = &layer_infos[i];

        layer->max_sprites = create_info->max_sprites;
        MEMORY_ARENA_ALLOC_ARRAY(arena, layer->free_sprite_indices, &layer->free_sprite_indices_length, layer->max_sprites);

        // push the free indices.
        if(i == 0){
            i32 j = i == 0 ? 1 : 0; // exclude the Nil sprite.
            for(i32 j = 1; j < layer->max_sprites; j++){
                ARRAY_PUSH(layer->free_sprite_indices, layer->free_sprite_indices_length, &layer->free_sprite_indices_count, free_index);
                free_index++;
            }
        }

        manager->is_init = true;
    }
}

void renderer_buffer_map_async_callback(WGPUMapAsyncStatus status, WGPUStringView message, void* user_data_1, void* user_data_2){
    BufferMapAsyncCallbackCtx* ctx = (BufferMapAsyncCallbackCtx*)user_data_1;
    ctx->is_valid = (status == WGPUMapAsyncStatus_Success) ? true : false;
    ctx->request_ended = true;
}

bool renderer_write_to_buffer(RenderBuffer* dst, WGPUDevice device, void* src, u32 src_byte_size){

    if(src_byte_size > dst->length_in_bytes){
        ASSERT(0!=0, "attempted to write data to a buffer of insufficient size.");
        return false;
    }

    // handle count & lengths.
    dst->count_in_bytes = src_byte_size;

    // asynchronously map the staging buffer for writing.
    BufferMapAsyncCallbackCtx map_ctx = {0};
    WGPUBufferMapCallbackInfo map_ctx_info = {0};
    map_ctx_info.callback = renderer_buffer_map_async_callback;
    map_ctx_info.userdata1 = &map_ctx;
    wgpuBufferMapAsync(dst->host, WGPUMapMode_Write, 0, dst->length_in_bytes, map_ctx_info);
    while(map_ctx.request_ended == false){
        wgpuQueueSubmit(wgpuDeviceGetQueue(device), 0, NULL);        
        /*
            TODO(nich s)
            you may want to sleep the thread here in the case the thread of the cpu decides to melt lol.
        */
    }
    if(map_ctx.is_valid == false){
        return false;
    }

    // write to the mapped data.
    void* mapped = wgpuBufferGetMappedRange(dst->host, 0, dst->length_in_bytes);
    COPY_MEMORY(mapped, src, src_byte_size);

    // hand ownership back to the gpu.
    wgpuBufferUnmap(dst->host);

    // copy from host staging to device local buffer.
    WGPUCommandEncoderDescriptor cmd_encoder_desc = {0};
    WGPUCommandEncoder cmd_encoder = wgpuDeviceCreateCommandEncoder(device, &cmd_encoder_desc);
    wgpuCommandEncoderCopyBufferToBuffer(cmd_encoder, dst->host, 0, dst->device, 0, dst->count_in_bytes);
    WGPUCommandBufferDescriptor cmd_buffer_desc = {0};
    WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(cmd_encoder, &cmd_buffer_desc);
    wgpuQueueSubmit(wgpuDeviceGetQueue(device), 1, &cmd_buffer);
    
    return true;
}

/**
    `remarks`:
    `data_size_in_bytes` should be no greater than the renderer context's initialisation value; otherwise memory corruption will occur.
**/
bool renderer_write_to_user_uniform_buffer(RendererContext* ctx, void* buffer_data, u32 data_size_in_bytes){
    ASSERT(ctx->is_init, "renderer context has not been init.");
    return renderer_write_to_buffer(&ctx->user_uniform_buffer, ctx->device, buffer_data, data_size_in_bytes); 
}

void renderer_vertex_buffer_init(RenderBuffer* buffer, WGPUDevice device){
    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init an already init vertex buffer.");
    WGPUBufferUsage host_usage = WGPUBufferUsage_MapWrite | WGPUBufferUsage_CopySrc;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex;
    renderer_buffer_init(buffer, device, host_usage, device_usage, 4, sizeof(Vertex));

    Vertex* vertices = (Vertex[]){
        { // top left.
            .position = {.x = -0.5f, .y = 0.5f},
            .uv = {.x = 0.0f, .y = 0.0f}
        },
        { // top right.
            .position = {.x = 0.5f, .y = 0.5f},
            .uv = {.x = 1.0f, .y = 0.0f}
        },
        { // bottom right
            .position = {.x = 0.5f, .y = -0.5f},
            .uv = {.x = 1.0f, .y = 1.0f}
        },
        { // bottom left
            .position = {.x = -0.5f, .y = -0.5f},
            .uv = {.x = 0.0f, .y = 1.0f}
        }
    };
    renderer_write_to_buffer(buffer, device, (void*)vertices, sizeof(Vertex) * 4);
}

void renderer_index_buffer_init(RenderBuffer* buffer, MemoryArena* transient, WGPUDevice device, i32 max_sprites){

    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init index buffer with an already init buffer.");

    // round to the next multiple of four.
    i32 vertices_count = max_sprites * 4;
    i32 total_vertices = (vertices_count + 3) & ~3;
    // calculate the required amount of indices.
    i32 total_indices = total_vertices / 4 * 6;

    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index;
    renderer_buffer_init(buffer, device, host_usage, device_usage, total_indices, sizeof(u32));

    u32* indices;
    i32 indices_length;
    i32 indices_count = 0;
    MEMORY_ARENA_ALLOC_ARRAY(transient, indices, &indices_length, total_indices);

    for(int i = 0; i < total_vertices; i+=4){
        ARRAY_PUSH(indices, total_indices, &indices_count, i);
        ARRAY_PUSH(indices, total_indices, &indices_count, i+1);
        ARRAY_PUSH(indices, total_indices, &indices_count, i+2);
        ARRAY_PUSH(indices, total_indices, &indices_count, i+2);
        ARRAY_PUSH(indices, total_indices, &indices_count, i+3);
        ARRAY_PUSH(indices, total_indices, &indices_count, i);
    }

    renderer_write_to_buffer(buffer, device, indices, sizeof(u32) * total_indices);
}

void renderer_user_uniform_buffer_init(RenderBuffer* buffer, WGPUDevice device, u32 size_in_bytes){
    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init user-uniform-buffer with an already init buffer.");
    // ensure that the size is a multiple of 16; accounting for the 16 byte padding of WG.
    // uint adjustedSize = (sizeOfUbo + 15) & ~15u;
    WGPUBufferUsage host_usage = WGPUBufferUsage_MapWrite | WGPUBufferUsage_CopySrc;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform; 
    renderer_buffer_init(buffer, device, host_usage, device_usage, 1, size_in_bytes);
}

void renderer_user_storage_buffer_init(RenderBuffer* buffer, WGPUDevice device, u32 size_in_bytes){
    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init a user-defined-storage buffer with an aleady init buffer.");
    WGPUBufferUsage host_usage = WGPUBufferUsage_MapWrite | WGPUBufferUsage_CopySrc;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Storage;
    renderer_buffer_init(buffer, device, host_usage, device_usage, 1, size_in_bytes);
}

void renderer_window_surface_init(WindowSurface* surface, WGPUInstance instance, WGPUSurfaceDescriptor desc, u32 window_width, u32 window_height){
    ASSERT(surface->is_init == false, "attempted to init an already init window surface.");
    surface->is_init = true;
    surface->surface = wgpuInstanceCreateSurface(instance, &desc);
    surface->window_extents = (WGPUExtent3D){0};
    surface->window_extents.width = window_width;
    surface->window_extents.height = window_height;
    surface->window_extents.depthOrArrayLayers = 1;
}

void renderer_configure_window_surface(WindowSurface* surface, WGPUDevice device, WGPUAdapter device_adapter, u32 window_width, u32 window_height){
    
    { // validation.
        ASSERT(surface != NULL, "attempted to configure a nul window surface.");
        ASSERT(surface->is_init, "attempted to configure an uninitialised window surface.");
    }
    wgpuSurfaceUnconfigure(surface->surface);
    
    // get the preferred format of the window surface.
    WGPUSurfaceCapabilities capabilities = {0};
    wgpuSurfaceGetCapabilities(surface->surface, device_adapter, &capabilities);
    WGPUTextureFormat preferredFormat = capabilities.formats[0]; // 0 is guaranteed to be the preferred/optimal format.

    // configuration.
    WGPUSurfaceConfiguration config = {0};
    config.width = window_width;
    config.height = window_height;
    config.format = preferredFormat;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.device = device;
    /*
        Specify the swap chain behaviour:

        Immediate:
            No off-screen texture is used, the render process directly draws on the surface, which might lead
            to artifacts (e.g. tearing) but has zero latency. 
        Mailbox:
            There is only one slot in the queue, and when a new frame is rendered, it replacees the one currently
            waiting (whch is discarded without ever being presented).
        Fifo:
            Stands for "first-in, fisrt-out", meaning that the presented texture is always the oldest one like a
            regular queue. No rendered texture is wasted. 
    */
    config.presentMode = WGPUPresentMode_Immediate;
    config.alphaMode  = WGPUCompositeAlphaMode_Auto;
    surface->window_extents.width = window_width;
    surface->window_extents.height = window_height;
    wgpuSurfaceConfigure(surface->surface, &config);

    // clean-up.
    wgpuSurfaceCapabilitiesFreeMembers(capabilities);
}

void renderer_link_to_window(RendererContext* ctx, WindowContext window_ctx, u32 window_width, u32 window_height){
    if(ctx->window_surface.is_init){
        wgpuSurfaceRelease(ctx->window_surface.surface);
    }

    WGPUSurfaceDescriptor surface_desc = {0};
    WGPUSurfaceSourceWindowsHWND win32_desc = {0};
#if OS_WINDOWS
    win32_desc.chain = (WGPUChainedStruct){.sType = WGPUSType_SurfaceSourceWindowsHWND};
    win32_desc.hinstance = window_ctx.win32_hinstance;
    win32_desc.hwnd = window_ctx.win32_hwnd;
    surface_desc.nextInChain = (WGPUChainedStruct*)&win32_desc;
#else
#   error OS not set up.
#endif
    // initialise render specifics.
    renderer_window_surface_init(&ctx->window_surface, ctx->instance, surface_desc, window_width, window_height);    
    renderer_configure_window_surface(&ctx->window_surface, ctx->device, ctx->adapter, window_width, window_height);
}

void renderer_texture_init(Texture* texture, WGPUDevice device, WGPUTextureFormat format, WGPUTextureUsage usage, WGPUTextureAspect aspect, u32 width, u32 height){
    ASSERT(texture->is_init == false, "attempted to init an already init texture.");
    texture->is_init = true;

    // texture creation.
    WGPUTextureDescriptor text_desc = {0};
    text_desc.dimension = WGPUTextureDimension_2D;
    text_desc.format = format;
    text_desc.mipLevelCount = 1;
    text_desc.sampleCount = 1;
    text_desc.size.width = width;
    text_desc.size.height = height;
    text_desc.size.depthOrArrayLayers = 1;
    text_desc.usage = usage;
    text_desc.viewFormatCount = 1;
    text_desc.viewFormats = &format;
    texture->extents = text_desc.size;
    texture->ptr = wgpuDeviceCreateTexture(device, &text_desc);

    // create the view.
    WGPUTextureViewDescriptor view_desc = {0};
    view_desc.aspect = aspect;
    view_desc.baseArrayLayer = 0;
    view_desc.arrayLayerCount = 1;
    view_desc.baseMipLevel = 0;
    view_desc.mipLevelCount = 1;
    view_desc.dimension = WGPUTextureViewDimension_2D;
    view_desc.format = format;
    texture->view = wgpuTextureCreateView(texture->ptr, &view_desc);
}

void renderer_depth_texture_init(Texture* texture, WGPUDevice device, u32 width, u32 height){
    WGPUTextureFormat format = WGPUTextureFormat_Depth24Plus;
    WGPUTextureUsage usage = WGPUTextureUsage_RenderAttachment;
    WGPUTextureAspect aspect = WGPUTextureAspect_DepthOnly; 
    renderer_texture_init(texture, device, format, usage, aspect, width, height);
}

void renderer_free_texture(Texture* texture){
    wgpuTextureViewRelease(texture->view);
    wgpuTextureDestroy(texture->ptr);
    wgpuTextureRelease(texture->ptr);
    texture->is_init = false;
}

void renderer_final_render_target_init(Texture* texture, WGPUDevice device, u32 width, u32 height){
    WGPUTextureFormat format = WGPUTextureFormat_RGBA8UnormSrgb; // this used to be WGPUTextureFormat_BGRA8UnormSrgb.
    WGPUTextureUsage usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding;
    WGPUTextureAspect aspect = WGPUTextureAspect_All;
    renderer_texture_init(texture, device, format, usage, aspect, width, height);
}

void renderer_sampler_init(WGPUSampler* sampler, WGPUDevice device){
    WGPUSamplerDescriptor desc = {0};
    /*
        the addressing mode can be specified per axis; note that the axes are called 
        {U,V,W} instead of {X,Y,Z}.
    */
    desc.addressModeU = WGPUAddressMode_Repeat;
    desc.addressModeV = WGPUAddressMode_Repeat;
    desc.addressModeW = WGPUAddressMode_Repeat;
    /*
        The mag and min filter specify how to interpolate texels that are magnified or minified.
        The magnigation concerns oversampling problems while minification concerns undersampling.
    */
    desc.magFilter = WGPUFilterMode_Nearest;
    desc.minFilter = WGPUFilterMode_Nearest;
    desc.lodMinClamp = 1.0f;
    desc.lodMaxClamp = 1.0f;
    desc.compare = WGPUCompareFunction_Undefined;
    desc.maxAnisotropy = 1;

    *sampler = wgpuDeviceCreateSampler(device, & desc);
}

void renderer_shader_module_init(WGPUShaderModule* module, WGPUDevice device, String shader_code){
    WGPUShaderSourceWGSL wgsl = {0};
    wgsl.code = (WGPUStringView){.data = shader_code.chars, .length = shader_code.length};
    wgsl.chain.sType = WGPUSType_ShaderSourceWGSL;
    WGPUShaderModuleDescriptor desc = (WGPUShaderModuleDescriptor){.nextInChain = (WGPUChainedStruct*)&wgsl};
    *module = wgpuDeviceCreateShaderModule(device, &desc);
}

WGPUShaderModule renderer_shader_module_load_from_file(String file_path, WGPUDevice device){
    size_t buffer_size;
    void* buffer = platform_load_file(file_path,&buffer_size);
    WGPUShaderSourceWGSL wgsl = {.chain.sType = WGPUSType_ShaderSourceWGSL, .code = (WGPUStringView){.data = buffer, .length = WGPU_STRLEN}};
    WGPUShaderModuleDescriptor desc = {.nextInChain = &wgsl.chain};
    WGPUShaderModule shader_module = wgpuDeviceCreateShaderModule(device, &desc);
    platform_free_memory(buffer);
    return shader_module;
}

void renderer_blit_pipeline_init(BlitPipeline* pipeline, WindowSurface window_surface, WGPUDevice device, WGPUAdapter device_adapter){
    { // validation.
        ASSERT(pipeline->is_init == false, "attempted to init an already init graphics pipeline.");
    }

    renderer_sampler_init(&pipeline->sampler, device);

    WGPUShaderModule shader_module = {0};

    // shader creation.
    renderer_shader_module_init(&shader_module, device, (String){.chars = BLIT_SHADER_CODE, .length = WGPU_STRLEN});

    /*
        primitive state.
    */
    WGPUPrimitiveState primitive_state = {
        // Must be WebGPU.PrimitiveTopology.TriangleStrip for the 4-vertex quad setup to map correctly.
        .topology = WGPUPrimitiveTopology_TriangleStrip,
        .cullMode = WGPUCullMode_None
    };

    /*
        multisample state.
    */
    WGPUMultisampleState multisample_state = {
        .count = 1,
        .mask = U32_MAX,
        .alphaToCoverageEnabled = WGPU_FALSE
    };

    /*
        vertex state.
    */
    WGPUVertexState vertext_state = {    
        .module = shader_module,
        .entryPoint = (WGPUStringView){.data = VERTEX_SHADER_ENTRY_POINT, .length = WGPU_STRLEN},
        .bufferCount = 0,
        .buffers = NULL
    };

    /*
        fragment state.
    */
    WGPUBlendState blend_state = {
        // turn off any form of blending as the blit should not carry over any of the previous frames colours.
        .color.srcFactor = WGPUBlendFactor_One, 
        .color.operation = WGPUBlendOperation_Add,
        .color.dstFactor = WGPUBlendFactor_Zero,
        .alpha.srcFactor = WGPUBlendFactor_One,
        .alpha.operation = WGPUBlendOperation_Add,
        .alpha.dstFactor = WGPUBlendFactor_Zero
    };
    WGPUSurfaceCapabilities capabilities = {0};
    wgpuSurfaceGetCapabilities(window_surface.surface, device_adapter, &capabilities);
    WGPUTextureFormat preferred_format = capabilities.formats[0]; // 0 is guaranteed to be the preferred/optimal format.
    WGPUColorTargetState colour_target_state = {    
        .format = preferred_format,
        .blend = &blend_state,
        .writeMask = WGPUColorWriteMask_All
    };
    WGPUFragmentState fragment_state = {
        .module = shader_module,
        .entryPoint = (WGPUStringView){.data = FRAGMENT_SHADER_ENTRY_POINT, .length = WGPU_STRLEN},
        .targetCount = 1,
        .targets = &colour_target_state
    };

    /*
        layout.
    */
        /*
            bind group.
        */
        WGPUBindGroupLayoutEntry* group_layout_entries = (WGPUBindGroupLayoutEntry[BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT]){};
        
        BOUNDS_CHECK(BLIT_PIPELINE_SAMPLER_BINDING, BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT);
        WGPUBindGroupLayoutEntry* sampler_layout_entry = &group_layout_entries[BLIT_PIPELINE_SAMPLER_BINDING];
        *sampler_layout_entry = (WGPUBindGroupLayoutEntry){
            .binding = BLIT_PIPELINE_SAMPLER_BINDING,
            .visibility = WGPUShaderStage_Fragment,
            .sampler.type = WGPUSamplerBindingType_NonFiltering
        };
        
        BOUNDS_CHECK(BLIT_PIPELINE_TEXTURE_BINDING, BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT);
        WGPUBindGroupLayoutEntry* texture_layout_entry = &group_layout_entries[BLIT_PIPELINE_TEXTURE_BINDING];
        *texture_layout_entry = (WGPUBindGroupLayoutEntry){
            .binding = BLIT_PIPELINE_TEXTURE_BINDING,
            .visibility = WGPUShaderStage_Fragment,
            .texture.sampleType = WGPUTextureSampleType_Float,
            .texture.viewDimension = WGPUTextureViewDimension_2D
        };
        
        WGPUBindGroupLayoutDescriptor group_layout_desc = {
            .entryCount = BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT,
            .entries = group_layout_entries
        };
        
        pipeline->bind_group_layout = wgpuDeviceCreateBindGroupLayout(device, &group_layout_desc);
        
        WGPUBindGroupLayout* layouts = (WGPUBindGroupLayout[1]){pipeline->bind_group_layout};
        WGPUPipelineLayoutDescriptor pipeline_layout_desc = {0};
        pipeline_layout_desc.bindGroupLayoutCount = 1;
        pipeline_layout_desc.bindGroupLayouts = layouts;
        WGPUPipelineLayout pipeline_layout = wgpuDeviceCreatePipelineLayout(device, &pipeline_layout_desc);

        /*
            descriptor sets.
        */
            /*
                bind group
            */
            /**
                note that the binding groups are not setup here at all as they should be dynamically created during the render pass,
                This is because the swap chain texture which is being blitted constantly changes (as per the nature of swap chain structures).
            **/

        /*
            pipeline creation.
        */
        WGPURenderPipelineDescriptor pipeline_desc = {0};
        pipeline_desc.layout = pipeline_layout;
        pipeline_desc.primitive = primitive_state;
        pipeline_desc.multisample = multisample_state;
        pipeline_desc.fragment = &fragment_state;
        pipeline_desc.vertex = vertext_state;
        pipeline_desc.depthStencil = NULL;
        pipeline->render_pipeline = wgpuDeviceCreateRenderPipeline(device, &pipeline_desc);
        pipeline->is_init = true;
    
    /*
        cleanup.
    */
    wgpuSurfaceCapabilitiesFreeMembers(capabilities);
    wgpuPipelineLayoutRelease(pipeline_layout);
    wgpuShaderModuleRelease(shader_module);
}

/*  
    parameters:

    `texture_arrays_count`: the total amount of texture arrays to store. 
*/  
void renderer_graphics_pipeline_init(
    GraphicsPipeline* pipeline, MemoryArena* transient, WGPUDevice device, WGPUAdapter device_adapter, 
    WindowSurface window_surface, VirtualTextureManager virtual_texture_manager, Texture final_render_texture, 
    String shader_file_path, 
    RenderBuffer user_uniform_buffer, RenderBuffer user_storage_buffer, RenderBuffer sprite_storage_buffer,
    u32 user_uniform_buffer_size_in_bytes, u32 user_storage_buffer_size_in_bytes
){
    
    { // validation.
        ASSERT(pipeline->is_init==false, "attempted to init an already init graphics pipeline.");
    }

    i32 texture_arrays_count = virtual_texture_manager.texture_arrays_length;

    renderer_sampler_init(&pipeline->non_filter_sampler, device);

    /**
        TODO: (nich s) 
        Load Shader Module.
    **/
    WGPUShaderModule shader_module = renderer_shader_module_load_from_file(shader_file_path, device);
    
    WGPURenderPipelineDescriptor pipeline_desc = {0};
    pipeline_desc.vertex.module = shader_module;
    pipeline_desc.vertex.bufferCount = 0;
    pipeline_desc.vertex.entryPoint = (WGPUStringView){.data = VERTEX_SHADER_ENTRY_POINT, .length = WGPU_STRLEN};
    pipeline_desc.vertex.constantCount = 0;
    pipeline_desc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    // Specify the order of vertices that should be connected; when not specified like so: vertices are considered sequentially.
    pipeline_desc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    // clock wise.
    pipeline_desc.primitive.frontFace = WGPUFrontFace_CW;
    pipeline_desc.primitive.cullMode = WGPUCullMode_None;

    /*
        fragment state.
    */
    WGPUBlendState blend_state = {
        /** 
            The blending equation can be set independently for the rgb channels and the alpha channel, in general, it takes the following form:

                rgb = Color.SrcFactor * srcRgb [Color.Operation] Color.DstFactor * dstRgb;

            the usual blending equation is configures as:

                rgb = srcAlpha * srcRgb + (1 - srcAlpha) * dstRgb;

            corresponding to the intuition of "layering" rendered fragments over the existing pixel's value.
        */
        .color.srcFactor = WGPUBlendFactor_SrcAlpha,
        .color.operation = WGPUBlendOperation_Add,
        .color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha,
        /*
            There is a similar blending equation for the alpha channel:

                alpha = Alpha.SrcFactor * srcAlpha [Alpha.Operation] Alpha.DstFactor * dstAlpha

            the target alpha should stay untouched:

                alpha = dstAlpha = 0 * srcAlpha + 1 * dstAlpha;
        */
        .alpha.srcFactor = WGPUBlendFactor_SrcAlpha,
        .alpha.operation = WGPUBlendOperation_Add,
        .alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha
    };
    WGPUSurfaceCapabilities capabilities = {0};
    wgpuSurfaceGetCapabilities(window_surface.surface, device_adapter, &capabilities);
    WGPUTextureFormat preferred_format = capabilities.formats[0]; // 0 is guaranteed to be the preferred/optimal format.
    WGPUColorTargetState colour_target_state = {
        .format = preferred_format,
        .blend = &blend_state,
        .writeMask = WGPUColorWriteMask_All,
    };
    WGPUFragmentState frag_state = {
        .module = shader_module,
        .entryPoint = (WGPUStringView){.data = FRAGMENT_SHADER_ENTRY_POINT, .length = WGPU_STRLEN},
        .constantCount = 0,
        .targetCount = 1,
        .targets = &colour_target_state
    };
    pipeline_desc.fragment = &frag_state;

    /*
        depth state.
    */
    WGPUStencilFaceState stencil_face_state = {
        .compare = WGPUCompareFunction_Less,
        .depthFailOp = WGPUStencilOperation_Keep,
        .failOp = WGPUStencilOperation_Keep,
        .passOp = WGPUStencilOperation_Replace,
    };
    WGPUDepthStencilState depth_stencil_state = {
        .depthCompare = WGPUCompareFunction_Less,
        .depthWriteEnabled = WGPUOptionalBool_True,
        .format = WGPUTextureFormat_Depth24Plus,
        .stencilFront = stencil_face_state,
        .stencilBack = stencil_face_state,
        .stencilReadMask = 0,
        .stencilWriteMask = 0,
        .depthWriteEnabled = WGPUOptionalBool_False,
    };
    pipeline_desc.depthStencil = &depth_stencil_state;

    /*
        multisample state.
    */
    // Multi-sampling/Anti-aliasing is off for now with the set values.
    pipeline_desc.multisample.count = 1u;
    pipeline_desc.multisample.mask = ~0u;
    pipeline_desc.multisample.alphaToCoverageEnabled = WGPU_FALSE;

    /*
        layouts.
    */
    /*
        Layouts define the way a resource (buffer/texture) is accessed by the driver, where as a descriptor set (a descriptor in WebGPU)
        defines the actual data that is accessed; this enables the driver to perform optimisations and validation checks ahead of time.
    */
    // create the pipeline layout.
    WGPUPipelineLayoutDescriptor layout_desc = {0};
        /*
            vertex buffer layout.
        */
        /*
            For the 'vertex fetch' stage to transform this raw data from the vertex buffer 
            into what the vertex shader expects, we need to specify a layout.
        */
        i32 vertex_attribute_count = 2;
        WGPUVertexAttribute* vertex_atts = (WGPUVertexAttribute[]){
            { // position attribute.
                .shaderLocation = SHADER_VERTEX_LOCATION_POSITION,
                .format = WGPUVertexFormat_Float32x3, // vector3 = WGPUVertexFormat_Float32x3.
                .offset = offsetof(Vertex, position)
            },
            { // uv attribute.
                .shaderLocation = SHADER_VERTEX_LOCATION_UV,
                .format = WGPUVertexFormat_Float32x2, // vector2 = WGPUVertexFormat_Float32x2.
                .offset = offsetof(Vertex, uv)
            }
        };
        WGPUVertexBufferLayout vertex_buffer_layout = {
            /**
                The stride designates the number of bytes between two consecutive elements that form a vertex; in out case, the positions are
                contiguous so the stride is equal to the size of a vector2. This should only change when adding more interleaved attributes.
            **/
            .arrayStride = (u32)sizeof(Vertex),
            // vertexBufferLayout.ArrayStride = (uint)sizeof(Vertex);
            /**
                StepMode = Vertex:
                    each entry in the buffer corresponds to a different vertex.

                StepMode = Instance:
                    each entry is shared by all vertices of the same instance (i.e, copy) of the shape.
            **/
            .stepMode = WGPUVertexStepMode_Vertex,
            .attributeCount = (u32)vertex_attribute_count,
            .attributes = vertex_atts
        };
    pipeline_desc.vertex.bufferCount = 1;
    pipeline_desc.vertex.buffers = &vertex_buffer_layout;

    /*
        Bindings
    */
        /*
            Layouts.
        */
            
            /*
                Group 0 (buffers).
            */
            WGPUBindGroupLayoutEntry* gle0;
            i32 gle0_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, gle0, &gle0_length, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            
            // user-defined uniform.
            BOUNDS_CHECK(SHADER_BINDING_USER_UNIFORM, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            gle0[SHADER_BINDING_USER_UNIFORM] = (WGPUBindGroupLayoutEntry){
                .buffer.type = WGPUBufferBindingType_Uniform,
                .buffer.minBindingSize = 0,
                .binding = SHADER_BINDING_USER_UNIFORM,
                .visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment
            }; 
            
            // user-defined storage.
            BOUNDS_CHECK(SHADER_BINDING_USER_STORAGE, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            gle0[SHADER_BINDING_USER_STORAGE] = (WGPUBindGroupLayoutEntry){
                .buffer.type = WGPUBufferBindingType_ReadOnlyStorage,
                .buffer.minBindingSize = 0,
                .binding = SHADER_BINDING_USER_STORAGE,
                .visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment
            };

            // virtual textures uniform.
            BOUNDS_CHECK(SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            gle0[SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM] = (WGPUBindGroupLayoutEntry){
                .buffer.type = WGPUBufferBindingType_Uniform,
                .buffer.minBindingSize = 0,
                .binding = SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM,
                .visibility = WGPUShaderStage_Fragment
            };

            // sprite storage.
            BOUNDS_CHECK(SHADER_BINDING_SPRITE_STORAGE, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            gle0[SHADER_BINDING_SPRITE_STORAGE] = (WGPUBindGroupLayoutEntry){
                .buffer.type = WGPUBufferBindingType_ReadOnlyStorage,
                .buffer.minBindingSize = 0,
                .binding = SHADER_BINDING_SPRITE_STORAGE,
                .visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment
            };
            
            // create bind group layout.
            WGPUBindGroupLayoutDescriptor gld0 = (WGPUBindGroupLayoutDescriptor){
                .entryCount = SHADER_BUFFERS_GROUP_BINDING_COUNT,
                .entries = gle0,
            };
            pipeline->bind_group_layout_0 = wgpuDeviceCreateBindGroupLayout(device, &gld0);
            
            /*  
                Group 1 (texture arrays)
            */  
            WGPUBindGroupLayoutEntry* gle1;
            i32 gle1_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, gle1, &gle1_length, texture_arrays_count);
            // define the texture array entries.
            for(i32 i = 0; i < gle1_length; i++){
                gle1[i] = (WGPUBindGroupLayoutEntry){
                    .binding = i,
                    .visibility = WGPUShaderStage_Fragment,
                    .texture.sampleType = WGPUTextureSampleType_Float,
                    .texture.viewDimension = WGPUTextureViewDimension_2DArray 
                };
            } 
            // bind group layout creation.
            WGPUBindGroupLayoutDescriptor gld1 = (WGPUBindGroupLayoutDescriptor){
                .entryCount = texture_arrays_count,
                .entries = gle1
            };
            pipeline->bind_group_layout_1 = wgpuDeviceCreateBindGroupLayout(device, &gld1);
            
            /*  
                Group 2 (utilities)
            */  
            WGPUBindGroupLayoutEntry* gle2;
            i32 gle2_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, gle2, &gle2_length, SHADER_UTILITIES_GROUP_BINDING_COUNT);
            
            // define the entries
            BOUNDS_CHECK(SHADER_BINDING_NON_FILTER_SAMPLER, SHADER_UTILITIES_GROUP_BINDING_COUNT);
            gle2[SHADER_BINDING_NON_FILTER_SAMPLER] = (WGPUBindGroupLayoutEntry){
                .binding = SHADER_BINDING_NON_FILTER_SAMPLER,
                .visibility = WGPUShaderStage_Fragment,
                // non filtering as we dont use bilinear interpolation or whater; nearest is best for pixel art.
                .sampler.type = WGPUSamplerBindingType_NonFiltering
            };
            
            // create the group layout.
            WGPUBindGroupLayoutDescriptor gld2 = {
                .entryCount = gle2_length,
                .entries = gle2,
            };
            pipeline->bind_group_layout_2 = wgpuDeviceCreateBindGroupLayout(device, &gld2);
        /*
            set the layouts; note that ordering matters here, descriptor sets cannot be made without their 
            respective layouts not being binded to the descriptor before hand. 
        */
        layout_desc.bindGroupLayoutCount = 3;
        layout_desc.bindGroupLayouts = (WGPUBindGroupLayout[]){pipeline->bind_group_layout_0, pipeline->bind_group_layout_1, pipeline->bind_group_layout_2};

        /*  
            Descriptor Sets.
        */  
            /*  
                Group 0
            */  
            WGPUBindGroupEntry* ge0;
            i32 ge0_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, ge0, &ge0_length, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            
            // user-uniform.
            BOUNDS_CHECK(SHADER_BINDING_USER_UNIFORM, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            ge0[SHADER_BINDING_USER_UNIFORM] = (WGPUBindGroupEntry){
                .binding = SHADER_BINDING_USER_UNIFORM,
                .buffer = user_uniform_buffer.device,
                .size = user_uniform_buffer_size_in_bytes
            };
            
            // user-storage
            BOUNDS_CHECK(SHADER_BINDING_USER_STORAGE, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            ge0[SHADER_BINDING_USER_STORAGE] = (WGPUBindGroupEntry){
                .binding = SHADER_BINDING_USER_STORAGE,
                .buffer = user_storage_buffer.device,
                .size = user_storage_buffer_size_in_bytes
            };
            
            // virtual textures uniform.
            BOUNDS_CHECK(SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            ge0[SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM] = (WGPUBindGroupEntry){
                .binding = SHADER_BINDING_VIRTUAL_TEXTURES_UNIFORM,
                .buffer = virtual_texture_manager.device_virtual_texture_buffer.device,
                .size = virtual_texture_manager.device_virtual_texture_buffer.length_in_bytes
            };
            
            // sprite storage.
            BOUNDS_CHECK(SHADER_BINDING_SPRITE_STORAGE, SHADER_BUFFERS_GROUP_BINDING_COUNT);
            ge0[SHADER_BINDING_SPRITE_STORAGE] = (WGPUBindGroupEntry){
                .binding = SHADER_BINDING_SPRITE_STORAGE,
                .buffer = sprite_storage_buffer.device,
                .size = sprite_storage_buffer.length_in_bytes
            };
            
            // bind group creation.
            WGPUBindGroupDescriptor gd0 = {
                .layout = pipeline->bind_group_layout_0,
                .entryCount = SHADER_BUFFERS_GROUP_BINDING_COUNT,
                .entries = ge0,
            };
            pipeline->bind_group_0 = wgpuDeviceCreateBindGroup(device, &gd0);

            /*  
                Group 1.
            */  
            // texture array entries.
            WGPUBindGroupEntry* ge1;
            i32 ge1_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, ge1, &ge1_length, texture_arrays_count);
            for(i32 i = 0; i < texture_arrays_count; i++){
                BOUNDS_CHECK(i, virtual_texture_manager.texture_arrays_length);
                ge1[i] = (WGPUBindGroupEntry){
                    .binding = i,
                    .textureView = virtual_texture_manager.texture_arrays[i].view
                };
            }

            // bind group creation.
            WGPUBindGroupDescriptor gd1 = {
                .layout = pipeline->bind_group_layout_1,
                .entryCount = texture_arrays_count,
                .entries = ge1
            };
            pipeline->bind_group_1 = wgpuDeviceCreateBindGroup(device, &gd1);

            /*  
                Group 2.
            */  
            // sampler entry 
            WGPUBindGroupEntry* ge2;
            i32 ge2_length;
            MEMORY_ARENA_ALLOC_ARRAY(transient, ge2, &ge2_length, SHADER_UTILITIES_GROUP_BINDING_COUNT);
            
            // non-filter sampler.
            BOUNDS_CHECK(SHADER_BINDING_NON_FILTER_SAMPLER, SHADER_UTILITIES_GROUP_BINDING_COUNT);
            ge2[SHADER_BINDING_NON_FILTER_SAMPLER] = (WGPUBindGroupEntry){
                .binding = SHADER_BINDING_NON_FILTER_SAMPLER,
                .sampler = pipeline->non_filter_sampler,
            };
            
            // bind group creation.
            WGPUBindGroupDescriptor gd2 = {
                .layout = pipeline->bind_group_layout_2,
                .entryCount = SHADER_UTILITIES_GROUP_BINDING_COUNT,
                .entries = ge2
            };
            pipeline->bind_group_2 = wgpuDeviceCreateBindGroup(device, &gd2);
    
    // create the pipeline layout.
    pipeline->pipeline_layout = wgpuDeviceCreatePipelineLayout(device, &layout_desc);
    pipeline_desc.layout = pipeline->pipeline_layout;

    /*  
        Creation.
    */  
    pipeline->render_pipeline = wgpuDeviceCreateRenderPipeline(device, &pipeline_desc);
    pipeline->is_init = true;

    /*  
        cleanup.
    */  
    wgpuShaderModuleRelease(shader_module);
}

DestinationRectangle renderer_calculate_destination_rectangle(u32 src_width, u32 src_height, u32 dst_width, u32 dst_height){
    f32 back_buffer_aspect_ratio = (f32)dst_width / (f32)dst_height;
    f32 render_target_aspect_ratio = (f32)src_width / (f32)src_height;
    // scale the image to fit into the window'sback buffer.
    DestinationRectangle rect = {.width = dst_width, .height = dst_height};
    // stretch image (render target) width to fit on the window's back buffer.
    if(back_buffer_aspect_ratio > render_target_aspect_ratio){
        rect.width = (u32)((f32)rect.height * render_target_aspect_ratio);
        rect.x = ((float)dst_width - rect.width) * 0.5f;
    }
    // shrink image (render target) height to fit on the window's back buffer.
    else if (back_buffer_aspect_ratio < render_target_aspect_ratio){
        rect.height = (u32)((f32)rect.width / render_target_aspect_ratio);
        rect.y = ((float)dst_height - rect.height) * 0.5f;
    }
    return rect;
}

void renderer_update_render_destination_rectangle(RendererContext* ctx){
    ASSERT(ctx->is_init, "attempted to mutate an unintialised renderer context.");
    ctx->destination_rectangle = renderer_calculate_destination_rectangle(
        ctx->final_render_texture.extents.width, ctx->final_render_texture.extents.height,
        ctx->window_surface.window_extents.width, ctx->window_surface.window_extents.height
    );
}

SurfaceTexture renderer_get_next_spawn_chain_image_view(RendererContext* ctx){
    ASSERT(ctx->is_init, "attempted to use a non initialised rendering context.");

    /*  
        Get the texture to draw onto; Note that the 'surface texture' is not really an object, 
        rather a container for the multiple things that this function returns.
    */  
    SurfaceTexture surface_texture = {0};
    wgpuSurfaceGetCurrentTexture(ctx->window_surface.surface, &surface_texture.ptr);
    if(surface_texture.ptr.status != WGPUSurfaceGetCurrentTextureStatus_SuccessOptimal && surface_texture.ptr.status != WGPUSurfaceGetCurrentTextureStatus_SuccessSuboptimal){
        ASSERT(false, "Failed to get next swapchain surface texture.");
        return (SurfaceTexture){0};
    }
    /*
        Wrap up the raw surface texture data into a texture view.
    */
    WGPUTextureViewDescriptor desc = {
        .format = wgpuTextureGetFormat(surface_texture.ptr.texture),
        .dimension = WGPUTextureViewDimension_2D,
        // swap chain images dont use mipmaps.
        .baseMipLevel = 0,
        .mipLevelCount = 1,
        .baseArrayLayer = 0,
        .arrayLayerCount = 1,
        .aspect = WGPUTextureAspect_All, // equivalent to vulkan image aspect.
    };
    surface_texture.view = wgpuTextureCreateView(surface_texture.ptr.texture, &desc);
    surface_texture.extents = ctx->window_surface.window_extents;
    return surface_texture;
}

void renderer_free_surface_texture(SurfaceTexture* texture){
    wgpuTextureRelease(texture->ptr.texture);
    wgpuTextureViewRelease(texture->view);
}

void renderer_dealloc_sprite_unsafe(RendererContext* ctx, i32 sprite_index, i32 sprite_layer){
    
    SpriteManager* manager = &ctx->sprite_manager;
    
    BOUNDS_CHECK(sprite_layer, manager->sprite_layers_length);
    SpriteLayer* layer = &manager->sprite_layers[sprite_layer];

    BOUNDS_CHECK(sprite_index, manager->host_sprites_length);
    manager->host_sprites[sprite_index] = (HostSprite){0};

    BOUNDS_CHECK(sprite_index, manager->device_sprites_length);
    manager->device_sprites[sprite_index].state = SpriteState_Deallocated;
    
    ARRAY_PUSH(layer->free_sprite_indices, layer->free_sprite_indices_length, &layer->free_sprite_indices_count, sprite_index);
}

SpriteId renderer_sprite_alloc(RendererContext* ctx, i32 layer_index, bool* out_success){

    ASSERT(ctx->sprite_manager.is_init == true, "sprite manager has not been init");
    SpriteManager* sprite_manager = &ctx->sprite_manager;

    BOUNDS_CHECK(layer_index, sprite_manager->sprite_layers_length);
    SpriteLayer* layer = &sprite_manager->sprite_layers[layer_index];
    if(layer->free_sprite_indices_count == 0){
        ASSERT(0!=0, "memory limit it; cannot allocate more sprites.");
        *out_success = false;
        return (SpriteId){0};
    }

    i32 sprite_index = 0;
    ARRAY_POP(layer->free_sprite_indices, layer->free_sprite_indices_length, &layer->free_sprite_indices_count, &sprite_index);
    
    BOUNDS_CHECK(sprite_index, ctx->sprite_manager.device_sprites_length);
    DeviceSprite* sprite = &ctx->sprite_manager.device_sprites[sprite_index];
    sprite->state = SpriteState_Inactive;
    sprite->layer = layer_index;
    
    BOUNDS_CHECK(sprite_index, ctx->sprite_manager.sprite_generations_length);
    GenId gen_id = gen_id_make(sprite_index, ctx->sprite_manager.sprite_generations[sprite_index]);
    
    *out_success = true;
    return (SpriteId){.gen_id = gen_id, .layer = layer_index};
}

bool renderer_sprite_init(
    RendererContext* ctx, SpriteId sprite_id, Matrix4x4 transform, Colour colour, SpriteRegion region, ColourState colour_state,
    i32 virtual_texture, i32 material, bool is_active    
){
    SpriteManager* sprite_manager = &ctx->sprite_manager;
    i32 index = gen_id_get_index(sprite_id.gen_id);
    i32 gen = gen_id_get_generation(sprite_id.gen_id);

    ASSERT(sprite_manager->is_init == true, "sprite manager has not been init.");
    ASSERT(index > 0, "invalid sprite id.");
    ASSERT(virtual_texture > 0, "invalid virtual texture index.");
    ASSERT(material > 0, "invalid material index.");

    BOUNDS_CHECK(index, sprite_manager->sprite_generations_length);
    if(gen != ctx->sprite_manager.sprite_generations[index]){
        return false;
    }

    renderer_sprite_set_transform_unsafe(ctx, index, transform);
    renderer_sprite_set_material_unsafe(ctx, index, material);
    renderer_sprite_set_region_unsafe(ctx, index, region);
    renderer_sprite_set_virtual_texture_unsafe(ctx, index, virtual_texture);
    renderer_sprite_set_state_unsafe(ctx, index, is_active ? SpriteState_Active : SpriteState_Inactive);
    renderer_sprite_set_colour_unsafe(ctx, index, colour);
    renderer_sprite_set_colour_state_unsafe(ctx, index, colour_state);

    return true;
}

bool renderer_dealloc_sprite(RendererContext* ctx, SpriteId sprite_id){
    
    // validation.
    ASSERT(ctx->is_init == true, "renderer context is not init.");
    ASSERT(ctx->sprite_manager.is_init == true, "renderer sprite manager has not been init.");

    SpriteManager* manager = &ctx->sprite_manager;
    i32 generation = gen_id_get_generation(sprite_id.gen_id);
    i32 index = gen_id_get_index(sprite_id.gen_id);

    // validation.
    if(index <= 0){
        ASSERT(0!=0, "attempted to deallocate an invalid or the Nil sprite.");
        return false;
    }
    if(generation != manager->sprite_generations[index]){
        ASSERT(0!=0, "attempted to deallocate a sprite with a stale index.");
        return false;
    }

    DeviceSprite* sprite = &manager->device_sprites[index];
    if(sprite->state == SpriteState_Deallocated){
        ASSERT(0!=0, "attempted to deallocate a sprite that has already been deallocated.");
        return false;
    }

    // DeallocSpriteUnsafe(ref manager, index, spriteId.Layer);
    return true;

}

bool renderer_sprite_string_init(
    RendererContext* ctx, SpriteId sprite_id, String text, Matrix4x4 transform, i32 virtual_texture_index, 
    i32 material_index, bool is_active
){
    i32 first_index = gen_id_get_index(sprite_id.gen_id);
    i32 generation = gen_id_get_generation(sprite_id.gen_id);

    BOUNDS_CHECK(virtual_texture_index, ctx->virtual_texture_manager.host_virtual_textures_length);
    HostVirtualTexture* vt = &ctx->virtual_texture_manager.host_virtual_textures[virtual_texture_index];

    
    { // validation.
        ASSERT(ctx->sprite_manager.is_init == false, "sprite manager isnt init.");
        ASSERT(material_index > 0, "invalid material index.");
        if(vt->texture_type != VirtualTextureType_Font){
            ASSERT(false, "virtual texture is not of type 'Font'.");
            return false;
        }
        BOUNDS_CHECK(first_index, ctx->sprite_manager.sprite_generations_length);
        if(ctx->sprite_manager.sprite_generations[first_index] != generation){
            ASSERT(false, "attempted to init a sprite string with a stale sprite-id.");
            return false;
        }
        BOUNDS_CHECK(first_index, ctx->sprite_manager.host_sprites_length);
        if(!renderer_sprite_is_chain_sprite(ctx->sprite_manager.host_sprites[first_index])){
            ASSERT(false, "sprite is not a sprite string / apart of a sprite chain.");
            return false;
        }
    }

    /**
        Order Matters Here:
            virtual_texture -> text -> transform.
    **/
    renderer_sprite_chain_set_virtual_texture_unsafe(ctx, first_index, virtual_texture_index);
    renderer_sprite_chain_set_transform_unsafe(ctx, first_index, transform);
    renderer_sprite_chain_set_state_unsafe(ctx, first_index, is_active ? SpriteState_Active : SpriteState_Inactive);
    renderer_sprite_chain_set_material_unsafe(ctx, first_index, material_index);

    return true;
}

void renderer_renderer_ctx_init(
    RendererContext* ctx, RendererContextInitInfo info, 
    MemoryArena* persistent, MemoryArena* transient, 
    WindowContext window_ctx, u32 window_width, u32 window_height
){
    
    ASSERT(ctx->is_init == false, "cannot init an already init renderer ctx");
    WGPUInstanceDescriptor desc = {0};
    ctx->instance = wgpuCreateInstance(&desc);
    ASSERT(ctx->instance, "failed to create a web gpu instance");

    ctx->adapter = renderer_request_adapter(ctx->instance);
    ctx->device = renderer_request_device(ctx->adapter);

    // surface configuration must be done at the end of the program.
    renderer_virtual_texture_manager_init(
        &ctx->virtual_texture_manager, persistent, ctx->device, 
        info.image_textures_init_infos, info.image_textures_init_infos_length, 
        info.font_textures_init_info, info.max_virtual_textures, info.max_file_path_length
    );

    renderer_sprite_manager_init(&ctx->sprite_manager, ctx->device, persistent, info.sprite_layer_create_infos, info.sprite_layer_create_infos_length);
    renderer_vertex_buffer_init(&ctx->vertex_buffer, ctx->device);
    renderer_index_buffer_init(&ctx->index_buffer, transient, ctx->device, ctx->sprite_manager.device_sprites_length);
    renderer_user_uniform_buffer_init(&ctx->user_uniform_buffer, ctx->device, info.max_user_uniform_buffer_size_in_bytes);
    renderer_user_storage_buffer_init(&ctx->user_storage_buffer, ctx->device, info.max_user_storage_buffer_size_in_bytes);
    renderer_link_to_window(ctx, window_ctx, window_width, window_height);
    renderer_final_render_target_init(&ctx->final_render_texture, ctx->device, info.final_render_texture_width, info.final_render_texture_height);
    renderer_depth_texture_init(&ctx->depth_texture, ctx->device, info.final_render_texture_width, info.final_render_texture_height);
    renderer_blit_pipeline_init(&ctx->blit_pipeline, ctx->window_surface, ctx->device, ctx->adapter);
    renderer_graphics_pipeline_init(
        &ctx->graphics_pipeline, transient, ctx->device, ctx->adapter, ctx->window_surface, 
        ctx->virtual_texture_manager, ctx->final_render_texture, info.graphics_pipeline_shader_file_path,
        ctx->user_uniform_buffer, ctx->user_storage_buffer, ctx->sprite_manager.sprite_buffer,
        info.max_user_uniform_buffer_size_in_bytes, info.max_user_storage_buffer_size_in_bytes
    );
    ctx->is_init = true;
    
    renderer_update_render_destination_rectangle(ctx);
}

void renderer_draw_renderer(RendererContext* ctx){
    
    ASSERT(ctx->is_init, "attempted to draw a non-initialised rendering context.");
    SurfaceTexture swapchain_texture = renderer_get_next_spawn_chain_image_view(ctx);
    
    WGPUDevice device = ctx->device;

    /**
        uniform preparation.
    **/
    renderer_write_to_buffer(&ctx->virtual_texture_manager.device_virtual_texture_buffer, ctx->device, ctx->virtual_texture_manager.device_virtual_textures, ctx->virtual_texture_manager.device_virtual_textures_length);
    /**
        TODO: (nich s)
        This may have to be optimised out later for a compute buffer operation to sort sprites; so that it is faster.
        but that depends entirely upon how many sprites the game is actually going to have; right now CPU sorting is fast enough.
    **/
    // prepare for sorting.
    Colour c = ctx->sprite_manager.device_sprites[1].colour;
    COPY_MEMORY(ctx->sprite_manager.device_sprites_scratch_space, ctx->sprite_manager.device_sprites, sizeof(DeviceSprite) * ctx->sprite_manager.device_sprites_length);
    Colour a = ctx->sprite_manager.device_sprites_scratch_space[1].colour;
    /**
        sort sprites by their z position within their local layer groups.

        Example Output:
        ----------------------------------
        | Sprite Id | Z Position | Layer |
        ----------------------------------
        | 0         | 0          | 0     |
        | 1         | 1          | 0     |
        | 2         | 2          | 0     |
        | 3         | -1         | 1     |
        | 4         | 23         | 1     |
        | 5         | 43         | 1     |
        | 6         | -99        | 2     |
        ----------------------------------
    **/
    i32 ptr_offset = 0;

    
    for(i32 i = 0; i < ctx->sprite_manager.sprite_layers_length; i++){
        SpriteLayer* layer = &ctx->sprite_manager.sprite_layers[i];
        DeviceSprite* ptr = ctx->sprite_manager.device_sprites_scratch_space + ptr_offset;
        quicksort_device_sprite_dsc(ptr, layer->max_sprites);
        ptr_offset += layer->max_sprites;
    }
    renderer_write_to_buffer(
        &ctx->sprite_manager.sprite_buffer, 
        device, 
        ctx->sprite_manager.device_sprites_scratch_space, 
        sizeof(DeviceSprite) * ctx->sprite_manager.device_sprites_scratch_space_length
    );
    
    /**
        command encoder creation.
    **/
    WGPUCommandEncoderDescriptor cmd_enc_desc = {0};
    WGPUCommandEncoder cmd_enc = wgpuDeviceCreateCommandEncoder(device, &cmd_enc_desc);

    /**
        render pass 1: final render texture.
    **/
    {
        /**
            colour attachment.
        **/
        // render to the final render target image view.
        ASSERT(ctx->final_render_texture.is_init == true, "attempted render pass initialisation with an un-init final render texture.");
        WGPURenderPassColorAttachment colour_att = {
            .view = ctx->final_render_texture.view,
            .loadOp = WGPULoadOp_Clear,
            .storeOp = WGPUStoreOp_Store,
            .clearValue = (WGPUColor){.r = 0.01f, .g = 0.01f, .b = 0.01f, .a = 1.0f},
            .depthSlice = WGPU_DEPTH_SLICE_UNDEFINED
        };
        
        /**
            depth attachment.
        **/
        ASSERT(ctx->depth_texture.is_init == true, "attempted render pass initialisation with an un-init depth texture.");
        WGPURenderPassDepthStencilAttachment depth_att = {
            .view = ctx->depth_texture.view,
            // the initial value of the depth buffer, 1 = "far".
            .depthClearValue = 1.0f,
            .depthLoadOp = WGPULoadOp_Clear,
            .depthStoreOp = WGPUStoreOp_Store,
            .depthReadOnly = WGPU_FALSE,
            // stencil setup is mandatory but unused.
            .stencilClearValue = 0,
            .stencilLoadOp = WGPULoadOp_Clear,
            .stencilStoreOp = WGPUStoreOp_Store,
            .stencilReadOnly = WGPU_FALSE
        };
        
        /**
            render pass creation and encoding.
        **/
        WGPURenderPassDescriptor render_pass_desc = {
            .colorAttachmentCount = 1,
            .colorAttachments = &colour_att,
            .depthStencilAttachment = &depth_att,
        };
        WGPURenderPassEncoder render_pass = wgpuCommandEncoderBeginRenderPass(cmd_enc, &render_pass_desc);
        wgpuRenderPassEncoderSetPipeline(render_pass, ctx->graphics_pipeline.render_pipeline);
        wgpuRenderPassEncoderSetVertexBuffer(render_pass, 0, ctx->vertex_buffer.device, 0, ctx->vertex_buffer.count_in_bytes);
        wgpuRenderPassEncoderSetIndexBuffer(render_pass, ctx->index_buffer.device, WGPUIndexFormat_Uint32, 0, ctx->index_buffer.count_in_bytes);
        wgpuRenderPassEncoderSetBindGroup(render_pass, 0, ctx->graphics_pipeline.bind_group_0, 0, NULL);
        wgpuRenderPassEncoderSetBindGroup(render_pass, 1, ctx->graphics_pipeline.bind_group_1, 0, NULL);
        wgpuRenderPassEncoderSetBindGroup(render_pass, 2, ctx->graphics_pipeline.bind_group_2, 0, NULL);
        wgpuRenderPassEncoderDrawIndexed(render_pass, 6, ctx->sprite_manager.device_sprites_length, 0, 0, 0);
        wgpuRenderPassEncoderEnd(render_pass);
        wgpuRenderPassEncoderRelease(render_pass);
    }

    /**
        render pass 2: blit to the back buffer.
    **/
    {
        /**
            bind group creation.
        **/
        /**
            Dynamically create per-frame bind groups, linking the offscreen texture to the current frame's swap chain texture.
        **/
            /**
                entries.
            **/
            WGPUBindGroupEntry* entries = (WGPUBindGroupEntry[2]){};
            // no-filter sampler
            BOUNDS_CHECK(BLIT_PIPELINE_SAMPLER_BINDING, 2);
            entries[BLIT_PIPELINE_SAMPLER_BINDING] = (WGPUBindGroupEntry){
                .binding = BLIT_PIPELINE_SAMPLER_BINDING,
                .sampler = ctx->blit_pipeline.sampler,
            };
            // final render texture.
            BOUNDS_CHECK(BLIT_PIPELINE_TEXTURE_BINDING, 2);
            entries[BLIT_PIPELINE_TEXTURE_BINDING] = (WGPUBindGroupEntry){
                .binding = BLIT_PIPELINE_TEXTURE_BINDING,
                .textureView = ctx->final_render_texture.view
            };

            /**
                group.
            **/
            WGPUBindGroupDescriptor gd = {
                .layout = ctx->blit_pipeline.bind_group_layout,
                .entryCount = BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT,
                .entries = entries
            };
            WGPUBindGroup bind_group = wgpuDeviceCreateBindGroup(device, &gd);

        /**
            colour attachment.
        **/
        WGPURenderPassColorAttachment colour_att = {
            .view = swapchain_texture.view,
            .loadOp = WGPULoadOp_Clear,
            .storeOp = WGPUStoreOp_Store,
            .depthSlice = WGPU_DEPTH_SLICE_UNDEFINED
        };

        /**
            render pass creation.
        **/
        WGPURenderPassDescriptor render_pass_desc= {
            .colorAttachmentCount = 1,
            .colorAttachments = &colour_att,
        };
        WGPURenderPassEncoder render_pass = wgpuCommandEncoderBeginRenderPass(cmd_enc, &render_pass_desc);

        /**
            render pass encoding.
        **/
        wgpuRenderPassEncoderSetPipeline(render_pass, ctx->blit_pipeline.render_pipeline);
        wgpuRenderPassEncoderSetBindGroup(render_pass, 0, bind_group, 0, NULL);
        // WebGPUApi.RenderPassEncoderSetViewport(
        //     renderPass, 0, 0, ctx.FinalRenderTexture.Extents.Width, ctx.FinalRenderTexture.Extents.Height, 0.0f, 1.0f
        // );
        wgpuRenderPassEncoderSetViewport(
            render_pass, 
            ctx->destination_rectangle.x, ctx->destination_rectangle.y,
            (f32)ctx->destination_rectangle.width, (f32)ctx->destination_rectangle.height,
            0.0f, 1.0f
        );
        // draw the quad (sampling into the src texture) onto the destination texture.
        wgpuRenderPassEncoderDraw(render_pass, 4, 1, 0, 0);
        wgpuRenderPassEncoderEnd(render_pass);

        /**
            clean-up.
        **/
        wgpuBindGroupRelease(bind_group);
    }

    /**
        submit command buffer.
    **/
    WGPUCommandBufferDescriptor cmd_buf_desc = {0};;
    WGPUCommandBuffer cmd_buf = wgpuCommandEncoderFinish(cmd_enc, &cmd_buf_desc);
    wgpuCommandEncoderRelease(cmd_enc);
    wgpuQueueSubmit(wgpuDeviceGetQueue(device), 1, &cmd_buf);
    wgpuCommandBufferRelease(cmd_buf);
    
    wgpuSurfacePresent(ctx->window_surface.surface);

    /**
        clean-up.
    **/
    renderer_free_surface_texture(&swapchain_texture);
    for(i32 i = 0; i < ctx->sprite_manager.one_frame_sprites_stack_count; i++){
        renderer_dealloc_sprite(ctx, ctx->sprite_manager.one_frame_sprites_stack[i]);
    }
    ctx->sprite_manager.one_frame_sprites_stack_count = 0;
}

void renderer_texture_free_resources(Texture* texture){
    wgpuTextureViewRelease(texture->view);
    wgpuTextureDestroy(texture->ptr);
    wgpuTextureRelease(texture->ptr);
    texture->is_init = false;
}

void renderer_texture_array_free_resources(TextureArray* array){
    wgpuTextureDestroy(array->texture);
    wgpuTextureRelease(array->texture);
    wgpuTextureViewRelease(array->view);
}

void renderer_buffer_free_resources(RenderBuffer* buffer){
    /**
        TODO:
        you may need to also release the buffer, but that crashes for some reason???
    **/
    wgpuBufferDestroy(buffer->host);
    wgpuBufferDestroy(buffer->device);
    buffer->host = NULL;
    buffer->device = NULL;
}

void renderer_virtual_texture_manager_free_resources(VirtualTextureManager* manager){
    for(i32 i = 0; i < manager->texture_arrays_length; i++){
        renderer_texture_array_free_resources(&manager->texture_arrays[i]);
    }
    renderer_buffer_free_resources(&manager->device_virtual_texture_buffer);    
}

void renderer_sprite_manager_free_resources(SpriteManager* sprite_manager){
    renderer_buffer_free_resources(&sprite_manager->sprite_buffer);
}

void renderer_graphics_pipeline_free_resources(GraphicsPipeline* pipeline){
    wgpuRenderPipelineRelease(pipeline->render_pipeline);
    wgpuPipelineLayoutRelease(pipeline->pipeline_layout);
    wgpuSamplerRelease(pipeline->non_filter_sampler);
    wgpuBindGroupLayoutRelease(pipeline->bind_group_layout_0);
    wgpuBindGroupLayoutRelease(pipeline->bind_group_layout_1);
    wgpuBindGroupLayoutRelease(pipeline->bind_group_layout_2);
    wgpuBindGroupRelease(pipeline->bind_group_0);
    wgpuBindGroupRelease(pipeline->bind_group_1);
    wgpuBindGroupRelease(pipeline->bind_group_2);
}

void renderer_window_surface_free_resources(WindowSurface* window_surface){
    wgpuSurfaceUnconfigure(window_surface->surface);
    wgpuSurfaceRelease(window_surface->surface);
}

void renderer_context_free_resources(RendererContext* ctx){
    ASSERT(ctx->is_init, "attempted to free an un-initialised rendering context.");
    renderer_texture_free_resources(&ctx->depth_texture);
    renderer_texture_free_resources(&ctx->final_render_texture);
    renderer_buffer_free_resources(&ctx->index_buffer);
    renderer_buffer_free_resources(&ctx->vertex_buffer);
    renderer_buffer_free_resources(&ctx->user_uniform_buffer);
    renderer_buffer_free_resources(&ctx->user_storage_buffer);
    renderer_graphics_pipeline_free_resources(&ctx->graphics_pipeline);
    renderer_virtual_texture_manager_free_resources(&ctx->virtual_texture_manager);
    renderer_window_surface_free_resources(&ctx->window_surface);
    wgpuInstanceRelease(ctx->instance);   
}

void renderer_perspective_camera_init(Camera* camera, Vector3 position, f32 near_z, f32 far_z, f32 fov_in_radians){

    { // validation.
        ASSERT(!camera->is_init, "attempted to init an already init camera.");
        ASSERT(near_z >= F32_EPSILON, "camera shouldnt be init with a near_z plane value less than F32_EPSILON");
        ASSERT(far_z > near_z, "far_z value should be less than the near_z.");
    }

    camera->position = position;
    camera->near_z = CLAMP(near_z, F32_EPSILON, F32_MAX);
    camera->far_z = CLAMP(far_z, near_z, F32_MAX);
    camera->perspective_fov = fov_in_radians;
    camera->projection_type = CameraProjectionType_Perspective;
    camera->is_init = true;
}

void renderer_orthographic_camera_init(Camera* camera, Vector3 position, f32 near_z, f32 far_z, f32 orthographic_size){
    
    { // validation.
        ASSERT(!camera->is_init, "attempted to init an already init camera.");
        ASSERT(near_z >= F32_EPSILON, "camera shouldnt be init with a near_z plane value less than F32_EPSILON");
        ASSERT(far_z > near_z, "far_z value should be less than the near_z.");
    }

    camera->position = position;
    camera->near_z = CLAMP(near_z, F32_EPSILON, F32_MAX);
    camera->far_z = CLAMP(far_z, near_z, F32_MAX);
    camera->orthographic_size = orthographic_size;
    camera->projection_type = CameraProjectionType_Orthographic;
    camera->is_init = true;
}

void renderer_camera_update_projection_matrix(Camera* camera, f32 surface_aspect_ratio){
    Vector3 look_at_pos = {.x = camera->position.x, .y = camera->position.y, .z = camera->position.z + 0.001f}; // TODO: try F32_EPSILON instead of 0.001f.
    Vector3 world_up_dir = {.y = 1};

    camera->view = matrix4x4_create_look_at(camera->position, look_at_pos, world_up_dir);    
    camera->model = MATRIX4X4_IDENTITY;

    switch(camera->projection_type){
        case CameraProjectionType_Perspective:{
            camera->projection = matrix4x4_create_perspective(camera->perspective_fov, surface_aspect_ratio, camera->near_z, camera->far_z);
        }break;
        case CameraProjectionType_Orthographic:{
            // Compute half-width and half-height in world units based on virtual resolution
            f32 half_height = camera->orthographic_size * 0.5f;
            f32 half_width = half_height * surface_aspect_ratio;
            camera->projection = matrix4x4_create_orthographic(-half_width, half_width, -half_height, half_height, camera->near_z, camera->far_z);
        }break;
    }
}
