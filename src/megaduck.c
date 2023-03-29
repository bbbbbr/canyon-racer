// Copyright 2023 (c) bbbbbr
//
// This software is licensed under:
//
// CC-BY-NC-SA: Attribution Non-Commercial Share Alike Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International License
// See: http://creativecommons.org/licenses/by-nc-sa/4.0/
//
// For the purposes of this project "Share Alike" shall also include
// distribution of the source code and any changes to it.

#include <gbdk/platform.h>
#include <gbdk/bcd.h>
#include <stdint.h>
#include <stdbool.h>

#include <gb/gbdecompress.h>

#include "input.h"
#include "common.h"
#include "fade.h"


#ifdef MEGADUCK

    #include "../res/megaduck_on_ap_data.h"
    #include "../res/megaduck_on_ap_map_comp.h"
    #include "../res/megaduck_on_ap_tiles_comp.h"


    bool is_analogue_pocket_megaduck = false;

    const uint8_t WAVE_RAM_AP_CORE_DMG_AND_DUCK[] = {
        0x70, 0xB0, 0xFC, 0xD8,
        0x88, 0xD3, 0x12, 0x60,
        0x62, 0x77, 0x3B, 0xFE,
        0x6D, 0xA9, 0x76, 0xDB,};

    uint8_t wave_ram_save[ARRAY_LEN(WAVE_RAM_AP_CORE_DMG_AND_DUCK)];

    static void save_wave_ram(void) {
        // Turn on sound hardware
        NR52_REG = 0x80;

        // Read Channel 3 Wave RAM before anything sets them
        // In CGB they may be initialized, others semi-random
        for (uint8_t c = 0u; c < ARRAY_LEN(WAVE_RAM_AP_CORE_DMG_AND_DUCK); c++) {
            wave_ram_save[c] = AUD3WAVE[c];
        }
    }

    static bool check_fingerprint(const uint8_t * p_tocheck, const uint8_t * p_reference) {
        for (uint8_t c = 0u; c < ARRAY_LEN(WAVE_RAM_AP_CORE_DMG_AND_DUCK); c++) {
            if (*p_tocheck != *p_reference)
                return false;
            p_tocheck++;
            p_reference++;
        }
        return true;
    }

    // This should run before other things that touch the sound hardware
    void megaduck_check_is_analogue_pocket(void) {

        save_wave_ram();
        is_analogue_pocket_megaduck = check_fingerprint(wave_ram_save, WAVE_RAM_AP_CORE_DMG_AND_DUCK);
    }


    void megaduck_show_warning_if_analogue_pocket_core(void) {


        set_1bpp_colors(DMG_BLACK, DMG_WHITE);

        // == Compressed 1bpp assets version ==
        gb_decompress(megaduck_on_ap_tiles_comp, decomp_buf);
        set_bkg_1bpp_data(0, megaduck_on_ap_data_TILE_COUNT, decomp_buf);

        // Clear screen (may not be cleared on Mega Duck)
        fill_bkg_rect(0u,0u, DEVICE_SCREEN_WIDTH, DEVICE_SCREEN_HEIGHT, 0u);

        if (is_analogue_pocket_megaduck) {

            gb_decompress(megaduck_on_ap_map_comp, decomp_buf);
            set_bkg_tiles((DEVICE_SCREEN_WIDTH - (megaduck_on_ap_data_WIDTH / megaduck_on_ap_data_TILE_W)) / 2u,
                          (DEVICE_SCREEN_HEIGHT - (megaduck_on_ap_data_HEIGHT / megaduck_on_ap_data_TILE_H)) / 2u,
                          (megaduck_on_ap_data_WIDTH / megaduck_on_ap_data_TILE_W),
                          (megaduck_on_ap_data_HEIGHT / megaduck_on_ap_data_TILE_H),
                          decomp_buf);

            // Fades in and then runs until effect completes
            fade_in(FADE_DELAY_INTRO);
            waitpadticked_lowcpu(J_ANY);
            fade_out(FADE_DELAY_INTRO);
        }
    }

#endif