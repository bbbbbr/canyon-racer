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

#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

bool check_collisions(void);

extern uint8_t collision_last_type;
#define COLLISION_GET_LAST_TYPE() (collision_last_type)

extern uint8_t collision_last_index;
#define COLLISION_GET_LAST_INDEX() (collision_last_index)

#endif