#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gb/isr.h>

#include "common.h"

static uint8_t map_lcd_scy_start = 0;

// Start of New Frame
//
// 1. VBlank:
// 3. HBlank Interrupts:

// TODO : reduce amount of space allocated
// # -Wl-g_shadow_OAM=0xC800 -Wl-b_DATA=0xc8a0
uint8_t __at(0xC000) scx_table[] = {
     0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0

    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0

    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0
};


// = {
//     0,1,2,3,4,5,6,7,8,9
//     ,10,11,12,13,14,15,16,17,18,19
//     ,20,21,22,23,24,25,26,27,28,29
//     ,30,30,30,30,30,30,30,30,30,30
//     ,30,30,30,30,30,30,30,30,30,30

//     ,40,40,40,40,40,40,40,40,40,40
//     ,40,40,40,40,40,40,40,40,40,40
//     ,40,40,40,40,40,40,40,40,40,40
//     ,40,40,40,40,40,40,40,40,40,40
//     ,40,40,40,40,40,40,40,40,40,40

//     ,0,1,2,3,4,5,6,7,8,9
//     ,10,11,12,13,14,15,16,17,18,19
//     ,20,21,22,23,24,25,26,27,28,29
//     ,30,30,30,30,30,30,30,30,30,30
//     ,30,30,30,30,30,30,30,30,30,30};

uint8_t * p_scx_addr = scx_table;


#define SCROLL_START_Y 10u

// 4 REGION MODE: Outer / Mid / Inner / Center
// Left Side: SCY bit-Rotated Right once before each region to create
//            a 2x scrolling speeds there, and then left shifted back
// Right Side: Same as left side, but Rotated Left to gradually restore
//             original SCY value
// The bits wrapping around is ok (TODO: fix tilemap pattern where it pops)
void map_fx_stat_isr_dmg(void) __interrupt __naked {
    __asm \

                    // push hl  // 4
    push af

                // = 7
                // ldh     a, (_SCY_REG+0) // 3
                // ld      l,a      // 1
                // ld      h,#0xC0  // 2
                // ld      a,(hl)   // 2
                // ldh     (_SCX_REG+0), a // 3

                // 4 + 11 = 15, only have 9
                // Change LCD to trigger on previous mode - may need to change timing alignment method


    ldh a, (_SCY_REG+0) // 3


    // Rendering Outer V Scroll Area (4 tiles) + Mode 2 OAM Scan [LEFT]

    .rept 9
        nop
    .endm
    rra
    ldh (_SCY_REG+0), a

        // Rendering Mid V Scroll Area (2 tiles) [LEFT]
        rra
        ldh (_SCY_REG+0), a

            // Rendering Inner V Scroll Area (2 tiles) [LEFT]
            nop
            rra
            ldh (_SCY_REG+0), a

                // Rendering Center Water Area  (4 tiles) [CENTER]
                .rept 3
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


    pop af
                //    pop hl
    reti

    __endasm;
}


// CGB: Same deal as DMG version, different timing
// np = 2 dots

// 4 REGION MODE: Outer / Mid / Inner / Center
void map_fx_stat_isr_cgb(void) __interrupt __naked {
    __asm \

    push af

    // Get current Scroll Y
    ldh a, (_SCY_REG+0)

    // Rendering Outer V Scroll Area (4 tiles) + Mode 2 OAM Scan [LEFT]
    .rept 39
        nop
    .endm
    rra
    ldh (_SCY_REG+0), a

        // Rendering Mid V Scroll Area (2 tiles) [LEFT]
        rra
        .rept 3
            nop
        .endm
        ldh (_SCY_REG+0), a

            // Rendering Inner V Scroll Area (2 tiles) [LEFT]
            .rept 3
                nop
            .endm
            rra
            ldh (_SCY_REG+0), a

                // Rendering Center Water Area  (4 tiles) [CENTER]
                .rept 14
                    nop
                .endm
                rla
                ldh (_SCY_REG+0), a

            // Rendering Inner V Scroll Area (2 tiles) [RIGHT]
            rla
            .rept 3
                nop
            .endm
            ldh (_SCY_REG+0), a

        // Rendering Mid V Scroll Area (2 tiles) [RIGHT]
        rla
        .rept 4
            nop
        .endm
        ldh (_SCY_REG+0), a

    // Rendering Outer V Scroll Area (4 tiles) [RIGHT]
    // No need to update SCY, original value has been restored
    pop af
    reti

    __endasm;
}



// Register scanline / STAT ISR handler function for the STAT interrupt
// ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr_cgb)
ISR_VECTOR(VECTOR_STAT, map_fx_stat_isr_dmg)


// 4 Region mode slow
#define SCROLL_Y_OUTER 2u // 2u: good, 1u: slow (better for testing)

// * Top of Screen Area
//   - Increment frame-global SCY
//
void vblank_isr_map_reset (void) {

    // Y Axis: Scroll  the outer vertical edges by max amount
    SCY_REG -= SCROLL_Y_OUTER;

    // X Axis: Reset SCX
    p_scx_addr = &scx_table[0];
    // if (_cpu == CGB_TYPE) {
}


void map_fx_isr_enable(void) {

    // Enable STAT ISR
    __critical {
        // Actually want to start at the beginning of Mode 3 / Rendering, but there isn't a STAT interrupt for that
        STAT_REG = STATF_MODE10; // Fire interrupt at start of OAM SCAN (Mode 2) right before rendering
        add_VBL(vblank_isr_map_reset);
    }
    // Try to wait until just after the start of the next frame
    wait_vbl_done();
    set_interrupts(IE_REG | LCD_IFLAG);
}


void map_fx_isr_disable(void) {

    // Disable STAT ISR
    __critical {
        set_interrupts(IE_REG & ~LCD_IFLAG);
        remove_VBL(vblank_isr_map_reset);
    }
}





