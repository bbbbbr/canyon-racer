// Copyright 2023 (c) bbbbbr
//
// This software is licensed under:
//
// CC-BY-NC-SA: Attribution Non-Commercial Share Alike Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International License
// See: http://creativecommons.org/licenses/by-nc-sa/4.0/
//
// For the purposes of this project "Share Alike" shall also include
// distribution of the source code and any changes to it.

#ifndef ENTITY_OBSTACLES_H
#define ENTITY_OBSTACLES_H


// == Obstacle Spawning Number control ==
#define OBSTACLES_COUNT_TOTAL 5u                            // Array size for obstacles
#define OBSTACLES_COUNT_MIN   0u                            // Min array index number
#define OBSTACLES_COUNT_MAX   (OBSTACLES_COUNT_TOTAL - 1u)  // Max array index number
#define OBSTACLES_COUNT_WRAP  (OBSTACLES_COUNT_TOTAL)       // Wrap around to zero when (array index == this num)


// ========== Settings ==========

    // == Obstacle Speed and Removal ==
    // Speed is in Fractional units of 1/256th of a pixel
    #define FIXED_LS_BYTE_SZ      256u

    #define OBST_INC_SPD_EASY     500u
    #define OBST_INC_SPD_MED      550u
    #define OBST_INC_SPD_MED_HARD 575u
    #define OBST_INC_SPD_HARD     597u
        // Old, easier settings
        // #define OBST_INC_SPD_EASY     450u
        // #define OBST_INC_SPD_MED      500u
        // #define OBST_INC_SPD_MED_HARD 540u
        // #define OBST_INC_SPD_HARD     597u

    // == Obstacle Spawning Distance control ==

    // Scale up to speed units (x 256) and makes easier to divide by speed at runtime
    #define OBS_COUNT_SCALE_UP(scanlines) (scanlines * FIXED_LS_BYTE_SZ)

    #define OBST_DIST_MIN_EASY    OBS_COUNT_SCALE_UP(90u)
    #define OBST_DIST_MIN_MED     OBS_COUNT_SCALE_UP(85u)
    #define OBST_DIST_MIN_HARD    OBS_COUNT_SCALE_UP(70u)
    #define OBST_DIST_MIN_HARD2   OBS_COUNT_SCALE_UP(55u)
    #define OBST_DIST_DBL         OBS_COUNT_SCALE_UP(10u) // Doubles: 10 pixels between spawning

    // == Max num obstacles per level ==
    #define OBST_QTY_LVL_EASY           ((OBSTACLES_COUNT_TOTAL) - 3u)
    #define OBST_QTY_LVL_MED            ((OBSTACLES_COUNT_TOTAL) - 2u)
    #define OBST_QTY_LVL_HARD           ((OBSTACLES_COUNT_TOTAL) - 1u)
    #define OBST_QTY_LVL_HARDEST        ((OBSTACLES_COUNT_TOTAL) - 0u)



    // Bitmask to select Range in addition to Min from above
    #define OBSTACLE_NEXT_COUNT_BITMASK_EASY 0x3Fu  // 0 - 31
    #define OBSTACLE_NEXT_COUNT_BITMASK_MED 0x1Fu  // 0 - 31
    #define OBSTACLE_NEXT_COUNT_BITMASK_HARD 0x0Fu  // 0 - 31

    #define OBSTACLE_NEXT_COUNT_BITMASK (OBSTACLE_NEXT_COUNT_BITMASK_MED) // 0x0Fu

    #define OBSTACLE_NEXT_COUNT_GAMEPLAY_START  (60u * 3u) // 3 Second Delay at start of gameplay for extra player breathing room. 255 MAX

// ========== Type / Spawn Location ==========

    #define OBSTACLE_SPAWN_Y      (0u << 8)
    #define OBSTACLE_SPAWN_STATE  0u

    #define OBSTACLE_REMOVE_Y     144u    // Remove obstacles after they exceed this threshold (Then added to the score. obstacle_y is top edge)


    #define OBSTACLE_TYPE_LEFT    0x00u
    #define OBSTACLE_TYPE_RIGHT   0x01u
    #define OBSTACLE_TYPE_MIDDLE  0x02u
    #define OBSTACLE_TYPE_FULL    0x03u

    #define OBSTACLE_TYPE_MASK    0x03u  // Should exactly cover range of values used above, subset of OBJECT_TYPE

    // Excluded by the OBSTACLE_TYPE_MASK generation mask
    #define OBJECT_TYPE_ITEM_PLUS_1  0x04u

    #define OBJECT_TYPE_ITEM_MASK  0x07u // Should include OBSTACLE_TYPE_MASK and OBJECT_TYPE_ITEM_PLUS_1

    // Items must have type > 0
    // For: obstacles_item_queued
    #define ITEM_NOT_QUEUED 0u

    // Must be large enough to fit all obstacle and item types above
    #define OBJECT_SPRITE_MASK    0x0Fu

    #define OBJECTS_FLAG_HIDDEN_BIT  0x80u // Should not overlap with any item or obstacle values

    // Can overlap with items as long as > OBSTACLE_TYPE_MASK, used to set obstacles_next_isdouble.
    // Not actually stored in "type" itself, just sampled from the shared random generation byte
    #define OBJECTS_RANDOM_DOUBLE_BIT       0x04u
    // Determines rate of special item spawning (mask match on random number == 0)
    #define OBJECTS_RANDOM_ITEM_SPAWN_MASK  0xF8u // 0xF0u  // Every ~32th object spawned, subject to random number generation
    #define OBJECTS_RANDOM_ITEM_SPAWN_OK    0x00u


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


    #define OBSTACLE_HITBOX_X_ST__TYPE_ITEM_PLUS_1 ((sprite_obstacles_WIDTH / 4u) * 1u)
    #define OBSTACLE_HITBOX_X_END_TYPE_ITEM_PLUS_1 ((sprite_obstacles_WIDTH / 4u) * 3u)


    #define OBSTACLE_HITBOX_Y_TOP    ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u)
    #define OBSTACLE_HITBOX_Y_BOTTOM ((sprite_obstacles_PIVOT_H) + ((sprite_obstacles_HEIGHT - sprite_obstacles_PIVOT_H) / 2u))


extern const uint8_t obstacles_x_hitbox_left[];
extern const uint8_t obstacles_x_hitbox_right[];


void entity_obstacles_init(void);
uint8_t entity_obstacles_update(uint8_t);
void entity_obstacles_check_spawn_new(void);



#endif