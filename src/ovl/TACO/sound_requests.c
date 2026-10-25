/* sound_requests.c: src/ovl/TACO/801335A0.c:4: sound effect throttling and triangle voice playback; body 11: if (shooter_background_flags != 0) {; 12: if (a == 1 && (b & 0xFF) < 0xA) {. Original object 0x801335A0-0x80133870 (config/objects/TACO.txt). */
#include "common.h"
#include "ovl/TACO.h"

s32 shooter_play_se(a, b)
s32 a;
s32 b;
{
    s32 v;
    u8 *p;

    if (shooter_background_flags != 0) {
        if (a == 1 && (b & 0xFF) < 0xA) {
            v = (b & 0xFF) & 0xF;
            p = &shooter_se_cooldown + v;
            if (*p != 0) {
                return 0;
            }
            *p = rand() % (u32)(shooter_se_cooldown_range[v] * 2 + 1);
        }
        v = b & 0xFF;
    } else {
        v = b & 0xFF;
    }
    if (a & 1) {
        audio_command((v | 0x500) & 0xFFFF);
    } else {
        audio_command((v | 0x600) & 0xFFFF);
    }
}

void shooter_tick_se_cooldowns(void) {
    s32 i;

    if (shooter_se_cooldown != 0) {
        shooter_se_cooldown -= 1;
    }
    if (shooter_se_cooldown_1 != 0) {
        shooter_se_cooldown_1 -= 1;
    }
    for (i = 0; i < 8; i++) {
        if (shooter_se_cooldown_2[i] != 0) {
            shooter_se_cooldown_2[i] -= 1;
        }
    }
}

/* The table is read as flat word pairs: `&shooter_triangle_voice_table[idx * 2]` takes the ugen temporaries of the
 * original; a pair-struct element (`&pairs[idx]`) gets them one register lower (T-16561). */
void shooter_play_triangle_voice(void) {
    s32 *p;
    s32 idx;

    idx = 3;
    if ((game_state.mode_stage & 1) && (game_state.mode_step == 1)) {
        if ((shooter_load_request == 0) && (game_state.pad_pressed & PAD_TRIANGLE) && (game_state.frame_count & 0xFF)) {
            if (cd_queue_idle() == 1) {
                if (cd_audio_state() & 2) {
                    if (shooter_actors[8].z < shooter_actors->z) {
                        idx = (rand() & 1) + 2;
                    } else if (game_state.timer_ticks & 1) {
                        idx = rand() % 6;
                    }
                    p = &shooter_triangle_voice_table[idx * 2];
                    cd_audio_request_set(p[0], p[1], 8);
                    audio_command(0x300);
                }
            }
        }
    }
}
