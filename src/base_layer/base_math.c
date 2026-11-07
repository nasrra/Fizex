#include <math.h>

/*========================================
    types.
========================================*//**/

typedef struct{
    f32* x;
    f32* y;
    i32 length;
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
    i32 count;
    /*
        the size of all backing arrays.
    */
    i32 length;
    bool is_init;
} Soa_Vector2;

typedef struct{
    /*
        Remarks:
        elements are accessed via `chunk_element_index`.
    */
    f32* x;
    i32 x_length;
    /*
        Remarks:
        elements are accessed via `chunk_element_index`.
    */
    f32* y;
    i32 y_length;
    /*
        Remarks:
        Elements are accessed via `chunk_index`.
    */
    i32* chunk_count;
    i32 chunk_count_length;
    /*
        the fixed stride of each entry.
    */
    i32 chunk_stride;
    /**
        the amount of chunks stored.
    **/
    i32 chunk_length;
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
} Transform3D;

typedef struct{
    Vector2 position;
    Vector2 scale;
    f32 sine;
    f32 cosine;
} Transform2D;

typedef struct{
    Soa_Vector2 position;
    Soa_Vector2 scale;
    f32* sine;
    f32* cosine;
    i32 length;
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
        the length of all backing arrays.
    */
    i32 length;
    /*
        the count of allocated entries from appending.
    */
    i32 count;
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
    f32 x[4];
    /*
        Remarks:
        the size of this array is `POLYGON_RECTANGLE_VERTICES_SIZE`
    */
    f32 y[4];
} PolygonRectangle;

/*========================================
    defines
========================================*//**/

#define POLYGON_CONTACT_POINT_EPSILON 1e-5f
#define VECTOR2_ONE ((Vector2){1.0f, 1.0f})
#define VECTOR2_UP ((Vector2){0.0f, 1.0f})
#define VECTOR2_DOWN ((Vector2){0.0f, -1.0f})
#define VECTOR2_LEFT ((Vector2){-1.0f, 0.0f})
#define VECTOR2_RIGHT ((Vector2){1.0f, 0.0f})
#define VECTOR3_RIGHT ((Vector3){1.0f, 0.0f, 0.0f})
#define VECTOR3_UP ((Vector3){0.0f, 1.0f, 0.0f})
#define VECTOR3_FORWARD ((Vector3){0.0f, 0.0f, 1.0f})
#define VECTOR3_MAX ((Vector3){f32_MAX, f32_MAX, f32_MAX})
#define VECTOR3_ONE ((Vector3){1.0f, 1.0f, 1.0f})
#define QUATERNION_IDENTITY ((Quaternion){.w = 1.0f})
#define TRANSFORM3D_IDENTITY ((Transform){.scale = VECTOR3_ONE, .rotation = QUATERNION_IDENTITY})
#define TRANSFORM2D_IDENTITY ((Transform2D){.scale = VECTOR2_ONE, .cosine = 1})
#define POLYGON_RECTANGLE_VERTICES_LENGTH 4
#define PI 3.1415926535897932384626433f
#define TAU 6.283185307179586f
#define ONE_SIXTH 1.0f / 6.0f
#define ONE_TWENTY_FOURTH 1.0f / 24.0f
#define MATRIX4X4_IDENTITY (Matrix4x4){ \
    .m = {                              \
        1.0f, 0.0f, 0.0f, 0.0f,         \
        0.0f, 1.0f, 0.0f, 0.0f,         \
        0.0f, 0.0f, 1.0f, 0.0f,         \
        0.0f, 0.0f, 0.0f, 1.0f          \
    }                                   \
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

///
/// @ functions: f32
///


f32 f32_step_clamp(f32 val, f32 step){
    return (f32)round(val / step) * step;
}

bool f32_nearly_equal(f32 a, f32 b, f32 epsilon){
    /**
        Note: norm-based comparison enurses
        the epsilon comparison doesnt return false negatives
        at large floating point values.
    **/
    f32 diff = ABS(a - b);
    return diff <= epsilon;
}

inline i32 i32_sign(i32 val){
    return (val > 0.0f) - (val < 0.0f);
}

inline f32 f32_sign(f32 val){
    return (f32)(val > 0.0f) - (val < 0.0f);
}

inline f32 f32_sin(f32 val){
    return sinf(val);
}

inline f32 f32_cos(f32 val){
    return cosf(val);
}

inline f32 f32_tan(f32 val){
    return tanf(val);
}

inline f32 f32_sqrt(f32 val){
    return sqrtf(val);
}

inline f32 f32_atan2(f32 y, f32 x){
    return atan2f(y, x);
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

inline f32 degrees_to_radians(f32 degrees){
    return degrees * PI * 0.00555555555555555555555555555556f;
}


///
/// functions: f64
///


f64 f64_sin(f64 val){
    return sin(val);
}

f64 f64_cos(f64 val){
    return cos(val);
}

f64 f64_tan(f64 val){
    return tan(val);
}

f64 f64_sqrt(f64 val){
    return sqrt(val);
}

f64 f64_atan2(f64 y, f64 x){
    return atan2(y, x);
}


///
/// @ functions: Vector2
///


inline f32 vector2_radian_difference_scalar(
    f32 sineA, f32 cosineA,
    f32 sineB, f32 cosineB
){
    f32 cross = sineB * cosineA - cosineB * sineA;
    f32 dot = cosineB * cosineA + sineB * sineA;
    return f32_atan2(cross, dot);
}

f32 vector2_dot_scalar(f32 lhs_x, f32 lhs_y, f32 rhs_x, f32 rhs_y){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y);
}

inline bool vector2_equals(Vector2 a, Vector2 b){
    return a.x == b.x && a.y == b.y;
}

inline f32 vector2_len_sqrd_scalar(f32 x, f32 y){
     return (x * x) + (y * y);
}

inline f32 vector2_len_sqrd(Vector2 vector){
    return vector2_len_sqrd_scalar(vector.x, vector.y);
}

inline f32 vector2_len_scalar(f32 x, f32 y){
    f32 sqrd = vector2_len_sqrd_scalar(x, y);
    return sqrd == 0.0f ? 0.0f : f32_sqrt(sqrd);
}

inline f32 vector2_len(Vector2 vector){
    return vector2_len_scalar(vector.x, vector.y);
}

Vector2 vector2_get_relative_to_destination_rectangle(Vector2 vector, Rectangle dst_rect, Vector2I dst_resolution){
    // get the distance from the destination rect to the mouse position.
    vector.x = vector.x - dst_rect.x;
    vector.y = vector.y - dst_rect.y;

    // normalise the value between zero and one, to find
    // how far into the destination rect the mouse is.
    vector.x /= dst_rect.width;
    vector.y /= dst_rect.height;

    // bring into the destination resolution coordinate space.
    vector.x *= (f32)dst_resolution.x;
    vector.y *= (f32)dst_resolution.y;
    return vector;
}

void vector2_normalise_scalar(f32 x, f32 y, f32* n_x, f32* n_y){
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

Vector2 vector2_normalise(Vector2 vector){
    vector2_normalise_scalar(vector.x, vector.y, &vector.x, &vector.y);
    return vector;
}

inline f32 vector2_cross_scalar(f32 lhs_x, f32 lhs_y, f32 rhs_x, f32 rhs_y){
    return lhs_x * rhs_y - lhs_y * rhs_x;
}

inline f32 vector2_dist_sqrd_scalar(f32 from_x, f32 from_y, f32 to_x, f32 to_y){
    f32 dx = from_x - to_x;
    f32 dy = from_y - to_y;
    return dx * dx + dy * dy;
}

inline f32 vector2_dist_scalar(f32 from_x, f32 from_y, f32 to_x, f32 to_y){
    f32 sqrd = vector2_dist_sqrd_scalar(from_x, from_y, to_x, to_y);

    // NAN guard.
    if(sqrd <= 0.0f){
        return 0.0f;
    }

    return f32_sqrt(sqrd);
}

void vector2_transform_scalar(
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

Vector2 vector2_rotate(Vector2 vector, f32 radians){
    f32 sin = f32_sin(radians);
    f32 cos = f32_cos(radians);
    vector.x = (vector.x * cos) - (vector.y * sin);
    vector.y = (vector.x * sin) + (vector.y * cos);
    return vector;
}

Vector2 vector2_unary(Vector2 val){
    val.x *= -1.0f;
    val.y *= -1.0f;
    return val;
}

Vector2 vector2_add(Vector2 lhs, Vector2 rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2 vector2_sub(Vector2 lhs, Vector2 rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 vector2_mul(Vector2 lhs, Vector2 rhs){
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

Vector2 vector2_mul_val(Vector2 vec, f32 val){
    vec.x *= val;
    vec.y *= val;
    return vec;
}

Vector2 vector2_div(Vector2 lhs, Vector2 rhs){
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

Vector2 vector2_div_val(Vector2 lhs, f32 rhs){
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}

f32 vector2_dist_sqrd(Vector2 from, Vector2 to){
    return vector2_dist_sqrd_scalar(from.x, from.y, to.x, to.y);
}

f32 vector2_dist(Vector2 from, Vector2 to){
    return vector2_dist_scalar(from.x, from.y, to.x, to.y);
}

Vector2 vector2_transform(Vector2 v, Transform2D t){
    vector2_transform_scalar(
        v.x, v.y, t.scale.x, t.scale.y, t.cosine, t.sine,
        t.position.x, t.position.y, &v.x, &v.y
    );
    return v;
}

/*
    `returns`
    the angle between the two points in `radians`.
*/
f32 vector2_get_angle_between_points(Vector2 start, Vector2 end){
    Vector2 delta = vector2_sub(end, start);
    return atan2f(delta.y, delta.x);
}

Vector3 vector2_to_vector3(Vector2 v){
    return (Vector3){v.x, v.y, 0.0f};
}

inline Vector2I vector2_to_vector2i(Vector2I v){
    return (Vector2I){(i32)v.x, (i32)v.y};
}

Vector2 vector2_clamp_to_radius(Vector2 v, f32 radius){
    f32 sqr_len = vector2_len_sqrd(v);
    if(sqr_len > radius * radius){
        f32 len = f32_sqrt(sqr_len);
        v = vector2_div_val(v, len);        // normalize
        v = vector2_mul_val(v, radius);     // scale to radius
    }
    return v;
}


///
/// functions: Vector2I
///


bool vector2i_equals(Vector2I lhs, Vector2I rhs){
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

Vector2I vector2i_add(Vector2I lhs, Vector2I rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector2I vector2i_sub(Vector2I lhs, Vector2I rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector2 vector2i_to_vector2(Vector2I vector){
    return (Vector2){.x = (f32)vector.x, .y = (f32)vector.y};
}


///
/// functions: Vector3
///


f32 vector3_dot_scalar(f32 lhs_x, f32 lhs_y, f32 lhs_z, f32 rhs_x, f32 rhs_y, f32 rhs_z){
    return (lhs_x * rhs_x) + (lhs_y * rhs_y) + (lhs_z * rhs_z);
}

f32 vector3_dot(Vector3 lhs, Vector3 rhs){
    return vector3_dot_scalar(lhs.x, lhs.y, lhs.z, rhs.x, rhs.y, rhs.z);
}

Vector3 vector3_sub(Vector3 lhs, Vector3 rhs){
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

Vector3 vector3_add(Vector3 lhs, Vector3 rhs){
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Vector3 vector3_mul(Vector3 lhs, Vector3 rhs){
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs;
}

Vector3 vector3_mul_val(Vector3 lhs, f32 rhs){
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

Vector3 vector3_div(Vector3 lhs, Vector3 rhs){
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    lhs.z /= rhs.z;
    return lhs;
}

Vector3 vector3_div_val(Vector3 lhs, f32 rhs){
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

f32 vector3_len_sqrd(Vector3 vector){
    return (vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z);
}

f32 vector3_len(Vector3 vector){
    f32 sqrd = vector3_len_sqrd(vector);
    return sqrd == 0.0f ? 0.0f : f32_sqrt(sqrd);
}

Vector3 vector3_clamp_to_radius(Vector3 v, f32 radius){
    f32 sqr_len = vector3_len_sqrd(v);
    if(sqr_len > radius * radius){
        f32 len = f32_sqrt(sqr_len);
        v = vector3_div_val(v, len);        // normalize
        v = vector3_mul_val(v, radius);     // scale to radius
    }
    return v;
}


///
/// functions: Vector3
///


Vector2 vector3_to_vector2(Vector3 v){
    return (Vector2){v.x, v.y};
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
Vector3 vector3_rotate(Vector3 v, Quaternion q){
    Vector3 q_v;
    q_v.x = q.x;
    q_v.y = q.y;
    q_v.z = q.z;

    Vector3 cross1 = cross_vector3(q_v, v);
    Vector3 cross2 = cross_vector3(q_v, cross1);

    cross1 = vector3_mul_val(cross1, (2.0f * q.w));
    cross2 = vector3_mul_val(cross2, 2.0f);

    Vector3 result;
    result = v;
    result = vector3_add(result, cross1);
    result = vector3_add(result, cross2);
    return result;
}

Aabb aabb_add_vector2(Aabb aabb, Vector2 vector){
    aabb.min_x += vector.x;
    aabb.min_y += vector.y;
    aabb.max_x += vector.x;
    aabb.max_y += vector.y;
    return aabb;
}

Aabb aabb_sub_vector2(Aabb aabb, Vector2 vector){
    aabb.min_x -= vector.x;
    aabb.min_y -= vector.y;
    aabb.max_x -= vector.x;
    aabb.max_y -= vector.y;
    return aabb;
}

bool aabb_equals(Aabb a, Aabb b){
    return
    a.min_x == b.min_x
    && a.min_y == b.min_y
    && a.max_x == b.max_x
    && a.max_y == b.max_y;
}

/*
    Calculates the sum of all i32egers from <c><paramref name="n"/></c> to 1.

    Remarks:
    `n` should not be larger than 46430.
*/
i32 calculate_triangular_sum(i32 n){
    ASSERT(n < 46430, "'n' is greater than or equal to 46430.");
    return n * (n+1) / 2;
}

void vector3_normalise_scalar(f32 x, f32 y, f32 z, f32* n_x, f32* n_y, f32* n_z){
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

Vector3 vector3_normalise(Vector3 vector){
    Vector3 result = {0};
    vector3_normalise_scalar(vector.x, vector.y, vector.z, &result.x, &result.y, &result.z);
    return result;
}

f32 len_sqrd_f32(f32 point_x, f32 point_y){
    return vector2_dot_scalar(point_x, point_y, point_x, point_y);
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
    *out_sine = f32_sin(src_radians);
    *out_cosine = f32_cos(src_radians);
}


///
/// @ LineSegment
///


void line_segment_closest_point_scalar(
    f32 line_start_x, f32 line_start_y, f32 line_end_x, f32 line_end_y,
    f32 query_point_x, f32 query_point_y, f32* out_closest_point_x, f32* out_closest_point_y
){
    f32 line_dist_x = line_end_x - line_start_x;
    f32 line_dist_y = line_end_y - line_start_y;
    f32 point_dist_x = query_point_x - line_start_x;
    f32 point_dist_y = query_point_y - line_start_y;

    f32 projection = vector2_dot_scalar(point_dist_x, point_dist_y, line_dist_x, line_dist_y);

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
    calculates the closest point along a line segmenet towards the query point.
*/
Vector2 line_segmenet_closest_point(Vector2 line_start, Vector2 line_end, Vector2 query_point){
    Vector2 result;
    line_segment_closest_point_scalar(
        line_start.x, line_start.y,
        line_end.x, line_end.y,
        query_point.x, query_point.y,
        &result.x, &result.y
    );
    return result;
}

void line_project_point(
    f32 line_start_x, f32 line_start_y,
    f32 line_dist_x, f32 line_dist_y,
    f32 point_x, f32 point_y,
    f32* out_x, f32* out_y
){
    // vector from circcle center to the line start.
    f32 start_to_point_x = point_x - line_start_x;
    f32 start_to_point_y = point_y - line_start_y;
    
    f32 line_length_sqrd = vector2_dot_scalar(line_dist_x, line_dist_y, line_dist_x, line_dist_y);
    
    // project the circle center onto the infinite line, as a ratio segment length
    // a degenerate (zero length) line is just a point, so `t` stays 0.
    f32 t = 0.0f;
    if(line_length_sqrd  > F32_EPSILON){
        t = vector2_dot_scalar(start_to_point_x, start_to_point_y, line_dist_x, line_dist_y) / line_length_sqrd;
    }
    
    // clamp to segment so that the closest point can be one of the end points.
    t = CLAMP(t, 0.0f, 1.0f);
    
    *out_x = line_start_x + line_dist_x * t;
    *out_y = line_start_y + line_dist_y * t;
}

/*
    `remarks`:
    `dist` values are the distance from `start` vertex to `end` vertex of their respective line segment.
*/
bool line_overlaps_line(
    f32 a_start_x, f32 a_start_y, f32 a_dist_x, f32 a_dist_y,
    f32 b_start_x, f32 b_start_y, f32 b_dist_x, f32 b_dist_y,
    f32* out_a_t
){
    
    /*
        check whether or not the lines are parallel to one another.
        bail if they aren't.
            
        dist_cross is also the total area and the rotational direction (or orientation) of the cross product's parallelogram.
    */
    f32 dist_cross = vector2_cross_scalar(a_dist_x, a_dist_y, b_dist_x, b_dist_y);
f32 len_sq = (a_dist_x*a_dist_x + a_dist_y*a_dist_y) *
             (b_dist_x*b_dist_x + b_dist_y*b_dist_y);
// |cross|^2 < eps^2 * |a|^2 * |b|^2  <=> sin(angle) < eps
if (dist_cross * dist_cross < F32_EPSILON * F32_EPSILON * len_sq) return false;

    f32 a_to_b_x = b_start_x - a_start_x;
    f32 a_to_b_y = b_start_y - a_start_y;
    
    /*
        by dividing the following two cross products by dist_cross, we are using dist_cross as a common denominator
        to normalise the measurement; i.e, bringing the "area" generated by the cross product operations into the 
        total parallelogram (dist_cross) space.
        
        note that dividing by the partial area (a_t or b_t; i.e, numerator) by the total area (dist_cross) the units
        cancel out, leaving a ratio (or percentage) value.  
    */
    
    // a_t = cross(a_to_b, b_dist) / dist_cross.
    // how far along 'a' that 'b' intersects with it.
    f32 a_t = vector2_cross_scalar(a_to_b_x, a_to_b_y, b_dist_x, b_dist_y) / dist_cross;
    
    // b_t = cross(a_to_b, a_dist) / dist_cross.
    // how far along 'b' that 'a' intersects with it.
    f32 b_t = vector2_cross_scalar(a_to_b_x, a_to_b_y, a_dist_x, a_dist_y) / dist_cross;
    
    /*
        'a_dist' and 'b_dist' are infinite lines (vectors) that intersect somewhere,
        however, that intersection must be inside both of the line segments bounds
        for it to count as 'hit'. 
    */
    if(
        a_t < 0.0f || 
        a_t > 1.0f ||
        b_t < 0.0f || 
        b_t > 1.0f
    ){
        return false;
    }
    
    *out_a_t = a_t;
    return true;
}

bool line_overlaps_circle(
    f32 line_start_x, f32 line_start_y, 
    f32 line_end_x, f32 line_end_y,
    f32 circle_x, f32 circle_y, f32 circle_radius,
    f32* out_contact_point_1_x, f32* out_contact_point_1_y,
    f32* out_contact_point_2_x, f32* out_contact_point_2_y,
    f32* out_normal_1_x, f32* out_normal_1_y,
    f32* out_normal_2_x, f32* out_normal_2_y,
    f32* out_depth_1,
    f32* out_depth_2,
    i32* out_contact_point_count
){
    
    f32 line_dist_x = line_end_x - line_start_x;
    f32 line_dist_y = line_end_y - line_start_y;
    
    // degenerate check.
    f32 line_length_sqrd = vector2_dot_scalar(line_dist_x, line_dist_y, line_dist_x, line_dist_y);
    if(line_length_sqrd < F32_EPSILON){
        return false;
    }

    f32 start_to_circle_x = circle_x - line_start_x;
    f32 start_to_circle_y = circle_y - line_start_y;
    
    // project the center of the circle onto the axis.
    // note that it is not clamped between 0-1 and is a ratio. 
    f32 t_mid = vector2_dot_scalar(start_to_circle_x, start_to_circle_y, line_dist_x, line_dist_y) / line_length_sqrd;
    f32 mid_x = line_start_x + (line_dist_x * t_mid);
    f32 mid_y = line_start_y + (line_dist_y * t_mid);

    f32 perp_x = circle_x - mid_x;
    f32 perp_y = circle_y - mid_y;
    f32 perp_dist_sqrd =
        (perp_x * perp_x) +
        (perp_y * perp_y)
    ;
    f32 radius_sqrd = circle_radius * circle_radius;
    
    // the project point on the line is not within the circle's radial area.
    if(perp_dist_sqrd > radius_sqrd){
        return false;
    } 
    
    // calculate the length to apply on ooposite axis to get the 'enter' and 'exit' points
    // this is done by getting half of the 'chord' (the line connecting to points on the circle edge).
    // note that it is done this way as the length of perpendicular vector is relative to its proximity to the circle center
    // meaning that the closer it is, the greater the magnitude it will be, resulting in a longer chord length; which is true 
    // in vice versa, shrinking as it moves away.
    f32 half_chord = f32_sqrt(radius_sqrd - perp_dist_sqrd); // note that this is in world-space.
    f32 half_cord_t = half_chord / f32_sqrt(line_length_sqrd); // convert back into line-space (a ratio).
    f32 t_enter = t_mid - half_cord_t;
    f32 t_exit = t_mid + half_cord_t;
    
    f32 depth = circle_radius - vector2_dist_scalar(mid_x, mid_y, circle_x, circle_y);
    
    f32 dx;
    f32 dy;
    if(t_enter < 0.0f){
        if(t_exit > 1.0f){
            // do nothing when submerged inside.
            return false;
        }
        else{
            *out_contact_point_count = 1;
            *out_contact_point_1_x = line_start_x + line_dist_x * t_exit;
            *out_contact_point_1_y = line_start_y + line_dist_y * t_exit;
            dx = *out_contact_point_1_x - circle_x;
            dy = *out_contact_point_1_y - circle_y;
            *out_depth_1 = depth;
            vector2_normalise_scalar(dx, dy, out_normal_1_x, out_normal_1_y);
        }
    }
    else{
        if(t_exit > 1.0f){
            *out_contact_point_count = 1;
            *out_contact_point_1_x = line_start_x + line_dist_x * t_enter;
            *out_contact_point_1_y = line_start_y + line_dist_y * t_enter;
            dx = *out_contact_point_1_x - circle_x;
            dy = *out_contact_point_1_y - circle_y;
            *out_depth_1 = depth;
            vector2_normalise_scalar(dx, dy, out_normal_1_x, out_normal_1_y);
        }
        else{
            *out_contact_point_count = 2;

            *out_contact_point_1_x = line_start_x + line_dist_x * t_enter;
            *out_contact_point_1_y = line_start_y + line_dist_y * t_enter;
            dx = *out_contact_point_1_x - circle_x;
            dy = *out_contact_point_1_y - circle_y;
            *out_depth_1 = depth;
            vector2_normalise_scalar(dx, dy, out_normal_1_x, out_normal_1_y);

            *out_contact_point_2_x = line_start_x + line_dist_x * t_exit;
            *out_contact_point_2_y = line_start_y + line_dist_y * t_exit;
            dx = *out_contact_point_2_x - circle_x;
            dy = *out_contact_point_2_y - circle_y;
            *out_depth_2 = depth;
            vector2_normalise_scalar(dx, dy, out_normal_2_x, out_normal_2_y);
        }
    }
    return true;
}

        // // direction of the line's travel.
        // f32 dir_x = d_x * inv_d_len; // reuses d and inv_d_len (a > 0 is guaranteed above, so no zero check needed)
        // f32 dir_y = d_y * inv_d_len;

        // // reflect the travel direction about the normal: r = dir - 2 (dir . n) n
        // f32 d_dot_n    = dir_x * *out_normal_x + dir_y * *out_normal_y;
        // *out_normal_x = dir_x - 2.0f * d_dot_n * *out_normal_x;
        // *out_normal_y = dir_y - 2.0f * d_dot_n * *out_normal_y;

///
/// @ Matrix4x4
///


Transform2D matrix4x4_to_transform2d(Matrix4x4 matrix){

    f32* m = matrix.m;
    Transform2D transform = TRANSFORM2D_IDENTITY;

    // column 0 `length` is the horizontal scale.
    transform.scale.x = f32_sqrt(m[0]*m[0] + m[1]*m[1]);
    transform.scale.y = f32_sqrt(m[4]*m[4] + m[5]*m[5]);

    // undo the scale to get pure rotation back
    if(transform.scale.y > F32_MIN){
        transform.cosine = m[0] / transform.scale.x;
        transform.sine = m[1] / transform.scale.x;
    }

    transform.position.x = m[12];
    transform.position.y = m[13];

    return transform;
}

Matrix4x4 matrix4x4_mul(Matrix4x4 lhs, Matrix4x4 rhs){
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

/*
    Computes a Left-Handed View Matrix (Column-Major)
*/
Matrix4x4 matrix4x4_create_look_at(Vector3 camera_pos, Vector3 look_at_pos, Vector3 world_up_dir){
    // Left-Handed forward: Positive Z goes forward i32o the screen
    Vector3 forward = vector3_normalise(
        vector3_sub(look_at_pos, camera_pos)
    );

    // find the local space right and up directions of the camera.
    Vector3 right = {0};
    right.x = (world_up_dir.y * forward.z) - (world_up_dir.z * forward.y);
    right.y = (world_up_dir.z * forward.x) - (world_up_dir.x * forward.z);
    right.z = (world_up_dir.x * forward.y) - (world_up_dir.y * forward.x);
    right = vector3_normalise(right);
    Vector3 up = {0};
    up.x = (forward.y * right.z) - (forward.z * right.y);
    up.y = (forward.z * right.x) - (forward.x * right.z);
    up.z = (forward.x * right.y) - (forward.y * right.x);
    up = vector3_normalise(up);

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
    m[12] = -vector3_dot(right, camera_pos);
    m[13] = -vector3_dot(up, camera_pos);
    m[14] = -vector3_dot(forward, camera_pos);
    m[15] = 1.0f;

    return result;
}

/*
    Computes a Left-Handed Perspective Matrix mapping depth to Vulkan 0..1 (Column-Major)
*/
Matrix4x4 matrix4x4_create_perspective(f32 fov_y_radians, f32 aspect_ratio, f32 z_near, f32 z_far){
    /**
        Calculate half of the vertical line that describes the length from the bottom to the top of out total vertical viewing angle;

        Note:
            think of it like the fovYRadians is how much your eye (or camera lens) sees,
            the tangent describes the length from the top to bot.
    **/
    f32 tan_half_fov_y = f32_tan(fov_y_radians * 0.5f);
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
Matrix4x4 matrix4x4_rotate(Matrix4x4 src, f32 radians, Vector3 axis){

    // how much the of the objects original orientation is kept along its original axis.
    f32 c = f32_cos(radians);
    // how much of the objects orientation is shifted perpendicularly i32o a new direction.
    f32 s = f32_sin(radians);

    axis = vector3_normalise(axis);

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
    Scales the lhs by the rhs.

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_scale_by(Matrix4x4 lhs, Matrix4x4 rhs){
    lhs.m[0] *= rhs.m[0];
    lhs.m[5] *= rhs.m[5];
    lhs.m[10] *= rhs.m[10];
    return lhs;
}

/*
    Scales the matrix by a vector.

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_scale_by_vector2(Matrix4x4 matrix, Vector2 vector){
    matrix.m[0] *= vector.x;
    matrix.m[5] *= vector.y;
    return matrix;
}

/*
    Applies a translation to a matrix4x4

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_translate_by_vector3(Matrix4x4 matrix, Vector3 vector){
    matrix.m[12] += vector.x;
    matrix.m[13] += vector.y;
    matrix.m[14] += vector.z;
    return matrix;
}

/*
    Sets the translation components of a a matrix4x4

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_set_translation(Matrix4x4 matrix, Vector3 position){
    matrix.m[12] = position.x;
    matrix.m[13] = position.y;
    matrix.m[14] = position.z;
    return matrix;
}

/*
    Applies a translation to a matrix4x4

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_translate_by_vector2(Matrix4x4 matrix, Vector2 vector){
    matrix.m[12] += vector.x;
    matrix.m[13] += vector.y;
    return matrix;
}

/*
    Applies a scaling vector to a matrix4x4

    `remarks`
    The matrix must be in left hand side format.
*/
Matrix4x4 matrix4x4_scale_by_vector3(Matrix4x4 matrix, Vector3 scale){
    matrix.m[0] *= scale.x;
    matrix.m[5] *= scale.y;
    matrix.m[10] *= scale.z;
    return matrix;
}

/*
    Computes a Left-Handed Orthographic Matrix mapping depth to Vulkan 0..1 (Column-Major)

    Parameters:
    `lowerX`: the lower-bound x-value of the camera resolution in pixels; e.g, 0.
    `upperX`: the upper-bound x-value of the camera resolution in pixels; e.g, 1920.
    `lowerY`: the lower-bound y-value of the camera resolution in pixels; e.g, 0.
    `upperY`: the upper-bound y-value of the camera resolution in pixels; e.g, 1080.
*/
Matrix4x4 matrix4x4_create_orthographic(f32 lower_x, f32 upper_x, f32 lower_y, f32 upper_y, f32 z_near, f32 z_far){
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

Vector3 matrix4x4_get_scale(Matrix4x4 matrix){
    return (Vector3){
        .x = matrix.m[0],
        .y = matrix.m[5],
        .z = matrix.m[10]
    };
}


///
/// functions: Quaternion
///


Quaternion quaternion_normalise(Quaternion q){
    f32 len_sqrd = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

    // NAN guard.
    if(len_sqrd==0.0f){
        return (Quaternion){0};
    }

    f32 len = f32_sqrt(len_sqrd);
    q.x /= len;
    q.y /= len;
    q.z /= len;
    q.w /= len;
    return q;
}

Quaternion quaternion_mul(Quaternion lhs, Quaternion rhs){
    Quaternion result;
    result.x = (lhs.w * rhs.x) + (lhs.x * rhs.w) + (lhs.y * rhs.z) - (lhs.z * rhs.y);
    result.y = (lhs.w * rhs.y) - (lhs.x * rhs.z) + (lhs.y * rhs.w) + (lhs.z * rhs.x);
    result.z = (lhs.w * rhs.z) + (lhs.x * rhs.y) - (lhs.y * rhs.x) + (lhs.z * rhs.w);
    result.w = (lhs.w * rhs.w) - (lhs.x * rhs.x) - (lhs.y * rhs.y) - (lhs.z * rhs.z);
    return result;
}

Quaternion quaternion_rotate(Quaternion q, f32 axis_x, f32 axis_y, f32 axis_z, f32 angle_radians){
    // 1. Normalize the axis vector to ensure safe rotation math
    f32 length = f32_sqrt(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    if (length < 0.0001f) return q; // Avoid division by zero

    axis_x /= length;
    axis_y /= length;
    axis_z /= length;

    // 2. Compute half-angles for the rotation representation
    f32 half_angle = angle_radians * 0.5f;
    f32 sin_half = f32_sin(half_angle);
    f32 cos_half = f32_cos(half_angle);

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
    result = quaternion_normalise(result);
    return result;
}

/*
    Creates a rotation quaternion around a normalised axis vector by a given angle in radians.
*/
Quaternion quaternion_create_from_axis_angle(Vector3 axis, f32 angle){

    // Half angle calculations required by quaternion space
    f32 half_angle = angle * 0.5f;
    f32 sin = f32_sin(half_angle);
    f32 cos = f32_cos(half_angle);

    // Scale the normalized directional axis components by the sine projection
    Quaternion result;
    result.x = axis.x * sin,
    result.y = axis.y * sin,
    result.z = axis.z * sin,
    result.w = cos;
    return result;
}

Quaternion quaternion_get_rotation_between_points(Vector3 point_a, Vector3 point_b){
    // Get the direction vector target poi32ing from A to B
    Vector3 delta;
    delta.x = point_b.x - point_a.x;
    delta.y = point_b.y - point_a.y;
    delta.z = point_b.z - point_a.z;

    Vector3 direction = vector3_normalise(delta);

    // Define the default local resting axis (e.g., Vector3(0, 1, 0) if poi32ing Up)
    Vector3 starting_axis = VECTOR3_UP;

    f32 dot = vector3_dot(starting_axis, direction);

    // Edge Case Handling: Check if target vectors poi32 directly opposite to prevent a divide-by-zero crash (180 deg)
    if (dot < -0.99999f){

        // Pick an arbitrary perpendicular backup axis to rotate around instead
        Vector3 perpendicular = cross_vector3(starting_axis, VECTOR3_RIGHT);
        if (vector3_len_sqrd(perpendicular) < 0.001f){
            perpendicular = cross_vector3(starting_axis, VECTOR3_FORWARD);
        }

        return quaternion_create_from_axis_angle(vector3_normalise(perpendicular), PI);
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
    result = quaternion_normalise(result);

    return result;
}


///
/// functions: Transform3D
///


Transform3D transform3d_transform(Transform3D lhs, Transform3D rhs){
    Transform3D result;
    // combine scales.
    result.scale = vector3_mul(lhs.scale, rhs.scale);
    // combine rotations (order matters: rhs*lhs means rhs rotates lhs)
    result.rotation = quaternion_mul(lhs.rotation, rhs.rotation);
    // combine positions (order matters: scale->rotate->translate).
    Vector3 sp = vector3_mul(lhs.position, rhs.scale);
    result.position = vector3_rotate(sp, rhs.rotation);
    result.position = vector3_add(result.position, rhs.position);
    return result;
}

/*
    Creates a col-major matrix from a transform.
*/
Matrix4x4 transform3d_to_matrix4x4(Transform3D transform){
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


///
/// functions: FsSoa_Vector2
///


bool fssoa_vector2_init(FsSoa_Vector2* soa, MemoryArena* arena, i32 chunk_stride, i32 chunk_length){
    if(soa->is_init){
        ASSERT(!soa->is_init, "attempted to init an already init FsSoa_Vector2.");
        return false;
    }
    soa->is_init = true;
    i32 length = chunk_stride * chunk_length;
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->x, &soa->x_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->y, &soa->y_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->chunk_count, &soa->chunk_count_length, length);
    soa->chunk_stride = chunk_stride;
    soa->chunk_length = chunk_length;
    return true;
}

bool fssoa_vector2_push(FsSoa_Vector2* soa, i32 chunk_index, f32 x, f32 y){
    // ensure that the entry slot isnt full.
    i32 append_count = soa->chunk_count[chunk_index];
    if(append_count >= soa->chunk_stride){
        ASSERT(0!=0, "attempted to append to a full fssoa_vector2.");
        return false;
    }
    i32 append_index = chunk_index * soa->chunk_stride + append_count;

    // set the value.
    soa->x[append_index] = x;
    soa->y[append_index] = y;

    // increment append index.
    soa->chunk_count[chunk_index]++;
    return true;
}

/*
    Sets the append count of an entry to zero in a fixed stride soa instance.
*/
void fssoa_vector2_clear_chunk_count(FsSoa_Vector2* soa, i32 chunk_index){
    BOUNDS_CHECK(chunk_index, soa->chunk_count_length);
    soa->chunk_count[chunk_index] = 0;
}

void fssoa_vector2_clear_chunk(FsSoa_Vector2* soa, i32 chunk_idx){
    i32 element_idx = chunk_idx * soa->chunk_stride;
    // (-1) as we are checking the first byte (inclusive) 
    // without this, say size is 4 bytes, we would actually be checking 5 bytes instead.  
    BOUNDS_CHECK(element_idx + soa->chunk_stride - 1, soa->x_length);
    BOUNDS_CHECK(element_idx + soa->chunk_stride - 1, soa->y_length);
    ZERO_MEMORY(soa->x + element_idx, sizeof(*soa->x) * soa->chunk_stride);
    ZERO_MEMORY(soa->y + element_idx, sizeof(*soa->y) * soa->chunk_stride);
    fssoa_vector2_clear_chunk_count(soa, chunk_idx);
}


///
/// functions: Soa_Vector2
///


void soa_vector2_init(Soa_Vector2* soa, MemoryArena* arena, i32 length){
    ASSERT(!soa->is_init, "already init");
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->x, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->y, &soa->length, length);
    soa->is_init = true;
}

void soa_vector2_insert(Soa_Vector2* soa, i32 insert_index, f32 x, f32 y){
    BOUNDS_CHECK(insert_index, soa->length);
    soa->x[insert_index] = x;
    soa->y[insert_index] = y;
}

bool soa_vector2_push(Soa_Vector2* soa, f32 x, f32 y){

    if(soa->count >= soa->length){
        ASSERT(0!=0, "attempted to append to a full soa_vector2");
        return false;
    }

    soa_vector2_insert(soa, soa->count, x, y);
    soa->count++;
    return true;
}

void soa_vector2_reset_count(Soa_Vector2* soa){
    soa->count = 0;
}

void soa_vector2_clear_element(Soa_Vector2* soa, i32 elem_idx){
    BOUNDS_CHECK(elem_idx, soa->length);
    soa->x[elem_idx] = 0;
    soa->y[elem_idx] = 0;
}


///
/// @ functions: Transform2D.
///


f32 transform2d_get_rotation_radians(Transform2D transform){
    return atan2f(transform.sine, transform.cosine);
}

Vector2 transform2d_get_forward_angle_vector(Transform2D transform){
    return (Vector2){.x = transform.cosine, .y = transform.sine};
}

Transform2D transform2d_make(Vector2 position, Vector2 scale, f32 rotation_radians){
    return (Transform2D){
        .position = position,
        .scale = scale,
        .sine = f32_sin(rotation_radians),
        .cosine = f32_cos(rotation_radians)
    };
}

// polygon_get_min_max_vertices(v_x, v_y, v_length, &soa->aabb.min_x[shape_idx], &soa->aabb.min_y[shape_idx], &soa->aabb.max_x[shape_idx], &soa->aabb.max_y[shape_idx]);

PolygonRectangle transform2d_to_polygon_rectangle_centered_origin(Transform2D transform){
    PolygonRectangle poly_rect = {
        .x = {-0.5f, 0.5f, 0.5f, -0.5f},
        .y = {0.5f, 0.5f, -0.5f, -0.5f}
    };

    for(i32 i = 0; i < 4; i++){
        vector2_transform_scalar(
            poly_rect.x[i], poly_rect.y[i],
            transform.scale.x, transform.scale.y,
            transform.cosine, transform.sine,
            transform.position.x, transform.position.y,
            &poly_rect.x[i], &poly_rect.y[i]
        );
    }

    return poly_rect;
}

PolygonRectangle transform2d_to_polygon_rectangle_top_left_origin(Transform2D transform){
    PolygonRectangle poly_rect = {
        .x = {0.0f, 1.0f, 1.0f, 0.0f},
        .y = {0.0f, 0.0f, -1.0f, -1.0f}
    };

    for(i32 i = 0; i < 4; i++){
        vector2_transform_scalar(
            poly_rect.x[i], poly_rect.y[i],
            transform.scale.x, transform.scale.y,
            transform.cosine, transform.sine,
            transform.position.x, transform.position.y,
            &poly_rect.x[i], &poly_rect.y[i]
        );
    }

    return poly_rect;
}

inline Matrix4x4 transform2d_to_matrix4x4_depth(Transform2D transform, f32 depth){
    Matrix4x4 result = {0};
    f32* m = result.m;

    f32 c = transform.cosine;
    f32 s = transform.sine;

    //COLUMN 0 (X-Basis * ScaleX)
    m[0] = c * transform.scale.x;
    m[1] = s * transform.scale.x;
    m[2] = 0.0f;
    m[3] = 0.0f;

    // COLUMN 1 (Y-Basis * ScaleY)
    m[4] = -s * transform.scale.y;
    m[5] = c * transform.scale.y;
    m[6] = 0.0f;
    m[7] = 0.0f;

    // COLUMN 2 (Z-Basis * ScaleZ)
    m[8] = 0.0f;
    m[9] = 0.0f;
    m[10] = 1.0f;
    m[11] = 0.0f;

    // COLUMN 3 (Translation)
    m[12] = transform.position.x;
    m[13] = transform.position.y;
    m[14] = depth;
    m[15] = 1.0f;

    return result;
}

Matrix4x4 transform2d_to_matrix4x4(Transform2D transform){
    return transform2d_to_matrix4x4_depth(transform, 0.0f);
}

bool soa_transform2d_init(Soa_Transform2D* soa, MemoryArena* arena, i32 length){
    if(soa->is_init){
        ASSERT(0!=0, "attempted to init already init soa_transform2d.");
        return false;
    }

    soa->is_init = true;
    soa_vector2_init(&soa->position, arena, length);
    soa_vector2_init(&soa->scale, arena, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->sine, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->cosine, &soa->length, length);
    return true;
}

Transform2D soa_transform2d_copy_elem(Soa_Transform2D* soa, i32 index){
    BOUNDS_CHECK(index, soa->length);
    Transform2D dst = {
        .position = {.x = soa->position.x[index], .y = soa->position.y[index]},
        .scale    = {.x = soa->scale.x[index], .y = soa->scale.y[index]},
        .sine       = soa->sine[index],
        .cosine     = soa->cosine[index]
    };
    return dst;
}

void soa_transform2d_insert_scalar(
    Soa_Transform2D* soa, i32 elem_index, f32 pos_x, f32 pos_y,
    f32 scale_x, f32 scale_y, f32 sin, f32 cos
){
    BOUNDS_CHECK(elem_index, soa->position.length);
    soa->position.x[elem_index] = pos_x;
    soa->position.y[elem_index] = pos_y;

    BOUNDS_CHECK(elem_index, soa->scale.length);
    soa->scale.x[elem_index] = scale_x;
    soa->scale.y[elem_index] = scale_y;

    BOUNDS_CHECK(elem_index, soa->length);
    soa->sine[elem_index] = sin;

    BOUNDS_CHECK(elem_index, soa->length);
    soa->cosine[elem_index] = cos;
}

void soa_transform2d_insert(Soa_Transform2D* soa, Transform2D transform, i32 elem_index){
    soa_transform2d_insert_scalar(
        soa, elem_index, transform.position.x, transform.position.y,
        transform.scale.x, transform.scale.y, transform.sine, transform.cosine
    );
}

Transform2D transform2d_rotate(Transform2D transform, f32 radians){
    f32 rotation = radians + transform2d_get_rotation_radians(transform);
    transform.sine = f32_sin(rotation);
    transform.cosine = f32_cos(rotation);
    return transform;
}

Transform3D transform2d_to_transform3d(Transform2D transform){
    f32 half = transform2d_get_rotation_radians(transform) * 0.5f;
    Transform3D result =  {
        .position = {transform.position.x, transform.position.y, 0.0f},
        .scale = {transform.scale.x, transform.scale.y, 1.0f},
        .rotation = {.z = f32_sin(half), .w = f32_cos(half)}
    };
    return result;
}

void soa_transform2d_clear_element(Soa_Transform2D* soa, i32 elem_idx){
    soa_vector2_clear_element(&soa->position, elem_idx);
    soa_vector2_clear_element(&soa->scale, elem_idx);
    BOUNDS_CHECK(elem_idx, soa->length);
    soa->sine[elem_idx] = 0.0f;
    soa->cosine[elem_idx] = 0.0f;
}

/**
    Offsets a transform2d by another.

    `remarks`
    `rhs` is applied to `lhs`.
**/
void transform2d_transform_scalar(
    f32 lhs_pos_x, f32 lhs_pos_y, f32 lhs_scale_x, f32 lhs_scale_y, f32 lhs_sin, f32 lhs_cos,
    f32 rhs_pos_x, f32 rhs_pos_y, f32 rhs_scale_x, f32 rhs_scale_y, f32 rhs_sin, f32 rhs_cos,
    f32* out_pos_x, f32* out_pos_y, f32* out_scale_x, f32* out_scale_y, f32* out_sin, f32* out_cos
){
    // scale the local offset relative to the world.
    f32 scaled_x = lhs_pos_x * rhs_scale_x;
    f32 scaled_y = lhs_pos_y * rhs_scale_y;

    /**
        rotate the local scaled offset around the parents origin.
            Standard 2D rotation matrix formula:
                x' = x * cos - y * sin
                y' = x * sin + y * cos
    **/
    f32 rotated_x = (scaled_x * rhs_cos) - (scaled_y * rhs_sin);
    f32 rotated_y = (scaled_x * rhs_sin) + (scaled_y * rhs_cos);

    // translate the rotated offset to the world position.
    *out_pos_x = rotated_x + rhs_pos_x;
    *out_pos_y = rotated_y + rhs_pos_y;

    // combine the scale properties
    *out_scale_x = lhs_scale_x * rhs_scale_x;
    *out_scale_y = lhs_scale_y * rhs_scale_y;

    /**
        combine the rotation properties.
            Use the trigonometrix identity formulas for combining angles:
                sin(a + b) = sin(a)cos(b) + cos(a)sin(b)
                cos(a + b) = cos(a)cos(b) - sin(a)sin(b)
    **/
    *out_sin = (lhs_sin * rhs_cos) + (lhs_cos * rhs_sin);
    *out_cos = (lhs_cos * rhs_cos) - (lhs_sin * rhs_sin);
}

Transform2D transform2d_transform(Transform2D lhs, Transform2D rhs){
    Transform2D out;
    transform2d_transform_scalar(
        lhs.position.x, lhs.position.y, lhs.scale.x, lhs.scale.y, lhs.sine, lhs.cosine,
        rhs.position.x, rhs.position.y, rhs.scale.x, rhs.scale.y, rhs.sine, rhs.cosine,
        &out.position.x, &out.position.y, &out.scale.x, &out.scale.y, &out.sine, &out.cosine
    );
    return out;
}

Transform2D transform2D_sub(Transform2D lhs, Transform2D rhs){
    Transform2D result;
    result.position = vector2_sub(lhs.position, rhs.position);
    result.scale = vector2_sub(lhs.scale, rhs.scale);
    f32 radians = vector2_radian_difference_scalar(lhs.sine, lhs.cosine, rhs.sine, rhs.sine);
    result.sine = f32_sin(radians);
    result.cosine = f32_cos(radians);
    return result;
}

Transform2D transform3d_to_transform2d(Transform3D transform){

    // Extract Roll (rotation around Z-axis) from Quaternion
    f32 num1 = (transform.rotation.w * transform.rotation.z) + (transform.rotation.x * transform.rotation.y);
    f32 num2 = (transform.rotation.y * transform.rotation.y) + (transform.rotation.z * transform.rotation.z);
    f32 rot_rad = f32_atan2(2.0f * num1, 1.0f - (2.0f * num2));

    Transform2D result;
    result.position.x = transform.position.x;
    result.position.y = transform.position.y;
    result.scale.x = transform.scale.x;
    result.scale.y = transform.scale.y;
    result.sine = f32_sin(rot_rad);
    result.cosine = f32_cos(rot_rad);
    return result;
}

void transform_scalar_transform2d(
    f32 lhs_pos_x, f32 lhs_pos_y, f32 lhs_scale_x, f32 lhs_scale_y, f32 lhs_sine, f32 lhs_cosine,
    f32 rhs_pos_x, f32 rhs_pos_y, f32 rhs_scale_x, f32 rhs_scale_y, f32 rhs_sine, f32 rhs_cosine,
    f32* out_pos_x, f32* out_pos_y, f32* out_scale_x, f32* out_scale_y, f32* out_sine, f32* out_cosine
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
    *out_cosine = (lhs_cosine * rhs_cosine) - (lhs_sine * rhs_sine);
}

Transform2D transform_transform2d(Transform2D lhs, Transform2D rhs){
    Transform2D res;
    transform_scalar_transform2d(
        lhs.position.x, lhs.position.y, lhs.scale.x, lhs.scale.y, lhs.sine, lhs.cosine,
        rhs.position.x, rhs.position.y, rhs.scale.x, rhs.scale.y, rhs.sine, rhs.cosine,
        &res.position.x, &res.position.y, &res.scale.x, &res.scale.y, &res.sine, &res.cosine
    );
    return res;
}


///
/// functions: Soa_Aabb.
///


void soa_aabb_init(Soa_Aabb* soa, MemoryArena* arena, i32 length){
    ASSERT(!soa->is_init, "already init");
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->max_x, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->max_y, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->min_x, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->min_y, &soa->length, length);
    soa->is_init = true;
}

void soa_aabb_insert(Soa_Aabb* soa, i32 elem_index, f32 min_x, f32 min_y, f32 max_x, f32 max_y){
    BOUNDS_CHECK(elem_index, soa->length);
    soa->min_x[elem_index] = min_x;
    soa->min_y[elem_index] = min_y;
    soa->max_x[elem_index] = max_x;
    soa->max_y[elem_index] = max_y;
}

bool soa_aabb_push(Soa_Aabb* soa, f32 min_x, f32 min_y, f32 max_x, f32 max_y){

    if(soa->count >= soa->length){
        ASSERT(0!=0, "attempted to append to a full soa_aabb");
        return false;
    }

    soa_aabb_insert(soa, soa->count, min_x, min_y, max_x, max_y);
    soa->count+=1;
    return true;
}

void soa_aabb_reset_count(Soa_Aabb* soa){
    soa->count = 0;
}

void soa_aabb_calculate_centroids(Soa_Aabb* soa, f32* out_x, f32* out_y){

    simd_f32_add(out_x, soa->max_x, soa->min_x, soa->length, 0);
    simd_f32_mul_val(out_x, out_x, 0.5f, soa->length, 0);

    simd_f32_add(out_y, soa->max_y, soa->min_y, soa->length, 0);
    simd_f32_mul_val(out_y, out_y, 0.5f, soa->length, 0);
}

void soa_aabb_clear_element(Soa_Aabb* soa, i32 elem_idx){

    BOUNDS_CHECK(elem_idx, soa->length);
    soa->min_x[elem_idx] = 0;
    soa->min_y[elem_idx] = 0;
    soa->max_x[elem_idx] = 0;
    soa->max_y[elem_idx] = 0;
}


///
/// functions: Aabb.
///


f32 aabb_get_height(Aabb aabb){
    return aabb.max_y - aabb.min_y;
}

f32 aabb_get_width(Aabb aabb){
    return aabb.max_x - aabb.min_x;
}

void aabb_calc_centroid_scalar(f32 min_x, f32 min_y, f32 max_x, f32 max_y, f32* out_center_x, f32* out_center_y){
    *out_center_x = (max_x + min_x) * 0.5f;
    *out_center_y = (max_y + min_y) * 0.5f;
}

Vector2 aabb_calc_centroid(Aabb aabb){
    Vector2 result;
    aabb_calc_centroid_scalar(aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y, &result.x, &result.y);
    return result;
}

Vector2 aabb_get_min_vector(Aabb aabb){
    Vector2 result;
    result.x = aabb.min_x;
    result.y = aabb.min_y;
    return result;
}

Vector2 aabb_get_max_vector(Aabb aabb){
    Vector2 result;
    result.x = aabb.max_x;
    result.y = aabb.max_y;
    return result;
}

bool aabb_overlaps_scalar(f32 a_min_x, f32 b_min_x, f32 a_min_y, f32 b_min_y, f32 a_max_x, f32 b_max_x, f32 a_max_y, f32 b_max_y){
    if(a_max_x <= b_min_x || b_max_x <= a_min_x){
        return false;
    }
    if (a_max_y <= b_min_y || b_max_y <= a_min_y){
        return false;
    }

    return true;
}

bool aabb_overlaps(Aabb a, Aabb b){
    return aabb_overlaps_scalar(
        a.min_x, b.min_x,
        a.min_y, b.min_y,
        a.max_x, b.max_x,
        a.max_y, b.max_y
    );

}

bool aabb_overlaps_point_scalar(f32 aabb_min_x, f32 aabb_min_y, f32 aabb_max_x, f32 aabb_max_y, f32 point_x, f32 point_y){
    return
    aabb_min_x <= point_x &&
    aabb_min_y <= point_y &&
    aabb_max_x >= point_x &&
    aabb_max_y >= point_y;
}

bool aabb_overlaps_point(Aabb aabb, Vector2 point){
    return aabb_overlaps_point_scalar(
        aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y,
        point.x, point.y
    );
}

bool aabb_overlaps_line_scalar(
    f32 aabb_min_x, f32 aabb_min_y, f32 aabb_max_x, f32 aabb_max_y,
    f32 line_start_x, f32 line_start_y, f32 line_end_x, f32 line_end_y
){
    f32 closest_point_x;
    f32 closest_point_y;

    line_segment_closest_point_scalar(line_start_x, line_start_y, line_end_x, line_end_y, aabb_min_x, aabb_min_y, &closest_point_x, &closest_point_y);

    if(aabb_overlaps_point_scalar(aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y, closest_point_x, closest_point_y)){

        line_segment_closest_point_scalar(line_start_x, line_start_y, line_end_x, line_end_y, aabb_max_x, aabb_max_y, &closest_point_x, &closest_point_y);

        if(aabb_overlaps_point_scalar(aabb_min_x, aabb_min_y, aabb_max_x, aabb_max_y, closest_point_x, closest_point_y)){
            return true;
        }
    }
    return false;
}

bool aabb_overlaps_line(Aabb aabb, Vector2 line_start, Vector2 line_end){
    return aabb_overlaps_line_scalar(
        aabb.min_x, aabb.min_y, aabb.max_x, aabb.max_y,
        line_start.x, line_start.y, line_end.x, line_end.y
    );
}

/*
    creates an AABB of the greatest max and lowest min vector.
*/
void aabb_combine_scalar(
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
Aabb aabb_combine(Aabb a, Aabb b){
    Aabb result;
    aabb_combine_scalar(
        a.min_x, a.min_y, a.max_x, a.max_y,
        b.min_x, b.min_y, b.max_x, b.max_y,
        &result.min_x, &result.min_y,
        &result.max_x, &result.max_y
    );
    return result;
}

Aabb aabb_translate(Aabb aabb, Vector2 translation){
    aabb.min_x += translation.x;
    aabb.max_x += translation.x;
    aabb.min_y += translation.y;
    aabb.max_y += translation.y;
    return aabb;
}





/*
    finds the closest vertex on a polygon to a given position and returns its index.
*/
i32 find_closest_vertex_scalar_polygon(f32 query_pos_x, f32 query_pos_y, f32* verts_x, f32* verts_y, i32 verts_size){

    i32 result = 0;
    f32 min_dist = F32_MAX;

    for(i32 i = 0; i < verts_size; i++){
        f32 distance = vector2_dist_sqrd_scalar(verts_x[i], verts_y[i], query_pos_x, query_pos_y);

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
void polygon_calc_centroid_scalar(f32* verts_x, f32* verts_y, i32 verts_length, f32* out_centroid_x, f32* out_centroid_y){
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

    // if(verts_length == 2){
    //     *out_centroid_x = (verts_x[0] + verts_x[1]) * 0.5f;
    //     *out_centroid_y = (verts_y[0] + verts_y[1]) * 0.5f;
    //     return;
    // }

    for(i32 i = 0; i < verts_length; i++){
        next_index = i + 1;
        next_in_range = next_index < verts_length;

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
        cross_prod = vector2_cross_scalar(x0, y0, x1, y1);

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
        // the polygon is degenerate or a single vertex.
        *out_centroid_x = verts_x[0];
        *out_centroid_y = verts_y[0];
    }
}

Vector2 calc_centroid_polygon(f32* verts_x, f32* verts_y, i32 verts_size){
    Vector2 result;
    polygon_calc_centroid_scalar(verts_x, verts_y, verts_size, &result.x, &result.y);
    return result;
}




/**====================
    functions: Circle.
====================**//**/




void circle_get_min_max_vertices_scalar(f32 x, f32 y, f32 radius, f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y){
    *out_min_x = x - radius;
    *out_min_y = y - radius;
    *out_max_x = x + radius;
    *out_max_y = y + radius;
}

inline f32 circle_get_area_scalar(f32 radius){
    return radius * radius * PI;
}

inline f32 circle_get_area(Circle circle){
    return circle_get_area_scalar(circle.radius);
}

void polygon_rectangle_init_from_verts(PolygonRectangle* rect, Vector2 vert0, Vector2 vert1, Vector2 vert2, Vector2 vert3){
    rect->x[0] = vert0.x;
    rect->y[0] = vert0.y;
    rect->x[1] = vert1.x;
    rect->y[1] = vert1.y;
    rect->x[2] = vert2.x;
    rect->y[2] = vert2.y;
    rect->x[3] = vert3.x;
    rect->y[3] = vert3.y;
}

void polygon_rectangle_init(PolygonRectangle* rect, f32 x, f32 y, f32 width, f32 height){
    f32 left = x;
    f32 top = y;
    f32 right = x+width;
    f32 bottom = y-height;

    // top left.
    rect->x[0] = left;
    rect->y[0] = top;
    // top right.
    rect->x[1] = right;
    rect->y[1] = top;
    // bottom right.
    rect->x[2] = right;
    rect->y[2] = bottom;
    // bottom left.
    rect->x[3] = left;
    rect->y[3] = bottom;
}

PolygonRectangle polygon_rectangle_from_rectangle(Rectangle rect){
    PolygonRectangle poly;
    polygon_rectangle_init(&poly, rect.x, rect.y, rect.width, rect.height);
    return poly;
}

PolygonRectangle transform_polygon_rectangle(PolygonRectangle rect, Transform2D transform){
    PolygonRectangle result;

    Vector2 v0 = {rect.x[0], rect.y[0]};
    v0 = vector2_transform(v0, transform);

    Vector2 v1 = {rect.x[1], rect.y[1]};
    v1 = vector2_transform(v1, transform);

    Vector2 v2 = {rect.x[2], rect.y[2]};
    v2 = vector2_transform(v2, transform);

    Vector2 v3 = {rect.x[3], rect.y[3]};
    v3 = vector2_transform(v3, transform);

    polygon_rectangle_init_from_verts(&result, v0, v1, v2, v3);
    return result;
}

void polygon_calc_centroid_scalar_rectangle(PolygonRectangle rect, f32* out_centroid_x, f32* out_centroid_y){
    polygon_calc_centroid_scalar(rect.x, rect.y, 4, out_centroid_x, out_centroid_y);
}

Vector2 calc_centroid_polygon_rectangle(PolygonRectangle rect){
    return calc_centroid_polygon(rect.x, rect.y, 4);
}

void get_min_max_vectors_scalar_polygon(
    f32* verts_x, f32* verts_y, i32 verts_size,
    f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y
){
    *out_min_x = F32_MAX;
    *out_min_y = F32_MAX;
    *out_max_x = -F32_MAX;
    *out_max_y = -F32_MAX;

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





/**
    functions: PolygonRectangle.
**/






Aabb polygon_rectangle_calc_aabb(PolygonRectangle rect){
    Aabb aabb;
    get_min_max_vectors_scalar_polygon(
        rect.x, rect.y, 4,
        &aabb.min_x, &aabb.min_y, &aabb.max_x, &aabb.max_y
    );
    return aabb;
}

f32 polygon_rectangle_get_width(PolygonRectangle rect){
    return vector2_dist_scalar(rect.x[0], rect.y[0], rect.x[1], rect.y[1]);
}

f32 polygon_rectangle_get_height(PolygonRectangle rect){
    return vector2_dist_scalar(rect.x[0], rect.y[0], rect.x[3], rect.y[3]);
}




/**====================
    functions: Circle
====================**//**/



void circle_get_min_max_vertices(f32 x, f32 y, f32 radius, f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y){
    *out_min_x = x - radius;
    *out_min_y = y - radius;
    *out_max_x = x + radius;
    *out_max_y = y + radius;
}

bool circle_overlaps_scalar(
    f32 lhs_x, f32 lhs_y, f32 lhs_radius,
    f32 rhs_x, f32 rhs_y, f32 rhs_radius,
    f32* out_normal_x, f32* out_normal_y, f32* out_depth
){
    *out_normal_x = INITIAL_NORMAL.x;
    *out_normal_y = INITIAL_NORMAL.y;

    f32 dist_sqrd = vector2_dist_sqrd_scalar(lhs_x, lhs_y, rhs_x, rhs_y);
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

    f32 dist = f32_sqrt(dist_sqrd);
    vector2_normalise_scalar(lhs_x - rhs_x, lhs_y - rhs_y, out_normal_x, out_normal_y);
    *out_depth = radius_sum - dist;
    return true;
}

bool circle_overlaps(Circle lhs, Circle rhs, Vector2* out_normal, f32* out_depth){
    return circle_overlaps_scalar(
        lhs.x, lhs.y, lhs.radius,
        rhs.x, rhs.y, rhs.radius,
        &out_normal->x, &out_normal->y, out_depth
    );
}

/*
    projects the edges of a circle onto a given axis.
*/
void circle_project_scalar(
    f32 circle_x, f32 circle_y, f32 circle_radius, f32 axis_x, f32 axis_y,
    f32* out_min_circle_edge, f32* out_max_circle_edge
){
    f32 dir_and_radius_x = axis_x * circle_radius;
    f32 dir_and_radius_y = axis_y * circle_radius;

    f32 vAX = circle_x + dir_and_radius_x;
    f32 vAY = circle_y + dir_and_radius_y;
    f32 vBX = circle_x - dir_and_radius_x;
    f32 vBY = circle_y - dir_and_radius_y;

    *out_min_circle_edge = vector2_dot_scalar(vAX, vAY, axis_x, axis_y);
    *out_max_circle_edge = vector2_dot_scalar(vBX, vBY, axis_x, axis_y);

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
void circle_project(Circle circle, Vector2 axis, f32* out_min_circle_edge, f32* out_max_circle_edge){
    circle_project_scalar(circle.x, circle.y, circle.radius, axis.x, axis.y, out_min_circle_edge, out_max_circle_edge);
}

/*
    calculates the contact point between two circles.
*/
void circle_calc_contact_points_scalar(f32 a_x, f32 a_y, f32 a_radius, f32 b_x, f32 b_y, f32* out_contact_point_x, f32* out_contact_point_y){
    f32 dist_x = b_x - a_x;
    f32 dist_y = b_y - a_y;
    f32 dir_x;
    f32 dir_y;
    vector2_normalise_scalar(dist_x, dist_y, &dir_x, &dir_y);

    // check for Nan in case the two circles are perfectly ontop of one another,
    // as normalising a distance of zero gives a NaN.
    *out_contact_point_x = a_x + (dir_x * a_radius);
    *out_contact_point_y = a_y + (dir_y * a_radius);
}

/*
    calculates the contact point between two circles.
*/
Vector2 circle_calc_contact_points(Circle a, Circle b){
    Vector2 result;
    circle_calc_contact_points_scalar(a.x, a.y, a.radius, b.x, b.y, &result.x, &result.y);
    return result;
}


///
/// @ Circle-Cast
///


bool circle_cast_overlaps_circle(
    f32 cast_start_x, f32 cast_start_y,
    f32 cast_end_x, f32 cast_end_y,
    f32 cast_radius,
    f32 circle_x, f32 circle_y,
    f32 circle_radius,
    f32* out_contact_point_x, f32* out_contact_point_y,
    f32* out_normal_x, f32* out_normal_y, 
    f32* out_depth
){
    f32 cast_dist_x = cast_end_x - cast_start_x;
    f32 cast_dist_y = cast_end_y - cast_start_y;
    
    // degenerate check.
    f32 cast_length_sqrd = vector2_dot_scalar(cast_dist_x, cast_dist_y, cast_dist_x, cast_dist_y);
    if(cast_length_sqrd < F32_EPSILON){
        return false;
    }

    f32 start_to_circle_x = circle_x - cast_start_x;
    f32 start_to_circle_y = circle_y - cast_start_y;
    
    // project the center of the circle onto the axis.
    // note that it is not clamped between 0-1 and is a ratio. 
    f32 t_mid = vector2_dot_scalar(start_to_circle_x, start_to_circle_y, cast_dist_x, cast_dist_y) / cast_length_sqrd;
    f32 mid_x = cast_start_x + (cast_dist_x * t_mid);
    f32 mid_y = cast_start_y + (cast_dist_y * t_mid);

    f32 perp_x = circle_x - mid_x;
    f32 perp_y = circle_y - mid_y;
    f32 perp_dist_sqrd =
        (perp_x * perp_x) +
        (perp_y * perp_y)
    ;
    f32 combined_radius = cast_radius + circle_radius;
    f32 radius_sqrd = combined_radius * combined_radius;
        
    // the project point on the line is not within the circle's radial area.
    if(perp_dist_sqrd > radius_sqrd){
        return false;
    } 
    
    // calculate the length to apply on ooposite axis to get the 'enter' and 'exit' points
    // this is done by getting half of the 'chord' (the line connecting to points on the circle edge).
    // note that it is done this way as the length of perpendicular vector is relative to its proximity to the circle center
    // meaning that the closer it is, the greater the magnitude it will be, resulting in a longer chord length; which is true 
    // in vice versa, shrinking as it moves away.
    f32 half_chord = f32_sqrt(radius_sqrd - perp_dist_sqrd); // note that this is in world-space.
    f32 half_cord_t = half_chord / f32_sqrt(cast_length_sqrd); // convert back into line-space (a ratio).
    // add epsilon to avoid false 'negatives' when checking the circle overlap later.
    // this is because the cast-circle's position is exactly it's radius away from the edge of the
    // circle we want to check against; (floating point inaccuracies can result in false 'negative's').  
    f32 t_enter = t_mid - half_cord_t + F32_EPSILON;
    
    // calculate the point at which to perform the circle overlap check.
    f32 cast_circle_x = cast_start_x + cast_dist_x * t_enter;
    f32 cast_circle_y = cast_start_y + cast_dist_y * t_enter;
        
    bool overlaps = circle_overlaps_scalar(
        cast_circle_x, cast_circle_y, cast_radius,
        circle_x, circle_y, circle_radius,
        out_normal_x, out_normal_y, out_depth
    );
    
    if(!overlaps){
        return false;        
    }

    circle_calc_contact_points_scalar(
        circle_x, circle_y, circle_radius, 
        cast_circle_x, cast_circle_y, 
        out_contact_point_x, out_contact_point_y
    );

    return true;
}

bool circle_cast_overlaps_line(
    f32 cast_start_x, f32 cast_start_y, f32 cast_dist_x, f32 cast_dist_y, f32 cast_radius,
    f32 line_start_x, f32 line_start_y, f32 line_dist_x, f32 line_dist_y,
    f32* out_line_t
){
    
    f32 line_len = f32_sqrt(line_dist_x * line_dist_x + line_dist_y * line_dist_y);    
    if(line_len <= F32_EPSILON){
        return false;
    }
    
    f32 inv_line_len = 1.0f / line_len;
    f32 normal_x = -line_dist_y * inv_line_len;
    f32 normal_y = line_dist_x * inv_line_len;
    
    // signed distance of the cast start from the line, and which side it is on (+1 or -1).
    // i.e, get the sign multplier to move the cast line closer to the line we want to check overlaps against. 
    f32 ds = vector2_dot_scalar(
        cast_start_x - line_start_x, 
        cast_start_y - line_start_y,
        normal_x,
        normal_y 
    );
    // get which side of the line the circle cast is on.
    f32 side = ds >= 0.0f ? 1.0f : -1.0f;
    f32 abs_ds = ds * side;
    
    // move the cast line towards the line we want to overlap, but never firther than the line itself (otherwise we over shoot it).
    // if the start is closer than `cast radius`, the shifted start lands on the line.
    f32 shift = MIN(cast_radius, abs_ds);
    f32 shift_x = normal_x * side * shift;
    f32 shift_y = normal_y * side * shift;
    f32 shifted_start_x = cast_start_x - shift_x + F32_EPSILON;
    f32 shifted_start_y = cast_start_y - shift_y + F32_EPSILON;
    cast_dist_x -= shift_x;
    cast_dist_y -= shift_y;
    
    { // shifted_line_overlaps_other_line();
    
        f32 dist_cross = vector2_cross_scalar(cast_dist_x, cast_dist_y, line_dist_x, line_dist_y);
    
        // if(dist_cross > -F32_EPSILON && dist_cross < F32_EPSILON){
        //     return false;
        // }
    
        f32 shifted_to_line_x = line_start_x - shifted_start_x;
        f32 shifted_to_line_y = line_start_y - shifted_start_y;
        
        /*
            by dividing the following two cross products by dist_cross, we are using dist_cross as a common denominator
            to normalise the measurement; i.e, bringing the "area" generated by the cross product operations into the 
            total parallelogram (dist_cross) space.
            
            note that dividing by the partial area (a_t or b_t; i.e, numerator) by the total area (dist_cross) the units
            cancel out, leaving a ratio (or percentage) value.  
        */
        
        // shifted_t = cross(a_to_b, b_dist) / dist_cross.
        // how far along 'shifted' that 'line' intersects with it.
        f32 shifted_t = vector2_cross_scalar(shifted_to_line_x, shifted_to_line_y, line_dist_x, line_dist_y) / dist_cross;
        
        // line_t = cross(a_to_b, a_dist) / dist_cross.
        // how far along 'line' that 'shifted' intersects with it.
        f32 line_t = vector2_cross_scalar(shifted_to_line_x, shifted_to_line_y, cast_dist_x, cast_dist_y) / dist_cross;
    
        /*
            'a_dist' and 'b_dist' are infinite lines (vectors) that intersect somewhere,
            however, that intersection must be inside both of the line segments bounds
            for it to count as 'hit'. 
        */
        if(
            shifted_t < 0.0f || 
            shifted_t > 1.0f ||
            line_t < 0.0f || 
            line_t > 1.0f
        ){
        
            f32 radius_sqrd = cast_radius * cast_radius;
            f32 p_x;
            f32 p_y;            

            { // check start to start.
            
                line_project_point(
                    cast_start_x, cast_start_y,
                    cast_dist_x, cast_dist_y,
                    line_start_x, line_start_y,
                    &p_x, &p_y
                );
            
                f32 dist_sqrd = vector2_dist_sqrd_scalar(line_start_x, line_start_y, p_x, p_y);
                if(dist_sqrd <= radius_sqrd){
                    *out_line_t = 0.0f;
                    return true;
                }
            }
            
            { // check end to end.
                
                f32 end_x = line_start_x + line_dist_x; 
                f32 end_y = line_start_y + line_dist_y;
                
                line_project_point(
                    cast_start_x, cast_start_y,
                    cast_dist_x, cast_dist_y,
                    end_x, end_y,
                    &p_x, &p_y
                );
            
                f32 dist_sqrd = vector2_dist_sqrd_scalar(end_x, end_y, p_x, p_y);
                if(dist_sqrd <= radius_sqrd){
                    *out_line_t = 1.0f;
                    return true;
                }
            }
        
            return false;
        }
        
        *out_line_t = line_t;
        return true;
    }
}



///
/// @ Polygon
///


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
    *out_max_edge = -F32_MAX;

    for(i32 i = 0; i < verts_size; i++){
        f32 projection = vector2_dot_scalar(verts_x[i], verts_y[i], axis_x, axis_y);

        if(projection < *out_min_edge){
            *out_min_edge = projection;
        }
        if(projection > *out_max_edge){
            *out_max_edge = projection;
        }
    }
}

bool line_overlaps_polygon(
    f32 line_start_x, f32 line_start_y,
    f32 line_end_x, f32 line_end_y,
    f32* poly_vert_x, f32* poly_vert_y, i32 poly_vert_length,
    f32* out_contact_point_1_x, f32* out_contact_point_1_y,
    f32* out_contact_point_2_x, f32* out_contact_point_2_y,
    f32* out_normal_1_x, f32* out_normal_1_y,
    f32* out_normal_2_x, f32* out_normal_2_y,
    f32* out_depth_1,
    f32* out_depth_2,
    i32* out_contact_point_count
){    

    f32 line_t;
    f32 line_dist_x     = line_end_x - line_start_x;
    f32 line_dist_y     = line_end_y - line_start_y;

    f32 poly_start_x;
    f32 poly_start_y; 
    f32 poly_end_x;
    f32 poly_end_y;
    f32 poly_dist_x;
    f32 poly_dist_y;

    i32 contact_point_count = 0;

    for(i32 idx = 0; idx < poly_vert_length; idx++){
        i32 next_idx = (idx + 1 == poly_vert_length) ? 0 : idx + 1;
        
        poly_start_x    = poly_vert_x[idx];
        poly_end_x      = poly_vert_x[next_idx];
        poly_start_y    = poly_vert_y[idx];
        poly_end_y      = poly_vert_y[next_idx];    
        poly_dist_x     = poly_end_x - poly_start_x;
        poly_dist_y     = poly_end_y - poly_start_y;
        f32 poly_dir_x;
        f32 poly_dir_y;

        bool overlaps = line_overlaps_line(
            line_start_x, line_start_y, line_dist_x, line_dist_y,
            poly_start_x, poly_start_y, poly_dist_x, poly_dist_y,
            &line_t
        );
        
        if(!overlaps){
            continue;
        }
        
        if(!contact_point_count){
            contact_point_count = 1;

            *out_contact_point_1_x = line_start_x + (line_dist_x * line_t);
            *out_contact_point_1_y = line_start_y + (line_dist_y * line_t);
            
            vector2_normalise_scalar(poly_dist_x, poly_dist_y, &poly_dir_x, &poly_dir_y);
        
            *out_normal_1_x = -poly_dir_y;
            *out_normal_1_y = poly_dir_x;
        }
        else{
            
            contact_point_count = 2;
            
            *out_contact_point_2_x = line_start_x + (line_dist_x * line_t);
            *out_contact_point_2_y = line_start_y + (line_dist_y * line_t);

            vector2_normalise_scalar(poly_dist_x, poly_dist_y, &poly_dir_x, &poly_dir_y);        

            *out_normal_2_x = -poly_dir_y;
            *out_normal_2_y = poly_dir_x;
            
            f32 dist_1 = vector2_dist_sqrd_scalar(*out_contact_point_1_x, *out_contact_point_1_y, line_start_x, line_start_y);
            f32 dist_2 = vector2_dist_sqrd_scalar(*out_contact_point_2_x, *out_contact_point_2_y, line_start_x, line_start_y);
                                                
            // swap the contact points around if they are 
            if(dist_2 < dist_1){
                
                f32 temp_contact_point_x    = *out_contact_point_1_x;
                f32 temp_contact_point_y    = *out_contact_point_1_y;
                f32 temp_normal_x           = *out_normal_1_x; 
                f32 temp_normal_y           = *out_normal_1_y;
                
                *out_contact_point_1_x  = *out_contact_point_2_x;
                *out_contact_point_1_y  = *out_contact_point_2_y;
                *out_normal_1_x         = *out_normal_2_x; 
                *out_normal_1_y         = *out_normal_2_y;
                
                *out_contact_point_2_x   = temp_contact_point_x;
                *out_contact_point_2_y   = temp_contact_point_y;
                *out_normal_2_x          = temp_normal_x; 
                *out_normal_2_y          = temp_normal_y;                
            }           
            break;
        }
            
    }

    *out_contact_point_count = contact_point_count;
    if(contact_point_count > 0){
        
        // ordering matters for depth calculations.
        // so we do them here at the end.
        
        *out_depth_1 = vector2_dist_scalar(
            *out_contact_point_1_x, *out_contact_point_1_y,
            line_start_x, line_start_y
        );

        if(contact_point_count > 1){
            *out_depth_2 = vector2_dist_scalar(
                *out_contact_point_1_x, *out_contact_point_1_y,
                line_end_x, line_end_y
            );
        }

        return true;
    }
    return false;    
}

bool circle_cast_overlaps_polygon(
    f32 cast_start_x, f32 cast_start_y, f32 cast_end_x, f32 cast_end_y, f32 cast_radius,
    f32* poly_vert_x, f32* poly_vert_y, i32 poly_vert_length,
    f32* out_contact_point_1_x, f32* out_contact_point_1_y,
    f32* out_contact_point_2_x, f32* out_contact_point_2_y,
    f32* out_normal_1_x, f32* out_normal_1_y,
    f32* out_normal_2_x, f32* out_normal_2_y,
    f32* out_depth_1,
    f32* out_depth_2,
    i32* out_contact_point_count
){    

    f32 poly_t;
    f32 cast_dist_x     = cast_end_x - cast_start_x;
    f32 cast_dist_y     = cast_end_y - cast_start_y;

    f32 poly_start_x;
    f32 poly_start_y; 
    f32 poly_end_x;
    f32 poly_end_y;
    f32 poly_dist_x;
    f32 poly_dist_y;

    f32 temp_contact_point_x;
    f32 temp_contact_point_y;
    f32 temp_normal_x;
    f32 temp_normal_y;
    f32 temp_depth;
    f32 temp_dist;

    i32 contact_point_count = 0;

    for(i32 idx = 0; idx < poly_vert_length; idx++){
        i32 next_idx = (idx + 1 == poly_vert_length) ? 0 : idx + 1;
        
        poly_start_x    = poly_vert_x[idx];
        poly_end_x      = poly_vert_x[next_idx];
        poly_start_y    = poly_vert_y[idx];
        poly_end_y      = poly_vert_y[next_idx];    
        poly_dist_x     = poly_end_x - poly_start_x;
        poly_dist_y     = poly_end_y - poly_start_y;
        f32 poly_dir_x;
        f32 poly_dir_y;

        bool overlaps = circle_cast_overlaps_line(
            cast_start_x, cast_start_y, cast_dist_x, cast_dist_y, cast_radius,
            poly_start_x, poly_start_y, poly_dist_x, poly_dist_y,
            &poly_t
        );
        
        if(!overlaps){
            continue;
        }
        
        f32 contact_point_x; 
        f32 contact_point_y; 
        f32 normal_x;
        f32 normal_y;
                
        if(poly_t <= 0.0f || poly_t >= 1.0f){
        
            /*
                claude did this,
                have no idea how it works, have fun if it breaks :)))) 
            */
        
            // vertex hit: the edge normal is wrong here. the correct normal is the
            // direction from the vertex to the circle's centre at the moment of contact.
        
            // snap to the exact vertex so both edges sharing a corner report identical points.
            // (`start + dist * 1.0f` isn't guaranteed to land exactly on `end` in float maths.)
            bool hit_start  = poly_t <= 0.0f;
            contact_point_x = hit_start ? poly_start_x : poly_end_x;
            contact_point_y = hit_start ? poly_start_y : poly_end_y;
        
            // vector from the vertex to the cast start.
            f32 vertex_to_start_x = cast_start_x - contact_point_x;
            f32 vertex_to_start_y = cast_start_y - contact_point_y;
        
            // where along the cast (0 = start, 1 = end) the circle first touches the vertex.
            // stays 0 if the cast has no length, since the circle just sits at its start.
            f32 vertex_t = 0.0f;
        
            f32 cast_len_sqrd = cast_dist_x * cast_dist_x + cast_dist_y * cast_dist_y;
            
            if(cast_len_sqrd > F32_EPSILON){
                // solve |vertex_to_start + cast_dist * t|^2 = radius^2 for t (a quadratic).
                //   half_b: is the circle of the cast heading towards the vertex (negative) or away (positive)?
                //   c:      gap between the circle's edge and the vertex at the start (negative = overlapping).
                //   disc:   does the path get within `radius` of the vertex at all? (negative = no)
                f32 half_b = vector2_dot_scalar(vertex_to_start_x, vertex_to_start_y, cast_dist_x, cast_dist_y);
                f32 c      = vector2_dot_scalar(vertex_to_start_x, vertex_to_start_y, vertex_to_start_x, vertex_to_start_y)
                           - cast_radius * cast_radius;
                f32 disc   = half_b * half_b - cast_len_sqrd * c;
        
                // smaller root = first touch. if disc < 0 the path never reaches the vertex,
                // so clamping it to 0 makes this collapse to -half_b / cast_len_sqrd,
                // which is the closest approach, our fallback instead of rejecting the hit.
                vertex_t = (-half_b - f32_sqrt(MAX(disc, 0.0f))) / cast_len_sqrd;
        
                // keep inside the cast: t < 0 means already overlapping at the start,
                // t > 1 means the contact is past the end of the cast.
                vertex_t = MAX(0.0f, MIN(1.0f, vertex_t));
            }
        
            // centre at the moment of contact, relative to the vertex.
            // it points from the vertex out to the centre, so it is the (unnormalised) normal.
            normal_x = vertex_to_start_x + cast_dist_x * vertex_t;
            normal_y = vertex_to_start_y + cast_dist_y * vertex_t;
        
            // normalise. a ~0 length means the centre is on top of the vertex: no direction exists.
            f32 normal_len = f32_sqrt(normal_x * normal_x + normal_y * normal_y);
            if(normal_len <= F32_EPSILON){
                continue;
            }
            normal_x /= normal_len;
            normal_y /= normal_len;
        }
        else{
            vector2_normalise_scalar(poly_dist_x, poly_dist_y, &poly_dir_x, &poly_dir_y);
            contact_point_x = poly_start_x + poly_dist_x * poly_t;
            contact_point_y = poly_start_y + poly_dist_y * poly_t;
            // edge body hit: outward edge normal.
            normal_x = -poly_dir_y;   // see winding note below
            normal_y = poly_dir_x;
        }
        
        if(cast_dist_x * normal_x + cast_dist_y * normal_y >= 0.0f){
            continue;
        }
                
        // set to zero because idk
        // the circle is always on the edge of the polygon edge
        // so there shouldnt be a depth.
        f32 depth = 0.0f;
        
        switch(contact_point_count){
            
            default:{
                ASSERT(false, "unknown contact point count");
            }break;
            case 0:{
                contact_point_count = 1;
                *out_contact_point_1_x = contact_point_x;
                *out_contact_point_1_y = contact_point_y;        
                *out_normal_1_x = normal_x;
                *out_normal_1_y = normal_y;                
                *out_depth_1 = depth;
            }break;
            case 1:{
                contact_point_count = 2;
                                
                f32 dist_n = vector2_dist_sqrd_scalar(contact_point_x, contact_point_y, cast_start_x, cast_start_y);
                f32 dist_1 = vector2_dist_sqrd_scalar(*out_contact_point_1_x, *out_contact_point_1_y, cast_start_x, cast_start_y);
                if(dist_n < dist_1){
                    temp_contact_point_x    = *out_contact_point_1_x;
                    temp_contact_point_y    = *out_contact_point_1_y;
                    temp_normal_x           = *out_normal_1_x;
                    temp_normal_y           = *out_normal_1_y;
                    temp_depth              = *out_depth_1;
                    
                    *out_contact_point_1_x  = contact_point_x;
                    *out_contact_point_1_y  = contact_point_y;
                    *out_normal_1_x         = normal_x;
                    *out_normal_1_y         = normal_y;
                    *out_depth_1            = depth;
                    
                    *out_contact_point_2_x  = temp_contact_point_x;
                    *out_contact_point_2_y  = temp_contact_point_y;
                    *out_normal_2_x         = temp_normal_x;
                    *out_normal_2_y         = temp_normal_y;
                    *out_depth_2            = temp_depth;
                }
                else{
                    *out_contact_point_2_x  = contact_point_x;
                    *out_contact_point_2_y  = contact_point_y;
                    *out_normal_2_x         = normal_x;
                    *out_normal_2_y         = normal_y;
                    *out_depth_2            = depth;
                }
            }break;
            case 2:{
            
                f32 dist_n = vector2_dist_sqrd_scalar(contact_point_x, contact_point_y, cast_start_x, cast_start_y);
                f32 dist_1 = vector2_dist_sqrd_scalar(*out_contact_point_1_x, *out_contact_point_1_y, cast_start_x, cast_start_y);
                f32 dist_2 = vector2_dist_sqrd_scalar(*out_contact_point_2_x, *out_contact_point_2_y, cast_start_x, cast_start_y);
            
                if(dist_n < dist_1){
                    temp_contact_point_x    = *out_contact_point_1_x;
                    temp_contact_point_y    = *out_contact_point_1_y;
                    temp_normal_x           = *out_normal_1_x;
                    temp_normal_y           = *out_normal_1_y;
                    temp_depth              = *out_depth_1;
                    temp_dist               = dist_1;
                        
                    *out_contact_point_1_x  = contact_point_x;
                    *out_contact_point_1_y  = contact_point_y;
                    *out_normal_1_x         = normal_x;
                    *out_normal_1_y         = normal_y;
                    *out_depth_1            = depth;
    
                    // perform swap.            
                    contact_point_x         = temp_contact_point_x;
                    contact_point_y         = temp_contact_point_y;
                    normal_x                = temp_normal_x;
                    normal_y                = temp_normal_y;
                    depth                   = temp_depth;
                    dist_1                  = dist_n;
                    dist_n                  = temp_dist;
                }
                if(dist_n < dist_2){
                    *out_contact_point_2_x  = contact_point_x;
                    *out_contact_point_2_y  = contact_point_y;
                    *out_normal_2_x         = normal_x;
                    *out_normal_2_y         = normal_y;
                    *out_depth_2            = depth;
                }
            }break;
        }           
    }

    *out_contact_point_count = contact_point_count;
    if(contact_point_count > 0){
        
        // ordering matters for depth calculations.
        // so we do them here at the end.
        
        *out_depth_1 = vector2_dist_scalar(
            *out_contact_point_1_x, *out_contact_point_1_y,
            cast_start_x, cast_start_y
        );

        if(contact_point_count > 1){
            *out_depth_2 = vector2_dist_scalar(
                *out_contact_point_1_x, *out_contact_point_1_y,
                cast_end_x, cast_end_y
            );
        }

        return true;
    }
    return false;    
}

bool polygon_overlaps_point_scalar(
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
        f32 point_proj = vector2_dot_scalar(point_x, point_y, axis_x, axis_y);

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

    f32 mag = f32_sqrt(min_mag_sqrd);
    *out_depth = min_axis_depth / mag; // Only one sqrt if this is the new minimum translation distance.
    *out_normal_x = min_axis_x / mag;
    *out_normal_y = min_axis_y / mag;

    return true;
}

/*
    remarks:
    `out_normal` and `out_depth` are relative to polygon `a`.

    this is an internal function for `polygon_overlaps()`.
    do NOT use this function; instead use said function instead.
*/
bool polygon_overlaps_one_way(
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
    f32 a_max_edge      = -F32_MAX;
    f32 b_max_edge      = -F32_MAX;

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

    f32 mag = f32_sqrt(min_mag_sqrd);
    *out_depth      = min_axis_depth / mag; // Only one sqrt if this is the new minimum translation distance.
    *out_normal_x   = min_axis_x / mag;
    *out_normal_y   = min_axis_y / mag;

    return true;
}

bool polygon_overlaps(
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


    bool lhs_overlaps_rhs = polygon_overlaps_one_way(
        lhs_verts_x, lhs_verts_y, lhs_verts_size,
        rhs_verts_x, rhs_verts_y, rhs_verts_size,
        &found_normal_x, &found_normal_y, &found_depth
    );

    if (lhs_overlaps_rhs){
        if(*out_depth > found_depth){
            *out_depth = found_depth;
            *out_normal_x = found_normal_x;
            *out_normal_y = found_normal_y;
        }
    }
    else{
        return false;
    }

    bool rhs_overlaps_lhs = polygon_overlaps_one_way(
        rhs_verts_x, rhs_verts_y, rhs_verts_size,
        lhs_verts_x, lhs_verts_y, lhs_verts_size,
        &found_normal_x, &found_normal_y, &found_depth
    );

    if (rhs_overlaps_lhs){
        if(*out_depth > found_depth){
            *out_depth = found_depth;
            *out_normal_x = found_normal_x;
            *out_normal_y = found_normal_y;
        }
    }
    else{
        return false;
    }

    // when a new smaller
    // depth is found but in relation to rect B, not A.
    // this is so that the resolution code will always push A out of B
    // and not push the two i32o each other when a smaller depth is found when
    // looping through rect B.
    if(vector2_dot_scalar(rhs_centroid_x - lhs_centroid_x, rhs_centroid_y - lhs_centroid_y, *out_normal_x, *out_normal_y) >= 0){
        *out_normal_x *= -1.0f;
        *out_normal_y *= -1.0f;
    }

    return true;
}

/*
    remarks:
    this is an internal function for `polygon_find_contact_points()`
    do NOT use this function; instead use said function instead.
*/
void polygon_find_contact_points_one_way(
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

            line_segment_closest_point_scalar(edge_start_x, edge_start_y, edge_end_x, edge_end_y, point_x, point_y, &closest_point_x, &closest_point_y);
            f32 dist_sqrd = vector2_dist_sqrd_scalar(point_x, point_y, closest_point_x, closest_point_y);

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
void polygon_find_contact_points(
    f32* a_vert_x, f32* a_vert_y, i32 a_verts_length,
    f32* b_vert_x, f32* b_vert_y, i32 b_verts_length,
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
    polygon_find_contact_points_one_way(
        a_vert_x, a_vert_y, a_verts_length,
        b_vert_x, b_vert_y, b_verts_length,
        epsilon, &min_dist_sqrd, out_contact_point_1_x, out_contact_point_1_y,
        out_contact_point_2_x, out_contact_point_2_y, out_contact_point_count
    );

    // polygon b to a.
    polygon_find_contact_points_one_way(
        b_vert_x, b_vert_y, b_verts_length,
        a_vert_x, a_vert_y, a_verts_length,
        epsilon, &min_dist_sqrd, out_contact_point_1_x, out_contact_point_1_y,
        out_contact_point_2_x, out_contact_point_2_y, out_contact_point_count
    );
}

bool polygon_overlaps_circle_scalar(
    f32* poly_vert_x, f32* poly_vert_y, i32 poly_vert_length, f32 poly_centroid_x, f32 poly_centroid_y,
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

    for(i32 i = 0; i < poly_vert_length; i++){
        i32 current_index = i;
        i32 next_index = i+1;

        // this is faster than modulo.
        if(next_index >= poly_vert_length)
            next_index = 0;

        f32 curr_x = poly_vert_x[current_index];
        f32 next_x = poly_vert_x[next_index];
        f32 curr_y = poly_vert_y[current_index];
        f32 next_y = poly_vert_y[next_index];

        f32 edge_x = next_x - curr_x;
        f32 edge_y = next_y - curr_y;

        // the normal of the edge.
        // note: this only works as vertices are assumed to be in clockwise winding order.
        // change to new Vector2(edge.y, -edge.x); if anti-clockwise.
        axis_x = -edge_y;
        axis_y = edge_x;

        // normalize (important for correct depth).
        vector2_normalise_scalar(axis_x, axis_y, &axis_x, &axis_y);

        // project all vertices onto the current edge to find the min and max values
        // of the two rectangles along the edge.
        project_polygon(poly_vert_x, poly_vert_y, poly_vert_length, axis_x, axis_y, &a_min, &a_max);
        circle_project_scalar(circle_x, circle_y, circle_radius, axis_x, axis_y, &b_min, &b_max);

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

    i32 closest_point_index = find_closest_vertex_scalar_polygon(circle_x, circle_y, poly_vert_x, poly_vert_y, poly_vert_length);
    f32 closest_point_x = poly_vert_x[closest_point_index];
    f32 closest_point_y = poly_vert_y[closest_point_index];

    axis_x = closest_point_x - circle_x;
    axis_y = closest_point_y - circle_y;
    vector2_normalise_scalar(axis_x, axis_y, &axis_x, &axis_y);

    // project all vertices onto the current edge to find the min and max values
    // of the two rectangles along the edge.
    project_polygon(poly_vert_x, poly_vert_y, poly_vert_length, axis_x, axis_y, &a_min, &a_max);
    circle_project_scalar(circle_x, circle_y, circle_radius, axis_x, axis_y, &b_min, &b_max);

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
    if(vector2_dot_scalar(dist_x, dist_y, *out_normal_x,  *out_normal_y) >= 0){
        *out_normal_x *= -1.0f;
        *out_normal_y *= -1.0f;
    }

    return true;
}

/**
    Finds the contact points between a polygon and a point.
**/
void polygon_find_point_contact_points_scalar(
    f32* poly_vert_x, f32* poly_vert_y, i32 poly_vert_length,
    f32 point_x, f32 point_y,
    f32* out_contact_point_x, f32* out_contact_point_y
){
    *out_contact_point_x = F32_MAX;
    *out_contact_point_y = F32_MAX;
    f32 min_dist_sqrd = F32_MAX;

    f32 closest_point_x;
    f32 closest_point_y;
    f32 dist_sqrd;

    // find the closest point for each edge of the rectangle.
    for(int start_idx = 0; start_idx < poly_vert_length; start_idx++){
        int end_idx = start_idx + 1;

        // this is faster than modulo.
        if(end_idx >= poly_vert_length){
            end_idx = 0;
        }

        BOUNDS_CHECK(start_idx, poly_vert_length);
        BOUNDS_CHECK(end_idx, poly_vert_length);
        line_segment_closest_point_scalar(
            poly_vert_x[start_idx], poly_vert_y[start_idx],
            poly_vert_x[end_idx], poly_vert_y[end_idx],
            point_x, point_y, &closest_point_x, &closest_point_y
        );

        dist_sqrd = vector2_dist_sqrd_scalar(point_x, point_y, closest_point_x, closest_point_y);

        if(dist_sqrd < min_dist_sqrd){
            min_dist_sqrd = dist_sqrd;
            *out_contact_point_x = closest_point_x;
            *out_contact_point_y = closest_point_y;
        }
    }
}

bool polygon_overlaps_circle(Polygon polygon, Vector2 polygon_centroid, Circle circle, Vector2* out_normal, f32* out_depth){
    return polygon_overlaps_circle_scalar(
        polygon.x, polygon.y, polygon.length, polygon_centroid.x, polygon_centroid.y,
        circle.x, circle.y, circle.radius,
        &(*out_normal).x, &(*out_normal).y, out_depth
    );
}

/*
    finds the contact points between a polygon and a query-point.
*/
void find_point_contact_points_scalar_polygon(
    f32* poly_verts_x, f32* poly_verts_y, i32 poly_verts_length,
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
    for(i32 start_idx = 0; start_idx < poly_verts_length; start_idx++){
        i32 next_idx = start_idx + 1;
        // this is faster than modulo.
        if(next_idx >= poly_verts_length)
            next_idx = 0;

        line_segment_closest_point_scalar(
            poly_verts_x[start_idx], poly_verts_y[start_idx], poly_verts_x[next_idx], poly_verts_y[next_idx],
            point_x, point_y, &closest_point_x, &closest_point_y
        );

        dist_sqrd = vector2_dist_sqrd_scalar(point_x, point_y, closest_point_x, closest_point_y);

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

/**
    Gets the min and max vectors from an array of vertices.
**/
void polygon_get_min_max_vertices(f32* vert_x, f32* vert_y, i32 vert_length, f32* out_min_x, f32* out_min_y, f32* out_max_x, f32* out_max_y){
    *out_min_x = F32_MAX;
    *out_min_y = F32_MAX;
    *out_max_x = -F32_MAX;
    *out_max_y = -F32_MAX;

    for(i32 i = 0; i < vert_length; i++){
        f32 v = vert_x[i];
        if (v < *out_min_x){
            *out_min_x = v;
        }
        if(v > *out_max_x){
            *out_max_x = v;
        }
    }

    for(i32 i = 0; i < vert_length; i++){
        f32 v = vert_y[i];
        if(v < *out_min_y){
            *out_min_y = v;
        }
        if(v > *out_max_y){
            *out_max_y = v;
        }
    }
}


///
/// @ rand globals.
///


u32 base_rand_state;
bool base_rand_initial_state_set;
void * (*base_panic_pre_abort_funcptr) (char* msg);


///
/// @ rand
///


/*
    sets the global random seed to a new seed.

    returns:
    the newl set seed.
*/
i32 rand_next_seed(){
    // get the current calender time.
    time_t raw_time;
    time(&raw_time);
    struct tm *t = localtime(&raw_time);

    i32 second  = t->tm_sec;
    i32 minute  = t->tm_min;
    i32 hour    = t->tm_hour;
    i32 day     = t->tm_mday;
    i32 month   = t->tm_mon+1; // +1 for 1-12; not 0-11
    i32 year    = t->tm_year+1980; // years since 1980.

    // Use the stack address of a local variable as entropy noise
    i32 stack_noise;
    i32 result = (i32)(uintptr_t)&stack_noise;

    result += second;
    result += minute;
    result += hour;
    result += day;
    result += month;
    result += year;

    base_rand_state = result;
    return result;
}

u32 u32_rand(){
    // lazy init the first seed.
    if(base_rand_initial_state_set == false){
        rand_next_seed();
        base_rand_initial_state_set = true;
    }

    u32 result = base_rand_state;
	result ^= result << 13u;
	result ^= result >> 17u;
	result ^= result << 5u;
    base_rand_state = result;
	return result;
}

i32 i32_rand(){
	return (i32)u32_rand();
}

/*
    `remarks`
    generated values are never fractional or negative; user f32_rand_range for more granual control of numbers.
*/
f32 f32_rand(){
    return (f32)(u32_rand() >> 8);
}

f32 f32_rand_range(f32 min, f32 max){
    ASSERT(min <= max, "min is greater than max.");
    // shift down 8 bits to push the sign and exponent bits into the mantissa.
    // ranging the generated value in [0 to 1].
    f32 nv = f32_rand() * (1.0f / 16777216.0f);
    return min + nv * (max - min);
}

i32 i32_srand(i32 seed){
    base_rand_state = seed;
    base_rand_initial_state_set = true;
    return i32_rand();
}

f32 f32_srand(i32 seed){
    base_rand_state = seed;
    base_rand_initial_state_set = true;
    return f32_rand();
}