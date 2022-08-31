// stats.h

#ifndef _STATS_H
#define _STATS_H

#include <gbdk/bcd.h>


void stats_load(void);
void stats_reset(void);
void stats_update(BCD latest_score);


#endif // _STATS_H


