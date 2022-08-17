#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

// Graphics
#include "../res/splash_logo.h"

#include "input.h"
#include "common.h"

#include "map_fx.h"


#define WIN_X_OFFSET 7u // GameBoy Window hardware X offset

static void splash_init(uint8_t bg_next_free_tile) {


    set_bkg_data(bg_next_free_tile, splash_logo_TILE_COUNT, splash_logo_tiles);

    // Splash logo goes on the Window so it can overlay on top of the scrolling background
    set_win_based_tiles(0,0, splash_logo_WIDTH / splash_logo_TILE_W, splash_logo_HEIGHT / splash_logo_TILE_H, splash_logo_map, bg_next_free_tile);

    // HIDE_SPRITES;
    // TODO: Fade in

    // TODO: Play Intro Splash Music?

    // Window start off-screen
    WY_REG = (SCREENHEIGHT + 1u);

    // move_win(0 + WIN_X_OFFSET, (SCREENHEIGHT) - (splash_logo_HEIGHT));
    SHOW_WIN;

    // Window destination
    uint8_t scroll_amt;
    uint8_t win_y_moveto = (SCREENHEIGHT) - (splash_logo_HEIGHT);

    // Scroll window into view (with a small ease-in)
    while (WY_REG > win_y_moveto) {
        if ((WY_REG - win_y_moveto) > 20u)
            scroll_amt = 4u; // 6u;
        else if ((WY_REG - win_y_moveto) > 10u)
            scroll_amt = 2u;
        else
            scroll_amt = 1u;

        WY_REG -= scroll_amt;
        wait_vbl_done();
    }

}


static void window_move_down(void) {

    // Window destination
    uint8_t scroll_amt;
    uint8_t win_y_moveto = (SCREENHEIGHT + 1u);

    // Scroll window out of view (with a small ease-out)
    while (WY_REG < win_y_moveto) {
        if ((WY_REG - win_y_moveto) <= 8u)
            scroll_amt = 1u;
        else if ((WY_REG - win_y_moveto) <= 18u)
            scroll_amt = 2u;
        else
            scroll_amt = 4u; // 6u;

        WY_REG += scroll_amt;
        wait_vbl_done();
    }
}


void splash_intro_run(uint8_t bg_next_free_tile) {

    splash_init(bg_next_free_tile);
    waitpadticked_lowcpu(J_ANY);

    window_move_down();

    HIDE_WIN;
    SHOW_SPRITES;
}

