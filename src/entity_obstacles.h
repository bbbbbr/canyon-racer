#ifndef ENTITY_OBSTACLES_H
#define ENTITY_OBSTACLES_H


// == Obstacle Spawning Number control ==
#define OBSTACLES_COUNT_TOTAL 4u                            // Array size for obstacles
#define OBSTACLES_COUNT_MIN   0u                            // Min array index number
#define OBSTACLES_COUNT_MAX   (OBSTACLES_COUNT_TOTAL - 1u)  // Max array index number
#define OBSTACLES_COUNT_WRAP  (OBSTACLES_COUNT_TOTAL)       // Wrap around to zero when (array index == this num)

// == Obstacle Speed and Removal ==
// Speed is in Fractional units of 1/255th of a pixel
// TODO: try to match speed to innermost canyon section?
#define FIXED_LS_BYTE_MAX     255u
// TODO
    #define OBSTACLE_INC_SPEED_MIN    450u
    #define OBSTACLE_INC_SPEED_MAX    597u
#define OBSTACLE_INC_SPEED    500u // 551u //597u // 500u //0x255u      // vertical 1.9 pixel pers frame  // TODO make variable? Speed seems fairly reasonable & universal
#define OBSTACLE_Y_REMOVE     144u    // Remove obstacles after they exceed this threshold (Then added to the score. obstacle_y is top edge)

// THIS ISNT WORKING RIGHT:
#define OBS_COUNT_FROM_SCANLINES(scanlines) (scanlines / (OBSTACLE_INC_SPEED / FIXED_LS_BYTE_MAX))

// == Obstacle Spawning Distance control ==
// TODO: Min below depends on whether jumping distance / duration becomes variable
#define OBSTACLE_NEXT_COUNT_MIN     OBS_COUNT_FROM_SCANLINES(25u) // Min frames between spawning 25 scanlines
#define OBSTACLE_NEXT_COUNT_DOUBLE  OBS_COUNT_FROM_SCANLINES(10u) // Doubles: 10 pixels between spawning


#define OBST_DIST_MIN_LVL_EASY       OBS_COUNT_FROM_SCANLINES(45u)
#define OBST_DIST_MIN_LVL_MED        OBS_COUNT_FROM_SCANLINES(35u)
#define OBST_DIST_MIN_LVL_HARD       (OBSTACLE_NEXT_COUNT_MIN) // TODO: is this too small and unplayable? Maybe 30?

#define OBST_QTY_LVL_EASY ((OBSTACLES_COUNT_TOTAL) - 2u)
#define OBST_QTY_LVL_MED  ((OBSTACLES_COUNT_TOTAL) - 1u)
#define OBST_QTY_LVL_HARD (OBSTACLES_COUNT_TOTAL)


// TODO: maybe make this adjustable?
#define OBSTACLE_NEXT_COUNT_BITMASK 0x3Fu  // Bitmask to select Range + Min from above  // TODO: fixme or ok just fixed as is?

// TODO: scroll screen down so starting at zero doesn't pop on-screen? if so, adjust OBSTACLE_Y_REMOVE
#define OBSTACLE_SPAWN_Y      (0u << 8) // TODO: fixme
#define OBSTACLE_SPAWN_STATE  0u        // TODO: fixme

// #define OBSTACLE_NEXT_TYPE_BITMASK 0x03u  // 4 types of obstacles to spawn

#define OBSTACLE_TYPE_MASK    0x03u  // Should exactly cover range of values used below
#define OBSTACLE_TYPE_LEFT    0x00u
#define OBSTACLE_TYPE_RIGHT   0x01u
#define OBSTACLE_TYPE_MIDDLE  0x02u
#define OBSTACLE_TYPE_FULL    0x03u

#define OBJECTS_FLAG_DOUBLE_BIT  0x04u
#define OBJECTS_FLAG_BOBBING_BIT 0x08u



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


extern const uint8_t obstacles_x_hitbox_left[];
extern const uint8_t obstacles_x_hitbox_right[];

void obstacles_level_set(uint8_t, uint8_t);

void entity_obstacles_init(void);

uint8_t entity_obstacles_update(uint8_t);



#endif