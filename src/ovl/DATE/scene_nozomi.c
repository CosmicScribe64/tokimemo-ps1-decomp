/* scene_nozomi.c: src/ovl/DATE/80156600.c:7: mode 0x5E scenes of Nozomi (girl 6). Original object 0x80156600-0x80157880 (config/objects/DATE.txt). */
#define MAIN_API_OVERRIDE_talk_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 talk_work;

void func_80156600(void) {
    D_8015FCE0 = 0x801CE0DC;
    D_8015FCE4 = 0x801CE0E0;
    D_8015FCE8 = 0x801CE0F8;
    D_8015FCEC = *(s16 *)0x801CE10C;
    D_8015FCF0 = 0x801B0000;
    D_8015FCF4 = 0x801B2000;
    D_8015FCF8 = 0x801B6000;
    D_8015FCFC = 0x801BA000;
    D_8015FD00 = 0x801BE000;
    D_8015FD04 = 0x801C2000;
    D_8015FD08 = 0x801C6000;
}

void func_801566B0(void) {
    D_8015FD0C = 0x801D2100;
    D_8015FD10 = 0x801D2108;
    D_8015FD14 = 0x801D2128;
    D_8015FD18 = *(s16 *)0x801D2144;
    D_8015FD1C = 0x801B0000;
    D_8015FD20 = 0x801B2000;
    D_8015FD24 = 0x801B6000;
    D_8015FD28 = 0x801BA000;
    D_8015FD2C = 0x801BE000;
    D_8015FD30 = 0x801C2000;
    D_8015FD34 = 0x801C6000;
}

void func_80156760(void) {
    D_8015FD38 = 0x801D2240;
    D_8015FD3C = 0x801D2248;
    D_8015FD40 = 0x801D2288;
    D_8015FD44 = *(s16 *)0x801D22A0;
    D_8015FD48 = 0x801B0000;
    D_8015FD4C = 0x801B2000;
    D_8015FD50 = 0x801B6000;
    D_8015FD54 = 0x801BA000;
    D_8015FD58 = 0x801BE000;
    D_8015FD5C = 0x801C2000;
    D_8015FD60 = 0x801C6000;
}

void func_80156810(void) {
    D_8015FD64 = 0x801CE080;
    D_8015FD68 = 0x801CE084;
    D_8015FD6C = 0x801CE094;
    D_8015FD70 = *(s16 *)0x801CE0A8;
    D_8015FD74 = 0x801B0000;
    D_8015FD78 = 0x801B2000;
    D_8015FD7C = 0x801B6000;
    D_8015FD80 = 0x801BA000;
    D_8015FD84 = 0x801BE000;
    D_8015FD88 = 0x801C2000;
    D_8015FD8C = 0x801C6000;
}

void func_801568C0(void) {
    switch (talk_work) {
    case 2:
        func_8015703C();
        return;
    case 3:
        func_801571D4();
        return;
    case 4:
        func_8015750C();
        return;
    case 5:
        func_8015761C();
        return;
    default:
        game_mode_reset_to_title();
        return;
    }
}

void func_80156954(void) {
    D_8015E208 = D_8015DF18;
    D_8015E20C = D_8015E054;
    D_8015E210 = D_8015E190;
    func_80156600();
    load_palette(D_8015FCF0, 0x11, 1, 2, 0);
    scene_still_show(D_8015FCF4, D_8015FCF8, D_8015FCFC, D_8015FD00, D_8015FD04, D_8015FD08);
    scene_anim_show(D_8015FCE4, D_8015FCE8, D_8015FCE0, (s32) D_8015FCEC);
    game_state.girls[6].tokimeki += 1;
    game_state.girls[6].affection += 1;
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    game_state.girls[game_state.current_girl].scene_counters[0xD] += 1;
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    D_800CA224[0] = 2;
    D_800CA224[2] = 2;
    D_800CA224[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    D_800CA234[0] = 1;
    D_800CA234[2] = 1;
    D_800CA234[1] = 1;
    mode_step_next();
}

void func_80156B20(void) {
    D_8015E208 = D_8015DF1C;
    D_8015E20C = D_8015E058;
    D_8015E210 = D_8015E194;
    func_801566B0();
    load_palette(D_8015FD1C, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD20, D_8015FD24, D_8015FD28, D_8015FD2C, D_8015FD30, D_8015FD34);
    scene_anim_show(D_8015FD10, D_8015FD14, D_8015FD0C, (s32) D_8015FD18);
    D_800CA360 = 1;
    game_state.girls[6].tokimeki += 3;
    game_state.girls[6].affection += 2;
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    mode_step_next();
}

void func_80156C40(void) {
    D_8015E208 = D_8015DF20;
    D_8015E20C = D_8015E05C;
    D_8015E210 = D_8015E198;
    func_80156760();
    load_palette(D_8015FD48, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD4C, D_8015FD50, D_8015FD54, D_8015FD58, D_8015FD5C, D_8015FD60);
    scene_anim_show(D_8015FD3C, D_8015FD40, D_8015FD38, (s32) D_8015FD44);
    D_800CA360 = 1;
    game_state.girls[6].tokimeki += 2;
    game_state.girls[6].affection += 2;
    game_state.girls[6].syoushin -= 0xA;
    check_para_limit();
    mode_step_next();
}

void func_80156D60(void) {
    D_8015E208 = D_8015DF24;
    D_8015E20C = D_8015E060;
    D_8015E210 = D_8015E19C;
    if ((u8)(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2) {
        game_state.girls[6].tokimeki += 3;
        game_state.girls[6].affection += 2;
    } else {
        game_state.girls[6].tokimeki += 1;
        game_state.girls[6].affection += 3;
    }
    game_state.girls[6].syoushin -= 0x14;
    check_para_limit();
    func_80156810();
    load_palette(D_8015FD74, 0x11, 1, 2, 0);
    scene_still_show(D_8015FD78, D_8015FD7C, D_8015FD80, D_8015FD84, D_8015FD88, D_8015FD8C);
    scene_anim_show(D_8015FD68, D_8015FD6C, D_8015FD64, D_8015FD70);
    D_800CA360 = 1;
    mode_step_next();
}

void func_80156EC0(void) {
    bg_read_sub2(0x4860);
    mode_step_next();
}

void func_80156EE8(void) {
    bg_read_sub2(0x44F1);
    mode_step_next();
}

void func_80156F10(void) {
    strcpy(D_800CA1DC, "無人島");
    bg_read_sub2(0x45B6);
    mode_step_next();
}

void func_80156F4C(void) {
    bg_read_sub2(0x48CD);
    mode_step_next();
}

void func_80156F74(void) {
    bg_read_sub2(0x45A3);
    mode_step_next();
}

void func_80156F9C(void) {
    bg_read_sub2(0x48F7);
    mode_step_next();
}

void func_80156FC4(void) {
    bg_read_sub2(0x45D3);
    mode_step_next();
}

void func_80156FEC(void) {
    bg_read_sub2(0x492F);
    mode_step_next();
}

void func_80157014(void) {
    bg_read_sub2(0x4631);
    mode_step_next();
}

void func_8015703C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_8015FD90;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_801570C4(void) {
    audio_command(0x500);
    mode_step_next();
}

void func_801570EC(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8984);
    func_80156600();
    mode_step_next();
}

void func_80157124(void) {
    if (get_g_zyotai_s(game_state.current_girl) < 2U) {
        portrait_layout_set(0);
    } else {
        portrait_layout_set(get_g_zyotai_h(game_state.current_girl));
    }
    mode_step_next();
}

void func_80157184(void) {
    u8 s;

    s = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 5;
    }
    mode_step_next();
}

void func_801571D4(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl61 tbl;

    tbl = D_8015FE58;
    if (game_state.mode_step < 4 || game_state.mode_step >= 0x2C) {
        if (dec_bg_brightness == 0x80) {
            sprite_light_effect_draw();
        }
    }
    tbl.f[game_state.mode_step](0x80);
}

void func_80157284(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x89C1);
    func_801566B0();
    mode_step_next();
}

void func_801572BC(void) {
    audio_command(0x500);
    mode_step_next();
}

/* FAKE: element-zero views of the GameState bytes preserve load/store ordering (the record copy is one
 * Actor since T-20900). T-9550 */
void func_801572E4(void) {
    s16 i;

    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    D_801206EE = 4;
    D_801206DA = 9;
    for (i = 0; i < 6; i++) {
        sprite_quads[i + 4].flags |= 0x80000000;
    }
    ((Actor *)actor_pool)[96].draw_flags &= 0xFF7F;
    ((Actor *)actor_pool)[97].draw_flags &= 0xFF7F;
    D_80122CF8 = 0;
    (&game_state.clear_color_r)[0] = (&game_state.clear_color_g)[0] = (&game_state.clear_color_b)[0] = 0x7F;
    mode_step_next();
}

s32 func_801573F0(void) {
    game_state.clear_color_r += 0x10;
    game_state.clear_color_g += 0x10;
    game_state.clear_color_b += 0x10;
    if (game_state.clear_color_r >= 0xF0U) {
        mode_step_next();
    }
}

void func_8015745C(void) {
    s16 i;

    game_state.clear_color_g = 0;
    game_state.clear_color_r = 0;
    /* FAKE: the last store and the loop on one source line; as1 then places the store after the
     * loop setup as the original does (line-based scheduling, T-6070). Not a separate symbol. T-7010 */
    game_state.clear_color_b = 0; for (i = 0; i < 6; i++) {
        sprite_quads[i + 4].flags &= 0x7FFFFFFF;
    }
    ((Actor *)actor_pool_96)->draw_flags |= 0x80;
    ((Actor *)actor_pool_96)[1].draw_flags |= 0x80;
    D_80122CF8 = 1;
    mode_step_next();
}

void func_8015750C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8015FF4C;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80157594(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x8A06);
    func_80156760();
    mode_step_next();
}

void func_801575CC(void) {
    u8 s;

    s = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 3;
    }
    mode_step_next();
}

void func_8015761C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = D_8015FFF0;
    idx = game_state.mode_step;
    tbl.f[idx](0x80);
}

void func_80157698(void) {
    u8 x;
    u32 t;
    /* FAKE: constant-1 local keeps the original bne operand order (constant register first); T-17206 */
    s32 one;

    one = 1;
    func_8014C5C8();
    x = get_g_zyotai_s(GIRL_NOZOMI);
    t = x & 0x7F;
    if ((t >= 2U) && (D_800CA154 == 8) && (one == game_state.mode_step_timer.w)) {
        portrait_layout_set(2);
    } else if ((t >= 2U) && (D_800CA154 == 0xA) && (one == game_state.mode_step_timer.w)) {
        portrait_layout_set(3);
    } else if ((t >= 2U) && (D_800CA154 == 6) && (one == game_state.mode_step_timer.w)) {
        portrait_layout_set(1);
    } else if ((t >= 2U) && (D_800CA154 == 0xC) && (one == game_state.mode_step_timer.w)) {
        portrait_layout_set(1);
    }
    game_state.mode_step_timer.w += 1;
}

void func_801577C4(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8A4B);
    func_80156810();
    mode_step_next();
}

void func_801577FC(void) {
    audio_command(0x501);
    mode_step_next();
}

void func_80157824(void) {
    u8 s;

    s = get_g_zyotai_s(game_state.current_girl) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 3;
    }
    mode_step_next();
}
