
typedef struct{
    i32 clicked_entity_idx;
    Vector3 clicked_entity_initial_position;
} PlayerMouseState;

PlayerMouseState player_mouse_state;

#define PLAYER_MOUSE_MAX_DRAW_RADIUS 2.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.5f

void player_update(EntityManager* entity_manager, Vector2 mouse_world_position, f32 delta_time){
    if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
        for(i32 e_idx = 0; e_idx < entity_manager->entity_length; e_idx++){
            Entity* entity = &entity_manager->entity[e_idx];
            Aabb world_aabb = aabb_translate(entity->clickable_aabb, vector3_to_vector2(entity->transform.position));
            if(aabb_overlaps_point(world_aabb, mouse_world_position)){
                platform_output_message("clicked entity\n");
                if(entity->is_physics_body){
                    fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);
                }
                player_mouse_state.clicked_entity_initial_position = entity->transform.position;
                player_mouse_state.clicked_entity_idx = e_idx;
            }
        }
    }
    
    Vector3 impulse_magnitude;
    
    if(player_mouse_state.clicked_entity_idx){
        Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
        Vector3 position_diff = {
            .x = player_mouse_state.clicked_entity_initial_position.x - mouse_world_position.x,
            .y = player_mouse_state.clicked_entity_initial_position.y - mouse_world_position.y,
            .z = player_mouse_state.clicked_entity_initial_position.z
        };
        position_diff = vector3_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
        impulse_magnitude = position_diff; 
        Vector2 new_position = {
            .x  = player_mouse_state.clicked_entity_initial_position.x - position_diff.x, 
            .y = player_mouse_state.clicked_entity_initial_position.y - position_diff.y
        };
        
        entity->transform.position.x = new_position.x;
        entity->transform.position.y = new_position.y;
        fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, new_position);
    }
    
    if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
        if(player_mouse_state.clicked_entity_idx > 0){
            Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
            player_mouse_state.clicked_entity_idx = 0;
            fizx_body_clear_forces_and_velocities(&entity_manager->fizx_state, entity->physics_body_gid);
            impulse_magnitude = vector3_mul_val(impulse_magnitude, PLAYER_MOUSE_LAUNCH_FORCE);
            fizx_body_impulse_force(&entity_manager->fizx_state, (Vector2){.x = impulse_magnitude.x, .y = impulse_magnitude.y}, entity->physics_body_gid);
        }
    }
    
}
