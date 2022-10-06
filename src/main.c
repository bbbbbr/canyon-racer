
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

#include "intro_credits.h"
#include "splash_screen.h"
#include "gameplay.h"
#include "gameover_screen.h"
#include "map_fx.h"


uint8_t bg_next_free_tile;
uint8_t spr_next_free_tile;
uint8_t game_state;


void init_gfx(void) {

    fade_out(FADE_DELAY_NORM);

    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;

    DISPLAY_ON;

    // TODO: Move into main()
    // Fades in, shows credits, fades out
    #ifndef DEBUG_SKIP_INTRO_CREDITS
        intro_credits_show();
    #endif

    // TODO: move to gameplay_init_gfx()
    bg_next_free_tile = 0u;
    spr_next_free_tile = 0u;
    bg_next_free_tile = init_gfx_bg_mapfx(bg_next_free_tile);
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

    // Call this BEFORE mapfx_isr_install() to ensure audio is configured before it's VBL starts
    audio_init();

    mapfx_set_intro();
}



void main() {

    init();
    game_state = GAME_STATE_SHOW_INTRO;

    while (1) {

        switch (game_state) {

            case GAME_STATE_SHOW_INTRO:
                HIDE_SPRITES;
                // Expects: screen palettes faded-out

                // Start up effect
                // In Splash screen music is run manually, not by vbl
                mapfx_isr_install(MAPFX_AUDIO_VBL_NO);
                splash_intro_run(bg_next_free_tile);
                // Remove effect from running after fade-out
                mapfx_isr_deinstall();
                game_state = GAME_STATE_START_GAME;
                break;

            case GAME_STATE_START_GAME:
                // Expects: screen palettes faded-out

                mapfx_isr_install(MAPFX_AUDIO_VBL_YES);
                spr_next_free_tile = 0u; // TODO: fix this up
                spr_next_free_tile = init_gfx_sprites_gameplay(spr_next_free_tile);
                SHOW_SPRITES;
                gameplay_prestart();
                #ifdef DEBUG_SOUND_TEST
                    audio_music_set(song_test_counter);
                #else
                    audio_music_set(MUSIC_GAMEPLAY);
                #endif
                audio_music_unpause();
                game_state = GAME_STATE_RUNNING;
                break;

            case GAME_STATE_RUNNING:
                gameplay_run(spr_next_free_tile);
                game_state = GAME_STATE_OVER;
                break;

            case GAME_STATE_OVER:
                // TODO: audio_music_set(MUSIC_GAMEOVER_SONG);
                gameover_screen_show(bg_next_free_tile);
                game_state = GAME_STATE_SHOW_INTRO;
                // Audio and Screen are faded at this point
                delay_lowcpu( SECONDS_IN_FRAMES(GAME_OVER_WAIT_SECONDS) );
                // Remove effect from running after fade-out
                mapfx_isr_deinstall();
                break;
        }
    }
}
