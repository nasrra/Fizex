#include "vendors/webgpu/webgpu.h"

/*====================
    types.
====================*//**/

typedef struct{
    WGPUSurfaceTexture ptr;
    WGPUTextureView view;
    WGPUExtent3D Extents;
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
} Buffer;

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
    Buffer virtual_texture_buffer;
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
    i32 virtual_texture_index;
    i32 material_index; 
    i32 colour_state;
    i32 layer;
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
    GenId genid;
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
        the virtual textures to initialise as font textures.
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
    u32 max_user_defined_uniform_buffer_size_in_bytes;
    u32 max_user_defined_storage_buffer_size_in_bytes;
    u32 final_render_texture_width;
    u32 final_render_texture_height;
    String graphics_pipeline_shader_file_path;
} RendererCtxInitInfo;

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
    DeviceSprite* device_sprites_scratch_space;
    i32 device_sprites_scratch_space_length;
    SpriteLayer* sprite_layers;
    i32 sprite_layers_length;
    Buffer sprite_buffer;
    bool is_init;
} SpriteManager;

typedef struct{
    WGPUInstance instance;
    WGPUAdapter adapter;
    WGPUDevice device;
    GraphicsPipeline graphics_pipeline;
    BlitPipeline blit_pipeline;
    Buffer vertex_buffer;
    Buffer index_buffer;
    Buffer user_defined_uniform_buffer;
    Buffer user_defined_storage_buffer;
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
    Rectangle destination_rectangle;
    bool is_init;
} RendererCtx;

/*====================
    defines
====================*//**/

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

#define VERTEX_SHADER_ENTRY_POINT_LENGTH 7
#define VERTEX_SHADER_ENTRY_POINT "vs_main"
#define FRAGMENT_SHADER_ENTRY_POINT_LENGTH 7
#define FRAGMENT_SHADER_ENTRY_POINT "fs_main"

#define BLIT_SHADER_CODE_LENGTH 1014
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

/*====================
    functions.
====================*//**/

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
    /*
        Start with everything undefined, meaning:
        "don't require a specific value for this limit."    
    */
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

void renderer_buffer_init(Buffer* buffer, WGPUDevice device, WGPUBufferUsage host_usage, WGPUBufferUsage device_usage, u32 array_length, u32 element_length_in_bytes){

    *buffer = (Buffer){0};

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
        ASSERT(manager->is_init==false, "virtual texture manager is already init.");
        ASSERT(device==NULL, "cannot init virtual texture manager with a null gpu device.");
        ASSERT(max_virtual_textures >= 2, "virtual texture manager should not be initialised with less than 2 virtual textures.");
        ASSERT(max_virtual_textures <= DEVICE_VIRTUAL_TEXTURE_MAX_AMOUNT, "max virtual textures exceeds 4096.");
    }

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->device_virtual_textures, &manager->device_virtual_textures_length, max_virtual_textures);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->host_virtual_textures, &manager->host_virtual_textures_length, max_virtual_textures);
    
    // initialise virtual textures.
    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    renderer_buffer_init(&manager->virtual_texture_buffer, device, host_usage, device_usage, max_virtual_textures, sizeof(DeviceVirtualTexture));
    for(i32 i = 0; i < manager->host_virtual_textures_length; i++){
        BOUNDS_CHECK(i,manager->host_virtual_textures_length);
        string_init(&manager->host_virtual_textures[i].file_path, arena, file_path_max_chars);
    }

    // initialise font virtual textures.
    for(i32 i = 0; i < font_info.virtual_textures_length; i++){
        i32 virtual_texture_index = font_info.virtual_textures[i];

        BOUNDS_CHECK(virtual_texture_index, manager->host_virtual_textures_length);
        HostVirtualTexture* host = &manager->host_virtual_textures[virtual_texture_index];

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
    renderer_texture_array_init(&manager->texture_arrays[write_index], device, arena, nil_texture_format, 1, 1, 1);

    // init the font texture.
    write_index = VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX;
    /*
        Font textures are an array of bytes ranging from 0-256 for one channel ('red' - relative to the gpu - as it is a one channel value).
        These values should be normalised in the shader; converting 0-256 to 0-1.
    */
    WGPUTextureFormat font_texture_format = WGPUTextureFormat_R8Unorm;
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
        renderer_texture_array_init(
            &manager->texture_arrays[write_index], device, arena, image_texture_format, create_info->width, create_info->height, create_info->max_textures
        );
        write_index++;
    }
    manager->is_init = true;
}

void renderer_sprite_manager_init(SpriteManager* manager, WGPUDevice device, MemoryArena* arena, SpriteLayerCreateInfo* layer_infos, i32 layer_infos_length){
    
    { // validation step.
        ASSERT(manager->is_init == false, "attempted to init an already init sprite manager.");
        ASSERT(layer_infos == NULL, "layer_infos is NULL.");
    }

    i32 max_sprites = 0;
    for(i32 i = 0; i < layer_infos_length; i++){
        ASSERT(layer_infos[i].max_sprites > 0, "should not init sprite layer with less than 1 sprite; as a NIL element is required.");
        max_sprites += layer_infos[i].max_sprites;
    }

    ASSERT(max_sprites>=DEVICE_SPRITE_MAX_AMOUNT, "attempted to init sprite manager with a sprite amount greater than the max device sprite amount.");
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

bool renderer_write_to_buffer(Buffer* dst, WGPUDevice device, void* src, u32 src_byte_size){

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

void renderer_vertex_buffer_init(Buffer* buffer, WGPUDevice device){
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
            .position = {.x = 1, .y = 1},
            .uv = {.x = 1.0f, .y = 1.0f}
        },
        { // bottom left
            .position = {.x = -0.5f, .y = -0.5f},
            .uv = {.x = 0.0f, .y = 1.0f}
        }
    };
    renderer_write_to_buffer(buffer, device, (void*)vertices, sizeof(Vertex) * 4);
}

void renderer_index_buffer_init(Buffer* buffer, WGPUDevice device, i32 max_sprites){

    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init index buffer with an already init buffer.");

    // round to the next multiple of four.
    i32 vertices_count = max_sprites * 4;
    i32 total_vertices = (vertices_count + 3) & ~3;
    // calculate the required amount of indices.
    i32 total_indices = total_vertices / 4 * 6;

    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index;
    renderer_buffer_init(buffer, device, host_usage, device_usage, total_indices, sizeof(u32));

    i32 temp_count = 0;
    u32* indices = (u32[]){total_indices};
    for(int i = 0; i < total_vertices; i+=4){
        ARRAY_PUSH(indices, total_indices, &temp_count, i);
        ARRAY_PUSH(indices, total_indices, &temp_count, i+1);
        ARRAY_PUSH(indices, total_indices, &temp_count, i+2);
        ARRAY_PUSH(indices, total_indices, &temp_count, i+2);
        ARRAY_PUSH(indices, total_indices, &temp_count, i+3);
        ARRAY_PUSH(indices, total_indices, &temp_count, i);
    }

    renderer_write_to_buffer(buffer, device, indices, sizeof(u32) * total_indices);
}

void renderer_user_defined_uniform_buffer_init(Buffer* buffer, WGPUDevice device, u32 size_in_bytes){
    ASSERT(buffer->host == NULL && buffer->device == NULL, "attempted to init user-uniform-buffer with an already init buffer.");
    // ensure that the size is a multiple of 16; accounting for the 16 byte padding of WG.
    // uint adjustedSize = (sizeOfUbo + 15) & ~15u;
    WGPUBufferUsage host_usage = WGPUBufferUsage_MapWrite | WGPUBufferUsage_CopySrc;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform; 
    renderer_buffer_init(buffer, device, host_usage, device_usage, 1, size_in_bytes);
}

void renderer_user_defined_storage_buffer_init(Buffer* buffer, WGPUDevice device, u32 size_in_bytes){
    ASSERT(buffer->host == NULL && buffer->device, "attempted to init a user-defined-storage buffer with an aleady init buffer.");
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

void renderer_link_to_window(RendererCtx* ctx, WindowCtx window_ctx, u32 window_width, u32 window_height){
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
    WGPUTextureFormat format = WGPUTextureFormat_BGRA8UnormSrgb;
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

void renderer_blit_pipeline_init(BlitPipeline* pipeline, WindowSurface window_surface, WGPUDevice device, WGPUAdapter device_adapter){
    { // validation.
        ASSERT(pipeline->is_init == false, "attempted to init an already init graphics pipeline.");
    }

    renderer_sampler_init(&pipeline->sampler, device);

    WGPUShaderModule shader_module = {0};

    // shader creation.
    renderer_shader_module_init(&shader_module, device, (String){.chars = BLIT_SHADER_CODE, .length = BLIT_SHADER_CODE_LENGTH});

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
        .entryPoint = (WGPUStringView){.data = VERTEX_SHADER_ENTRY_POINT, .length = VERTEX_SHADER_ENTRY_POINT_LENGTH},
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
    WGPUTextureFormat preferredFormat = capabilities.formats[0]; // 0 is guaranteed to be the preferred/optimal format.
    WGPUColorTargetState colour_target_state = {    
        .format = preferredFormat,
        .blend = &blend_state,
        .writeMask = WGPUColorWriteMask_All
    };
    WGPUFragmentState fragment_state = {
        .module = shader_module,
        .entryPoint = (WGPUStringView){.data = FRAGMENT_SHADER_ENTRY_POINT, .length = FRAGMENT_SHADER_ENTRY_POINT_LENGTH},
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
        WGPUBindGroupLayoutEntry* sampler_layout_entry = &group_layout_entries[BLIT_PIPELINE_SAMPLER_BINDING];
        *sampler_layout_entry = (WGPUBindGroupLayoutEntry){
            .binding = BLIT_PIPELINE_SAMPLER_BINDING,
            .visibility = WGPUShaderStage_Fragment,
            .sampler.type = WGPUSamplerBindingType_NonFiltering
        };
        WGPUBindGroupLayoutEntry* texture_layout_entry = &group_layout_entries[BLIT_PIPELINE_TEXTURE_BINDING];
        // *texture_layout_entry = (WGPUBindGroupLayout){
        //     .binding 
        // };
        WGPUBindGroupLayoutDescriptor group_layout_desc = {
            .entryCount = BLIT_PIPELINE_BIND_GROUP_ENTRY_COUNT,

        };

    //     groupLayoutDesc.EntryCount = BlitPipeline.BindGroupEntryCount;
    //     WebGPU.BindGroupLayoutEntry* groupLayoutEntries = stackalloc WebGPU.BindGroupLayoutEntry[(int)BlitPipeline.BindGroupEntryCount];
    //         ref WebGPU.BindGroupLayoutEntry textureLayoutEntry = ref groupLayoutEntries[BlitPipeline.TextureBinding];
    //         textureLayoutEntry.Binding = BlitPipeline.TextureBinding;
    //         textureLayoutEntry.Visibility = WebGPU.ShaderStage.Fragment;
    //         textureLayoutEntry.Texture.SampleType = WebGPU.TextureSampleType.Float;
    //         textureLayoutEntry.Texture.ViewDimension = WebGPU.TextureViewDimension.Dimension2D;
    //     }
    //     groupLayoutDesc.Entries = groupLayoutEntries;
    //     ctx.BlitPipeline.BindGroupLayout = WebGPUApi.DeviceCreateBindGroupLayout(GetChosenDevice(ref ctx).Pointer, ref groupLayoutDesc);

    // WebGPU.BindGroupLayout** layouts = stackalloc WebGPU.BindGroupLayout*[1]{ctx.BlitPipeline.BindGroupLayout};
    // WebGPU.PipelineLayoutDescriptor pipelineLayoutDesc = default;
    // pipelineLayoutDesc.BindGroupLayoutCount = 1;
    // pipelineLayoutDesc.BindGroupLayouts = layouts;
    // WebGPU.PipelineLayout* pipelineLayout = WebGPUApi.DeviceCreatePipelineLayout(device.Pointer, ref pipelineLayoutDesc);


    // /**========================================
    //     DESCRIPTOR SETS
    // ========================================**/
    //     /**========================================
    //         BIND GROUP
    //     ========================================**/
    //     /**
    //         note that the binding groups are not setup here at all as they should be dynamically created during the render pass,
    //         This is because the swap chain texture which is being blitted constantly changes (as per the nature of swap chain structures).
    //     **/

    // /**========================================
    //     PIPELINE CREATION.
    // ========================================**/    
    // WebGPU.RenderPipelineDescriptor pipelineDesc = default;
    // pipelineDesc.Layout = pipelineLayout;
    // pipelineDesc.Primitive = primitiveState;
    // pipelineDesc.Multisample = multisampleState;
    // pipelineDesc.Fragment = &fragState;
    // pipelineDesc.Vertex = vertState;
    // pipelineDesc.DepthStencil = null;
    // ctx.BlitPipeline.RenderPipeline = WebGPUApi.DeviceCreateRenderPipeline(device.Pointer, ref pipelineDesc);
    // ctx.BlitPipeline.IsInitialised = true;

    /*
        cleanup.
    */
    wgpuSurfaceCapabilitiesFreeMembers(capabilities);

    // /**========================================
    //     CLEAN-UP.
    // ========================================**/
    // WebGPUApi.PipelineLayoutRelease(pipelineLayout);
    // WebGPUApi.ShaderModuleRelease(shaderModule);
}

/*
public static void FreeBlitPipeline(
    ref BlitPipeline pipeline
){
    WebGPUApi.RenderPipelineRelease(pipeline.RenderPipeline);
}

*/

void renderer_renderer_ctx_init(
    RendererCtx* ctx, RendererCtxInitInfo info, 
    MemoryArena* persistent, MemoryArena* transient, 
    WindowCtx window_ctx, u32 window_width, u32 window_height
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
    renderer_index_buffer_init(&ctx->index_buffer, ctx->device, ctx->sprite_manager.device_sprites_length);
    renderer_user_defined_uniform_buffer_init(&ctx->user_defined_uniform_buffer, ctx->device, info.max_user_defined_uniform_buffer_size_in_bytes);
    renderer_user_defined_storage_buffer_init(&ctx->user_defined_storage_buffer, ctx->device, info.max_user_defined_storage_buffer_size_in_bytes);
    renderer_link_to_window(ctx, window_ctx, window_width, window_height);
    renderer_final_render_target_init(&ctx->final_render_texture, ctx->device, info.final_render_texture_width, info.final_render_texture_height);
    renderer_depth_texture_init(&ctx->depth_texture, ctx->device, info.final_render_texture_width, info.final_render_texture_height);

    /*    
    InitBlitPipeline(ref ctx);
    InitGraphicsPipeline(
        ref ctx, info.GraphicsPipelineShaderFilePath, 
        info.MaxUserUniformBufferSizeInBytes, info.MaxUserStorageBufferSizeInBytes
    );
    UpdateDestinationRectangle(ref ctx);
    ctx.IsInitialised = true;
    */
}