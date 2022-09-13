#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "lookup_tables.h"


// Lookup Tables for effects

// The Horizontal wave effect scrolls vertically frin
// the end of SCX tables until it reaches the start.
//
// A new table is loaded once the end is reached.
//
// In order to have a seamless transition, the first
// frame-worth of a new table should match the last
// frame-worth of the previous table.
//
// Negative values are explicitly cast to their
// unsigned wraparound equivalent

// 143 -> 0
#define SCX_TABLE_STRAIGHT_144  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

// Deleted a "(uint8_t)0" off start of the following - TODO: need to re-add?
// Max -11
#define SCX_TABLE_WAVE_144_LOW   (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,
// Max -23
#define SCX_TABLE_WAVE_144_MED   (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,
// Max -23
#define SCX_TABLE_WAVE_288_MED   (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,

// TODO: Remove?
// TODO: there wouldn't be as much distortion if the curves were centered in their offsets
// Max -35 - Too much edge distorion? - compensation is too expensive
// #define SCX_TABLE_WAVE_144_HI    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-22,(uint8_t)-23,(uint8_t)-24,(uint8_t)-24,(uint8_t)-25,(uint8_t)-26,(uint8_t)-26,(uint8_t)-27,(uint8_t)-28,(uint8_t)-28,(uint8_t)-29,(uint8_t)-30,(uint8_t)-30,(uint8_t)-31,(uint8_t)-31,(uint8_t)-32,(uint8_t)-32,(uint8_t)-33,(uint8_t)-33,(uint8_t)-33,(uint8_t)-34,(uint8_t)-34,(uint8_t)-34,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-35,(uint8_t)-34,(uint8_t)-34,(uint8_t)-34,(uint8_t)-34,(uint8_t)-33,(uint8_t)-33,(uint8_t)-32,(uint8_t)-32,(uint8_t)-31,(uint8_t)-31,(uint8_t)-30,(uint8_t)-30,(uint8_t)-29,(uint8_t)-29,(uint8_t)-28,(uint8_t)-27,(uint8_t)-27,(uint8_t)-26,(uint8_t)-25,(uint8_t)-25,(uint8_t)-24,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-20,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-13,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,
// Max -29 - Reduced version of above
// #define SCX_TABLE_WAVE_144_HI    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-14,(uint8_t)-14,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-24,(uint8_t)-24,(uint8_t)-25,(uint8_t)-25,(uint8_t)-25,(uint8_t)-26,(uint8_t)-26,(uint8_t)-27,(uint8_t)-27,(uint8_t)-27,(uint8_t)-28,(uint8_t)-28,(uint8_t)-28,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-29,(uint8_t)-28,(uint8_t)-28,(uint8_t)-28,(uint8_t)-28,(uint8_t)-27,(uint8_t)-27,(uint8_t)-26,(uint8_t)-26,(uint8_t)-26,(uint8_t)-25,(uint8_t)-25,(uint8_t)-24,(uint8_t)-24,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,


// Max -23
#define SCX_TABLE_WAVE_72_MED    (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-5,(uint8_t)-6,(uint8_t)-7,(uint8_t)-8,(uint8_t)-9,(uint8_t)-10,(uint8_t)-11,(uint8_t)-12,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-16,(uint8_t)-17,(uint8_t)-18,(uint8_t)-19,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-19,(uint8_t)-18,(uint8_t)-17,(uint8_t)-16,(uint8_t)-15,(uint8_t)-14,(uint8_t)-13,(uint8_t)-12,(uint8_t)-11,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-7,(uint8_t)-6,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,
// Reduced version of above, seems like it's not required after improving hitbox Y alignment fairness
// Max -17 TODO: Remove
// #define SCX_TABLE_WAVE_72_MED (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-9,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-13,(uint8_t)-14,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,


// TODO: Optional Medium hight long/short wave table (has a kink in the middle of long waves
// Max -23, mixed med front + long end
// #define SCX_TABLE_WAVE_216_MIX_FRONT   (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,
// Max -23, mixed long front + med end
// #define SCX_TABLE_WAVE_216_MIX_END     (uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)0,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-4,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-5,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-6,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-8,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-9,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-10,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-11,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-12,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-13,(uint8_t)-14,(uint8_t)-14,(uint8_t)-14,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-15,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-16,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-17,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-19,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-23,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-22,(uint8_t)-21,(uint8_t)-21,(uint8_t)-21,(uint8_t)-20,(uint8_t)-20,(uint8_t)-20,(uint8_t)-19,(uint8_t)-19,(uint8_t)-18,(uint8_t)-18,(uint8_t)-18,(uint8_t)-17,(uint8_t)-17,(uint8_t)-16,(uint8_t)-16,(uint8_t)-15,(uint8_t)-15,(uint8_t)-14,(uint8_t)-14,(uint8_t)-13,(uint8_t)-13,(uint8_t)-12,(uint8_t)-12,(uint8_t)-11,(uint8_t)-11,(uint8_t)-10,(uint8_t)-10,(uint8_t)-9,(uint8_t)-8,(uint8_t)-8,(uint8_t)-7,(uint8_t)-7,(uint8_t)-7,(uint8_t)-6,(uint8_t)-6,(uint8_t)-5,(uint8_t)-5,(uint8_t)-4,(uint8_t)-4,(uint8_t)-3,(uint8_t)-3,(uint8_t)-3,(uint8_t)-2,(uint8_t)-2,(uint8_t)-2,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)-1,(uint8_t)0,(uint8_t)0,(uint8_t)0,



#define SCX_TABLE_SEG_SIZE  (144u)

// The common transition is typically SCX_TABLE_WAVE_144_LOW
// so that's interspersed between most differeing sections.
//
// The segment directly below a starting address will only
// get displayed in it's entirely ONCE. Each subsequent frame
// will display ONE LESS scanline of the segment and ONE MORE
// from the (next) segment ABOVE it.
const uint8_t scx_merged_segment_table[] = {

    // Numbers below indicate:
    // * Start address of following segment
    // * N-1 is last value previous segment

    // 0
    SCX_TABLE_WAVE_144_LOW
        // 144 x 1
        SCX_TABLE_WAVE_288_MED
    // 144 x 3 (yes, 2x since above is 288)
    SCX_TABLE_WAVE_144_LOW
        // 144 x 4
        SCX_TABLE_WAVE_144_MED
        // 144 x 5
        SCX_TABLE_WAVE_144_MED
    // 144 x 6
    SCX_TABLE_WAVE_144_LOW
        // 144 x 7
        SCX_TABLE_WAVE_144_LOW
    // 144 x 8
    SCX_TABLE_WAVE_144_LOW
        // 144 x 9
        SCX_TABLE_STRAIGHT_144
        // 144 x 10
        SCX_TABLE_STRAIGHT_144
    // 144 x 11
    SCX_TABLE_WAVE_144_LOW

        // 144 x 12
        SCX_TABLE_WAVE_72_MED  // 2x these to make 144 entry
        SCX_TABLE_WAVE_72_MED
    // 144 x 13
    SCX_TABLE_WAVE_144_LOW
    // SCX_TABLE_WAVE_144_MED

    
    //     // 144 x 14
    //     // TODO: Optional Hi - TODO: Double up Hi Section?
    //     SCX_TABLE_WAVE_144_HI
    // // 144 x 15
    // SCX_TABLE_WAVE_144_LOW
    // // 144 x 16


    // TODO: Optional Mixed
    // SCX_TABLE_WAVE_216_MIX_END
    // SCX_TABLE_WAVE_216_MIX_FRONT
    // SCX_TABLE_WAVE_144_LOW
    // // 144 x 18


};

#define SCX_TABLE_ADDR(offset_count) (scx_merged_segment_table + (offset_count * SCX_TABLE_SEG_SIZE))

// The following array has entries composed of subsets of the preceding segment table
//
// IMPORTANT: If entries changed here, update SCX_TABLE_* in lookup_tables.h
//
// Start Addr, Stop Addr, Start Seg Type, End Seg Type <-- todo, utilize these + custom tables per level + transition tables to build improved version
const scx_table_entry scx_tables[] = {

    // 0-3 (0x03 mask) Low Difficulty
    {.start_address = SCX_TABLE_ADDR(11u), .end_address = SCX_TABLE_ADDR(8u) }, // low -> straight -> straight -> low
    {.start_address = SCX_TABLE_ADDR(8u),  .end_address = SCX_TABLE_ADDR(6u) }, // low -> low -> low // Repeat 2x so it comes up more ofter
    {.start_address = SCX_TABLE_ADDR(8u),  .end_address = SCX_TABLE_ADDR(6u) }, // low -> low -> low
    {.start_address = SCX_TABLE_ADDR(3u),  .end_address = SCX_TABLE_ADDR(0u) }, // low -> long-med -> low

    // 4-7 (0x03 mask + 4u) Medium Difficulty
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low
    {.start_address = SCX_TABLE_ADDR(3u),  .end_address = SCX_TABLE_ADDR(0u) }, // low -> long-med -> low
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low

    // 8-11 (0x03 mask + 8u) Higher Difficulty
    // TODO: implement transition tables so this can use all med and no low
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low
    {.start_address = SCX_TABLE_ADDR(13u), .end_address = SCX_TABLE_ADDR(11u) }, // low -> short-med -> low
    {.start_address = SCX_TABLE_ADDR(13u), .end_address = SCX_TABLE_ADDR(11u) }, // low -> short-med -> low
    {.start_address = SCX_TABLE_ADDR(13u), .end_address = SCX_TABLE_ADDR(11u) }, // low -> short-med -> low

    // Special sgments that start or end with Straight
    // 12...
    {.start_address = SCX_TABLE_ADDR(9u),  .end_address = SCX_TABLE_ADDR(8u) }, // straight -> low
    {.start_address = SCX_TABLE_ADDR(11u), .end_address = SCX_TABLE_ADDR(10u)}, // low -> straight
    {.start_address = SCX_TABLE_ADDR(10u), .end_address = SCX_TABLE_ADDR(9u) }, // straight -> straight

// TODO: Hi? Reduce max range from
    // {.start_address = SCX_TABLE_ADDR(15u), .end_address = SCX_TABLE_ADDR(13u) }, // low -> hi -> low
// TODO: Optional
    // {.start_address = SCX_TABLE_ADDR(17u), .end_address = SCX_TABLE_ADDR(13u) }, // low -> mixed front -> mixed end -> low


};

const scx_table_level_entry scx_table_level[] = {
    {.mask = 0x03u, .base = 0u},   // 0-3  // Easy
    {.mask = 0x07u, .base = 0u},   // 0-7
    {.mask = 0x03u, .base = 4u},   // 4-7  // Medium
    {.mask = 0x07u, .base = 4u},   // 4-11
    {.mask = 0x03u, .base = 8u},   // 8-11 // Harder
};

