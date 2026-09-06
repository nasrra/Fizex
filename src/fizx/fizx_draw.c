typedef struct{
    Camera camera;
    Colour colour_dynamic_shape;
    Colour colour_passive_trigger_shape;
    Colour colour_kinematic_shape;
    Colour colour_active_trigger_shape;
    Colour colour_aabb;
    Colour colour_fallback_shape;
    Colour colour_inactive_physics_body;
    Colour colour_bvh_leaf;
    Colour colour_bvh_branch;
    Colour colour_contact_point;
    Colour colour_linear_velocity;
    Colour colour_global_position;
    Colour colour_centroid;
    Colour colour_collision_other;
    Colour colour_collision_normal;
    Colour colour_center_of_mass;
    i32 sprite_layer;
    f32 z_position;
    f32 wireframe_thickness;
    i32 material_idx;
    bool draw_body_global_positions;
    bool draw_body_shapes;
    bool draw_body_aabbs;
    bool draw_bvh_branches;
    bool draw_bvh_leaves;
    bool draw_collision_info;
    bool draw_linear_velocities;
    bool draw_centers_of_mass_unrotated;
} FIZXDrawInfo;

void fizx_state_draw(FIZXState state, RendererContext* renderer, FIZXDrawInfo info, f32 delta_time){
    /**
        draw global positions.
    **/
    if(info.draw_body_global_positions){
        for(i32 i = 1; i < state.body_hierarchy.length; i++){
            i32 body_idx = state.body_hierarchy.root_index[i];
            IntrusiveListNode* body_node = &state.body_hierarchy.node[body_idx];
            i32 first_shape_idx = body_node->first_child;

            if(first_shape_idx == 0){
                continue;
            }

            i32 shape_idx = first_shape_idx;

            while(true){
                BOUNDS_CHECK(shape_idx, state.bodies.global_transform.length);
                Circle shape = {
                    .x = state.bodies.global_transform.position.x[shape_idx],
                    .y = state.bodies.global_transform.position.y[shape_idx],
                    .radius = 0.1f
                };

                renderer_draw_wire_circle(renderer, shape, info.colour_global_position, info.z_position, info.sprite_layer, info.material_idx);

                BOUNDS_CHECK(shape_idx, state.body_hierarchy.length);
                shape_idx = state.body_hierarchy.node[shape_idx].next_sibling;
                if(shape_idx == first_shape_idx){
                    break;
                }
            }
        }
    }

    /**
        draw shapes.
    **/
    if(info.draw_body_shapes){
        Colour colour;
        f32* poly_vert_x;
        f32* poly_vert_y;
        i32 poly_vert_length;

        for(i32 i = 1; i < state.body_hierarchy.length; i++){
            i32 body_idx = state.body_hierarchy.root_index[i];
            IntrusiveListNode* body_node = &state.body_hierarchy.node[body_idx];
            i32 first_shape_idx = body_node->first_child;
            if(first_shape_idx == 0){
                continue;
            }

            i32 shape_idx = first_shape_idx;
            while(true){
                BOUNDS_CHECK(shape_idx, state.bodies.category_length);
                i32 category = state.bodies.category[shape_idx];

                // determine shape behaviour.
                if(shape_category_is_dynamic(category)){
                    colour = info.colour_dynamic_shape;
                }
                else if(shape_category_is_kinematic(category)){
                    colour = info.colour_kinematic_shape;
                }
                else if(shape_category_is_trigger(category)){
                    colour = collision_manifold_shape_has_collisions(state.collision_manifold, shape_idx)
                    ? info.colour_active_trigger_shape
                    : info.colour_passive_trigger_shape;
                }
                else{
                    ASSERT(false, "unknown shape category (behaviour).");
                }

                // draw in accordance with the shape.
                if(shape_category_is_polygon(category)){
                    soa_body_get_vertices_unsafe(state.bodies.global_vertex, shape_idx, &poly_vert_x, &poly_vert_y, &poly_vert_length);
                    renderer_draw_wire_poly(renderer, poly_vert_x, poly_vert_y, poly_vert_length, colour, info.z_position, info.sprite_layer, info.material_idx);
                }
                else if(shape_category_is_circle(category)){
                    BOUNDS_CHECK(shape_idx, state.bodies.centroid.length);
                    BOUNDS_CHECK(shape_idx, state.bodies.global_radius_length);
                    Circle shape = {
                        .x = state.bodies.centroid.x[shape_idx],
                        .y = state.bodies.centroid.y[shape_idx],
                        .radius = state.bodies.global_radius[shape_idx]
                    };
                    renderer_draw_wire_circle(renderer, shape, colour, info.z_position, info.sprite_layer, info.material_idx);
                }
                else{
                    ASSERT(false, "unknown shape category (shape type)");
                }

                BOUNDS_CHECK(shape_idx, state.body_hierarchy.length);
                shape_idx = state.body_hierarchy.node[shape_idx].next_sibling;
                if(shape_idx == first_shape_idx){
                    break;
                }
            }
        }
    }

    /**
        draw centroids un-rotated.
    **/
    if(info.draw_centers_of_mass_unrotated){
        i32 count = state.body_hierarchy.root_index_count;
        for(i32 i = 1; i < count; i++){
            i32 body_idx = state.body_hierarchy.root_index[i];

            BOUNDS_CHECK(body_idx, state.body_hierarchy.length);
            IntrusiveListNode* body_node = &state.body_hierarchy.node[body_idx];

            i32 first_shape_idx = body_node->first_child;
            if(first_shape_idx == 0){
                continue;
            }

            i32 shape_idx = first_shape_idx;
            while(true){
                BOUNDS_CHECK(shape_idx, state.bodies.centroid.length);
                Circle shape = {
                    .x = state.bodies.centroid.x[shape_idx],
                    .y = state.bodies.centroid.y[shape_idx],
                    .radius = 0.1f
                };

                renderer_draw_wire_circle(renderer, shape, info.colour_centroid, info.z_position, info.sprite_layer, info.material_idx);

                BOUNDS_CHECK(shape_idx, state.body_hierarchy.length);
                shape_idx = state.body_hierarchy.node[shape_idx].next_sibling;

                if(shape_idx == first_shape_idx){
                    break;
                }
            }
        }
    }

    if(info.draw_linear_velocities){

        /**
            TODO: (nich s)
            check that this works, the C# code was commented out for this one so it might just crash lol.
        **/
        i32 count = state.body_hierarchy.root_index_count;
        for(i32 i = 1; i < count; i++){

            i32 body_idx = state.body_hierarchy.root_index[i];

            BOUNDS_CHECK(body_idx, state.bodies.global_transform.length);
            f32 start_x = state.bodies.global_transform.position.x[body_idx];
            f32 start_y = state.bodies.global_transform.position.y[body_idx];
            BOUNDS_CHECK(body_idx, state.bodies.linear_velocity.length);
            f32 end_x = start_x + state.bodies.linear_velocity.x[body_idx];
            f32 end_y = start_y + state.bodies.linear_velocity.y[body_idx];

            Vector3 start = {.x = start_x, .y = start_y, .z = info.z_position};
            Vector3 end = {.x = end_x, .y = end_y, .z = info.z_position};
            renderer_draw_line(renderer, info.colour_linear_velocity, start, end, info.sprite_layer, info.material_idx, renderer_global_wireframe_thickness);
        }
    }

    /**
        draw aabbs.
    **/
    if(info.draw_body_aabbs){
        for(i32 i = 1; i < state.body_hierarchy.root_index_count; i++){
            i32 body_idx = state.body_hierarchy.root_index[i];

            BOUNDS_CHECK(body_idx, state.body_hierarchy.length);
            IntrusiveListNode* body_node = &state.body_hierarchy.node[body_idx];
            i32 first_shape_idx = body_node->first_child;
            if(first_shape_idx == 0){
                continue;
            }

            i32 shape_idx = first_shape_idx;
            while(true){
                i32 vert_length = 4;
                BOUNDS_CHECK(shape_idx, state.bodies.aabb.length);
                f32* x = (f32[4]){};
                x[0] = state.bodies.aabb.min_x[shape_idx];
                x[1] = state.bodies.aabb.max_x[shape_idx];
                x[2] = x[1];
                x[3] = x[0];
                f32* y = (f32[4]){};
                y[0] = state.bodies.aabb.max_y[shape_idx];
                y[1] = y[0];
                y[2] = state.bodies.aabb.min_y[shape_idx];
                y[3] = y[2];

                renderer_draw_wire_poly(renderer, x, y, 4, info.colour_aabb, info.z_position, info.sprite_layer, info.material_idx);

                BOUNDS_CHECK(shape_idx, state.body_hierarchy.length);
                shape_idx = state.body_hierarchy.node[body_idx].next_sibling;
                if(shape_idx == first_shape_idx){
                    break;
                }
            }
        }
    }

    /**
        draw bvh branches.
    **/
    if(info.draw_bvh_branches){
        for(i32 i = 0; i < state.bvh.branches.count; i++){
            Rectangle shape = {
                .x = state.bvh.branches.aabb.min_x[i],
                .y = state.bvh.branches.aabb.max_y[i],
                .width = state.bvh.branches.aabb.max_x[i] - state.bvh.branches.aabb.min_x[i],
                .height = state.bvh.branches.aabb.max_y[i] - state.bvh.branches.aabb.min_y[i]
            };
            renderer_draw_wire_rect(renderer, shape, info.colour_bvh_branch, info.z_position, info.sprite_layer, info.material_idx);
        }
    }

    /**
        draw bvh leaves.
    **/
    if(info.draw_bvh_leaves){
        for(i32 i = 0; i < state.bvh.leaves.count; i++){
            Rectangle shape = {
                .x = state.bvh.leaves.aabb.min_x[i],
                .y = state.bvh.leaves.aabb.max_y[i],
                .width = state.bvh.leaves.aabb.max_x[i] - state.bvh.leaves.aabb.min_x[i],
                .height = state.bvh.leaves.aabb.max_y[i] - state.bvh.leaves.aabb.min_y[i]
            };
            renderer_draw_wire_rect(renderer, shape, info.colour_bvh_leaf, info.z_position, info.sprite_layer, info.material_idx);
        }
    }

    /**
        draw collision information.
    **/
    if(info.draw_collision_info){
        f32 contact_point_x;
        f32 contact_point_y;
        f32 normal_x;
        f32 normal_y;
        f32 other_centroid_x;
        f32 other_centroid_y;
        Vector3 normal_start;
        Vector3 normal_end;
        Circle shape;

        for(i32 i = 0; i < state.collision_manifold.active_index.chunk_count_length; i++){
            i32 count = state.collision_manifold.active_index.chunk_count[i];
            if(count <= 0){
                continue;
            }

            i32 chunk_element_idx = fixed_stride_array_get_element_idx(i, state.collision_manifold.collider_stride, 0);
            BOUNDS_CHECK(chunk_element_idx, state.collision_manifold.active_index.data_length);
            i32 collision_idx = state.collision_manifold.active_index.data[chunk_element_idx];

            i32 owner_idx = collision_idx / state.collision_manifold.collider_stride; // int div truncates the remainder, always giving the owner index.
            i32 other_idx = collision_idx % state.collision_manifold.collider_stride;

            // avoid duplicates.
            if(owner_idx > other_idx){
                continue;
            }

            // get normal data.
            BOUNDS_CHECK(collision_idx, state.collision_manifold.normal.length);
            normal_x = state.collision_manifold.normal.x[collision_idx];
            normal_y = state.collision_manifold.normal.y[collision_idx];

            // get contact point 1 data.
                BOUNDS_CHECK(collision_idx, state.collision_manifold.first_contact_point.length);
            contact_point_x = state.collision_manifold.first_contact_point.x[collision_idx];
            contact_point_y = state.collision_manifold.first_contact_point.y[collision_idx];

            // get centroid data.
            BOUNDS_CHECK(collision_idx, state.collision_manifold.collider_centroid.length);
            other_centroid_x = state.collision_manifold.collider_centroid.x[collision_idx];
            other_centroid_y = state.collision_manifold.collider_centroid.y[collision_idx];

            // draw centroid.
            shape.x = other_centroid_x;
            shape.y = other_centroid_y;
            shape.radius = 0.1f;
            renderer_draw_wire_circle(renderer, shape, info.colour_collision_other, info.z_position, info.sprite_layer, info.material_idx);

            // draw contact point.
            shape.x = contact_point_x;
            shape.y = contact_point_y;
            shape.radius = 0.1f;
            renderer_draw_wire_circle(renderer, shape, info.colour_collision_other, info.z_position, info.sprite_layer, info.material_idx);

            // draw normal from contact point.
            normal_start.x = contact_point_x;
            normal_start.y = contact_point_y;
            normal_start.z = info.z_position;
            normal_end.x = normal_start.x + normal_start.x;
            normal_end.y = normal_start.y + normal_start.y;
            normal_end.z = normal_start.z;
            renderer_draw_line(renderer, info.colour_collision_normal, normal_start, normal_end, info.sprite_layer, info.material_idx, renderer_global_wireframe_thickness);

            // draw second contact point if there is one.
            BOUNDS_CHECK(collision_idx, state.collision_manifold.two_contact_points_length);
            if(state.collision_manifold.two_contact_points[collision_idx]){
            // get contact point 1 data.
                BOUNDS_CHECK(collision_idx, state.collision_manifold.second_contact_point.length);
                contact_point_x = state.collision_manifold.second_contact_point.x[collision_idx];
                contact_point_y = state.collision_manifold.second_contact_point.y[collision_idx];

                // draw contact point.
                shape.x = contact_point_x;
                shape.y = contact_point_y;
                shape.radius = 0.1f;
                renderer_draw_wire_circle(renderer, shape, info.colour_collision_other, info.z_position, info.sprite_layer, info.material_idx);

                // draw normal from contact point.
                normal_start.x = contact_point_x;
                normal_start.y = contact_point_y;
                normal_start.z = info.z_position;
                normal_end.x = normal_start.x + normal_start.x;
                normal_end.y = normal_start.y + normal_start.y;
                normal_end.z = normal_start.z;
                renderer_draw_line(renderer, info.colour_collision_normal, normal_start, normal_end, info.sprite_layer, info.material_idx, renderer_global_wireframe_thickness);
            }
        }
    }

    if(info.draw_centers_of_mass_unrotated){
        for(i32 i = 1; i < state.body_hierarchy.root_index_count; i++){
            i32 body_idx = state.body_hierarchy.root_index[i];

            BOUNDS_CHECK(body_idx, state.bodies.global_transform.length);
            BOUNDS_CHECK(body_idx, state.bodies.local_center_of_mass.length);
            Circle shape = {
                .x = state.bodies.global_transform.position.x[body_idx] + state.bodies.local_center_of_mass.x[body_idx],
                .y = state.bodies.global_transform.position.y[body_idx] + state.bodies.local_center_of_mass.y[body_idx],
                .radius = 0.1f
            };

            renderer_draw_wire_circle(renderer, shape, info.colour_center_of_mass, info.z_position, info.sprite_layer, info.material_idx);
        }
    }
}