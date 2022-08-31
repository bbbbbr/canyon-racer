// stats.c

#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "stats.h"

#include <cartsave.h>


// Quick checksum on all bytes of the stats record except the checksum byte itself
static uint8_t stats_calc_checksum(void) {

    uint8_t * p_save_data = ((uint8_t *)&game_settings) + sizeof(game_settings.save_checksum);
    uint8_t checksum = 0x00u;

    // Calculate checksum for save, skip past checksum (first byte)
    for (uint16_t c = sizeof(game_settings.save_checksum); c < (uint8_t) sizeof(game_settings); c++) {
        checksum += *p_save_data++;
    }

    return checksum;
}


// Initialize settings and stats on power-up, try to load values from cart sram/flash
void stats_load(void) {

    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        // Check signature, reset stats and notify if signature failed
        // It is expected to fail on first power-up
        cartsave_restore_data();

        if ((game_settings.save_check0 != STATS_SIG_CHECK_0) ||
            (game_settings.save_check1 != STATS_SIG_CHECK_1) ||
            (game_settings.save_checksum != stats_calc_checksum() )) {
            stats_reset();
            // options_reset();
        }
    #else
        // (bare 32k) Saveless cart just resets stats instead of loading
        stats_reset();
        // options_reset();
    #endif
}


// Zeros out stat struture and if supported saves the change
void stats_reset(void) {

    // Set all bytes to zero by default
    memset((void *)&game_settings, 0x00u, sizeof(game_settings));

    game_settings.save_check0 = STATS_SIG_CHECK_0;
    game_settings.save_check1 = STATS_SIG_CHECK_1;

    game_settings.hi_score = MAKE_BCD(000000);

    game_settings.save_checksum = stats_calc_checksum();

    // For relevant carts, save the reset stats
    #if defined(CART_31k_1kflash) || defined(CART_mbc5)
        cartsave_save_data();
    #endif
}


// Called after end of a game and before title/splash screen is displayed
void stats_update(BCD latest_score) {


    // Score is stored as packed BCD in a uint32_t, one digit per nybble
    if (latest_score > game_settings.hi_score) {

        game_settings.hi_score = latest_score;

        game_settings.save_checksum = stats_calc_checksum();

        // For relevant carts, save the reset stats
        #if defined(CART_31k_1kflash) || defined(CART_mbc5)
            cartsave_save_data();
        #endif
    }
}
