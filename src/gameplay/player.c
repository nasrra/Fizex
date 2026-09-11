
typedef struct{
    i32 clicked_entity_idx;
    Vector2 clicked_entity_initial_position;
} PlayerMouseState;

PlayerMouseState player_mouse_state;

#define PLAYER_MOUSE_MAX_DRAW_RADIUS 2.5f
#define PLAYER_MOUSE_LAUNCH_FORCE 7.5f

void player_update(EntityManager* entity_manager, Vector2 mouse_world_position, f32 delta_time){
    if(input_is_mouse_button_just_pressed(MOUSE_BUTTON_LEFT)){
        for(i32 e_idx = 0; e_idx < entity_manager->entity_length; e_idx++){
            Entity* entity = &entity_manager->entity[e_idx];
            Aabb world_aabb = aabb_translate(entity->clickable_aabb, entity->transform.position);
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
    
    Vector2 impulse_magnitude;
    
    if(player_mouse_state.clicked_entity_idx){
        Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
        Vector2 position_diff = vector2_sub(player_mouse_state.clicked_entity_initial_position, mouse_world_position);
        
        position_diff = vector2_clamp_to_radius(position_diff, PLAYER_MOUSE_MAX_DRAW_RADIUS);
        impulse_magnitude = vector2_mul_val(position_diff, PLAYER_MOUSE_LAUNCH_FORCE); 
        
        Vector2 new_position = vector2_sub(player_mouse_state.clicked_entity_initial_position, position_diff);
            
        entity->transform.position = new_position;
        fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, new_position);
    }
    
    if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
        if(player_mouse_state.clicked_entity_idx > 0){
            Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
            player_mouse_state.clicked_entity_idx = 0;
            fizx_body_clear_forces_and_velocities(&entity_manager->fizx_state, entity->physics_body_gid);

            fizx_body_impulse_force(&entity_manager->fizx_state,impulse_magnitude, entity->physics_body_gid);
        }
    }
    
}
