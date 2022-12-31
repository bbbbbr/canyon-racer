#ifndef ENTITY_COLLISIONS_H
#define ENTITY_COLLISIONS_H

bool check_collisions(void);

extern uint8_t collision_last_type;
#define COLLISION_GET_LAST_TYPE() (collision_last_type)

extern uint8_t collision_last_index;
#define COLLISION_GET_LAST_INDEX() (collision_last_index)

#endif