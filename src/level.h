
#ifndef LEVEL_H
#define LEVEL_H


#define LEVEL_OBSTACLES_TILL_NEXT  20u // TODO: Debug - Finalize
// #define LEVEL_OBSTACLES_TILL_NEXT  20u // This works if there are more levels. Right now it gets brutal pretty fast
#define LEVEL_OBSTACLES_TILL_NEXT_RESET (LEVEL_OBSTACLES_TILL_NEXT)


typedef struct level_entry {

    // Map FX
    uint8_t   mapfx_scx_table_level;       // Used to select which range of SCX table segments to for a given difficulty level
    bool      mapfx_scx_inc_every_frame;   // Whether current scx wave table is scrolled every frame, or every other frame

    // Obstacles
    uint8_t   obst_qty_max;     // Controls max number of spawned obstacles
    uint16_t  obst_speed;       // Speed of obstacles incremented per frame

    uint8_t   obst_dist_min;    // Min distance between next spawned objext
    uint8_t   obst_dist_double; // Set distance when double objects are spawned (always recalc based on obstacle speed)

    uint16_t   obst_dist_min_raw;    // Value before scaling by speed

} level_entry;

extern level_entry cur_level;

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
