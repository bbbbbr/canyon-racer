#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "input.h"
#include "score.h"

#include "map_fx.h"

#include "entity_collisions.h"

#include "entity_ship.h"
#include "../res/sprite_ship.h"

#include "entity_obstacles.h"
#include "../res/sprite_obstacles.h"



fixed   ship_x, ship_y;
fixed   ship_z;
uint8_t ship_sprite_sel;
uint8_t ship_state;
uint8_t ship_counter;
uint8_t ship_sprite_sel;
int16_t ship_velocity;

void entity_ship_init(void) {
    ship_x.w = SHIP_X_INIT;
    ship_y.w = SHIP_Y_INIT;
    ship_z.w = SHIP_Z_INIT;
    ship_state = SHIP_STATE_STARTUP;
    ship_counter = SHIP_COUNTER_STARTUP;
    ship_velocity = 0u;
}

#define SHIP_VELOCITY_START 768
#define SHIP_GRAVITY 32
#define SHIP_VELOCITY_MIN -512
#define SHIP_VELOCITY_MIN_ABS 512


static void ship_handle_input(void) {

    switch (KEYS() & (J_LEFT | J_RIGHT)) {

        case J_LEFT:
            if (ship_x.w > SHIP_X_MIN) {
                ship_x.w -= SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel = SHIP_SPR_LEFT;
            break;

        case J_RIGHT:
            if (ship_x.w < SHIP_X_MAX) {
                ship_x.w += SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel = SHIP_SPR_RIGHT;
            break;
    }

    // Vertical Movement
    switch (KEYS() & (J_UP | J_DOWN)) {

        case J_UP:
            if (ship_y.w > SHIP_Y_MIN) {
                ship_y.w -= SHIP_MOVE_AMT_Y;
            }
            break;

        case J_DOWN:
            if (ship_y.w < SHIP_Y_MAX) {
                ship_y.w += SHIP_MOVE_AMT_Y;
            }
            break;
    }

    // Jumping
    if (KEY_TICKED(J_A)) {
        if (ship_state ==  SHIP_STATE_PLAYING) {

            ship_state = SHIP_STATE_JUMP;
            // ship_counter = SHIP_COUNTER_JUMPSTART;
            ship_velocity = SHIP_VELOCITY_START;
            ship_z.h = 1;
        }
    }


}


// TODO: split to functions if there is enough overhead left
uint8_t entity_ship_update(uint8_t oam_high_water) {

    ship_sprite_sel = SHIP_SPR_DEFAULT;

    switch (ship_state) {

        case SHIP_STATE_STARTUP:
            if (ship_counter)
                ship_counter--;
            else
                ship_state = SHIP_STATE_PLAYING;

            // Blink ship on/off to ready player
            if ((ship_counter & 0x03u) == 0)
                ship_sprite_sel = SHIP_SPR_DEFAULT;
            else
                ship_sprite_sel = SHIP_SPR_NONE;

            // Run ship along edge of canyon automatically during startup
            uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - p_scx_table_frame_base[ship_y.h];
            ship_x.w = (ship_canyon_left_x + (CANYON_WIDTH - sprite_ship_WIDTH) / 2) << 8;
            break;

        case SHIP_STATE_CRASHED:
            // If crashed, render explosion then restart
            if (ship_counter) {
                ship_counter--;
                // Select crash frame
                ship_sprite_sel = (ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;
            }
            else {
                // Done showing crash explosion, hide ship and exit gameplay
                ship_state = SHIP_STATE_GAMEOVER;
                    // TODO: FOR DEBUG
                    // ship_state = SHIP_STATE_STARTUP;
                    // ship_counter = SHIP_COUNTER_STARTUP;
                ship_sprite_sel = SHIP_SPR_NONE;
            }

            break;

        case SHIP_STATE_JUMP:

            // Ship movement is allowed during jumping
            ship_handle_input();

            ship_z.w += ship_velocity;
            if (ship_z.w <= SHIP_VELOCITY_MIN_ABS) {
                // Ship landed
                ship_z.w = SHIP_Z_INIT;
                ship_state = SHIP_STATE_PLAYING;

            } else {
                // Ship still in air
                ship_velocity -= SHIP_GRAVITY;
                if (ship_velocity < SHIP_VELOCITY_MIN)
                    ship_velocity = SHIP_VELOCITY_MIN;
                // TODO: Use a lookup table for Y offset of ship from shadow?
                // TODO: Adjustable duration jump?
            }
            // Set shadow
            ship_sprite_sel = SHIP_SPR_JUMP; // (ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;

            // And also draw separated ship
            oam_high_water += move_metasprite(sprite_ship_metasprites[SHIP_SPR_DEFAULT],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, ship_x.h, ship_y.h - ship_z.h);
            break;

        case SHIP_STATE_PLAYING:

            ship_handle_input();

            if (check_collisions() == true) {
                // Will take effect next frame
                ship_state = SHIP_STATE_CRASHED;
                ship_counter = SHIP_COUNTER_CRASH;
            }
            break;
    }

    // ==== Update Sprite ====

    // if (KEY_PRESSED(J_B)) {
    //     entity_shots_add(ship_x.h, ship_y.h);
    // }

    if (ship_sprite_sel != SHIP_SPR_NONE)
        oam_high_water += move_metasprite(sprite_ship_metasprites[ship_sprite_sel],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, ship_x.h, ship_y.h);

    return (oam_high_water);
}


