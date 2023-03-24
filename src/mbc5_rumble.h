
#ifndef _MBC5_RUMBLE_H
#define _MBC5_RUMBLE_H

#include <gbdk/platform.h>
#include <cartsave.h>

// Rumble requires MBC set to one of the following:
// 0x1C   MBC-5                    RUMBLE   8 MB
// 0x1D   MBC-5   SRAM             RUMBLE   8 MB
// 0x1E   MBC-5   SRAM   BATTERY   RUMBLE   8 MB

#define RUMBLE_FRAMES(N) (N + 1u)
#define RUMBLE_COUNT_CRASH     RUMBLE_FRAMES(70) // Matches crashing sound duration
#define RUMBLE_COUNT_SHIP_LAND RUMBLE_FRAMES(7)  // Short, single pulse with the 5/8ths duty cycle

#define RUMBLE_COUNT_DONE      0u

#define MBC_RUMBLE_BIT_ON   0b00001000u
#define MBC_RUMBLE_BIT_OFF  0b00000000u
#define MBC5_RUMBLE_ON  SWITCH_RAM(MBC_RAM_BANK_0 | MBC_RUMBLE_BIT_ON)
#define MBC5_RUMBLE_OFF SWITCH_RAM(MBC_RAM_BANK_0 | MBC_RUMBLE_BIT_OFF)

extern uint8_t rumble_counter;

#endif // _MBC5_RUMBLE_H