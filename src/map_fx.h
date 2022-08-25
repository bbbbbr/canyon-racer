
#ifndef map_fx_H
#define map_fx_H

extern const uint8_t * p_scx_table_base;

#define SCREEN_H_PLUS_1 (145u)


// #define SCY_FX_SPEED_STOPPED
// #define SCY_FX_SPEED_SLOW
#define SCY_FX_SPEED_MED
// #define SCY_FX_SPEED_FAST  // <-- Current default
// #define SCY_FX_SPEED_MAX
// TODO: half speed steps? 1.5, 2.5, etc


// Turns on / off SCX table scrolling
#define SCX_TABLE_SCROLL


// TODO: Make speed an adjustable var
//  * 1/2/4 for Y parallax speed
//  * 1/2/4 for Y scroll speed or every/other/fourth for update masking

// Region scrolling speed
// table size MUST be multiple of SCROLL_SCX_TABLE_MAP_SPEED
#define MAPFX_SCY_SPEED_STOP    0u
#define MAPFX_SCX_SPEED_STOP    0u
#define MAPFX_SCY_SPEED_SLOW    1u
#define MAPFX_SCX_SPEED_SLOW    1u
#define MAPFX_SCY_SPEED_MED     2u
#define MAPFX_SCX_SPEED_MED     2u
#define MAPFX_SCY_SPEED_FAST    2u
#define MAPFX_SCX_SPEED_FAST    4u
#define MAPFX_SCY_SPEED_MAX     8u
#define MAPFX_SCX_SPEED_MAX     8u

#define MAPFX_SCY_SPEED_DEFAULT (MAPFX_SCY_SPEED_MED)
#define MAPFX_SCX_SPEED_DEFAULT (MAPFX_SCY_SPEED_STOP)

// Makes Canyon Shape scrolling even slower
#define SCX_TABLE_EVERY_OTHER_FRAME


extern uint8_t mapfx_scroll_y_parallax_speed;
extern uint8_t mapfx_scroll_scx_table_map_speed;
#define MAPFX_Y_PARALLAX_SPEED(NEWVAL) (mapfx_scroll_y_parallax_speed = NEWVAL)
#define MAPFX_SCX_EFFECT_SPEED(NEWVAL) (mapfx_scroll_scx_table_map_speed = NEWVAL)

void mapfx_set_intro(void);
void mapfx_set_gameplay(void);

void mapfx_scx_table_reset(void);

void mapfx_isr_enable(void);
void mapfx_isr_disable(void);

#endif