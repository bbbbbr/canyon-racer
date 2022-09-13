#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 #include <rand.h>

#include <gb/isr.h>
#include "common.h"

#include "map_fx.h"
#include "lookup_tables.h"


const uint8_t * p_scx_table_scanline;
const uint8_t * p_scx_table_frame_base;
const uint8_t * p_scx_table_stop;
const uint8_t * p_scx_cur_table;

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

                // Load something useful during idle cycles
                // Adds up to a useful amount of cycles over the whole frame
                ld  hl, #_p_scx_table_scanline  // 3

                // Rendering Center Water Area  (4 tiles) [CENTER]
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
    // SCX_REG = *p_scx_table_scanline++;
        // ld  hl, #_p_scx_table_scanline  // Loaded above during idle cycle burn instead
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
    ldh (_SCX_REG + 0), a    // Load value at (*p_scx_table_scanline)

    pop af
    reti


    __endasm;
}

// Register scanline / STAT ISR handler function for the STAT interrupt
ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr)



// * End of VBlank ISR (start of new frame):
//   - Increment frame-global SCY
//   - Increment, reset SCX table pointers, select next if needed
//
void vblank_isr_map_reset (void) {

    #ifdef DEBUG_BENCHMARK_BG
        // Debug: Benchmark time left by toggling background source at end of processing
        LCDC_REG ^= 0x08u; // Toggle BG source (VBL int for toggle back on)
    #endif


    // == Y Axis ==
    // Scroll the Outer vertical edges of the canyon map by configured amount.
    // The Inner regions will get scrolled an amount derived from this value.
    SCY_REG -= mapfx_y_parallax_speed;


    // == X Axis ==
    // Update SCX table for start of new frame. Sets the base pointer to an index
    // that has usually been incremented by a small amount deeper in the current table.
    //
    // * p_scx_table_scanline  : Incremented once per scanline to drive the SCX wave effect
    // * p_scx_table_frame_base: Updated at start of frame only, used by entities to find map X offset at a given Y position
    //
    p_scx_table_frame_base = p_scx_table_scanline = p_scx_cur_table;

    // Load SCX value for upcoming scanline
    // and increment pointer to prepare for next scanline.
    SCX_REG = *p_scx_table_scanline++;

    // LOOP MODE
    #ifdef SCX_TABLE_EVERY_OTHER_FRAME
    if (sys_time & 0x01) {
    #endif

        p_scx_cur_table -= mapfx_scx_table_map_speed;
        // TODO: optimize: make this less expensive to run in-frame
        if (p_scx_cur_table == p_scx_table_stop) {
            // End of table reached, transition to next table
            // TODO: optimize if needed (could just bump a pointer)
            // TODO: find a way to queue up changes from outside ISR but not relying on code sprinkled in every possible active state?
            // TODO: use level selector to map a local struct pointer
            uint8_t next     = (rand() & scx_table_level[0u].mask) + scx_table_level[0u].base;
            p_scx_cur_table  = scx_tables[next].start_address;
            p_scx_table_stop = scx_tables[next].end_address;
        }

    #ifdef SCX_TABLE_EVERY_OTHER_FRAME
    }
    #endif
}



// Medium Y Parallax, No X Scrolling
void mapfx_set_intro(void) {

    mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_STOP;


    // SCX table: Set to all Straight
    p_scx_cur_table  = scx_tables[SCX_TABLE_STR_STR].start_address;
    p_scx_table_stop = scx_tables[SCX_TABLE_STR_STR].end_address;
    // mapfx_scx_table_reset();
}

// Medium Y Parallax, Medium X Scrolling
// TODO: selectable speed param
void mapfx_set_gameplay(void) {

    mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_MED;
    // SCX table: Set to all Straight -> Low
    p_scx_cur_table  = scx_tables[SCX_TABLE_STR_LOW].start_address;
    p_scx_table_stop = scx_tables[SCX_TABLE_STR_LOW].end_address;
    // mapfx_scx_table_reset();
}



void mapfx_scx_table_reset(void) {
    // Init control vars

    // These get updated once per vblan, but need a way to be jumpstarted
    // for the first frame after being turned on since that likely won't not
    //  align exactly with being right before vblank
    //
    // Should match what happens in vblank
    p_scx_cur_table  = scx_tables[SCX_TABLE_STR_STR].start_address;
    p_scx_table_stop = scx_tables[SCX_TABLE_STR_STR].end_address;
    p_scx_table_frame_base = p_scx_table_scanline = p_scx_cur_table;
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





