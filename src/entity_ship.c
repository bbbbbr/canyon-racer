#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "input.h"

#include "map_fx.h"

#include "entity_ship.h"
#include "../res/sprite_ship.h"



/*
extern const uint8_t * p_scx_addr;
extern const uint8_t scx_table_1[];
extern uint16_t map_lcd_scx_table_start;
int8_t map_offset_x_last = 0;
uint8_t ship_move_hyst = 0;

*/

fixed   ship_x, ship_y;
uint8_t ship_z;
uint8_t ship_sprite_sel;
uint8_t ship_state;
uint8_t ship_counter;
uint8_t ship_sprite_sel;

void entity_ship_init(void) {
    ship_x.w = SHIP_X_INIT;
    ship_y.w = SHIP_Y_INIT;
    ship_z   = SHIP_Z_INIT;
    ship_state = SHIP_STATE_STARTUP;
    ship_counter = SHIP_COUNTER_STARTUP;
}



uint8_t entity_ship_update(uint8_t oam_high_water) {

    ship_sprite_sel = SHIP_SPR_DEFAULT;

    if (ship_state == SHIP_STATE_STARTUP) {
        // TODO: this should just be normal movable, but with flickering and no collision

        if (ship_counter)
            ship_counter--;
        else
            ship_state = SHIP_STATE_PLAYING;

        // Blink ship on/off to ready player
        if ((ship_counter & 0x03u) == 0)
            ship_sprite_sel = SHIP_SPR_DEFAULT;
        else
            ship_sprite_sel = SHIP_SPR_NONE;
    }
    else if (ship_state == SHIP_STATE_CRASHED) {

        // If crashed, render explosion then restart
        if (ship_counter)
            ship_counter--;
        else
            ship_state = SHIP_STATE_DO_RESET;

        // Select crash frame
        ship_sprite_sel = (ship_counter >> (SHIP_COUNTER_CRASH_BITSHIFT)) + SHIP_SPR_CRASH_MIN;
    }
    else {

        // Implied: SHIP_STATE_PLAYING

        // ==== Handle Input ====

        // Horizontal movement
        if (KEY_PRESSED(J_LEFT)) {
            if (ship_x.w > SHIP_X_MIN) {
                ship_x.w -= SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel > SHIP_SPR_LEFT;
        }
        else if (KEY_PRESSED(J_RIGHT)) {
            if (ship_x.w < SHIP_X_MAX) {
                ship_x.w += SHIP_MOVE_AMT_X;
            }
            ship_sprite_sel > SHIP_SPR_RIGHT;
        }

        // Vertical Movement
        if (KEY_PRESSED(J_UP)) {
            if (ship_y.w > SHIP_Y_MIN) {
                ship_y.w -= SHIP_MOVE_AMT_Y;
            }
        }
        else if (KEY_PRESSED(J_DOWN)) {
            if (ship_y.w < SHIP_Y_MAX) {
                ship_y.w += SHIP_MOVE_AMT_Y;
            }
        }


        // ==== Check for collision ====
        // Hitbox is forgiving
        // Controlled by metasprite info (set during conversion)

        #define SHIP_HITBOX_X_LEFT   ((sprite_ship_WIDTH - sprite_ship_PIVOT_W) / 2u)
        #define SHIP_HITBOX_X_RIGHT  ((sprite_ship_PIVOT_W) + ((sprite_ship_WIDTH - sprite_ship_PIVOT_W) / 2u))
                    // #define SHIP_HITBOX_X_HALF 4u
                    // #define SHIP_HITBOX_X      8u

        // ship_x & ship_y are in upper left of metasprite hit box (0, 8) of metasprite

        // Check walls

        // Use the ship Y position to index into the canyon background X offset table (one row per scanline)
        // p_scx_table_base gets updated at the start of every frame

        // ship_y.h = 144u / 2u;
        uint8_t ship_canyon_left_x = CANYON_LEFT_X_BASE - p_scx_table_base[ship_y.h];

        // ship_x.h = ship_canyon_left_x;

        if ((ship_x.h + SHIP_HITBOX_X_LEFT) < ship_canyon_left_x) {
            ship_state = SHIP_STATE_CRASHED;
            ship_counter = SHIP_COUNTER_CRASH;
        }
        else if ((ship_x.h + SHIP_HITBOX_X_RIGHT) > (ship_canyon_left_x + CANYON_WIDTH)) {
            ship_state = SHIP_STATE_CRASHED;
            ship_counter = SHIP_COUNTER_CRASH;
        }

    }

    // ==== Update Sprite ====

    // if (KEY_PRESSED(J_B)) {
    //     entity_shots_add(ship_x.h, ship_y.h);
    // }

    if (ship_sprite_sel != SHIP_SPR_NONE)
        oam_high_water += move_metasprite(sprite_ship_metasprites[ship_sprite_sel],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, ship_x.h, ship_y.h);

    // TODO: FIXME
    if (ship_state == SHIP_STATE_DO_RESET) {
        entity_ship_init();
    }

    return (oam_high_water);
}


