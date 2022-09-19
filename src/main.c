
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"
#include "stats.h"

#include "splash_screen.h"
#include "gameplay.h"
#include "map_fx.h"


uint8_t bg_next_free_tile;
uint8_t spr_next_free_tile;
uint8_t game_state;


void init_gfx(void) {

    fade_out(FADE_DELAY_NORM);

    bg_next_free_tile = 0u;
    spr_next_free_tile = 0u;

    bg_next_free_tile = init_gfx_bg_mapfx(bg_next_free_tile);

    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;

    DISPLAY_ON;
}


void init(void) {

    #if defined(CART_mbc5)
        // Initialize MBC bank defaults
        // Upper ROM bank to 1, And SRAM/XRAM bank to 0
        SWITCH_ROM_MBC5(1);
        SWITCH_RAM(0);
        DISABLE_RAM_MBC5;
    #endif

    // if (_cpu == CGB_TYPE) {
        // Don't use CGB 2x mode, it throws off timing (unless an additional ISR is implemented)
        // Use 2x CGB speed if available
        // cpu_fast();
    // }

    stats_load();
    init_gfx();

    mapfx_set_intro();
    mapfx_isr_enable();

    audio_init();
}



void main() {

    init();
    game_state = GAME_STATE_SHOW_INTRO;

    while (1) {

        switch (game_state) {

            case GAME_STATE_SHOW_INTRO:
                HIDE_SPRITES;
                // Expects: screen palettes faded-out
                splash_intro_run(bg_next_free_tile);
                game_state = GAME_STATE_START_GAME;
                break;

            case GAME_STATE_START_GAME:
                // Expects: screen palettes faded-out
                spr_next_free_tile = 0u; // TODO: fix this up
                spr_next_free_tile = init_gfx_sprites_gameplay(spr_next_free_tile);
                SHOW_SPRITES;
                gameplay_prestart();
                // TODO: audio_music_set(MUSIC_GAMEPLAY_SONG);
                audio_music_set(MUSIC_DEMO_SONG);
                audio_music_unpause();
                game_state = GAME_STATE_RUNNING;
                break;

            case GAME_STATE_RUNNING:
                gameplay_run();
                game_state = GAME_STATE_OVER;
                break;

            case GAME_STATE_OVER:
                // TODO: ?? audio_music_set(MUSIC_GAMEOVER_SONG);
                gameplay_show_gameover(bg_next_free_tile);
                game_state = GAME_STATE_SHOW_INTRO;
                audio_music_pause();
                break;
        }
    }
}
