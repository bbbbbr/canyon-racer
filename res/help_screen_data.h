//AUTOGENERATED FILE FROM png2asset
#ifndef METASPRITE_help_screen_data_H
#define METASPRITE_help_screen_data_H

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

#define help_screen_data_TILE_ORIGIN 0
#define help_screen_data_TILE_W 8
#define help_screen_data_TILE_H 8
#define help_screen_data_WIDTH 160
#define help_screen_data_HEIGHT 144
#define help_screen_data_TILE_COUNT 70
#define help_screen_data_PALETTE_COUNT 1
#define help_screen_data_COLORS_PER_PALETTE 2
#define help_screen_data_TOTAL_COLORS 2
#define help_screen_data_MAP_ATTRIBUTES 0

BANKREF_EXTERN(help_screen_data)

extern const palette_color_t help_screen_data_palettes[2];
extern const uint8_t help_screen_data_tiles[560];

extern const unsigned char help_screen_data_map[360];
#define help_screen_data_map_attributes help_screen_data_map

#endif
