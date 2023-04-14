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

#ifndef ENTITY_SHIP_H
#define ENTITY_SHIP_H

 // divide by half to get pixel movement per frame after fixed point conversion
// #define SHIP_SPEED 3u
// #define SHIP_SPEED 2u
#define SHIP_SPEED 1u

#define SHIP_MOVE_AMT_X ((SHIP_SPEED) << (FIXED_BITSHIFT - 1))
#define SHIP_MOVE_AMT_Y ((SHIP_SPEED) << (FIXED_BITSHIFT - 1))

#define SHIP_X_MIN (0u)
#define SHIP_X_MAX ((DEVICE_SCREEN_PX_WIDTH - sprite_ship_PIVOT_W) << FIXED_BITSHIFT)

#define SHIP_Y_MIN ((40u) << FIXED_BITSHIFT)
#define SHIP_Y_MAX ((DEVICE_SCREEN_PX_HEIGHT - (sprite_ship_HEIGHT)) << FIXED_BITSHIFT)

#define SHIP_X_CENTERED (((DEVICE_SCREEN_PX_WIDTH - sprite_ship_PIVOT_H) / 2u) << FIXED_BITSHIFT)

#define SHIP_X_INIT (SHIP_X_CENTERED)
#define SHIP_Y_INIT (SHIP_Y_MAX)

#define SHIP_Z_MIN      0   // Ground
// #define SHIP_Z_LANDING  2u   // Landing ground is more generous
// #define SHIP_Z_MAX  60u  // Max air time

// Z axis is for jumping
#define SHIP_Z_INIT (SHIP_Z_MIN)
#define SHIP_Z_JUMP_START (SHIP_Z_MIN)


// Used for shared state testing
#define SHIP_BITS_PLAYER_ACTIVE      0x10u
#define SHIP_BITS_PLAYER_NOT_ACTIVE  0x20u

#define SHIP_STATE_ON_GROUND     (0u | SHIP_BITS_PLAYER_ACTIVE)
#define SHIP_STATE_JUMP          (1u | SHIP_BITS_PLAYER_ACTIVE)
#define SHIP_STATE_CRASHING      (2u | SHIP_BITS_PLAYER_NOT_ACTIVE)
#define SHIP_STATE_CRASH_ENDED   (3u | SHIP_BITS_PLAYER_NOT_ACTIVE)


#define SHIP_SPR_DEFAULT   0u // First sprite is neutral one
#define SHIP_SPR_LEFT      1u //
#define SHIP_SPR_RIGHT     2u //

#define SHIP_SPR_CRASH_MIN 3u // END of Crash sequence (blank)
#define SHIP_SPR_CRASH_MID 4u //
#define SHIP_SPR_CRASH_MAX 6u // START of Crash sequence

#define SHIP_SPR_JUMP      7u // Shadow sprite

// #define SHIP_SPR_JUMP_MIN 7u // END of Jump Shadow sequence
// #define SHIP_SPR_JUMP_MID 8u //
// #define SHIP_SPR_JUMP_MAX 9u // START of Jump Shadow sequence


#define SHIP_SPR_NONE      0xFFu


#define SHIP_COUNTER_STARTUP_INVINCIBLE 90u // 1.5 sec

#define SHIP_COUNTER_CRASH_FRAMES   ((SHIP_SPR_CRASH_MAX - SHIP_SPR_CRASH_MIN) + 1u)  // Should match number of crash frames
// #define SHIP_COUNTER_CRASH_BITSHIFT 4u  // Sets number of frames as power of 2: 4u = 16 frames per (1/4 sec)
// Make ship crash segment longer so there is more time to press Restore State button
#define SHIP_COUNTER_CRASH_BITSHIFT 5u  // Sets number of frames as power of 2: 5u = 32 frames per metaspr frame (1/2 sec)
#define SHIP_COUNTER_CRASH   (SHIP_COUNTER_CRASH_FRAMES << SHIP_COUNTER_CRASH_BITSHIFT)

#define SHIP_COUNTER_JUMP   30u // 1/2 sec


// ship_x & ship_y are in upper left of metasprite hit box (0, 8) of metasprite
#define SHIP_HITBOX_X_LEFT   ((sprite_ship_WIDTH - sprite_ship_PIVOT_W) / 2u)
#define SHIP_HITBOX_X_RIGHT  (SHIP_HITBOX_X_LEFT + (sprite_ship_PIVOT_W))

// Pivot W/H is hitbox size
// Actual ship is (0,0)-(31, 25)
// Make hitbox forgiving (8 x 17)
// 8 pix buffer on sides, top has a little more padding than bottom since ship front is "hollowish"
// Aligned to top instead of centered since ship isn't centered in gfx
#define SHIP_HITBOX_Y_TOP       (5u)
#define SHIP_HITBOX_Y_BOTTOM    (SHIP_HITBOX_Y_TOP + sprite_ship_PIVOT_H)
// Old, stricter hitbox:
// #define SHIP_HITBOX_Y_TOP    ((sprite_ship_HEIGHT - sprite_ship_PIVOT_H) / 2u)
// #define SHIP_HITBOX_Y_BOTTOM ((sprite_ship_PIVOT_H) + ((sprite_ship_HEIGHT - sprite_ship_PIVOT_H) / 2u))

#define SHIP_STATE_GET() (state.ship_state)
#define SHIP_STATE_SET(value) (state.ship_state = value)

void entity_ship_center_in_canyon(void);
void entity_ship_init(void);

uint8_t entity_ship_update(uint8_t);

#endif