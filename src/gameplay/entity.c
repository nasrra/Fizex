#define ENTITY_NAME_LENGTH 32 

typedef enum{
    EntityTypeId_None = 0,
    EntityTypeId_LevelRoot = 1,
    EntityTypeId_RedBird = 2,
    EntityTypeId_YellowBird = 3,
    EntityTypeId_WoodBlock = 4,
    EntityTypeId_InvisibleWall = 5
} EntityTypeId;

typedef struct{
    String name;
    Transform2D transform;
    GFX_SpriteId sprite_id;
    f32 sprite_depth;
    GenId physics_body_gid;
    Aabb clickable_aabb;
    EntityTypeId type_id;
    i32 health;
    bool is_health;
    bool is_clickable;
    bool is_physics_body;
} Entity;

typedef struct{
    // the transform to spawn the entity at.
    Transform2D spawn_transform;
    /*
        the gid of the spawned entity.
        
        `remarks`
        this field should be populated with valid data after the entity has been allocated in the entity manager.
    */
    GenId entity_gid;
    // the id of entity to spawn's type. 
    i32 entity_type_id;
    // the index of the entity's parent in the deserialised entity list. 
    i32 parent_deserialised_entity_idx;
} DeserialisedEntity;

typedef struct{
    Entity* entity;
    i32 entity_length;
    DeserialisedEntity* deserialised_entity;
    i32 deserialised_entity_length;
    i32 deserialised_entity_count;
    GenIdAllocator gen_id_allocator;
    FIZX_State fizx_state;
    GFX_State* gfx_state;
    IntrusiveList entity_hierarchy;
    bool is_init;
} EntityManager;


///
/// Physics layers.
///

#define PHYSICS_LAYER_ALL I32_MAX
#define PHYSICS_LAYER_PLAYER (1 << 1)
#define PHYSICS_LAYER_ENEMY (1 << 2)
#define PHYSICS_LAYER_ENVIRONMENT (1 << 3)

///
/// Virtual Texture ID.
///
#define VIRTUAL_TEXTURE_ID_RED_BIRD 3
#define VIRTUAL_TEXTURE_ID_SLING_SHOT 4
#define VIRTUAL_TEXTURE_ID_YELLOW_BIRD 5
#define VIRTUAL_TEXTURE_ID_WOOD_BLOCK 6

GenId entity_manager_alloc_entity(EntityManager* manager, GenId parent){
    GenId gid = gen_id_allocator_alloc(&manager->gen_id_allocator);
    if(gid == 0){
        return gid;
    }
    
    i32 idx = gen_id_get_index(gid);
    
    if(parent == 0){
        if(!intrusive_list_add_root(&manager->entity_hierarchy, idx)){
            ASSERT(false, "failed to insert entity into entity hierarchy.");
            gen_id_allocator_dealloc(&manager->gen_id_allocator, gid);
            gid = 0;
        }
    }
    else{
        i32 parent_idx = gen_id_get_index(parent);
        if(!intrusive_list_add_branch(&manager->entity_hierarchy, idx, parent_idx)){
            ASSERT(false, "failed to insert entity into entity hierarchy.");
            gen_id_allocator_dealloc(&manager->gen_id_allocator, gid);
            gid = 0;
        }
    }
    
    return gid;
}

void entity_manager_dealloc_entity_data_unsafe(EntityManager* manager, i32 idx){
    BOUNDS_CHECK(idx, manager->entity_length);
    Entity* entity = &manager->entity[idx];

    if(entity->physics_body_gid != 0){
        fizx_body_dealloc(&manager->fizx_state, entity->physics_body_gid);
    }
    
    if(!gfx_sprite_id_equals(entity->sprite_id, (GFX_SpriteId){0})){
        gfx_dealloc_sprite(manager->gfx_state, entity->sprite_id);
    }

    *entity = (Entity){0};
}

void entity_on_entity_hierarchy_dealloc(IntrusiveList* list, i32 node_idx, void* user_data){
    EntityManager* manager = (EntityManager*)user_data;
    entity_manager_dealloc_entity_data_unsafe(manager, node_idx);
}

bool entity_manager_dealloc_entity(EntityManager* manager, GenId gid){
    if(!gen_id_allocator_dealloc(&manager->gen_id_allocator, gid)){
        return false;
    }
    i32 idx = gen_id_get_index(gid);

    // TODO:
    // may want to add functionality later so the user can deallocate an entity independently from its children.
    
    // NOTE:
    // the on_dealloc callback for this calls entity_manager_dealloc_entity_data_unsafe.
    if(!intrusive_list_remove_node_and_children(&manager->entity_hierarchy, idx, manager)){
        ASSERT(false, "failed to remove entity from entity hierarchy.");
    }
    return true;
}

bool entity_manager_get_entity(EntityManager manager, GenId entity_gid, Entity** out_entity){
    if(gen_id_allocator_is_gen_id_invalid(&manager.gen_id_allocator, entity_gid)){
        return false;
    }
    i32 idx = gen_id_get_index(entity_gid);
    BOUNDS_CHECK(idx, manager.entity_length);
    *out_entity = &manager.entity[idx];
    return true;
}

void entity_manager_debug_draw(EntityManager manager, GFX_State* gfx, f32 delta_time){
#if 0
    for(i32 i = 0; i < manager.entity_length; i++){
        Entity* entity = &manager.entity[i];
        if(entity->is_clickable){
            Rectangle shape = {
                .x = entity->transform.position.x  + entity->clickable_aabb.min_x,
                .y = entity->transform.position.y  + entity->clickable_aabb.max_y,
                .width = entity->clickable_aabb.max_x - entity->clickable_aabb.min_x,
                .height = entity->clickable_aabb.max_y - entity->clickable_aabb.min_y
            };
            gfx_draw_wire_rect(gfx, shape , GFX_COLOUR_WHITE, 0.0f, SPRITE_LAYER_WORLD, SPRITE_MATERIAL_DEBUG);
        }
    }
#endif
}

void entity_deplete_health(EntityManager* manager, GenId entity_gid, i32 amount){
    Entity* entity;
    if(!entity_manager_get_entity(*manager, entity_gid, &entity)){
        ASSERT(false, "failed to get entity.");
    }

    ASSERT(entity->is_health, "entity doesnt use health.");
    entity->health -= amount;
    if(entity->health <= 0){
        entity_manager_dealloc_entity(manager, entity_gid);
    }
}

GenId entity_spawn_red_bird(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);
         
        entity->type_id = EntityTypeId_RedBird; 
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 2.0f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);

        entity->is_clickable = true;
        entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};

        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.bot_right = {.x = 150, .y = 150}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_RED_BIRD, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}

GenId entity_spawn_yellow_bird(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    // clickable entity (angry bird).
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        entity->type_id = EntityTypeId_YellowBird; 
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 1.0f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        fizx_body_set_active(&entity_manager->fizx_state, entity->physics_body_gid, false);

        entity->is_clickable = true;
        entity->clickable_aabb = (Aabb) {.min_x = -0.75f, .min_y = -0.75f, .max_x = 0.75f, .max_y = 0.75f};

        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.bot_right = {.x = 150, .y = 150}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_YELLOW_BIRD, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}

GenId entity_spawn_wood_block(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        string_clear(&entity->name);
        string_push(&entity->name, name);

        entity->type_id = EntityTypeId_WoodBlock;
        entity->transform = transform;
        entity->is_physics_body = true;

        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 0.75f, .kinetic_friction = 0.5f, .density = 22.6f, .restitution = 0.0f};

        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, entity->transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(&entity_manager->fizx_state, entity->physics_body_gid, shape_transform, FIZX_ShapeBehaviour_Dynamic, &entity_gid, PHYSICS_LAYER_PLAYER, square, material, true);
        
        Transform2D sprite_transform = TRANSFORM2D_IDENTITY;
        // sprite_transform.scale = vector2_mul_val(sprite_transform.scale, 1000.0f);
        entity->sprite_depth = 1.0f;
        bool success = false;
        entity->sprite_id = gfx_sprite_alloc(gfx_ctx, SPRITE_LAYER_WORLD, &success);
        GFX_SpriteRegion region = {.bot_right = {.x = 150, .y = 150}};
        gfx_sprite_init(
            gfx_ctx, entity->sprite_id, sprite_transform, GFX_COLOUR_WHITE, region, GFX_ColourState_Tint,
            GFX_SpriteOrigin_Center, VIRTUAL_TEXTURE_ID_WOOD_BLOCK, SPRITE_MATERIAL_IMAGE, entity->sprite_depth, true
        );
    }
    return entity_gid;
}


GenId entity_spawn_invisible_wall(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {        
        string_clear(&entity->name);
        string_push(&entity->name, name);
        
        entity->type_id = EntityTypeId_InvisibleWall;
        
        Rectangle square = {.x = -0.5f, .y = 0.5f, .width = 1.0f, .height = 1.0f};
        FIZX_Material material = {.static_friction = 1.0f, .kinetic_friction = 1.0f, .density = 22.6f, .restitution = 0.0f};
        Transform2D shape_transform = TRANSFORM2D_IDENTITY;
        entity->is_physics_body = true;
        entity->physics_body_gid = fizx_body_alloc(&entity_manager->fizx_state, transform, false);
        GenId entity_shape_gid = fizx_rectangle_rigid_alloc(
            &entity_manager->fizx_state, 
            entity->physics_body_gid, 
            shape_transform, 
            FIZX_ShapeBehaviour_Kinematic, 
            &entity_gid, 
            PHYSICS_LAYER_ENVIRONMENT, 
            square, 
            material, 
            true
        );
    }
    return entity_gid;
}

GenId entity_spawn_level_root(EntityManager* entity_manager, GFX_State* gfx_ctx, String name, Transform2D transform, GenId parent){
    GenId entity_gid = entity_manager_alloc_entity(entity_manager, parent);
    Entity* entity;
    entity_manager_get_entity(*entity_manager, entity_gid, &entity);
    {
        entity->type_id = EntityTypeId_LevelRoot;
        string_push(&entity->name, name); 
    }
    return entity_gid;
}

void load_lvl(EntityManager* entity_manager, GFX_State* gfx, String file_path, GenId parent_entity){
    /*
        .scsv are .csv files that are separated with ';' instead of ','

        a level file is as follows:

        entity_id ; entity_position_x ; entity_position_y ; entity_scale_x ; entity_scale_y ; entity_rotation_radians ;
    */

    size_t buffer_size;
    void* raw_file = platform_load_file(file_path, &buffer_size);
    char* file_data = (char*)raw_file;

    DeserialisedEntity* deserialised_entity;
    i32 lines_read = 1;
    i32 bytes_consumed = 0;

    entity_manager->deserialised_entity_count = 1;

    while(true){
        BOUNDS_CHECK(entity_manager->deserialised_entity_count, entity_manager->deserialised_entity_length);
        deserialised_entity = &entity_manager->deserialised_entity[entity_manager->deserialised_entity_count];
    
        // NOTE:
        // this can corrupt the stack, as sscanf doesnt bounds check at all for name strings.
        // be very careful here, if the program nukes itself when loadiing saved data, this is probs why.
        // have fun champ :)
        String name = {.chars =  (char[ENTITY_NAME_LENGTH]){0}, .length = ENTITY_NAME_LENGTH};
        deserialised_entity->spawn_transform = TRANSFORM2D_IDENTITY;
        lines_read = sscanf(
            file_data,
            "%[^;]%n;%i;%i;%f;%f;%f;%f;%f;%n",
            name.chars,
            &name.count,
            &deserialised_entity->entity_type_id,
            &deserialised_entity->parent_deserialised_entity_idx,
            &deserialised_entity->spawn_transform.position.x,
            &deserialised_entity->spawn_transform.position.y,
            &deserialised_entity->spawn_transform.scale.x,
            &deserialised_entity->spawn_transform.scale.y,
            &deserialised_entity->spawn_transform.rotation,
            &bytes_consumed
        );

        entity_manager->deserialised_entity_count += 1;
        if(entity_manager->deserialised_entity_count > entity_manager->deserialised_entity_length){
            ASSERT(false, "cannot store all required deserialised entities from level file.");
            break;
        }

        // deserialsied entities should be in the order (from 0 to length) parent->child.
        BOUNDS_CHECK(deserialised_entity->parent_deserialised_entity_idx, entity_manager->deserialised_entity_count);
        // GenId parent_gid = deserialised_entity->parent_deserialised_entity_idx <= 0
        // ? parent_entity
        // : 

        if(lines_read == 8){
            switch(deserialised_entity->entity_type_id){
                case EntityTypeId_RedBird:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_red_bird(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_entity);
                }break;
                case EntityTypeId_YellowBird:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_yellow_bird(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_entity);
                }break;
                case EntityTypeId_WoodBlock:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_wood_block(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_entity);
                }break;
                case EntityTypeId_InvisibleWall:{
                    deserialised_entity->entity_gid 
                        = entity_spawn_invisible_wall(entity_manager, gfx, name, deserialised_entity->spawn_transform, parent_entity);
                }break;
            }
            file_data += bytes_consumed;
        }
        else{
            break;
        }        
    }
    platform_free_memory(raw_file);
}

void entity_manager_init(
    EntityManager* manager, MemoryArena* arena, GFX_State* gfx_state, 
    i32 entity_amount, i32 physics_body_amount
){
    ASSERT(!manager->is_init, "already init.");
    
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->entity, &manager->entity_length, entity_amount);
    MEMORY_ARENA_ALLOC_ARRAY(arena, manager->deserialised_entity, &manager->deserialised_entity_length, entity_amount);
    manager->deserialised_entity_count = 0;
    
    for(i32 i = 0; i < entity_amount; i++){
        Entity* entity = &manager->entity[i];
        string_init(&entity->name, arena, ENTITY_NAME_LENGTH);
    }
    
    gen_id_allocator_init(&manager->gen_id_allocator, arena, physics_body_amount);
    intrusive_list_init(&manager->entity_hierarchy, arena, entity_amount, false);
    manager->entity_hierarchy.on_dealloc_callback = entity_on_entity_hierarchy_dealloc;
    manager->gfx_state = gfx_state;
    fizx_state_init(&manager->fizx_state, arena, entity_amount, 4, sizeof(GenId));
    manager->is_init = true;
}
