
#ifndef lookup_tables_H
#define lookup_tables_H

// extern const uint8_t scx_table_1[];
// extern const uint8_t scx_table_straight[];

// extern const size_t scx_table_1_SZ;
// extern const size_t scx_table_straight_SZ;

// TODO: add level control with a mask that only selects first 2 as well
//       Easy  : 0x01 (first 2)
//       Easier: 0x03 (first 4)
//       Medium: 0x07 (first 8)
//       Harder: 0x03 << 2 needs a mask & bitshift up (second 4)
#define SCX_TABLES_RAND_MASK (0x07u) // First 4 table entries are the ones that can flow into each other

// Should match scx_tables[] in lookup_tables.c
// TODO: rename SCX_TABLE_SEG_* ?
// #define SCX_TABLE_LOW_STR_LOW        0
// #define SCX_TABLE_LOW_LOW            1
// #define SCX_TABLE_LOW_LOW         2
// #define SCX_TABLE_LOW_LONGMED_LOW    3

// #define SCX_TABLE_LOW_MED_LOW        4
// #define SCX_TABLE_LOW_LONGMED_LOW 5
//#define SCX_TABLE_LOW_LOW          6
//#define SCX_TABLE_LOW_MED_LOW      7

#define SCX_TABLE_STR_LOW            12
#define SCX_TABLE_LOW_STR            13
#define SCX_TABLE_STR_STR            14

// TODO: cleanup
// #define SCX_TABLE_HI                11

// #define SCX_TABLE_LOW_SHORTMED_LOW  12

// #define SCX_TABLE_MIXED             12


typedef struct scx_table_entry {
    uint8_t * start_address;
    uint8_t * end_address;
    // uint8_t   start_seg_type;
    // uint8_t   end_seg_type;
} scx_table_entry;

typedef struct scx_table_level_entry {
    uint8_t  mask;
    uint8_t  base;
    // uint8_t   start_seg_type;
    // uint8_t   end_seg_type;
} scx_table_level_entry;


extern const scx_table_entry       scx_tables[];
extern const scx_table_level_entry scx_table_levels[];

extern const uint8_t SCX_TABLE_LEVEL_MIN;
extern const uint8_t SCX_TABLE_LEVEL_MAX;


#endif
