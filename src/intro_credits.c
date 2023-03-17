#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gb/gbdecompress.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"

#include "ram_lcd_isr.h"
#include "map_fx.h"
#include "intro_credits.h"


#include "../res/intro_credits_data.h"
#include "../res/intro_credits_map_comp.h"
#include "../res/intro_credits_tiles_comp.h"

volatile uint8_t scroll_x_amount;
volatile uint8_t effect_y_line;

static void intro_credits_effect_run(void);
static void lcd_wave_effect_isr(void) __interrupt;
static void lcd_wave_effect_isr__end(void);

static void intro_credits_effect_init(void);
static void intro_credits_effect_run(void);


// Show the intro credits screen
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


    // Music will be set up, but doesn't run until Music VBL ISR is installed
    audio_music_set(MUSIC_CREDITS);
    audio_music_unpause();

    // Installs LCD and Music VBL ISRs
    intro_credits_effect_init();

    // Fades in and then runs until effect completes
    fade_in(FADE_DELAY_INTRO);
    intro_credits_effect_run();

    delay(800);
    fade_out(FADE_DELAY_INTRO);

    // Then de-install audio vbl isr
    __critical {
        remove_VBL(audio_vbl_isr);
    }
}



// VBL ISR portion of effect
// Resets counters and slowly increments starting scanline
static void intro_credits_effect_vbl_isr() {

    // Reset scroll registers to zero at the start of every frame
    scroll_x_amount = 0u;
    SCY_REG = 0u;
    SCX_REG = 0u;

    // Increment effect start line downward by +1 scanline every other frame
    if (sys_time & 0x01)
        effect_y_line++;

    // Clear any pending LCD/STAT interrupt to prevent Map Effect ISR running
    // on scanlines > 143 which aren't visible. Saves a little cpu time and minor glitches.
    IF_REG = IF_REG & ~LCD_IFLAG;
}

#define INTRO_EFFECT_ASM_VERSION

// LCD ISR Horizontal wave reveal effect
//
// Every line after [effect_y_line] scroll left/right by
// by the num lines since [effect_y_line] * 4, alternate
// left/right every other line.
#ifndef INTRO_EFFECT_ASM_VERSION
    static void intro_credits_effect_lcd_isr(void) __interrupt {

        // Hide trailing lines after EFFECT_TRANSITION_HEIGHT lines below effect_y_line
        if (LY_REG > (effect_y_line + EFFECT_TRANSITION_HEIGHT)) {
            SCY_REG = -LY_REG; // Scroll to empty line at top of screen
        }
        else if (LY_REG > effect_y_line) {
            scroll_x_amount += EFFECT_SCX_AMOUNT;

            if (LY_REG & 0x01U) SCX_REG = scroll_x_amount;
            else SCX_REG = 255U - scroll_x_amount;
         }
    }
#else
    // ASM version ensure it has no absolute jumps and can be relocated
    static void intro_credits_effect_lcd_isr(void) __interrupt __naked {
        __asm \

        push af
        push hl

        ld  a, (#_effect_y_line)
        ld  l, a
        ldh a, (_LY_REG + 0)
        ld  h, a   // ; Save LY_REG in H

        sub a, l
        jr  c, 3$  // ; Exit
        // ; if (LY_REG > effect_y_line) {
            // ; A has (LY_REG - effect_y_line) from above
            sub a, #0x20
            jr  c, 1$ // ; Skip hide trailing effect and do visible effect area
            // ; if (LY_REG > (effect_y_line + EFFECT_TRANSITION_HEIGHT)) {

                // ; Hide trailing lines after EFFECT_TRANSITION_HEIGHT lines below effect_y_line
                // ; SCY_REG = -LY_REG; // Scroll to empty line at top of screen
                ld a, h  // ; H has LY_REG
                cpl
                inc a
                ldh (_SCY_REG + 0), a
                jr 3$ // ; Exit

            // ; } else {
            1$:
                ld a, h // ; A now has LY_REG

                // ; scroll_x_amount += EFFECT_SCX_AMOUNT (0x04);
                ld  hl, #_scroll_x_amount
                inc (hl)
                inc (hl)
                inc (hl)
                inc (hl)

                bit 0, a  // ; A has LY_REG
                ld a, (hl)  // ; Reload scroll_x_amount
                jr nz, 2$ // ; Skip negate scroll_x_amount
                    // ; src/intro_credits.c:116: else SCX_REG = -scroll_x_amount;
                    cpl
                    inc a
                2$:
                // ; src/intro_credits.c:115: if (LY_REG & 0x01U) SCX_REG = scroll_x_amount;
                ldh (_SCX_REG + 0), a
            // ; }
        // ; }
        3$:
        pop hl
        pop af
        reti

        __endasm;
    }
#endif // #ifdef INTRO_EFFECT_ASM_VERSION

// For calculating Effect ISR function length
static void intro_credits_effect_lcd_isr__end(void) __naked { }


// Initialize the LCD Effect and install the ISRs (LCD + Music)
static void intro_credits_effect_init(void) {

    scroll_x_amount = 0;
    effect_y_line = EFFECT_START_Y;

    mapfx_isr_lcd_disable();
    // Copy the Effect LCD ISR into the RAM buffer the interrupt executes from
    copy_lcd_isr_to_isr_ram((void *)&intro_credits_effect_lcd_isr, (void *)&intro_credits_effect_lcd_isr__end);

    __critical {
        #ifndef INTRO_EFFECT_ASM_VERSION
            // Fire interrupt at start of OAM SCAN (Mode 2) right before rendering
            STAT_REG = STATF_MODE10;
        #else
            // Fire interrupt at start of HBlank (Mode 0) right before rendering
            STAT_REG = STATF_MODE00;
        #endif
        add_VBL(intro_credits_effect_vbl_isr); // Resets scroll counters each frame + allows trailing music ISR to be interrupted
        add_VBL(audio_vbl_isr);       // Make sure music is installed after Effect ISR
    }

    // Try to wait until just after the start of the next frame before enabling effect
    wait_vbl_done();
    mapfx_isr_lcd_enable();
}


// Runs until effect completes, turns off effect and deinstalls vbl handler when done
// Loops until effect_y_line reaches the bottom of the screen
static void intro_credits_effect_run(void) {

    volatile uint8_t cur_line;  // Compiler doesn't like break inside a critical section :)

    while (1) {
        __critical {
            cur_line = effect_y_line;
        }
        if (cur_line > EFFECT_Y_LINE_MAX)
            break;
        wait_vbl_done();
    }

    mapfx_isr_lcd_disable();
    __critical {
        remove_VBL(intro_credits_effect_vbl_isr); // Resets scroll counters each frame
    }
}
