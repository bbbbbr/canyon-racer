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



uint8_t ship_sprite_sel;

void entity_ship_init(void) {
    state.ship_x.w = SHIP_X_INIT;  // X Position on screen
    state.ship_y.w = SHIP_Y_INIT;  // Y Position on screen
    state.ship_z.w = SHIP_Z_INIT;  // Z Jump position (alters Y position)
    state.ship_state = SHIP_STATE_STARTUP;
    state.ship_counter = SHIP_COUNTER_STARTUP;
    state.ship_jump_velocity = 0u;
}


// Roughly "classic" version
// All values used with fixed point 8.8
#define SHIP_JUMP_GRAVITY              32   // Gravity per frame
#define SHIP_JUMP_VELOCITY_START       800 // 768 (prev) // Controls max jump height (if button remains dowNn
#define SHIP_JUMP_VELOCITY_SMALL       350  // Min jump height if button released early
#define SHIP_JUMP_VELOCITY_DOWN_LIMIT  -512 // Max downward velocity


static void ship_handle_input(void) {

    switch (KEYS() & (J_LEFT | J_RIGHT)) {

        case J_LEFT:
            if (state.ship_x.w > SHIP_X_MIN) {
                state.ship_x.w -= SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel = SHIP_SPR_LEFT;
            break;

        case J_RIGHT:
            if (state.ship_x.w < SHIP_X_MAX) {
                state.ship_x.w += SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel = SHIP_SPR_RIGHT;
            break;
    }

    // Vertical Movement
    // Note: Jumping applies a secondary vertical offset to a secondary sprite
    switch (KEYS() & (J_UP | J_DOWN)) {

        case J_UP:
            if (state.ship_y.w > SHIP_Y_MIN) {
                state.ship_y.w -= SHIP_MOVE_AMT_Y;
            }
            break;

        case J_DOWN:
            if (state.ship_y.w < SHIP_Y_MAX) {
                state.ship_y.w += SHIP_MOVE_AMT_Y;
            }
            break;
    }

    // Jumping
    if (KEY_TICKED(BUTTON__JUMP)) {
        // Jump can only start when ship is on the ground (SHIP_STATE_PLAYING)
        if (state.ship_state == SHIP_STATE_PLAYING) {

            // TODO: separate jump state from game state
            state.ship_state = SHIP_STATE_JUMP;
            audio_sfx_play(SFX_SHIP_JUMP);

            state.ship_jump_velocity = SHIP_JUMP_VELOCITY_START;
            // state.ship_z.h = SHIP_Z_JUMP_START;  // TODO: Doesn't seem to be needed
        }
    } else if (state.ship_state == SHIP_STATE_JUMP) {
        if (!KEY_PRESSED(BUTTON__JUMP)) {

            // If jump button is released before min jump threshold is reached
            // then then clamp jump to min jump amount instead
            if (state.ship_jump_velocity > SHIP_JUMP_VELOCITY_SMALL) {
                state.ship_jump_velocity = SHIP_JUMP_VELOCITY_SMALL;
            }
        }
    }

}


// TODO: split to functions if there is enough overhead left
uint8_t entity_ship_update(uint8_t oam_high_water) {

    ship_sprite_sel = SHIP_SPR_DEFAULT;

    switch (state.ship_state) {

        case SHIP_STATE_STARTUP:
            if (state.ship_counter)
                state.ship_counter--;
            else
                state.ship_state = SHIP_STATE_PLAYING;

            // Blink ship on/off to ready player
            if ((state.ship_counter & 0x03u) == 0)
                ship_sprite_sel = SHIP_SPR_DEFAULT;
            else
                ship_sprite_sel = SHIP_SPR_NONE;

            // Run ship along edge of canyon automatically during startup
            uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - state.p_scx_table_frame_base[state.ship_y.h];
            state.ship_x.w = (ship_canyon_left_x + (CANYON_WIDTH - sprite_ship_WIDTH) / 2) << 8;
            break;

        case SHIP_STATE_CRASHED:
            // If crashed, render explosion then restart
            if (state.ship_counter) {
                state.ship_counter--;
                // Select crash frame
                ship_sprite_sel = (state.ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;
            }
            else {
                // Done showing crash explosion, hide ship and exit gameplay
                state.ship_state = SHIP_STATE_GAMEOVER;
                // Debug option to resume gameplay after crashing
                #ifdef DEBUG_RESUME_AFTER_CRASH
                    state.ship_state = SHIP_STATE_STARTUP;
                    state.ship_counter = SHIP_COUNTER_STARTUP;
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
            if ((state.ship_jump_velocity < 0) && ((state.ship_jump_velocity * -1) > state.ship_z.w)) {
                // Ship landed
                state.ship_z.w = SHIP_Z_INIT;
                state.ship_state = SHIP_STATE_PLAYING;
                audio_sfx_play(SFX_SHIP_LAND);

            } else {
                // Ship still in air

                // Apply current velocity to Jump Z position
                state.ship_z.w += state.ship_jump_velocity;

                // Apply gravity for next frame
                // and limit max speed
                state.ship_jump_velocity -= SHIP_JUMP_GRAVITY;
                if (state.ship_jump_velocity < SHIP_JUMP_VELOCITY_DOWN_LIMIT)
                    state.ship_jump_velocity = SHIP_JUMP_VELOCITY_DOWN_LIMIT;
            }

            // Set sprite at main player location to shadow
            ship_sprite_sel = SHIP_SPR_JUMP; // (state.ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;

            // And also draw separated ship using Jump Z offset for Y axis
            oam_high_water += move_metasprite(sprite_ship_metasprites[SHIP_SPR_DEFAULT],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, state.ship_x.h, state.ship_y.h - state.ship_z.h);
            break;

        case SHIP_STATE_PLAYING:

            ship_handle_input();

            if (check_collisions() == true) {
                // Will take effect next frame
                state.ship_state = SHIP_STATE_CRASHED;
                state.ship_counter = SHIP_COUNTER_CRASH;
                // audio_sfx_play(SFX_SHIP_CRASH);

                audio_music_pause();
                audio_music_set(MUSIC_GAMEOVER_SONG);
                audio_music_unpause();
            }
            break;
    }

    // ==== Update Sprite ====

    // if (KEY_PRESSED(J_B)) {
    //     entity_shots_add(state.ship_x.h, state.ship_y.h);
    // }

    if (ship_sprite_sel != SHIP_SPR_NONE)
        oam_high_water += move_metasprite(sprite_ship_metasprites[ship_sprite_sel],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, state.ship_x.h, state.ship_y.h);

    return (oam_high_water);
}


