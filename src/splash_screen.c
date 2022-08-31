#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

// Graphics
#include "../res/splash_logo.h"
#include "../res/tiles_font_nums_bg.h"

#include "input.h"
#include "common.h"
#include "fade.h"
#include "score.h"

#include "map_fx.h"

#include "splash_screen.h"

#define WIN_X_OFFSET   7u // GameBoy Window hardware X offset
#define WIN_X_FINAL    0u // Hide almost one column off-screen
#define WIN_X_INITIAL  0u // Same as final, but gets changed almost instantly

#define WIN_SPEED_SLOW 1u
#define WIN_SPEED_MED  2u
#define WIN_SPEED_FAST 4u


static void splash_init(uint8_t bg_next_free_tile) {

    mapfx_set_intro();
    set_bkg_data(bg_next_free_tile, splash_logo_TILE_COUNT, splash_logo_tiles);

    // Splash logo goes on the Window so it can overlay on top of the scrolling background
    set_win_based_tiles(SPLASH_LOGO_WIN_X, SPLASH_LOGO_WIN_Y,
                        splash_logo_WIDTH / splash_logo_TILE_W, splash_logo_HEIGHT / splash_logo_TILE_H,
                        splash_logo_map, bg_next_free_tile);

    bg_next_free_tile += splash_logo_TILE_COUNT;

    // Load Splash BG Font Num tiles and render score
    set_bkg_data(bg_next_free_tile, tiles_font_nums_bg_TILE_COUNT, tiles_font_nums_bg_tiles);
    hi_score_render(get_win_xy_addr(SPLASH_LOGO_WIN_SCORE_X, SPLASH_LOGO_WIN_SCORE_Y), bg_next_free_tile);

    // TODO: Play Intro Splash Music?

    // Window start off-screen
    WY_REG = (SCREENHEIGHT + 1u);
    WX_REG = WIN_X_INITIAL;
    SHOW_WIN;
}

const uint8_t scx_shake_offset[] = {7u, 6u, 5u, 4u, 3u, 2u};


#define WIN_MOVE_DIR_DOWN 0
#define WIN_MOVE_DIR_UP   1
static void window_move_with_shake(uint8_t win_y_moveto, uint8_t move_dir) {

    // Window destination
    uint8_t scroll_amt;
    uint8_t win_y_delta;

    // Scroll window into view (with a small ease-in)
    while (1) {
        win_y_delta = WY_REG - win_y_moveto;

        if (win_y_delta > 20u)
            scroll_amt =  WIN_SPEED_FAST;
        else if (win_y_delta > 10u)
            scroll_amt = WIN_SPEED_MED;
        else
            scroll_amt = WIN_SPEED_SLOW;

        if (move_dir == WIN_MOVE_DIR_UP) {
            // Give the window progressively smaller X Axis shakes as it
            // approaches the target Y location (scrolling in from the bottom).
            //
            // To prevent the background from showing through during the shakes:
            // * There is a 8 pixel (1 tile) margin on the left side
            // * The Window left edge gradually moves left as the shakes diminish
            //   to ensure it ends up with the logo properly aligned and the
            //   left margin hidden.
            //
            WX_REG = WIN_X_OFFSET + ((DIV_REG >> 4) & (0x1u << (win_y_delta >> 5))) - scx_shake_offset[(win_y_delta >> 5)];
            WY_REG -= scroll_amt;
            if (WY_REG <= win_y_moveto) break;
        }
        else {
            // Implied: WIN_MOVE_DIR_DOWN
            WY_REG += scroll_amt;
            if (WY_REG >= win_y_moveto) break;
        }
        wait_vbl_done();
    }
    // Make sure Window ends up at desired location
    WX_REG = WIN_X_FINAL;
}



// Expects Sprites to be turned off
void splash_intro_run(uint8_t bg_next_free_tile) {

    splash_init(bg_next_free_tile);

    fade_in(FADE_DELAY_FX_RUNNING);
    window_move_with_shake(((SCREENHEIGHT) - (splash_logo_HEIGHT)), WIN_MOVE_DIR_UP);


    // Idle until user presses any button
    waitpadticked_lowcpu(J_ANY);

    window_move_with_shake(((SCREENHEIGHT) + 1u), WIN_MOVE_DIR_DOWN);
    fade_out(FADE_DELAY_FX_RUNNING);

    HIDE_WIN;
}

