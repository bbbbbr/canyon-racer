#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

#include <hUGEDriver.h>
#include <cbtfx.h>

#include "audio.h"


// Sound FX
#include "sfx/levelup.h"
#include "sfx/pause.h"
#include "sfx/shipjump.h"
#include "sfx/shipland.h"
#include "sfx/speedup.h"
#include "sfx/titleexit.h"
#include "sfx/SFX_05.h"

// Music
// Demo song
extern const hUGESong_t music_titlescreen;
extern const hUGESong_t music_gameplay;
extern const hUGESong_t music_gameover;

uint8_t sfx_test_counter = 0u;
uint8_t song_test_counter = 0u;

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
    &SFX_05[0], // crash     6  // TODO: from cbtfx demos, replace
};

const hUGESong_t * song_list[] = {
    &music_titlescreen,
    &music_gameplay,
    &music_gameover
};


bool    music_is_playing;
uint8_t sfx_enqueued;
uint8_t audio_fading_out;


// Runs as the last part of VBL
//
// Must be installed AFTER mapfx_isr_install()
//
// TODO: should this get merged into mapfx_isr_install() to save the additial calling overhead?
void audio_vbl_isr() {

    // Allow nested interrupts for this portion of VBL to allow
    // Stat LCD (Parallax Effect) ISR to interrupt it if needed
   __asm__("EI");

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
    NR52_REG = 0;  // ?
    NR52_REG = 0x80;
    NR51_REG = 0xff;
    NR50_REG = 0x77;

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

    __critical {
        hUGE_init(song_list[song_id]);
    }
}


void audio_start_fadeout(void) {
    __critical {
       audio_fading_out = AUDIO_FADE_OUT_START;
   }
}


void audio_sfx_play(uint8_t sfx_id) {

    // Optional safety test
    if (sfx_id < ARRAY_LEN(sfx_list)) {
        __critical {
            SFX_ENQUE(sfx_id);
        }
    }
}


// ====== START: DEBUG ======
// TODO: For debugging, remove

void audio_sfx_test_increment(void) {

    sfx_test_counter++;
    if (sfx_test_counter >= ARRAY_LEN(sfx_list))
        sfx_test_counter = 0u;
}

void audio_sfx_test_decrement(void) {

    if (sfx_test_counter > 0u)
        sfx_test_counter--;
    else
        sfx_test_counter = ARRAY_LEN(sfx_list) - 1u;
}


void audio_song_test_increment(void) {

    song_test_counter++;
    if (song_test_counter >= ARRAY_LEN(song_list))
        song_test_counter = 0u;
}

void audio_song_test_decrement(void) {

    if (song_test_counter > 0u)
        song_test_counter--;
    else
        song_test_counter = ARRAY_LEN(song_list) - 1u;
}

// ====== END: DEBUG ======


// Call this BEFORE mapfx_isr_install()
void audio_init() {

    // Music not playing by default
    music_is_playing = false;
    sfx_enqueued     = SFX_ENQUED_NONE;
    audio_fading_out = AUDIO_FADE_DONE;

    // Enable audio output
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    // Instead of here, the music ISR gets installed by
    // mapfx_isr_install() to ensure that the music VBL handler
    // is placed AFTER the mapfx_isr_install() vbl handler
    //
    // __critical {
    //     add_VBL(audio_vbl_isr);
    // }
}
