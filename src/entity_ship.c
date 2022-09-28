#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include <audio.h>

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
int16_t ship_jump_velocity;

void entity_ship_init(void) {
    ship_x.w = SHIP_X_INIT;  // X Position on screen
    ship_y.w = SHIP_Y_INIT;  // Y Position on screen
    ship_z.w = SHIP_Z_INIT;  // Z Jump position (alters Y position)
    ship_state = SHIP_STATE_STARTUP;
    ship_counter = SHIP_COUNTER_STARTUP;
    ship_jump_velocity = 0u;
}


// Roughly "classic" version
// All values used with fixed point 8.8
#define SHIP_JUMP_GRAVITY              32   // Gravity per frame
#define SHIP_JUMP_VELOCITY_START       800 // 768 (prev) // Controls max jump height (if button remains dowNn
#define SHIP_JUMP_VELOCITY_SMALL       350  // Min jump height if button released early
#define SHIP_JUMP_VELOCITY_DOWN_LIMIT  -512 // Max downward velocity

// This "works" ok but doesn't feel as fun
// TODO: Delete these
// #define SHIP_JUMP_GRAVITY              70
// #define SHIP_JUMP_VELOCITY_START       1300 // TODO: 1350?
// #define SHIP_JUMP_VELOCITY_SMALL       600
// #define SHIP_JUMP_VELOCITY_DOWN_LIMIT  -600
    // #define SHIP_JUMP_GRAVITY              60
    // #define SHIP_JUMP_VELOCITY_START       1200
    // #define SHIP_JUMP_VELOCITY_SMALL       500
    // #define SHIP_JUMP_VELOCITY_DOWN_LIMIT  -600
// #define SHIP_JUMP_GRAVITY              50
// #define SHIP_JUMP_VELOCITY_START       1000
// #define SHIP_JUMP_VELOCITY_SMALL       480
// #define SHIP_JUMP_VELOCITY_DOWN_LIMIT  -512


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
    // Note: Jumping applies a secondary vertical offset to a secondary sprite
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
        // Jump can only start when ship is on the ground (SHIP_STATE_PLAYING)
        if (ship_state == SHIP_STATE_PLAYING) {

            // TODO: separate jump state from playing state?
            ship_state = SHIP_STATE_JUMP;
            audio_sfx_play(SFX_SHIP_JUMP);

            ship_jump_velocity = SHIP_JUMP_VELOCITY_START;
            // ship_z.h = SHIP_Z_JUMP_START;  // TODO: Doesn't seem to be needed
        }
    } else if (ship_state == SHIP_STATE_JUMP) {
        if (!KEY_PRESSED(J_A)) {

            // If jump button is released before min jump threshold is reached
            // then then clamp jump to min jump amount instead
            if (ship_jump_velocity > SHIP_JUMP_VELOCITY_SMALL) {
                ship_jump_velocity = SHIP_JUMP_VELOCITY_SMALL;
            }
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
                // Debug option to resume gameplay after crashing
                #ifdef DEBUG_RESUME_AFTER_CRASH
                    ship_state = SHIP_STATE_STARTUP;
                    ship_counter = SHIP_COUNTER_STARTUP;
                #endif
                ship_sprite_sel = SHIP_SPR_NONE;
            }

            break;

        case SHIP_STATE_JUMP:

            // Ship movement is allowed during jumping
            ship_handle_input();

            // If current velocity will cause ship to hit the ground (<= 0)
            // then land it exactly and change state
            // TODO: use var to speed up past crest of jump testing?
            if ((ship_jump_velocity < 0) && ((ship_jump_velocity * -1) > ship_z.w)) {
                // Ship landed
                ship_z.w = SHIP_Z_INIT;
                ship_state = SHIP_STATE_PLAYING;
                audio_sfx_play(SFX_SHIP_LAND);

            } else {
                // Ship still in air

                // Apply current velocity to Jump Z position
                ship_z.w += ship_jump_velocity;

                // Apply gravity for next frame
                // and limit max speed
                ship_jump_velocity -= SHIP_JUMP_GRAVITY;
                if (ship_jump_velocity < SHIP_JUMP_VELOCITY_DOWN_LIMIT)
                    ship_jump_velocity = SHIP_JUMP_VELOCITY_DOWN_LIMIT;
            }

            // Set sprite at main player location to shadow
            ship_sprite_sel = SHIP_SPR_JUMP; // (ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;

            // And also draw separated ship using Jump Z offset for Y axis
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
                audio_sfx_play(SFX_SHIP_CRASH);
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


