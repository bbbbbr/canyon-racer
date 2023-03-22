
#ifndef _MBC5_RUMBLE_H
#define _MBC5_RUMBLE_H

#include <gbdk/platform.h>

// Rumble requires MBC set to one of the following:
// 0x1C   MBC-5                    RUMBLE   8 MB
// 0x1D   MBC-5   SRAM             RUMBLE   8 MB
// 0x1E   MBC-5   SRAM   BATTERY   RUMBLE   8 MB

#define RUMBLE_FRAMES(N) (N + 1u)
#define RUMBLE_COUNT_CRASH     RUMBLE_FRAMES(70) // Matches crashing sound duration
#define RUMBLE_COUNT_SHIP_LAND RUMBLE_FRAMES(5)  // 4 is a little more subtle

#define RUMBLE_COUNT_DONE      0u

#define MBC_RUMBLE_BIT  0b00001000u
#define MBC5_RUMBLE_ON  (rRAMB |= (MBC_RUMBLE_BIT))
#define MBC5_RUMBLE_OFF (rRAMB = rRAMB & ~(MBC_RUMBLE_BIT))

extern uint8_t rumble_counter;

#endif // _MBC5_RUMBLE_H