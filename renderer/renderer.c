#include "vendors/webgpu/webgpu.h"

/*====================
    types.
====================*//**/

/*
    The adapter provides information about the underlying implementation and harware,
    specifying what its capabilities are.
*/
typedef struct{
    /*
        A pointer to the underlying web gpu resource.
    */
    WGPUAdapter wgpu_adapter;
    /*
        Describe the maximum and minimum values that may limit the behaviour of the
        underlying GPU and its driver.
    */
    WGPULimits supported_limits;
    /*
        Extra information about the adapter; typically used facing, like its name, vendor, etc.
    */    
    WGPUAdapterPropertiesWGPU properties;
    i32 properties_size;
    bool is_init;
} Adapter;

typedef struct{
    /*
        A pointer to the underlying web gpu resource.
    */
    WGPUDevice ptr;
    /*
        The queue used to send both commands and data to the gpu.
    */
    WGPUQueue Queue;
    bool is_init; 
} Device;

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
    u32 size_in_bytes;
    /*
        The count of the `host` and `device` buffers respectively.
    */
    u32 count_in_bytes;
} Buffer;

typedef struct{
    Vector3 position;
    Vector3 uv;
} Vertex;

typedef struct {
    bool request_ended;
    bool is_valid;
} MapAsyncCtx;

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
    i32 free_layer_indices_stack_size;
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
    i32 glyphs_size;
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
    i32 device_virtual_textures_size;
    /*
        remarks:
        - Contains a NIL element.
        - Elements are vertically associated with `device_virtual_textures`.
    */
    HostVirtualTexture* host_virtual_textures;
    i32 host_virtual_textures_size;
    TextureArray* texture_arrays;
    i32 texture_arrays_size;
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
    Vector2 glyph_quad_size;
    i32 next_in_chain;
    bool is_first_in_chain;
} HostSprite;

typedef struct{
    i32 max_sprites;
    /*
        remarks:
        this is a stack array.
    */
    i32* free_sprite_indices_stack;
    i32 free_sprite_indices_stack_size;
} SpriteLayer;

typedef struct{
    /*
        remarks:
        mutating this value will not change the actual windows extents; this is a copied value from initialisation.
    */
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

typedef struct{
    WGPUInstance instance;
    WGPUAdapter adapter;
    WGPUDevice device;
    bool is_init;
} RendererCtx;

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
    i32 virtual_textures_size;
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
    i32 image_textures_init_infos_size;
    FontTextureInitInfo font_textures_init_info;
    /*
        remarks:
        The order of the layers determines the order that they are drawn.
        E.g, layer index 4 will be above layers 3, 2, 1, etc, ... 
    */
    SpriteLayerCreateInfo* sprite_layer_create_infos;
    i32 sprite_layer_create_infos_size;
    i32 max_file_path_length;
    u32 max_user_uniform_buffer_size_in_bytes;
    u32 max_user_storage_buffer_size_in_bytes;
    u32 final_render_texture_width;
    u32 final_render_texture_height;
    String graphics_pipeline_shader_file_path;
} RendererCtxInitInfo;

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
#define DEVICE_SPRITE_MAX_AMOUNT = 1677721

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
    DEBUG_ASSERT(glyph_count > 1, "font data should be init with a glyph count greater than one to account for the Nil element.");
    glyph_count = CLAMP(glyph_count, 1, I32_MAX);
    font_data->base_glyph_index = base_glyph_index;
    MEMORY_ARENA_ALLOC_ARRAY(arena, font_data->glyphs, &font_data->glyphs_size, glyph_count);
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
    DEBUG_ASSERT(wgpu_adapter!=NULL, "failed to retrieve adapter.");
    
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
    DEBUG_ASSERT(wgpu_device!=NULL, "Failed to get the gpu device");

    return wgpu_device;
}

void renderer_buffer_init(Buffer* buffer, WGPUDevice device, WGPUBufferUsage host_usage, WGPUBufferUsage device_usage, u32 array_size, u32 element_size_in_bytes){

    *buffer = (Buffer){0};

    buffer->size_in_bytes = element_size_in_bytes * array_size;
    /*
        create the host staging buffer.
    */
    WGPUBufferDescriptor host_desc = {0};
    host_desc.size = buffer->size_in_bytes;
    host_desc.usage = host_usage;
    host_desc.mappedAtCreation = false;
    buffer->host = wgpuDeviceCreateBuffer(device, &host_desc);
    /*
        create the device local buffer
    */
    WGPUBufferDescriptor device_desc = {0};
    device_desc.size = buffer->size_in_bytes;
    device_desc.usage = device_usage;
    buffer->device = wgpuDeviceCreateBuffer(device, &device_desc);
}

void renderer_texture_array_init(TextureArray* array, WGPUDevice device, MemoryArena* arena, WGPUTextureFormat format, u32 width, u32 height, u32 layer_count){
    
    { // validation steps.
        DEBUG_ASSERT(array->is_init==false, "attempted to init an already init texture array.");
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
    MEMORY_ARENA_ALLOC_ARRAY(arena, array->free_layer_indices_stack, &array->free_layer_indices_stack_size, layer_count);
    for(i32 i = 0; i < array->free_layer_indices_stack_size; i++){
        array->free_layer_indices_stack[i] = i;
    }

    array->extents = text_desc.size;
    array->is_init = true;
}

void renderer_virtual_texture_manager_init(
    VirtualTextureManager* manager, MemoryArena* arena, WGPUDevice device, ImageTexturesInitInfo* image_infos, i32 image_infos_size,
    FontTextureInitInfo font_info, i32 max_virtual_textures, i32 file_path_max_chars 
){
    { // validation.
        DEBUG_ASSERT(manager->is_init==false, "virtual texture manager is already init.");
        DEBUG_ASSERT(device==NULL, "cannot init virtual texture manager with a null gpu device.");
        DEBUG_ASSERT(max_virtual_textures >= 2, "virtual texture manager should not be initialised with less than 2 virtual textures.");
        DEBUG_ASSERT(max_virtual_textures <= DEVICE_VIRTUAL_TEXTURE_MAX_AMOUNT, "max virtual textures exceeds 4096.");
    }

    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->device_virtual_textures, &manager->device_virtual_textures_size, max_virtual_textures);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->host_virtual_textures, &manager->host_virtual_textures_size, max_virtual_textures);
    
    // initialise virtual textures.
    WGPUBufferUsage host_usage = WGPUBufferUsage_CopySrc | WGPUBufferUsage_MapWrite;
    WGPUBufferUsage device_usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    renderer_buffer_init(&manager->virtual_texture_buffer, device, host_usage, device_usage, max_virtual_textures, sizeof(DeviceVirtualTexture));
    for(i32 i = 0; i < manager->host_virtual_textures_size; i++){
        BOUNDS_CHECK(i,manager->host_virtual_textures_size);
        string_init(&manager->host_virtual_textures[i].file_path, arena, file_path_max_chars);
    }

    // initialise font virtual textures.
    for(i32 i = 0; i < font_info.virtual_textures_size; i++){
        i32 virtual_texture_index = font_info.virtual_textures[i];

        BOUNDS_CHECK(virtual_texture_index, manager->host_virtual_textures_size);
        HostVirtualTexture* host = &manager->host_virtual_textures[virtual_texture_index];

        renderer_font_data_init(&host->font_data, arena, font_info.glyph_count, font_info.base_glyph_index);
        host->texture_type = VirtualTextureType_Font;
    }

    /*
        initialise texture arrays.
        +1 for the nil entry and the font texture array.
    */
    i32 texture_array_count = image_infos_size + VIRTUAL_TEXTURE_MANAGER_FONT_TEXTURE_ARRAY_INDEX + 1;
    i32 write_index = 0;
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->texture_arrays, &manager->texture_arrays_size, texture_array_count);
    
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
        font_info.texture_width, font_info.texture_height, font_info.virtual_textures_size
    );

}

void renderer_renderer_ctx_init(
    RendererCtx* ctx, RendererCtxInitInfo info, 
    MemoryArena* persistent, MemoryArena* transient, 
    WindowInfo window_info, u32 window_width, u32 window_height
){
    
    DEBUG_ASSERT(ctx->is_init == false, "cannot init an already init renderer ctx");

    WGPUInstanceDescriptor desc = {0};
    ctx->instance = wgpuCreateInstance(&desc);
    DEBUG_ASSERT(ctx->instance, "failed to create a web gpu instance");

    ctx->adapter = renderer_request_adapter(ctx->instance);
    ctx->device = renderer_request_device(ctx->adapter);


}