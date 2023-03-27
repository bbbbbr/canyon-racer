// Copyright 2023 (c) bbbbbr
//
// This software is licensed under:
//
// For the purposes of this project "Share Alike" shall also include
// distribution of the source code and any changes to it.
//
// CC-BY-NC-SA: Attribution Non-Commercial Share Alike Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International License
// See: http://creativecommons.org/licenses/by-nc-sa/4.0/


#ifndef _MAGIC_CODE_H
#define _MAGIC_CODE_H


#define MAGIC_CODE_STATE_RESET     0x00u
// NOTE: There are multiple counter states between
//       these two upper and lower bound #defines
#define MAGIC_CODE_STATE_ACTIVATED 0xFFu // This should be higher than the index of the last button sequence entry

extern uint8_t magic_code_state;

#define IS_MAGIC_CODE_ACTIVE (magic_code_state == MAGIC_CODE_STATE_ACTIVATED)

void magic_code_reset(void);
bool magic_code_update(void);


#endif // _MAGIC_CODE_H