#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "common.h"
#include "input.h"
#include "score.h"

#include "map_fx.h"

#include "entity_obstacles.h"
#include "../res/sprite_obstacles.h"


obs_ent obstacles[ENTITY_COUNT_OBSTACLES_TOTAL];
uint8_t obstacles_active_first;
uint8_t obstacles_active_last;
uint8_t obstacles_active_count;
uint8_t obstacles_spawn_countdown;


static inline void reset_spawn_countdown(void) {

    // TODO: just use div reg for this for speed?
    // obstacles_spawn_countdown = (DIV_REG & OBJECTS_SPAWN_COUNT_BITMASK) + OBJECTS_SPAWN_COUNT_MIN;
    obstacles_spawn_countdown = (rand() & OBJECTS_SPAWN_COUNT_BITMASK) + OBJECTS_SPAWN_COUNT_MIN;
}


void entity_obstacles_init(void) {
    obstacles_active_count = 0;
    obstacles_active_last = 0;
    obstacles_active_first = 0;
    reset_spawn_countdown();
}



// TODO: optimize: make oam_high_water global and don't pass it between functions
uint8_t entity_obstacles_update(uint8_t oam_high_water) {


    // Update all active obstacles
    // Process them in order from top of screen (last) down to bottom (first)
    // TODO: swap this out to using obstacles_active_first for counter?
    uint8_t idx = obstacles_active_last;
    for (uint8_t c = obstacles_active_count; c != 0; c--) {

        obstacles[idx].y.w += OBSTACLE_INC_SPEED;
        uint8_t y_pos = obstacles[idx].y.h;

        if (y_pos > OBSTACLE_Y_REMOVE) {
            // Obstacles are always in linear order. First in is always
            // first out, so the ones further down can be removed just by
            // reducing the total count.
            obstacles_active_count--;

            // // Record number of obstacles cleared
            // TODO: obstacles_cleared_count++;
            // Increment score and update display
            score_update();

            // Move head of list
            obstacles_active_first++;
            if (obstacles_active_first == ENTITY_COUNT_OBSTACLES_WRAP)
                obstacles_active_first = 0;

        } else {
            uint8_t object_sprite_sel = 2u;  // TODO

            // X position is found in the X scroll effect offset table at the Y position of the obstacle
            oam_high_water += move_metasprite(sprite_obstacles_metasprites[object_sprite_sel],
                                             (SPR_TILES_START_OBSTACLES),
                                             oam_high_water,
                                             CANYON_LEFT_X_BASE - p_scx_table_base[y_pos],
                                             y_pos);
        }

        // TODO: collision, could test and set marker for last index to start on
        //  if (y_pos > (ship.y + ship

        // Obstacles are stored as a circular buffer, wrap around to top if reached bottom
        if (idx == 0)
            idx = ENTITY_COUNT_OBSTACLES_MAX;
        else
            idx--;
    }



    // Add more obstacles if needed
    if (obstacles_active_count != ENTITY_COUNT_OBSTACLES_TOTAL) {

        // Is it time to spawn yet?
        if (obstacles_spawn_countdown)
            //  If not just increment counter
            obstacles_spawn_countdown--;
        else {
            // Spawn a new obstacle
            reset_spawn_countdown();

            obstacles_active_count++;

            // Obstacles are stored as a circular buffer, wrap around to start if maxed
            obstacles_active_last++;
            if (obstacles_active_last == ENTITY_COUNT_OBSTACLES_WRAP)
                obstacles_active_last = 0;

            // TODO: scroll screen down so starting at zero doesn't pop on-screen?
            obstacles[obstacles_active_last].y.w = OBSTACLE_SPAWN_Y;
            obstacles[obstacles_active_last].state = OBSTACLE_SPAWN_STATE; // TODO

            // If it is the first entry added, also set head of list
            if (obstacles_active_count == 1)
                obstacles_active_first = obstacles_active_last;
        }
    }

    return (oam_high_water);
}
