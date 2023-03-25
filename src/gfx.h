// gfx.h

#ifndef _GFX_H
#define _GFX_H

uint8_t init_gfx_bg_mapfx(uint8_t bg_next_free_tile);
uint8_t init_gfx_sprites_gameplay(uint8_t spr_next_free_tile);

void gameplay_display_notice(uint8_t spr_next_free_tile, uint8_t tile_count, uint8_t * tiles_addr);

// In-game animated sprite notice
#define GAMEPLAY_NOTICE_TILES_PER_SPRITE 2u // 8x16 sprite mode
#define GAMEPLAY_NOTICE_HEIGHT_PX        16u // Notice is 16 pixels high
#define GAMEPLAY_NOTICE_SPRITE_Y         (((DEVICE_SCREEN_PX_HEIGHT - GAMEPLAY_NOTICE_HEIGHT_PX) / 2u) -  DEVICE_SPRITE_PX_OFFSET_Y)

#define BYTES_PER_TILE                   16u

#endif


