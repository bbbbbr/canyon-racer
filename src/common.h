
#ifndef common_H
#define common_H

#define HORIZON_Y_START (32 - 1) // One line before desired start line

#define SPR_NUM_START            0
#define SPR_TILES_START_BOULDERS (SPR_NUM_START)
#define SPR_TILES_START_SHIP     (SPR_TILES_START_BOULDERS + sprite_boulders_TILE_COUNT)
#define SPR_TILES_START_SHIP_CANOPY (SPR_TILES_START_SHIP + sprite_ship_TILE_COUNT)
#define SPR_TILES_START_SHOTS       (SPR_TILES_START_SHIP_CANOPY + sprite_ship_canopy_TILE_COUNT)

#define PARTITION_BITSHIFT 5 // 3 bits worth of buckets
#define PARTITION_COUNT    ((255 >> PARTITION_BITSHIFT) + 1) // 8 // 4


#define SPR_STATUS_HIDDEN    0
#define SPR_STATUS_ACTIVE    1
#define SPR_STATUS_EXPLODING 2

typedef struct entity {
    uint8_t status;
    uint8_t x;
    uint8_t y_counter;
    uint8_t y_pos;
} entity;


#endif