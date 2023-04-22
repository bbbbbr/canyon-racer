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

#ifndef AUDIO_H
#define AUDIO_H


// See sfx_list[]
#define SFX_PAUSE      0u
#define SFX_TITLE_EXIT 1u
#define SFX_SHIP_JUMP  2u
#define SFX_SHIP_LAND  3u
#define SFX_LEVEL_UP   4u
#define SFX_SPEED_UP   5u

// SFX TO DO:
#define SFX_GOT_ITEM        (SFX_PAUSE)

#define SFX_STATE_SAVE_OK   (SFX_PAUSE)
#define SFX_STATE_SAVE_FAIL (SFX_PAUSE)

#define SFX_STATE_RESTORE   (SFX_PAUSE)

#define SFX_MAGIC_CODE      (SFX_LEVEL_UP)

#define SFX_RESET_HISCORE   (SFX_PAUSE)

#define SFX_SOUNDTEST_CURSOR (SFX_PAUSE)
// #define SFX_SHIP_CRASH 6u // Replaced with a gameover crash sound + music track



extern uint8_t sfx_test_counter;
extern uint8_t song_test_counter;


// See song_list[]
#define MUSIC_TITLE_SCREEN  0u
#define MUSIC_GAMEPLAY      1u
#define MUSIC_GAMEOVER_SONG 2u
#define MUSIC_CREDITS       3u

extern const uint8_t sfx_list_max;
extern const uint8_t music_list_max;
extern bool music_is_playing;

void audio_music_pause(void);
void audio_music_unpause(void);
void audio_music_set(uint8_t);

void audio_start_fadeout(void);

void audio_sfx_play(uint8_t);

void audio_vbl_isr(void);
void audio_init(void);

#endif
