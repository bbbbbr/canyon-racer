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

#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

#include "map_fx.h"

#include "entity_collisions.h"

#include "entity_ship.h"
#include "../res/sprite_ship.h"

#include "entity_obstacles.h"
#include "../res/sprite_obstacles.h"


// Used for retrieving info about collision if one is detected
uint8_t collision_last_type;
uint8_t collision_last_index;

// Checks collisions between:
// * Ship & Walls
// * Ship & Obstacles
//
bool check_collisions(void) {

    // Hitboxes are sized to be forgiving
    // Controlled by metasprite info (set during conversion)

    // == Check Wall collision

    // Use the ship Y position (middle of ship, not top which is ship.y) to index into the
    // canyon background X offset table (one row per scanline)
    // p_scx_table_frame_base gets updated at the start of every frame
    uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - state.p_scx_table_frame_base[state.ship_y.h  + (sprite_ship_HEIGHT / 2u)];

    if ((state.ship_x.h + SHIP_HITBOX_X_LEFT) < ship_canyon_left_x) {
        // Collision with wall on left edge
        return true;
    }
    else if ((state.ship_x.h + SHIP_HITBOX_X_RIGHT) > (ship_canyon_left_x + CANYON_WIDTH)) {
        // Collision with wall on right edge
        return true;
    }


    // == Check Obstacle collisions ==

    // They are in order from bottom of screen (first) to top (last)
    uint8_t idx = state.obstacles_active_first;

    for (uint8_t c = state.obstacles_active_count; c != 0; c--) {


        uint8_t obstacle_y_pos = state.obstacles[idx].y.h;
        // uint8_t obstacle_type  = state.obstacles[idx].type;
        collision_last_type  = state.obstacles[idx].type;


        // Skip hidden obstacles
        if (!(collision_last_type & OBJECTS_FLAG_HIDDEN_BIT)) {

            // First check if BOTTOM of obstacle is ABOVE top of ship
            if ((obstacle_y_pos + OBSTACLE_HITBOX_Y_BOTTOM) < (state.ship_y.h + SHIP_HITBOX_Y_TOP)) {
                // If true then No overlap/collision. Exit the loop since
                // Obstacles are in Y order and there can be no more collisions
                return false;
            }


            // Next check if TOP of obstacle is somewhere ABOVE bottom of ship
            // If True, then combined with above test, there MUST be Y overlap
            // (since both non-overlap tests failed)
            if ((obstacle_y_pos + OBSTACLE_HITBOX_Y_TOP) <= (state.ship_y.h + SHIP_HITBOX_Y_BOTTOM)) {

                // // If this is reached then the ship has Y overlap with the obstacle
                uint8_t obstacle_x_pos = CANYON_LEFT_X_BASE - state.p_scx_table_frame_base[obstacle_y_pos];

                uint8_t obstacle_x_hitbox_left  = obstacle_x_pos + obstacles_x_hitbox_left[collision_last_type];
                uint8_t obstacle_x_hitbox_right = obstacle_x_pos + obstacles_x_hitbox_right[collision_last_type];

                // Check X axis Overlap (via non-overlap)
                if ( ((obstacle_x_hitbox_left) <= (state.ship_x.h + SHIP_HITBOX_X_RIGHT)) &&
                     ((obstacle_x_hitbox_right) >= (state.ship_x.h + SHIP_HITBOX_X_LEFT)) ) {
                    // Collision with object
                    collision_last_index = idx;
                    return true;
                }
            }
        }

        // Obstacles are stored as a circular buffer, wrap around to top if reached bottom
        idx++;
        if (idx == OBSTACLES_COUNT_WRAP)
            idx = 0;
    }

    return false;
}
