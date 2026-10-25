/* frame_loop_and_pause.c: src/ovl/TT/8013E850.c:5: overlay entry and frame loop, pause, pad-sequence checks and frame begin/end; body 11: a = tt_sys; 12: b = tt_core;. Original object 0x8013E850-0x8013F040 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

/* T-11550: matched by writing the restart/loop/reset gotos as two nested for (;;) loops (break to the reset call, goto tail out): same code, but the CFG lets as1 fill the first test's delay slot with the store's lui $at (the goto form left it in the second test's slot). The 0 is still stored through another symbol (FAKE, see the line) so uopt keeps 'lbu; or; sb' for the first |=. */
void tt_main(void) {
    u8 *sys;
    u8 *b;
    u8 *run;
    u8 *video;

    sys = tt_sys;
    b = tt_core;
    run = tt_run;
    tt_state_init();
    tt_config_load();
    tt_vsync_event_init();
    tt_video_init();
    tt_vram_clear();
    tt_assets_load();
    tt_sound_banks_load();
    for (;;) {
        b[0xE] = 0;
        b[0x10] = 0;
        audio_command(0x21);
        audio_command(0x22);
        audio_command(0x2F);
        audio_command(0x21);
        audio_command(0xF1);
        cd_command_reset();
        for (;;) {
            tt_frame_begin();
            if (tt_reset_combo_poll() != 0) {
                if (sys[0x53] != 0) {
                    break;
                }
                *(u16 *)b = 8;
                *(s16 *)(b + 6) = 0;
                *(s16 *)(b + 4) = 0;
                *(s16 *)(b + 2) = 0;
                b[0xB] = 0;
                b[0xA] = 0;
                b[9] = 0;
            }
            if (b[0x11] != 0) {
                tt_pause_update();
            }
            if (b[0x10] == 0) {
                tt_state_table[*(u16 *)b]();
                *(u16 *)(sys + 0xA) += 1;
            }
            if (sys[0x55] != 0) {
                goto tail;
            }
            tt_time_text_set(*(u16 *)(run + 8), 0x27);
            *(u16 *)(sys + 8) += 1;
            tt_texts_draw();
            tt_sprites_draw();
            tt_player_draw();
            tt_options_draw();
            tt_shots_draw();
            tt_bombs_draw();
            tt_bomb_targets_draw();
            tt_air_enemies_draw();
            tt_bells_draw();
            tt_boss_draw();
            tt_clouds_draw();
            tt_ground_targets_draw();
            tt_enemy_shots_draw();
            tt_bg_draw();
            func_8009C674(0);
            func_800AD950(0);
            tt_pad_update();
            tt_frame_end();
            audio_command_pump();
            cd_command_pump();
            tt_se_flags_clear();
        }
        tt_state_clear_all();
    }
tail:
    D_800E7D3C = *(u16 *)(sys + 0x30);
    D_800E7D42 = sys[0x32];
    D_800E7D43 = sys[0x33];
    D_800E7D44 = sys[0x34];
    D_800E7D3E = *(u16 *)(sys + 0x36);
    D_800E7D45 = sys[0x38];
    D_800E7D46 = sys[0x39];
    D_800E7D47 = sys[0x3A];
    D_800E7D40 = *(u16 *)(sys + 0x3C);
    D_800E7D48 = sys[0x3E];
    D_800E7D49 = sys[0x3F];
    D_800E7D4A = sys[0x40];
    D_800E7D4B = sys[0x44];
    *(u8 *)&D_800E7D4C = sys[0x45];
    video = tt_video;
    if (sys[0x53] != 0 || sys[0x52] == 0) {
        (&D_800E7D4E)[-1] = 0; /* FAKE: same byte as ((u8 *)&D_800E7D4C)[1] through another symbol, so uopt does not forward the 0 into the first |= (the original keeps lbu; or; sb). T-11520 */
        ((u8 *)&D_800E7D4C)[1] |= (sys[0x43] != 0) ? 1 : 0;
        ((u8 *)&D_800E7D4C)[1] |= (sys[0x49] != 0) ? 2 : 0;
    }
    D_800E7D4E = run[0xB];
    EnterCriticalSection();
    CloseEvent(*(s32 *)video);
    printf("open event %x ", *(s32 *)video);
    ExitCriticalSection();
    func_8009C210(3);
    func_8009C5E0(0);
    func_8009C674(0);
    printf("return to main!!\n");
}

void tt_vsync_tick(void) {
    u8 *core = tt_core;
    u8 *run = tt_run;

    if (core[0x12] != 0) {
        if (*(u16 *)(run + 8) < 0x8CA0U) {
            *(u16 *)(run + 8) += 1;
        }
    }
}

void tt_pause_cheat_check(void) {
    s32 pad; /* FAKE: moves the spill slot of core to sp+0x28. T-11810 */
    u8 *core;

    core = tt_core;
    if ((*(u16 *)(tt_sys + 0x14) & tt_pause_code_masks[core[0x21]]) == tt_pause_code_masks[core[0x21]]) {
        core[0x20] = 0x10;
        if (++core[0x21] == 10) {
            if (*(u16 *)core == 2 && *(u16 *)(core + 2) >= 0x50) {
                tt_se_request(TT_SE_CHEAT_BOSS);
            } else {
                tt_se_request(TT_SE_CHEAT);
            }
            core[0x21] = 0;
            tt_cheat_grant_all();
        }
    } else if (*(u16 *)(*(s32 *)&tt_sys + 0x14) != 0) { /* FAKE: the s32 view stops uopt reusing the first read. T-11810 */
        core[0x21] = 0;
    }
    if (--core[0x20] == 0) {
        core[0x21] = 0;
    }
}

void tt_pause_update(void) {
    u8 *sys;
    u8 *core;
    u8 *c;

    sys = tt_sys;
    core = tt_core;
    c = tt_texts + 0x9EC;
    if (core[0x10] != 0) {
        if (*(u16 *)(sys + 0x14) & 0x800) {
            audio_command(0xF1);
            core[0x10] = 0;
            c[0] = 0;
        }
        tt_pause_cheat_check();
        if ((*(u16 *)(sys + 8) >> 4) & 1) {
            *(u16 *)(c + 6) = 1;
        } else {
            *(u16 *)(c + 6) = 0;
        }
    } else if (*(u16 *)(tt_sys + 0x14) & 0x800) {
        func_8008FD68(0);
        audio_command(0xE1);
        tt_se_request(TT_SE_PAUSE);
        core[0x10] = 1;
        *(s16 *)(c + 0xA) = -0x14;
        *(s16 *)(c + 0xC) = 0x74;
        *(s16 *)(c + 0xE) = 8;
        c[2] = 6;
        c[0] = 1;
        *(s16 *)(c + 0x10) = 0;
    }
}

s32 tt_reset_combo_poll(void) {
    u8 *core = tt_core;
    u16 *pad = (u16 *)(tt_sys + 0x10);

    if (core[0xE] != 0) {
        if ((pad[0] & 0x90C) == 0x90C && (pad[1] & 0x90C) != 0x90C) {
            core[0xF] = 1;
        }
        if (core[0xF] != 0 && (pad[0] & 0x90C) == 0x90C) {
            *(u16 *)(core + 0xC) += 1;
        } else {
            *(u16 *)(core + 0xC) = 0;
        }
    }
    if ((u32)*(u16 *)(core + 0xC) >= 0x1F) {
        return 1;
    }
    return 0;
}

void tt_frame_begin(void) {
    u8 *video = tt_video;
    u8 *p;

    *(s32 *)(video + 8) += 1;
    *(s32 *)(video + 8) &= 1;
    p = tt_frames + *(s32 *)(video + 8) * 0x80B8;
    *(s32 *)(p + 4) = *(s32 *)p;
    func_8009CAAC(p + 0x8008, 0x10);
}

void tt_frame_end(void) {
    s32 pad; /* FAKE: unused slot above p, the original frame has it (T-3330 idiom, real source unknown). T-6010 */
    u8 *p;

    p = tt_frames + *(s32 *)(tt_video + 8) * 0x80B8;
    func_8009C210(1);
    func_8009CC04(p + 0x8048);
    func_8009CCF4(p + 0x80A4);
    func_8009CB9C(p + 0x8044);
}
