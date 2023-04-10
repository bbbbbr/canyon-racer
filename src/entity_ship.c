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

#include <audio.h>

#include "common.h"
#include "input.h"
#include "score.h"
#include "lives.h"

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
    state.ship_state = SHIP_STATE_ON_GROUND;
    state.ship_counter = SHIP_COUNTER_STARTUP_INVINCIBLE; // TODO: consider ship_invincible_counter
    state.ship_jump_velocity = 0u;
    // Place ship centered in canyon during startup
    entity_ship_center_in_canyon();
}

// Should be called after parallax/etc has been initialized
void entity_ship_center_in_canyon(void) {
    uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - state.p_scx_table_frame_base[state.ship_y.h];
    state.ship_x.w = (ship_canyon_left_x + (CANYON_WIDTH - sprite_ship_WIDTH) / 2) << 8;
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
        // Jump can only start when ship is on the ground
        if (state.ship_state == SHIP_STATE_ON_GROUND) {

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


static void ship_handle_collisions(void) {

    if (check_collisions() == true) {

        if (COLLISION_GET_LAST_TYPE() == OBJECT_TYPE_ITEM_PLUS_1) {
            // Flag object as hidden to remove it
            state.obstacles[COLLISION_GET_LAST_INDEX()].type |= OBJECTS_FLAG_HIDDEN_BIT;
            audio_sfx_play(SFX_GOT_ITEM); // TODO: SFX Got Item
            lives_count_increment();
            lives_display_update();
        }
        else {
            // Will take effect next frame
            state.ship_state = SHIP_STATE_CRASHING;
            state.ship_counter = SHIP_COUNTER_CRASH;
            // audio_sfx_play(SFX_SHIP_CRASH); // Replaced by crashed/gameover audio track

            audio_music_pause();
            audio_music_set(MUSIC_GAMEOVER_SONG);
            audio_music_unpause();
        }
    }
}


uint8_t entity_ship_update(uint8_t oam_high_water) {

    ship_sprite_sel = SHIP_SPR_DEFAULT;

    switch (state.ship_state) {

        case SHIP_STATE_CRASHING:
            // If crashed, render explosion then restart
            if (state.ship_counter) {
                state.ship_counter--;
                // Select crash frame
                ship_sprite_sel = (state.ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;
            }
            else {
                // Done showing crash explosion, hide ship and exit gameplay
                state.ship_state = SHIP_STATE_CRASH_ENDED;
                // Debug option to resume gameplay after crashing
                #ifdef DEBUG_RESUME_AFTER_CRASH
                    state.ship_state = SHIP_STATE_ON_GROUND;
                    state.ship_counter = SHIP_COUNTER_STARTUP_INVINCIBLE;
                #endif
                ship_sprite_sel = SHIP_SPR_NONE;
            }

            break;

        case SHIP_STATE_JUMP:

            // Ship movement is allowed during jumping
            ship_handle_input();
            // Save selected sprite for use in air (ground one is replaced by shadow)
            uint8_t ship_sprite_sel_jump = ship_sprite_sel;

            // If current velocity will cause ship to hit the ground (<= 0)
            // then land it exactly and change state
            if ((state.ship_jump_velocity < 0) && ((state.ship_jump_velocity * -1) > state.ship_z.w)) {
                // Ship landed
                state.ship_z.w = SHIP_Z_INIT;
                state.ship_state = SHIP_STATE_ON_GROUND;
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
            ship_sprite_sel = SHIP_SPR_JUMP;

            // Blink ship on/off to ready player if in startup invincibility mode
            bool flicker_hide_ship = false;
            if (state.ship_counter) {
                state.ship_counter--;
                if ((state.ship_counter & 0x03u) != 0)
                    flicker_hide_ship = true;
            }

            // And also draw separated ship using Jump Z offset for Y axis
            if (!flicker_hide_ship)
                oam_high_water += move_metasprite(sprite_ship_metasprites[ship_sprite_sel_jump],
                                             (SPR_TILES_START_SHIP),
                                             oam_high_water, state.ship_x.h, state.ship_y.h - state.ship_z.h);
            break;

        case SHIP_STATE_ON_GROUND:

            ship_handle_input();

            // Blink ship on/off to ready player if in startup invincibility mode
            // Otherwise handle collisions
            if (state.ship_counter) {
                state.ship_counter--;
                if ((state.ship_counter & 0x03u) != 0)
                    ship_sprite_sel = SHIP_SPR_NONE; // Overrides default of SHIP_SPR_DEFAULT
            } else
                ship_handle_collisions();

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


