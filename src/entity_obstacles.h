#ifndef ENTITY_OBSTACLES_H
#define ENTITY_OBSTACLES_H


#define ENTITY_COUNT_OBSTACLES_TOTAL 4u
#define ENTITY_COUNT_OBSTACLES_MAX   (ENTITY_COUNT_OBSTACLES_TOTAL - 1u)
#define ENTITY_COUNT_OBSTACLES_WRAP  (ENTITY_COUNT_OBSTACLES_TOTAL)

// TODO: make spawn count min a function of obstacle speed so it's always correct
// #define OBSTACLE_NEXT_COUNT_MIN     45u // for speed ~128u  // min frames between spawning
        #define OBSTACLE_NEXT_COUNT_MIN     42u // for speed ~255u   // min frames between spawning
#define OBSTACLE_NEXT_COUNT_BITMASK 0x3Fu  // bitmask to select range + min
// #define OBSTACLE_NEXT_COUNT_MIN     0x1Fu  // min frames between spawning
// #define OBSTACLE_NEXT_COUNT_BITMASK 0x7Fu  // bitmask to select range + min

#define OBSTACLE_NEXT_COUNT_DOUBLE     10u // TODO: adjust for movement speed

#define OBSTACLE_SPAWN_Y      (0u << 8) // TODO: fixme
#define OBSTACLE_SPAWN_STATE  0u  // TODO: fixme

// #define OBSTACLE_NEXT_TYPE_BITMASK 0x03u  // 4 types of obstacles to spawn

#define OBSTACLE_TYPE_MASK    0x03u  // Should exactly cover range of values used below
#define OBSTACLE_TYPE_LEFT    0x00u
#define OBSTACLE_TYPE_RIGHT   0x01u
#define OBSTACLE_TYPE_MIDDLE  0x02u
#define OBSTACLE_TYPE_FULL    0x03u

#define OBJECTS_FLAG_DOUBLE_BIT  0x04u
#define OBJECTS_FLAG_BOBBING_BIT 0x08u


// TODO: try to match speed to innermost canyon section
// #define OBSTACLE_INC_SPEED    0x128u  // 1/2 pixel per frame  // TODO make variable?
        #define OBSTACLE_INC_SPEED    0x255u  // 1/2 pixel per frame  // TODO make variable?
#define OBSTACLE_Y_REMOVE     144u    // Remove obstacles after they exceed this threshold


// obstacle_x & obstacle_y are in upper left of metasprite hit box (0, 8) of metasprite
// #define OBSTACLE_HITBOX_X_LEFT   ((sprite_obstacles_WIDTH - sprite_obstacles_PIVOT_W) / 2u)
// #define OBSTACLE_HITBOX_X_RIGHT  ((sprite_obstacles_PIVOT_W) + ((sprite_obstacles_WIDTH - sprite_obstacles_PIVOT_W) / 2u))
#define OBSTACLE_HITBOX_X_ST__TYPE_LEFT   0u
#define OBSTACLE_HITBOX_X_END_TYPE_LEFT   ((sprite_obstacles_WIDTH / 4u) * 1u)

#define OBSTACLE_HITBOX_X_ST__TYPE_RIGHT  ((sprite_obstacles_WIDTH / 4u) * 3u)
#define OBSTACLE_HITBOX_X_END_TYPE_RIGHT  ((sprite_obstacles_WIDTH / 4u) * 4u)

#define OBSTACLE_HITBOX_X_ST__TYPE_MIDDLE ((sprite_obstacles_WIDTH / 4u) * 1u)
#define OBSTACLE_HITBOX_X_END_TYPE_MIDDLE ((sprite_obstacles_WIDTH / 4u) * 3u)

#define OBSTACLE_HITBOX_X_ST__TYPE_FULL   ((sprite_obstacles_WIDTH / 4u) * 0u)
#define OBSTACLE_HITBOX_X_END_TYPE_FULL   ((sprite_obstacles_WIDTH / 4u) * 4u)



#define OBSTACLE_HITBOX_Y_TOP    ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u)
#define OBSTACLE_HITBOX_Y_BOTTOM ((sprite_obstacles_PIVOT_H) + ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u))


typedef struct obs_ent {
    fixed   y;
    uint8_t type;  // Should this be "Type" instead? how to deal with "bobbing" ?
} obs_ent;

extern obs_ent obstacles[];
extern uint8_t obstacles_active_last;
extern uint8_t obstacles_active_first;
extern uint8_t obstacles_active_count;
extern uint8_t obstacles_spawn_countdown;


extern const uint8_t obstacles_x_hitbox_left[];
extern const uint8_t obstacles_x_hitbox_right[];

void entity_obstacles_init(void);

uint8_t entity_obstacles_update(uint8_t);


#endif