
typedef struct{
    i32 clicked_entity_idx;
} PlayerMouseState;

PlayerMouseState player_mouse_state;

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
                player_mouse_state.clicked_entity_idx = e_idx;
            }
        }
    }
    
    if(input_is_mouse_button_just_released(MOUSE_BUTTON_LEFT)){
        if(player_mouse_state.clicked_entity_idx > 0){
            Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
            fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, true);
            player_mouse_state.clicked_entity_idx = 0;
        }
    }
    
    if(player_mouse_state.clicked_entity_idx){
        
        Entity* entity = &entity_manager->entity[player_mouse_state.clicked_entity_idx];
                
        entity->transform.position.x = mouse_world_position.x;
        entity->transform.position.y = mouse_world_position.y;
        fizx_body_set_global_position(&entity_manager->fizx_state, entity->physics_body_gid, mouse_world_position);
    }
}