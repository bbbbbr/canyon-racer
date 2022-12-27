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
    uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - p_scx_table_frame_base[ship_y.h  + (sprite_ship_HEIGHT / 2u)];

    if ((ship_x.h + SHIP_HITBOX_X_LEFT) < ship_canyon_left_x) {
        // Collision with wall on left edge
        return true;
    }
    else if ((ship_x.h + SHIP_HITBOX_X_RIGHT) > (ship_canyon_left_x + CANYON_WIDTH)) {
        // Collision with wall on right edge
        return true;
    }


    // == Check Obstacle collisions ==

    // They are in order from bottom of screen (first) to top (last)
    uint8_t idx = obstacles_active_first;

    for (uint8_t c = obstacles_active_count; c != 0; c--) {


        uint8_t obstacle_y_pos = obstacles[idx].y.h;
        uint8_t obstacle_type  = obstacles[idx].type;

        // First check if BOTTOM of obstacle is ABOVE top of ship
        if ((obstacle_y_pos + OBSTACLE_HITBOX_Y_BOTTOM) < (ship_y.h + SHIP_HITBOX_Y_TOP)) {
            // If true then No overlap/collision. Exit the loop since
            // Obstacles are in Y order and there can be no more collisions
            return false;
        }


        // Next check if TOP of obstacle is somewhere ABOVE bottom of ship
        // If True, then combined with above test, there MUST be Y overlap
        // (since both non-overlap tests failed)
        if ((obstacle_y_pos + OBSTACLE_HITBOX_Y_TOP) <= (ship_y.h + SHIP_HITBOX_Y_BOTTOM)) {

            // // If this is reached then the ship has Y overlap with the obstacle
            uint8_t obstacle_x_pos = CANYON_LEFT_X_BASE - p_scx_table_frame_base[obstacle_y_pos];

            uint8_t obstacle_x_hitbox_left  = obstacle_x_pos + obstacles_x_hitbox_left[obstacle_type];
            uint8_t obstacle_x_hitbox_right = obstacle_x_pos + obstacles_x_hitbox_right[obstacle_type];

            // Check X axis Overlap (via non-overlap)
            if ( ((obstacle_x_hitbox_left) <= (ship_x.h + SHIP_HITBOX_X_RIGHT)) &&
                 ((obstacle_x_hitbox_right) >= (ship_x.h + SHIP_HITBOX_X_LEFT)) ) {
                // Collision with object
                return true;
            }
        }

        // Obstacles are stored as a circular buffer, wrap around to top if reached bottom
        idx++;
        if (idx == OBSTACLES_COUNT_WRAP)
            idx = 0;
    }

    return false;
}
