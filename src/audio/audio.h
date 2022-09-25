
#ifndef AUDIO_H
#define AUDIO_H


// See sfx_list[]
#define SFX_SHIP_CRASH   05u
#define SFX_SHIP_JUMP    06u
#define SFX_EXIT_SPLASH  07u

// See song_list[]
#define MUSIC_DEMO_SONG  0u

void audio_music_pause(void);
void audio_music_unpause(void);
void audio_music_set(uint8_t);

void audio_start_fadeout(void);

void audio_sfx_play(uint8_t);
void audio_sfx_play_increment(void);

void audio_vbl_isr();
void audio_init();

#endif
