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

#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

#include <hUGEDriver.h>
#include <cbtfx.h>

#include "audio.h"
#include "../mbc5_rumble.h"


// Sound FX
#include "sfx/levelup.h"
#include "sfx/pause.h"
#include "sfx/shipjump.h"
#include "sfx/shipland.h"
#include "sfx/speedup.h"
#include "sfx/titleexit.h"
// #include "sfx/SFX_05.h"

// Music
// Demo song
extern const hUGESong_t music_titlescreen;
extern const hUGESong_t music_gameplay;
extern const hUGESong_t music_gameover;
extern const hUGESong_t music_credits;

const uint8_t audio_fade_steps[] = {0b00000000, // Sound off
                                    0b00010001,
                                    0b00100010,
                                    0b00110011,
                                    0b01100110,
                                    0b01110111,
                                  //0b11111111 // Sound on highest - don't need this step
                                    };
#define AUDIO_FADE_BITSHIFT 2u
#define AUDIO_FADE_DONE 0u
#define AUDIO_FADE_OUT_START ((ARRAY_LEN(audio_fade_steps) + 1) << AUDIO_FADE_BITSHIFT) // Intentionally +1 since it decrements first then uses value

#define SFX_ENQUED_NONE    0u
#define SFX_ENQUE(sfx_num) (sfx_enqueued = sfx_num + 1u)  // Off by +1 so it can be non-zero tested
#define SFX_GET_NUM()      (sfx_enqueued - 1u)


// TODO: Reorg and name these to match, then maybe just ditch array and use include to make it all work?
const uint8_t * sfx_list[] = {
    &SFX_0D[0], // pause     0
    &SFX_0E[0], // titleexit 1
    &SFX_0F[0], // shipjump  2
    &SFX_10[0], // shipland  3
    &SFX_11[0], // levelup   4
    &SFX_12[0], // speedup   5
    // &SFX_05[0], // crash     6  // Replaced with a gameover crash sound + music track
};

const hUGESong_t * song_list[] = {
    &music_titlescreen,
    &music_gameplay,
    &music_gameover,
    &music_credits
};


const uint8_t sfx_list_max = ARRAY_LEN(sfx_list) - 1u;
const uint8_t music_list_max = ARRAY_LEN(song_list) -1u;

bool    music_is_playing;
uint8_t sfx_enqueued;
uint8_t audio_fading_out;


// Runs as the last part of VBL
//
// Must be installed AFTER mapfx_isr_install()
//
// TODO: should this get merged into mapfx_isr_install() to save the additial calling overhead?
void audio_vbl_isr(void) {

    // Allow nested interrupts for this portion of VBL to allow
    // Stat LCD (Parallax Effect) ISR to interrupt it if needed
   enable_interrupts();

    #ifdef VISUAL_DEBUG_BENCHMARK_AUDIO
        // VISUAL BENCHMARK END
        // Debug: Benchmark time left by toggling background on/off
        LCDC_REG &= ~LCDCF_BGON; // Turn BG / WIN OFF
    #endif


    // Update music driver
    if (music_is_playing) {

        hUGE_dosound();

        if (audio_fading_out) {

            audio_fading_out--;
            // TODO: only write this once value, use a second timeout counter or other method
            NR50_REG = audio_fade_steps[audio_fading_out >> AUDIO_FADE_BITSHIFT];

            // // If done fading out, stop music from playing
            if (audio_fading_out == AUDIO_FADE_DONE) {
                // Fixme: faint pop at the end
                audio_music_pause();
            }
        }

    }

    // Check and update SFX
    if (sfx_enqueued) {
        CBTFX_init(sfx_list[SFX_GET_NUM()]);
        sfx_enqueued = SFX_ENQUED_NONE;
    } else {
        CBTFX_update();
    }

    #if defined(CART_mbc5_rumble)
        // Check if Rumble is running and needs to be turned off
        if (rumble_counter) {
            rumble_counter--;

            // if (rumble_counter & 0x04u)
            if ((rumble_counter & 0x07u) > 02u)  // 5/8ths duty cycle
                MBC5_RUMBLE_ON;
            else
                MBC5_RUMBLE_OFF;
        }
    #endif

    #ifdef VISUAL_DEBUG_BENCHMARK_AUDIO
        // VISUAL BENCHMARK START
        // Debug: Benchmark time left by toggling background on/off
        LCDC_REG |= LCDCF_BGON; // Toggle BG source (VBL int for toggle back on)
    #endif
}


void audio_music_pause(void) {
    __critical {
        music_is_playing = false;
    }

    // Turn off and reset sound
    NR52_REG = AUDENA_OFF;  // ??
    NR52_REG = AUDENA_ON;
    NR51_REG = AUDTERM_ALL_LEFT | AUDTERM_ALL_RIGHT;
    NR50_REG = 0x77u;
}


void audio_music_unpause(void) {

    #ifndef DEBUG_MUSIC_IS_OFF
    __critical {
        music_is_playing = true;
        // Clear any pending audio fade out
        // to avoid potential race condition
        audio_fading_out = AUDIO_FADE_DONE;
    }
    #endif
    // NR50_REG = 0x77;
}


void audio_music_set(uint8_t song_id) {

    // Having hUGE_init() in a critical section causes a noticeable
    // gfx glitch when starting the game over music right after a
    // crash. Instead just make sure music is not playing and
    // call that good enough to avoid problems.

    __critical {
        #if defined(CART_mbc5_rumble)
            if (song_id == MUSIC_GAMEOVER_SONG) {
                rumble_counter = RUMBLE_COUNT_CRASH;
                MBC5_RUMBLE_ON;
            }
        #endif

        music_is_playing = false;
    }

    hUGE_init(song_list[song_id]);

}


void audio_start_fadeout(void) {
    __critical {
       audio_fading_out = AUDIO_FADE_OUT_START;
   }
}


void audio_sfx_play(uint8_t sfx_id) {

    if (sfx_id < ARRAY_LEN(sfx_list)) {
        __critical {
            #if defined(CART_mbc5_rumble)
                if (sfx_id == SFX_SHIP_LAND) {
                    rumble_counter = RUMBLE_COUNT_SHIP_LAND;
                    MBC5_RUMBLE_ON;
                }
            #endif

            SFX_ENQUE(sfx_id);
        }
    }
}



// Call this BEFORE mapfx_isr_install()
void audio_init(void) {

    __critical {
        // Music not playing by default
        music_is_playing = false;
        sfx_enqueued     = SFX_ENQUED_NONE;
        audio_fading_out = AUDIO_FADE_DONE;
    }

    // Enable audio output
    NR52_REG = AUDENA_ON;
    NR51_REG = AUDTERM_ALL_LEFT | AUDTERM_ALL_RIGHT;
    NR50_REG = 0x77u;

    // Instead of here, the music ISR gets installed by
    // mapfx_isr_install() to ensure that the music VBL handler
    // is placed AFTER the mapfx_isr_install() vbl handler
    //
    // __critical {
    //     add_VBL(audio_vbl_isr);
    // }
}
