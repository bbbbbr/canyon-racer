#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

#include <hUGEDriver.h>
#include <cbtfx.h>

#include "audio.h"

#include "sfx/SFX_00.h"
#include "sfx/SFX_01.h"
#include "sfx/SFX_02.h"
#include "sfx/SFX_03.h"
#include "sfx/SFX_04.h"
#include "sfx/SFX_05.h"
#include "sfx/SFX_06.h"
#include "sfx/SFX_07.h"
#include "sfx/SFX_08.h"
#include "sfx/SFX_09.h"


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


const uint8_t * sfx_list[] = { &SFX_00[0], &SFX_01[0], &SFX_02[0], &SFX_03[0], &SFX_04[0], &SFX_05[0], &SFX_06[0], &SFX_07[0], &SFX_08[0], &SFX_09[0] };

const hUGESong_t * song_list[] = { &Intro };

// Demo song
extern const hUGESong_t Intro;


bool    music_is_playing;
uint8_t sfx_enqueued;
uint8_t audio_fading_out;


// Runs as the last part of VBL
void audio_vbl_isr() {

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
    music_is_playing = true;
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
    // CBTFX_init(sfx_list[sfx_id]);
}


void audio_sfx_play_increment(void) {

    static uint8_t sfx_test_counter = 0u;

    // CBTFX_init(sfx_list[sfx_test_counter++]);
    __critical {
        SFX_ENQUE(sfx_test_counter++);
    }


    if (sfx_test_counter >= ARRAY_LEN(sfx_list))
        sfx_test_counter = 0u;
}

void audio_init() {

    // Music not playing by default
    music_is_playing = false;
    sfx_enqueued     = SFX_ENQUED_NONE;
    audio_fading_out = AUDIO_FADE_DONE;

    // Enable audio output
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    __critical {
        add_VBL(audio_vbl_isr);
    }

    // Select demo song
    // audio_music_set(AUDIO_DEMO_SONG);

    //         case J_UP    : c0 ^= 1; hUGE_mute_channel(HT_CH1, c0); set_bkg_tiles(0,0,1,1,&map[c0 & 1]); waitpadup(); break;
    //         case J_DOWN  : c1 ^= 1; hUGE_mute_channel(HT_CH2, c1); set_bkg_tiles(1,0,1,1,&map[c1 & 1]); waitpadup(); break;
    //         case J_LEFT  : c2 ^= 1; hUGE_mute_channel(HT_CH3, c2); set_bkg_tiles(2,0,1,1,&map[c2 & 1]); waitpadup(); break;
    //         case J_RIGHT : c3 ^= 1; hUGE_mute_channel(HT_CH4, c3); set_bkg_tiles(3,0,1,1,&map[c3 & 1]); waitpadup(); break;
}
