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



obs_ent obstacles[OBSTACLES_COUNT_TOTAL];

// == Difficulty level related ==
    // See struct level_entry {}, level_entry settings

// == Active entities related
uint8_t obstacles_active_first;
uint8_t obstacles_active_last;
uint8_t obstacles_active_count;

uint8_t obstacles_next_countdown;
uint8_t obstacles_next_type;
uint8_t obstacles_next_isdouble;

const uint8_t obstacles_x_hitbox_left[] = {
    OBSTACLE_HITBOX_X_ST__TYPE_LEFT,
    OBSTACLE_HITBOX_X_ST__TYPE_RIGHT,
    OBSTACLE_HITBOX_X_ST__TYPE_MIDDLE,
    OBSTACLE_HITBOX_X_ST__TYPE_FULL
};

const uint8_t obstacles_x_hitbox_right[] = {
    OBSTACLE_HITBOX_X_END_TYPE_LEFT,
    OBSTACLE_HITBOX_X_END_TYPE_RIGHT,
    OBSTACLE_HITBOX_X_END_TYPE_MIDDLE,
    OBSTACLE_HITBOX_X_END_TYPE_FULL
};


static inline void queue_next(void) {

    // TODO:
    // #define OBJECTS_FLAG_DOUBLE_BIT  0x04u
    // #define OBJECTS_FLAG_BOBBING_BIT 0x08u

    uint8_t type = rand();
    obstacles_next_type      = (type & OBSTACLE_TYPE_MASK);

    // If random selection is for a double obstacle, space the next one close together
    if ((type & OBJECTS_FLAG_DOUBLE_BIT) && (!obstacles_next_isdouble)) {
        obstacles_next_countdown = cur_level.obst_dist_double;
        obstacles_next_isdouble = true;
    }
    else {
        obstacles_next_countdown = (rand() & OBSTACLE_NEXT_COUNT_BITMASK) + cur_level.obst_dist_min;
        // TODO: DEBUG: force distance to min distance
        // obstacles_next_countdown = cur_level.obst_dist_min;
        obstacles_next_isdouble = false;
    }
}



// Init obstacles, mostly zero out the ring buffer, etc
// Note: Call after level_update_vars();
void entity_obstacles_init(void) {

    // Some vars are initialized by level_update_vars();

    obstacles_active_count = 0u;
    obstacles_active_last  = 0u;
    obstacles_active_first = 0u;

    obstacles_next_isdouble = false;

    // Seed the initial obstacle
    queue_next();
}



// TODO: optimize: make oam_high_water global and don't pass it between functions
uint8_t entity_obstacles_update(uint8_t oam_high_water) {


    // Update all active obstacles
    // Process them in order from top of screen (last) down to bottom (first)
    // TODO: Swap this out to using obstacles_active_first for counter? - Why?
    uint8_t idx = obstacles_active_last;
    for (uint8_t c = obstacles_active_count; c != 0; c--) {

        obstacles[idx].y.w += cur_level.obst_speed;
        uint8_t y_pos = obstacles[idx].y.h;
        uint8_t object_sprite_sel = obstacles[idx].type;

        // Obstacle scrolled off-screen?
        if (y_pos > OBSTACLE_REMOVE_Y) {
            // Obstacles are always in linear order. First in is always
            // first out, so the ones further down can be removed just by
            // reducing the total count.
            obstacles_active_count--;

            // Then move head of list to match reduced size (+ handle wraparound)
            // (Again- since it's FIFO ordering: the one to remove will always be
            //  the first entry. So it's sufficient to increment that marker var)
            obstacles_active_first++;
            if (obstacles_active_first == OBSTACLES_COUNT_WRAP)
                obstacles_active_first = 0;

            // Record number of obstacles cleared
            // Only count for score if game is not over
            // TODO: HACK: state test here is temporary
            if ((ship_state == SHIP_STATE_PLAYING) || (ship_state == SHIP_STATE_JUMP)) {
                SCORE_INCREMENT();
                score_update();
                LEVEL_INC_AND_CHECK();
            }
        } else {
            // Obstacle is active and not getting removed - Draw it

            // X position is found in the X scroll effect offset table at the Y position of the obstacle
            oam_high_water += move_metasprite(sprite_obstacles_metasprites[object_sprite_sel],
                                             (SPR_TILES_START_OBSTACLES),
                                             oam_high_water,
                                             CANYON_LEFT_X_BASE - p_scx_table_frame_base[y_pos],
                                             y_pos);
        }

        // TODO: collision, could test and set marker for last index to start on
        //  if (y_pos > (ship.y + ship

        // Obstacles are stored as a circular buffer, wrap around to top if reached bottom
        if (idx == OBSTACLES_COUNT_MIN)
            idx = OBSTACLES_COUNT_MAX;
        else
            idx--;
    }


    // TODO: move to function is there is overhead for it:
    // entity_obstacles_check_spawn_new()

    // Add more obstacles if needed
    // But only when the in normal gameplay -- TODO: Split ship motion from game state
    if ((obstacles_active_count != cur_level.obst_qty_max) &&
        ((SHIP_STATE_GET() == SHIP_STATE_PLAYING) ||
        (SHIP_STATE_GET() == SHIP_STATE_JUMP)) ) {

        // Is it time to spawn yet?
        if (obstacles_next_countdown) {
            //  If not just increment counter
            obstacles_next_countdown--;
        } else {
            // Spawn a new obstacle
            obstacles_active_count++;

            // Obstacles are stored as a circular buffer, wrap around to start if maxed
            obstacles_active_last++;
            if (obstacles_active_last == OBSTACLES_COUNT_WRAP)
                obstacles_active_last = 0;

            // TODO: scroll screen down so starting at zero doesn't pop on-screen? If so, adjust OBSTACLE_REMOVE_Y
            obstacles[obstacles_active_last].y.w = OBSTACLE_SPAWN_Y;
            obstacles[obstacles_active_last].type = obstacles_next_type;

            // If this is first entry added, also set head of list
            if (obstacles_active_count == 1)
                obstacles_active_first = obstacles_active_last;

            // Queue up another now that current is activated
            queue_next();
        }
    }

    return (oam_high_water);
}
