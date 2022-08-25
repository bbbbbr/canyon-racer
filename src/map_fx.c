#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gb/isr.h>
#include "common.h"

#include "map_fx.h"
#include "lookup_tables.h"


static bool      mapfx_scx_table_is_inc;
       uint16_t  mapfx_scx_table_start;
const  uint8_t * p_scx_table;
const  uint8_t * p_scx_table_base;

       uint8_t   mapfx_y_parallax_speed = MAPFX_SCY_SPEED_DEFAULT;
       uint8_t   mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_DEFAULT;


// Effect pans up from end of SCX table to start to reveal curves


// == Vertical Parallax clock timing ==
// TODO: Analogue Pocket .pocket format special timing & #define?

// == Interrupt Activity ==
//
// 1. VBlank:
// * SCX Wave Tables
//   - Increment SCX table base pointer
//   - Reset per-scanline pointer
//   - Apply SCX value for first scanline
// * Y Parallax Effect
//   - Increment ... TODO
//
// 2. HBlank Interrupts: (Start in Mode 2 OAM Scan)
// * Y Parallax Effect
//   - Load Y Scroll amount, shift and apply to each column area
// * SCX Wave Tables
//   - Apply SCX value for NEXT scanline
//   - Increment per-scanline pointer value
//



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
ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr)


#define MAPfX_SCX_TABLE_START    (scx_table_1_SZ - SCREEN_H_PLUS_1) // start at farthest point possible into the table, it scrolls toward the start
 // #define MAPfX_SCX_TABLE_START    (scx_table_straight_SZ - SCREEN_H_PLUS_1) // start at farthest point possible into the table, it scrolls toward the start

#define MAPfX_SCX_TABLE_INC_STOP (MAPfX_SCX_TABLE_START)
#define MAPfX_SCX_TABLE_DEC_STOP 0u
#define MAPfX_SCX_COUNTER_WAIT_TIME  120u



// * Top of Screen Area
//   - Increment frame-global SCY
//
void vblank_isr_map_reset (void) {

    #ifdef DEBUG_BENCHMARK_BG
        // Debug: Benchmark time left by toggling background source at end of processing
        LCDC_REG ^= 0x08u; // Toggle BG source (VBL int for toggle back on)
    #endif


    // Y Axis: Scroll  the outer vertical edges by max amount
    SCY_REG -= mapfx_y_parallax_speed;


    // X Axis: Reset SCX table
    p_scx_table_base = p_scx_table = &scx_table_1[mapfx_scx_table_start];
    // p_scx_table_base = p_scx_table = &scx_table_straight[mapfx_scx_table_start];
    SCX_REG = *p_scx_table++;

    // LOOP MODE
    #ifdef SCX_TABLE_SCROLL
        #ifdef SCX_TABLE_EVERY_OTHER_FRAME
        if (sys_time & 0x01) {
        #endif

            mapfx_scx_table_start -= mapfx_scx_table_map_speed;

            if (mapfx_scx_table_start == MAPfX_SCX_TABLE_DEC_STOP) {
                // MODE: LOOP (requires SCX table to have perfectly matching start and end segments
                mapfx_scx_table_start = MAPfX_SCX_TABLE_START;

            }
        #ifdef SCX_TABLE_EVERY_OTHER_FRAME
        }
        #endif
    #endif


    // if (_cpu == CGB_TYPE) {
}



// Medium Y Parallax, No X Scrolling
void mapfx_set_intro(void) {

    mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_STOP;
    // TODO: set straight SCX table
    mapfx_scx_table_reset();
}

// Medium Y Parallax, Medium X Scrolling
// TODO: selectable speed param
void mapfx_set_gameplay(void) {

    mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_MED;
    // TODO: set SCX table ______
    mapfx_scx_table_reset();
}


void mapfx_scx_table_reset(void) {
    // Init control vars
    mapfx_scx_table_is_inc = false;
    mapfx_scx_table_start = MAPfX_SCX_TABLE_START;
    p_scx_table = &scx_table_1[mapfx_scx_table_start];
    // p_scx_table = &scx_table_straight[mapfx_scx_table_start];
}


// mapfx_set_*() should be called before this
// to correcty initialize scoll speeds
void mapfx_isr_enable(void) {

    // Enable STAT ISR
    __critical {
        STAT_REG = STATF_MODE10; // Fire interrupt at start of OAM SCAN (Mode 2) right before rendering
        add_VBL(vblank_isr_map_reset);
    }

    // Try to wait until just after the start of the next frame before enabling
    wait_vbl_done();

    // Pre-load SCX
    // SCX_REG = *p_scx_table++;
    set_interrupts(IE_REG | LCD_IFLAG);
}


void mapfx_isr_disable(void) {

    // Disable STAT ISR
    __critical {
        set_interrupts(IE_REG & ~LCD_IFLAG);
        remove_VBL(vblank_isr_map_reset);
    }
}





