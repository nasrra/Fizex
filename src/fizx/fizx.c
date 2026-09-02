/**
    TODO: (nich s)
    - add entity type checks to body and shape functions.
**/


/**====================
    types.
====================**//**/


typedef enum{
    ShapeBehaviour_Dynamic,
    ShapeBehaviour_Kinematic,
    ShapeBehaviour_Trigger
} ShapeBehaviour;

/**
    `remarks`
    ordering matters here, value 0 is highest precedence in

    TODO: write this in C from C#.
    <see cref="PhysicsSystem.FormatCategorisedOverlaps(Howl.CategorisedLeafOverlaps, System.Span{int}, System.Span{int})"/>.
**/
typedef enum{
    ShapeCategory_DynRigPolygon,
    ShapeCategory_DynRigCircle,
    ShapeCategory_DynRigCapsule,
    ShapeCategory_TriRigPolygon,
    ShapeCategory_TriRigCircle,
    ShapeCategory_TriRigCapsule,
    // note: everything greater than KinematicRigidPolygon
    // is not apart of the rigid body movement step.
    ShapeCategory_KinRigPolygon,
    ShapeCategory_KinRigCircle,
    ShapeCategory_KinRigCapsule,
    ShapeCategory_DynColPolygon,
    ShapeCategory_DynColCircle,
    ShapeCategory_DynColCapsule,
    ShapeCategory_TriColPolygon,
    ShapeCategory_TriColCircle,
    ShapeCategory_TriColCapsule,
    ShapeCategory_KinColPolygon,
    ShapeCategory_KinColCircle,
    ShapeCategory_KinColCapsule,
    ShapeCategory_Count
} ShapeCategory;

/**
    A pair of indices that are registered within a collision manifold.
**/
typedef struct {
    i32 a_to_b;
    i32 b_to_a;
} IndexPair;

typedef enum{
    CollisionResolutionCategory_Dynamic,
    CollisionResolutionCategory_Kinematic,
    CollisionResolutionCategory_Count
} CollisionResolutionCategory;

typedef enum{
    /**
        There is no collision between the two bodies.
    **/
    ContactState_None,
    /**
        The two colliders have just started contacting with one another.
    **/
    ContactState_Enter,
    /**
        The two colliders are in sustained contact with one another.
    **/
    ContactState_Sustain,
    /**
        The two colliders have just left contact with one another.
    **/
    ContactState_Exit
} ContactState;

typedef enum{
    ShapeType_Circle,
    ShapeType_Rectangle
} ShapeType;

typedef struct{
    f32* static_friction;
    f32* kinetic_friction;
    f32* density;
    f32* restitution;
    i32 length;
    bool is_init;
} Soa_Material;

typedef enum {

    MovementStepConfig_LinearVelocityOnly,
    MovementStepConfig_DisplacementOnly,
    MovementStepConfig_Full

} MovementStepConfig;

typedef enum {
    /**
        -----------------
        | Connections:  |
        |---------------|
        | Shape | Body  |
        |-------|-------|
        |   0   |   1   |
        |---------------|
    **/
    EntityType_Shape,
    /**
        -----------------
        | Connections:  |
        |---------------|
        | Shape | Body  |
        |-------|-------|
        |   N   |   0   |
        |---------------|
    **/
    EntityType_Body,
} EntityType;

typedef struct {
    Colour colour_dynamic_shape;
    Colour colour_passive_trigger_shape;
    Colour colour_kinematic_shape;
    Colour colour_active_trigger_shape;
    Colour colour_aabb;
    Colour colour_fallback_shape;
    Colour colour_inactive_physics_body;
    Colour colour_bvh_leaf_aabb;
    Colour colour_bvh_branch_aabb;
    Colour colour_contact_point;
    Colour colour_linear_velocity;
    Colour colour_position;
    Colour colour_centroid;
    Colour colour_collision_other;
    Colour colour_collision_normal;
    Colour colour_center_of_mass;
    i32 sprite_layer;
    i32 camera_id;
    f32 position_z;
    i32 material;
} DrawInfo;

typedef struct{
    /**
        Friction has two values: kinetic and static.
        this is because - in the real world - objects require much more
        initial force to start moving compared to when they would already be
        in motion. This is simulated as two friction values.
    **/
    /****/
    f32 static_friction;
    f32 kinetic_friction;
    f32 density;
    f32 restitution;
} Material;

typedef struct{
    /**
        the base - untransformed - vertices for all shapes.

        `remarks`
        - fixed-stride array structured.
        - elements are accessed via `vertex_index`.
    **/
    FsSoa_Vector2 base_vertex;
    /**
        The global-space vertices for all shapes.

        `remarks`
        - fixed-stride array structured.
        - Elements are accessed via `vertex_index`.
    **/
    FsSoa_Vector2 global_vertex;
    /**
        The local-space transforms for all entities.

        `remarks`
       Elements are accessed via `body_index`
    **/
    Soa_Transform2D local_transform;
    /**
        The global-space transforms for all entities.

        `remarks`
        Elements are accessed via `body_index`
    **/
    Soa_Transform2D global_transform;
    /**
        The positions of entities from the previous step.

        `remarks`
        Elements are accessed via `body_index`
    **/
    Soa_Vector2 previous_step_position;
    /**
        The force values that will be applied in to rigidbodies.
        `remarks`
       Elements are accessed via `body_index`.
    **/
    Soa_Vector2 force;
    /**
        The linear velocity values for all rigidbodies.
    **/
    Soa_Vector2 linear_velocity;
    /**
        The centroids of all shapes; in global-space.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    Soa_Vector2 centroid;
    /**
        The center of masses of all bodies; relative to their global position.

        `remarks`
       Elements are accessed via `body_index`
    **/
    Soa_Vector2 local_center_of_mass;
    /**
        The Axis Aligned Bounding Boxes of all shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    Soa_Aabb aabb;
    /**
        The physics materials for all shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    Soa_Material material;
    /**
        the angular velocities for all rigidbodies.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* angular_velocity;
    i32 angular_velocity_length;
    /**
        The mass values of all rigid bodies and their associated shapes.

        `remarks`
        Elements are accessed via `entityIndex`.
    **/
    f32* mass;
    i32 mass_length;
    /**
        The inverse mass values of all rigid bodies and their associated shapes.

        `remarks`
        Elements are accessed via `entityIndex`.
    **/
    f32* inverse_mass;
    i32 inverse_mass_length;
    /**
        The base width values of all rectangle shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* base_width;
    i32 base_width_length;
    /**
        The base height values of all rectangle shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* base_height;
    i32 base_height_length;
    /**
        The base radii values of all circle shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* base_radius;
    i32 base_radius_length;
    /**
        The global-space radii values of all circle shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* global_radius;
    i32 global_radius_length;
    /**
        The rotational inertia values of all rigidbodies.

        `remarks`
        Elements are accessed via `entityIndex`.
    **/
    f32* rotational_inertia;
    i32 rotational_inertia_length;
    /**
        The inverse rotational inertia values of all rigidbodies.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    f32* inverse_rotational_inertia;
    i32 inverse_rotational_inertia_length;
    /**
        The generations of all bodies.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    i32* generation;
    i32 generation_length;
    /**
        The categories of all shapes.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    i32* category;
    i32 category_length;

    /**
        The bvh indices of all shapes.

        `remarks`
        Elements are accessed via `body_index`
    **/
    i32* bvh_leaf_index;
    i32 bvh_leaf_index_length;
    /**
        The padding of all shapes to apply to their AABB when inserted into the bvh..

        `remarks`
        Elements are accessed via `entityIndex`.
    **/
    f32* bvh_leaf_padding;
    i32 bvh_leaf_padding_length;
    /**
        The shape value of all rigid shapes.

       `remarks`
       Elements are accessed via `body_index`.
    **/
    ShapeType* shape_type;
    i32 shape_type_length;
    /**
        Whether a rigidbody uses rotational response.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    bool* rotational_response;
    i32 rotational_response_length;
    /**
        The types of all entities.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    EntityType* entity_type;
    i32 entity_type_length;
    /**
        Whether or not an entity is active.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    bool* active;
    i32 active_length;
    /**
        Whether or not a body is gravity affected.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    bool* gravity_affected;
    i32 gravity_affected_length;
    /**
        All bodies shape collision displacement vectors.

        `remarks`
        Elements are accessed via `body_index`.
    **/
    Soa_Vector2 shape_collision_displacement;
    /**
        The indices of bodies that have been displaced this substep.

        `remarks`
        this is a stack array.
    **/
    i32* displaced_this_sub_step;
    i32 displaced_this_sub_step_length;
    i32 displaced_this_sub_step_count;
    bool is_init;
} Soa_Body;

typedef struct{
    /**
        The normal vector of a collision.
    **/
    Soa_Vector2 normal;
    /**
        The centroids of the colliding physics bodys.
    **/
    Soa_Vector2 collider_centroid; // this should be removed and use the physics system state centroids instead.
    /**
        The first contact poi32 of all collisions.
    **/
    Soa_Vector2 first_contact_point;
    /**
        The second contact poi32 of all collisions.
    **/
    Soa_Vector2 second_contact_point;
    /**
        The depth of the collisions.
    **/
    f32* depth;
    i32 depth_length;
    /**
        Whether or not a collision has a second contact point.
    **/
    bool* two_contact_points;
    i32 two_contact_points_length;
    /**
        The indices of `active` collision elements separated by `chunk` in the current step.

        `remarks`
        fixed-stride-swapback array formatted.
    **/
    FixedStrideArray active_index;
    /**
        The `phase` a collision element is of being `active`.

        `remarks`
        Value Key:
            - 0: the element has not been active at all.
            - 1: the element is active in the `current` step; meaning there is contact between the two colliders.
            - 2: the element is active in the `previous` step; meaning the contact between the two colliders has just stopped.
            - 3: the element is active in the `preultimate` step; meaning the contact between the two colliders has completely ceased.
    **/
    i32* active_phase;
    i32 active_phase_length;
    /**
        The state of all collisions this step.
    **/
    ContactState* contact_state;
    i32 contact_state_length;
    /**
        The state of all collisions in the previous step.
    **/
    ContactState* previous_contact_state;
    i32 previous_contact_state_length;
    /**
        The fixed stride of each entry.
    **/
    i32 collider_stride;
    /**
        The amount of entries this collection can hold.
    **/
    bool is_init;
} CollisionManifold;

typedef struct{
    i32 dynamic;
    i32 trigger;
    i32 kinematic;
} BodyCount;

typedef struct{

    f64 step_time_in_ms;
    f64 sub_step_time_in_ms;
    f64 body_movement_step_in_ms;
    f64 transform_vertices_step_in_ms;
    f64 bvg_construction_step_in_ms;
    f64 find_collisions_step_in_ms;
    f64 collider_resolution_step_in_ms;
    f64 rigid_resolution_step_in_ms;

    Soa_Body bodies;
    /**
        The scratch buffer for retrieving overlap data from the bvh.
    **/
    BvhCategorisedLeafOverlaps overlaps_scratch_buffer;
    /**
        The indices in the `CollisionManifoldState` of collider collisions to resolve in the current substep.
    **/
    CategorisedOverlapArray sub_step_shape_collisions_to_resolve;
    /**
        The indices in the `CollisionManifoldState` of rigidbody collisions to resolve in the current substep.
    **/
    CategorisedOverlapArray sub_step_rigid_collisions_to_resolve;
    /**
        The gen-id allocator for all phsyics bodies.
    **/
    GenIdAllocator gen_id_allocator;
    /**
    `remarks`
        - Roots are the indices of `bodies`.
        - All subsequent children are the indices of `shapes`.
        - Elements are accessed via `entityIndex`.
    **/
    IntrusiveList body_hierarchy;
    /**
        Gets the bounding volume hierarchy for a collision system.
    **/
    BoundingVolumeHierarchy bvh;
    CollisionManifold collision_manifold;
    /**
        The collision manifold.
    **/
    Vector2 gravity_direction;
    /**
        Gets and sets the gravity force.
    **/
    f32 gravity_force;

    BodyCount polygon_collider_count;
    BodyCount polygon_rigid_count;
    BodyCount circle_collider_count;
    BodyCount circle_rigid_count;
    BodyCount capsule_collider_count;
    BodyCount capsule_rigid_count;
    bool is_init;
} FIZXState;

/**====================
    defines.
====================**//**/

#define MATERIAL_MIN_FRICTION 0
#define MATERIAL_MAX_FRICTION 1
#define MATERIAL_MIN_DENSITY F32_EPSILON
#define MATERIAL_MAX_DENSITY 22.60f // osmium density.
#define MATERIAL_MIN_RESTITUTION 0.0f
#define MATERIAL_MAX_RESTITUTION 1.0f
#define RECTANGLE_ROTATIONAL_INERTIA 0.0833333333333f
#define CIRCLE_ROTATIONAL_INERTIA 0.5f
#define BODY_MIN_SIZE F32_EPSILON
#define BODY_MAX_SIZE F32_MAX
#define COLLISION_MAX_CONTACT_POINTS 2
/**
    The maximum amount of colliders

    `remarks`
    This is because colliders are stored in a one dimensional array, meaning anything higher than 46340 * 46340 will cause an integer overflow.
**/
#define MAX_COLLIDERS 46340
/**
    The dense index of a physics body if it is inactive.
**/
#define BODY_INACTIVE_DENSE_INDE 0

#define ASSERT_KINETIC_FRICTION_IN_RANGE(value) ASSERT(value >= MATERIAL_MIN_FRICTION && value <= MATERIAL_MAX_FRICTION, "kinetic friction out of bounds.")
#define ASSERT_STATIC_FRICTION_IN_RANGE(value, kinetic_friction) ASSERT(value >= kinetic_friction && value <= MATERIAL_MAX_FRICTION, "static friction out of bounds.")
#define ASSERT_DENSITY_IN_RANGE(value) ASSERT(value >= MATERIAL_MIN_DENSITY && value <= MATERIAL_MAX_DENSITY, "density out of bounds.")
#define ASSERT_RESTITUTION_IN_RANGE(value) ASSERT(value >= MATERIAL_MIN_RESTITUTION && value <= MATERIAL_MAX_RESTITUTION, "restitution out of bounds.")

#define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
#define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
#define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
#define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
#define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
#define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
#define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY null
#define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY null

#define COLLISION_MANIFOLD_ACTIVE_INDEX_TYPE i32

// add a macro check here to flip owner and other indexes.

#define COLLISION_DETECTION(manifold, info, bodies, body_hierarchy, shape_collisions_to_resolve, rigid_collisions_to_resolve) do {          \
    for(i32 COLLISION_DETECTION_i = 0; COLLISION_DETECTION_i < info.length; COLLISION_DETECTION_i++){                                       \
        i32 COLLISION_DETECTION_owner_leaf_idx = info.owner_leaf_index[COLLISION_DETECTION_i];                                              \
        i32 COLLISION_DETECTION_other_leaf_idx = info.other_leaf_index[COLLISION_DETECTION_i];                                              \
        BOUNDS_CHECK(COLLISION_DETECTION_owner_leaf_idx, bodies.bvh_leaf_index_length);                                                     \
        i32 COLLISION_DETECTION_owner_bvh_idx = bodies.bvh_leaf_index[COLLISION_DETECTION_owner_leaf_idx];                                  \
        i32 COLLISION_DETECTION_other_bvh_idx = bodies.bvh_leaf_index[COLLISION_DETECTION_other_leaf_idx];                                  \
                                                                                                                                            \
        bool COLLISION_DETECTION_broad_phase = collision_detection_broad_phase(                                                             \
            body_hierarchy, bodies.aabb, COLLISION_DETECTION_owner_bvh_idx, COLLISION_DETECTION_other_bvh_idx                               \
        );                                                                                                                                  \
        if(!COLLISION_DETECTION_broad_phase){                                                                                               \
            continue;                                                                                                                       \
        }                                                                                                                                   \
                                                                                                                                            \
        IndexPair COLLISION_DETECTION_idx_pair;                                                                                             \
        bool COLLISION_DETECTION_narrow_phase = false;                                                                                      \
        if(COLLISION_DETECTION_CONFIG_POLY_TO_POLY){                                                                                        \
            COLLISION_DETECTION_narrow_phase = collision_detection_polygon_to_polygon(                                                      \
                manifold, bodies.global_vertex, bodies.centroid,                                                                            \
                COLLISION_DETECTION_owner_bvh_idx, COLLISION_DETECTION_other_bvh_idx, &COLLISION_DETECTION_idx_pair                         \
            );                                                                                                                              \
        }                                                                                                                                   \
        else if(COLLISION_DETECTION_CONFIG_POLY_TO_CIRC){                                                                                   \
            COLLISION_DETECTION_narrow_phase = collision_detection_polygon_to_circle(                                                       \
                manifold, bodies.global_vertex, bodies.centroid, bodies.global_radius, bodies.global_radius_length,                         \
                COLLISION_DETECTION_owner_bvh_idx, COLLISION_DETECTION_other_bvh_idx, &COLLISION_DETECTION_idx_pair                         \
            );                                                                                                                              \
        }                                                                                                                                   \
        else if(COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC){                                                                                   \
            COLLISION_DETECTION_narrow_phase = collision_detection_circle_to_circle(                                                        \
                manifold, bodies.centroid, bodies.global_radius, bodies.global_radius_length,                                               \
                COLLISION_DETECTION_owner_bvh_idx, COLLISION_DETECTION_other_bvh_idx, &COLLISION_DETECTION_idx_pair                         \
            );                                                                                                                              \
        }                                                                                                                                   \
        if(!COLLISION_DETECTION_narrow_phase){                                                                                              \
            continue;                                                                                                                       \
        }                                                                                                                                   \
                                                                                                                                            \
        if(COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC){                                                               \
            i32 temp = COLLISION_DETECTION_idx_pair.a_to_b;                                                                                 \
            COLLISION_DETECTION_idx_pair.a_to_b = COLLISION_DETECTION_idx_pair.b_to_a;                                                      \
            COLLISION_DETECTION_idx_pair.b_to_a = temp;                                                                                     \
        }                                                                                                                                   \
                                                                                                                                            \
        if(COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION){                                                                             \
            categorised_overlap_array_push(                                                                                                 \
                shape_collisions_to_resolve,                                                                                                \
                &COLLISION_DETECTION_idx_pair.a_to_b, sizeof(COLLISION_DETECTION_idx_pair.a_to_b),                                          \
                COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY, COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY                  \
            );                                                                                                                              \
            farted();                                                                                                                       \
        }                                                                                                                                   \
        if(COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION){                                                                             \
            categorised_overlap_array_push(                                                                                                 \
                rigid_collisions_to_resolve,                                                                                                \
                &COLLISION_DETECTION_idx_pair.a_to_b, sizeof(COLLISION_DETECTION_idx_pair.a_to_b),                                          \
                COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY, COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY                  \
            );                                                                                                                              \
        }                                                                                                                                   \
    }                                                                                                                                       \
} while(0)

#define STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE i32
#define STATE_SUB_STEP_RIGID_COLLISIONS_TO_RESOLVE_TYPE i32



void farted(){
    i32 x = 12;
}


/**====================
    functions: Material
====================**//**/




inline void fizx_material_set_kinematic_friction_scalar(f32* dst, f32 value){
    ASSERT_KINETIC_FRICTION_IN_RANGE(value);
    *dst = CLAMP(value, MATERIAL_MIN_FRICTION, MATERIAL_MAX_FRICTION);
}

inline void fizx_material_set_kinematic_friction(Material* material, f32 value){
    fizx_material_set_kinematic_friction_scalar(&material->kinetic_friction, value);
}

inline void fizx_material_set_static_friction_scalar(f32* dst, f32 kinetic_friction_value, f32 static_friction_value){
    ASSERT_STATIC_FRICTION_IN_RANGE(static_friction_value, kinetic_friction_value);
    *dst = CLAMP(static_friction_value, kinetic_friction_value, MATERIAL_MAX_FRICTION);
}

inline void fizx_material_set_static_friction(Material* material, f32 value){
    fizx_material_set_static_friction_scalar(&material->static_friction, material->kinetic_friction, value);
}

inline void fizx_material_set_density_scalar(f32* dst, f32 value){
    ASSERT_DENSITY_IN_RANGE(value);
    *dst = CLAMP(value, MATERIAL_MIN_DENSITY, MATERIAL_MAX_DENSITY);
}

inline void fizx_material_set_density(Material* material, f32 value){
    fizx_material_set_density_scalar(&material->density, value);
}

inline void fizx_material_set_restitution_scalar(f32* dst, f32 value){
    ASSERT_RESTITUTION_IN_RANGE(value);
    *dst = CLAMP(value, MATERIAL_MIN_RESTITUTION, MATERIAL_MAX_RESTITUTION);
}

inline void fizx_material_set_restitution(Material* material, f32 value){
    fizx_material_set_restitution_scalar(&material->restitution, value);
}

void fizx_material_init(Material* material, f32 static_friction, f32 kinematic_friction, f32 density, f32 restitution){
    // order matters here: kinematic -> static.
    fizx_material_set_kinematic_friction(material, kinematic_friction);
    fizx_material_set_static_friction(material, static_friction);
    fizx_material_set_density(material, density);
    fizx_material_set_restitution(material, restitution);
}




/**====================
    functions: Soa_Material.
====================**//**/




void fizx_soa_material_init(Soa_Material* soa, MemoryArena* arena, i32 length){
    ASSERT(!soa->is_init, "soa already init.");
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->static_friction, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->kinetic_friction, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->density, &soa->length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->restitution, &soa->length, length);
    soa->is_init = true;
}

void fizx_soa_material_insert(Soa_Material* soa, f32 static_friction, f32 kinetic_friction, f32 density, f32 restitution, i32 insert_index){
    ASSERT_KINETIC_FRICTION_IN_RANGE(kinetic_friction);
    ASSERT_STATIC_FRICTION_IN_RANGE(static_friction, kinetic_friction);
    ASSERT_DENSITY_IN_RANGE(density);
    ASSERT_RESTITUTION_IN_RANGE(restitution);

    BOUNDS_CHECK(insert_index, soa->length);
    soa->static_friction[insert_index] = static_friction;
    soa->kinetic_friction[insert_index] = kinetic_friction;
    soa->density[insert_index] = density;
    soa->restitution[insert_index] = restitution;
}





/**====================
    functions: Collision Resolution.
====================**//**/




/**
    `parameters`:
    all `scratch-space` arrays passed into this function must have an allocated length of atleast `COLLISION_MAX_CONTACT_POINTS`; any less than that and it will corrupt memory.
**/
inline void fizx_resolve_rigid_collisions(
    CollisionManifold manifold, IntrusiveList body_hierarchy, Soa_Body body, i32* collision_to_resolve, i32 collision_to_resolve_length,
    f32* impulses_x_scratch_space, f32* impulses_y_scratch_space, f32* impulse_magnitude_scratch_space,
    f32* contact_points_x_scratch_space, f32* contact_points_y_scratch_space,
    f32* owner_distance_x_scratch_space, f32* owner_distance_y_scratch_space,
    f32* other_distance_x_scratch_space, f32* other_distance_y_scratch_space,
    f32* owner_perpendicular_x_scratch_space, f32* owner_perpendicular_y_scratch_space,
    f32* other_perpendicular_x_scratch_space, f32* other_perpendicular_y_scratch_space,
    bool other_shape_is_kinematic
){
    i32 contact_points_count = 0;
    f32 inverse_normal_x = 0;
    f32 inverse_normal_y = 0;

    for(i32 i = 0; i < collision_to_resolve_length; i++){
        i32 collision_idx = collision_to_resolve[i];
        i32 owner_shape_idx = collision_idx / manifold.collider_stride;
        i32 other_shape_idx = collision_idx % manifold.collider_stride;

        BOUNDS_CHECK(owner_shape_idx, body_hierarchy.length);
        i32 owner_body_idx = body_hierarchy.node[owner_shape_idx].parent;
        BOUNDS_CHECK(other_shape_idx, body_hierarchy.length);
        i32 other_body_idx = body_hierarchy.node[other_shape_idx].parent;

        BOUNDS_CHECK(collision_idx, manifold.normal.length);
        f32 normal_x = manifold.normal.x[collision_idx];
        f32 normal_y = manifold.normal.y[collision_idx];

        BOUNDS_CHECK(owner_body_idx, body.global_transform.length);
        BOUNDS_CHECK(owner_body_idx, body.local_center_of_mass.length);
        BOUNDS_CHECK(other_body_idx, body.global_transform.length);
        BOUNDS_CHECK(other_body_idx, body.local_center_of_mass.length);
        f32 owner_global_center_x = body.global_transform.position.x[owner_body_idx] + body.local_center_of_mass.x[owner_body_idx];
        f32 other_global_center_x = body.global_transform.position.x[other_body_idx] + body.local_center_of_mass.x[other_body_idx];
        f32 owner_global_center_y = body.global_transform.position.y[owner_body_idx] + body.local_center_of_mass.y[owner_body_idx];
        f32 other_global_center_y = body.global_transform.position.y[other_body_idx] + body.local_center_of_mass.y[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.linear_velocity.length);
        BOUNDS_CHECK(other_body_idx, body.linear_velocity.length);
        f32* owner_linear_velocity_x = &body.linear_velocity.x[owner_body_idx];
        f32* other_linear_velocity_x = &body.linear_velocity.x[other_body_idx];
        f32* owner_linear_velocity_y = &body.linear_velocity.y[owner_body_idx];
        f32* other_linear_velocity_y = &body.linear_velocity.y[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.angular_velocity_length);
        BOUNDS_CHECK(other_body_idx, body.angular_velocity_length);
        f32* owner_angular_velocity = &body.angular_velocity[owner_body_idx];
        f32* other_angular_velocity = &body.angular_velocity[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.inverse_mass_length);
        BOUNDS_CHECK(other_body_idx, body.inverse_mass_length);
        f32 owner_inverse_mass = body.inverse_mass[owner_body_idx];
        f32 other_inverse_mass = body.inverse_mass[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.inverse_rotational_inertia_length);
        BOUNDS_CHECK(other_body_idx, body.inverse_rotational_inertia_length);
        f32 owner_inverse_rotational_inertia = body.inverse_rotational_inertia[owner_body_idx];
        f32 other_inverse_rotational_inertia = body.inverse_rotational_inertia[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.material.length);
        BOUNDS_CHECK(other_body_idx, body.material.length);
        f32 owner_restitution = body.material.restitution[owner_body_idx];
        f32 other_restitution = body.material.restitution[other_body_idx];
        f32 owner_kinetic_friction = body.material.kinetic_friction[owner_body_idx];
        f32 other_kinetic_friction = body.material.kinetic_friction[other_body_idx];
        f32 owner_static_friction = body.material.static_friction[owner_body_idx];
        f32 other_static_friction = body.material.static_friction[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.mass_length);
        BOUNDS_CHECK(other_body_idx, body.mass_length);
        f32 owner_mass = body.mass[owner_body_idx];
        f32 other_mass = body.mass[other_body_idx];

        BOUNDS_CHECK(owner_body_idx, body.rotational_response_length);
        BOUNDS_CHECK(other_body_idx, body.rotational_response_length);
        bool owner_rotational_response = body.rotational_response[owner_shape_idx];
        bool other_rotational_response = body.rotational_response[other_shape_idx];

        inverse_normal_x = normal_x * -1.0f;
        inverse_normal_y = normal_y * -1.0f;

        /**
            zero scratch-space

            these have to be set to zero.
            this function resuses these stack allocated System.Spans
            so without this, the loop could operate on garbage data from the previous step.
        **/
        {
            size_t scratch_space_size = sizeof(f32) * COLLISION_MAX_CONTACT_POINTS;
            ZERO_MEMORY(impulses_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(impulses_y_scratch_space, scratch_space_size);

            ZERO_MEMORY(impulse_magnitude_scratch_space, scratch_space_size);

            ZERO_MEMORY(contact_points_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(contact_points_y_scratch_space, scratch_space_size);

            ZERO_MEMORY(owner_distance_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(owner_distance_y_scratch_space, scratch_space_size);

            ZERO_MEMORY(other_distance_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(other_distance_y_scratch_space, scratch_space_size);

            ZERO_MEMORY(owner_perpendicular_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(owner_perpendicular_y_scratch_space, scratch_space_size);

            ZERO_MEMORY(other_perpendicular_x_scratch_space, scratch_space_size);
            ZERO_MEMORY(other_perpendicular_y_scratch_space, scratch_space_size);
        }

        BOUNDS_CHECK(collision_idx, manifold.two_contact_points_length);
        if(manifold.two_contact_points[collision_idx]){
            BOUNDS_CHECK(collision_idx, manifold.first_contact_point.length);
            BOUNDS_CHECK(collision_idx, manifold.second_contact_point.length);
            contact_points_count = 2;
            contact_points_x_scratch_space[0] = manifold.first_contact_point.x[collision_idx];
            contact_points_x_scratch_space[1] = manifold.second_contact_point.x[collision_idx];
            contact_points_y_scratch_space[0] = manifold.first_contact_point.y[collision_idx];
            contact_points_y_scratch_space[1] = manifold.second_contact_point.y[collision_idx];
        }
        else{
            BOUNDS_CHECK(collision_idx, manifold.first_contact_point.length);
            contact_points_count = 1;
            contact_points_x_scratch_space[0] = manifold.first_contact_point.x[collision_idx];
            contact_points_y_scratch_space[0] = manifold.first_contact_point.y[collision_idx];
        }

        /**
            resolve rigid collision rotational
        **/
        if(owner_rotational_response || other_rotational_response){
            f32 restitution = MIN(owner_restitution, other_restitution);

            for(i32 j = 0; j < contact_points_count; j++){
                f32 contact_point_x = contact_points_x_scratch_space[j];
                f32 contact_point_y = contact_points_y_scratch_space[j];

                owner_distance_x_scratch_space[j] = contact_point_x - owner_global_center_x;
                owner_distance_y_scratch_space[j] = contact_point_y - owner_global_center_y;
                other_distance_x_scratch_space[j] = contact_point_x - other_global_center_x;
                other_distance_y_scratch_space[j] = contact_point_y - other_global_center_y;

                f32 owner_perpendicular_x = -owner_distance_y_scratch_space[j];
                f32 owner_perpendicular_y = owner_distance_x_scratch_space[j];
                f32 other_perpendicular_x = -other_distance_y_scratch_space[j];
                f32 other_perpendicular_y = other_distance_x_scratch_space[j];

                // calc angular velocity to travel in.
                f32 owner_angular_velocity_x = owner_perpendicular_x * *owner_angular_velocity;
                f32 owner_angular_velocity_y = owner_perpendicular_y * *owner_angular_velocity;
                f32 other_angular_velocity_x = other_perpendicular_x * *other_angular_velocity;
                f32 other_angular_velocity_y = other_perpendicular_y * *other_angular_velocity;

                f32 relative_velocity_x = (*other_linear_velocity_x + other_angular_velocity_x) - (*owner_linear_velocity_x + owner_angular_velocity_x);
                f32 relative_velocity_y = (*other_linear_velocity_y + other_angular_velocity_y) - (*owner_linear_velocity_y + owner_angular_velocity_y);

                f32 magnitude = vector2_dot_scalar(relative_velocity_x, relative_velocity_y, inverse_normal_x, inverse_normal_y);

                /**
                    this stops the edge-case when two bodies that are brushing up against one another begin rotating despite little to no forces being applied.
                    this keeps the rotational physics stable when bodies are at "rest" with eachother (like stacked polygons or balls :) ).
                **/
                if(magnitude > 0){
                    continue;
                }

                // calc the denominator.
                f32 owner_perpendicular_dot_normal = vector2_dot_scalar(owner_perpendicular_x, owner_perpendicular_y, inverse_normal_x, inverse_normal_y);
                f32 other_perpendicular_dot_normal = vector2_dot_scalar(other_perpendicular_x, other_perpendicular_y, inverse_normal_x, inverse_normal_y);
                f32 owner_perpendicular_dot_normal_sqrd = owner_perpendicular_dot_normal * owner_perpendicular_dot_normal;
                f32 other_perpendicular_dot_normal_sqrd = other_perpendicular_dot_normal * other_perpendicular_dot_normal;
                f32 denominator = owner_inverse_mass + other_inverse_mass +
                    owner_perpendicular_dot_normal_sqrd * owner_inverse_rotational_inertia +
                    other_perpendicular_dot_normal_sqrd * other_inverse_rotational_inertia;

                // magnitude of the impulse.
                f32 impulse_magnitude = -(1.0f + restitution) * magnitude;
                impulse_magnitude /= denominator;

                /**
                    divide by the contact point count to ensure that impulse is evenly spread
                    across all contact points.
                **/
                impulse_magnitude /= (float)contact_points_count;

                // chache the impulse magnitude for later friction resolution.
                impulse_magnitude_scratch_space[j] = impulse_magnitude;
                impulses_x_scratch_space[j] = impulse_magnitude * inverse_normal_x;
                impulses_y_scratch_space[j] = impulse_magnitude * inverse_normal_y;
            }

            f32 impulse_x;
            f32 impulse_y;
            f32 owner_distance_x;
            f32 owner_distance_y;
            f32 other_distance_x;
            f32 other_distance_y;

            for(i32 j = 0; j < contact_points_count; j++){

                impulse_x = impulses_x_scratch_space[j];
                impulse_y = impulses_y_scratch_space[j];

                /**
                    cross producting the dist and impulse gives a value indicating
                    how much angular velocity - in radians - is needed to be applied based on the impulse direction.
                    this is because cross producting two directions that are parallel to eachother, results in zero.
                    which means that there should be no rotation if the collision is head on.
                    but if the closer the two directions come to being perpendicular to one another,
                    the larger the angular impulse will be, causing the body to rotate.
                **/

                *owner_linear_velocity_x += -impulse_x * owner_inverse_mass;
                *owner_linear_velocity_y += -impulse_y * owner_inverse_mass;
                if(owner_rotational_response){
                    owner_distance_x = owner_distance_x_scratch_space[j];
                    owner_distance_y = owner_distance_y_scratch_space[j];
                    *owner_angular_velocity += -vector2_cross_scalar(owner_distance_x, owner_distance_y, impulse_x, impulse_y);
                }

                if(other_shape_is_kinematic){
                    continue;
                }
                *other_linear_velocity_x += impulse_x * other_inverse_mass;
                *other_linear_velocity_y += impulse_y * other_inverse_mass;
                if(other_rotational_response){
                    other_distance_x = other_distance_x_scratch_space[j];
                    other_distance_y = other_distance_y_scratch_space[j];
                    *other_angular_velocity += -vector2_cross_scalar(other_distance_x, other_distance_y, impulse_x, impulse_y);
                }
            }
        }

        /**
            resolve rigid collision basic.
        **/
        else{
            for(i32 j = 0; j < contact_points_count; j++){
                f32 relative_velocity_x = *other_linear_velocity_x - *owner_linear_velocity_x;
                f32 relative_velocity_y = *other_linear_velocity_y - *owner_linear_velocity_y;
                f32 magnitude = vector2_dot_scalar(relative_velocity_x, relative_velocity_y, inverse_normal_x, inverse_normal_y);
                if(magnitude > 0){
                    continue;
                }
                f32 restitution = MIN(owner_restitution, other_restitution);
                f32 impulse_magnitude = -(1.0f + restitution) * magnitude;
                impulse_magnitude /= owner_inverse_mass + other_inverse_mass;
                // divide by the contact point count to ensure that impulse is evenly spread across all contact points.
                impulse_magnitude /= (f32)contact_points_count;

                BOUNDS_CHECK(contact_points_count, COLLISION_MAX_CONTACT_POINTS + 1);
                impulse_magnitude_scratch_space[contact_points_count] = impulse_magnitude;
            }

            f32 impulse_force_x;
            f32 impulse_force_y;

            for(i32 j = 0; j < contact_points_count; j++){
                BOUNDS_CHECK(i, COLLISION_MAX_CONTACT_POINTS + 1);
                f32 mag = impulse_magnitude_scratch_space[j];
                impulse_force_x = -(mag / owner_mass * inverse_normal_x);
                impulse_force_y = -(mag / owner_mass * inverse_normal_y);
                *owner_linear_velocity_x += impulse_force_x;
                *owner_linear_velocity_y += impulse_force_y;

                if(other_shape_is_kinematic){
                    continue;
                }

                impulse_force_x = mag / other_mass * inverse_normal_x;
                impulse_force_y = mag / other_mass * inverse_normal_y;
                *other_linear_velocity_x += impulse_force_x;
                *other_linear_velocity_y += impulse_force_y;
            }
        }

        /**
            resolve rigid collision friction.
        **/
        {
            /**
                get an approximation of the friction values.
                this is faster than the actual physics way.
            **/
            f32 static_friction = (owner_static_friction + other_static_friction) * 0.5f;
            f32 kinetic_friction = (owner_kinetic_friction + other_kinetic_friction) * 0.5f;

            for(i32 j = 0; j < contact_points_count; j++){
                /**
                    note that these are recalculated values with the new linear and angular velocities.
                    this is because these velocities were mutated in the above procedures, without the recalculation,
                    the opposing inertial force would be relative to the previous frame's velocity values.
                **/
                f32 contact_point_x = contact_points_x_scratch_space[j];
                f32 contact_point_y = contact_points_y_scratch_space[j];

                owner_distance_x_scratch_space[j] = contact_point_x - owner_global_center_x;
                owner_distance_y_scratch_space[j] = contact_point_y - owner_global_center_y;
                other_distance_x_scratch_space[j] = contact_point_x - other_global_center_x;
                other_distance_y_scratch_space[j] = contact_point_y - other_global_center_y;

                f32 owner_perpendicular_x = -owner_distance_y_scratch_space[j];
                f32 owner_perpendicular_y = owner_distance_x_scratch_space[j];
                f32 other_perpendicular_x = -other_distance_y_scratch_space[j];
                f32 other_perpendicular_y = other_distance_x_scratch_space[j];

                // calc angular velocity to travel in.
                f32 owner_angular_velocity_x = owner_perpendicular_x * *owner_angular_velocity;
                f32 owner_angular_velocity_y = owner_perpendicular_y * *owner_angular_velocity;
                f32 other_angular_velocity_x = other_perpendicular_x * *other_angular_velocity;
                f32 other_angular_velocity_y = other_perpendicular_y * *other_angular_velocity;

                f32 relative_velocity_x = (*other_linear_velocity_x + other_angular_velocity_x) - (*owner_linear_velocity_x + owner_angular_velocity_x);
                f32 relative_velocity_y = (*other_linear_velocity_y + other_angular_velocity_y) - (*owner_linear_velocity_y + owner_angular_velocity_y);

                // this is the direction the body is travelling in along the contact point surface.
                f32 relative_dot_normal = vector2_dot_scalar(relative_velocity_x, relative_velocity_y, inverse_normal_x, inverse_normal_y);
                f32 tangent_x = relative_velocity_x - relative_dot_normal * inverse_normal_x;
                f32 tangent_y = relative_velocity_y - relative_dot_normal * inverse_normal_y;

                if(f32_nearly_equal((tangent_x * tangent_x) + (tangent_y * tangent_y), 0, 1e-12f)){
                    continue;
                }

                vector2_normalise_scalar(tangent_x, tangent_y, &tangent_x, &tangent_y);

                // calculate the denominator.
                f32 owner_perpendicular_dot_tangent = vector2_dot_scalar(owner_perpendicular_x, owner_perpendicular_y, tangent_x, tangent_y);
                f32 other_perpendicular_dot_tangent = vector2_dot_scalar(other_perpendicular_x, other_perpendicular_y, tangent_x, tangent_y);
                f32 owner_perpendicular_dot_tangent_sqrd = owner_perpendicular_dot_tangent * owner_perpendicular_dot_tangent;
                f32 other_perpendicular_dot_tangent_sqrd = other_perpendicular_dot_tangent * other_perpendicular_dot_tangent;
                f32 denominator =
                    owner_inverse_mass + other_inverse_mass +
                    owner_perpendicular_dot_tangent_sqrd * owner_inverse_rotational_inertia +
                    other_perpendicular_dot_tangent_sqrd * other_inverse_rotational_inertia;

                // Calculate the full friction magnitude to stop all sliding.
                f32 friction_impulse_magnitude = -vector2_dot_scalar(relative_velocity_x, relative_velocity_y, tangent_x, tangent_y);

                // Coulomb's Law.
                {
                    f32 static_friction_threshold = impulse_magnitude_scratch_space[j] * static_friction;
                    if(ABS(friction_impulse_magnitude) > static_friction_threshold){
                        // the friction applied will be the kinetic friction value instead.
                        friction_impulse_magnitude = (impulse_magnitude_scratch_space[j] * kinetic_friction) * f32_sign(friction_impulse_magnitude);
                    }

                    // apply the capped magnitude to the tangent vector.
                    impulses_x_scratch_space[j] = friction_impulse_magnitude * tangent_x;
                    impulses_y_scratch_space[j] = friction_impulse_magnitude * tangent_y;
                }
            }

            f32 impulse_x;
            f32 impulse_y;
            f32 owner_distance_x;
            f32 owner_distance_y;
            f32 other_distance_x;
            f32 other_distance_y;

            for(i32 j = 0; j < contact_points_count; j++){
                impulse_x = impulses_x_scratch_space[j];
                impulse_y = impulses_y_scratch_space[j];

                *owner_linear_velocity_x += -impulse_x * owner_inverse_mass;
                *owner_linear_velocity_y += -impulse_y * owner_inverse_mass;
                if(owner_rotational_response){
                    /**
                        cross producting the dist and impulse gives a value indicating
                        how much angular velocity - in radians - is needed to be applied based on the impulse direction.
                        this is because cross producting two directions that are parallel to eachother, results in zero.
                        which means that there should be no rotation if the collision is head on.
                        but if the closer the two directions come to being perpendicular to one another,
                        the larger the angular impulse will be, causing the body to rotate.
                    **/
                    owner_distance_x = owner_distance_x_scratch_space[j];
                    owner_distance_y = owner_distance_y_scratch_space[j];
                    *owner_angular_velocity += -vector2_cross_scalar(owner_distance_x, owner_distance_y, impulse_x, impulse_y) * owner_inverse_rotational_inertia;
                }

                if(other_shape_is_kinematic){
                    continue;
                }

                *other_linear_velocity_x += -impulse_x * other_inverse_mass;
                *other_linear_velocity_y += -impulse_y * other_inverse_mass;
                if(other_rotational_response){
                    /**
                        cross producting the dist and impulse gives a value indicating
                        how much angular velocity - in radians - is needed to be applied based on the impulse direction.
                        this is because cross producting two directions that are parallel to eachother, results in zero.
                        which means that there should be no rotation if the collision is head on.
                        but if the closer the two directions come to being perpendicular to one another,
                        the larger the angular impulse will be, causing the body to rotate.
                    **/
                    other_distance_x = other_distance_x_scratch_space[j];
                    other_distance_y = other_distance_y_scratch_space[j];
                    *other_angular_velocity += vector2_cross_scalar(other_distance_x, other_distance_y, impulse_x, impulse_y) * other_inverse_rotational_inertia;
                }
            }
        }
    }
}




/**====================
    functions: CollisionManifold
====================**//**/




void collision_manifold_init(CollisionManifold* manifold, MemoryArena* arena, i32 total_colliders){
    ASSERT(!manifold->is_init, "already init");
    ASSERT(total_colliders <= MAX_COLLIDERS, "total colliders exceeds max supported colliders.");
    total_colliders = CLAMP(total_colliders, 0, MAX_COLLIDERS);
    manifold->collider_stride = total_colliders;
    i32 data_length = total_colliders * total_colliders;
    soa_vector2_init(&manifold->normal, arena, data_length);
    soa_vector2_init(&manifold->collider_centroid, arena, data_length);
    soa_vector2_init(&manifold->first_contact_point, arena, data_length);
    soa_vector2_init(&manifold->second_contact_point, arena, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->depth, &manifold->depth_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->two_contact_points, &manifold->two_contact_points_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->contact_state, &manifold->contact_state_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->previous_contact_state, &manifold->previous_contact_state_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->active_phase, &manifold->active_phase_length, data_length);
    fixed_stride_array_init(&manifold->active_index, arena, total_colliders, total_colliders, sizeof(COLLISION_MANIFOLD_ACTIVE_INDEX_TYPE));
    manifold->is_init = true;
}

void collision_manifold_swap_contact_state_contexts(CollisionManifold* manifold){
    ContactState* swap_data = manifold->previous_contact_state;
    manifold->previous_contact_state = manifold->contact_state;
    manifold->contact_state = swap_data;
}

inline void collision_manifold_prepare_for_next_step(CollisionManifold* manifold){
    collision_manifold_swap_contact_state_contexts(manifold);
}

/**
    Sets a one-way collision data entry at a given index.

    `parameters`
    `manifold`: the state instance to set.
    `recipient_idx`: the index in the state instance arrays to write to.
    `collider_idx`: the physics body index of the of the `colliding` collider.
    `collider_centroid_x`: the x-component of the `colliding` collider's centroid.
    `collider_centroid_y`: the y-component of the `colliding` collider's centroid.
    `normal_x`: the x-component of the normal vector in relation to collider A to B.
    `normal_y`: the y-component of the normal vector in relation to collider A to B.
    `contact_point_1_x`: the x-component of the first contact point.
    `contact_point_1_y`: the y-component of the first contact point.
    `contact_point_2_x`: the x-component of the second contact point.
    `contact_point_2_y`: the y-component of the second contact point.
    `depth`: the depth of the collision.

    `returns`
    the collision index the data was written to.
**/
i32 collision_manifold_set_data_one_way(
    CollisionManifold* manifold,
    i32 recipient_idx, i32 collider_index,
    f32 collider_centroid_x, f32 collider_centroid_y,
    f32 normal_x, f32 normal_y,
    f32 contact_point_1_x, f32 contact_point_1_y,
    f32 contact_point_2_x, f32 contact_point_2_y,
    f32 depth, bool two_contact_points
){
    i32 idx = fixed_stride_array_get_element_idx(recipient_idx, manifold->collider_stride, collider_index);
    char* idx_char_ptr = (char*)&idx;
    BOUNDS_CHECK(idx, manifold->active_phase_length);
    i32* phase = &manifold->active_phase[idx];
    if(*phase <= 0){
        fixed_stride_array_push(&manifold->active_index, recipient_idx, idx_char_ptr, sizeof(COLLISION_MANIFOLD_ACTIVE_INDEX_TYPE));
    }

    *phase = 1;

    BOUNDS_CHECK(idx, manifold->normal.length);
    manifold->normal.x[idx] = normal_x;
    manifold->normal.y[idx] = normal_y;
    BOUNDS_CHECK(idx, manifold->collider_centroid.length);
    manifold->collider_centroid.x[idx] = collider_centroid_x;
    manifold->collider_centroid.y[idx] = collider_centroid_y;
    BOUNDS_CHECK(idx, manifold->first_contact_point.length);
    manifold->first_contact_point.x[idx] = contact_point_1_x;
    manifold->first_contact_point.y[idx] = contact_point_1_y;
    BOUNDS_CHECK(idx, manifold->second_contact_point.length);
    manifold->second_contact_point.x[idx] = contact_point_2_x;
    manifold->second_contact_point.y[idx] = contact_point_2_y;
    BOUNDS_CHECK(idx, manifold->depth_length);
    manifold->depth[idx] = depth;
    BOUNDS_CHECK(idx, manifold->two_contact_points_length);
    manifold->two_contact_points[idx] = two_contact_points;

    return idx;
}

/**
    Sets a two-way collision data entry at a given entry.

    `manifold`: the state instance to append to.
    `a_idx`: the index of collider A.
    `b_idx`: the index of collider B.
    `x_centroid_x`: the x-component of collider A's centroid.
    `a_centroid_y`: the y-component of collider A's centroid.
    `b_centroid_x`: the x-component of collider B's centroid.
    `b_centroid_y`: the y-component of collider B's centroid.
    `normal_x`: the x-component of the normal vector in relation to collider A to B.
    `normal_y`: the y-component of the normal vector in relation to collider A to B.
    `contact_point_1_x`: the x-component of the first contact point.
    `contact_point_1_y`: the y-component of the first contact point.
    `contact_point_2_x`: the x-component of the second contact point.
    `contact_point_2_y`: the y-component of the second contact point.
    `depth`: the depth of the collision.

    `returns`
    A collision index pair of the collision indices the data was written to.
**/
IndexPair collision_manifold_set_data_two_way(
    CollisionManifold* manifold,
    i32 a_idx, i32 b_idx,
    f32 a_centroid_x, f32 a_centroid_y,
    f32 b_centroid_x, f32 b_centroid_y,
    f32 normal_x, f32 normal_y,
    f32 contact_point_1_x, f32 contact_point_1_y,
    f32 contact_point_2_x, f32 contact_point_2_y,
    f32 depth, bool two_contact_points
){
    i32 a = collision_manifold_set_data_one_way(
        manifold,
        a_idx, b_idx,
        b_centroid_x, b_centroid_y,
        normal_x, normal_y,
        contact_point_1_x, contact_point_1_y,
        contact_point_2_x, contact_point_2_y,
        depth, two_contact_points
    );

    // note: the normal reversing.
    i32 b = collision_manifold_set_data_one_way(
        manifold,
        b_idx, a_idx,
        a_centroid_x, a_centroid_y,
        -normal_x, -normal_y,
        contact_point_1_x, contact_point_1_y,
        contact_point_2_x, contact_point_2_y,
        depth, two_contact_points
    );

    return (IndexPair){.a_to_b = a, .b_to_a = b};
}

/**
    Completes the update step for a state instance.

    `parameters`:
    `manifold`: the state instance to complete the step for.
**/
void collision_manifold_complete_step(CollisionManifold* manifold){
    ContactState* contact_state = manifold->contact_state;
    ContactState* previous_state = manifold->previous_contact_state;

    i32* active_phase = manifold->active_phase;

    for(i32 chunk_idx = 0; chunk_idx < manifold->active_index.chunk_count_length; chunk_idx++){
        BOUNDS_CHECK(chunk_idx, manifold->active_index.chunk_count_length);
        i32 count = manifold->active_index.chunk_count[chunk_idx];
        if(count){
            continue;
        }

        for(i32 chunk_element_idx = 0; chunk_element_idx < count; chunk_element_idx++){

            // get the active phase of the collision.
            i32 element_idx = fixed_stride_array_get_element_idx(chunk_idx, manifold->collider_stride, chunk_element_idx);
            BOUNDS_CHECK(element_idx, manifold->active_index.data_length);
            i32 collision_idx = manifold->active_index.data[element_idx];
            i32* phase = &manifold->active_phase[collision_idx];

            // update the collision state.
            BOUNDS_CHECK(collision_idx, manifold->previous_contact_state_length);
            ContactState* previous = &manifold->previous_contact_state[collision_idx];
            BOUNDS_CHECK(collision_idx, manifold->contact_state_length);
            ContactState* current = &manifold->contact_state[collision_idx];

            switch(*phase){
                case 1:{
                    switch(*current){
                        case ContactState_None:{*current = ContactState_Enter;}break;
                        case ContactState_Enter:{*current = ContactState_Sustain;}break;
                        case ContactState_Sustain:{*current = ContactState_Sustain;}break;
                        case ContactState_Exit:{*current = ContactState_Enter;}break;
                    }
                }break;
                case 2:{
                    *current = ContactState_Exit;
                }break;
                case 3:{
                    *current = ContactState_None;
                }break;
                default:{
                    ASSERT(false, "phase contains unknown state.");
                };
            }

            // update the active phase of the collision.
            *phase += 1;
            *phase %= 4;
            if(*phase == 0){
                fixed_stride_array_unordered_remove_at(&manifold->active_index, chunk_idx, chunk_element_idx, sizeof(COLLISION_MANIFOLD_ACTIVE_INDEX_TYPE));
            }
        }
    }
}

/**
    Gets whether a collider is in contact with another.

    `parameters`:
    `manifold`: the state instance that contains the collider.
    `shape_idx`: the index of the collider in the state instance.

    `returns`:
    true, if the collider is in contact with another; otherwise false.
**/
inline bool collision_manifold_shape_has_collisions(CollisionManifold manifold, i32 shape_idx){
    BOUNDS_CHECK(shape_idx, manifold.active_index.chunk_count_length);
    return manifold.active_index.chunk_count[shape_idx] > 0;
}




/**====================
    functions: Soa_Body.
====================**//**/




void fizx_soa_body_init(Soa_Body* soa, MemoryArena* arena, i32 length, i32 vertices_per_body){
    ASSERT(!soa->is_init, "already init");
    fssoa_vector2_init(&soa->base_vertex, arena, vertices_per_body, length);
    fssoa_vector2_init(&soa->global_vertex, arena, vertices_per_body, length);
    soa_transform2d_init(&soa->local_transform, arena, length);
    soa_transform2d_init(&soa->global_transform, arena, length);
    soa_vector2_init(&soa->previous_step_position, arena, length);
    soa_vector2_init(&soa->force, arena, length);
    soa_vector2_init(&soa->linear_velocity, arena, length);
    soa_vector2_init(&soa->centroid, arena, length);
    soa_vector2_init(&soa->shape_collision_displacement, arena, length);
    soa_vector2_init(&soa->local_center_of_mass, arena, length);
    soa_aabb_init(&soa->aabb, arena, length);
    fizx_soa_material_init(&soa->material, arena, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->angular_velocity, &soa->angular_velocity_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->mass, &soa->mass_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->inverse_mass, &soa->inverse_mass_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->base_width, &soa->base_width_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->base_height, &soa->base_height_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->base_radius, &soa->base_radius_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->global_radius, &soa->global_radius_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->rotational_inertia, &soa->rotational_inertia_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->inverse_rotational_inertia, &soa->inverse_rotational_inertia_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->generation, &soa->generation_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->category, &soa->category_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->bvh_leaf_index, &soa->bvh_leaf_index_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->bvh_leaf_padding, &soa->bvh_leaf_padding_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->shape_type, &soa->shape_type_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->rotational_response, &soa->rotational_response_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->entity_type, &soa->entity_type_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->active, &soa->active_length, length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, soa->gravity_affected, &soa->gravity_affected_length, length);
}

void soa_body_get_vertices_unsafe(FsSoa_Vector2 vertices, i32 body_idx, f32** out_x, f32** out_y, i32* out_length){
    i32 start_idx = fixed_stride_array_get_element_idx(body_idx, vertices.chunk_stride, 0);
    BOUNDS_CHECK(body_idx, vertices.chunk_count_length);
    i32 count = vertices.chunk_count[body_idx];

    *out_x = vertices.x + start_idx;
    *out_y = vertices.y + start_idx;
    *out_length = count;
}

void soa_body_transform_shape_vertices(Soa_Body* soa, i32 shape_idx){

    BOUNDS_CHECK(shape_idx, soa->shape_type_length);
    ShapeType shape_type = soa->shape_type[shape_idx];

    BOUNDS_CHECK(shape_idx, soa->global_transform.length);
    f32* scale_x = &soa->global_transform.scale.x[shape_idx];

    BOUNDS_CHECK(shape_idx, soa->global_transform.length);
    f32* scale_y = &soa->global_transform.scale.y[shape_idx];

    BOUNDS_CHECK(shape_idx, soa->base_vertex.chunk_count_length);
    i32 vertex_count = soa->base_vertex.chunk_count[shape_idx];
    i32 start_idx = fixed_stride_array_get_element_idx(shape_idx, soa->base_vertex.chunk_stride, 0);
    for(i32 vertex = 0; vertex < vertex_count; vertex++){
        i32 current_idx = vertex + start_idx;
        f32 x;
        f32 y;

        // transform base vertex.
        BOUNDS_CHECK(current_idx, soa->base_vertex.x_length);
        BOUNDS_CHECK(current_idx, soa->base_vertex.y_length);
        BOUNDS_CHECK(shape_idx, soa->global_transform.length);
        vector2_transform_scalar(
            soa->base_vertex.x[current_idx], soa->base_vertex.y[current_idx],
            *scale_x, *scale_y, soa->global_transform.cosine[shape_idx], soa->global_transform.sine[shape_idx],
            soa->global_transform.position.x[shape_idx], soa->global_transform.position.y[shape_idx],
            &x, &y
        );

        BOUNDS_CHECK(current_idx, soa->global_vertex.x_length);
        soa->global_vertex.x[current_idx] = x;
        BOUNDS_CHECK(current_idx, soa->global_vertex.y_length);
        soa->global_vertex.y[current_idx] = y;
    }
    soa->global_vertex.chunk_count[shape_idx] = vertex_count;

    // set the new centroid.
    f32* v_x;
    f32* v_y;
    i32 v_length;
    soa_body_get_vertices_unsafe(soa->global_vertex, shape_idx, &v_x, &v_y, &v_length);
    BOUNDS_CHECK(shape_idx, soa->centroid.length);
    polygon_calc_centroid_scalar(v_x, v_y, v_length, &soa->centroid.x[shape_idx], &soa->centroid.y[shape_idx]);

    switch (shape_type){
        default:{
            ASSERT(false, "shape not implemented");
        }break;
        case ShapeType_Rectangle:{
            // set the new min and max vectors.
            BOUNDS_CHECK(shape_idx, soa->aabb.length);
            polygon_get_min_max_vertices(v_x, v_y, v_length, &soa->aabb.min_x[shape_idx], &soa->aabb.min_y[shape_idx], &soa->aabb.max_x[shape_idx], &soa->aabb.max_y[shape_idx]);
        }break;
        case ShapeType_Circle:{
            BOUNDS_CHECK(shape_idx, soa->aabb.length);
            BOUNDS_CHECK(shape_idx, soa->global_transform.length);
            circle_get_min_max_vertices(
                v_x[0], v_y[0],
                soa->global_radius[shape_idx], &soa->aabb.min_x[shape_idx], &soa->aabb.min_y[shape_idx], &soa->aabb.max_x[shape_idx], &soa->aabb.max_y[shape_idx]
            );
        }break;
    }
}


/**====================
    functions: ShapeCategory
====================**//**/

inline bool shape_category_is_polygon(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return category % 3 == 0;
}

inline bool shape_category_is_circle(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return category % 3 == 1;
}

inline bool shape_category_is_capsule(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return category % 3 == 2;
}

inline bool shape_category_is_trigger(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return
    (category >= ShapeCategory_TriRigPolygon && category <= ShapeCategory_TriRigCapsule) ||
    (category >= ShapeCategory_TriColPolygon && category <= ShapeCategory_TriColCapsule);
}

inline bool shape_category_is_dynamic(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return
    (category >= ShapeCategory_DynRigPolygon && category <= ShapeCategory_DynRigCapsule) ||
    (category >= ShapeCategory_DynColPolygon && category <= ShapeCategory_DynColCapsule);
}

inline bool shape_category_is_kinematic(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return
    (category >= ShapeCategory_KinRigPolygon && category <= ShapeCategory_KinRigCapsule) ||
    (category >= ShapeCategory_KinColPolygon && category <= ShapeCategory_KinColCapsule);
}

inline bool shape_category_is_rigid(i32 category){
    BOUNDS_CHECK(category, ShapeCategory_Count);
    return category >= ShapeCategory_DynRigPolygon && category <= ShapeCategory_KinRigCapsule;
}

void shape_category_set_to_rigid(i32* category){
    switch(*category){
        case ShapeCategory_DynRigPolygon: {*category = ShapeCategory_DynRigPolygon;} break;
        case ShapeCategory_DynRigCircle: {*category = ShapeCategory_DynRigCircle;} break;
        case ShapeCategory_DynRigCapsule: {*category = ShapeCategory_DynRigCapsule;} break;

        case ShapeCategory_TriRigPolygon: {*category = ShapeCategory_TriRigPolygon;} break;
        case ShapeCategory_TriRigCircle: {*category = ShapeCategory_TriRigCircle;} break;
        case ShapeCategory_TriRigCapsule: {*category = ShapeCategory_TriRigCapsule;} break;

        case ShapeCategory_KinRigPolygon: {*category = ShapeCategory_KinRigPolygon;} break;
        case ShapeCategory_KinRigCircle: {*category = ShapeCategory_KinRigCircle;} break;
        case ShapeCategory_KinRigCapsule: {*category = ShapeCategory_KinRigCapsule;} break;

        case ShapeCategory_DynColPolygon: {*category = ShapeCategory_DynRigPolygon;} break;
        case ShapeCategory_DynColCircle: {*category = ShapeCategory_DynRigCircle;} break;
        case ShapeCategory_DynColCapsule: {*category = ShapeCategory_DynRigCapsule;} break;

        case ShapeCategory_TriColPolygon: {*category = ShapeCategory_TriRigPolygon;} break;
        case ShapeCategory_TriColCircle: {*category = ShapeCategory_TriRigCircle;} break;
        case ShapeCategory_TriColCapsule: {*category = ShapeCategory_TriRigCapsule;} break;

        case ShapeCategory_KinColPolygon: {*category = ShapeCategory_KinRigPolygon;} break;
        case ShapeCategory_KinColCircle: {*category = ShapeCategory_KinRigCircle;} break;
        case ShapeCategory_KinColCapsule: {*category = ShapeCategory_KinRigCapsule;} break;

        default: {ASSERT(false, "unknown category.");} break;
    }
}

void shape_category_set_to_collider(i32* category){
    switch(*category){
        case ShapeCategory_DynRigPolygon: {*category = ShapeCategory_DynColPolygon;} break;
        case ShapeCategory_DynRigCircle: {*category = ShapeCategory_DynColCircle;} break;
        case ShapeCategory_DynRigCapsule: {*category = ShapeCategory_DynColCapsule;} break;

        case ShapeCategory_TriRigPolygon: {*category = ShapeCategory_TriColPolygon;} break;
        case ShapeCategory_TriRigCircle: {*category = ShapeCategory_TriColCircle;} break;
        case ShapeCategory_TriRigCapsule: {*category = ShapeCategory_TriColCapsule;} break;

        case ShapeCategory_KinRigPolygon: {*category = ShapeCategory_KinColPolygon;} break;
        case ShapeCategory_KinRigCircle: {*category = ShapeCategory_KinColCircle;} break;
        case ShapeCategory_KinRigCapsule: {*category = ShapeCategory_KinColCapsule;} break;

        case ShapeCategory_DynColPolygon: {*category = ShapeCategory_DynColPolygon;} break;
        case ShapeCategory_DynColCircle: {*category = ShapeCategory_DynColCircle;} break;
        case ShapeCategory_DynColCapsule: {*category = ShapeCategory_DynColCapsule;} break;

        case ShapeCategory_TriColPolygon: {*category = ShapeCategory_TriColPolygon;} break;
        case ShapeCategory_TriColCircle: {*category = ShapeCategory_TriColCircle;} break;
        case ShapeCategory_TriColCapsule: {*category = ShapeCategory_TriColCapsule;} break;

        case ShapeCategory_KinColPolygon: {*category = ShapeCategory_KinColPolygon;} break;
        case ShapeCategory_KinColCircle: {*category = ShapeCategory_KinColCircle;} break;
        case ShapeCategory_KinColCapsule: {*category = ShapeCategory_KinColCapsule;} break;

        default: {ASSERT(false, "unknown category.");} break;
    }
}




/**====================
    functions: Entity.
====================**//**/




inline void shape_set_active_unsafe(FIZXState* state, i32 shape_idx, bool is_active){
    BOUNDS_CHECK(shape_idx, state->bodies.active_length);
    state->bodies.active[shape_idx] = is_active;
}

void shape_set_active(FIZXState* state, GenId shape){

}

void shape_set_local_transform_unsafe(FIZXState* state, i32 shape_idx, Transform2D transform){
    BOUNDS_CHECK(shape_idx, state->bodies.local_transform.length);
    BOUNDS_CHECK(shape_idx, state->bodies.local_transform.position.length);
    BOUNDS_CHECK(shape_idx, state->bodies.local_transform.scale.length);
    state->bodies.local_transform.position.x[shape_idx] = transform.position.x;
    state->bodies.local_transform.position.y[shape_idx] = transform.position.y;
    state->bodies.local_transform.scale.x[shape_idx] = transform.scale.x;
    state->bodies.local_transform.scale.y[shape_idx] = transform.scale.y;
    state->bodies.local_transform.cosine[shape_idx] = transform.cosine;
    state->bodies.local_transform.sine[shape_idx] = transform.sine;
}

inline void body_set_active_unsafe(FIZXState* state, i32 body_idx, bool is_active){
    BOUNDS_CHECK(body_idx, state->bodies.active_length);
    state->bodies.active[body_idx] = is_active;
}

bool body_set_active(FIZXState* state, GenId body_gid, bool is_active){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        return false;
    }
    i32 body_idx = gen_id_get_index(body_gid);
    BOUNDS_CHECK(body_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[body_idx] != EntityType_Body){
        ASSERT(false, "not a body.");
        return false;
    }
    body_set_active_unsafe(state, body_idx, is_active);
    return true;
}

bool body_is_active_unsafe(FIZXState* state, i32 body_idx){
    BOUNDS_CHECK(body_idx, state->bodies.active_length);
    return state->bodies.active[body_idx];
}

bool body_is_active(FIZXState* state, GenId body_gid){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        return false;
    }
    i32 body_idx = gen_id_get_index(body_gid);
    BOUNDS_CHECK(body_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[body_idx] != EntityType_Body){
        ASSERT(false, "not a body.");
        return false;
    }
    return body_is_active_unsafe(state, body_idx);
}

void body_set_local_transform_unsafe(FIZXState* state, i32 body_idx, Transform2D transform){
    BOUNDS_CHECK(body_idx, state->bodies.local_transform.length);
    BOUNDS_CHECK(body_idx, state->bodies.local_transform.position.length);
    BOUNDS_CHECK(body_idx, state->bodies.local_transform.scale.length);
    state->bodies.local_transform.position.x[body_idx] = transform.position.x;
    state->bodies.local_transform.position.y[body_idx] = transform.position.y;
    state->bodies.local_transform.scale.x[body_idx] = transform.scale.x;
    state->bodies.local_transform.scale.y[body_idx] = transform.scale.y;
    state->bodies.local_transform.cosine[body_idx] = transform.cosine;
    state->bodies.local_transform.sine[body_idx] = transform.sine;
}

bool body_set_local_transform(FIZXState* state, GenId body_gid, Transform2D transform){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        return false;
    }
    i32 body_idx = gen_id_get_index(body_gid);
    BOUNDS_CHECK(body_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[body_idx] != EntityType_Body){
        ASSERT(false, "not a body.");
        return false;
    }
    body_set_local_transform_unsafe(state, body_idx, transform);
    return true;
}

void body_set_global_transform_unsafe(FIZXState* state, i32 body_idx, Transform2D transform){
    BOUNDS_CHECK(body_idx, state->bodies.global_transform.length);
    BOUNDS_CHECK(body_idx, state->bodies.global_transform.position.length);
    BOUNDS_CHECK(body_idx, state->bodies.global_transform.scale.length);
    state->bodies.global_transform.position.x[body_idx] = transform.position.x;
    state->bodies.global_transform.position.y[body_idx] = transform.position.y;
    state->bodies.global_transform.scale.x[body_idx] = transform.scale.x;
    state->bodies.global_transform.scale.y[body_idx] = transform.scale.y;
    state->bodies.global_transform.cosine[body_idx] = transform.cosine;
    state->bodies.global_transform.sine[body_idx] = transform.sine;
}

bool body_set_global_transform(FIZXState* state, GenId body_gid, Transform2D transform){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        return false;
    }
    i32 body_idx = gen_id_get_index(body_gid);
    BOUNDS_CHECK(body_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[body_idx] != EntityType_Body){
        ASSERT(false, "not a body.");
        return false;
    }
    body_set_global_transform_unsafe(state, body_idx, transform);
    return true;
}

Vector2 body_get_linear_velocity_unsafe(FIZXState* state, i32 body_idx){
    Soa_Vector2* soa = &state->bodies.linear_velocity;
    BOUNDS_CHECK(body_idx, soa->length);
    return (Vector2){.x = soa->x[body_idx], .y = soa->y[body_idx]};
}

Vector2 body_get_linear_velocity(FIZXState* state, GenId body_gid){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        return (Vector2){0};
    }
    i32 body_idx = gen_id_get_index(body_gid);
    BOUNDS_CHECK(body_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[body_idx] != EntityType_Body){
        ASSERT(false, "not a body.");
        return (Vector2){0};
    }
    if(!body_is_active_unsafe(state, body_idx)){
        return (Vector2){0};
    }
    return body_get_linear_velocity_unsafe(state, body_idx);
}

void body_translate_unsafe(FIZXState* state, f32 displacement_x, f32 displacement_y, i32 body_idx){
    BOUNDS_CHECK(body_idx, state->bodies.shape_collision_displacement.length);
    f32 tx = state->bodies.shape_collision_displacement.x[body_idx];
    f32 ty = state->bodies.shape_collision_displacement.y[body_idx];
    if(tx == 0 || ty == 0){
        ARRAY_PUSH(state->bodies.displaced_this_sub_step, state->bodies.displaced_this_sub_step_length, &state->bodies.displaced_this_sub_step_count, body_idx);
    }
    tx += displacement_x;
    ty += displacement_y;
}

void bodies_calculate_bvh_leaf_padding(
    f32* current_position_x, i32 current_position_x_length,
    f32* current_position_y, i32 current_position_y_length,
    f32* previous_position_x, i32 previous_position_x_length,
    f32* previous_position_y, i32 previous_position_y_length,
    f32* out_bvh_leaf_padding, i32 bvh_leaf_padding_length,
    i32* active, i32 active_length, i32 active_count,
    f32 delta_time
){
    for(i32 i = 0; i < active_count; i++){
        i32 idx = active[i];

        BOUNDS_CHECK(idx, current_position_x_length);
        BOUNDS_CHECK(idx, previous_position_x_length);
        f32 delta_movement_x = current_position_x[idx] - previous_position_x[idx];

        BOUNDS_CHECK(i, current_position_y_length);
        BOUNDS_CHECK(i, previous_position_y_length);
        f32 delta_movement_y = current_position_y[idx] - previous_position_y[idx];
        f32 delta_movement = MAX(ABS(delta_movement_x), ABS(delta_movement_y));
        f32 time_factor = 1 + delta_time;

        BOUNDS_CHECK(idx, bvh_leaf_padding_length);
        out_bvh_leaf_padding[idx] = delta_movement * time_factor;
    }
}

void body_clear_forces_and_velocities_unsafe(FIZXState* state, i32 body_idx){
    BOUNDS_CHECK(body_idx, state->bodies.linear_velocity.length);
    state->bodies.linear_velocity.x[body_idx] = 0;
    state->bodies.linear_velocity.y[body_idx] = 0;
    BOUNDS_CHECK(body_idx, state->bodies.angular_velocity_length);
    state->bodies.angular_velocity[body_idx] = 0;
    BOUNDS_CHECK(body_idx, state->bodies.force.length);
    state->bodies.force.x[body_idx] = 0;
    state->bodies.force.y[body_idx] = 0;
}

/**
    Calculates the inverse/mass, inverse/rotational inertia, and center of mass for a rigid body.
**/
void body_integrate_shape_properties_unsafe(FIZXState* state, i32 body_idx){
    f32 center_of_mass_x = 0;
    f32 center_of_mass_y = 0;
    f32 total_mass = 0;
    f32 total_inverse_mass = 0;
    f32 total_rotational_inertia = 0;
    f32 total_inverse_rotational_inertia = 0;

    // fallback to the global position if there are not valid rigid shapes associated with the body.
    BOUNDS_CHECK(body_idx, state->bodies.global_transform.length);
    f32 body_global_pos_x =  state->bodies.global_transform.position.x[body_idx];
    f32 body_global_pos_y =  state->bodies.global_transform.position.y[body_idx];

    IntrusiveListNode* node = &state->body_hierarchy.node[body_idx];
    i32 first_shape_idx = node->first_child;
    if(first_shape_idx!=0){
        // calc center of mass & total mass.
        {
           i32 shape_idx = first_shape_idx;
           center_of_mass_x = 0.0f;
           center_of_mass_y = 0.0f;

            while(true){

                BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
                f32 mass = state->bodies.mass[shape_idx];
                total_mass += mass;

                BOUNDS_CHECK(shape_idx, state->bodies.centroid.length);
                f32 centroid_x = state->bodies.centroid.x[shape_idx];
                f32 centroid_y = state->bodies.centroid.y[shape_idx];
                center_of_mass_x -= mass * (body_global_pos_x - centroid_x);
                center_of_mass_y -= mass * (body_global_pos_y - centroid_y);

                BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
                IntrusiveListNode* shape_node = &state->body_hierarchy.node[shape_idx];
                i32 next_shape_idx = shape_node->next_sibling;
                if(next_shape_idx == first_shape_idx){
                    /**
                        center of mass in global space.
                        note: use global-sapce coordinates first as shape centroids are in global space.
                    **/
                    center_of_mass_x /= total_mass;
                    center_of_mass_y /= total_mass;
                    total_inverse_mass = 1.0f / total_mass;
                    break;
                }
                shape_idx = next_shape_idx;
            }
        }

        // calc total rotational inertia.
        {
            i32 shape_idx = first_shape_idx;

            // move center of mass into global space.
            center_of_mass_x += body_global_pos_x;
            center_of_mass_y += body_global_pos_y;

            while(true){
                BOUNDS_CHECK(shape_idx, state->bodies.centroid.length);
                f32 centroid_x = state->bodies.centroid.x[shape_idx];
                f32 centroid_y = state->bodies.centroid.y[shape_idx];
                f32 dist_sqrd = vector2_dist_sqrd_scalar(centroid_x, centroid_y, center_of_mass_x, center_of_mass_y);

                BOUNDS_CHECK(shape_idx, state->bodies.rotational_inertia_length);
                f32 rotational_inertia = state->bodies.rotational_inertia[shape_idx];

                BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
                f32 mass = state->bodies.mass[shape_idx];

                total_rotational_inertia += rotational_inertia + (mass * dist_sqrd);

                BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
                IntrusiveListNode* shape_node = &state->body_hierarchy.node[shape_idx];
                i32 next_shape_idx = shape_node->next_sibling;
                if(next_shape_idx == first_shape_idx){
                    // move centerof mass back into local space.
                    center_of_mass_x -= body_global_pos_x;
                    center_of_mass_y -= body_global_pos_y;
                    total_inverse_rotational_inertia = 1.0f / total_rotational_inertia;
                    break;
                }

                shape_idx = next_shape_idx;
            }
        }
    }

    BOUNDS_CHECK(body_idx, state->bodies.mass_length);
    state->bodies.mass[body_idx] = total_mass;

    BOUNDS_CHECK(body_idx, state->bodies.inverse_mass_length);
    state->bodies.inverse_mass[body_idx] = total_inverse_mass;

    BOUNDS_CHECK(body_idx, state->bodies.local_center_of_mass.length);
    state->bodies.local_center_of_mass.x[body_idx] = center_of_mass_x;
    state->bodies.local_center_of_mass.y[body_idx] = center_of_mass_y;

    BOUNDS_CHECK(body_idx, state->bodies.rotational_inertia_length);
    state->bodies.rotational_inertia[body_idx] = total_rotational_inertia;

    BOUNDS_CHECK(body_idx, state->bodies.inverse_rotational_inertia_length);
    state->bodies.inverse_rotational_inertia[body_idx] = total_inverse_rotational_inertia;

}

/**
    `remarks`:
    stale id and entity type checks are not enforced; the entity index will always go through the deallocation procedure.
**/
void shape_dealloc_unsafe(FIZXState* state, i32 shape_idx, bool recalculate_body_center_of_mass){
    BOUNDS_CHECK(shape_idx, state->bodies.category_length);
    i32 category = state->bodies.category[shape_idx];

    // decrement the category counter in the state.
    switch (category){
        case ShapeCategory_DynRigPolygon: {state->polygon_rigid_count.dynamic -= 1;} break;
        case ShapeCategory_DynRigCircle: {state->circle_rigid_count.dynamic -= 1;} break;
        case ShapeCategory_DynRigCapsule: {state->capsule_rigid_count.dynamic -= 1;} break;

        case ShapeCategory_TriRigPolygon: {state->polygon_rigid_count.trigger -= 1;} break;
        case ShapeCategory_TriRigCircle: {state->circle_rigid_count.trigger -= 1;} break;
        case ShapeCategory_TriRigCapsule: {state->capsule_rigid_count.trigger -= 1;} break;

        case ShapeCategory_KinRigPolygon: {state->polygon_rigid_count.kinematic -= 1;} break;
        case ShapeCategory_KinRigCircle: {state->circle_rigid_count.kinematic -= 1;} break;
        case ShapeCategory_KinRigCapsule: {state->capsule_rigid_count.kinematic -= 1;} break;

        case ShapeCategory_DynColPolygon: {state->polygon_collider_count.dynamic -= 1;} break;
        case ShapeCategory_DynColCircle: {state->circle_collider_count.dynamic -= 1;} break;
        case ShapeCategory_DynColCapsule: {state->capsule_collider_count.dynamic -= 1;} break;

        case ShapeCategory_TriColPolygon: {state->polygon_collider_count.trigger -= 1;} break;
        case ShapeCategory_TriColCircle: {state->circle_collider_count.trigger -= 1;} break;
        case ShapeCategory_TriColCapsule: {state->capsule_collider_count.trigger -= 1;} break;

        case ShapeCategory_KinColPolygon: {state->polygon_collider_count.kinematic -= 1;} break;
        case ShapeCategory_KinColCircle: {state->circle_collider_count.kinematic -= 1;} break;
        case ShapeCategory_KinColCapsule: {state->capsule_collider_count.kinematic -= 1;} break;

        default: {ASSERT(false, "unknwon category.");} break;
    }

    shape_set_active_unsafe(state, shape_idx, false);
    intrusive_list_remove_node(&state->body_hierarchy, shape_idx);
    gen_id_allocator_dealloc_unsafe(&state->gen_id_allocator, shape_idx);
    if(recalculate_body_center_of_mass){
        BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
        i32 body_idx = state->body_hierarchy.node[shape_idx].parent;
        body_integrate_shape_properties_unsafe(state, body_idx);
    }
}

bool shape_dealloc(FIZXState* state, GenId gid, bool recalculate_body_center_of_mass){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, gid)){
        return false;
    }

    i32 gidx = gen_id_get_index(gid);
    BOUNDS_CHECK(gidx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[gidx] != EntityType_Shape){
        ASSERT(false, "attempted to dealloc an entity that isnt a shape.");
        return false;
    }

    shape_dealloc_unsafe(state, gidx, recalculate_body_center_of_mass);
    return true;
}

/**
    allocates a body into a physics state.

    `returns`:
    A gen-id handle to the allocated body; note that it returns zero when failing to allocate a body.
**/
GenId body_alloc(FIZXState* state, Transform2D global_transform, bool gravity_affected){
    GenId gid = gen_id_allocator_alloc(&state->gen_id_allocator);
    ASSERT(gid != (GenId){0}, "memory limit hit");
    i32 body_idx = gen_id_get_index(gid);

    body_set_active_unsafe(state, body_idx, true);
    body_set_global_transform_unsafe(state, body_idx, global_transform);
    state->bodies.shape_collision_displacement.x[body_idx] = 0;
    state->bodies.shape_collision_displacement.y[body_idx] = 0;
    state->bodies.mass[body_idx] = 0;
    state->bodies.inverse_mass[body_idx] = 0;
    state->bodies.entity_type[body_idx] = EntityType_Body;
    state->bodies.gravity_affected[body_idx] = gravity_affected;
    body_clear_forces_and_velocities_unsafe(state, body_idx);

    bool added_root = intrusive_list_add_root(&state->body_hierarchy, body_idx);
    if(!added_root){
        ASSERT(false, "failed to make physics body a root node.");
        gen_id_allocator_dealloc(&state->gen_id_allocator, gid);
        body_set_active_unsafe(state, body_idx, false);
        return (GenId){0};
    }
    return gid;
}

/**
    `remarks`:
    stale id and entity type checks are not enforced; the `body_idx` will always go through the deallocation procedure.
**/
void body_dealloc_unsafe(FIZXState* state, i32 body_idx){
    gen_id_allocator_dealloc_unsafe(&state->gen_id_allocator, body_idx);
    /**
        deallocate all shapes.
        note the reverse order and starting deallocation at the last child.
        this is so first shape is preserved until the end of the loop, ensuring the loop knows when to stop.
    **/
    IntrusiveListNode* node = state->body_hierarchy.node;
    BOUNDS_CHECK(body_idx, state->body_hierarchy.length);
    i32 first_shape_idx = node[body_idx].first_child;
    BOUNDS_CHECK(first_shape_idx, state->body_hierarchy.length);
    i32 last_shape_idx = node[first_shape_idx].previous_sibling;
    if(last_shape_idx != 0){
        i32 shape_idx = last_shape_idx;
        i32 previous_shape_idx = 0;
        while(true){
            if(shape_idx == previous_shape_idx){
                break;
            }
            shape_dealloc_unsafe(state, shape_idx, false);
            previous_shape_idx = shape_idx;
            BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
            shape_idx = node[shape_idx].previous_sibling;
        }
    }

    intrusive_list_remove_node(&state->body_hierarchy, body_idx);
    BOUNDS_CHECK(body_idx, state->bodies.gravity_affected_length);
    state->bodies.gravity_affected[body_idx] = false;
    body_set_active_unsafe(state, body_idx, false);
}

bool body_dealloc(FIZXState* state, GenId gid){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, gid)){
        return false;
    }

    i32 gidx = gen_id_get_index(gid);
    BOUNDS_CHECK(gidx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[gidx] != EntityType_Body){
        ASSERT(false, "attempted to dealloc an entity that isnt a body.");
        return false;
    }

    body_dealloc_unsafe(state, gidx);
    return true;
}

i32 shape_set_category_unsafe(FIZXState* state, ShapeType shape_type, ShapeBehaviour behaviour, bool is_rigid, i32 shape_idx){
    BOUNDS_CHECK(shape_idx, state->bodies.shape_type_length);
    state->bodies.shape_type[shape_idx] = shape_type;
    i32* category = &state->bodies.category[shape_idx];

    switch(shape_type){
        case ShapeType_Rectangle:{
            switch(behaviour){
                case ShapeBehaviour_Dynamic: {*category = is_rigid? ShapeCategory_DynRigPolygon : ShapeCategory_DynColPolygon;} break;
                case ShapeBehaviour_Kinematic: {*category = is_rigid? ShapeCategory_KinRigPolygon : ShapeCategory_KinColPolygon;} break;
                case ShapeBehaviour_Trigger: {*category = is_rigid? ShapeCategory_TriRigPolygon : ShapeCategory_TriColPolygon;} break;
                default:{ASSERT(false, "unknown shape behaviour.");}break;
            }
        }break;
        case ShapeType_Circle:{
            switch(behaviour){
                case ShapeBehaviour_Dynamic: {*category = is_rigid? ShapeCategory_DynRigCircle : ShapeCategory_DynColCircle;} break;
                case ShapeBehaviour_Kinematic: {*category = is_rigid? ShapeCategory_KinRigCircle : ShapeCategory_KinColCircle;} break;
                case ShapeBehaviour_Trigger: {*category = is_rigid? ShapeCategory_TriRigCircle : ShapeCategory_TriColCircle;} break;
                default:{ASSERT(false, "unknown shape behaviour.");}break;
            }
        }break;
        default:{ASSERT(false, "unknown shape category.");}break;
    }
    return *category;
}

void fizx_shape_init_prepare(FIZXState* state, ShapeType type, ShapeBehaviour behaviour, i32 shape_idx, i32 body_idx, bool is_rigid){
    state->bodies.entity_type[shape_idx];
    // clear any garbage data from previous allocations.
    fssoa_vector2_clear_chunk_count(&state->bodies.base_vertex, shape_idx);

    // set this so that the previous position isnt garbage from previous steps.
    BOUNDS_CHECK(shape_idx, state->bodies.global_transform.position.length);
    f32 global_pos_x = state->bodies.global_transform.position.x[shape_idx];
    f32 global_pos_y = state->bodies.global_transform.position.y[shape_idx];
    BOUNDS_CHECK(shape_idx, state->bodies.previous_step_position.length);
    state->bodies.previous_step_position.x[shape_idx] = global_pos_x;
    state->bodies.previous_step_position.y[shape_idx] = global_pos_y;

    // set the new data.
    shape_set_active_unsafe(state, shape_idx, true);
    i32 category = shape_set_category_unsafe(state, type, behaviour, is_rigid, shape_idx);

    // increment category counter.
    switch(category){
        case ShapeCategory_DynRigPolygon: {state->polygon_rigid_count.dynamic += 1;} break;
        case ShapeCategory_DynRigCircle: {state->circle_rigid_count.dynamic += 1;} break;
        case ShapeCategory_DynRigCapsule: {state->capsule_rigid_count.dynamic += 1;} break;

        case ShapeCategory_TriRigPolygon: {state->polygon_rigid_count.trigger += 1;} break;
        case ShapeCategory_TriRigCircle: {state->circle_rigid_count.trigger += 1;} break;
        case ShapeCategory_TriRigCapsule: {state->capsule_rigid_count.trigger += 1;} break;

        case ShapeCategory_KinRigPolygon: {state->polygon_rigid_count.kinematic += 1;} break;
        case ShapeCategory_KinRigCircle: {state->circle_rigid_count.kinematic += 1;} break;
        case ShapeCategory_KinRigCapsule: {state->capsule_rigid_count.kinematic += 1;} break;

        case ShapeCategory_DynColPolygon: {state->polygon_collider_count.dynamic += 1;} break;
        case ShapeCategory_DynColCircle: {state->circle_collider_count.dynamic += 1;} break;
        case ShapeCategory_DynColCapsule: {state->capsule_collider_count.dynamic += 1;} break;

        case ShapeCategory_TriColPolygon: {state->polygon_collider_count.trigger += 1;} break;
        case ShapeCategory_TriColCircle: {state->circle_collider_count.trigger += 1;} break;
        case ShapeCategory_TriColCapsule: {state->capsule_collider_count.trigger += 1;} break;

        case ShapeCategory_KinColPolygon: {state->polygon_collider_count.kinematic += 1;} break;
        case ShapeCategory_KinColCircle: {state->circle_collider_count.kinematic += 1;} break;
        case ShapeCategory_KinColCapsule: {state->capsule_collider_count.kinematic += 1;} break;

        default: {ASSERT(false, "unknown category.");} break;
    }
}

void fizx_shape_init_finalise(
    FIZXState* state, Transform2D local_transform, f32* shape_vert_x, f32* shape_vert_y, i32 shape_vert_length, i32 shape_idx, i32 body_idx, bool is_rigid
){
    /**
        note:
        order matters here (from top to bottom):
        - set Transform2D data
        - set vertce data
        - Transform2D vertice data (getting centroid as well).
        - add shape to tree.
        - integrate the now intialised shape into the body (if it is a rigid shape.)
    **/

    BOUNDS_CHECK(shape_idx, state->bodies.global_transform.length);
    BOUNDS_CHECK(body_idx, state->bodies.global_transform.length);
    Soa_Transform2D* global_transforms = &state->bodies.global_transform;
    transform2d_transform_scalar(
        local_transform.position.x, local_transform.position.y, local_transform.scale.x, local_transform.scale.y, local_transform.sine, local_transform.cosine, local_transform.rotation,
        global_transforms->position.x[body_idx], global_transforms->position.y[body_idx], global_transforms->scale.x[body_idx], global_transforms->scale.y[body_idx], global_transforms->sine[body_idx], global_transforms->cosine[body_idx], global_transforms->rotation[body_idx],
        &global_transforms->position.x[shape_idx], &global_transforms->position.y[shape_idx], &global_transforms->scale.x[shape_idx], &global_transforms->scale.y[shape_idx], &global_transforms->sine[shape_idx], &global_transforms->cosine[shape_idx], &global_transforms->rotation[shape_idx]
    );
    shape_set_local_transform_unsafe(state, shape_idx, local_transform);

    for(i32 i = 0; i < shape_vert_length; i++){
        fssoa_vector2_push(&state->bodies.base_vertex, shape_idx, shape_vert_x[i], shape_vert_y[i]);
    }

    soa_body_transform_shape_vertices(&state->bodies, shape_idx);
    intrusive_list_add_branch(&state->body_hierarchy, shape_idx, body_idx);

    if(is_rigid){
        body_integrate_shape_properties_unsafe(state, body_idx);
    }
}

void shape_set_rotational_response_unsafe(FIZXState* state, i32 shape_idx, bool enabled){
    BOUNDS_CHECK(shape_idx, state->bodies.rotational_response_length);
    state->bodies.rotational_response[shape_idx] = enabled;
}

bool shape_set_rotational_repsonse(FIZXState* state, GenId shape_gid, bool enabled){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, shape_gid)){
        return false;
    }
    i32 shape_idx = gen_id_get_index(shape_gid);
    BOUNDS_CHECK(shape_idx, state->bodies.entity_type_length);
    if(state->bodies.entity_type[shape_idx] != EntityType_Shape){
        ASSERT(false, "entity is not shape.");
        return false;
    }
    shape_set_rotational_response_unsafe(state, shape_idx, enabled);
    return true;
}

/**
    `returns`:
    the gen-id to the allocate shape collider; otherwise zero upon failure.
**/
GenId fizx_circle_collider_alloc(FIZXState* state, GenId body_gid, Circle shape, Transform2D transform, ShapeBehaviour behaviour){

    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        ASSERT(false, "invalid body gid");
        return (GenId){0};
    }

    GenId gid = gen_id_allocator_alloc(&state->gen_id_allocator);
    if(gid == (GenId){0}){
        ASSERT(false, "failed");
        return gid;
    }

    i32 shape_idx = gen_id_get_index(gid);
    i32 body_idx = gen_id_get_index(body_gid);

    fizx_shape_init_prepare(state, ShapeType_Circle, behaviour, shape_idx, body_idx, false);

    // set specific data.
    {
        // rigidbodies should respond to this like a kinematic rigidbody if it is solid or kinematic.
        BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
        state->bodies.mass[shape_idx] = 0;
        BOUNDS_CHECK(shape_idx, state->bodies.inverse_mass_length);
        state->bodies.inverse_mass[shape_idx] = 0;
        BOUNDS_CHECK(shape_idx, state->bodies.base_radius_length);
        state->bodies.base_radius[shape_idx] = shape.radius;
    }

    fizx_shape_init_finalise(state, transform, &shape.x, &shape.y, 1, shape_idx, body_idx, false);
    return gid;
}

GenId fizx_circle_rigid_alloc(FIZXState* state, GenId body_gid, Circle shape, Transform2D local_transform, ShapeBehaviour behaviour, Material material, bool rotational_repsonse){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        ASSERT(false, "invalid body gid");
        return (GenId){0};
    }

    GenId gid = gen_id_allocator_alloc(&state->gen_id_allocator);
    if(gid == (GenId){0}){
        ASSERT(false, "failed");
        return gid;
    }

    i32 shape_idx = gen_id_get_index(gid);
    i32 body_idx = gen_id_get_index(body_gid);

    fizx_shape_init_prepare(state, ShapeType_Circle, behaviour, shape_idx, body_idx, true);

    { // set specific data.
        shape_set_rotational_response_unsafe(state, shape_idx, rotational_repsonse);
        fizx_soa_material_insert(&state->bodies.material, material.static_friction, material.kinetic_friction, material.density, material.restitution, shape_idx);
        BOUNDS_CHECK(shape_idx, state->bodies.base_radius_length);
        state->bodies.base_radius[shape_idx] = shape.radius;
    }

    // integrate properties.
    {
        BOUNDS_CHECK(body_idx, state->bodies.global_transform.length);
        f32 global_pos_x = state->bodies.global_transform.position.x[body_idx];
        f32 global_pos_y = state->bodies.global_transform.position.y[body_idx];
        f32 global_radius = MAX(state->bodies.global_transform.scale.x[body_idx], state->bodies.global_transform.scale.y[body_idx]) * shape.radius;

        BOUNDS_CHECK(shape_idx, state->bodies.global_radius_length);
        state->bodies.global_radius[shape_idx] = global_radius;

        BOUNDS_CHECK(shape_idx, state->bodies.material.length);
        f32 mass = state->bodies.material.density[shape_idx] * circle_get_area_scalar(shape.radius);
        BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
        state->bodies.mass[shape_idx] = mass;
        BOUNDS_CHECK(shape_idx, state->bodies.inverse_mass_length);
        state->bodies.inverse_mass[shape_idx] = mass == 0? 0 : 1.0f / mass;

        f32 inertia = CIRCLE_ROTATIONAL_INERTIA * mass * (shape.radius * shape.radius);
        BOUNDS_CHECK(shape_idx, state->bodies.rotational_inertia_length);
        state->bodies.rotational_inertia[shape_idx] = inertia;
        BOUNDS_CHECK(shape_idx, state->bodies.inverse_rotational_inertia_length);
        state->bodies.inverse_rotational_inertia[shape_idx] = inertia == 0? 0.0f : 1.0f / inertia;
    }

    fizx_shape_init_finalise(state, local_transform, &shape.x, &shape.y, 1, shape_idx, body_idx, true);

    return gid;
}

GenId fizx_rectangle_collider_alloc(FIZXState* state, Rectangle shape, Transform2D local_transform, ShapeBehaviour behaviour, GenId body_gid){

    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        ASSERT(false, "invalid body gid");
        return (GenId){0};
    }

    GenId gid = gen_id_allocator_alloc(&state->gen_id_allocator);
    if(gid == (GenId){0}){
        return gid;
    }

    PolygonRectangle poly = polygon_rectangle_from_rectangle(shape);
    i32 shape_idx = gen_id_get_index(gid);
    i32 body_idx = gen_id_get_index(body_gid);

    fizx_shape_init_prepare(state, ShapeType_Rectangle, behaviour, shape_idx, body_idx, false);

    // set specific data.
    {
        // apply data.
        BOUNDS_CHECK(shape_idx, state->bodies.base_height_length);
        state->bodies.base_height[shape_idx] = shape.height;
        BOUNDS_CHECK(shape_idx, state->bodies.base_width_length);
        state->bodies.base_width[shape_idx] = shape.width;

        // rigidbodies should respond to this like a kinematic rigidbody if it is solid or kinematic.
        BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
        state->bodies.mass[shape_idx] = 0;
        BOUNDS_CHECK(shape_idx, state->bodies.inverse_mass_length);
        state->bodies.inverse_mass[shape_idx] = 0;
    }

    fizx_shape_init_finalise(state, local_transform, poly.x, poly.y, POLYGON_RECTANGLE_VERTICES_LENGTH, shape_idx, body_idx, false);

    return gid;
}

GenId fizx_rectangle_rigid_alloc(FIZXState* state, Rectangle shape, Transform2D local_transform, ShapeBehaviour behaviour, GenId body_gid, Material material, bool rotational_response){

    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_gid)){
        ASSERT(false, "invalid body gid");
        return (GenId){0};
    }

    GenId gid = gen_id_allocator_alloc(&state->gen_id_allocator);
    if(gid == (GenId){0}){
        return gid;
    }

    PolygonRectangle poly = polygon_rectangle_from_rectangle(shape);
    i32 shape_idx = gen_id_get_index(gid);
    i32 body_idx = gen_id_get_index(body_gid);

    fizx_shape_init_prepare(state, ShapeType_Rectangle, behaviour, shape_idx, body_idx, true);

    // set specific data.
    {
        shape_set_rotational_response_unsafe(state, shape_idx, rotational_response);
        BOUNDS_CHECK(shape_idx, state->bodies.base_height_length);
        state->bodies.base_height[shape_idx] = shape.height;
        BOUNDS_CHECK(shape_idx, state->bodies.base_width_length);
        state->bodies.base_width[shape_idx] = shape.width;
        fizx_soa_material_insert(&state->bodies.material, material.static_friction, material.kinetic_friction, material.density, material.restitution, shape_idx);
    }

    // integrate rigid properties.
    {
        BOUNDS_CHECK(body_idx, state->bodies.global_transform.scale.length);
        f32 global_width = shape.width * state->bodies.global_transform.scale.x[body_idx];
        f32 global_height = shape.height * state->bodies.global_transform.scale.y[body_idx];

        BOUNDS_CHECK(shape_idx, state->bodies.material.length);
        f32 mass = state->bodies.material.density[shape_idx] * (global_height * global_width);
        BOUNDS_CHECK(shape_idx, state->bodies.mass_length);
        state->bodies.mass[shape_idx] = mass;
        state->bodies.inverse_mass[shape_idx] = mass == 0.0f ? 0.0f : 1.0f / mass;

        f32 inertia = RECTANGLE_ROTATIONAL_INERTIA * mass * ((global_width * global_width) + (global_height * global_height));
        BOUNDS_CHECK(shape_idx, state->bodies.rotational_inertia_length);
        state->bodies.rotational_inertia[shape_idx] = inertia;
        BOUNDS_CHECK(shape_idx, state->bodies.inverse_rotational_inertia_length);
        state->bodies.inverse_rotational_inertia[shape_idx] = inertia == 0.0f ? 0.0f : 1.0f / inertia;
    }

    fizx_shape_init_finalise(state, local_transform, poly.x, poly.y, POLYGON_RECTANGLE_VERTICES_LENGTH, shape_idx, body_idx, true);

    return gid;
}




/**====================
    extension functions: BvhCategorisedLeafOverlaps.
====================**//**/




/**
    Formats overlap data so that the `owner` of an overlap is always the `solid` collider.

    `parameters`
    `overlaps`: the overlap instance to format.
    `bvhLeafIndices`: the mapping of bvh leaf indices onto a physics body.
    `bvhCategories`: the categories of all physics bodies when being put into the bvh.
**/
void fizx_bvh_categorised_leaf_overlaps_format(BvhCategorisedLeafOverlaps* overlaps, i32* bvh_leaf_index, i32 bvh_leaf_index_length, i32* bvh_category, i32 bvh_category_length){
    i32 temp;
    i32* owner_leaf;
    i32* other_leaf;
    i32 owner_index;
    i32 other_index;
    i32 other_category;
    i32 owner_category;
    for(i32 i = 0; i < ShapeCategory_Count; i++){
        for(i32 j = 0; j < ShapeCategory_Count; j++){
            BvhOverlapInfo info = bvh_categorised_leaf_overlaps_get_overlaps(*overlaps, i, j);
            for(i32 w = 0; w < info.length; w++){
                //get the data about the owner and other.
                owner_leaf = &info.owner_leaf_index[w];
                other_leaf = &info.owner_leaf_index[w];

                BOUNDS_CHECK(*owner_leaf, bvh_leaf_index_length);
                owner_index = bvh_leaf_index[*owner_leaf];
                BOUNDS_CHECK(*other_leaf, bvh_leaf_index_length);
                other_index = bvh_leaf_index[*other_leaf];

                BOUNDS_CHECK(owner_index, bvh_category_length);
                owner_category = bvh_category[owner_index];
                BOUNDS_CHECK(other_index, bvh_category_length);
                other_category = bvh_category[other_index];

                // order the leaves in ascending order.
                if(owner_category > other_category){
                    temp = *owner_leaf;
                    *owner_leaf = *other_leaf;
                    *other_leaf = temp;
                }
            }
        }
    }
}


/**====================
    fuctions: CollisionDetection.
====================**//**/

/**

**/
bool collision_detection_polygon_to_polygon(
    CollisionManifold* manifold, FsSoa_Vector2 vertices, Soa_Vector2 centroids, i32 a_idx, i32 b_idx, IndexPair* out_idx_pair
){
    BOUNDS_CHECK(a_idx, centroids.length);
    f32* a_cen_x = &centroids.x[a_idx];
    f32* a_cen_y = &centroids.y[a_idx];

    BOUNDS_CHECK(b_idx, centroids.length);
    f32* b_cen_x = &centroids.x[b_idx];
    f32* b_cen_y = &centroids.y[b_idx];

    f32* a_vert_x;
    f32* a_vert_y;
    i32 a_vert_length;
    f32* b_vert_x;
    f32* b_vert_y;
    i32 b_vert_length;

    // gather polygon vertices.
    soa_body_get_vertices_unsafe(vertices, a_idx, &a_vert_x, &a_vert_y, &a_vert_length);
    soa_body_get_vertices_unsafe(vertices, b_idx, &b_vert_x, &b_vert_y, &b_vert_length);

    // narrow phase.
    f32 normal_x;
    f32 normal_y;
    f32 depth;
    bool overlaps = polygon_overlaps(
        a_vert_x, a_vert_y, a_vert_length,
        b_vert_x, b_vert_y, b_vert_length,
        *a_cen_x, *a_cen_y,
        *b_cen_x, *b_cen_y,
        &normal_x, &normal_y, &depth
    );

    if(overlaps){
        f32 contact_point_1_x;
        f32 contact_point_1_y;
        f32 contact_point_2_x;
        f32 contact_point_2_y;
        i32 contact_count;

        polygon_find_contact_points(
            a_vert_x, a_vert_y, a_vert_length,
            b_vert_x, b_vert_y, b_vert_length,
            F32_EPSILON,
            &contact_point_1_x, &contact_point_1_y,
            &contact_point_2_x, &contact_point_2_y,
            &contact_count
        );

        ASSERT(contact_count <= 2 && contact_count >= 1, "unknown contact count.");

        *out_idx_pair = collision_manifold_set_data_two_way(
            manifold,
            a_idx, b_idx,
            *a_cen_x, *a_cen_y,
            *b_cen_x, *b_cen_y,
            normal_x, normal_y,
            contact_point_1_x, contact_point_1_y,
            contact_point_2_x, contact_point_2_y,
            depth,
            contact_count == 2 ? true : false
        );

        return true;
    }
    return false;
}

/**
    `remarks`:
    - `IndexPair.a_to_b` = poly to circle
    - `IndexPair.b_to_a` = circle to poly
**/
bool collision_detection_polygon_to_circle(
    CollisionManifold* manifold, FsSoa_Vector2 vertices, Soa_Vector2 centroids, f32* radius, i32 radius_length, i32 poly_idx, i32 circ_idx, IndexPair* out_idx_pair
){
    BOUNDS_CHECK(poly_idx, centroids.length);
    f32 poly_cen_x = centroids.x[poly_idx];
    f32 poly_cen_y = centroids.y[poly_idx];
    BOUNDS_CHECK(circ_idx, centroids.length);
    f32 circ_cen_x = centroids.x[circ_idx];
    f32 circ_cen_y = centroids.y[circ_idx];
    BOUNDS_CHECK(circ_idx, radius_length);
    f32 circ_radius = radius[circ_idx];

    // gather polygon a vertices.
    f32* poly_vert_x;
    f32* poly_vert_y;
    i32 poly_vert_length;
    soa_body_get_vertices_unsafe(vertices, poly_idx, &poly_vert_x, &poly_vert_y, &poly_vert_length);

    f32 normal_x;
    f32 normal_y;
    f32 depth;

    bool overlaps = polygon_overlaps_circle_scalar(
        poly_vert_x, poly_vert_y, poly_vert_length, poly_cen_x, poly_cen_y,
        circ_cen_x, circ_cen_y, circ_radius, &normal_x, &normal_y, &depth
    );

    if(!overlaps){
        return false;
    }

    f32 contact_point_x;
    f32 contact_point_y;
    polygon_find_point_contact_points_scalar(
        poly_vert_x, poly_vert_y, poly_vert_length,
        circ_cen_x, circ_cen_y,
        &contact_point_x, &contact_point_y
    );
    *out_idx_pair = collision_manifold_set_data_two_way(
        manifold,
        poly_idx, circ_idx,
        poly_cen_x, poly_cen_y,
        circ_cen_x, circ_cen_y,
        normal_x, normal_y,
        contact_point_x, contact_point_y,
        0,0,
        depth, false
    );
    return true;
}

bool collision_detection_circle_to_circle(
    CollisionManifold* manifold, Soa_Vector2 centroids, f32* radius, i32 radius_length,
    i32 a_idx, i32 b_idx, IndexPair* out_index_pair
){
    f32 a_x = centroids.x[a_idx];
    f32 a_y = centroids.y[a_idx];

    f32 b_x = centroids.x[b_idx];
    f32 b_y = centroids.y[b_idx];

    f32 a_r = radius[a_idx];

    f32 b_r = radius[b_idx];

    f32 normal_x;
    f32 normal_y;
    f32 depth;

    bool overlaps = circle_overlaps_scalar(a_x, a_y, a_r, b_x, b_y, b_r, &normal_x, &normal_y, &depth);

    if(!overlaps){
        return false;
    }

    f32 cp_x;
    f32 cp_y;
    circle_calc_contact_points_scalar(a_x, a_y, a_r, b_x, b_y, &cp_x, &cp_y);
    *out_index_pair = collision_manifold_set_data_two_way(
        manifold,
        a_idx, b_idx,
        a_x, a_y,
        b_x, b_y,
        normal_x, normal_y,
        cp_x, cp_y,
        0, 0, depth, false
    );

    return true;
}

bool collision_detection_broad_phase(IntrusiveList body_hierarchy, Soa_Aabb aabb, i32 a_shape_idx, i32 b_shape_idx){
    // skip if the two shapes are apart of the same body.
    BOUNDS_CHECK(a_shape_idx, body_hierarchy.length);
    BOUNDS_CHECK(b_shape_idx, body_hierarchy.length);
    if(body_hierarchy.node[a_shape_idx].parent == body_hierarchy.node[b_shape_idx].parent){
        return false;
    }

    bool broad_phase = aabb_overlaps_scalar(
        aabb.min_x[a_shape_idx], aabb.min_x[b_shape_idx],
        aabb.min_y[a_shape_idx], aabb.min_y[b_shape_idx],
        aabb.max_x[a_shape_idx], aabb.max_x[b_shape_idx],
        aabb.max_y[a_shape_idx], aabb.max_y[b_shape_idx]
    );

    return broad_phase;
}




/**====================
    functions: State
====================**//**/




void fizx_state_init(FIZXState* state, MemoryArena* arena, i32 max_bodies, i32 vertices_per_body){
    ASSERT(!state->is_init, "already init");
    i32 max_collisions = max_bodies * max_bodies;
    fizx_soa_body_init(&state->bodies, arena, max_bodies, vertices_per_body);
    gen_id_allocator_init(&state->gen_id_allocator, arena, max_bodies);
    bvh_init(&state->bvh, arena, max_bodies);
    bvh_categorised_leaf_overlaps_init(&state->overlaps_scratch_buffer, arena, ShapeCategory_Count, max_collisions);
    collision_manifold_init(&state->collision_manifold, arena, max_bodies);
    categorised_overlap_array_init(&state->sub_step_shape_collisions_to_resolve, arena, CollisionResolutionCategory_Count, max_collisions, sizeof(STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE));
    categorised_overlap_array_init(&state->sub_step_rigid_collisions_to_resolve, arena, CollisionResolutionCategory_Count, max_collisions, sizeof(STATE_SUB_STEP_RIGID_COLLISIONS_TO_RESOLVE_TYPE));
    intrusive_list_init(&state->body_hierarchy, arena, max_bodies, false);
    state->gravity_direction = VECTOR2_DOWN;
    state->gravity_force = 9.81f;
    state->is_init = true;
}

/**
    constructs the bvh tree in relation to physics body data.

    `remarks`
    All arrays must be of equal length and elements should be accessed via a `physics_body_index` integer.
**/
void fizx_bvh_construct_tree(
    BoundingVolumeHierarchy* bvh,
    i32* active_body, i32 active_body_count, i32 active_body_length,
    IntrusiveListNode* node, i32 node_length,
    Soa_Aabb aabbs, Soa_Vector2 centroids,
    i32* bvh_category, i32 bvh_category_length,
    f32* bvh_leaf_padding, i32 bvh_leaf_padding_length,
    i32* bvh_leaf_index, i32 bvh_leaf_index_length
){
    // clear previous bvh data.
    bvh_clear(bvh);

    for(i32 i = 1; i < active_body_count; i++){ // start at 1 to avoid nil.
        BOUNDS_CHECK(i, active_body_length);
        i32 body_idx = active_body[i];

        BOUNDS_CHECK(body_idx, node_length);
        IntrusiveListNode* body_node = &node[body_idx];
        i32 first_shape_idx = body_node->first_child;
        if(first_shape_idx == 0){
            continue;
        }

        f32 min_x;
        f32 min_y;
        f32 max_x;
        f32 max_y;
        i32 shape_idx = first_shape_idx;
        while(true){

            BOUNDS_CHECK(shape_idx, node_length);
            IntrusiveListNode* shape_node = &node[shape_idx];

            BOUNDS_CHECK(shape_idx, bvh_leaf_padding_length);
            f32 padding = bvh_leaf_padding[body_idx];

            BOUNDS_CHECK(shape_idx, aabbs.length);
            min_x = aabbs.min_x[shape_idx] - padding;
            min_y = aabbs.min_y[shape_idx] - padding;
            max_x = aabbs.max_x[shape_idx] + padding;
            max_y = aabbs.max_y[shape_idx] + padding;

            { // insert into bvh.

                BOUNDS_CHECK(shape_idx, centroids.length);
                f32 cx = centroids.x[shape_idx];
                f32 cy = centroids.y[shape_idx];

                BOUNDS_CHECK(shape_idx, bvh_category_length);
                i32 category = bvh_category[shape_idx];

                i32 leaf_idx = bvh->leaves.count;
                soa_bvh_leaf_push(&bvh->leaves, min_x, min_y, max_x, max_y, cx, cy, category);

                BOUNDS_CHECK(leaf_idx, bvh_leaf_index_length);
                bvh_leaf_index[leaf_idx] = shape_idx;
            }

            shape_idx = shape_node->next_sibling;
            if(shape_idx == first_shape_idx){
                break;
            }
        }
    }

    // construct the bvh with the new data.
    bvh_construct_tree(bvh);
}

/**
    Transforms `in_use` bodies local-space vertices by their global-space transforms.

    `remarks`
    All arrays must be of the same length and elements should be vertivally accessible via `body_idx`.
**/
void fizx_state_transform_all_shape_vertices(FIZXState* state){
    ZERO_MEMORY(state->bodies.global_vertex.chunk_count, sizeof(*state->bodies.global_vertex.chunk_count) * state->bodies.global_vertex.chunk_count_length);
    i32 length = state->body_hierarchy.root_index_count;

    for(i32 i = 1; i < length; i++){ // start afer the nil.
        BOUNDS_CHECK(i, state->body_hierarchy.length);
        i32 body_idx = state->body_hierarchy.root_index[i];
        BOUNDS_CHECK(body_idx, state->body_hierarchy.length);
        i32 body_first_shape_idx = state->body_hierarchy.node[body_idx].first_child;

        if(body_first_shape_idx == 0){
            return;
        }

        if(i==2){
            i32 x = 0;
        }

        i32 shape_idx = body_first_shape_idx;
        while(true){
            soa_body_transform_shape_vertices(&state->bodies, shape_idx);

            BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
            shape_idx = state->body_hierarchy.node[shape_idx].next_sibling;
            if(shape_idx == body_first_shape_idx){
                break;
            }
        }
    }
}

void fizx_state_fixed_update(FIZXState* state, f32 delta_time, i32 sub_steps){

    f32* y = &state->bodies.global_transform.position.y[1];

    // scratch buffers for rigid body reslution.

    f32* rigid_collision_resolution_impulse_x_scratch_space         = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_impulse_y_scratch_space         = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_impulse_magnitude_scratch_space = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_contact_point_x_scratch_space   = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_contact_point_y_scratch_space   = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_owner_distance_x_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_owner_distance_y_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_other_distance_x_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_other_distance_y_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_owner_perpendicular_x_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_owner_perpendicular_y_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    f32* rigid_collision_resolution_other_perpendicular_x_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* rigid_collision_resolution_other_perpendicular_y_scratch_space  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

    // scale delta time by the substeps.
    delta_time /= (f32)sub_steps;

    { // prepare substep collisions.
        collision_manifold_prepare_for_next_step(&state->collision_manifold);
        i32 dynamic_count = state->polygon_collider_count.dynamic + state->circle_collider_count.dynamic + state->polygon_rigid_count.dynamic + state->circle_rigid_count.dynamic;
        i32 kinematic_count = state->polygon_collider_count.kinematic + state->circle_collider_count.kinematic + state->polygon_rigid_count.kinematic + state->circle_rigid_count.kinematic;

        // prepare sub step collision resolution collection.
        BOUNDS_CHECK(CollisionResolutionCategory_Dynamic, state->sub_step_shape_collisions_to_resolve.category_stride_length);
        state->sub_step_shape_collisions_to_resolve.category_stride[CollisionResolutionCategory_Dynamic] = dynamic_count;
        BOUNDS_CHECK(CollisionResolutionCategory_Kinematic, state->sub_step_shape_collisions_to_resolve.category_stride_length);
        state->sub_step_shape_collisions_to_resolve.category_stride[CollisionResolutionCategory_Kinematic] = kinematic_count;
        categorised_overlap_array_build_chunks(&state->sub_step_shape_collisions_to_resolve);

        BOUNDS_CHECK(CollisionResolutionCategory_Dynamic, state->sub_step_rigid_collisions_to_resolve.category_stride_length);
        state->sub_step_rigid_collisions_to_resolve.category_stride[CollisionResolutionCategory_Dynamic] = dynamic_count;
        BOUNDS_CHECK(CollisionResolutionCategory_Kinematic, state->sub_step_rigid_collisions_to_resolve.category_stride_length);
        state->sub_step_rigid_collisions_to_resolve.category_stride[CollisionResolutionCategory_Kinematic] = kinematic_count;
        categorised_overlap_array_build_chunks(&state->sub_step_rigid_collisions_to_resolve);
    }

    { // bvh.
        bodies_calculate_bvh_leaf_padding(
            state->bodies.global_transform.position.x, state->bodies.global_transform.position.length,
            state->bodies.global_transform.position.y, state->bodies.global_transform.position.length,
            state->bodies.previous_step_position.x, state->bodies.previous_step_position.length,
            state->bodies.previous_step_position.y, state->bodies.previous_step_position.length,
            state->bodies.bvh_leaf_padding, state->bodies.bvh_leaf_padding_length,
            state->body_hierarchy.root_index, state->body_hierarchy.length, state->body_hierarchy.root_index_count,
            delta_time
        );

        { // Update Overlap Scratch Buffer Category Stride.
            bvh_categorised_leaf_overlaps_clear(&state->overlaps_scratch_buffer);

            BOUNDS_CHECK(ShapeCategory_DynColCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_DynColCircle]  = state->circle_collider_count.dynamic;

            BOUNDS_CHECK(ShapeCategory_TriColCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_TriColCircle]  = state->circle_collider_count.trigger;

            BOUNDS_CHECK(ShapeCategory_KinColCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_KinColCircle]  = state->circle_collider_count.kinematic;

            BOUNDS_CHECK(ShapeCategory_DynRigCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_DynRigCircle]  = state->circle_rigid_count.dynamic;

            BOUNDS_CHECK(ShapeCategory_TriRigCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_TriRigCircle]  = state->circle_rigid_count.trigger;

            BOUNDS_CHECK(ShapeCategory_KinRigCircle, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_KinRigCircle]  = state->circle_rigid_count.kinematic;

            BOUNDS_CHECK(ShapeCategory_DynColPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_DynColPolygon] = state->polygon_collider_count.dynamic;

            BOUNDS_CHECK(ShapeCategory_TriColPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_TriColPolygon] = state->polygon_collider_count.trigger;

            BOUNDS_CHECK(ShapeCategory_KinColPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_KinColPolygon] = state->polygon_collider_count.kinematic;

            BOUNDS_CHECK(ShapeCategory_DynRigPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_DynRigPolygon] = state->polygon_rigid_count.dynamic;

            BOUNDS_CHECK(ShapeCategory_TriRigPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_TriRigPolygon] = state->polygon_rigid_count.trigger;

            BOUNDS_CHECK(ShapeCategory_KinRigPolygon, state->overlaps_scratch_buffer.category_stride_length);
            state->overlaps_scratch_buffer.category_stride[ShapeCategory_KinRigPolygon] = state->polygon_rigid_count.kinematic;

            bvh_categorised_leaf_overlaps_build_chunks(&state->overlaps_scratch_buffer);
        }

        // reconstruct bvh.
        fizx_bvh_construct_tree(
            &state->bvh,
            state->body_hierarchy.root_index, state->body_hierarchy.root_index_count, state->body_hierarchy.length,
            state->body_hierarchy.node, state->body_hierarchy.length,
            state->bodies.aabb, state->bodies.centroid,
            state->bodies.category, state->bodies.category_length,
            state->bodies.bvh_leaf_padding, state->bodies.bvh_leaf_padding_length,
            state->bodies.bvh_leaf_index, state->bodies.bvh_leaf_index_length
        );

        bvh_get_overlaps(state->bvh, &state->overlaps_scratch_buffer);
        fizx_bvh_categorised_leaf_overlaps_format(&state->overlaps_scratch_buffer, state->bodies.bvh_leaf_index, state->bodies.bvh_leaf_index_length, state->bodies.category, state->bodies.category_length);
    }

    /**
        set the new previous positions for this tick.
        note that ordering matters here; keep this below the bvh section always.
    **/
    ASSERT(state->bodies.previous_step_position.length == state->bodies.global_transform.position.length, "state body arrays not equal for memcopy.");
    COPY_MEMORY(state->bodies.previous_step_position.x, state->bodies.global_transform.position.x, sizeof(f32) * state->bodies.global_transform.position.length);
    COPY_MEMORY(state->bodies.previous_step_position.y, state->bodies.global_transform.position.y, sizeof(f32) * state->bodies.global_transform.position.length);

    /**
        retrieve overlap info.
    **/
    // dynamic polygon rigidbody.
    BvhOverlapInfo overlaps_dyn_rig_pol_to_dyn_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_DynRigPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_kin_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_KinRigPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_tri_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_TriRigPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_dyn_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_DynColPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_kin_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_KinColPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_dyn_rig_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigPolygon, ShapeCategory_TriColCircle);

    // kinematic polygon rigid body.
    BvhOverlapInfo overlaps_kin_rig_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_kin_rig_pol_to_kin_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_KinRigPolygon);
    BvhOverlapInfo overlaps_kin_rig_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_kin_rig_pol_to_tri_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_TriRigPolygon);
    BvhOverlapInfo overlaps_kin_rig_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_kin_rig_pol_to_dyn_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_DynColPolygon);
    BvhOverlapInfo overlaps_kin_rig_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_kin_rig_pol_to_kin_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_KinColPolygon);
    BvhOverlapInfo overlaps_kin_rig_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_kin_rig_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_kin_rig_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigPolygon, ShapeCategory_TriColCircle);

    // trigger polygon rigid body.
    BvhOverlapInfo overlaps_tri_rig_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_tri_rig_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_tri_rig_pol_to_tri_rig_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_TriRigPolygon);
    BvhOverlapInfo overlaps_tri_rig_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_tri_rig_pol_to_dyn_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_DynColPolygon);
    BvhOverlapInfo overlaps_tri_rig_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_tri_rig_pol_to_kin_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_KinColPolygon);
    BvhOverlapInfo overlaps_tri_rig_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_tri_rig_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_tri_rig_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigPolygon, ShapeCategory_TriColCircle);

    // dynamic polygon collider.
    BvhOverlapInfo overlaps_dyn_col_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_dyn_col_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_dyn_col_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_dyn_col_pol_to_dyn_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_DynColPolygon);
    BvhOverlapInfo overlaps_dyn_col_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_dyn_col_pol_to_kin_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_KinColPolygon);
    BvhOverlapInfo overlaps_dyn_col_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_dyn_col_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_dyn_col_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColPolygon, ShapeCategory_TriColCircle);

    // kinematic polygon collider.
    BvhOverlapInfo overlaps_kin_col_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_kin_col_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_kin_col_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_kin_col_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_kin_col_pol_to_kin_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_KinColPolygon);
    BvhOverlapInfo overlaps_kin_col_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_kin_col_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_kin_col_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColPolygon, ShapeCategory_TriColCircle);

    // trigger polygon collider.
    BvhOverlapInfo overlaps_tri_col_pol_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_tri_col_pol_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_tri_col_pol_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_tri_col_pol_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_tri_col_pol_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_tri_col_pol_to_tri_col_pol = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_TriColPolygon);
    BvhOverlapInfo overlaps_tri_col_pol_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColPolygon, ShapeCategory_TriColCircle);

    // dynamic circle rigid body.
    BvhOverlapInfo overlaps_dyn_rig_cir_to_dyn_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_DynRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_cir_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_cir_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_dyn_rig_cir_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_dyn_rig_cir_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_dyn_rig_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynRigCircle, ShapeCategory_TriColCircle);

    // kinematic circle rigid body.
    BvhOverlapInfo overlaps_kin_rig_cir_to_kin_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigCircle, ShapeCategory_KinRigCircle);
    BvhOverlapInfo overlaps_kin_rig_cir_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigCircle, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_kin_rig_cir_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigCircle, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_kin_rig_cir_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigCircle, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_kin_rig_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinRigCircle, ShapeCategory_TriColCircle);

    // trigger circle rigidbody.
    BvhOverlapInfo overlaps_tri_rig_cir_to_tri_rig_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigCircle, ShapeCategory_TriRigCircle);
    BvhOverlapInfo overlaps_tri_rig_cir_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigCircle, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_tri_rig_cir_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigCircle, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_tri_rig_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriRigCircle, ShapeCategory_TriColCircle);

    // dynamic circle collider.
    BvhOverlapInfo overlaps_dyn_col_cir_to_dyn_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColCircle, ShapeCategory_DynColCircle);
    BvhOverlapInfo overlaps_dyn_col_cir_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColCircle, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_dyn_col_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_DynColCircle, ShapeCategory_TriColCircle);

    // kinematic circle collider.
    BvhOverlapInfo overlaps_kin_col_cir_to_kin_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColCircle, ShapeCategory_KinColCircle);
    BvhOverlapInfo overlaps_kin_col_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_KinColCircle, ShapeCategory_TriColCircle);

    // trigger circle collider.
    BvhOverlapInfo overlaps_tri_col_cir_to_tri_col_cir = bvh_categorised_leaf_overlaps_get_overlaps(state->overlaps_scratch_buffer, ShapeCategory_TriColCircle, ShapeCategory_TriColCircle);

    for(i32 sub_step = 0; sub_step < sub_steps; sub_step++){
        // clear garbage collisions that were resolved last sub step.
        categorised_overlap_array_clear_counts(&state->sub_step_shape_collisions_to_resolve);
        categorised_overlap_array_clear_counts(&state->sub_step_rigid_collisions_to_resolve);

        /**
            Body Movement Step.
        **/
        {
            Vector2 gravity = vector2_mul_val(state->gravity_direction, state->gravity_force * delta_time);
            for(i32 i = 1; i < state->body_hierarchy.root_index_count; i++){ // skip the Nil.

                BOUNDS_CHECK(i, state->body_hierarchy.length);
                i32 body_index = state->body_hierarchy.root_index[i];

                BOUNDS_CHECK(body_index, state->bodies.global_transform.length);
                f32* body_pos_x     = &state->bodies.global_transform.position.x[body_index];
                f32* body_pos_y     = &state->bodies.global_transform.position.y[body_index];
                f32* body_sine      = &state->bodies.global_transform.sine[body_index];
                f32* body_cosine    = &state->bodies.global_transform.cosine[body_index];
                f32* body_rotation  = &state->bodies.global_transform.rotation[body_index];
                f32* body_scale_x   = &state->bodies.global_transform.scale.x[body_index];
                f32* body_scale_y   = &state->bodies.global_transform.scale.y[body_index];
                BOUNDS_CHECK(body_index, state->bodies.linear_velocity.length);
                f32* lin_vel_x      = &state->bodies.linear_velocity.x[body_index];
                f32* lin_vel_y      = &state->bodies.linear_velocity.y[body_index];
                BOUNDS_CHECK(body_index, state->bodies.mass_length);
                f32* mass           = &state->bodies.mass[body_index];
                BOUNDS_CHECK(body_index, state->bodies.gravity_affected_length);
                bool gravity_affected = state->bodies.gravity_affected[body_index];

                if(gravity_affected){
                    *lin_vel_x += gravity.x;
                    *lin_vel_y += gravity.y;
                }

                if(*mass > 0){
                    BOUNDS_CHECK(body_index, state->bodies.force.length);
                    f32 mass_factor = *mass * delta_time;
                    *lin_vel_x += state->bodies.force.x[body_index] / mass_factor;
                    *lin_vel_y += state->bodies.force.y[body_index] / mass_factor;
                }

                /**
                    rotate body around center of mass.
                **/
                {

                    // apply rotation.
                    BOUNDS_CHECK(body_index, state->bodies.angular_velocity_length);
                    f32 rot_amt = state->bodies.angular_velocity[body_index] * delta_time;
                    rotor_multiply(*body_sine, *body_cosine, rot_amt, body_sine, body_cosine);
                    *body_rotation = f32_atan2(*body_sine, *body_cosine);

                    // offset the body in relation to the center of mass.
                    f32 rot_cos = f32_cos(rot_amt);
                    f32 rot_sin = f32_sin(rot_amt);
                    BOUNDS_CHECK(body_index, state->bodies.local_center_of_mass.length);
                    f32 com_x = *body_pos_x + state->bodies.local_center_of_mass.x[body_index];
                    f32 com_y = *body_pos_y + state->bodies.local_center_of_mass.y[body_index];

                    // translate to origin.
                    f32 com_offset_x = *body_pos_x - com_x;
                    f32 com_offset_y = *body_pos_y - com_y;
                    f32 rotated_x = com_offset_x * rot_cos - com_offset_y * rot_sin;
                    f32 rotated_y = com_offset_x * rot_sin + com_offset_y * rot_cos;
                    *body_pos_x = rotated_x + com_x;
                    *body_pos_y = rotated_y + com_y;
                }

                /**
                    apply linear velocity translation.
                **/
                {
                    *body_pos_x += *lin_vel_x * delta_time;
                    *body_pos_y += *lin_vel_y * delta_time;
                }

                /**
                    apply displacements
                **/
                {
                    BOUNDS_CHECK(body_index, state->bodies.shape_collision_displacement.length);

                    f32* displacement_x = &state->bodies.shape_collision_displacement.x[body_index];
                    *body_pos_x += *displacement_x;
                    *displacement_x = 0;

                    f32* displacement_y = &state->bodies.shape_collision_displacement.y[body_index];
                    *body_pos_y += *displacement_y;
                    *displacement_y = 0;
                }

                /**
                    move and rotate body's shapes.
                **/
                {
                    BOUNDS_CHECK(body_index, state->body_hierarchy.length);
                    IntrusiveListNode* node = &state->body_hierarchy.node[body_index];
                    i32 body_first_shape_idx = node->first_child;
                    if(body_first_shape_idx != 0){
                        i32 shape_idx = body_first_shape_idx;
                        while(true){

                            BOUNDS_CHECK(shape_idx, state->bodies.global_transform.length);
                            BOUNDS_CHECK(shape_idx, state->bodies.local_transform.length);
                            transform2d_transform_scalar(

                                state->bodies.local_transform.position.x[shape_idx],    state->bodies.local_transform.position.y[shape_idx],
                                state->bodies.local_transform.scale.x[shape_idx],       state->bodies.local_transform.scale.y[shape_idx],
                                state->bodies.local_transform.sine[shape_idx],          state->bodies.local_transform.cosine[shape_idx],
                                state->bodies.local_transform.rotation[shape_idx],

                                *body_pos_x, *body_pos_y, *body_scale_x, *body_scale_y, *body_sine, *body_cosine, *body_rotation,

                                &state->bodies.global_transform.position.x[shape_idx],  &state->bodies.global_transform.position.y[shape_idx],
                                &state->bodies.global_transform.scale.x[shape_idx],     &state->bodies.global_transform.scale.y[shape_idx],
                                &state->bodies.global_transform.sine[shape_idx],        &state->bodies.global_transform.cosine[shape_idx],
                                &state->bodies.global_transform.rotation[shape_idx]
                            );

                            BOUNDS_CHECK(shape_idx, state->body_hierarchy.length);
                            shape_idx = state->body_hierarchy.node[shape_idx].next_sibling;
                            if(shape_idx == body_first_shape_idx){
                                break;
                            }
                        }
                    }
                }
            }
        }

        fizx_state_transform_all_shape_vertices(state);

        // find collisions.
        {
            // dyn_rig_pol_to_dyn_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_dyn_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_pol_to_kin_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_kin_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_pol_to_tri_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_tri_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // dyn_rig_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // dyn_rig_pol_to_dyn_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_dyn_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_pol_to_kin_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_kin_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // dyn_rig_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_rig_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_rig_pol_to_kin_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_kin_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_rig_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_rig_pol_to_tri_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_tri_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_rig_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_rig_pol_to_dyn_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_dyn_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_rig_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_rig_pol_to_kin_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_kin_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_rig_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_rig_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_rig_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_tri_rig_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_tri_rig_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // tri_rig_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_dyn_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_dyn_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // tri_rig_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_kin_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_kin_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // tri_rig_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_rig_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // tri_rig_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // dyn_col_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // dyn_col_pol_to_dyn_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_dyn_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_kin_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_kin_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // dyn_col_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_col_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_col_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_col_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_col_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_col_pol_to_kin_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_kin_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_col_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // kin_col_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // kin_col_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // tri_col_pol_to_tri_col_pol
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_tri_col_pol, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_POLY
                #define COLLISION_DETECTION_CONFIG_POLY_TO_POLY false
            }
            // tri_col_pol_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_pol_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_POLY_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_POLY_TO_CIRC false
            }
            // dyn_rig_cir_to_dyn_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_dyn_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_cir_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION false
            }
            // dyn_rig_cir_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // dyn_rig_cir_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_cir_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_rig_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_rig_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_rig_cir_to_kin_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_cir_to_kin_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_rig_cir_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_cir_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_rig_cir_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_cir_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
                #undef COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_BODY_IS_SOLE_DYNAMIC false
            }
            // kin_rig_cir_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_cir_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_rig_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_rig_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // tri_rig_cir_to_tri_rig_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_cir_to_tri_rig_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // tri_rig_cir_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_cir_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // tri_rig_cir_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_cir_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // tri_rig_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_rig_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // dyn_col_cir_to_dyn_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_cir_to_dyn_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_cir_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION true
                #undef  COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY CollisionResolutionCategory_Dynamic
                #undef  COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY
                #define COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY CollisionResolutionCategory_Kinematic

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_cir_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
                #undef  COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION
                #define COLLISION_DETECTION_CONFIG_RESOLVE_SHAPE_COLLISION false
            }
            // dyn_col_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_dyn_col_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_col_cir_to_kin_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_cir_to_kin_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // kin_col_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_kin_col_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
            // tri_col_cir_to_tri_col_cir
            {
                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC true

                COLLISION_DETECTION(&state->collision_manifold, overlaps_tri_col_cir_to_tri_col_cir, state->bodies, state->body_hierarchy, &state->sub_step_shape_collisions_to_resolve, &state->sub_step_rigid_collisions_to_resolve);

                #undef  COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC
                #define COLLISION_DETECTION_CONFIG_CIRC_TO_CIRC false
            }
        }

        /**
            resolve shape collisions.
        **/
        // ordering is important here, shape collision resolution should always come before rigid resolution.
        {
            f32 depth;
            f32 displacement_x;
            f32 displacement_y;
            // always the dynamic shape.
            i32 owner_idx;
            // always the ohe kinematic or dynamic shape.
            i32 other_idx;

            /**
                resolve dynamic to dynamic collisions.
            **/
            char* collisions_to_resolve_char_ptr;
            i32 collisions_to_resolve_length;
            STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE* collisions_to_resolve;

            categorised_overlap_get_overlaps(
                state->sub_step_shape_collisions_to_resolve,
                CollisionResolutionCategory_Dynamic,
                CollisionResolutionCategory_Dynamic,
                sizeof(STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE),
                &collisions_to_resolve_char_ptr,
                &collisions_to_resolve_length
            );
            collisions_to_resolve = (STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE*)collisions_to_resolve_char_ptr;

            for(i32 i = 0; i < collisions_to_resolve_length; i++){
                STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE collision_idx = collisions_to_resolve[i];
                owner_idx = collision_idx / state->collision_manifold.collider_stride; // i32 div truncates the remainder, always giving the owner index.
                other_idx = collision_idx % state->collision_manifold.collider_stride;

                BOUNDS_CHECK(collision_idx, state->collision_manifold.depth_length);
                depth = state->collision_manifold.depth[collision_idx];
                BOUNDS_CHECK(collision_idx, state->collision_manifold.normal.length);
                displacement_x = state->collision_manifold.normal.x[collision_idx] * depth * 0.5f;
                displacement_y = state->collision_manifold.normal.y[collision_idx] * depth * 0.5f;

                BOUNDS_CHECK(owner_idx, state->body_hierarchy.length);
                IntrusiveListNode* owner_node = &state->body_hierarchy.node[owner_idx];
                BOUNDS_CHECK(other_idx, state->body_hierarchy.length);
                IntrusiveListNode* other_node = &state->body_hierarchy.node[other_idx];

                BOUNDS_CHECK(owner_node->parent, state->bodies.shape_collision_displacement.length);
                state->bodies.shape_collision_displacement.x[other_node->parent] -= displacement_x;
                state->bodies.shape_collision_displacement.y[other_node->parent] -= displacement_y;
                state->bodies.shape_collision_displacement.x[owner_node->parent] += displacement_x;
                state->bodies.shape_collision_displacement.y[owner_node->parent] += displacement_y;
            }

            /**
                resolve dynamic to kinematic collision.
            **/

            categorised_overlap_get_overlaps(
                state->sub_step_shape_collisions_to_resolve,
                CollisionResolutionCategory_Dynamic,
                CollisionResolutionCategory_Kinematic,
                sizeof(STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE),
                &collisions_to_resolve_char_ptr,
                &collisions_to_resolve_length
            );
            collisions_to_resolve = (STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE*)collisions_to_resolve_char_ptr;

            for(i32 i = 0; i < collisions_to_resolve_length; i++){
                STATE_SUB_STEP_SHAPE_COLLISIONS_TO_RESOLVE_TYPE collision_idx = collisions_to_resolve[i];
                owner_idx = collision_idx / state->collision_manifold.collider_stride; // i32 div truncates the remainder, always giving the owner index.

                BOUNDS_CHECK(collision_idx, state->collision_manifold.depth_length);
                depth = state->collision_manifold.depth[collision_idx];
                displacement_x = state->collision_manifold.normal.x[collision_idx] * depth;
                displacement_y = state->collision_manifold.normal.y[collision_idx] * depth;

                BOUNDS_CHECK(owner_idx, state->body_hierarchy.length);
                IntrusiveListNode* owner_node = &state->body_hierarchy.node[owner_idx];

                BOUNDS_CHECK(owner_node->parent, state->bodies.shape_collision_displacement.length);
                state->bodies.shape_collision_displacement.x[owner_node->parent] += displacement_x;
                state->bodies.shape_collision_displacement.y[owner_node->parent] += displacement_y;
            }
        }

        /**
            resolve rigid collisions
        **/
        {
            bool other_shape_is_kinematic;
            char* collision_to_resolve_char_ptr;
            i32* collision_to_resolve;

            other_shape_is_kinematic = true;
            i32 collision_to_resolve_length;
            categorised_overlap_get_overlaps(
                state->sub_step_rigid_collisions_to_resolve,
                CollisionResolutionCategory_Dynamic, CollisionResolutionCategory_Kinematic,
                sizeof(STATE_SUB_STEP_RIGID_COLLISIONS_TO_RESOLVE_TYPE), &collision_to_resolve_char_ptr, &collision_to_resolve_length
            );
            collision_to_resolve = (i32*)collision_to_resolve_char_ptr;
            fizx_resolve_rigid_collisions(
                state->collision_manifold, state->body_hierarchy, state->bodies, collision_to_resolve, collision_to_resolve_length,
                rigid_collision_resolution_impulse_x_scratch_space,
                rigid_collision_resolution_impulse_y_scratch_space,
                rigid_collision_resolution_impulse_magnitude_scratch_space,
                rigid_collision_resolution_contact_point_x_scratch_space,
                rigid_collision_resolution_contact_point_y_scratch_space,
                rigid_collision_resolution_owner_distance_x_scratch_space,
                rigid_collision_resolution_owner_distance_y_scratch_space,
                rigid_collision_resolution_other_distance_x_scratch_space,
                rigid_collision_resolution_other_distance_y_scratch_space,
                rigid_collision_resolution_owner_perpendicular_x_scratch_space,
                rigid_collision_resolution_owner_perpendicular_y_scratch_space,
                rigid_collision_resolution_other_perpendicular_x_scratch_space,
                rigid_collision_resolution_other_perpendicular_y_scratch_space,
                other_shape_is_kinematic
            );

            other_shape_is_kinematic = false;
            categorised_overlap_get_overlaps(
                state->sub_step_rigid_collisions_to_resolve,
                CollisionResolutionCategory_Dynamic, CollisionResolutionCategory_Dynamic,
                sizeof(STATE_SUB_STEP_RIGID_COLLISIONS_TO_RESOLVE_TYPE), &collision_to_resolve_char_ptr, &collision_to_resolve_length
            );
            collision_to_resolve = (i32*)collision_to_resolve_char_ptr;
            fizx_resolve_rigid_collisions(
                state->collision_manifold, state->body_hierarchy, state->bodies, collision_to_resolve, collision_to_resolve_length,
                rigid_collision_resolution_impulse_x_scratch_space,
                rigid_collision_resolution_impulse_y_scratch_space,
                rigid_collision_resolution_impulse_magnitude_scratch_space,
                rigid_collision_resolution_contact_point_x_scratch_space,
                rigid_collision_resolution_contact_point_y_scratch_space,
                rigid_collision_resolution_owner_distance_x_scratch_space,
                rigid_collision_resolution_owner_distance_y_scratch_space,
                rigid_collision_resolution_other_distance_x_scratch_space,
                rigid_collision_resolution_other_distance_y_scratch_space,
                rigid_collision_resolution_owner_perpendicular_x_scratch_space,
                rigid_collision_resolution_owner_perpendicular_y_scratch_space,
                rigid_collision_resolution_other_perpendicular_x_scratch_space,
                rigid_collision_resolution_other_perpendicular_y_scratch_space,
                other_shape_is_kinematic
            );
        }

    }

    collision_manifold_complete_step(&state->collision_manifold);
    /**
        Transform bodies by collision resolution.

        NOTE: this is needed at the end as the final
        sub-step iteration does not Transform2D the bodies
        at the end of it's loop; meaning the final collision
        resolution wouldn't be applied.
    **/
    fizx_state_transform_all_shape_vertices(state);
}