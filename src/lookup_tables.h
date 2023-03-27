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

#ifndef LOOKUP_TABLES_H
#define LOOKUP_TABLES_H


// These need to match scx_table_levels[]
#define MAPFX_LVL_EASY    0u
#define MAPFX_LVL_MED_LO  1u
#define MAPFX_LVL_MED     2u
#define MAPFX_LVL_MED_HI  3u
#define MAPFX_LVL_HARD    4u


extern const uint8_t SCX_TABLE_STR_LOW;
extern const uint8_t SCX_TABLE_STR_STR;



typedef struct scx_table_entry {
    uint8_t * start_address;
    uint8_t * end_address;
    // uint8_t   start_seg_type;
    // uint8_t   end_seg_type;
} scx_table_entry;

typedef struct scx_table_level_entry {
    uint8_t  mask;
    uint8_t  base;
} scx_table_level_entry;


extern const scx_table_entry       scx_tables[];
extern const scx_table_level_entry scx_table_levels[];

extern const uint8_t SCX_TABLE_LEVEL_MIN;
extern const uint8_t SCX_TABLE_LEVEL_MAX;


#endif
