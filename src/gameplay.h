// gameplay.h

#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#define GAMEPLAY_START_RAND_INIT (0x1234u)

void gameplay_prestart(void);
void gameplay_run(void);
void gameplay_show_gameover(uint8_t spr_next_free_tile);

#endif
