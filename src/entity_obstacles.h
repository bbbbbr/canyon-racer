#ifndef entity_obstacles_H
#define entity_obstacles_H


#define ENTITY_COUNT_OBSTACLES_TOTAL 8u
#define ENTITY_COUNT_OBSTACLES_MAX   (ENTITY_COUNT_OBSTACLES_TOTAL - 1u)
#define ENTITY_COUNT_OBSTACLES_WRAP  (ENTITY_COUNT_OBSTACLES_TOTAL)

#define OBJECTS_SPAWN_COUNT_MIN     45u // 0x1Fu  // min frames between spawning
#define OBJECTS_SPAWN_COUNT_BITMASK 0x3Fu  // bitmask to select range + min
// #define OBJECTS_SPAWN_COUNT_MIN     0x1Fu  // min frames between spawning
// #define OBJECTS_SPAWN_COUNT_BITMASK 0x7Fu  // bitmask to select range + min

#define OBSTACLE_SPAWN_Y      (0u << 8) // TODO: fixme
#define OBSTACLE_SPAWN_STATE  0u  // TODO: fixme

// TODO: try to match speed to innermost canyon section
#define OBSTACLE_INC_SPEED    0x128u  // 1/2 pixel per frame  // TODO make variable?
#define OBSTACLE_Y_REMOVE     144u    // Remove obstacles after they exceed this threshold

void entity_obstacles_init(void);

uint8_t entity_obstacles_update(uint8_t);

#endif