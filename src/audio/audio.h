
#ifndef AUDIO_H
#define AUDIO_H


// See sfx_list[]
#define SFX_PAUSE      0u
#define SFX_TITLE_EXIT 1u
#define SFX_SHIP_JUMP  2u
#define SFX_SHIP_LAND  3u
#define SFX_LEVEL_UP   4u
#define SFX_SPEED_UP   5u
#define SFX_SHIP_CRASH 6u



extern uint8_t sfx_test_counter;
extern uint8_t song_test_counter;


// See song_list[]
#define MUSIC_DEMO_SONG  0u

void audio_music_pause(void);
void audio_music_unpause(void);
void audio_music_set(uint8_t);

void audio_start_fadeout(void);

void audio_sfx_play(uint8_t);
void audio_sfx_test_increment(void);
void audio_sfx_test_decrement(void);
void audio_song_test_increment(void);
void audio_song_test_decrement(void);

void audio_vbl_isr();
void audio_init();

#endif
