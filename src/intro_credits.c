#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"

#include "../res/intro_credits_data.h"

void intro_credits_show(void) {

    // TODO: OPTIONAL: fancier tile load sequence?

    // set_1bpp_colors(DMG_WHITE, DMG_BLACK);
    set_1bpp_colors(DMG_BLACK, DMG_WHITE);

    set_bkg_1bpp_data(0, intro_credits_data_TILE_COUNT, intro_credits_data_tiles);
    set_bkg_tiles(0,0,
                  (intro_credits_data_WIDTH / intro_credits_data_TILE_W),
                  (intro_credits_data_HEIGHT / intro_credits_data_TILE_H),
                  intro_credits_data_map);


    delay(sizeof(intro_credits_data_map));
    fade_in(FADE_DELAY_INTRO);
    delay(1750);
    fade_out(FADE_DELAY_INTRO);
}