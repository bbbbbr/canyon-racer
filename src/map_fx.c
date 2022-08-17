#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <gb/isr.h>

#include "common.h"

static uint8_t map_lcd_scy_start;

static bool            map_lcd_scx_table_is_inc;
       uint16_t        map_lcd_scx_table_start;
const  uint8_t *       p_scx_table;
const  uint8_t *       p_scx_table_base;
static uint8_t         map_lcd_scx_wait_counter;

// Start of New Frame
//
// 1. VBlank:
// 3. HBlank Interrupts:

// TODO: doesn't actually need to be 256 byte aligned anymore, can be const
// TODO : reduce amount of space allocated
// # -Wl-g_shadow_OAM=0xC800 -Wl-b_DATA=0xc8a0
// uint8_t __at(0xC000) scx_table[] = {


// Needs to be an even multple of....
const uint8_t scx_table_1[] = {

    // Non-curving zone
    // 143 & 144
    0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 120
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 60
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 0

    // Sharper curves (144)
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,

    // Slow curve (288)
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,

    // Sharper curves (144)
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,


    // TODO: Increase max range of slower low frequency waves?
    // Lower frequency waves
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,
    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,

    // padding to multiple of 2 entry
    0,

    // Non-curving zone
    // 143 & 144
    0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 120
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 60
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 0

};


const uint8_t scx_table_straight[] = {

    // Non-curving zone
    // 143 & 144
    0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 120
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 60
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 0

    // Non-curving zone
    // 143 & 144
    0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 120
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 60
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    // 0

    // Padding
    0,
};


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
void map_fx_stat_isr_dmg(void) __interrupt __naked {
    __asm \

    push af // 4

    ldh a, (_SCY_REG+0) // 3

    // Rendering Outer V Scroll Area (4 tiles) + Mode 2 OAM Scan [LEFT]

    // TODO: Fixup timing back to hardware. It was working on hardware, but changed
    //  it to work with emulator timing for easier debugging

    // .rept 9
    .rept 8
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

    push hl
    push bc

    // Set Y scroll for next line (won't apply to current line due to timing)
    // SCX_REG = *p_scx_table++;
    ld  hl, #_p_scx_table
    ld  a, (hl+)
    ld  c, a
    ld  a, (hl-)
    ld  b, a
    ld  a, (bc)
    ldh (_SCX_REG + 0), a

    // Increment pointer to next line value
    inc (hl)
    jr  nz, 00103$

    inc hl
    inc (hl)
    00103$:

    pop bc
    pop hl

    pop af
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


// ### Test/debug controls ###

// #define DMG_SPEED_STOPPED
// #define DMG_SPEED_SLOW
#define DMG_SPEED_MED
// #define DMG_SPEED_FAST  // <-- Current default
// #define DMG_SPEED_MAX


// Turns on / off SCX table scrolling
#define SCX_TABLE_SCROLL


// TODO: Make speed an adjustable var
//  * 1/2/4 for Y parallax speed
//  * 1/2/4 for Y scroll speed or every/other/fourth for update masking

// Region scrolling speed
// table size MUST be multiple of SCROLL_Y_MAP_SPEED
#if defined (DMG_SPEED_STOPPED)
    #define SCROLL_Y_PARALLAX_SPEED   0u
    #define SCROLL_Y_MAP_SPEED        0u
#elif defined (DMG_SPEED_SLOW)
    #define SCROLL_Y_PARALLAX_SPEED   1u
    #define SCROLL_Y_MAP_SPEED        1u
#elif defined (DMG_SPEED_MED)
    #define SCROLL_Y_PARALLAX_SPEED   2u
    #define SCROLL_Y_MAP_SPEED        2u
#elif defined (DMG_SPEED_FAST)
    #define SCROLL_Y_PARALLAX_SPEED   2u
    #define SCROLL_Y_MAP_SPEED        4u
#elif defined (DMG_SPEED_MAX)
    #define SCROLL_Y_PARALLAX_SPEED   8u
    #define SCROLL_Y_MAP_SPEED        8u
#endif


// Effect pans up from end of SCX table to start to reveal curves

//  #define MAP_LCD_SCX_TABLE_START    (sizeof(scx_table_1) - 145u) // start at farthest point possible into the table, it scrolls toward the start
 #define MAP_LCD_SCX_TABLE_START    (sizeof(scx_table_straight) - 145u) // start at farthest point possible into the table, it scrolls toward the start

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
    // p_scx_table_base = p_scx_table = &scx_table_1[map_lcd_scx_table_start];
    p_scx_table_base = p_scx_table = &scx_table_straight[map_lcd_scx_table_start];
    SCX_REG = *p_scx_table++;

    // LOOP MODE
    #ifdef SCX_TABLE_SCROLL
        // if ((sys_time & 0x07) == 0x00) {
        // if ((sys_time & 0x03) == 0x00) {
        if (sys_time & 0x01) {

            map_lcd_scx_table_start -= SCROLL_Y_MAP_SPEED;

            if (map_lcd_scx_table_start == MAP_LCD_SCX_TABLE_DEC_STOP) {
                // MODE: LOOP (requires SCX table to have perfectly matching start and end segments
                map_lcd_scx_table_start = MAP_LCD_SCX_TABLE_START;
                map_lcd_scy_start = 0;

            }
        }
    #endif


    // if (_cpu == CGB_TYPE) {
}


void map_fx_isr_enable(void) {

    // Init control vars
    map_lcd_scx_table_is_inc = false;
    map_lcd_scx_table_start = MAP_LCD_SCX_TABLE_START;
    map_lcd_scy_start = 0;
    // p_scx_table = &scx_table_1[map_lcd_scx_table_start];
    p_scx_table = &scx_table_straight[map_lcd_scx_table_start];
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





