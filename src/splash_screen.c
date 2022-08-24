#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

// Graphics
#include "../res/splash_logo.h"

#include "input.h"
#include "common.h"
#include "fade.h"

#include "map_fx.h"


#define WIN_X_OFFSET   7u // GameBoy Window hardware X offset
#define WIN_X_FINAL    0u // Hide almost one column off-screen
#define WIN_X_INITIAL  0u // Same as final, but gets changed almost instantly


static void splash_init(uint8_t bg_next_free_tile) {

    set_bkg_data(bg_next_free_tile, splash_logo_TILE_COUNT, splash_logo_tiles);

    // Splash logo goes on the Window so it can overlay on top of the scrolling background
    set_win_based_tiles(0,0, splash_logo_WIDTH / splash_logo_TILE_W, splash_logo_HEIGHT / splash_logo_TILE_H, splash_logo_map, bg_next_free_tile);


    // TODO: Play Intro Splash Music?

    // Window start off-screen
    WY_REG = (SCREENHEIGHT + 1u);
    WX_REG = WIN_X_INITIAL;
    SHOW_WIN;
}

static void window_move_up_with_shake(void) {

    // Window destination
    uint8_t scroll_amt;
    uint8_t win_y_moveto = (SCREENHEIGHT) - (splash_logo_HEIGHT);
    uint8_t win_y_delta;

    // Scroll window into view (with a small ease-in)
    while (WY_REG > win_y_moveto) {
        win_y_delta = WY_REG - win_y_moveto;

        if (win_y_delta > 20u)
            scroll_amt =  4u;
        else if (win_y_delta > 10u)
            scroll_amt = 2u;
        else
            scroll_amt = 1u;

        const uint8_t scx_shake_offset[] = {7u, 6u, 5u, 4u, 3u, 2u};
        // This one looks a little better than the version below, against expectations
        WX_REG = WIN_X_OFFSET + ((DIV_REG >> 4) & (0x1u << (win_y_delta >> 5))) - scx_shake_offset[(win_y_delta >> 5)];
        // const uint8_t scx_shake_mask[] = {0x01u, 0x03u, 0x05u, 0x07u, 0x0Fu};
        // WX_REG = WIN_X_OFFSET + ((DIV_REG >> 3) & scx_shake_mask[(win_y_delta >> 4)]);

        WY_REG -= scroll_amt;
        wait_vbl_done();
    }
    // Make sure Window ends up at desired location
    WX_REG = WIN_X_FINAL;
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
            scroll_amt = 4u;

        WY_REG += scroll_amt;
        wait_vbl_done();
    }
}


// Expects Sprites to be turned off
void splash_intro_run(uint8_t bg_next_free_tile) {

    splash_init(bg_next_free_tile);

    fade_in(FADE_DELAY_FX_RUNNING);
    window_move_up_with_shake();

    // Idle until user presses any button
    waitpadticked_lowcpu(J_ANY);

    window_move_down();
    fade_out(FADE_DELAY_FX_RUNNING);

    HIDE_WIN;
}

