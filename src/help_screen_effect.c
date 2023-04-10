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
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

#include "ram_lcd_isr.h"
#include "map_fx.h"


#define EFFECT_HIDE_LINE      10u // Map scanline to use as Blank/Hidden

// The speed the flip reveal effect visible region increases (1 = slow, larger = faster/coarser)
#define EFFECT_DIR_UP          (uint8_t)-4
#define EFFECT_DIR_DOWN        4u
#define EFFECT_DIR_NONE        0u
#define EFFECT_TRUNC_TO_INC(VAL)  (VAL - (VAL % EFFECT_DIR_DOWN)) // Ensures increments and target scanlines are even multiple of increment size

#define EFFECT_CYCLES          3u  // Total number of flips to perform
#define EFFECT_CYCLE_INC_SIZE  (EFFECT_TRUNC_TO_INC((DEVICE_SCREEN_PX_HEIGHT / 2u) / EFFECT_CYCLES))

// Start with: compressed to center line, vertical flip ON, move up
#define EFFECT_INIT_Y          (EFFECT_CYCLE_INC_SIZE * EFFECT_CYCLES) // As close to middle of screen as possible
#define EFFECT_INIT_STOP_Y     (EFFECT_CYCLE_INC_SIZE * (EFFECT_CYCLES - 1u)) // Second cycle step
#define EFFECT_INIT_INC        (EFFECT_DIR_UP)
// To end effect with screen oriented correctly: Odd # of cycles starts NON-FLIPPED, Even # starts FLIPPED
#if (EFFECT_CYCLES & 0x01u)
    #define EFFECT_INIT_VFLIP  (false);
#else
    #define EFFECT_INIT_VFLIP  (true);
#endif

#define CALC_SCY_TO_SHOW_MAP_Y_AT_SCANLINE_N(map_y_to_show, scanline_to_display_at) (map_y_to_show - scanline_to_display_at)

static uint16_t map_y_line_to_display;
static uint16_t map_y_line_inc;

static uint8_t effect_line_inc;
static uint8_t effect_line_stop;
static uint8_t effect_line_stop_next;
static uint8_t effect_vflip;

static uint8_t effect_y_line;
static uint8_t effect_y_line_mirror;

static uint8_t effect_done;


// VBL ISR portion of effect
// Moves effect starting scanline up and down
static void help_effect_vbl_isr(void) {

    // Update the effect start for the new frame
    effect_y_line += effect_line_inc;

    // Handle changes of flip growth direction and orientation
    if (effect_y_line == effect_line_stop_next) {

        if (effect_y_line == 0u) {
            effect_done = true;
            mapfx_isr_lcd_disable();
        }
        else {
            // Flip direction and calculate next Map Y stop line
            if (effect_line_inc == EFFECT_DIR_UP) {
                // Go back to center-ish line
                effect_line_stop_next = EFFECT_INIT_Y;
                effect_line_inc      = EFFECT_DIR_DOWN;
            }
            else {
                // Go back upward
                effect_line_stop     -= EFFECT_CYCLE_INC_SIZE;
                effect_line_stop_next = effect_line_stop;
                effect_line_inc       = EFFECT_DIR_UP;
                effect_vflip          = !effect_vflip; // invert flip render direction
            }
        }
    }

    // Calculate bottom of effect and increment per render scanline
    effect_y_line_mirror = DEVICE_SCREEN_PX_HEIGHT - effect_y_line;
    map_y_line_inc = (DEVICE_SCREEN_PX_HEIGHT << 8) / (uint16_t)(effect_y_line_mirror - effect_y_line);

    // Set up values used by scanline ISR during the frame for the stretched render area
    if (!effect_vflip) {
        // Normal Orientation: Start at first scanline and increment downwards
        map_y_line_to_display = 0u << 8;
    } else {
        // Upside Down Orientation: Start at last scanline and decrement upwards (inverted direction)
        map_y_line_to_display = (DEVICE_SCREEN_PX_HEIGHT - 1u) << 8;
        map_y_line_inc = (uint16_t) -map_y_line_inc;
    }
}


#define INTRO_EFFECT_ASM_VERSION

// Rotating Vertical flip reveal LCD ISR effect
//
// Starting line bounces between center and progressively higher stopping point
// until top of screen is reached, then it exits
#ifndef INTRO_EFFECT_ASM_VERSION

    // Effect might be able to run without a double buffer...
    static void help_effect_lcd_isr(void) __interrupt {

        // Hide above the effect line and it's mid-screen mirror point
        if ((LY_REG < effect_y_line) || (LY_REG > effect_y_line_mirror)) {
            // Scroll to blank line
            SCY_REG = CALC_SCY_TO_SHOW_MAP_Y_AT_SCANLINE_N(EFFECT_HIDE_LINE, LY_REG);
        } else {
            // Select source Map Y line to show in stretched render region, then increment to next source line
            SCY_REG = CALC_SCY_TO_SHOW_MAP_Y_AT_SCANLINE_N((uint8_t)(map_y_line_to_display >> 8), LY_REG);
            map_y_line_to_display += map_y_line_inc;
        }
    }

#else

    // Budget: 456
    // ASM version to ensure it has no absolute jumps and can be relocated
    static void help_effect_lcd_isr(void) __naked {
        __asm \

        push af
        push hl

        // if ((LY_REG < effect_y_line) -> no stretch region
        ldh a, (_LY_REG)
        ld  hl, #_effect_y_line
        cp  a, (hl)
        jr  c, .non_stretch_region_show_blank_line

        // if ((LY_REG > effect_y_line_mirror) -> no stretch region
        ld  hl, #_effect_y_line_mirror
        cp  a, (hl)
        jr  nc, .non_stretch_region_show_blank_line  // TODO: is this off by -1? (currently LY_REG >= effect_y_line_mirror)


        .stretch_fx_region:
        // SCY_REG = (uint8_t)(map_y_line_to_display >> 8) - LY_REG;
        ld  hl, #_map_y_line_to_display + 1
        cpl                    // Perform: A = (HL) - A (reverse subtraction direction vs sub a, (hl))
        add  a, (hl)
        inc  a
        ldh (_SCY_REG), a

        // map_y_line_to_display += map_y_line_inc;
        dec hl
        ld  a, (#_map_y_line_inc)
        add a, (hl)
        ld  (hl+), a       // Move to MSByte of _map_y_line_to_display as prep for increment below

        ld  a, (#_map_y_line_inc + 1)
        adc a, (hl)        // HL has #_map_y_line_to_display + 1
        ld  (hl), a

        jr .help_effect_exit


        .non_stretch_region_show_blank_line:
        // SCY = map_y_of_blank_line - LY_REG
        ld  l, a     // a has LY_REG
        ld  a, #10 //#EFFECT_HIDE_LINE
        sub a, l
        ldh (_SCY_REG), a

        .help_effect_exit:
        pop hl
        pop af
        reti

        __endasm;
    }

#endif // #ifdef INTRO_EFFECT_ASM_VERSION

// For calculating Effect ISR function length
static void help_effect_lcd_isr__end(void) __naked { }



// Initialize the LCD Effect and install the ISRs (LCD + Music)
void help_effect_init(void) {

    effect_y_line     = EFFECT_INIT_Y;
    effect_line_inc   = EFFECT_INIT_INC;
    effect_line_stop  = effect_line_stop_next = EFFECT_INIT_STOP_Y;
    effect_vflip      = EFFECT_INIT_VFLIP;
    effect_done       = false;

    mapfx_isr_lcd_disable();
    // Copy the Effect LCD ISR into the RAM buffer the interrupt executes from
    copy_lcd_isr_to_isr_ram((void *)&help_effect_lcd_isr, (void *)&help_effect_lcd_isr__end);

    CRITICAL {
        // Fire interrupt at start of HBlank (Mode 0) right before rendering
        STAT_REG = STATF_MODE00;
        add_VBL(help_effect_vbl_isr); // Resets counters each frame and increments effect
    }

    // Wait until just after the start of the next frame before enabling effect
    vsync();
    mapfx_isr_lcd_enable();
}


// Runs until effect completes, turns off effect and deinstalls vbl handler when done
// Loops until effect_y_line reaches top of the screen
void help_effect_run(void) {

    volatile uint8_t done;

    while (1) {
        CRITICAL { done = effect_done; }
        if (done) break;
        vsync();
    }

    mapfx_isr_lcd_disable();
    CRITICAL {
        remove_VBL(help_effect_vbl_isr);
    }
    SCY_REG = 0u; // Restore default scy value
}
