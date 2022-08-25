#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gb/isr.h>
#include "common.h"

#include "map_fx.h"
#include "lookup_tables.h"

static uint8_t map_lcd_scy_start;

static bool            map_lcd_scx_table_is_inc;
       uint16_t        map_lcd_scx_table_start;
const  uint8_t *       p_scx_table;
const  uint8_t *       p_scx_table_base;
static uint8_t         map_lcd_scx_wait_counter;


// *** Vertical Parallax clock timing ***
// TODO: Prob don't need this anymore
//       Found a timing that works on hardware AND BGB / Emulicious
// #define _EMU_PIXEL_TIMING  // Emulicious and BGB differ from hardware
//    -> It was: [8/0,0,4,0,0]

// TODO: Analogue Pocket .pocket format special timing & #define?




// Start of New Frame
//
// 1. VBlank:
// 3. HBlank Interrupts:

// TODO: doesn't actually need to be 256 byte aligned anymore, can be const
// TODO : reduce amount of space allocated
// # -Wl-g_shadow_OAM=0xC800 -Wl-b_DATA=0xc8a0
// uint8_t __at(0xC000) scx_table[] = {



// 4 REGION MODE: Outer / Mid / Inner / Center
//
// Left Side: SCY bit-Rotated Right once before each region to create
//            a 2x scrolling speeds for it,
//
// Right Side: Same as left side, but Rotated Left to gradually restore
//             original SCY value
//
// The bits wrapping around looks ok
//
// Note: To make this non-interruptable by other ISRs add: __critical
// http://sdcc.sourceforge.net/doc/sdccman.pdf#subsection.3.8.4
void map_fx_stat_isr(void) __interrupt __naked {
    __asm \

    push af // 4
    push hl // 4
    push bc // 4

    ldh a, (_SCY_REG+0) // 3

    // Rendering Outer V Scroll Area (4 tiles) + Mode 2 OAM Scan [LEFT]

    // .rept 1 // 9
    .rept 1
        nop
    .endm
    rra
    ldh (_SCY_REG+0), a

        // Rendering Mid V Scroll Area (2 tiles) [LEFT]
        rra
        ldh (_SCY_REG+0), a

            // Rendering Inner V Scroll Area (2 tiles) [LEFT]
            rra
            ldh (_SCY_REG+0), a

                // Rendering Center Water Area  (4 tiles) [CENTER]
                // Load something useful during idle cycles
                ld  hl, #_p_scx_table  // 3
                // .rept 4
                .rept 1
                    nop
                .endm
                rla
                ldh (_SCY_REG+0), a

            // Rendering Inner V Scroll Area (2 tiles) [RIGHT]
            rla
            ldh (_SCY_REG+0), a

        // Rendering Mid V Scroll Area (2 tiles) [RIGHT]
        rla
        ldh (_SCY_REG+0), a

    // Rendering Outer V Scroll Area (4 tiles) [RIGHT]
    // No need to update SCY, original value has been restored

    // Set Y scroll for next line (won't apply to current line due to timing)
    // SCX_REG = *p_scx_table++;
        // ld  hl, #_p_scx_table  // Loaded above during idle cycle burn instead
    ld  a, (hl+)
    ld  c, a                      // Low pointer address byte -> C
    ld  a, (hl-)
    ld  b, a                      // Hi  pointer address byte -> B
    ld  a, (bc)
        //ldh (_SCX_REG + 0), a   // Loaded below instead

    // Increment pointer to next line value
    inc (hl)
    jr  nz, 00103$

    // Upper nybble increment
    inc hl
    inc (hl)
    00103$:

    pop bc
    pop hl

    // Apply SCX update as late as possible to try and take effect
    // ~after rendering of scanline is completed
    // (sprites/etc could push timing out)
    ldh (_SCX_REG + 0), a    // Load value at (*p_scx_table)

    pop af
    reti


    __endasm;
}



// Register scanline / STAT ISR handler function for the STAT interrupt
// ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr_cgb)
ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr)


// ### Test/debug controls ###

// #define SCY_FX_SPEED_STOPPED
// #define SCY_FX_SPEED_SLOW
#define SCY_FX_SPEED_MED
// #define SCY_FX_SPEED_FAST  // <-- Current default
// #define SCY_FX_SPEED_MAX
// TODO: half speed steps? 1.5, 2.5, etc


// Turns on / off SCX table scrolling
#define SCX_TABLE_SCROLL


// TODO: Make speed an adjustable var
//  * 1/2/4 for Y parallax speed
//  * 1/2/4 for Y scroll speed or every/other/fourth for update masking

// Region scrolling speed
// table size MUST be multiple of SCROLL_Y_MAP_SPEED
#if defined (SCY_FX_SPEED_STOPPED)
    #define SCROLL_Y_PARALLAX_SPEED   0u
    #define SCROLL_Y_MAP_SPEED        0u
#elif defined (SCY_FX_SPEED_SLOW)
    #define SCROLL_Y_PARALLAX_SPEED   1u
    #define SCROLL_Y_MAP_SPEED        1u
#elif defined (SCY_FX_SPEED_MED)
    #define SCROLL_Y_PARALLAX_SPEED   2u
    #define SCROLL_Y_MAP_SPEED        2u
#elif defined (SCY_FX_SPEED_FAST)
    #define SCROLL_Y_PARALLAX_SPEED   2u
    #define SCROLL_Y_MAP_SPEED        4u
#elif defined (SCY_FX_SPEED_MAX)
    #define SCROLL_Y_PARALLAX_SPEED   8u
    #define SCROLL_Y_MAP_SPEED        8u
#endif


// Makes Canyon Shape scrolling even slower
#define SCX_TABLE_EVERY_OTHER_FRAME



// Effect pans up from end of SCX table to start to reveal curves

#define MAP_LCD_SCX_TABLE_START    (scx_table_1_SZ - SCREEN_H_PLUS_1) // start at farthest point possible into the table, it scrolls toward the start
 // #define MAP_LCD_SCX_TABLE_START    (scx_table_straight_SZ - SCREEN_H_PLUS_1) // start at farthest point possible into the table, it scrolls toward the start

#define MAP_LCD_SCX_TABLE_INC_STOP (MAP_LCD_SCX_TABLE_START)
#define MAP_LCD_SCX_TABLE_DEC_STOP 0u
#define MAP_LCD_SCX_COUNTER_WAIT_TIME  120u



// * Top of Screen Area
//   - Increment frame-global SCY
//
void vblank_isr_map_reset (void) {

    #ifdef DEBUG_BENCHMARK_BG
        // Debug: Benchmark time left by toggling background source at end of processing
        LCDC_REG ^= 0x08u; // Toggle BG source (VBL int for toggle back on)
    #endif


    // Y Axis: Scroll  the outer vertical edges by max amount
    SCY_REG -= SCROLL_Y_PARALLAX_SPEED;

    // X Axis: Reset SCX table
    p_scx_table_base = p_scx_table = &scx_table_1[map_lcd_scx_table_start];
    // p_scx_table_base = p_scx_table = &scx_table_straight[map_lcd_scx_table_start];
    SCX_REG = *p_scx_table++;

    // LOOP MODE
    #ifdef SCX_TABLE_SCROLL
        #ifdef SCX_TABLE_EVERY_OTHER_FRAME
        if (sys_time & 0x01) {
        #endif

            map_lcd_scx_table_start -= SCROLL_Y_MAP_SPEED;

            if (map_lcd_scx_table_start == MAP_LCD_SCX_TABLE_DEC_STOP) {
                // MODE: LOOP (requires SCX table to have perfectly matching start and end segments
                map_lcd_scx_table_start = MAP_LCD_SCX_TABLE_START;
                map_lcd_scy_start = 0;

            }
        #ifdef SCX_TABLE_EVERY_OTHER_FRAME
        }
        #endif
    #endif


    // if (_cpu == CGB_TYPE) {
}


void map_fx_isr_enable(void) {

    // Init control vars
    map_lcd_scx_table_is_inc = false;
    map_lcd_scx_table_start = MAP_LCD_SCX_TABLE_START;
    map_lcd_scy_start = 0;
    p_scx_table = &scx_table_1[map_lcd_scx_table_start];
    // p_scx_table = &scx_table_straight[map_lcd_scx_table_start];
    map_lcd_scx_wait_counter = 0;


    // Enable STAT ISR
    __critical {
        // Actually want to start at the beginning of Mode 3 / Rendering, but there isn't a STAT interrupt for that
        STAT_REG = STATF_MODE10; // Fire interrupt at start of OAM SCAN (Mode 2) right before rendering
        add_VBL(vblank_isr_map_reset);
    }
    // Try to wait until just after the start of the next frame
    wait_vbl_done();

    // Pre-load SCX
    // SCX_REG = *p_scx_table++;

    set_interrupts(IE_REG | LCD_IFLAG);
}


void map_fx_isr_disable(void) {

    // Disable STAT ISR
    __critical {
        set_interrupts(IE_REG & ~LCD_IFLAG);
        remove_VBL(vblank_isr_map_reset);
    }
}





