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
};

#define SCX_TABLE_ADDR(offset_count) (scx_merged_segment_table + (offset_count * SCX_TABLE_SEG_SIZE))

// The following array has entries composed of subsets of the preceding segment table
//
// IMPORTANT: If entries changed here, update SCX_TABLE_* in lookup_tables.h
//
// Start Addr, Stop Addr, Start Seg Type, End Seg Type
const scx_table_entry scx_tables[] = {

// 0-3 (0 - 3: 0x03 mask) Easier Segments
    {.start_address = SCX_TABLE_ADDR(11u), .end_address = SCX_TABLE_ADDR(8u) }, // low -> straight -> straight -> low
    {.start_address = SCX_TABLE_ADDR(8u),  .end_address = SCX_TABLE_ADDR(6u) }, // low -> low -> low // Repeat 2x so it comes up more ofter
    {.start_address = SCX_TABLE_ADDR(8u),  .end_address = SCX_TABLE_ADDR(6u) }, // low -> low -> low
    {.start_address = SCX_TABLE_ADDR(3u),  .end_address = SCX_TABLE_ADDR(0u) }, // low -> long-med -> low

// 4-7 (0 - 7: 0x07 mask) Harder Segments
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low
    {.start_address = SCX_TABLE_ADDR(3u),  .end_address = SCX_TABLE_ADDR(0u) }, // low -> long-med -> low
    {.start_address = SCX_TABLE_ADDR(8u),  .end_address = SCX_TABLE_ADDR(6u) }, // low -> low -> low
    {.start_address = SCX_TABLE_ADDR(6u),  .end_address = SCX_TABLE_ADDR(3u) }, // low -> med -> low


    // Special sgments that start or end with Straight
    {.start_address = SCX_TABLE_ADDR(9u),  .end_address = SCX_TABLE_ADDR(8u) }, // straight -> low
    {.start_address = SCX_TABLE_ADDR(11u), .end_address = SCX_TABLE_ADDR(10u)}, // low -> straight
    {.start_address = SCX_TABLE_ADDR(10u), .end_address = SCX_TABLE_ADDR(9u) }, // straight -> straight
};

