/* engine_initialization.c: src/ovl/TT/8013E290.c:4: VSync event setup, work-state clearing, saved configuration load and video initialization; body 8: p = (s32 *)tt_video; 10: ev = OpenEvent(0xF2000003, 2, 0x1000, tt_vsync_tick);. Original object 0x8013E290-0x8013E850 (config/objects/TT.txt). */
#include "common.h"
#include "ovl/TT.h"

void tt_vsync_event_init(void) {
    s32 *p;
    s32 ev;

    p = (s32 *)tt_video;
    EnterCriticalSection();
    ev = OpenEvent(0xF2000003, 2, 0x1000, tt_vsync_tick);
    *p = ev;
    EnableEvent(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    ExitCriticalSection();
    printf("v_ev=%d\n", *p);
}

void tt_state_clear_all(void) {
    bzero(tt_core, 0x38);
    *(s32 *)(tt_video + 8) = 0;
    bzero(tt_texts, 0xA00);
    bzero(tt_run, 0x20);
    bzero((u8 *)tt_stage_script, 0x20);
    bzero(tt_player, 0x74);
    bzero(tt_options, 0x1EC);
    bzero(tt_arms, 0x300);
    bzero(tt_shots, 0x3F0);
    bzero(tt_bombs, 0x2C0);
    bzero(tt_bomb_targets, 0x2C0);
    bzero(tt_boss, 0x424);
    bzero(tt_bells, 0xD80);
    bzero(tt_clouds, 0x270);
    bzero(tt_air_enemies, 0x1E00);
    bzero(tt_ground_targets, 0xF00);
    bzero(tt_enemy_shots, 0x3400);
    bzero(tt_sprites, 0x3200);
}

void tt_state_init(void) {
    bzero(tt_sys, 0xA8);
    bzero(tt_frames, 0x10170);
    tt_state_clear_all();
}

void tt_config_load(void) {
    u8 *sys;
    u8 *core;
    u8 *q;

    sys = tt_sys;
    core = tt_core;
    q = (u8 *)&system_data;
    sys[85] = 0;
    if (((TtConfigBits *)(q + 0x3C))->direct_mode) {
        sys[83] = 0;
    } else {
        sys[83] = 1;
    }
    sys[82] = ((TtConfigBits *)(q + 0x3C))->stage_select;
    sys[84] = ((TtConfigBits *)(q + 0x3C))->use_pad2;
    sys[53] = 0xFF;
    sys[59] = 0xFF;
    sys[65] = 0xFF;
    if (D_800E7D3C == 0) {
        *(u16 *)(sys + 48) = 0x63C8;
        sys[50] = 0x14;
        sys[51] = 0x24;
        sys[52] = 0x18;
        *(u16 *)(sys + 54) = 0x71D8;
        sys[56] = 0x17;
        sys[57] = 0x24;
        sys[58] = 0xA;
        *(u16 *)(sys + 60) = 0x7FE8;
        sys[62] = 0x16;
        sys[63] = 0x24;
        sys[64] = 0x12;
        sys[68] = 7;
        sys[69] = 6;
    } else {
        *(u16 *)(sys + 48) = D_800E7D3C;
        sys[50] = D_800E7D42;
        sys[51] = D_800E7D43;
        sys[52] = D_800E7D44;
        *(u16 *)(sys + 54) = D_800E7D3E;
        sys[56] = D_800E7D45;
        sys[57] = D_800E7D46;
        sys[58] = D_800E7D47;
        *(u16 *)(sys + 60) = D_800E7D40;
        sys[62] = D_800E7D48;
        sys[63] = D_800E7D49;
        sys[64] = D_800E7D4A;
        sys[68] = D_800E7D4B;
        sys[69] = ((TtConfigBits *)(q + 0x3C))->bomb_button;
        sys[67] = ((TtConfigBits *)(q + 0x3C))->autofire;
        if (sys[83] == 0 && sys[82] != 0) {
            sys[73] = 0;
        } else {
            sys[73] = (u8)(q[0x3D] >> 1) & 1; /* FAKE: the u8 conversion costs the ugen temporary the original skips (t8). T-11860 */
        }
    }
    if (sys[83] == 0 && sys[82] != 0) {
        core[34] = 1;
    } else {
        core[34] = 0;
    }
    *(u8 **)(sys + 76) = tt_demo_input_attract;
}

void tt_video_init(void) {
    u8 *state;
    TtFrame *frame;
    s32 x;
    s32 offset;

    frame = (TtFrame *)tt_frames;
    state = tt_sys;
    func_8009C5E0(0);
    func_8009F57C((s32)&frame[0].draw_clip, 0, 0, 320, 240);
    func_8009F57C((s32)&frame[1].draw_clip, 320, 0, 320, 240);
    if (state[73] != 0) {
        SetDefDispEnv(&frame[0].disp_rect, 352, 0, 256, 240);
        SetDefDispEnv(&frame[1].disp_rect, 32, 0, 256, 240);
    } else {
        SetDefDispEnv(&frame[0].disp_rect, 320, 0, 320, 240);
        SetDefDispEnv(&frame[1].disp_rect, 0, 0, 320, 240);
    }
    offset = 160;
    x = 32;
    do {
        frame->draw_ofs[0] = offset;
        frame->draw_clip.x = x;
        frame->draw_clip.y = 8;
        frame->draw_clip.w = 256;
        frame->draw_clip.h = 232;
        frame->unk_8060 = 0;
        frame->unk_805E = 0;
        frame->unk_805F = 1;
        frame->unk_80B4 = 0;
        frame->unk_80B5 = 0;
        x += 320;
        frame->next = (u8 *)frame + 8;
        frame++;
        offset += 320;
    } while (x != 0x2A0);
}

void tt_vram_clear(void) {
    func_8004111C();
}
