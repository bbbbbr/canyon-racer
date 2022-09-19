#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../common.h"

#include <hUGEDriver.h>
#include <cbtfx.h>

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



const uint8_t * sfx_list[] = { &SFX_00[0], &SFX_01[0], &SFX_02[0], &SFX_03[0], &SFX_04[0], &SFX_05[0], &SFX_06[0], &SFX_07[0], &SFX_08[0], &SFX_09[0] };

const hUGESong_t * song_list[] = { &Intro };

// Demo song
extern const hUGESong_t Intro;


bool music_is_playing;


void audio_vbl_isr() {

    // Update music driver
    if (music_is_playing)
        hUGE_dosound();

    // Check and update SFX
    CBTFX_update();
}


void audio_music_pause(void) {
    // Turn off and reset sound
    NR52_REG = 0;
    NR52_REG = 0x80;
    NR51_REG = 0xff;
    NR50_REG = 0x77;

    music_is_playing = false;
}


void audio_music_unpause(void) {
    music_is_playing = true;
}


void audio_music_set(uint8_t song_id) {
    __critical {
        hUGE_init(song_list[song_id]);
    }
}


void audio_sfx_play(uint8_t sfx_id) {

    // Optional safety test
    if (sfx_id < ARRAY_LEN(sfx_list))
        CBTFX_init(sfx_list[sfx_id]);
}


void audio_sfx_play_increment(void) {

    static uint8_t sfx_test_counter = 0u;

    CBTFX_init(sfx_list[sfx_test_counter++]);

    if (sfx_test_counter >= ARRAY_LEN(sfx_list))
        sfx_test_counter = 0u;
}

void audio_init() {

    // Music not playing by default
    music_is_playing = false;

    // Enable audio output
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    __critical {
        add_VBL(audio_vbl_isr);
    }

    // Select demo song
    // audio_music_set(MUSIC_DEMO_SONG);

    //         case J_UP    : c0 ^= 1; hUGE_mute_channel(HT_CH1, c0); set_bkg_tiles(0,0,1,1,&map[c0 & 1]); waitpadup(); break;
    //         case J_DOWN  : c1 ^= 1; hUGE_mute_channel(HT_CH2, c1); set_bkg_tiles(1,0,1,1,&map[c1 & 1]); waitpadup(); break;
    //         case J_LEFT  : c2 ^= 1; hUGE_mute_channel(HT_CH3, c2); set_bkg_tiles(2,0,1,1,&map[c2 & 1]); waitpadup(); break;
    //         case J_RIGHT : c3 ^= 1; hUGE_mute_channel(HT_CH4, c3); set_bkg_tiles(3,0,1,1,&map[c3 & 1]); waitpadup(); break;
}
