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


#ifndef INPUT_H
#define INPUT_H

#define J_ANY (J_UP | J_DOWN | J_LEFT | J_RIGHT | J_A | J_B | J_START | J_SELECT)
#define J_WAIT_ALL_RELEASED 0xFF
#define J_WAIT_ANY_PRESSED  0x00

#define UPDATE_KEYS() previous_keys = keys; keys = joypad()
#define UPDATE_KEY_REPEAT(MASK) if (MASK & previous_keys & keys) { key_repeat_count++; } else { key_repeat_count=0; }
#define RESET_KEY_REPEAT(NEWVAL) key_repeat_count = NEWVAL

#define KEY_PRESSED(K) (keys & (K))
#define KEY_TICKED(K)   ((keys & (K)) && !(previous_keys & (K)))
#define KEY_RELEASED(K) (!(keys & (K)) && (previous_keys & (K)))

#define GET_KEYS_TICKED(K)   ((keys & ~previous_keys) & (K))

#define KEYS()   (keys)

#define ANY_KEY_PRESSED (keys)

void waitpadticked_lowcpu(UINT8 button_mask);

extern UINT8 keys;
extern UINT8 previous_keys;
extern UINT8 key_repeat_count;

#endif // INPUT_H


