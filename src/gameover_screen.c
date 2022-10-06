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

// TODO: regenerate by hand?
// Strip every other out?
const int8_t sine_wave[256] = {
    0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,
    63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,
    106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,
    124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,
    126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,
    111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,
    73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,
    16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,
    -40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,
    -81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,
    -109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,
    -123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,
    -127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,
    -120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,
    -102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,
    -68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,
    -25,-22,-19,-16,-12,-9,-6,-3
};


#define SIN(angle)  (sine_wave[(uint8_t)(angle)])
#define COS(angle)  (sine_wave[(uint8_t)((uint8_t)(angle) + (256u / 4u))])

// manually built from image, 8x16 sprite mode = 2 tiles per, E repeats
const uint8_t gameover_sprite_map[] = {0u,  2u, 4u,  6u,   // "GAME"
                                       8u, 10u, 6u, 12u}; // "OVER"


// Load graphics for gameover screen
//
// Uses same oam range as gameplay sprites (doesn't get called till no more sprites on screen)
// Does not clear score sprites
static void gameover_screen_initgfx(uint8_t spr_next_free_tile) {

    // Make sure sprites are hidden (may be redundant)
    hide_sprites_range(GAME_OVER_SPR_COUNT_START, 40u);

    // Set sprite tile ID's and clear properties
    // TODO: use a map for this instead so E can be de-duplicated?
    uint8_t tile_counter = 0;
    for (uint8_t c = GAME_OVER_SPR_COUNT_START; c < GAME_OVER_SPR_COUNT_MAX; c++) {
        shadow_OAM[c].tile = spr_next_free_tile + gameover_sprite_map[tile_counter++];
        shadow_OAM[c].prop = 0;
    }

    // Load the letter tiles
    set_sprite_data(spr_next_free_tile, game_over_TILE_COUNT, game_over_tiles);

}


// Show gameover message (while BG continues to run) until user presses a button
void gameover_screen_show(uint8_t spr_next_free_tile) {

    uint16_t base_angle = GAME_OVER_START_ANGLE;
    uint8_t  x_pos      = GAME_OVER_X_START;
    uint8_t  y_pos      = GAME_OVER_Y_START;
    uint8_t  spr_count  = GAME_OVER_SPR_COUNT_START;

    gameover_screen_initgfx(spr_next_free_tile);

    while (1) {


        if (y_pos != (GAME_OVER_Y_END)) {
            y_pos++;
            base_angle += GAME_OVER_ROTATE_SPEED_NORM;
        }
        else {
        // TODO: slow down angle at certain speeds
            base_angle += GAME_OVER_ROTATE_SPEED_NORM;
        }

        wait_vbl_done();

        static uint8_t c;
        uint16_t spr_angle = base_angle >> GAME_OVER_BIT_SHIFT;
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
    hide_sprites_range(0u, 40u);
}

