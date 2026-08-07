#include <math.h>

/*========================================
    types.
========================================*//**/

typedef struct{
    f32* verts_x;
    f32* verts_y;
    i32 verts_size;
} Polygon;

typedef struct{
    f32 m[9];
} Matrix3x3;

typedef struct{
    f32 m[16];
} Matrix4x4;

typedef struct{
    f32 x;
    f32 y;
} Vector2;

typedef struct {
    f32* x;
    f32* y;
    /*
        the count of allocated entries from appending.
    */
    i32 append_count;
    /*
        the size of all backing arrays.
    */
    i32 size;
    bool is_init;
} Soa_Vector2;

typedef struct{
    /*
        Remarks:
        elements are accessed via `entryElementIndex`.
    */
    f32* x;
    i32 x_size;
    /*
        Remarks:
        elements are accessed via `entryElementIndex`.
    */
    f32* y;
    i32 y_size;
    /*
        Remarks:
        Elements are accessed via `entryIndex`.
    */
    i32* append_counts;
    i32 append_counts_size;
    /*
        the fixed stride of each entry.
    */
    i32 entry_stride;
    i32 max_entries;
    bool is_init;
} FsSoa_Vector2;

typedef struct{
    f32 x;
    f32 y;
    f32 z;
} Vector3;

typedef struct{
    i32 x;
    i32 y; 
} Vector2I;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Quaternion;

typedef struct{
    Quaternion rotation;
    Vector3 position;
    Vector3 scale; 
} Transform;

typedef struct{
    Vector2 position;
    Vector2 scale;
    f32 sine;
    f32 cosine;
    f32 rotation_radii;
} Transform2D;

typedef struct{
    Soa_Vector2 positions;
    Soa_Vector2 scales;
    f32* sines;
    i32 sines_size;
    f32* cosines;
    i32 cosines_size;
    f32* rotation_radii;
    i32 rotation_radii_size;
    bool is_init;
} Soa_Transform2D;

typedef struct{
    f32 x;
    f32 y;
    f32 width;
    f32 height;
} Rectangle;

typedef struct{
    f32 x;
    f32 y;
    f32 radius;
} Circle;

typedef struct{
    f32* min_x;
    f32* min_y;
    f32* max_x;
    f32* max_y;
    /*
        the size of all backing arrays.
    */
    i32 size;
    /*
        the count of allocated entries from appending.
    */
    i32 append_count;
    bool is_init;
} Soa_Aabb;

typedef struct{
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;
} Aabb;

typedef struct{
    /*
        Remarks:
        the size of this array is `POLYGON_RECTANGLE_VERTICES_SIZE`
    */  
    f32 vertices_x[4];
    /*
        Remarks:
        the size of this array is `POLYGON_RECTANGLE_VERTICES_SIZE`
    */  
    f32 vertices_y[4];
} PolygonRectangle;

/*========================================
    defines
========================================*//**/

#define POLYGON_CONTACT_POINT_EPSILON 1e-5f
#define VECTOR2_ONE ((Vector2){1.0f, 1.0f})
#define VECTOR2_UP ((Vector2){0.0f, 1.0f})
#define VECTOR2_DOWN ((Vector2){0.0f, -1.0f})
#define VECTOR2_LEFT ((Vector2)){-1.0f, 0.0f})
#define VECTOR2_RIGHT ((Vector2)){1.0f, 0.0f})
#define VECTOR3_RIGHT ((Vector3){1.0f, 0.0f, 0.0f})
#define VECTOR3_UP ((Vector3){0.0f, 1.0f, 0.0f})
#define VECTOR3_FORWARD ((Vector3){0.0f, 0.0f, 1.0f})
#define VECTOR3_MAX ((Vector3){f32_MAX, f32_MAX, f32_MAX})
#define VECTOR3_ONE ((Vector3){1.0f, 1.0f, 1.0f})
#define QUATERNION_IDENTITY ((Quaternion){.w = 1.0f})
#define TRANSFORM_IDENTITY ((Transform){.scale = VECTOR3_ONE, .rotation = QUATERNION_IDENTITY})
#define TRANSFORM2D_IDENTITY ((Transform2D){.scale = VECTOR2_ONE, .cosine = 1})
#define POLYGON_RECTANGLE_VERTICES_SIZE (i32)4
#define PI 3.1415926535897932384626433f
#define TAU 6.283185307179586f
#define ONE_SIXTH 1.0f / 6.0f
#define ONE_TWENTY_FOURTH 1.0f / 24.0f
#define MATRIX4X4_IDENTITY (Matrix4x4){ \
    .m = { \
        1.0f, 0.0f, 0.0f, 0.0f, \
        0.0f, 1.0f, 0.0f, 0.0f, \
        0.0f, 0.0f, 1.0f, 0.0f, \
        0.0f, 0.0f, 0.0f, 1.0f \
    } \
}
/*
    the fallback normal for any SAT intersect will be up.
    meaning that if any shapes perfectly overlap with eachother
    (sharing the same position) one will be pushed up and the other down.
*/
#define INITIAL_NORMAL VECTOR2_UP

/*========================================
    macros.
========================================*//**/

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ABS(val) ((val) < 0 ? (-(val)) : (val))
#define CLAMP(val, min, max) ((val) < min ? (min) : ((val) > max ? (max) : (val)))

/*========================================
    functions
========================================*//**/

f32 sin_f32(f32 val){
    return sinf(val);
}

f64 sin_f64(f64 val){
    return sin(val);
}

f32 cos_f32(f32 val){
    return cosf(val);
}

f64 cos_f64(f64 val){
    return cos(val);
}

f32 tan_f32(f32 val){
    return tanf(val);
}

f64 tan_f64(f64 val){
    return tan(val);
}

f32 sqrt_f32(f32 val){
    return sqrtf(val);
}

f64 sqrt_f64(f64 val){
    return sqrt(val);
}

f32 atan2_f32(f32 y, f32 x){
    return atan2f(y, x);
}

f64 atan2_f64(f64 y, f64 x){
    return atan2(y, x);
}

f32 dot_2d_f32(f32 lhs_x, f32 lhs_y, f32 rhs_x, f32 rhs_y){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y);   
}

f32 dot_3d_f32(f32 lhs_x, f32 lhs_y, f32 lhs_z, f32 rhs_x, f32 rhs_y, f32 rhs_z){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y) + (lhs_z * rhs_z);   
}

f64 dot_2d_f64(f64 lhs_x, f64 lhs_y, f64 rhs_x, f64 rhs_y){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y);   
}

f64 dot_3d_f64(f64 lhs_x, f64 lhs_y, f64 lhs_z, f64 rhs_x, f64 rhs_y, f64 rhs_z){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y) + (lhs_z * rhs_z);   
}

f32 dot_vector3(Vector3 lhs, Vector3 rhs){
    return dot_3d_f32(lhs.x, lhs.y, lhs.z, rhs.x, rhs.y, rhs.z);
}

// public static Quaternion Normalise(
//     Quaternion q
// ){
//     f32 len = (f32)Sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
//     return new(){X = q.x / len, Y = q.y / len, Z = q.z / len, W = q.w / len};
// }

Quaternion normalise_quaternion(Quaternion q){
    f32 len_sqrd = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    
    // NAN guard.
    if(len_sqrd==0.0f){
        return (Quaternion){0};
    } 

    f32 len = sqrt_f32(len_sqrd);
    q.x /= len;
    q.y /= len;
    q.z /= len;
    q.w /= len;
    return q;
}

Matrix4x4 multiply_matrix4x4(Matrix4x4 lhs, Matrix4x4 rhs){
    Matrix4x4 dst = {0};
    f32* d = dst.m;
    f32* l = lhs.m;
    f32* r = rhs.m;

    d[0]  = (l[0] * r[0]) + (l[4] * r[1]) + (l[8]  * r[2]) + (l[12] * r[3]);
    d[1]  = (l[1] * r[0]) + (l[5] * r[1]) + (l[9]  * r[2]) + (l[13] * r[3]);
    d[2]  = (l[2] * r[0]) + (l[6] * r[1]) + (l[10] * r[2]) + (l[14] * r[3]);
    d[3]  = (l[3] * r[0]) + (l[7] * r[1]) + (l[11] * r[2]) + (l[15] * r[3]);

    d[4]  = (l[0] * r[4]) + (l[4] * r[5]) + (l[8]  * r[6]) + (l[12] * r[7]);
    d[5]  = (l[1] * r[4]) + (l[5] * r[5]) + (l[9]  * r[6]) + (l[13] * r[7]);
    d[6]  = (l[2] * r[4]) + (l[6] * r[5]) + (l[10] * r[6]) + (l[14] * r[7]);
    d[7]  = (l[3] * r[4]) + (l[7] * r[5]) + (l[11] * r[6]) + (l[15] * r[7]);

    d[8]  = (l[0] * r[8]) + (l[4] * r[9]) + (l[8]  * r[10]) + (l[12] * r[11]);
    d[9]  = (l[1] * r[8]) + (l[5] * r[9]) + (l[9]  * r[10]) + (l[13] * r[11]);
    d[10] = (l[2] * r[8]) + (l[6] * r[9]) + (l[10] * r[10]) + (l[14] * r[11]);
    d[11] = (l[3] * r[8]) + (l[7] * r[9]) + (l[11] * r[10]) + (l[15] * r[11]);

    d[12] = (l[0] * r[12]) + (l[4] * r[13]) + (l[8]  * r[14]) + (l[12] * r[15]);
    d[13] = (l[1] * r[12]) + (l[5] * r[13]) + (l[9]  * r[14]) + (l[13] * r[15]);
    d[14] = (l[2] * r[12]) + (l[6] * r[13]) + (l[10] * r[14]) + (l[14] * r[15]);
    d[15] = (l[3] * r[12]) + (l[7] * r[13]) + (l[11] * r[14]) + (l[15] * r[15]);

    return dst;
}

Vector3 sub_vector3(Vector3 lhs, Vector3 rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Vector3 add_vector3(Vector3 lhs, Vector3 rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Vector3 mul_vector3(Vector3 lhs, Vector3 rhs){
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}

Vector3 mul_val_vector3(Vector3 lhs, f32 rhs){
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

Vector3 div_vector3(Vector3 lhs, Vector3 rhs){
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
}
        
Vector2 unary_vector2(Vector2 val){
    val.x *= -1.0f;
    val.y *= -1.0f;
    return val;
}
        
Vector2 add_vector2(Vector2 lhs, Vector2 rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2 sub_vector2(Vector2 lhs, Vector2 rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 mul_vector2(Vector2 lhs, Vector2 rhs){
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

Vector2 div_vector2(Vector2 lhs, Vector2 rhs){
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

Vector3 vector2_to_vector3(Vector2 v){
    return (Vector3){v.x, v.y, 0.0f};
}

Vector2 vector3_to_vector2(Vector3 v){
    return (Vector2){v.x, v.y};
}

Vector2I add_vector2i(Vector2I lhs, Vector2I rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2I sub_vector2i(Vector2I lhs, Vector2I rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

f32 len_sqrd_vector3(Vector3 vector){
    return (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
}

f32 len_vector3(Vector3 vector){
    f32 sqrd = len_sqrd_vector3(vector);
    return sqrd == 0.0f ? 0.0f : sqrt_f32(sqrd);
}

Vector3 cross_vector3(Vector3 a, Vector3 b){
    Vector3 result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

/*
    Rotates a Vector around the origin (0,0,0), by a quaternion rotation.
*/
Vector3 rotate_vector3(Vector3 v, Quaternion q){
    Vector3 q_v;
    q_v.x = q.x;
    q_v.y = q.y;
    q_v.z = q.z;    
    
    Vector3 cross1 = cross_vector3(q_v, v);
    Vector3 cross2 = cross_vector3(q_v, cross1);

    cross1 = mul_val_vector3(cross1, (2.0f * q.w));
    cross2 = mul_val_vector3(cross2, 2.0f);

    Vector3 result;
    result = v;
    result = add_vector3(result, cross1);
    result = add_vector3(result, cross2);
    return result;
}

bool equal_vector2i(Vector2I lhs, Vector2I rhs){
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Quaternion mul_quaternion(Quaternion lhs, Quaternion rhs){
    Quaternion result;
    result.x = (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
    result.y = (lhs.w * rhs.y) - (lhs.x * rhs.z) + (lhs.y * rhs.w) + (lhs.z * rhs.x);
    result.z = (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x) + (lhs.z * rhs.w);
    result.w = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
    return result;
}

Aabb add_vector2_aabb(Aabb aabb, Vector2 vector){
    aabb.min_x += vector.x;
    aabb.min_y += vector.y;
    aabb.max_x += vector.x;
    aabb.max_y += vector.y;
    return aabb;
}

Aabb sub_vector2_aabb(Aabb aabb, Vector2 vector){
    aabb.min_x -= vector.x;
    aabb.min_y -= vector.y;
    aabb.max_x -= vector.x;
    aabb.max_y -= vector.y;
    return aabb;
}

bool equal_aabb(Aabb a, Aabb b){
    return 
    a.min_x == b.min_x 
    && a.min_y == b.min_y
    && a.max_x == b.max_x
    && a.max_y == b.max_y;   
}

/*
    A rotation update using complex number multiplication (rotors)
    and a 4th order Taylor Series expansion for delta trigonometry.

    Remarks:
    This is significantly faster then Vector.SinCos as it avoids
    heavy transcendental instructions.

    `Accuracy`: High for theta < 90 degrees (1.57 radian) per step.
    `Stability`: Includes a renormalization pass to prevent f32ing-poi32 drift
    (scaling/shrinking) over time.
    
    Parameters:
    `sin`: the current sine values.</param>
    `cos`: the current cosing values.</param>
    `theta`: the angular change in radians: E.g. (angularVelocity * deltaTime).</param>
    `new_sine`: output for updated sine values.</param>
    `new_cosine`: oputput for updated cosine values.</param>
*/
void rotor_multiply(f32 sine, f32 cosine, f32 theta, f32* new_sine, f32* new_cosine){
    f32 theta_sqrd = theta * theta;

    // Get Sin/Cos of theta (Small Angle Approximation)
    f32 sine_delta = theta * (1 - (theta_sqrd * ONE_SIXTH));
    f32 cosine_delta = 1 - (theta_sqrd * 0.5f) + (theta_sqrd * theta_sqrd * ONE_TWENTY_FOURTH);

    // Complex Multiplication (identity math)
    // next sin = sin(a)cos(b) + cos(a)sin(b)
    f32 next_sine = (sine * cosine_delta) + (cosine * sine_delta);
    // next cos = cos(a)cos(b) - sin(a)sin(b)
    f32 next_cosine = (cosine * cosine_delta) - (sine * sine_delta);

    // renormalise.
    // Note: f32ing-poi32 numbers are imprecise, which accumulates the more they
    // are operated on. Renormalizing (the inv leng part) force the length back
    // to 1.0, so it doesnt drift and squish or enlargen undeterministically.
    f32 dot = (next_sine * next_sine) + (next_cosine * next_cosine);
    f32 inv_len = 1 / sqrtf(dot);

    // --- NAN PROTECTION ---
    // Define a tiny epsilon to avoid division by zero.        
    if (isnan(inv_len) || 1e-10f > inv_len)
    {
        return;
    }

    *new_sine = next_sine * inv_len;
    *new_cosine = next_cosine * inv_len;
}


/*
    Calculates the sum of all i32egers from <c><paramref name="n"/></c> to 1.

    Remarks: 
    `n` should not be larger than 46430.
*/
i32 calculate_triangular_sum(i32 n){
    DEBUG_ASSERT(n < 46430, "'n' is greater than or equal to 46430.");
    return n * (n+1) / 2;
}

void normalise_2d_f32(f32 x, f32 y, f32* n_x, f32* n_y){
    f32 len_sqrd = x * x + y * y;

    // NAN guard.
    if(len_sqrd == 0.0f){
        *n_x = 0;
        *n_y = 0;
        return;
    }
    
    f32 inv_len = 1.0f / sqrtf(len_sqrd);
    *n_x = x * inv_len;
    *n_y = y * inv_len;
}

void normalise_3d_f32(f32 x, f32 y, f32 z, f32* n_x, f32* n_y, f32* n_z){
    f32 len_sqrd = x * x + y * y + z * z;
    
    // NAN guard.
    if(len_sqrd == 0.0f){
        *n_x = 0;
        *n_y = 0;
        *n_z = 0;
        return;
    }
    
    f32 inv_len = 1.0f / sqrtf(len_sqrd);
    *n_x = x * inv_len;
    *n_y = y * inv_len;
    *n_z = z * inv_len;
}

Vector3 normalise_vector3(Vector3 vector){
    Vector3 result = {0};
    normalise_3d_f32(vector.x, vector.y, vector.z, &result.x, &result.y, &result.z);
    return result;
}

f32 len_sqrd_f32(f32 point_x, f32 point_y){
    return dot_2d_f32(point_x, point_y, point_x, point_y);
}

f32 dst_sqrd_f32(f32 from_x, f32 from_y, f32 to_x, f32 to_y){
    f32 dx = from_x - to_x;
    f32 dy = from_y - to_y;
    return dx * dx + dy * dy;
}

bool near_equal_f32(f32 a, f32 b, f32 epsilon){
    /*
        Note: norm-based comparison enurses
        the epsilon comparison doesnt return false negatives
        at large f32ing poi32 values.            
    */
    f32 diff = ABS(a-b);
    // f32 norm = Max(Abs(a),Abs(b));
    // return diff <= epsilon * Max(1f, norm);
    return diff <= epsilon;
}

f32 to_radians(f32 degrees){
    // PI / 180 = 0.0174532925199f
    return degrees * 0.0174532925199f;
}

void rotate_radians(f32 increment, f32 src_radians, f32* dst_radians, f32* out_sine, f32* out_cosine){
    *dst_radians = src_radians + increment;
    *out_sine = sin_f32(src_radians);
    *out_cosine = cos_f32(src_radians);
}

f32 cross_2d_f32(f32 lhs_x, f32 lhs_y, f32 rhs_x, f32 rhs_y){
    return lhs_x * rhs_y - lhs_y * rhs_x;    
}

/*
    Computes a Left-Handed View Matrix (Column-Major)
*/
Matrix4x4 matrix4x4_create_look_at(Vector3 camera_pos, Vector3 look_at_pos, Vector3 world_up_dir){
    // Left-Handed forward: Positive Z goes forward i32o the screen
    Vector3 forward = normalise_vector3(
        sub_vector3(look_at_pos, camera_pos)
    );

    // find the local space right and up directions of the camera.
    Vector3 right = {0};
    right.x = (world_up_dir.y * forward.z) - (world_up_dir.z * forward.y);
    right.y = (world_up_dir.z * forward.x) - (world_up_dir.x * forward.z); 
    right.z = (world_up_dir.x * forward.y) - (world_up_dir.y * forward.x);
    right = normalise_vector3(right);
    Vector3 up = {0};
    up.x = (forward.y * right.z) - (forward.z * right.y);
    up.y = (forward.z * right.x) - (forward.x * right.z);
    up.z = (forward.x * right.y) - (forward.y * right.x);
    up = normalise_vector3(up);

    Matrix4x4 result = {0};
    f32* m = result.m;

    // Column 0: Right Axis
    m[0] = right.x; 
    m[1] = right.y; 
    m[2] = right.z; 

    // Column 1: Up Axis
    m[4] = up.x; 
    m[5] = up.y; 
    m[6] = up.z; 

    // Column 2: Forward Axis
    m[8]  = forward.x; 
    m[9]  = forward.y; 
    m[10] = forward.z; 

    // Column 3: Calculate the translation; how far along the local-space axis the camera is, and move all objects relative to that.
    // note that is accounts for the camera rotation as well.
    m[12] = -dot_vector3(right, camera_pos);
    m[13] = -dot_vector3(up, camera_pos);
    m[14] = -dot_vector3(forward, camera_pos);
    m[15] = 1.0f;

    return result;
}

/*
    Computes a Left-Handed Perspective Matrix mapping depth to Vulkan 0..1 (Column-Major)
*/
Matrix4x4 create_perspective_matrix4x4(f32 fov_y_radians, f32 aspect_ratio, f32 z_near, f32 z_far){
    /**
        Calculate half of the vertical line that describes the length from the bottom to the top of out total vertical viewing angle;
        
        Note:
            think of it like the fovYRadians is how much your eye (or camera lens) sees, 
            the tangent describes the length from the top to bot.
    **/
    f32 tan_half_fov_y = tan_f32(fov_y_radians * 0.5f);
    /**
        bring the half-total vertical viewing area i32o window-space coordinates:

        Window Space:
            X = -1 (left) to 1 (right).
            Y = -1 (bottom) to 1 (top).
            Z = 0 (clost) to 1 (far).
    **/
    f32 g = 1.0f / tan_half_fov_y;

    Matrix4x4 result = {0};
    f32* m = result.m;

    /**
        shrink the x-scaling of objects relative to the vertical viewing area to ensure that they arent 
        elongated along the x-axis of the window due to window size differences.
    **/
    m[0] = g / aspect_ratio;
    
    /**
        set the y-scaling of objects to be relative to the vertical view of the camera; 
        squishing them down along their y-axis to fit them on the window.
    **/
    m[5] = g;

    /**
        unlike orthographic projection where depth mapping is perfectly linear, a perspective matrix mapping must compress
        depth non-linearly. More precision is allocated to objects close to the lens (zNear), while precision is agressively
        compressed as objects move toward the background (zFar) to preven distant objects from flickering (z-fighting).
        note that it is also 0-1 for modern graphics APIS (metal, vulkan, directX12) unlike opengl (-1 to 0).
    **/
    m[10] = z_far / (z_far - z_near);
    // add the world position of an object to its final 'w' value: perspective divsion (final 'w' division). 
    m[11] = 1.0f;

    /**
        This shears the z-values of objects backward, transforming them i32o window-space coordinates. this "shearing" backwards 
        is required as perspective projection has a plane that rendering is relative to (zNear) which must be above zero; otherwise
        there would be divide by zero issues. So when perspective division happens, we need to offset/shear the plane back to zero to
        ensure that our depth values are correct; offseting m[11]'s added value so its relative to the origin (0,0) and not the plane.  
    **/
    m[14] = -(z_far * z_near) / (z_far - z_near); 

    return result;
}

/*
    Computes a Left-Handed rotation Matrix applied to a source matrix (Column-Major)
*/
Matrix4x4 rotate_matrix4x4(Matrix4x4 src, f32 radians, Vector3 axis){

    // how much the of the objects original orientation is kept along its original axis.
    f32 c = cos_f32(radians);
    // how much of the objects orientation is shifted perpendicularly i32o a new direction.
    f32 s = sin_f32(radians);
    
    axis = normalise_vector3(axis);

    f32 temp_x = (1.0f - c) * axis.x;
    f32 temp_y = (1.0f - c) * axis.y;
    f32 temp_z = (1.0f - c) * axis.z;

    // Left-Handed basis rotation vectors
    Matrix3x3 rot = {0};
    f32* rot_ptr = rot.m;
    rot_ptr[0] = c + temp_x * axis.x;
    rot_ptr[1] = temp_x * axis.y + s * axis.z;
    rot_ptr[2] = temp_x * axis.z - s * axis.y;
    rot_ptr[3] = temp_y * axis.x - s * axis.z;
    rot_ptr[4] = c + temp_y * axis.y;
    rot_ptr[5] = temp_y * axis.z + s * axis.x;
    rot_ptr[6] = temp_z * axis.x + s * axis.y;
    rot_ptr[7] = temp_z * axis.y - s * axis.x;
    rot_ptr[8] = c + temp_z * axis.z;

    Matrix4x4 dst = {0};

    f32* src_ptr = src.m;
    f32* dst_ptr = dst.m;

    // Column 0 concatenation
    dst_ptr[0] = (src_ptr[0] * rot_ptr[0]) + (src_ptr[4] * rot_ptr[3]) + (src_ptr[8]   * rot_ptr[6]);
    dst_ptr[1] = (src_ptr[1] * rot_ptr[0]) + (src_ptr[5] * rot_ptr[3]) + (src_ptr[9]   * rot_ptr[6]);
    dst_ptr[2] = (src_ptr[2] * rot_ptr[0]) + (src_ptr[6] * rot_ptr[3]) + (src_ptr[10]  * rot_ptr[6]);
    dst_ptr[3] = (src_ptr[3] * rot_ptr[0]) + (src_ptr[7] * rot_ptr[3]) + (src_ptr[11]  * rot_ptr[6]);

    // Column 1 concatenation
    dst_ptr[4] = (src_ptr[0] * rot_ptr[1]) + (src_ptr[4] * rot_ptr[4]) + (src_ptr[8]  * rot_ptr[7]);
    dst_ptr[5] = (src_ptr[1] * rot_ptr[1]) + (src_ptr[5] * rot_ptr[4]) + (src_ptr[9]  * rot_ptr[7]);
    dst_ptr[6] = (src_ptr[2] * rot_ptr[1]) + (src_ptr[6] * rot_ptr[4]) + (src_ptr[10] * rot_ptr[7]);
    dst_ptr[7] = (src_ptr[3] * rot_ptr[1]) + (src_ptr[7] * rot_ptr[4]) + (src_ptr[11] * rot_ptr[7]);

    // Column 2 concatenation
    dst_ptr[8]  = (src_ptr[0] * rot_ptr[2]) + (src_ptr[4] * rot_ptr[5]) + (src_ptr[8]  * rot_ptr[8]);
    dst_ptr[9]  = (src_ptr[1] * rot_ptr[2]) + (src_ptr[5] * rot_ptr[5]) + (src_ptr[9]  * rot_ptr[8]);
    dst_ptr[10] = (src_ptr[2] * rot_ptr[2]) + (src_ptr[6] * rot_ptr[5]) + (src_ptr[10] * rot_ptr[8]);
    dst_ptr[11] = (src_ptr[3] * rot_ptr[2]) + (src_ptr[7] * rot_ptr[5]) + (src_ptr[11] * rot_ptr[8]);

    // Column 3 preserves source translations
    dst_ptr[12] = src_ptr[12];
    dst_ptr[13] = src_ptr[13];
    dst_ptr[14] = src_ptr[14];
    dst_ptr[15] = src_ptr[15];

    return dst;
}

/*
    Computes a Left-Handed Orthographic Matrix mapping depth to Vulkan 0..1 (Column-Major)

    Parameters:
    `lowerX`: the lower-bound x-value of the camera resolution in pixels; e.g, 0.
    `upperX`: the upper-bound x-value of the camera resolution in pixels; e.g, 1920.
    `lowerY`: the lower-bound y-value of the camera resolution in pixels; e.g, 0.
    `upperY`: the upper-bound y-value of the camera resolution in pixels; e.g, 1080.
*/
Matrix4x4 create_orthographic_matrix4x4(f32 lower_x, f32 upper_x, f32 lower_y, f32 upper_y, f32 z_near, f32 z_far){
    Matrix4x4 result = {0};

    // calculate the absolute width, height and depth of the viewing frustrum (box) to display to the screen.
    f32 x_range = upper_x - lower_x;
    f32 y_range = upper_y - lower_y;
    f32 z_range = z_far - z_near; 

    f32* m = result.m;

    /**
        scale objects from screen space i32o the graphics api's window-space.

        Window Space:
            X = -1 (left) to 1 (right).
            Y = -1 (bottom) to 1 (top).
            Z = 0 (close) to 1 (far).
    **/
    m[0] = 2.0f / x_range;
    m[5] = 2.0f / y_range;
    m[10] = 1.0f / z_range; 

    /**
        if left and right are symmetrical (e.g, -400 to 400) then right + left equals 0 (center of window-space)
        however if they are assymetrical (e.g, 0 to 1920) then the viewing frustum doesnt align with the center of window-space.

        these translations below shift the entire coordinate system horizontally and vertically so that whatever those values are
        it will always line up exactly with the center of window-space.
    **/
    m[12] = -(upper_x + lower_x) / x_range;
    m[13] = -(upper_y + lower_y) / y_range;
    // note that this is 0-1 for modern graphics APIS (metal, vulkan, directX12) unlike opengl (-1 to 0).
    m[14] = -z_near / z_range; 

    // homogenous coordinate value.
    m[15] = 1.0f;

    return result;
}

/*
    Creates a col-major matrix from a transform.
*/
Matrix4x4 create_matrix4x4_from_transform(Transform transform){
    Matrix4x4 result = {0};
    f32* m = result.m;

    // Pre-calculate squared terms for the quaternion rotation
    f32 x2 = transform.rotation.x + transform.rotation.x; 
    f32 y2 = transform.rotation.y + transform.rotation.y; 
    f32 z2 = transform.rotation.z + transform.rotation.z;
    f32 xx = transform.rotation.x * x2; 
    f32 xy = transform.rotation.x * y2; 
    f32 xz = transform.rotation.x * z2;
    f32 yy = transform.rotation.y * y2; 
    f32 yz = transform.rotation.y * z2; 
    f32 zz = transform.rotation.z * z2;
    f32 wx = transform.rotation.w * x2; 
    f32 wy = transform.rotation.w * y2; 
    f32 wz = transform.rotation.w * z2;

    // --- COLUMN 0 (X-Basis * ScaleX) ---
    m[0] = (1.0f - (yy + zz)) * transform.scale.x;
    m[1] = (xy + wz) * transform.scale.x;
    m[2] = (xz - wy) * transform.scale.x;
    m[3] = 0.0f;

    // --- COLUMN 1 (Y-Basis * ScaleY) ---
    m[4] = (xy - wz) * transform.scale.y;
    m[5] = (1.0f - (xx + zz)) * transform.scale.y;
    m[6] = (yz + wx) * transform.scale.y;
    m[7] = 0.0f;

    // --- COLUMN 2 (Z-Basis * ScaleZ) ---
    m[8] = (xz + wy) * transform.scale.z;
    m[9] = (yz - wx) * transform.scale.z;
    m[10] = (1.0f - (xx + yy)) * transform.scale.z;
    m[11] = 0.0f;

    // --- COLUMN 3 (Translation) ---
    m[12] = transform.position.x;
    m[13] = transform.position.y;
    m[14] = transform.position.z;
    m[15] = 1.0f;

    return result;
}

Quaternion rotate_quaternion(Quaternion q, f32 axis_x, f32 axis_y, f32 axis_z, f32 angle_radians){
    // 1. Normalize the axis vector to ensure safe rotation math
    f32 length = sqrt_f32(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    if (length < 0.0001f) return q; // Avoid division by zero
    
    axis_x /= length;
    axis_y /= length;
    axis_z /= length;

    // 2. Compute half-angles for the rotation representation
    f32 half_angle = angle_radians * 0.5f;
    f32 sin_half = sin_f32(half_angle);
    f32 cos_half = cos_f32(half_angle);

    // 3. Construct the 'new' rotation quaternion
    f32 new_x = axis_x * sin_half;
    f32 new_y = axis_y * sin_half;
    f32 new_z = axis_z * sin_half;
    f32 new_w = cos_half;

    // 4. Combine via multiplication (Local space: Original * New)
    f32 final_x = q.w * new_x + q.x * new_w + q.y * new_z - q.z * new_y;
    f32 final_y = q.w * new_y - q.x * new_z + q.y * new_w + q.z * new_x;
    f32 final_z = q.w * new_z + q.x * new_y - q.y * new_x + q.z * new_w;
    f32 final_w = q.w * new_w - q.x * new_x - q.y * new_y - q.z * new_z;

    // 5. Return the finalized normalized rotation
    Quaternion result = {.x = final_x, .y = final_y, .z = final_z, .w = final_w};
    result = normalise_quaternion(result);
    return result;
}

/*
    Creates a rotation quaternion around a normalised axis vector by a given angle in radians.
*/
Quaternion create_from_axis_angle_quaternion(Vector3 axis, f32 angle){
    
    // Half angle calculations required by quaternion space
    f32 half_angle = angle * 0.5f;
    f32 sin = sin_f32(half_angle);
    f32 cos = cos_f32(half_angle);

    // Scale the normalized directional axis components by the sine projection
    Quaternion result;
    result.x = axis.x * sin,
    result.y = axis.y * sin,
    result.z = axis.z * sin,
    result.w = cos;
    return result;
}

Quaternion get_rotation_between_poi32s(Vector3 poi32_a, Vector3 poi32_b){
    // Get the direction vector target poi32ing from A to B
    Vector3 delta;
    delta.x = poi32_b.x - poi32_a.x; 
    delta.y = poi32_b.y - poi32_a.y; 
    delta.z = poi32_b.z - poi32_a.z;

    Vector3 direction = normalise_vector3(delta);
    
    // Define the default local resting axis (e.g., Vector3(0, 1, 0) if poi32ing Up)
    Vector3 starting_axis = VECTOR3_UP; 

    f32 dot = dot_vector3(starting_axis, direction);

    // Edge Case Handling: Check if target vectors poi32 directly opposite to prevent a divide-by-zero crash (180 deg)
    if (dot < -0.99999f){
        
        // Pick an arbitrary perpendicular backup axis to rotate around instead
        Vector3 perpendicular = cross_vector3(starting_axis, VECTOR3_RIGHT);
        if (len_sqrd_vector3(perpendicular) < 0.001f){
            perpendicular = cross_vector3(starting_axis, VECTOR3_FORWARD);
        }
        
        return create_from_axis_angle_quaternion(normalise_vector3(perpendicular), PI);
    }
    
    // Edge Case Handling: Vectors already poi32 in the identical direction
    if (dot > 0.99999f){
        return QUATERNION_IDENTITY;
    }

    // Shortest arc computation mapping directly onto the native components
    Vector3 axis = cross_vector3(starting_axis, direction);
    
    Quaternion result;
    result.x = axis.x;
    result.y = axis.y;
    result.z = axis.z;
    result.w = 1.0f + dot; // W component maps directly to the cosine length offset prior to normalization.
    normalise_quaternion(result);
    
    return result;
}

f32 dist_sqrd_2d_f32(f32 from_x, f32 from_y, f32 to_x, f32 to_y){
    f32 dx = from_x - to_x;
    f32 dy = from_y - to_y; 
    return dx * dx + dy * dy;
}

f32 dist_2d_f32(f32 from_x, f32 from_y, f32 to_x, f32 to_y){
    f32 sqrd = dist_sqrd_2d_f32(from_x, from_y, to_x, to_y);
    
    // NAN guard.
    if(sqrd <= 0.0f){
        return 0.0f;
    }

    return sqrt_f32(sqrd);
}

f32 dist_sqrd_vector2(Vector2 from, Vector2 to){
    return dist_sqrd_2d_f32(from.x, from.y, to.x, to.y);
}

f32 dist_vector2(Vector2 from, Vector2 to){
    return dist_2d_f32(from.x, from.y, to.x, to.y);
}
void transform_scalar_2d_f32(
    f32 x, f32 y, f32 transform_scale_x, f32 transform_scale_y, f32 transform_cos, f32 transform_sin, 
    f32 transform_position_x, f32 transform_position_y, f32* out_x, f32* out_y
){
    // NOTE:
    // This ordering: Scale -> rotation -> Translation
    // should remain the same. It is pretty much Matrix math.

    // Scale:
    f32 sx = x * transform_scale_x;
    f32 sy = y * transform_scale_y; 

    // rotation:
    f32 rx = sx * transform_cos - sy * transform_sin;
    f32 ry = sx * transform_sin + sy * transform_cos;

    // Translation:
    *out_x = rx + transform_position_x;
    *out_y = ry + transform_position_y;
}

Vector2 transform_vector2(Vector2 v, Transform2D t){
    transform_scalar_2d_f32(    
        v.x, v.y, t.scale.x, t.scale.y, t.cosine, t.sine, 
        t.position.x, t.position.y, &v.x, &v.y
    );
    return v;
}

bool init_fssoa_vector2(FsSoa_Vector2* soa, MemoryArena* arena, i32 entry_stride, i32 max_entries){
    if(soa->is_init){
        DEBUG_ASSERT(!soa->is_init, "attempted to init an already init FsSoa_Vector2.");
        return false;
    }
    soa->is_init = true;
    i32 array_size = entry_stride*max_entries;
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->x, &soa->x_size, array_size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->y, &soa->y_size, array_size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->append_counts, &soa->append_counts_size, array_size);
    soa->entry_stride = entry_stride;
    soa->max_entries = max_entries;
    return true;
}

bool append_fssoa_vector2(FsSoa_Vector2* soa, i32 entry_index, f32 x, f32 y){
    // ensure that the entry slot isnt full.
    i32 append_count = soa->append_counts[entry_index];
    if(append_count >= soa->entry_stride){
        DEBUG_ASSERT(0!=0, "attempted to append to a full fssoa_vector2.");
        return false;
    }
    i32 append_index = entry_index * soa->entry_stride + append_count;

    // set the value.
    soa->x[append_index] = x;
    soa->y[append_index] = y;

    // increment append index.
    soa->append_counts[entry_index]++;
    return true;
}

/*
    Sets the append count of an entry to zero in a fixed stride soa instance.
*/
void clear_entry_append_count_fssoa_vector2(FsSoa_Vector2* soa, i32 entryIndex){
    BNDS_CHCK(entryIndex, soa->append_counts_size);
    soa->append_counts[entryIndex] = 0;
}

void clear_append_counts_fssoa_vector2(FsSoa_Vector2* soa){
    for(i32 i = 0; i < soa->append_counts_size; i++){
        soa->append_counts[i] = 0;
    }
}

bool init_soa_vector2(Soa_Vector2* soa, MemoryArena* arena, i32 size){
    
    if (soa->is_init){
        DEBUG_ASSERT(0!=0, "attempted to init an already init soa_vector2.");
        return false;
    }

    soa->is_init = true;
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->x, &soa->size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->y, &soa->size, size);
    return true;
}

void insert_soa_vector2(Soa_Vector2* soa, i32 insert_index, f32 x, f32 y){
    BNDS_CHCK(insert_index, soa->size);
    soa->x[insert_index] = x;
    soa->y[insert_index] = y;
}

bool append_soa_vector2(Soa_Vector2* soa, f32 x, f32 y){

    if(soa->append_count >= soa->size){
        DEBUG_ASSERT(0!=0, "attempted to append to a full soa_vector2");
        return false;
    }

    insert_soa_vector2(soa, soa->append_count, x, y);
    soa->append_count++;
    return true;
}

void reset_count_soa_vector2(Soa_Vector2* soa){
    soa->append_count = 0;
}

bool init_soa_transform2d(Soa_Transform2D* soa, MemoryArena* arena, i32 size){
    if(soa->is_init){
        DEBUG_ASSERT(0!=0, "attempted to init already init soa_transform2d.");
        return false;
    }

    soa->is_init = true;
    init_soa_vector2(&soa->positions, arena, size);
    init_soa_vector2(&soa->scales, arena, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->sines, &soa->sines_size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->cosines, &soa->cosines_size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->rotation_radii, &soa->rotation_radii_size, size);
    return true;
}

void copy_elem_from_soa_transform2d(Soa_Transform2D* soa, Transform2D* dst, i32 index){
    
    BNDS_CHCK(index, soa->positions.size);
    dst->position.x = soa->positions.x[index];
    dst->position.y = soa->positions.y[index];
    
    BNDS_CHCK(index, soa->scales.size);
    dst->scale.x = soa->scales.x[index];
    dst->scale.y = soa->scales.y[index];
    
    BNDS_CHCK(index, soa->sines_size);
    dst->sine = soa->sines[index];
    
    BNDS_CHCK(index, soa->cosines_size);
    dst->cosine = soa->cosines[index];

    BNDS_CHCK(index, soa->rotation_radii_size);
    dst->rotation_radii = soa->rotation_radii[index];
}

void insert_scalar_soa_transform2d(
    Soa_Transform2D* soa, i32 elem_index, f32 pos_x, f32 pos_y, 
    f32 scale_x, f32 scale_y, f32 sin, f32 cos, f32 rot_radians
){
    BNDS_CHCK(elem_index, soa->positions.size);
    soa->positions.x[elem_index] = pos_x;
    soa->positions.y[elem_index] = pos_y;

    BNDS_CHCK(elem_index, soa->scales.size);
    soa->scales.x[elem_index] = scale_x;
    soa->scales.y[elem_index] = scale_y;

    BNDS_CHCK(elem_index, soa->sines_size);
    soa->sines[elem_index] = sin;

    BNDS_CHCK(elem_index, soa->cosines_size);
    soa->cosines[elem_index] = cos;

    BNDS_CHCK(elem_index, soa->rotation_radii_size);
    soa->rotation_radii[elem_index] = rot_radians;
}

void insert_soa_transform2d(Soa_Transform2D* soa, Transform2D transform, i32 elem_index){
    insert_scalar_soa_transform2d(
        soa, elem_index, transform.position.x, transform.position.y, 
        transform.scale.x, transform.scale.y, transform.sine, transform.cosine, transform.rotation_radii
    );
}

// public static void TransformRelative(Soa_Transform2D src, Soa_Transform2D dst, i32 srcReadIndex, i32 dstWriteIndex, 
//     f32 worldPosX, f32 worldPosY, f32 worldScaleX, f32 worldScaleY, f32 worldSine, f32 worldCosine, f32 worldrotationRadians
// )
// {
//     TransformRelative(src.positions.x[srcReadIndex], src.positions.y[srcReadIndex], src.scales.x[srcReadIndex], 
//         src.scales.y[srcReadIndex], src.sines[srcReadIndex], src.cosines[srcReadIndex], src.rotation_radii[srcReadIndex], 
//         worldPosX, worldPosY, worldScaleX, worldScaleY, worldSine, worldCosine, worldrotationRadians, 
//         ref dst.positions.x[dstWriteIndex], ref dst.positions.y[dstWriteIndex], ref dst.scales.x[dstWriteIndex], 
//         ref dst.scales.y[dstWriteIndex], ref dst.sines[dstWriteIndex], ref dst.cosines[dstWriteIndex], 
//         ref dst.rotation_radii[dstWriteIndex] 
//     );
// }

Transform2D transform_to_transform2d(Transform transform){
    
    // Extract Roll (rotation around Z-axis) from Quaternion
    f32 num1 = (transform.rotation.w * transform.rotation.z) + (transform.rotation.x * transform.rotation.y);
    f32 num2 = (transform.rotation.y * transform.rotation.y) + (transform.rotation.z * transform.rotation.z);
    f32 rot_rad = atan2_f32(2.0f * num1, 1.0f - (2.0f * num2));

    Transform2D result;
    result.position.x = transform.position.x;
    result.position.y = transform.position.y;
    result.scale.x = transform.scale.x;
    result.scale.y = transform.scale.y;
    result.sine = sin_f32(rot_rad);
    result.cosine = cos_f32(rot_rad);
    result.rotation_radii = rot_rad;
    return result;
}

Transform2D rotate_transform2d(Transform2D transform, f32 radians){
    transform.rotation_radii += radians;
    transform.sine = sin_f32(transform.rotation_radii);
    transform.cosine = cos_f32(transform.rotation_radii);
    return transform;
}

void transform_scalar_transform2d(
    f32 lhs_pos_x, f32 lhs_pos_y, f32 lhs_scale_x, f32 lhs_scale_y, f32 lhs_sine, f32 lhs_cosine, f32 lhs_rot_radii, 
    f32 rhs_pos_x, f32 rhs_pos_y, f32 rhs_scale_x, f32 rhs_scale_y, f32 rhs_sine, f32 rhs_cosine, f32 rhs_rot_radii, 
    f32* out_pos_x, f32* out_pos_y, f32* out_scale_x, f32* out_scale_y, f32* out_sine, f32* out_cosine, f32* out_rot_radii 
){
    // scale the local offset relative to the world.
    f32 sx = lhs_pos_x * rhs_scale_x;
    f32 sy = lhs_pos_y * rhs_scale_y;
    
    // rotate the local scaled offset around the parents origin.
    //      Standard 2D rotation matrix formula:
    //          x' = x * cos - y * sin
    //          y' = x * sin + y * cos
    f32 rx = (sx * rhs_cosine) - (sy * rhs_sine);
    f32 ry = (sx * rhs_sine) + (sy * rhs_cosine);

    // translate the rotated offset to the world position.
    *out_pos_x = rx + rhs_pos_x;
    *out_pos_y = ry + rhs_pos_y;

    // combine the scale properties
    *out_scale_x = lhs_scale_x * rhs_scale_x;
    *out_scale_y = lhs_scale_y * rhs_scale_y;

    // combine the rotation properties.
    //      Use the trigonometrix identity formulas for combining angles:
    //          sin(a + b) = sin(a)cos(b) + cos(a)sin(b)
    //          cos(a + b) = cos(a)cos(b) - sin(a)sin(b)
    *out_sine = (lhs_sine * rhs_cosine) + (lhs_cosine * rhs_sine);
    *out_cosine = (lhs_cosine * rhs_cosine) + (lhs_sine * rhs_sine);
    *out_rot_radii = lhs_rot_radii + rhs_rot_radii;
}

Transform2D transform_transform2d(Transform2D lhs, Transform2D rhs){
    Transform2D res;
    transform_scalar_transform2d(
        lhs.position.x, lhs.position.y, lhs.scale.x, lhs.scale.y, lhs.sine, lhs.cosine, lhs.rotation_radii, 
        rhs.position.x, rhs.position.y, rhs.scale.x, rhs.scale.y, rhs.sine, rhs.cosine, rhs.rotation_radii, 
        &res.position.x, &res.position.y, &res.scale.x, &res.scale.y, &res.sine, &res.cosine, &res.rotation_radii
    );
    return res;
}

Transform to_transform_transform2d(Transform2D transform2D){
    // Create a 3D Quaternion rotating only around the Z axis
    Quaternion rotation = create_from_axis_angle_quaternion(VECTOR3_FORWARD, transform2D.rotation_radii);
    Transform transform;
    transform.position.x = transform2D.position.x;
    transform.position.y = transform2D.position.y;
    transform.position.z = 0;
    transform.scale.x = transform2D.scale.x;
    transform.scale.y = transform2D.scale.y;
    transform.scale.z = 0;
    transform.rotation = rotation;
    return transform;
}

Transform transform_transform(Transform lhs, Transform rhs){
    Transform result;
    // combine scales.
    result.scale = mul_vector3(lhs.scale, rhs.scale);
    // combine rotations (order matters: rhs*lhs means rhs rotates lhs)
    result.rotation = mul_quaternion(lhs.rotation, rhs.rotation);
    // combine positions (order matters: scale->rotate->translate).
    Vector3 sp = mul_vector3(lhs.position, rhs.scale);
    result.position = rotate_vector3(sp, rhs.rotation);
    result.position = add_vector3(result.position, rhs.position); 
    return result;
}

void closest_point_scalar_line_segment(
    f32 line_start_x, f32 line_start_y, f32 line_end_x, f32 line_end_y, 
    f32 query_point_x, f32 query_point_y, f32* out_closest_point_x, f32* out_closest_point_y
){
    f32 line_dist_x = line_end_x - line_start_x;
    f32 line_dist_y = line_end_y - line_start_y;
    f32 point_dist_x = query_point_x - line_start_x;
    f32 point_dist_y = query_point_y - line_start_y;
    
    f32 projection = dot_2d_f32(point_dist_x, point_dist_y, line_dist_x, line_dist_y);

    // move the point distance along the line segment.
    f32 delta = projection / len_sqrd_f32(line_dist_x, line_dist_y);

    if(delta <= 0){
        *out_closest_point_x = line_start_x;
        *out_closest_point_y = line_start_y;
    }
    else if(delta >= 1){
        *out_closest_point_x = line_end_x;
        *out_closest_point_y = line_end_y;
    }
    else{
        *out_closest_point_x = line_start_x + line_dist_x * delta;
        *out_closest_point_y = line_start_y + line_dist_y * delta;
    }
}

/*
    calculates the closest point along a line segmenet towards the query point;
    with the inclusion of the squared distance FROM the query point TO the closest point.
*/
void closest_point_with_sqrd_dist_scalar_line_segment(
    f32 line_start_x, f32 line_start_y, f32 line_end_x, f32 line_end_y, 
    f32 query_point_x, f32 query_point_y, f32* closest_point_x, f32* closest_point_y, 
    f32* dist_sqrd
){
    closest_point_scalar_line_segment(line_start_x, line_start_y, line_end_x, line_end_y, query_point_x, query_point_y, closest_point_x, closest_point_y);
    *dist_sqrd = dist_sqrd_2d_f32(query_point_x, query_point_y, *closest_point_x, *closest_point_y);
}

/*
    calculates the closest point along a line segmenet towards the query point.
*/
Vector2 closest_point_line_segmenet(Vector2 line_start, Vector2 line_end, Vector2 query_point){
    Vector2 result;
    closest_point_scalar_line_segment(
        line_start.x, line_start.y,
        line_end.x, line_end.y,
        query_point.x, query_point.y,
        &result.x, &result.y
    );
    return result;
}

/*
    calculates the closest point along a line segmenet towards the query point;
    with the inclusion of the squared distance FROM the query point TO the closest point.
*/
void closest_point_with_sqrd_dist_line_segment(Vector2 line_start, Vector2 line_end, Vector2 query_point, Vector2* out_closest_point, f32* out_dist_sqrd){
    closest_point_with_sqrd_dist_scalar_line_segment(
        line_start.x, line_start.y,
        line_end.x, line_end.y,
        query_point.x, query_point.y,
        &out_closest_point->x, &out_closest_point->y, out_dist_sqrd
    );
}

bool init_soa_aabb(Soa_Aabb* soa, MemoryArena* arena, i32 size){
    
    if(soa->is_init){
        DEBUG_ASSERT(0!=0, "attempted to init an already init soa_aabb");
        return false;
    }

    soa->is_init = true;
    
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->max_x, &soa->size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->max_y, &soa->size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->min_x, &soa->size, size);
    ALLOC_ARRAY_MEMORY_ARENA(arena, soa->min_y, &soa->size, size);
    return true;
}

void insert_soa_aabb(Soa_Aabb* soa, i32 elem_index, f32 min_x, f32 min_y, f32 max_x, f32 max_y){
    BNDS_CHCK(elem_index, soa->size);
    soa->min_x[elem_index] = min_x;
    soa->min_y[elem_index] = min_y;
    soa->max_x[elem_index] = max_x;
    soa->max_y[elem_index] = max_y;   
}

bool append_soa_aabb(Soa_Aabb* soa, f32 min_x, f32 min_y, f32 max_x, f32 max_y){
    
    if(soa->append_count >= soa->size){
        DEBUG_ASSERT(0!=0, "attempted to append to a full soa_aabb");
        return false;
    }

    insert_soa_aabb(soa, soa->append_count, min_x, min_y, max_x, max_y);
    soa->append_count++;
    return true;
}

void reset_count_soa_aabb(Soa_Aabb* soa){
    soa->append_count = 0;
}

void calculate_centroids_soa_aabb(Soa_Aabb* soa, f32* out_x, f32* out_y){
    
    simd_add_f32(soa->max_x, soa->min_x, out_x, soa->size);
    simd_mul_val_f32(out_x, 0.5f, out_x, soa->size);
    
    simd_add_f32(soa->max_y, soa->min_y, out_y, soa->size);
    simd_mul_val_f32(out_y, 0.5f, out_y, soa->size);
}

f32 get_height_aabb(Aabb aabb){
    return aabb.max_y - aabb.min_y;
}

f32 get_width_aabb(Aabb aabb){
    return aabb.max_x - aabb.min_x;
}


void calc_centroid_scalar_aabb(f32 min_x, f32 min_y, f32 max_x, f32 max_y, f32* out_center_x, f32* out_center_y){
    *out_center_x = (max_x + min_x) * 0.5f;
    *out_center_y = (max_y + min_y) * 0.5f;
}

Vector2 calc_centroid_aabb(Aabb aabb){
    Vector2 result;
    calc_centroid_scalar_aabb(aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y, &result.x, &result.y);
    return result;
}

Vector2 get_min_vector_aabb(Aabb aabb){
    Vector2 result;
    result.x = aabb.min_x;
    result.y = aabb.min_y;
    return result;
}

Vector2 get_max_vector_aabb(Aabb aabb){
    Vector2 result;
    result.x = aabb.max_x;
    result.y = aabb.max_y;
    return result;
}

bool is_overlapping_scalar_aabb(f32 a_min_x, f32 b_min_x, f32 a_min_y, f32 b_min_y, f32 a_max_x, f32 b_max_x, f32 a_max_y, f32 b_max_y){
    if(a_max_x <= b_min_x || b_max_x <= a_min_x){
        return false;
    }
    if (a_max_y <= b_min_y || b_max_y <= a_min_y){
        return false;
    }

    return true;
}

bool is_overlapping_aabb(Aabb a, Aabb b){
    return is_overlapping_scalar_aabb(a.min_x, b.min_x, a.min_y, b.min_y, a.max_x, b.max_x, a.max_y, b.max_y);
}

bool is_overlapping_point_scalar_aabb(f32 aabbMinX, f32 aabbMinY, f32 aabbMaxX, f32 aabbMaxY, f32 poi32X, f32 poi32Y){
    return 
    aabbMinX <= poi32X &&
    aabbMinY <= poi32Y && 
    aabbMaxX >= poi32X &&
    aabbMaxY >= poi32Y;
}

bool is_overlapping_point_aabb(Aabb aabb, Vector2 point){
    return is_overlapping_point_scalar_aabb(
        aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y,
        point.x, point.y
    );
}

bool is_overlapping_line_scalar_aabb(
    f32 aabb_min_x, f32 aabb_min_y, f32 aabb_max_x, f32 aabb_max_y,
    f32 line_start_x, f32 line_start_y, f32 line_end_x, f32 line_end_y
){
    f32 closest_point_x;
    f32 closest_point_y;

    closest_point_scalar_line_segment(line_start_x, line_start_y, line_end_x, line_end_y, aabb_min_x, aabb_min_y, &closest_point_x, &closest_point_y);

    if(is_overlapping_point_scalar_aabb(aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y, closest_point_x, closest_point_y)){

        closest_point_scalar_line_segment(line_start_x, line_start_y, line_end_x, line_end_y, aabb_max_x, aabb_max_y, &closest_point_x, &closest_point_y);
        
        if(is_overlapping_point_scalar_aabb(aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y, closest_point_x, closest_point_y)){
            return true;
        }
    }
    return false;
}

bool is_overlapping_line_aabb(Aabb aabb, Vector2 line_start, Vector2 line_end){
    return is_overlapping_line_scalar_aabb(
        aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y,
        line_start.x, line_start.y, line_end.x, line_end.y
    );
}

/*
    creates an AABB of the greatest max and lowest min vector.
*/
void combine_scalar_aabb(
    f32 a_min_x, f32 a_min_y, f32 a_max_x, f32 a_max_y,
    f32 b_min_x, f32 b_min_y, f32 b_max_x, f32 b_max_y,
    f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y
){
    *out_min_x = MIN(a_min_x, b_min_x);
    *out_min_y = MIN(a_min_y, b_min_y);
    *out_max_x = MAX(a_max_x, b_max_x);
    *out_max_y = MAX(a_max_y, b_max_y);
}

/*
    creates an AABB of the greatest max and lowest min vector.
*/
Aabb combine_aabb(Aabb a, Aabb b){
    Aabb result;
    combine_scalar_aabb(
        a.min_x, a.min_y, a.max_x, a.max_y,
        b.min_x, b.min_y, b.max_x, b.max_y,
        &result.min_x, &result.min_y,
        &result.max_x, &result.max_y    
    );
    return result;
}

/*
    finds the closest vertex on a polygon to a given position and returns its index.
*/
i32 find_closest_vertex_scalar_polygon(f32 query_pos_x, f32 query_pos_y, f32* verts_x, f32* verts_y, i32 verts_size){
    
    i32 result = 0;
    f32 min_dist = F32_MAX;
    
    for(i32 i = 0; i < verts_size; i++){
        f32 distance = dist_sqrd_2d_f32(verts_x[i], verts_y[i], query_pos_x, query_pos_y);

        if(distance <= min_dist){
            min_dist = distance;
            result = (i32)i;
        }
    }

    return result; 
}

/*
    finds the closest vertex on a polygon to a given position and returns its index.
*/
i32 find_closest_vertex_polygon(Vector2 query_pos, f32* verts_x, f32* verts_y, i32 verts_size){
    return find_closest_vertex_scalar_polygon(query_pos.x, query_pos.y, verts_x, verts_y, verts_size);
}

/*
    calculates the centroid of a convex or concave polygon using the shoelace formula.

    remarks:
    none of the polygon edges can be self-intersecting; otherwise the calculation will be wrong.
*/
void calc_centroid_scalar_polygon(f32* verts_x, f32* verts_y, i32 verts_size, f32* out_centroid_x, f32* out_centroid_y){
    f32 area = 0;
    f32 inv_area = 0;
    f32 temp_x = 0;
    f32 temp_y = 0;
    f32 x0 = 0;
    f32 y0 = 0;
    f32 x1 = 0;
    f32 y1 = 0;
    f32 cross_prod;
    i32 next_index;
    bool next_in_range;

    for(i32 i = 0; i < verts_size; i++){
        next_index = i + 1;
        next_in_range = next_index < verts_size;

        // get curernt vertex and the next one.
        x0 = verts_x[i];
        y0 = verts_y[i];
        if (next_in_range){
            x1 = verts_x[next_index];
            y1 = verts_y[next_index];    
        }
        else{
            x1 = verts_x[0];
            y1 = verts_y[0];    
        }

        // calculate the corss product (signed area of the triangle)
        // this is the "Shoelace" part.
        cross_prod = cross_2d_f32(x0, y0, x1, y1);

        area += cross_prod;
        temp_x += (x0 + x1) * cross_prod;
        temp_y += (y0 + y1) * cross_prod;
    }

    area *= 0.5f; // final signed area.

    if(ABS(area) > F32_EPSILON){
        inv_area = 1.0f / (area * 6.0f); 
        *out_centroid_x = temp_x * inv_area;
        *out_centroid_y = temp_y * inv_area;
    }
    else{
        // if the area is 0, the polygon is degenerate (a line or poi32)
        *out_centroid_x = verts_x[0];
        *out_centroid_y = verts_y[0];
    }
}

Vector2 calc_centroid_polygon(f32* verts_x, f32* verts_y, i32 verts_size){
    Vector2 result;
    calc_centroid_scalar_polygon(verts_x, verts_y, verts_size, &result.x, &result.y);
    return result;
}

void get_min_max_vertices_scalar_circle(f32 x, f32 y, f32 radius, f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y){
    *out_min_x = x - radius;
    *out_min_y = y - radius;
    *out_max_x = x + radius;
    *out_max_y = y + radius;
}

f32 get_area_circle(f32 radius){
    return radius * radius * PI;
}

void init_from_verts_polygon_rectangle(PolygonRectangle* rect, Vector2 vert0, Vector2 vert1, Vector2 vert2, Vector2 vert3){
    rect->vertices_x[0] = vert0.x;
    rect->vertices_y[0] = vert0.y;
    rect->vertices_x[1] = vert1.x;
    rect->vertices_y[1] = vert1.y;
    rect->vertices_x[2] = vert2.x;
    rect->vertices_y[2] = vert2.y;
    rect->vertices_x[3] = vert3.x;
    rect->vertices_y[3] = vert3.y;
}

void init_polygon_rectangle(PolygonRectangle* rect, f32 x, f32 y, f32 width, f32 height){
    f32 left = x;
    f32 top = y;
    f32 right = x+width;
    f32 bottom = y-height;

    // top left.
    rect->vertices_x[0] = left;
    rect->vertices_y[0] = top;
    // top right.
    rect->vertices_x[1] = right;
    rect->vertices_y[1] = top;
    // bottom right.
    rect->vertices_x[2] = right;
    rect->vertices_y[2] = bottom;
    // bottom left.
    rect->vertices_x[3] = left;
    rect->vertices_y[3] = bottom;
}

PolygonRectangle rectangle_to_polygon_rectangle(Rectangle rect){
    PolygonRectangle poly;
    init_polygon_rectangle(&poly, rect.x, rect.y, rect.width, rect.height);
    return poly;
}

PolygonRectangle transform_polygon_rectangle(PolygonRectangle rect, Transform2D transform){
    PolygonRectangle result;
    
    Vector2 v0 = {rect.vertices_x[0], rect.vertices_y[0]};
    v0 = transform_vector2(v0, transform);

    Vector2 v1 = {rect.vertices_x[1], rect.vertices_y[1]};
    v1 = transform_vector2(v1, transform);

    Vector2 v2 = {rect.vertices_x[2], rect.vertices_y[2]};
    v2 = transform_vector2(v2, transform);

    Vector2 v3 = {rect.vertices_x[3], rect.vertices_y[3]};
    v3 = transform_vector2(v3, transform);

    init_from_verts_polygon_rectangle(&result, v0, v1, v2, v3);
    return result;
}

void calc_centroid_scalar_polygon_rectangle(PolygonRectangle rect, f32* out_centroid_x, f32* out_centroid_y){
    calc_centroid_scalar_polygon(rect.vertices_x, rect.vertices_y, 4, out_centroid_x, out_centroid_y);
}

Vector2 calc_centroid_polygon_rectangle(PolygonRectangle rect){
    return calc_centroid_polygon(rect.vertices_x, rect.vertices_y, 4);
}

void get_min_max_vectors_scalar_polygon(
    f32* verts_x, f32* verts_y, i32 verts_size, 
    f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y 
){
    *out_min_x = F32_MAX;
    *out_min_y = F32_MAX;
    *out_max_x = F32_MIN;
    *out_max_y = F32_MIN;

    for(i32 i = 0; i < verts_size; i++){
        f32 v = verts_x[i];
        if (v < *out_min_x){
            *out_min_x = v;
        }
        if(v > *out_max_x){
            *out_max_x = v;
        }
    }

    for(i32 i = 0; i < verts_size; i++){
        f32 v = verts_y[i];
        if(v < *out_min_y){
            *out_min_y = v;
        }
        if(v > *out_max_y){
            *out_max_y = v;
        }
    }
}

Aabb calc_aabb_polygon_rectangle(PolygonRectangle rect){
    Aabb aabb;
    get_min_max_vectors_scalar_polygon(
        rect.vertices_x, rect.vertices_y, 4, 
        &aabb.min_x, &aabb.min_y, &aabb.max_x, &aabb.max_y
    );
    return aabb;
}

f32 get_width_polygon_rectangle(PolygonRectangle rect){
    return dist_2d_f32(rect.vertices_x[0], rect.vertices_y[0], rect.vertices_x[1], rect.vertices_y[1]);
}

f32 get_height_polygon_rectangle(PolygonRectangle rect){
    return dist_2d_f32(rect.vertices_x[0], rect.vertices_y[0], rect.vertices_x[3], rect.vertices_y[3]);
}

bool is_overlapping_sat_scalar_circle(
    f32 lhs_x, f32 lhs_y, f32 lhs_radius, 
    f32 rhs_x, f32 rhs_y, f32 rhs_radius, 
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    *out_normal_x = INITIAL_NORMAL.x;
    *out_normal_y = INITIAL_NORMAL.y;

    f32 dist_sqrd = dist_sqrd_2d_f32(lhs_x, lhs_y, rhs_x, rhs_y);
    f32 radius_sum = lhs_radius + rhs_radius;
    f32 radis_sum_sqrd = radius_sum * radius_sum;

    if (dist_sqrd >= radis_sum_sqrd){
        *out_depth = 0.0f;
        return false;
    }

    // Apply a full up force if the two colliders are in the exact same position.
    // this also stops the whole collision system from exploding.
    if (dist_sqrd <= F32_EPSILON){
        *out_depth = radius_sum;
        return true;
    }

    f32 dist = sqrt_f32(dist_sqrd);
    normalise_2d_f32(lhs_x - rhs_x, lhs_y - rhs_y, out_normal_x, out_normal_y);
    *out_depth = radius_sum - dist;
    return true;  
}

bool is_overlapping_sat_circle(Circle lhs, Circle rhs, Vector2* out_normal, f32* out_depth){
    return is_overlapping_sat_scalar_circle(
        lhs.x, lhs.y, lhs.radius, 
        rhs.x, rhs.y, rhs.radius, 
        &out_normal->x, &out_normal->y, out_depth
    );
}

/*
    projects the edges of a circle onto a given axis.
*/
void project_circle_scalar(
    f32 circle_x, f32 circle_y, f32 circle_radius, f32 axis_x, f32 axis_y,
    f32* out_min_circle_edge, f32* out_max_circle_edge
){
    f32 dir_and_radius_x = axis_x * circle_radius;
    f32 dir_and_radius_y = axis_y * circle_radius;

    f32 vAX = circle_x + dir_and_radius_x;
    f32 vAY = circle_y + dir_and_radius_y;
    f32 vBX = circle_x - dir_and_radius_x;
    f32 vBY = circle_y - dir_and_radius_y;
    
    *out_min_circle_edge = dot_2d_f32(vAX, vAY, axis_x, axis_y);
    *out_max_circle_edge = dot_2d_f32(vBX, vBY, axis_x, axis_y);

    if(*out_min_circle_edge > *out_max_circle_edge)
    {
        f32 temp = *out_min_circle_edge;
        *out_min_circle_edge = *out_max_circle_edge;
        *out_max_circle_edge = temp;
    }
}

/*
    projects the edges of a circle onto a given axis.
*/
void project_circle(Circle circle, Vector2 axis, f32* out_min_circle_edge, f32* out_max_circle_edge){
    project_circle_scalar(circle.x, circle.y, circle.radius, axis.x, axis.y, out_min_circle_edge, out_max_circle_edge);
}

/*
    calculates the contact point between two circles.
*/
void calc_contact_points_scalar_circle(f32 a_x, f32 a_y, f32 a_radius, f32 b_x, f32 b_y, f32* out_contact_point_x, f32* out_contact_point_y){
    f32 dist_x = b_x - a_x;
    f32 dist_y = b_y - a_y;
    f32 dir_x;
    f32 dir_y;
    normalise_2d_f32(dist_x, dist_y, &dir_x, &dir_y);
    
    // check for Nan in case the two circles are perfectly ontop of one another,
    // as normalising a distance of zero gives a NaN.
    *out_contact_point_x = a_x + (dir_x * a_radius);
    *out_contact_point_y = a_y + (dir_y * a_radius); 
}

/*
    calculates the contact point between two circles.
*/
Vector2 calc_contact_points_circle(Circle a, Circle b){
    Vector2 result;
    calc_contact_points_scalar_circle(a.x, a.y, a.radius, b.x, b.y, &result.x, &result.y);
    return result;
}
/*
    projects a set of vertices onto a normalised axis.

    remarks:
    the 'edge' of a polygon is defined as the outer most vertices that are projected onto the axis.
*/
void project_polygon(
    f32* verts_x, f32* verts_y, i32 verts_size, 
    f32 axis_x, f32 axis_y,
    f32* out_min_edge, f32* out_max_edge
){
    *out_min_edge = F32_MAX;
    *out_max_edge = F32_MIN;

    for(i32 i = 0; i < verts_size; i++){
        f32 projection = dot_2d_f32(verts_x[i], verts_y[i], axis_x, axis_y);

        if(projection < *out_min_edge){
            *out_min_edge = projection;
        }
        if(projection > *out_max_edge){
            *out_max_edge = projection;
        }
    }
}

bool is_overlapping_point_scalar_polygon(
    f32* verts_x, f32* verts_y, i32 verts_size,
    f32 point_x, f32 point_y, 
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    
    *out_depth = F32_MAX;
    *out_normal_x = 0.0f;
    *out_normal_y = 0.0f;

    f32 min_mag_sqrd    = F32_MAX;
    f32 min_depth_sqrd  = F32_MAX;
    f32 min_axis_depth  = F32_MAX;
    f32 min_axis_x      = F32_MAX;
    f32 min_axis_y      = F32_MAX;
    f32 min_edge        = F32_MAX;
    f32 max_edge        = F32_MAX;

    for(i32 i = 0; i < verts_size; i++){

        i32 next_index = (i + 1 == verts_size) ? 0 : i + 1;

        // calc the perpendicular edge.
        f32 axis_x = -(verts_y[next_index] - verts_y[i]);
        f32 axis_y = verts_x[next_index] - verts_x[i];

        // project using axis.
        project_polygon(verts_x, verts_y, verts_size, axis_x, axis_y, &min_edge, &max_edge);        
        f32 point_proj = dot_2d_f32(point_x, point_y, axis_x, axis_y);

        if(point_proj <= min_edge || point_proj >= max_edge){
            return false; // Separation found.
        }

        // Calculate overlap in "scaled space"
        f32 axis_depth = MIN(point_proj - min_edge, max_edge - point_proj);
        f32 axis_depth_sqrd = axis_depth * axis_depth;

        // to compare depths correctly, the squared length of the axis is needed.
        f32 mag_sqrd = axis_x * axis_x + axis_y * axis_y;

        // check if this is the minimum translation distance.
        if(min_depth_sqrd * mag_sqrd > axis_depth_sqrd){
            min_depth_sqrd = axis_depth_sqrd / mag_sqrd; // store relative squared depth.
            min_axis_x = axis_x;
            min_axis_y = axis_y;
            min_axis_depth = axis_depth;
            min_mag_sqrd = mag_sqrd;
        }
    }

    f32 mag = sqrt_f32(min_mag_sqrd);
    *out_depth = min_axis_depth / mag; // Only one sqrt if this is the new minimum translation distance.
    *out_normal_x = min_axis_x / mag;
    *out_normal_y = min_axis_y / mag;

    return true;
}

/*
    remarks:
    `out_normal` and `out_depth` are relative to polygon `a`.

    this is an internal function for `is_overlapping_polygon()`.
    do NOT use this function; instead use said function instead.
*/
bool is_overlapping_one_way_polygon(
    f32* a_verts_x, f32* a_verts_y, i32 a_verts_size,
    f32* b_verts_x, f32* b_verts_y, i32 b_verts_size,
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    *out_depth      = F32_MAX;
    *out_normal_x   = 0;
    *out_normal_y   = 0;

    f32 min_mag_sqrd    = F32_MAX;
    f32 min_depth_sqrd  = F32_MAX;
    f32 min_axis_depth  = F32_MAX;
    f32 min_axis_x      = F32_MAX;
    f32 min_axis_y      = F32_MAX;
    f32 a_min_edge      = F32_MAX;
    f32 b_min_edge      = F32_MAX;
    f32 a_max_edge      = F32_MAX;
    f32 b_max_edge      = F32_MAX;

    for(i32 i = 0; i < a_verts_size; i++){
        i32 next_index = (i + 1 == a_verts_size) ? 0 : i + 1;

        // edge.
        f32 axis_x = -(a_verts_y[next_index] - a_verts_y[i]);
        f32 axis_y = a_verts_x[next_index] - a_verts_x[i];

        // project using axis.
        project_polygon(a_verts_x, a_verts_y, a_verts_size, axis_x, axis_y, &a_min_edge, &a_max_edge);
        project_polygon(b_verts_x, b_verts_y, b_verts_size, axis_x, axis_y, &b_min_edge, &b_max_edge);
        
        if(a_min_edge >= b_max_edge || b_min_edge >= a_max_edge){
            return false; // Separation found.
        }

        // Calculate overlap in "scaled space"
        f32 axis_depth = MIN (b_max_edge - a_min_edge, a_max_edge - b_min_edge);

        // to compare depths correctly, the squared length of the axis is needed.
        f32 mag_sqrd = axis_x * axis_x + axis_y * axis_y;

        f32 axis_depth_sqrd = axis_depth * axis_depth;

        // check if this is the minimum translation distance.
        if(min_depth_sqrd * mag_sqrd > axis_depth_sqrd){
            min_depth_sqrd = axis_depth_sqrd / mag_sqrd; // store relative squared depth.
            min_axis_x = axis_x;
            min_axis_y = axis_y;
            min_axis_depth = axis_depth;
            min_mag_sqrd = mag_sqrd;
        }
    }

    f32 mag = sqrt_f32(min_mag_sqrd);
    *out_depth      = min_axis_depth / mag; // Only one sqrt if this is the new minimum translation distance.
    *out_normal_x   = min_axis_x / mag;
    *out_normal_y   = min_axis_y / mag;

    return true;
}

bool is_overlapping_polygon(
    f32* lhs_verts_x, f32* lhs_verts_y, i32 lhs_verts_size,
    f32* rhs_verts_x, f32* rhs_verts_y, i32 rhs_verts_size,
    f32 lhs_centroid_x, f32 lhs_centroid_y, f32 rhs_centroid_x, f32 rhs_centroid_y, 
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    *out_normal_x = INITIAL_NORMAL.x;
    *out_normal_y = INITIAL_NORMAL.y;
    f32 found_normal_x;
    f32 found_normal_y;
    f32 found_depth;
    *out_depth = F32_MAX;


    bool lhs_overlaps_rhs = is_overlapping_one_way_polygon(
        lhs_verts_x, lhs_verts_y, lhs_verts_size, 
        rhs_verts_x, rhs_verts_y, rhs_verts_size, 
        &found_normal_x, &found_normal_y, &found_depth
    ); 

    if (lhs_overlaps_rhs && *out_depth > found_depth){
        *out_depth = found_depth;
        *out_normal_x = found_normal_x; 
        *out_normal_y = found_normal_y;
    }
    else{
        return false;
    }

    bool rhs_overlaps_lhs = is_overlapping_one_way_polygon(
        rhs_verts_x, rhs_verts_y, rhs_verts_size, 
        lhs_verts_x, lhs_verts_y, lhs_verts_size, 
        &found_normal_x, &found_normal_y, &found_depth
    );

    if (rhs_overlaps_lhs && *out_depth > found_depth){            
        *out_depth = found_depth;
        *out_normal_x = found_normal_x; 
        *out_normal_y = found_normal_y;
    }
    else{
        return false;
    }

    // when a new smaller   
    // depth is found but in relation to rect B, not A.
    // this is so that the resolution code will always push A out of B
    // and not push the two i32o each other when a smaller depth is found when 
    // looping through rect B.
    if(dot_2d_f32(rhs_centroid_x - lhs_centroid_x, rhs_centroid_y - lhs_centroid_y, *out_normal_x, *out_normal_y) >= 0){
        *out_normal_x *= -1.0f;
        *out_normal_y *= -1.0f;
    }
    
    return true;
}

/*
    remarks:
    this is an internal function for `find_contact_points_polygon()`
    do NOT use this function; instead use said function instead.
*/
void find_contact_points_one_way_polygon(
    f32* a_verts_x, f32* a_verts_y, i32 a_verts_size, 
    f32* b_verts_x, f32* b_verts_y, i32 b_verts_size, 
    f32 epsilon, f32* out_min_dist_sqrd, f32* out_contact_point_1_x, f32* out_contact_point_1_y, 
    f32* out_contact_point_2_x, f32* out_contact_point_2_y, i32* out_contact_point_count
){
    for(i32 i = 0; i < a_verts_size; i++)
    {
        f32 point_x = a_verts_x[i];
        f32 point_y = a_verts_y[i];

        for(i32 start_index = 0; start_index < b_verts_size; start_index++)
        {
            // find the closest poi32 on polygon b to the vertice on polygon a.
            
            f32 edge_start_x = b_verts_x[start_index];
            f32 edge_start_y = b_verts_y[start_index];

            i32 end_index = start_index + 1;

            // this is faster than modulo.
            if(end_index >= b_verts_size){
                end_index = 0;
            }
            
            f32 edge_end_x = b_verts_x[end_index];
            f32 edge_end_y = b_verts_y[end_index];

            f32 closest_point_x;
            f32 closest_point_y;
            f32 dist_sqrd;

            closest_point_with_sqrd_dist_scalar_line_segment(
                edge_start_x, edge_start_y, edge_end_x, edge_end_y, point_x, point_y, 
                &closest_point_x, &closest_point_y, &dist_sqrd            
            );

            if(near_equal_f32(dist_sqrd, *out_min_dist_sqrd, epsilon))
            {
                // note: there is a chance that two contact points can be in the same place.
                // this is caused by when two vertices - one from each polygon - are in contact.
                // without this 'if check', all the contact information will be wiped out 
                // when those two corners hit eachother.

                if(near_equal_f32(closest_point_x, *out_contact_point_1_x, epsilon) == false
                || near_equal_f32(closest_point_y, *out_contact_point_1_y, epsilon) == false)
                {
                    // there are two contact points
                    *out_contact_point_count = 2;
                    *out_contact_point_2_x = closest_point_x;
                    *out_contact_point_2_y = closest_point_y;
                }
            }
            else if(dist_sqrd < *out_min_dist_sqrd)
            {
                // a new absolute minimum contact point has been found.
                // meaning that there is only one contact point.

                *out_min_dist_sqrd = dist_sqrd;
                *out_contact_point_count = 1;
                *out_contact_point_1_x = closest_point_x;
                *out_contact_point_1_y = closest_point_y;
            }
        } 
    } 
}

/*
    remarks:
    ensure to check `out_contact_point_count` before using returned contact points.

    parameters:
    `out_contact_point_count`: can return either 1 or 2.
*/
void find_contact_points_polygon(
    f32* a_verts_x, f32* a_verts_y, i32 a_verts_size,
    f32* b_verts_x, f32* b_verts_y, i32 b_verts_size,
    f32 epsilon, f32* out_contact_point_1_x, f32* out_contact_point_1_y, 
    f32* out_contact_point_2_x, f32* out_contact_point_2_y, i32* out_contact_point_count
){
    *out_contact_point_1_x = 0;
    *out_contact_point_1_y = 0;
    *out_contact_point_2_x = 0;
    *out_contact_point_2_y = 0;
    *out_contact_point_count = 0;
    f32 min_dist_sqrd = F32_MAX;

    // polygon a to b.
    find_contact_points_one_way_polygon(
        a_verts_x, a_verts_y, a_verts_size, 
        b_verts_x, b_verts_y, b_verts_size,
        epsilon, &min_dist_sqrd, out_contact_point_1_x, out_contact_point_1_y, 
        out_contact_point_2_x, out_contact_point_2_y, out_contact_point_count
    );

    // polygon b to a.
    find_contact_points_one_way_polygon(
        b_verts_x, b_verts_y, b_verts_size,
        a_verts_x, a_verts_y, a_verts_size, 
        epsilon, &min_dist_sqrd, out_contact_point_1_x, out_contact_point_1_y, 
        out_contact_point_2_x, out_contact_point_2_y, out_contact_point_count
    );
}

bool is_circle_overlapping_scalar_polygon(
    f32* poly_verts_x, f32* poly_verts_y, f32 poly_centroid_x, f32 poly_centroid_y, i32 poly_verts_size,
    f32 circle_x, f32 circle_y, f32 circle_radius,
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    *out_depth = F32_MAX;
    // store normals as f32s and operate on them as
    // f32s before allocating a Vector as numerical
    // arithematic is faster.
    *out_normal_x = INITIAL_NORMAL.x;
    *out_normal_y = INITIAL_NORMAL.y;

    f32 axis_x;
    f32 axis_y;
    f32 axis_depth;
    f32 a_min = F32_MAX;
    f32 a_max = F32_MAX;
    f32 b_min;
    f32 b_max;

    for(i32 i = 0; i < poly_verts_size; i++){
        i32 current_index = i;
        i32 next_index = i+1;

        // this is faster than modulo.
        if(next_index >= poly_verts_size)
            next_index = 0;

        f32 curr_x = poly_verts_x[current_index];
        f32 next_x = poly_verts_x[next_index];
        f32 curr_y = poly_verts_y[current_index];
        f32 next_y = poly_verts_y[next_index];

        f32 edge_x = next_x - curr_x; 
        f32 edge_y = next_y - curr_y; 

        // the normal of the edge.
        // note: this only works as vertices are assumed to be in clockwise winding order.
        // change to new Vector2(edge.y, -edge.x); if anti-clockwise.
        axis_x = -edge_y;
        axis_y = edge_x;
    
        // normalize (important for correct depth).
        normalise_2d_f32(axis_x, axis_y, &axis_x, &axis_y);
    
        // project all vertices onto the current edge to find the min and max values
        // of the two rectangles along the edge.
        project_polygon(poly_verts_x, poly_verts_y, poly_verts_size, axis_x, axis_y, &a_min, &a_max);
        project_circle_scalar(circle_x, circle_y, circle_radius, axis_x, axis_y, &b_min, &b_max);

        if(a_min > b_max || b_min > a_max){
            // there is separation.
            return false;
        }

        axis_depth = MIN(b_max - a_min, a_max - b_min);
        if(*out_depth > axis_depth){
            // only assign if the newly found i32ersection depth is smaller.
            *out_depth = axis_depth;
            *out_normal_x = axis_x;
            *out_normal_y = axis_y;
        }
    }

    i32 closest_point_index = find_closest_vertex_scalar_polygon(circle_x, circle_y, poly_verts_x, poly_verts_y, poly_verts_size);
    f32 closest_point_x = poly_verts_x[closest_point_index];
    f32 closest_point_y = poly_verts_y[closest_point_index];

    axis_x = closest_point_x - circle_x;
    axis_y = closest_point_y - circle_y;
    normalise_2d_f32(axis_x, axis_y, &axis_x, &axis_y);

    // project all vertices onto the current edge to find the min and max values
    // of the two rectangles along the edge.
    project_polygon(poly_verts_x, poly_verts_y, poly_verts_size, axis_x, axis_y, &a_min, &b_min);
    project_circle_scalar(circle_x, circle_y, circle_radius, axis_x, axis_y, &b_min, &b_max);

    if(a_min > b_max || b_min > a_max){
        // there is separation.
        return false;
    }

    axis_depth = MIN(b_max - a_min, a_max - b_min);
    if(*out_depth > axis_depth){
        // only assign if the newly found i32ersection depth is smaller.
        *out_depth = axis_depth;
        *out_normal_x = axis_x;
        *out_normal_y = axis_y;
    }

    f32 dist_x = circle_x - poly_centroid_x;
    f32 dist_y = circle_y - poly_centroid_y;

    // when a new smaller   
    // depth is found but in relation to rect B, not A.
    // this is so that the resolution code will always push A out of B
    // and not push the two i32o each other when a smaller depth is found when 
    // looping through rect B.
    if(dot_2d_f32(dist_x, dist_x, *out_normal_x,  *out_normal_y) >= 0){
        *out_normal_x *= -1.0f;
        *out_normal_y *= -1.0f;
    }

    return true;
}

bool is_circle_overlapping_polygon(Polygon polygon, Vector2 polygon_centroid, Circle circle, Vector2* out_normal, f32* out_depth){
    return is_circle_overlapping_scalar_polygon(
        polygon.verts_x, polygon.verts_y, polygon_centroid.x, polygon_centroid.y, polygon.verts_size,
        circle.x, circle.y, circle.radius,
        &(*out_normal).x, &(*out_normal).y, out_depth
    );
}

/*
    finds the contact points between a polygon and a query-point.
*/
void find_point_contact_points_scalar_polygon(
    f32* poly_verts_x, f32* poly_verts_y, i32 poly_verts_size,
    f32 point_x, f32 point_y,
    f32* out_contact_point_x, f32* out_contact_point_y
){

    *out_contact_point_x = F32_MAX;    
    *out_contact_point_y = F32_MAX;
    f32 min_dist_sqrd = F32_MAX;
    f32 closest_point_x;
    f32 closest_point_y;
    f32 dist_sqrd;

    // find the closest poi32 for each edge of the rectangle.
    for(i32 start_index = 0; start_index < poly_verts_size; start_index++){        
        i32 next_index = start_index + 1;
        // this is faster than modulo.
        if(next_index >= poly_verts_size)
            next_index = 0;

        closest_point_with_sqrd_dist_scalar_line_segment(
            poly_verts_x[start_index], poly_verts_y[start_index], poly_verts_x[next_index], poly_verts_y[next_index],
            point_x, point_y, &closest_point_x, &closest_point_y, &dist_sqrd
        );

        if(dist_sqrd < min_dist_sqrd){
            min_dist_sqrd = dist_sqrd;
            *out_contact_point_x = closest_point_x;
            *out_contact_point_y = closest_point_y;
        }
    } 
}

f32 calc_area_scalar_rectangle(f32 width, f32 height){
    return width*height;
}

f32 calc_area_rectangle(Rectangle rect){
    return calc_area_scalar_rectangle(rect.width, rect.height);
}