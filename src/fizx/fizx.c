/**====================

    types.
====================**//**/


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
       Elements are accessed via `entity_index`    
    **/
    Soa_Transform2D local_transform;
    /**
        The global-space transforms for all entities.
        
        `remarks`
        Elements are accessed via `entity_index`    
    **/
    Soa_Transform2D global_transform;
    /**
        The positions of entities from the previous step.

        `remarks`
        Elements are accessed via `entity_index`    
    **/
    Soa_Vector2 previous_step_position;
    /**
        The force values that will be applied in to rigidbodies.
        `remarks`
       Elements are accessed via `entity_index`.    
    **/
    Soa_Vector2 force;
    /**
        The linear velocity values for all rigidbodies.    
    **/
    Soa_Vector2 linear_velocity;
    /**
        The centroids of all shapes; in global-space.

        `remarks`
        Elements are accessed via `entity_index`.
    **/
    Soa_Vector2 centroid;
    /**
        The center of masses of all bodies; relative to their global position.
        
        `remarks`
       Elements are accessed via `entity_index`    
    **/
    Soa_Vector2 local_center_of_mass;
    /**
        The Axis Aligned Bounding Boxes of all shapes.
       
        `remarks`
        Elements are accessed via `entity_index`.
    **/
    Soa_Aabb aabb;
    /**
        The physics materials for all shapes.
        
        `remarks`
        Elements are accessed via `entity_index`.    
    **/
    Soa_Material material;
    /**
        the angular velocities for all rigidbodies.

        `remarks`
        Elements are accessed via `entity_index`.    
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
        Elements are accessed via `entity_index`.    
    **/
    f32* base_width;
    i32 base_width_length;
    /**
        The base height values of all rectangle shapes.

        `remarks`
        Elements are accessed via `entity_index`.    
    **/
    f32* base_height;
    i32 base_height_length;
    /**
        The base radii values of all circle shapes.
        
        `remarks`
        Elements are accessed via `entity_index`.        
    **/
    f32* base_radius;
    i32 base_radius_length;
    /**    
        The global-space radii values of all circle shapes.
        
        `remarks`
        Elements are accessed via `entity_index`.
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
        Elements are accessed via `entity_index`.    
    **/
    f32* inverse_rotational_inertia;
    i32 inverse_rotational_inertia_length;
    /**
        The generations of all bodies.
        
        `remarks`
        Elements are accessed via `entity_index`.    
    **/
    i32* generation;
    i32 generation_length;
    /**
        The categories of all shapes.
        
        `remarks`
        Elements are accessed via `entity_index`.
    **/
    i32* category;
    i32 category_length;

    /**
        The bvh indices of all shapes.
        
        `remarks`
        Elements are accessed via `entity_index`    
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
       Elements are accessed via `entity_index`.    
    **/
    ShapeType* shape_type;
    i32 shape_type_length;
    /**
        Whether a rigidbody uses rotational response.

        `remarks`
        Elements are accessed via `entity_index`.    
    **/
    bool* rotational_response;
    i32 rotational_response_length;
    /**
        The types of all entities.
        
        `remarks`
        Elements are accessed via `entity_index`.    
    **/
    EntityType* entity_type;
    i32 entity_type_length;
    /**
        Whether or not an entity is active.
        
        `remarks`
        Elements are accessed via `entity_index`.
    **/
    bool* active;
    i32 active_length;
    /**
        Whether or not a body is gravity affected.

        `remarks`
        Elements are accessed via `entity_index`.
    **/
    bool* gravity_affected;
    i32 gravity_affected_length;
    /**
        All bodies shape collision displacement vectors. 
        
        `remarks`
        Elements are accessed via `entity_index`.    
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
    i32* active_index;
    i32 active_index_length;
    i32* active_index_chunk_counts;
    i32 active_index_chunk_counts_length;
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
    ContactState* contact_states;
    i32 contact_states_length;
    /**
        The state of all collisions in the previous step.
    **/
    ContactState* previous_contact_states;
    i32 previous_contact_states_length;
    /**
        The fixed stride of each entry.
    **/
    i32 collider_stride;
    /**
        The amount of entries this collection can hold.
    **/
    i32 length;
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
    bool draw_body_global_positions;
    bool draw_body_shapes;
    bool draw_body_aabbs;
    bool draw_bvh_branches;
    bool draw_collision_info;
    bool draw_linear_velocities;
    bool draw_centroids_unrotated;
    bool draw_leaves;
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

// add a macro check here to flip owner and other indexes.

#define COLLISION_DETECTION(manifold, info, bodies, body_hiearachy, shape_collisions_to_resolve, rigid_collisions_to_resolve) do {          \
    for(i32 COLLISION_DETECTION_i = 0; COLLISION_DETECTION_i < info.length; COLLISION_DETECTION_i++){                                       \
        i32 COLLISION_DETECTION_owner_leaf_idx = info.owner_leaf_index[COLLISION_DETECTION_i];                                              \
        i32 COLLISION_DETECTION_other_leaf_idx = info.other_leaf_index[COLLISION_DETECTION_i];                                              \
        BOUNDS_CHECK(COLLISION_DETECTION_owner_leaf_idx, bodies.bvh_leaf_index_length);                                                     \
        i32 COLLISION_DETECTION_owner_bvh_idx = bodies.bvh_leaf_index[COLLISION_DETECTION_owner_leaf_idx];                                  \
        i32 COLLISION_DETECTION_other_bvh_idx = bodies.bvh_leaf_index[COLLISION_DETECTION_other_leaf_idx];                                  \
                                                                                                                                            \
        bool COLLISION_DETECTION_broad_phase = collision_detection_broad_phase(                                                             \
            body_hiearachy, bodies.aabb, COLLISION_DETECTION_owner_bvh_idx, COLLISION_DETECTION_other_bvh_idx                               \
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
            categorised_overlaps_array_push(                                                                                                \
                shape_collisions_to_resolve,                                                                                                \
                &COLLISION_DETECTION_idx_pair.a_to_b, sizeof(COLLISION_DETECTION_idx_pair.a_to_b),                                          \
                COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY, COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY                  \
            );                                                                                                                              \
        }                                                                                                                                   \
        if(COLLISION_DETECTION_CONFIG_RESOLVE_RIGID_COLLISION){                                                                             \
            categorised_overlaps_array_push(                                                                                                \
                rigid_collisions_to_resolve,                                                                                                \
                &COLLISION_DETECTION_idx_pair.a_to_b, sizeof(COLLISION_DETECTION_idx_pair.a_to_b),                                          \
                COLLISION_DETECTION_CONFIG_OWNER_RESOLUTION_CATEGORY, COLLISION_DETECTION_CONFIG_OTHER_RESOLUTION_CATEGORY                  \
            );                                                                                                                              \
        }                                                                                                                                   \
    }                                                                                                                                       \
} while(0)

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
    functions: CollisionManifold
====================**//**/




void collision_manifold_init(CollisionManifold* manifold, MemoryArena* arena, i32 total_colliders){
    ASSERT(!manifold->is_init, "already init");
    ASSERT(total_colliders <= MAX_COLLIDERS, "total colliders exceeds max supported colliders.");
    total_colliders = CLAMP(total_colliders, 0, MAX_COLLIDERS);
    manifold->collider_stride = total_colliders;
    manifold->length = total_colliders;
    i32 data_length = manifold->collider_stride * manifold->length;
    soa_vector2_init(&manifold->normal, arena, data_length);
    soa_vector2_init(&manifold->collider_centroid, arena, data_length);
    soa_vector2_init(&manifold->first_contact_point, arena, data_length);
    soa_vector2_init(&manifold->second_contact_point, arena, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->depth, &manifold->depth_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->two_contact_points, &manifold->two_contact_points_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->contact_states, &manifold->contact_states_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->previous_contact_states, &manifold->previous_contact_states_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->active_phase, &manifold->active_phase_length, data_length);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manifold->active_index, &manifold->active_index_length, data_length);
    manifold->is_init = true;
}

void collision_manifold_swap_contact_state_contexts(CollisionManifold* manifold){
    ContactState* swap_data = manifold->previous_contact_states;
    i32 swap_length = manifold->previous_contact_states_length;

    manifold->previous_contact_states = manifold->contact_states;
    manifold->previous_contact_states_length = manifold->contact_states_length;

    manifold->contact_states = swap_data;
    manifold->contact_states_length = swap_length;
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
    BOUNDS_CHECK(idx, manifold->active_phase_length);
    i32* phase = &manifold->active_phase[idx];
    if(*phase <= 0){
        FIXED_STRIDE_ARRAY_PUSH(
            manifold->active_index, manifold->active_index_length, 
            manifold->active_index_chunk_counts, manifold->active_index_chunk_counts_length, 
            manifold->collider_stride, recipient_idx, idx
        );
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
            polygon_get_min_max_vertices(v_x, v_y, v_length, &soa->aabb.min_x[shape_idx], &soa->aabb.min_y[shape_idx], &soa->aabb.max_x[shape_idx], &soa->aabb.max_x[shape_idx]);
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
    functions: Body.
====================**//**/




inline void body_set_active_unsafe(FIZXState* state, i32 body_idx, bool is_active){
    BOUNDS_CHECK(body_idx, state->bodies.active_length);
    state->bodies.active[body_idx] = is_active;
}

bool body_set_active(FIZXState* state, GenId body_id, bool is_active){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_id)){
        return false;
    }
    body_set_active_unsafe(state, gen_id_get_index(body_id), is_active);
    return true;
}

bool body_is_active_unsafe(FIZXState* state, i32 body_idx){
    BOUNDS_CHECK(body_idx, state->bodies.active_length);
    return state->bodies.active[body_idx];
}

bool body_is_active(FIZXState* state, GenId body_id){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_id)){
        return false;
    }
    return body_is_active_unsafe(state, gen_id_get_index(body_id));
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

bool body_set_local_transform(FIZXState* state, GenId body_id, Transform2D transform){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_id)){
        return false;
    }
    body_set_local_transform_unsafe(state, gen_id_get_index(body_id), transform);
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

bool body_set_global_transform(FIZXState* state, GenId body_id, Transform2D transform){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_id)){
        return false;
    }
    body_set_global_transform_unsafe(state, gen_id_get_index(body_id), transform);
    return true;
}

Vector2 body_get_linear_velocity_unsafe(FIZXState* state, i32 body_idx){
    Soa_Vector2* soa = &state->bodies.linear_velocity;
    BOUNDS_CHECK(body_idx, soa->length);
    return (Vector2){.x = soa->x[body_idx], .y = soa->y[body_idx]};
}

Vector2 body_get_linear_velocity(FIZXState* state, GenId body_id){
    if(gen_id_allocator_is_gen_id_invalid(&state->gen_id_allocator, body_id)){
        return (Vector2){0};
    }
    i32 index = gen_id_get_index(body_id);
    if(!body_is_active_unsafe(state, index)){
        return (Vector2){0};
    }
    return body_get_linear_velocity_unsafe(state, index);

}

void body_translate(FIZXState* state, f32 displacement_x, f32 displacement_y, i32 body_idx){
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
    f32* out_bvh_leaf_padding, i32 out_bvh_leaf_padding_length,
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

        BOUNDS_CHECK(idx, out_bvh_leaf_padding_length);
        out_bvh_leaf_padding[idx] = delta_movement * time_factor;
    }
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

    return aabb_overlaps_scalar(
        aabb.min_x[a_shape_idx], aabb.min_x[b_shape_idx], 
        aabb.min_y[a_shape_idx], aabb.min_y[b_shape_idx], 
        aabb.max_x[a_shape_idx], aabb.max_x[b_shape_idx], 
        aabb.max_y[a_shape_idx], aabb.max_y[b_shape_idx]
    );
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
    categorised_overlap_array_init(&state->sub_step_shape_collisions_to_resolve, arena, CollisionResolutionCategory_Count, max_collisions, sizeof(i32));
    categorised_overlap_array_init(&state->sub_step_rigid_collisions_to_resolve, arena, CollisionResolutionCategory_Count, max_collisions, sizeof(i32));
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
        i32 body_index = active_body[i];
        
        BOUNDS_CHECK(body_index, node_length);
        IntrusiveListNode* body_node = &node[body_index];
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
            f32 padding = bvh_leaf_padding[shape_idx];

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
                
                soa_bvh_leaf_push(&bvh->leaves, min_x, min_y, max_x, max_y, cx, cy, category);
                i32 leaf_idx = bvh->leaves.count;
                
                BOUNDS_CHECK(leaf_idx, bvh_leaf_index_length);
                bvh_leaf_index[leaf_idx] = shape_idx;
            }

            shape_idx = node->next_sibling;
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

    // scratch buffers for rigid body reslution.
    f32* impulse_magnitude  = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* contact_point_x    = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* contact_point_y    = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* impulse_x          = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* impulse_y          = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* dist_a_x           = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* dist_a_y           = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* dist_b_x           = (f32[COLLISION_MAX_CONTACT_POINTS]){0};
    f32* dist_b_y           = (f32[COLLISION_MAX_CONTACT_POINTS]){0};

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
        categorised_overlaps_array_build_chunks(&state->sub_step_shape_collisions_to_resolve);

        BOUNDS_CHECK(CollisionResolutionCategory_Dynamic, state->sub_step_rigid_collisions_to_resolve.category_stride_length);    
        state->sub_step_rigid_collisions_to_resolve.category_stride[CollisionResolutionCategory_Dynamic] = dynamic_count;
        BOUNDS_CHECK(CollisionResolutionCategory_Kinematic, state->sub_step_rigid_collisions_to_resolve.category_stride_length);    
        state->sub_step_rigid_collisions_to_resolve.category_stride[CollisionResolutionCategory_Kinematic] = kinematic_count;
        categorised_overlaps_array_build_chunks(&state->sub_step_rigid_collisions_to_resolve);
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
        categorised_overlaps_array_clear_counts(&state->sub_step_shape_collisions_to_resolve);
        categorised_overlaps_array_clear_counts(&state->sub_step_rigid_collisions_to_resolve);

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
                    *lin_vel_x += state->bodies.force.x[body_index];
                    *lin_vel_y += state->bodies.force.y[body_index];
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
        {
            // f32 depth;
            // f32 displacement_x;
            // f32 displacement_y;
            // i32 owner_idx;
            // i32 other_idx;
        
            /** 
        {
            // hoisting invariance.
            float depth;
            float displacementX;
            float displacementY;
            int ownerIndex; // always the solid collider.
            int otherIndex; // always the kinematic or other solid collider.

            System.Span<int> collisionsToResolve;

            // == resolve solid to solid collisions ==.
            collisionsToResolve = Collections.GetOverlaps(subStepCollisionsToResolve,
                Collisions.ResolutionCategory.Dynamic,
                Collisions.ResolutionCategory.Dynamic
            );

            for(int i = 0; i < collisionsToResolve.Length; i++)
            {
                int collisionIndex = collisionsToResolve[i];
                ownerIndex = collisionIndex / collisionsStride; // int div truncates the remainder, always giving the owner index.
                otherIndex = collisionIndex % collisionsStride;

                depth = collisionDepths[collisionIndex];
                displacementX = collisionNormalsX[collisionIndex] * depth * 0.5f;
                displacementY = collisionNormalsY[collisionIndex] * depth * 0.5f;

                ref IntrusiveListNode ownerNode = ref nodes[ownerIndex]; 
                ref IntrusiveListNode otherNode = ref nodes[otherIndex]; 
                
                // apply the displacement to the bodies of the shape.            
                displacementsX[otherNode.Parent] -= displacementX;
                displacementsY[otherNode.Parent] -= displacementY;
                displacementsX[ownerNode.Parent] += displacementX;
                displacementsY[ownerNode.Parent] += displacementY; 
            }

            // == resolve solid to kinematic collisions ==.

            collisionsToResolve = Collections.GetOverlaps(subStepCollisionsToResolve,
                Collisions.ResolutionCategory.Dynamic,
                Collisions.ResolutionCategory.Kinematic
            );

            for(int i = 0; i < collisionsToResolve.Length; i++)
            {            
                int collisionIndex = collisionsToResolve[i];
                ownerIndex = collisionIndex / collisionsStride; // int div truncates the remainder, always giving the owner index.
                depth = collisionDepths[collisionIndex];
                displacementX = collisionNormalsX[collisionIndex] * depth;
                displacementY = collisionNormalsY[collisionIndex] * depth;

                ref IntrusiveListNode ownerNode = ref nodes[ownerIndex]; 

                // apply the displacement to the body of the shape.
                displacementsX[ownerNode.Parent] += displacementX;
                displacementsY[ownerNode.Parent] += displacementY; 
            }
        }

            **/ 
        }
    }
}

/**            
            // Resolve Collider Collisions.
            // NOTE: ordering matters here, make sure to this is above rigidbody collision resolution.
            long startColliderResolutionTime = Howl.Time.GetSystemTick();
            ResolveColliderCollisions(nodes, shapeCollisionsToResolve, collisionDepths, collisionNormalsX, collisionNormalsY, 
                ref collisionDisplacementsX, ref collisionDisplacementsY, collisionsStride
            );
            long endColliderResolutionTime = Howl.Time.GetSystemTick();
            state.ColliderResolutionStepInMs = Howl.Time.ElapsedMilliseconds(startColliderResolutionTime, endColliderResolutionTime);

            // Resolve RigidBody Collisions.
            // NOTE: ordering matters here, make sure this is below collision resolution.
            long startRigidResolutionTime = Howl.Time.GetSystemTick();
            ResolveRigidShapeCollisions(rigidShapeCollisionsToResolve, nodes,
                collisionNormalsX, collisionNormalsY, collisionFirstContactPointsX, collisionFirstContactPointsY,
                globalPositionsX, globalPositionsY, localCentersOfMassX, localCentersOfMassY, 
                collisionSecondContactPointsX, collisionSecondContactPointsY, ref linearVelocitiesX, ref linearVelocitiesY, 
                restitutions, kineticFrictions, staticFrictions, ref angularVelocities, masses, inverseMasses, inverseRotationalInertia, 
                collisionTwoContactPoints, rotationalResponses, contactPointsX, contactPointsY, distsAX, distsAY, 
                distsBX, distsBY, impulseMagnitudes, impulsesX, impulsesY, 
                collisionsStride
            );
            long endRigidResolutionTime = Howl.Time.GetSystemTick();
            state.RigidResolutionStepInMs = Howl.Time.ElapsedMilliseconds(startRigidResolutionTime, endRigidResolutionTime);

            long endSubStepTime = Howl.Time.GetSystemTick();
            state.SubStepTimeInMs = Howl.Time.ElapsedMilliseconds(startSubStepTime, endSubStepTime);
        }

        Collisions.Manifold.CompleteStep(ref state.CollisionManifold);

        // Transform2D bodies by collision resolution.
        // NOTE: this is needed at the end as the final
        // sub-step iteration does not Transform2D the bodies
        // at the end of it's loop; meaning the final collision
        // resolution wouldn't be applied.
        TransformAllShapesVertices(activeBodies, nodes, ref globalVertices, localVertices, shapes, ref globalScalesX, ref globalScalesY, 
            ref globalPositionsX, ref globalPositionsY, ref globalSines, ref globalCosines, minAabbsX, minAabbsY, maxAabbsX, maxAabbsY, 
            ref centroidsX, ref centroidsY, baseRadii, ref globalRadii
        );

        long endStepTime = Howl.Time.GetSystemTick();
        state.StepTimeInMs = Howl.Time.ElapsedMilliseconds(startStepTime, endStepTime);
    }
**/