#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include <gb/gbdecompress.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"


#include "../res/intro_credits_data.h"
#include "../res/intro_credits_map_comp.h"
#include "../res/intro_credits_tiles_comp.h"


void intro_credits_show(void) {

    set_1bpp_colors(DMG_BLACK, DMG_WHITE);

    // TODO: More interesting load sequence later if possible

    // == Compressed 1bpp assets version ==
    // 1bpp tiles and map versions of gb_decompress.* would be nice
    gb_decompress(intro_credits_tiles_comp, decomp_buf);
    set_bkg_1bpp_data(0, intro_credits_data_TILE_COUNT, decomp_buf);

    gb_decompress(intro_credits_map_comp, decomp_buf);
    set_bkg_tiles(0,0,
                  (intro_credits_data_WIDTH / intro_credits_data_TILE_W),
                  (intro_credits_data_HEIGHT / intro_credits_data_TILE_H),
                  decomp_buf);

    // == Non-compressed 1bpp assets version ==
    // set_bkg_1bpp_data(0, intro_credits_data_TILE_COUNT, intro_credits_data_tiles);
    // set_bkg_tiles(0,0,
    //               (intro_credits_data_WIDTH / intro_credits_data_TILE_W),
    //               (intro_credits_data_HEIGHT / intro_credits_data_TILE_H),
    //               intro_credits_data_map);



    audio_music_set(MUSIC_CREDITS);
    audio_music_unpause();

    // Temporarily install the audio vbl isr
    __critical {
        add_VBL(audio_vbl_isr);
    }

    delay(120);
    fade_in(FADE_DELAY_INTRO);
    delay(1750);
    fade_out(FADE_DELAY_INTRO);

    // Then de-install audio vbl isr
    __critical {
        remove_VBL(audio_vbl_isr);
    }

}