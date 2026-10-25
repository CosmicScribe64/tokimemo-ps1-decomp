/* game_over_flow.c: src/ovl/TT/8013A040.c:27: core state 5; body 36: o = 0x14; 37: g = tt_sys;. Original object 0x8013A040-0x8013A3E0 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_game_over_tick(void) {
    u8 *sys = tt_sys;
    u8 *core = tt_core;
    u8 *r = tt_texts + 0x9D8;
    u16 *pad = (u16 *)(sys + 0x10);

    if ((*(u16 *)(sys + 8) >> 4) & 1) {
        *(u16 *)(r + 6) = 1;
    } else {
        *(u16 *)(r + 6) = 0;
    }
    if (core[0x22] == 0) {
        if ((pad[2] & 0x800) || (pad[2] & 0x20)) {
            core[9] = 1;
        }
    }
    if (*(u16 *)(core + 0x26) >= 0x1A4) {
        core[9] = 1;
        return;
    }
    *(u16 *)(core + 0x26) += 1;
}

void tt_state_game_over(void) {
    u8 *sys;
    u8 *s;
    u8 *run;
    u8 *stage_script;
    u8 *q;
    u8 *boss;
    u32 o; /* FAKE: second spelling of the pad offset keeps the pad word in a ugen temp (t2) instead of a uopt CSE. T-11850 */

    o = 0x14;
    sys = tt_sys;
    run = tt_run;
    stage_script = (u8 *)tt_stage_script;
    s = tt_core;
    *(s32 *)stage_script += *(s32 *)run;
    switch (*(u16 *)(s + 4)) {
    case 0:
        if (s[0x22] != 0 && sys[0x52] == 0xFF) {
            audio_command(0xB1);
        } else {
            audio_command(0x22);
            audio_command(0x2F);
            audio_command(4);
            audio_command(0x203);
            run[0xF] = 1;
        }
        s[0x11] = 0;
        q = tt_texts + 0x9D8;
        *(s16 *)(q + 0xA) = -0x24;
        *(s16 *)(q + 0xC) = 0x74;
        *(s16 *)(q + 0xE) = 9;
        q[2] = 2;
        q[0] = 1;
        *(s16 *)(q + 6) = 0;
        *(s16 *)(q + 0x10) = 0;
        if (*(u16 *)(stage_script + 2) > 0x400) {
            *(s32 *)(run + 4) = 0;
        } else {
            *(s32 *)(run + 4) = 0x8000;
        }
        *(s16 *)(s + 0x26) = 0;
        *(u16 *)(s + 4) = 0x40;
        break;
    case 0x40:
        switch (*(u16 *)(s + 2)) {
        case 0x40:
        default:
            if (*(u16 *)(stage_script + 2) > 0x400) {
                boss = tt_boss;
                *(s16 *)(boss + 4) = 0;
                boss[0xC] = 0;
                /* FAKE: TT zero-store chain pair; the dead chain reloads keep ranges that move s to a1 and stage_script to v1. T-11860 */
                *(u16 *)(boss + 6) = *(u16 *)(boss + 8) = *(u16 *)(boss + 0xA) = 0;
                boss[0xD] = boss[0xE] = boss[0xF] = 0;
                *(u16 *)(s + 2) = 0x50;
                }
            break;
        case 0x50:
            if (*(u16 *)(stage_script + 0x1E) == 0) {
                (*(void (**)(void))(stage_script + 0xC))();
            }
            break;
        }
        tt_game_over_tick();
        if (s[9] != 0) {
            if (s[0x22] != 0) {
                if (sys[0x52] == 0xFF) {
                    *(s16 *)s = 1;
                } else {
                    *(s16 *)s = 8;
                }
            } else {
                *(s16 *)s = 7;
            }
            /* FAKE: TT zero-store chain pair; the dead chain reloads keep ranges that move s to a1 and stage_script to v1. T-11860 */
            *(u16 *)(s + 2) = *(u16 *)(s + 4) = *(u16 *)(s + 6) = 0;
            s[9] = s[0xA] = s[0xB] = 0;
        }
        break;
    }
    if (s[0x22] != 0 && sys[0x52] == 0xFF && ((*(u16 *)(sys + o) & 0x800) || (*(u16 *)(sys + 0x14) & 0x20))) {
        *(s16 *)s = 1;
        /* FAKE: TT zero-store chain pair; the dead chain reloads keep ranges that move s to a1 and stage_script to v1. T-11860 */
        *(u16 *)(s + 2) = *(u16 *)(s + 4) = *(u16 *)(s + 6) = 0;
        s[9] = s[0xA] = s[0xB] = 0;
    }
    tt_shots_update();
    tt_bombs_update();
    tt_bomb_targets_update();
    tt_bells_update();
    tt_clouds_update();
    tt_air_enemies_update();
    tt_ground_targets_update();
    tt_enemy_shots_update();
    tt_scroll_speed_update();
    tt_speed_gauge_update();
}
