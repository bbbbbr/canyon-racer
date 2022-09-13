
#ifndef LEVEL_H
#define LEVEL_H


#define LEVEL_OBSTACLES_TILL_NEXT  10u // TODO: Debug - Finalize
// #define LEVEL_OBSTACLES_TILL_NEXT  20u // This works if there are more levels
#define LEVEL_OBSTACLES_TILL_NEXT_RESET (LEVEL_OBSTACLES_TILL_NEXT)

extern uint8_t level_count_till_next;

void level_update_vars(void);
void level_init(void);
void level_increment(void);


inline void LEVEL_INC_AND_CHECK() {

    level_count_till_next--;

    if (level_count_till_next == 0u)
        level_increment();
}



#endif
