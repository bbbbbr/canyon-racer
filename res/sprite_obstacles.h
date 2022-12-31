//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_sprite_obstacles_H
#define METASPRITE_sprite_obstacles_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define sprite_obstacles_TILE_ORIGIN 0
#define sprite_obstacles_TILE_W 8
#define sprite_obstacles_TILE_H 16
#define sprite_obstacles_WIDTH 32
#define sprite_obstacles_HEIGHT 16
#define sprite_obstacles_TILE_COUNT 14
#define sprite_obstacles_PALETTE_COUNT 1
#define sprite_obstacles_COLORS_PER_PALETTE 4
#define sprite_obstacles_TOTAL_COLORS 4
#define sprite_obstacles_PIVOT_X -8
#define sprite_obstacles_PIVOT_Y -16
#define sprite_obstacles_PIVOT_W 32
#define sprite_obstacles_PIVOT_H 8

BANKREF_EXTERN(sprite_obstacles)

extern const palette_color_t sprite_obstacles_palettes[4];
extern const uint8_t sprite_obstacles_tiles[224];

extern const metasprite_t* const sprite_obstacles_metasprites[8];

#endif
