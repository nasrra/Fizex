/******************

    DOCUMENTATION.

*******************/
/**
NOTE

    make sure that near the end of development to add supported limit suitability checks for devices and adapters.  
**/
/** 
LOCATIONS

    @location(l) is exclusively for Vertex Data (inputs coming from vertex buffers and Fragment Outputs  (sending colours to textures/screen). 
    It is a direct hardware register slot used during the fixed-function pipeline stages. Note that the max slots is ~16 to 32 (check device limits)
    also that locations are relative to the render pass stage (@location(0) on the vertex stage is not the same slot as @location(0) on the fragement).

        Vertex Shader:
            It maps data from the vertex buffer into the shader. Each location represents an attribute of a vertex:

            Example:
                struct Vertex{
                    @location(0) position: vec3f,
                    @location(1) uv: vec2f
                }

        Fragment Shader:
            it maps the colour a shader calculated to a specific output target (render target/texture).
            @location(0) is almost always the main screen or canvas texture.
**/
/**
GROUP & BINDING

    While vertex data flows dynamically per vertex through locations, textures and structures use groups & bindings.
    They form a two tier hierarchy to organise resources:

    @group(g) can bind up to four distinct groups at the exact same time (slots 0 to 3). Groups are typically used to categorise
    data by how frequently it changes; in our case the hierarchy is as follows:

        @group(0): uniform and ssbo buffers .
            @binding(0): user defined UBOs.
            @binding(1): virtual textures.
            @binding(2): sprite buffer.
            @binding(3): user defind SSBOs.
        
        @group(1): texture arrays.
            @binding(0..n): the registered texture arrays in the order that they were created (first-in first-out) 
        
        @group(2): utilities (samplers).
            @binding(0): non-filtering sampler.

    @binding(b) is the specific resource inside a group; you can have hundreds (max 1000) binding slots per group.
**/
/**
    Matrix4x4 Math

    r = right / horizontal.
    u = up / vertical.
    f = forward / depth.
    t = translation.

    -------------------------       -----       -----------------------------------------
    | r.x | u.x | f.x | t.x |       | x |       | (x*r.x) + (y*u.x) + (z*f.x) + (w*t.x) |
    | r.y | u.y | f.y | t.y |       | y |       | (x*r.y) + (y*u.y) + (z*f.y) + (w*t.y) |
    | r.z | u.z | f.z | t.z |   X   | z |   =   | (x*r.z) + (y*u.z) + (z*f.z) + (w*t.z) |
    | r.w | u.w | f.w | t.w |       | w |       | (x*r.w) + (y*u.w) + (z*f.w) + (w*t.w) |
    -------------------------       -----       -----------------------------------------

    the gpu divides the final 'x', 'y', and 'z' by the final 'w' value.
    converting a 4D Clip Space coorfinat into 3D Normalised Device Coordinates (NDC).

        4DCP to NDC    
    -------        -----
    | x/w |        | x |
    | y/w |        | y |
    | z/w |    =   | w |
    | w/w |        | 1 |
    -------        -----

**/

/**##########################################################################################################################################
    div: DEFINITIONS.
##########################################################################################################################################**/

/**========================================
    LAYERS
========================================**/
// everything above this layer (2,3,4...) is considered a screen layer.
const screen_layer = 1;

/**========================================
    COLOURS
========================================**/
const red: vec4f             = vec4f(1.0, 0.0, 0.0, 1.0);
const green: vec4f           = vec4f(0.0, 1.0, 0.0, 1.0);
const blue: vec4f            = vec4f(0.0, 1.0, 0.0, 1.0);
const yellow: vec4f          = vec4f(1.0, 1.0, 0.0, 1.0);
const orange: vec4f          = vec4f(1.0, 0.5, 0.0, 1.0);
const light_blue: vec4f      = vec4f(0.5, 0.5, 1.0, 1.0);
const light_green: vec4f     = vec4f(0.5, 1.0, 0.5, 1.0);
const white: vec4f           = vec4f(1.0, 1.0, 1.0, 1.0);
const black: vec4f           = vec4f(0.0, 0.0, 0.0, 1.0);
const fail_colour: vec4f     = vec4f(1.0, 0.0, 1.0, 1.0);

/**========================================
    SPRITE CONSTANTS.
========================================**/
const sprite_state_active: i32 = 2;
const sprite_state_inactive: i32 = 1;
const sprite_state_deallocated: i32 = 0;

/**========================================
    COLOUR STATES.
========================================**/
const colour_state_tint: i32 = 0;
const colour_state_override: i32 = 1;

/**========================================
    MATERIALS.
========================================**/
const material_debug = 1;
const material_simple_sprite = 2;
const material_simple_text = 3;

/**========================================
    BINDING STRUCTURES.
========================================**/
struct ubo{
    world_cam_mat: mat4x4f,
    screen_cam_mat: mat4x4f,
    time: f32,
    world_cam_far_z: f32,
    world_cam_near_z: f32,
    pading: f32
}

struct virtual_texture{
    binding: i32,
    layer_index: i32,
    is_loaded: i32,
    padding_0: i32,
}

struct virtual_texture_array{
    textures: array<virtual_texture, 12>
}

struct pixel_coords{
    top_left: vec2i,
    bot_right: vec2i
}

struct sprite{
    // 1 - 64.
    transform: mat4x4f,
    // 64 - 80
    pixel_coords: pixel_coords, 
    // 80 - 96
    colour: vec4f,
    // 96 - 100
    state: i32,
    // 100 - 104
    virtual_texture_index: i32,
    // 104 - 108
    material_id: i32,
    // 108 - 112
    colour_state: i32,
    // 112 - 116
    layer: i32,

    // 116 - 128
    padding_0: i32,
    padding_1: i32,
    padding_2: i32
}

struct sprite_array{
    instance: array<sprite, 512>
}

struct vertex_output {
    /**
        not that when this param is passed  from the vertex shader to the fragment shader, 
        it is automatically converted from Clip Space coordinates to Window Space.
    **/
    @builtin(position) position: vec4f,
    @location(0) vertex_position: vec4f,
    @location(1) pixel_position: vec2f,
    /**
        note that clip space Z value is not normalised (0-1).
    **/
    @location(2) clip_space_position: vec4f,
    @location(3) @interpolate(flat) sprite_index: u32
}

/**========================================
    BINDINGS
========================================**/
@group(0) @binding(0) var<uniform> user_uniform: ubo; 
// @group(0) @binding(1) var<storage> user_storage: user_storage_array;
@group(0) @binding(2) var<uniform> virtual_textures: virtual_texture_array; 
@group(0) @binding(3) var<storage> sprites: sprite_array; 
@group(1) @binding(1) var textures_fonts: texture_2d_array<f32>;
@group(1) @binding(2) var textures_512_512: texture_2d_array<f32>;
@group(1) @binding(3) var textures_360_162: texture_2d_array<f32>;
@group(1) @binding(4) var textures_640_360: texture_2d_array<f32>; 
@group(2) @binding(0) var non_filter_texture_sampler: sampler;

/**##########################################################################################################################################
    div: FUNCTIONS:
##########################################################################################################################################**/

@vertex
fn vs_main (
    @location(0) in_vertex_position: vec3f,
    @location(1) in_vertex_uv: vec2f, 
    // @location(2) in_vertex_sprite_index: i32,
    @builtin(instance_index) sprite_index: u32,
    @builtin(vertex_index) in_vertex_index: u32
) -> vertex_output {
    var out: vertex_output; // Fixed struct declaration syntax
    
    var sprite = sprites.instance[sprite_index];
    if(sprite.state != sprite_state_active){
        return out;
    }


    var position: vec4f;
    if(sprite.layer >= screen_layer){
        position = user_uniform.screen_cam_mat * sprite.transform * vec4f(in_vertex_position, 1.0);
        // position = sprite.transform * vec4f(in_vertex_position, 1.0);

    }
    else{
        
        position = user_uniform.world_cam_mat * sprite.transform * vec4f(in_vertex_position, 1.0);
    }

    out.clip_space_position = position;
    out.position = position;
    out.vertex_position = vec4f(in_vertex_position, 1.0); // Fixed vec3 to vec4 assignment.
    out.sprite_index = sprite_index;

    // 2. Linearly interpolate across the quad based on which corner vertex we are on
    out.pixel_position = mix(vec2f(sprite.pixel_coords.top_left), vec2f(sprite.pixel_coords.bot_right), in_vertex_uv);

    return out;
}

@fragment
fn fs_main(
    in: vertex_output
) -> @location(0) vec4f {

    var sprite = sprites.instance[in.sprite_index];

    var colour: vec4f;

    switch sprite.material_id {
        // return magenta if the material isnt valid.
        case default:{return fail_colour;}
        case material_debug:            {colour = vec4f(0,0,0,0);} // default to white in case colour state is set to Tint.
        case material_simple_sprite:    {colour = fs_draw_sprite_simple(sprite, vec2i(in.pixel_position));}
        case material_simple_text:      {colour = fs_draw_text_simple(sprite, vec2i(in.pixel_position));}
    }

    switch sprite.colour_state{
        case default:{return fail_colour;}
        case colour_state_tint: {colour = vec4f(mix(colour.rgb, colour.rgb * sprite.colour.rgb, sprite.colour.a), colour.a);}
        case colour_state_override: {colour = vec4f(mix(colour.rgb, sprite.colour.rgb, sprite.colour.a), sprite.colour.a);}
    }

    // gamma correction.
    let linear_colour = pow(colour.rgb, vec3f(1)); // was originally 2.2 on Bgra8 display.
    return vec4f(linear_colour, colour.a);   
}

fn fs_draw_sprite_simple(
    sprite: sprite, pixel_position: vec2i
) -> vec4f {
    
    let virtual_texture = virtual_textures.textures[sprite.virtual_texture_index];
    if(virtual_texture.is_loaded == 0){
        return fail_colour;
    }
    
    switch virtual_texture.binding {
        case 1: {return textureLoad(textures_fonts, pixel_position, virtual_texture.layer_index, 0);}
        case 2: {return textureLoad(textures_512_512, pixel_position, virtual_texture.layer_index, 0);}
        case 3: {return textureLoad(textures_360_162, pixel_position, virtual_texture.layer_index, 0);}
        case 4: {return textureLoad(textures_640_360, pixel_position, virtual_texture.layer_index, 0);}
        case default:{return fail_colour;}
    }
}

fn fs_draw_text_simple(
    sprite: sprite, pixel_position: vec2i
) -> vec4f {

    let virtual_texture = virtual_textures.textures[sprite.virtual_texture_index];
    if(virtual_texture.is_loaded == 0){
        return fail_colour;
    }
    
    var colour: vec4f;
    switch virtual_texture.binding {
        case 1: {colour = textureLoad(textures_fonts, pixel_position, virtual_texture.layer_index, 0);}
        case default:{return fail_colour;}
    }

    if(colour.r == 0){
        colour = vec4f(0.0, 0.0, 0.0, 0.0);    
    }
    else{
        colour = white;
    }

    return colour;
}
