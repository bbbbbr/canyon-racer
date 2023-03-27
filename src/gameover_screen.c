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
#include <rand.h>

#include "input.h"
#include "common.h"
#include "fade.h"
#include "audio.h"
#include "gfx.h"
#include "map_fx.h"

#include "../res/game_over.h"

#include "gameover_screen.h"

// TODO: wait for obstacles to clear off screen? or "Sink" them out to make it faster

// Start gameplay sprites after score sprite (overwrites obstacles/ship/etc)
#define SPR_ID_GAME_OVER_START (SPR_ID_FREE_START)


// TODO: Finalize which table to use. Movement looks a little chunkier in non-DMG gameover mode
// TODO: OPTIMIZE: The bit shifting for the smaller table might be pushed out to GAME_OVER_BIT_SHIFT for more savings
// #define USE_SIN_TABLE_128_BYTES_HALF_SIZE

#ifndef USE_SIN_TABLE_128_BYTES_HALF_SIZE
    const int8_t sine_table[256] = {
        0,    3,    6,    9,    12,   15,   18,   21,   24,   28,   31,   34,
        37,   40,   43,   46,   48,   51,   54,   57,   60,   63,   65,   68,
        71,   73,   76,   78,   81,   83,   85,   88,   90,   92,   94,   96,
        98,   100,  102,  104,  106,  108,  109,  111,  112,  114,  115,  117,
        118,  119,  120,  121,  122,  123,  124,  124,  125,  126,  126,  127,
        127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  126,  126,
        125,  124,  124,  123,  122,  121,  120,  119,  118,  117,  115,  114,
        112,  111,  109,  108,  106,  104,  102,  100,  98,   96,   94,   92,
        90,   88,   85,   83,   81,   78,   76,   73,   71,   68,   65,   63,
        60,   57,   54,   51,   48,   46,   43,   40,   37,   34,   31,   28,
        24,   21,   18,   15,   12,   9,    6,    3,    0,    -3,   -6,   -9,
        -12,  -15,  -18,  -21,  -24,  -28,  -31,  -34,  -37,  -40,  -43,  -46,
        -48,  -51,  -54,  -57,  -60,  -63,  -65,  -68,  -71,  -73,  -76,  -78,
        -81,  -83,  -85,  -88,  -90,  -92,  -94,  -96,  -98,  -100, -102, -104,
        -106, -108, -109, -111, -112, -114, -115, -117, -118, -119, -120, -121,
        -122, -123, -124, -124, -125, -126, -126, -127, -127, -127, -127, -127,
        -127, -127, -127, -127, -127, -127, -126, -126, -125, -124, -124, -123,
        -122, -121, -120, -119, -118, -117, -115, -114, -112, -111, -109, -108,
        -106, -104, -102, -100, -98,  -96,  -94,  -92,  -90,  -88,  -85,  -83,
        -81,  -78,  -76,  -73,  -71,  -68,  -65,  -63,  -60,  -57,  -54,  -51,
        -48,  -46,  -43,  -40,  -37,  -34,  -31,  -28,  -24,  -21,  -18,  -15,
        -12,  -9,   -6,   -3,
    };

    #define SIN(angle)  (sine_table[(uint8_t)angle])
    #define COS(angle)  (sine_table[(uint8_t)((uint8_t)angle + (256u / 4u))]) // relies on unsigned byte wraparound (0 - 255)

#else

    // Reduced resolution sine table, seems OK
    const int8_t sine_table[128] = {
        0,    6,    12,   18,   24,   31,   37,   43,   48,   54,   60,   65,
        71,   76,   81,   85,   90,   94,   98,   102,  106,  109,  112,  115,
        118,  120,  122,  124,  125,  126,  127,  127,  127,  127,  127,  126,
        125,  124,  122,  120,  118,  115,  112,  109,  106,  102,  98,   94,
        90,   85,   81,   76,   71,   65,   60,   54,   48,   43,   37,   31,
        24,   18,   12,   6,    0,    -6,   -12,  -18,  -24,  -31,  -37,  -43,
        -48,  -54,  -60,  -65,  -71,  -76,  -81,  -85,  -90,  -94,  -98,  -102,
        -106, -109, -112, -115, -118, -120, -122, -124, -125, -126, -127, -127,
        -127, -127, -127, -126, -125, -124, -122, -120, -118, -115, -112, -109,
        -106, -102, -98,  -94,  -90,  -85,  -81,  -76,  -71,  -65,  -60,  -54,
        -48,  -43,  -37,  -31,  -24,  -18,  -12,  -6,
    };

    #define SIN(angle)  (sine_table[(uint8_t)((uint8_t)angle >> 1)])
    #define COS(angle)  (sine_table[((uint8_t)((uint8_t)(angle >> 1) + ((256u >> 1) / 4u)) & 0x7fu)]) // Wrap around at 7 bits (0-127)

#endif

// manually built from image, 8x16 sprite mode = 2 tiles per, E repeats
const uint8_t gameover_sprite_map[] = {0u,  2u, 4u,  6u,  // "GAME"
                                       8u, 10u, 6u, 12u}; // "OVER"


// Load graphics for gameover screen
//
// Uses same oam range as gameplay sprites (doesn't get called till no more sprites on screen)
// Does not clear score sprites
static void gameover_screen_initgfx(uint8_t spr_next_free_tile) {

    // Make sure sprites are hidden (may be redundant)
    hide_sprites_range(GAME_OVER_SPR_COUNT_START, MAX_HARDWARE_SPRITES);

    // Set sprite tile ID's and clear properties
    // TODO: use a map for this instead so E can be de-duplicated?
    uint8_t tile_counter = 0;
    for (uint8_t c = GAME_OVER_SPR_COUNT_START; c < GAME_OVER_SPR_COUNT_MAX; c++) {
        shadow_OAM[c].tile = spr_next_free_tile + gameover_sprite_map[tile_counter++];
        shadow_OAM[c].prop = 0;
    }

    for (uint8_t c = 0; c < game_over_TILE_COUNT; c++) {
        // Load letter tiles at bottom of the screen one at a time to reduce
        // visual glitching + ensure safe vram writes while the map FX is running
        wait_in_halt_to_scanline(144u - 10u);
        __critical {
            set_sprite_data(spr_next_free_tile + c, 1u, game_over_tiles + (c * BYTES_PER_TILE));
        }
    }
}


uint8_t rotate_speed_mask[] = {0x03, 0x01, 0x00, 0x00};

// Show gameover message (while BG continues to run) until user presses a button
void gameover_screen_show(uint8_t spr_next_free_tile) {

    static uint16_t base_angle;
    base_angle = GAME_OVER_START_ANGLE;
    uint8_t  x_pos      = GAME_OVER_X_START;
    uint8_t  y_pos      = GAME_OVER_Y_START;
    uint8_t  spr_count  = GAME_OVER_SPR_COUNT_START;
    uint8_t  hold_time  = 0;

    gameover_screen_initgfx(spr_next_free_tile);

    while (1) {


        #ifndef DMG_MGB_LESS_BLUR_STYLE
            if (y_pos != (GAME_OVER_Y_END)) {
                y_pos+= GAME_OVER_Y_DROP_SPEED;
            }
            base_angle += GAME_OVER_ROTATE_SPEED_NORM;
        #else
            if (y_pos != (GAME_OVER_Y_END)) {
                y_pos+= GAME_OVER_Y_DROP_SPEED;
                base_angle += GAME_OVER_ROTATE_SPEED_NORM;
            }
            else {
                // Slow down angle at top of rotation briefly for better DMG readbility
                //
                // Creates a uint8_t based midpoint at the target slowdown angle
                // Then uses that to create a ramp to/from the midpoint
                uint8_t midpoint_delta = (base_angle >> GAME_OVER_BIT_SHIFT) - GAME_OVER_SLOWDOWN_ANGLE;
                if (midpoint_delta > 127u) midpoint_delta = 255u - midpoint_delta;  // 127u & 255u as uint8_t midpoint and max
                midpoint_delta >>= GAME_OVER_ROTATE_MIDPOINT_BITSHIFT;

                // Suppress slow wiggle/crawl while it's near the stopping point
                if (midpoint_delta == 0) {
                    hold_time++;
                    // Then give angle a bump to start moving again once done waiting
                    if (hold_time > GAME_OVER_MIDPOINT_HOLD_TIME) {
                        hold_time = 0;
                        base_angle += GAME_OVER_ROTATE_SPEED_NORM;
                    }
                } else
                    base_angle += (midpoint_delta >> GAME_OVER_ROTATE_MIDPOINT_BITSHIFT) + GAME_OVER_ROTATE_SPEED_SLOW;
            }
        #endif

        vsync();

        static uint8_t c;
        uint8_t spr_angle = (uint8_t)(base_angle >> GAME_OVER_BIT_SHIFT);
        // Update all rotating sprites based with X,y calculated
        // based on an incrementing angle (from initial rotating angle)
        for (c = GAME_OVER_SPR_COUNT_START; c < spr_count; c++) {
            shadow_OAM[c].x =  (uint8_t)((SIN(spr_angle) * (GAME_OVER_LETTER_RADIUS)) >> 7) + x_pos;
            shadow_OAM[c].y =  (uint8_t)((COS(spr_angle) * (GAME_OVER_LETTER_RADIUS)) >> 7) + y_pos;
            spr_angle -= GAME_OVER_SPR_ANGLE_GAP; // Minus to go counter clockwise for sprite letters
        }

        // Add a letter every couple frames
        if ((sys_time & GAME_OVER_ADD_LETTER_MASK) == 0x00u)
            if (spr_count < GAME_OVER_SPR_COUNT_MAX)
                spr_count++;

        UPDATE_KEYS();
        if (KEY_TICKED(J_ANY))
            break;
    }

    audio_start_fadeout();
    fade_out(FADE_DELAY_FX_RUNNING);

    // Hide all sprites
    hide_sprites_range(0u, MAX_HARDWARE_SPRITES);
}

