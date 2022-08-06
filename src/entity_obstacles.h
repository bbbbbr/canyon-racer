#ifndef ENTITY_OBSTACLES_H
#define ENTITY_OBSTACLES_H


#define ENTITY_COUNT_OBSTACLES_TOTAL 4u
#define ENTITY_COUNT_OBSTACLES_MAX   (ENTITY_COUNT_OBSTACLES_TOTAL - 1u)
#define ENTITY_COUNT_OBSTACLES_WRAP  (ENTITY_COUNT_OBSTACLES_TOTAL)

// TODO: make spawn count min a function of obstacle speed so it's always correct
// #define OBJECTS_SPAWN_COUNT_MIN     45u // for speed ~128u  // min frames between spawning
        #define OBJECTS_SPAWN_COUNT_MIN     42u // for speed ~255u   // min frames between spawning
#define OBJECTS_SPAWN_COUNT_BITMASK 0x3Fu  // bitmask to select range + min
// #define OBJECTS_SPAWN_COUNT_MIN     0x1Fu  // min frames between spawning
// #define OBJECTS_SPAWN_COUNT_BITMASK 0x7Fu  // bitmask to select range + min

#define OBSTACLE_SPAWN_Y      (0u << 8) // TODO: fixme
#define OBSTACLE_SPAWN_STATE  0u  // TODO: fixme

// TODO: try to match speed to innermost canyon section
// #define OBSTACLE_INC_SPEED    0x128u  // 1/2 pixel per frame  // TODO make variable?
        #define OBSTACLE_INC_SPEED    0x255u  // 1/2 pixel per frame  // TODO make variable?
#define OBSTACLE_Y_REMOVE     144u    // Remove obstacles after they exceed this threshold


// obstacle_x & obstacle_y are in upper left of metasprite hit box (0, 8) of metasprite
#define OBSTACLE_HITBOX_X_LEFT   ((sprite_obstacles_WIDTH - sprite_obstacles_PIVOT_W) / 2u)
#define OBSTACLE_HITBOX_X_RIGHT  ((sprite_obstacles_PIVOT_W) + ((sprite_obstacles_WIDTH - sprite_obstacles_PIVOT_W) / 2u))

#define OBSTACLE_HITBOX_Y_TOP    ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u)
#define OBSTACLE_HITBOX_Y_BOTTOM ((sprite_obstacles_PIVOT_H) + ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u))


typedef struct obs_ent {
    fixed   y;
    uint8_t state;  // Should this be "Type" instead? how to deal with "bobbing" ?
} obs_ent;

extern obs_ent obstacles[];
extern uint8_t obstacles_active_last;
extern uint8_t obstacles_active_first;
extern uint8_t obstacles_active_count;
extern uint8_t obstacles_spawn_countdown;



void entity_obstacles_init(void);

uint8_t entity_obstacles_update(uint8_t);


#endif