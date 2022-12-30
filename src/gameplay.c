#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "input.h"
#include "common.h"
#include "fade.h"
#include "audio.h"

#include "map_fx.h"

#include "gameover_screen.h"
#include "gameplay_pause.h"
#include "stats.h"
#include "score.h"
#include "level.h"

#include "entity_ship.h"
#include "entity_obstacles.h"

#include "gameplay.h"

// TODO: make global?
uint8_t oam_high_water;
uint8_t oam_high_water_prev;

static void gameplay_state_save();
static bool gameplay_state_restore();



// Setup before gameplay main loop runs
void gameplay_prestart(void) {

    state.paused = false;
    game_state_count_reset();
    state_restore_display_update();

    // This must be called before mapfx and others
    level_init();

    mapfx_set_gameplay();

    // Random number generator set to a (arbitrary) fixed value
    // at the start of each round so gameplay is deterministic
    initrand(GAMEPLAY_START_RAND_INIT);

    oam_high_water_prev = SPR_ID_MAX;
    score_reset();
    score_update();

    entity_ship_init();
    entity_obstacles_init();

    // Create initial save point
    gameplay_state_save();

    delay(50u); // Short delay before fade-in
    fade_in(FADE_DELAY_FX_RUNNING);
}


static void gameplay_state_save() {
    // Only save state if they haven't used them all up
    if (STATE_RESTORE_COUNT_GET()) {
        game_state_save();
        audio_sfx_play(SFX_PAUSE); // TODO: SFX:State Save OK
    } else
        audio_sfx_play(SFX_PAUSE);  // TODO: SFX:State Save Denied/Fail
}


static bool gameplay_state_restore() {
    // Only restore state if they haven't used them all up
    if (STATE_RESTORE_COUNT_GET()) {
        game_state_restore();
        audio_sfx_play(SFX_PAUSE);  // TODO: SFX:State Restore OK
        audio_music_set(MUSIC_GAMEPLAY);
        audio_music_unpause();
        score_update();
        state_restore_display_update();
        if (state.paused)
            return true;  // Request pause on return
    } else
        audio_sfx_play(SFX_PAUSE);  // TODO: SFX:State Restore Denied/Fail

    return false;
}


// Main game loop
void gameplay_run(uint8_t spr_next_free_tile) {

    while(1) {
        wait_vbl_done();

        UPDATE_KEYS();

        // TODO: consider moving into functions
        switch ((GET_KEYS_TICKED(BUTTON__STATE_SAVE | BUTTON__STATE_RESTORE | BUTTON__PAUSE))) {
            case BUTTON__STATE_SAVE:
                // If still in game play save the state
                // But if crashed translate the "save" to a restore to restore
                if (SHIP_STATE_GET() != SHIP_STATE_CRASHED) gameplay_state_save();
                else {
                    if (gameplay_state_restore())
                        gameplay_pause(spr_next_free_tile, oam_high_water);
                }
                break;

            case BUTTON__STATE_RESTORE:
                if (gameplay_state_restore())
                    gameplay_pause(spr_next_free_tile, oam_high_water);
                break;

            case BUTTON__PAUSE: gameplay_pause(spr_next_free_tile, oam_high_water);
                break;
        }

        // == Sprites ==
        oam_high_water = SPR_ID_FREE_START;

        oam_high_water = entity_obstacles_update(oam_high_water);
        oam_high_water = entity_ship_update(oam_high_water);

        // Only need to clear other sprites if more were used last frame
        // Plus, calling this with prev < current would cause a crash
        if (oam_high_water_prev > oam_high_water) {
            // Hide rest of the hardware sprites. Amount of sprites differ between animation frames.
            //
            // TODO: FIXME: +1 is a workaround for hide_sprites_range needing: End > Start + 1,
            //       otherwise hide_sprites_range() crashes
            hide_sprites_range(oam_high_water, oam_high_water_prev + 1);
        }
        oam_high_water_prev = oam_high_water;


        // If game is over, break out and return to main state select
        if (SHIP_STATE_GET() == SHIP_STATE_GAMEOVER)
            break;

        #ifdef VISUAL_DEBUG_BENCHMARK_MAIN
            // VISUAL BENCHMARK END
            // Debug: Benchmark time left by toggling background on/off
            LCDC_REG &= ~LCDCF_BGON; // Turn BG / WIN OFF
        #endif
    }

    // Game Over: Update high score if applicable
    stats_update(state.score);
}


