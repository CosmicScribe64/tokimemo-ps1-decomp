#define MAIN_API_OVERRIDE_dialogue_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 dialogue_work;

void func_80150010(void) {
    D_8015EA40 = 0x801B0000;
    D_8015EA44 = 0x801B2000;
    D_8015EA48 = 0x801B6000;
    D_8015EA4C = 0x801BA000;
    D_8015EA50 = 0x801BE000;
    D_8015EA54 = 0x801C2000;
    D_8015EA58 = 0x801C6000;
}

void func_80150080(void) {
    D_8015EA5C = (u8 *)0x801CE1A4;
    D_8015EA60 = (u8 *)0x801CE1A8;
    D_8015EA64 = (u8 *)0x801CE1E0;
    D_8015EA68 = *(s16 *)0x801CE1FC;
    D_8015EA6C = (u8 *)0x801B0000;
    D_8015EA70 = (u8 *)0x801B2000;
    D_8015EA74 = (u8 *)0x801B6000;
    D_8015EA78 = (u8 *)0x801BA000;
    D_8015EA7C = (u8 *)0x801BE000;
    D_8015EA80 = (u8 *)0x801C2000;
    D_8015EA84 = (u8 *)0x801C6000;
}

void func_80150130(void) {
    D_8015EA88 = (u8 *)0x801D2370;
    D_8015EA8C = (u8 *)0x801D2374;
    D_8015EA90 = (u8 *)0x801D23A4;
    D_8015EA94 = *(s16 *)0x801D23C0;
    D_8015EA98 = (u8 *)0x801B0000;
    D_8015EA9C = (u8 *)0x801D2000;
    D_8015EAA0 = (u8 *)0x801B2000;
    D_8015EAA4 = (u8 *)0x801B6000;
    D_8015EAA8 = (u8 *)0x801BA000;
    D_8015EAAC = (u8 *)0x801BE000;
    D_8015EAB0 = (u8 *)0x801C2000;
    D_8015EAB4 = (u8 *)0x801C6000;
    D_8015EAB8 = (u8 *)0x801CE000;
}

void func_80150200(void) {
    D_8015EABC = (u8 *)0x801D2174;
    D_8015EAC0 = (u8 *)0x801D217C;
    D_8015EAC4 = (u8 *)0x801D21A8;
    D_8015EAC8 = *(s16 *)0x801D21C4;
    D_8015EACC = (u8 *)0x801B0000;
    D_8015EAD0 = (u8 *)0x801B2000;
    D_8015EAD4 = (u8 *)0x801B6000;
    D_8015EAD8 = (u8 *)0x801BA000;
    D_8015EADC = (u8 *)0x801BE000;
    D_8015EAE0 = (u8 *)0x801C2000;
    D_8015EAE4 = (u8 *)0x801C6000;
}

void func_801502B0(void) {
    switch (dialogue_work) {
    case 2:
        func_80150BCC();
        return;
    case 3:
        func_80150ED4();
        return;
    case 4:
        func_801511B4();
        return;
    case 5:
        func_80151360();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_80150344(void) {
    D_8015E208 = D_8015DE8C;
    D_8015E20C = D_8015DFC8;
    D_8015E210 = D_8015E104;
    func_80150010();
    k_reset(0);
    message_window_set_title("園内");
    k_reset(1);
    load_palette(D_8015EA40, 0x11, 1, 2, 0);
    event_still_show(D_8015EA44, D_8015EA48, D_8015EA4C, D_8015EA50, D_8015EA54, D_8015EA58);
    event_anim_show(0, 0, 0, 0);
    game_state.girls[2].tokimeki += 3;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0x14;
    check_para_limit();
    main_step_next();
}

/* Copies display record 96 (actor_pool_96) over record 98 (D_801206D8). */
void func_80150460(void) {
    D_8015E208 = D_8015DE90;
    D_8015E20C = D_8015DFCC;
    D_8015E210 = D_8015E108;
    game_state.girls[2].tokimeki += 2;
    game_state.girls[2].syoushin -= 0xA;
    check_para_limit();
    func_80150080();
    load_palette((s32) D_8015EA6C, 0x11, 1, 2, 0);
    event_still_show((s32) D_8015EA70, (s32) D_8015EA74, (s32) D_8015EA78, (s32) D_8015EA7C, (s32) D_8015EA80, (s32) D_8015EA84);
    event_anim_show((s32) D_8015EA60, (s32) D_8015EA64, (s32) D_8015EA5C, (s32) D_8015EA68);
    ((Actor *)actor_pool)[98] = ((Actor *)actor_pool)[96];
    D_800CA360 = 1;
    main_step_next();
}

void func_801505B4(void) {
    D_8015E208 = D_8015DE94;
    D_8015E20C = D_8015DFD0;
    D_8015E210 = D_8015E10C;
    func_80150130();
    load_palette((s32) D_8015EA98, 0x11, 1, 2, 0);
    event_still_show((s32) D_8015EAA0, (s32) D_8015EAA4, (s32) D_8015EAA8, (s32) D_8015EAAC, (s32) D_8015EAB0, (s32) D_8015EAB4);
    event_anim_show((s32) D_8015EA8C, (s32) D_8015EA90, (s32) D_8015EA88, (s32) D_8015EA94);
    actor_clear(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EA8C;
    D_801206E8 = (s32) D_8015EA90;
    D_8012070C = (s32) D_8015EA88;
    D_801206EC = D_8015EA94;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    load_palette((s32) D_8015EA9C, 0x12, 1, 0, 0);
    game_state.tpage_buf[26] = 1;
    LoadSquare(0x280, 0x100, 0x40, 0x80, D_8015EAB8);
    D_80121FD4 = 0x81000000;
    D_80121FDC = 0x55;
    D_80121FDE = 0x38;
    D_80121FE2 = 0;
    D_80121FE3 = 0;
    D_80121FE4 = 0;
    D_80121FE0 = 0x1A;
    D_80121FE6 = 0x1F2;
    D_80121FE8 = 0;
    D_80121FE9 = 0;
    D_80121FEA = 0;
    D_80121FEC = 0;
    D_80121FEE = 0;
    D_80121FF0 = 0x1000;
    D_80121FF2 = 0x1000;
    D_80121FF4 = 0;
    D_80121FD8 = 5;
    D_80121FDA = -0x1E;
    game_state.quad_layer[57] = 0xC;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0xA;
    check_para_limit();
    main_step_next();
}

void func_80150890(void) {
    D_8015E208 = D_8015DE98;
    D_8015E20C = D_8015DFD4;
    D_8015E210 = D_8015E110;
    func_80150200();
    load_palette((s32) D_8015EACC, 0x11, 1, 2, 0);
    event_still_show((s32) D_8015EAD0, (s32) D_8015EAD4, (s32) D_8015EAD8, (s32) D_8015EADC, (s32) D_8015EAE0, (s32) D_8015EAE4);
    event_anim_show((s32) D_8015EAC0, (s32) D_8015EAC4, (s32) D_8015EABC, (s32) D_8015EAC8);
    D_800CA360 = 1;
    actor_clear(0x62);
    D_801206D9 = 9;
    D_801206DA = 1;
    D_80120710 = 0x41000000;
    D_801206DB = 4;
    D_801206E4 = (s32) D_8015EAC0;
    D_801206E8 = (s32) D_8015EAC4;
    D_8012070C = (s32) D_8015EABC;
    D_801206EC = D_8015EAC8;
    D_801206EE = 4;
    D_801206F0 = 0;
    D_801206DE = 1;
    D_8012071B = 0x11;
    D_801206FE = -0xA0;
    D_80120702 = -0x78;
    D_801206DF = 0;
    D_801206DD = 0;
    D_801206DC = 0;
    game_state.girls[2].tokimeki += 3;
    game_state.girls[2].affection += 2;
    game_state.girls[2].syoushin -= 0x14;
    check_para_limit();
    main_step_next();
}

void func_80150A84(void) {
    bg_read_sub2(0x4587);
    main_step_next();
}

void func_80150AAC(void) {
    bg_read_sub2(0x457D);
    main_step_next();
}

void func_80150AD4(void) {
    bg_read_sub2(0x4925);
    main_step_next();
}

void func_80150AFC(void) {
    bg_read_sub2(0x4631);
    main_step_next();
}

void func_80150B24(void) {
    bg_read_sub2(0x4845);
    main_step_next();
}

void func_80150B4C(void) {
    bg_read_sub2(0x44D6);
    main_step_next();
}

void func_80150B74(void) {
    bg_read_sub2(0x453B);
    main_step_next();
}

void func_80150B9C(void) {
    snd_command(0x203);
    bg_read_sub2(0x4520);
    main_step_next();
}

void func_80150BCC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015EAE8;
    idx = game_state.step;
    tbl.f[idx]();
    if (D_80122D04 != 0) {
        func_80150C78();
        D_800B5BD4 = 9;
        return;
    }
    D_800B5BD4 = 0xF;
}

/* Sixteen grey lines through prim_gradient_line. All nine s-registers are taken, so uopt keeps col1 in
 * memory: its stack home (0x64) and the frame size follow the declaration order. */
void func_80150C78(void) {
    s32 col0;
    s16 i;
    s32 col1;
    GameState *g = &game_state;
    u32 grey;
    s32 offset;

    grey = dec_bg_brightness / 10;
    offset = 0;
    col1 = col0 = (grey * 19 << 16) | (grey * 19 << 8) | (grey * 19);
    if (g->frame_count % 45 < 15) {
        offset = 0xAA;
    } else if (g->frame_count % 45 < 30) {
        offset = 0x155;
    }
    for (i = 0; i < 16; i++) {
        prim_gradient_line((s16)(func_800A0140((i << 8) + offset) / 20),
                      (s16)(rsin((i << 8) + offset) / 20 - 30),
                      (s16)(func_800A0140((i << 8) + offset) / 60),
                      (s16)(rsin((i << 8) + offset) / 60 - 30), col0, col1, 10);
    }
}

void func_80150E9C(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x8144);
    func_80150010();
    main_step_next();
}

void func_80150ED4(void) {
    GameState *g = &game_state; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_8015EB88;
    if (((g->year == 0x5F && g->month == 7 && g->day == 0x17) ||
         (g->year == 0x60 && g->month == 7 && g->day == 0x1C) ||
         (g->year == 0x61 && g->month == 7 && g->day == 0x1B)) &&
        dec_bg_brightness == 0x80 && D_80122D04 == 0 && g->step >= 0x1A) {
        light_effect_draw();
    }

    tbl.f[g->step](0x80);
    if (g->step >= 0xD && g->step < 0x10) {
        if (g->frame_count % 180 == 0) {
            func_8015114C();
        }
        if (g->frame_count % 180 == 0x5A) {
            func_8015116C();
        }
    }
}

void func_80151064(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8179);
    func_80150080();
    main_step_next();
}

void func_8015109C(void) {
    func_800853FC();
    D_801206DF = dec_bg_brightness;
    if ((u8) dec_bg_brightness < 8U) {
        D_801206DB &= 0xFF7F;
    }
}

void func_801510E8(void) {
    ((Actor *)actor_pool)[98].brightness = 0x80;
    ((Actor *)actor_pool)[98].draw_flags |= 0x80;
    ((Actor *)actor_pool)[98].unk_08 = 0;
    ((Actor *)actor_pool)[98].sequence_slot = 4;
    ((Actor *)actor_pool)[98].unk_18 = 0;
    ((Actor *)actor_pool)[98].flags = 1;
    main_step_next();
}

void func_8015114C(void) {
    snd_command(0x500);
}

void func_8015116C(void) {
    snd_command(0x503);
}

void func_8015118C(void) {
    snd_command(0x504);
    main_step_next();
}

/* D_80121FD4 is record 57 of the SpriteQuad table and D_801206D8 record 98 of the actor_pool display
 * records; the chain assignment keeps the loaded byte in one register for all four stores. */
void func_801511B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015EC24;
    idx = game_state.step;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        ((Actor *)actor_pool)[98].draw_flags |= 0x80;
        if (game_state.frame_count % 40 < 20) {
            sprite_quads[57].flags &= 0x7FFFFFFF;
        } else {
            sprite_quads[57].flags |= 0x80000000;
        }
        sprite_quads[57].tint_r = sprite_quads[57].tint_g = sprite_quads[57].tint_b =
            ((Actor *)actor_pool)[98].brightness = dec_bg_brightness;
        return;
    }
    ((Actor *)actor_pool)[98].draw_flags &= 0xFF7F;
    sprite_quads[57].flags |= 0x80000000;
}

void func_80151300(void) {
    snd_command(0x502);
    main_step_next();
}

void func_80151328(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x81B6);
    func_80150130();
    main_step_next();
}

void func_80151360(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_8015ECC4;
    idx = game_state.step;
    tbl.f[idx](0x80);
    if (D_80122D04 != 0) {
        D_801206DB |= 0x80;
        D_801206DF = dec_bg_brightness;
        return;
    }
    D_801206DB &= 0xFF7F;
}

void func_80151424(void) {
    cd_read_sectors(0x45U, 0x801B0000, 0x81FB);
    func_80150200();
    main_step_next();
}
