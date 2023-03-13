
#ifndef _LIVES_H
#define _LIVES_H

#include <gbdk/bcd.h>


#define LIVES_COUNT_MAX_BCD        0x99         // Limit of 99 in BCD
#define LIVES_COUNT_RESET          MAKE_BCD(10) // Initial number of lives / save states
#define LIVES_COUNT_SUBTRACT_ONE() bcd_sub(&lives_count, &lives_count_bcd_step_size)
#define LIVES_COUNT_ADD_ONE()      bcd_add(&lives_count, &lives_count_bcd_step_size)


extern BCD lives_count;
extern const BCD lives_count_bcd_step_size;

// Can truncate this to uint8 since value should never be over 99 BCD (0x99)
#define LIVES_COUNT_GET() ((uint8_t)lives_count)

void lives_count_reset(void);
void lives_count_increment();

void lives_display_update(void);

#endif
