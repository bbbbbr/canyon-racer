#ifndef ENTITY_OBSTACLES_H
#define ENTITY_OBSTACLES_H


// == Obstacle Spawning Number control ==
#define OBSTACLES_COUNT_TOTAL 4u                            // Array size for obstacles
#define OBSTACLES_COUNT_MIN   0u                            // Min array index number
#define OBSTACLES_COUNT_MAX   (OBSTACLES_COUNT_TOTAL - 1u)  // Max array index number
#define OBSTACLES_COUNT_WRAP  (OBSTACLES_COUNT_TOTAL)       // Wrap around to zero when (array index == this num)


// ========== Settings ==========

    // == Obstacle Speed and Removal ==
    // Speed is in Fractional units of 1/256th of a pixel
    // TODO: try to match speed to innermost canyon section?
    #define FIXED_LS_BYTE_SZ      256u
    // TODO
    #define OBST_INC_SPD_EASY     450u
    #define OBST_INC_SPD_MED      500u
    #define OBST_INC_SPD_MED_HARD 540u
    #define OBST_INC_SPD_HARD     597u

    // == Obstacle Spawning Distance control ==

    // Scale up to speed units (x 256) and makes easier to divide by speed at runtime
    #define OBS_COUNT_SCALE_UP(scanlines) (scanlines * FIXED_LS_BYTE_SZ)

    #define OBST_DIST_MIN_EASY    OBS_COUNT_SCALE_UP(45u)
    #define OBST_DIST_MIN_MED     OBS_COUNT_SCALE_UP(35u)
    #define OBST_DIST_MIN_HARD    OBS_COUNT_SCALE_UP(30u) // TODO: is 25 too small and unplayable? Maybe 30?

    #define OBST_DIST_DBL         OBS_COUNT_SCALE_UP(10u) // Doubles: 10 pixels between spawning

    // == Max num obstacles per level ==
    #define OBST_QTY_LVL_EASY           ((OBSTACLES_COUNT_TOTAL) - 2u)
    #define OBST_QTY_LVL_MED            ((OBSTACLES_COUNT_TOTAL) - 1u)
    #define OBST_QTY_LVL_HARD           ((OBSTACLES_COUNT_TOTAL) - 0u)


    // TODO: maybe make this adjustable?
    #define OBSTACLE_NEXT_COUNT_BITMASK 0x3Fu  // Bitmask to select Range + Min from above  // TODO: fixme or ok just fixed as is?


// ========== Type / Spawn Location ==========

    // TODO: scroll screen down so starting at zero doesn't pop on-screen? if so, adjust OBSTACLE_REMOVE_Y
    #define OBSTACLE_SPAWN_Y      (0u << 8) // TODO: fixme
    #define OBSTACLE_SPAWN_STATE  0u        // TODO: fixme

    #define OBSTACLE_REMOVE_Y     144u    // Remove obstacles after they exceed this threshold (Then added to the score. obstacle_y is top edge)

    // #define OBSTACLE_NEXT_TYPE_BITMASK 0x03u  // 4 types of obstacles to spawn

    #define OBSTACLE_TYPE_MASK    0x03u  // Should exactly cover range of values used below
    #define OBSTACLE_TYPE_LEFT    0x00u
    #define OBSTACLE_TYPE_RIGHT   0x01u
    #define OBSTACLE_TYPE_MIDDLE  0x02u
    #define OBSTACLE_TYPE_FULL    0x03u

    #define OBJECTS_FLAG_DOUBLE_BIT  0x04u
    #define OBJECTS_FLAG_BOBBING_BIT 0x08u


// ========== Hitbox ==========

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
    uint8_t type;  // how to deal with "bobbing" ?
} obs_ent;


// Need extern access to these for collision detection
extern obs_ent obstacles[];
extern uint8_t obstacles_active_last;
extern uint8_t obstacles_active_first;
extern uint8_t obstacles_active_count;
extern uint8_t obstacles_spawn_countdown;

// Extern access for level adjustment via OBSTACLES_LEVEL_SET() macro
// (less overhead than putting them on the stack)
extern uint8_t  obstacles_active_max;
extern uint8_t  obstacles_dist_min;
extern uint8_t  obstacles_dist_double;
extern uint16_t obstacles_inc_speed;


// Updates Difficulty level related control cars for
// max num of obstacles and their spawn distance spacing
#define OBSTACLES_LEVEL_SET(obst_qty_max, obst_dist_min, obst_dist_double, obst_speed) \
    obstacles_active_max  = obst_qty_max; \
    obstacles_dist_min    = obst_dist_min; \
    obstacles_dist_double = obst_dist_double; \
    obstacles_inc_speed   = obst_speed;





extern const uint8_t obstacles_x_hitbox_left[];
extern const uint8_t obstacles_x_hitbox_right[];


void entity_obstacles_init(void);

uint8_t entity_obstacles_update(uint8_t);



#endif