#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"


// Lookup Tables for effects

// The Horizontal wave effect scrolls vertically from the end of SCX tables until it reaches the start.
// New table is loaded once the end is reached.
// The first frame-worth of a new table should match the last frame-worth of the previous table (for seamelss transition).


// Negative numbers get cast to their unsigned wraparound equivalent
//
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


// Used for Intro screen (2x screens worth)
//
// TODO: Could probably use the table above
//       and just set a different END pointer address / counter
//
// OR:   Could just set a NULL pointer and not do SCX scrolling if NULL

const uint8_t scx_table_straight[] = {

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


// Array sizing constants for export
const size_t scx_table_1_SZ         = ARRAY_LEN(scx_table_1);
const size_t scx_table_straight_SZ = ARRAY_LEN(scx_table_straight);
