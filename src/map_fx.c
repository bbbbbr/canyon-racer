#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 #include <rand.h>

#include <gb/isr.h>
#include "common.h"
#include "level.h"

#include "audio.h"

#include "map_fx.h"
#include "lookup_tables.h"



// Effect pans up from end of SCX table to start to reveal curves
//
//
// == Vertical Parallax clock timing ==
// TODO: Analogue Pocket .pocket format special timing & #define?
//
// == Interrupt Activity ==
//
// = VBlank:
// * SCX Wave Tables
//   - Increment SCX table base pointer
//   - Reset per-scanline pointer
//   - Apply SCX value for first scanline
//   - Load new SCX table address if needed
// * Y Parallax Effect
//   - Increment SCY at preset speed
//
// = HBlank Interrupts: (Start in Mode 2 OAM Scan)
// * Y Parallax Effect
//   - Load Y Scroll amount, shift and apply to each column area
// * SCX Wave Tables
//   - Pre-Apply SCX value for NEXT scanline
//   - Increment per-scanline pointer value
//


// 4 REGIONS: Outer / Mid / Inner / Center
//
// Left Side: SCY bit-Rotated Right once before each region to create
//            a 2x scrolling speeds for it,
//
// Right Side: Same as left side, but Rotated Left to gradually restore
//             original SCY value
//
// The bits wrapping around looks ok!
//
//
// * Uses a bare function and manages register saving itself.
// * Does not use equivalent of wait_int_handler() to avoid GFX
//   corruption in main code since always exits ,midway in Mode 0/HBLANK
//   NOTE: if this behavior changes then start guarding GFX loading
void map_fx_stat_isr(void) __interrupt __naked {
    __asm \

    push af // 4
    push hl // 4
    push bc // 4

    ldh a, (_SCY_REG+0) // 3

    // Rendering Outer V Scroll Area (4 tiles) + Mode 2 OAM Scan [LEFT]

    // Was .rept 9 before moving pushes to above
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
                ld  hl, #(_state + 0)  // equivalent to: #(_state.p_scx_table_scanline) // 3

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
    // SCX_REG = *state.p_scx_table_scanline++;
        // ld  hl, #_state.p_scx_table_scanline  // Loaded above during idle cycle burn instead
    ld  a, (hl+)
    ld  c, a                      // Low pointer address byte -> C
    ld  a, (hl-)
    ld  b, a                      // Hi  pointer address byte -> B
    ld  a, (bc)
        //ldh (_SCX_REG + 0), a   // Loaded below instead

    // Increment pointer to next line value
    inc (hl)
    jr  nz, 1$

    // Upper nybble increment
    inc hl
    inc (hl)
    1$:

    pop bc
    pop hl

    // Apply SCX update as late as possible to try and take effect
    // ~after rendering of scanline is completed
    // (sprites/etc could push timing out)
    // Should be in Mode 0 (HBlank)
    ldh (_SCX_REG + 0), a    // Load value at (*state.p_scx_table_scanline)

    pop af
    reti


    __endasm;
}

// Register scanline / STAT ISR handler function for the STAT interrupt
ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr)



// * End of VBlank ISR (start of new frame):
//   - Turn LCD int OFF
//   - SCY: Increment frame-global SCY offset
//   - SCX: Reset SCX table pointers
//          Load for next frame
//          Increment
//          Select next table adderss if needed
//   - Turn LCD int ON
//
// * audio_vbl_isr() gets called after this
//
void vblank_isr_map_reset (void) {

    #ifdef VISUAL_DEBUG_BENCHMARK_MAIN
        // VISUAL BENCHMARK START
        // Debug: Benchmark time left by toggling background on/off
        LCDC_REG |= LCDCF_BGON; // Toggle BG source (VBL int for toggle back on)
    #endif

    // == Y Axis ==
    // Scroll the Outer vertical edges of the canyon map by configured amount.
    // The Inner regions will get scrolled an amount derived from this value.
    SCY_REG -= state.mapfx_y_parallax_speed;

    // == X Axis ==
    // Update SCX table for start of new frame. Sets the base pointer to an index
    // that has usually been incremented by a small amount deeper in the current table.
    //
    // * state.p_scx_table_scanline  : Incremented once per scanline to drive the SCX wave effect
    // * state.p_scx_table_frame_base: Updated at start of frame only, used by entities to find map X offset at a given Y position
    //
    state.p_scx_table_frame_base = state.p_scx_table_scanline = state.p_scx_cur_table;

    // Load SCX value for upcoming scanline
    // and increment pointer to prepare for next scanline.
    SCX_REG = *state.p_scx_table_scanline++;

    // Determines whether SCX wave table is incremented every frame or every other frame
    if ((state.cur_level.mapfx_scx_inc_every_frame) || (sys_time & 0x01)) {

        state.p_scx_cur_table -= state.mapfx_scx_table_map_speed;
        // TODO: optimize: make this less expensive to run in-frame
        if (state.p_scx_cur_table == state.p_scx_table_stop) {
            // End of table reached, transition to next table -> on the next frame
            // TODO: optimize if needed (could just bump a pointer)
            // TODO: find a way to queue up changes from outside ISR but not relying on code sprinkled in every possible active state?
            //       could do a flag for "load next from queue", in queue is table entry + flag
            //        --> mapfx_scx_next_queued
            //            - apply it, and generate next  randomly with allowed override and save
                    // TODO:
                    //  mapfx_scx_next_queued can be overwritten manually outside of ISR to set transition segments
                    //
                    // state.p_scx_cur_table  = scx_tables[mapfx_scx_next_queued].start_address;
                    // state.p_scx_table_stop = scx_tables[mapfx_scx_next_queued].end_address;
                    // mapfx_scx_next_queued  =  (rand() & state.mapfx_level_mask) + state.mapfx_level_base;
            uint8_t next     =  (rand() & state.mapfx_level_mask) + state.mapfx_level_base;
            // next     =  SCX_TABLE_STR_STR;
            state.p_scx_cur_table  = scx_tables[next].start_address;
            state.p_scx_table_stop = scx_tables[next].end_address;
            // ... frame_base set above
        }
    }

    // Clear any pending LCD/STAT interrupt to prevent Map Effect ISR running
    // on scanlines > 143 which aren't visible. Saves a little cpu time.
    IF_REG = IF_REG & ~LCD_IFLAG;
}



// Selects which scx table wave segments should be usable
// based on a level based lookup table
void mapfx_level_set(uint8_t level) {

    if (level > SCX_TABLE_LEVEL_MAX)
        level = SCX_TABLE_LEVEL_MAX;

    // TODO: Wrap these in a critical section? It would be good form. Would it throw off interrupt timing much? Prob not.
    // CRITICAL {
    state.mapfx_level_mask = scx_table_levels[level].mask;
    state.mapfx_level_base = scx_table_levels[level].base;
    // }
}


// Medium Y Parallax, No X Scrolling
void mapfx_set_intro(void) {

    state.mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    state.mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_STOP;


    // SCX table: Set to all Straight
    state.p_scx_cur_table  = scx_tables[SCX_TABLE_STR_STR].start_address;
    state.p_scx_table_stop = scx_tables[SCX_TABLE_STR_STR].end_address;

    // Below can get overridden by calls to mapfx_level_set()
    mapfx_level_set(SCX_TABLE_LEVEL_MIN);
}


// Medium Y Parallax, Medium X Scrolling
// TODO: selectable speed param
void mapfx_set_gameplay(void) {

    state.mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_MED;
    state.mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_MED;

    // SCX table: Set to all Straight -> Low transition
    // This gives a non-curved initial startup
    state.p_scx_cur_table  = scx_tables[SCX_TABLE_STR_LOW].start_address;
    state.p_scx_table_stop = scx_tables[SCX_TABLE_STR_LOW].end_address;
    // mapfx_level_set(SCX_TABLE_LEVEL_MIN);
    // mapfx_scx_table_reset();
}


// Pauses and Un-pauses map SCX Wave and SCY Column scrolling
void mapfx_set_setpause(bool is_paused) {

    if (is_paused) {
        __critical {
            // Save current effect speeds to re-apply during Un-Pause
            state.save__mapfx_y_parallax_speed    = state.mapfx_y_parallax_speed;
            state.save__mapfx_scx_table_map_speed = state.mapfx_scx_table_map_speed;

            // *Keep* applying SCX wave effect per-scanline
            // Stop incrementing SCY parallax columns
            // Stop incrementing SCX wave scrolling
            state.mapfx_y_parallax_speed    = MAPFX_SCY_SPEED_STOP;
            state.mapfx_scx_table_map_speed = MAPFX_SCX_SPEED_STOP;
        }
    }
    else {
        // Resume effect scrolling using saved values
        __critical {
            state.mapfx_y_parallax_speed = state.save__mapfx_y_parallax_speed;
            state.mapfx_scx_table_map_speed = state.save__mapfx_scx_table_map_speed;
        }
    }
}


void mapfx_scx_table_reset(void) {
    // Init control vars

    // These get updated once per vblank, but need a way to be jumpstarted
    // for the first frame after being turned on since that likely won't
    // align exactly with being right before vblank
    //
    // Should match what happens in vblank
    state.p_scx_cur_table  = scx_tables[SCX_TABLE_STR_STR].start_address;
    state.p_scx_table_stop = scx_tables[SCX_TABLE_STR_STR].end_address;
    state.p_scx_table_frame_base = state.p_scx_table_scanline = state.p_scx_cur_table;

    // Below can get overridden by calls to mapfx_level_set()
    mapfx_level_set(SCX_TABLE_LEVEL_MIN);
}


// Installs the Map Effect VBL ISR *AND* Audio VBL ISR
//
// Only call after:
// * mapfx_set_*() has been used to init Map Effect control vars
// * audio_init() has been called to init Audio control vars
void mapfx_isr_install(bool add_audio_isr) {

    // Enable STAT ISR
    __critical {
        // Fire interrupt at start of OAM SCAN (Mode 2) right before rendering
        STAT_REG = STATF_MODE10;

        add_VBL(vblank_isr_map_reset);

        // Audio VBL (music & sfx) MUST be installed AFTER vblank_isr_map_reset() ISR
        if (add_audio_isr)
            add_VBL(audio_vbl_isr);
    }

    // Try to wait until just after the start of the next frame before enabling effect
    wait_vbl_done();

    mapfx_isr_lcd_enable();
}


// Installs the Map Effect VBL ISR *AND* Audio VBL ISR
void mapfx_isr_deinstall(void) {

    mapfx_isr_lcd_disable();

    __critical {
        remove_VBL(audio_vbl_isr);
        remove_VBL(vblank_isr_map_reset);
    }
}


// Turn on LCD/STAT ISR
// * Should only be called if mapfx_isr_install() has been called once before
// * Does not need
void mapfx_isr_lcd_enable(void) {

    set_interrupts(IE_REG | LCD_IFLAG);
}


// Turns off LCD/STAT ISR
void mapfx_isr_lcd_disable(void) {

    set_interrupts(IE_REG & ~LCD_IFLAG);
}





