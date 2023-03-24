#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "common.h"
#include "input.h"

#include "score.h"
#include "level.h"

#include "entity_ship.h"

#include "map_fx.h"

#include "entity_obstacles.h"
#include "../res/sprite_obstacles.h"



const uint8_t obstacles_x_hitbox_left[] = {
    OBSTACLE_HITBOX_X_ST__TYPE_LEFT,
    OBSTACLE_HITBOX_X_ST__TYPE_RIGHT,
    OBSTACLE_HITBOX_X_ST__TYPE_MIDDLE,
    OBSTACLE_HITBOX_X_ST__TYPE_FULL,

    OBSTACLE_HITBOX_X_ST__TYPE_ITEM_PLUS_1
};

const uint8_t obstacles_x_hitbox_right[] = {
    OBSTACLE_HITBOX_X_END_TYPE_LEFT,
    OBSTACLE_HITBOX_X_END_TYPE_RIGHT,
    OBSTACLE_HITBOX_X_END_TYPE_MIDDLE,
    OBSTACLE_HITBOX_X_END_TYPE_FULL,

    OBSTACLE_HITBOX_X_END_TYPE_ITEM_PLUS_1
};


static inline void queue_next(void) {

    uint8_t type = rand();
    state.obstacles_next_type      = (type & OBSTACLE_TYPE_MASK);

    // Check for queueing an item
    // Must be a non-zero value to be utilized
    if ((type & OBJECTS_RANDOM_ITEM_SPAWN_MASK) == OBJECTS_RANDOM_ITEM_SPAWN_OK)
        state.obstacles_item_queued = OBJECT_TYPE_ITEM_PLUS_1;

    // If random selection is for a double obstacle, space the next one close together
    if ((type & OBJECTS_RANDOM_DOUBLE_BIT) && (!state.obstacles_next_isdouble)) {
        state.obstacles_next_countdown = state.cur_level.obst_dist_double;
        state.obstacles_next_isdouble = true;
    }
    else {
        state.obstacles_next_countdown = (rand() & OBSTACLE_NEXT_COUNT_BITMASK) + state.cur_level.obst_dist_min;
        // TODO: DEBUG: force distance to min distance
        // state.obstacles_next_countdown = state.cur_level.obst_dist_min;
        state.obstacles_next_isdouble = false;
    }
}



// Init obstacles, mostly zero out the ring buffer, etc
// Note: Call after level_update_vars();
void entity_obstacles_init(void) {

    // Some vars are initialized by level_update_vars();

    state.obstacles_active_count = 0u;
    state.obstacles_active_last  = 0u;
    state.obstacles_active_first = 0u;

    state.obstacles_next_isdouble = false;
    state.obstacles_item_queued   = ITEM_NOT_QUEUED;

    // Seed the initial obstacle
    queue_next();
    // Override value for "obstacles_next_countdown" in queue_next() to add longer initial delay
    state.obstacles_next_countdown = OBSTACLE_NEXT_COUNT_GAMEPLAY_START;
}



// TODO: MAYBE: OPTIMIZE: make oam_high_water global and don't pass it between functions
uint8_t entity_obstacles_update(uint8_t oam_high_water) {


    // Update all active obstacles
    // Process them in order from top of screen (last) down to bottom (first)
    uint8_t idx = state.obstacles_active_last;
    for (uint8_t c = state.obstacles_active_count; c != 0; c--) {

        state.obstacles[idx].y.w += state.cur_level.obst_speed;
        uint8_t y_pos = state.obstacles[idx].y.h;
        uint8_t object_type = state.obstacles[idx].type;

        // Obstacle scrolled off-screen?
        if (y_pos > OBSTACLE_REMOVE_Y) {
            // Obstacles are always in linear order. First in is always
            // first out, so the ones further down can be removed just by
            // reducing the total count.
            state.obstacles_active_count--;

            // Then move head of list to match reduced size (+ handle wraparound)
            // (Again- since it's FIFO ordering: the one to remove will always be
            //  the first entry. So it's sufficient to increment that marker var)
            state.obstacles_active_first++;
            if (state.obstacles_active_first == OBSTACLES_COUNT_WRAP)
                state.obstacles_active_first = 0;

            // Record number of obstacles cleared
            // Only count for score if game is not over (i.e. crashed, etc)
            if (SHIP_STATE_GET() & SHIP_BITS_PLAYER_ACTIVE) {

                // Update for OBSTACLES only no score and level incrementing for ITEMS
                // (not checked earlier since they still need to get moved off-screen and de-queued)
                if ((object_type & OBJECT_TYPE_ITEM_MASK) <= OBSTACLE_TYPE_FULL) {
                    SCORE_INCREMENT();
                    score_update();
                    LEVEL_INC_AND_CHECK();
                }
            }
        } else {
            // Obstacle is active and not getting removed so draw it

            // Skip hidden entries (i.e. consumed items)
            if (!(object_type & OBJECTS_FLAG_HIDDEN_BIT)) {

                // X position is found in the X scroll effect offset table at the Y position of the obstacle
                oam_high_water += move_metasprite(sprite_obstacles_metasprites[object_type & OBJECT_SPRITE_MASK],
                                                 (SPR_TILES_START_OBSTACLES),
                                                 oam_high_water,
                                                 CANYON_LEFT_X_BASE - state.p_scx_table_frame_base[y_pos],
                                                 y_pos);
            }
        }

        // Obstacles are stored as a circular buffer, wrap around to top if reached bottom
        if (idx == OBSTACLES_COUNT_MIN)
            idx = OBSTACLES_COUNT_MAX;
        else
            idx--;
    }


    // TODO: move to function if there is overhead for it:
    // entity_obstacles_check_spawn_new()

    // Add more obstacles if needed
    // But only when the in normal gameplay -- TODO: Split ship motion state from game state
    if ((state.obstacles_active_count != state.cur_level.obst_qty_max) &&
        (SHIP_STATE_GET() & SHIP_BITS_PLAYER_ACTIVE)) {

        // Is it time to spawn yet?
        if (state.obstacles_next_countdown) {
            //  If not just increment counter
            state.obstacles_next_countdown--;
        } else {
            // Spawn a new obstacle
            state.obstacles_active_count++;

            // Obstacles are stored as a circular buffer, wrap around to start if maxed
            state.obstacles_active_last++;
            if (state.obstacles_active_last == OBSTACLES_COUNT_WRAP)
                state.obstacles_active_last = 0;

            // TODO: scroll screen down so starting at zero doesn't cause sprites to pop on-screen? If so, adjust OBSTACLE_REMOVE_Y
            state.obstacles[state.obstacles_active_last].y.w = OBSTACLE_SPAWN_Y;

            // Override the obstacle and replace it with the queued item
            if (state.obstacles_item_queued) {
                state.obstacles[state.obstacles_active_last].type = state.obstacles_item_queued;
                state.obstacles_item_queued = ITEM_NOT_QUEUED; // Clear queue
            }
            else
                state.obstacles[state.obstacles_active_last].type = state.obstacles_next_type;

            // If this is first entry added, also set head of list
            if (state.obstacles_active_count == 1)
                state.obstacles_active_first = state.obstacles_active_last;

            // Queue up another now that current is activated
            queue_next();
        }
    }

    return (oam_high_water);
}
