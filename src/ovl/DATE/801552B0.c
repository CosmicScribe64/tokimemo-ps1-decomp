#define MAIN_API_OVERRIDE_dialogue_work /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

extern u32 dialogue_work;

void func_801552B0(void) {
    D_8015F900 = 0x801CE080;
    D_8015F904 = 0x801CE084;
    D_8015F908 = 0x801CE094;
    D_8015F90C = *(s16 *)0x801CE0A8;
    D_8015F910 = 0x801B0000;
    D_8015F914 = 0x801B2000;
    D_8015F918 = 0x801B6000;
    D_8015F91C = 0x801BA000;
    D_8015F920 = 0x801BE000;
    D_8015F924 = 0x801C2000;
    D_8015F928 = 0x801C6000;
}

void func_80155360(void) {
    D_8015F92C = 0x801CE31C;
    D_8015F930 = 0x801CE320;
    D_8015F934 = 0x801CE39C;
    D_8015F938 = *(s16 *)0x801CE3BC;
    D_8015F93C = 0x801B0000;
    D_8015F940 = 0x801B2000;
    D_8015F944 = 0x801B6000;
    D_8015F948 = 0x801BA000;
    D_8015F94C = 0x801BE000;
    D_8015F950 = 0x801C2000;
    D_8015F954 = 0x801C6000;
}

void func_80155410(void) {
    D_8015F958 = 0x801CE124;
    D_8015F95C = 0x801CE128;
    D_8015F960 = 0x801CE148;
    D_8015F964 = *(s16 *)0x801CE15C;
    D_8015F968 = 0x801B0000;
    D_8015F96C = 0x801B2000;
    D_8015F970 = 0x801B6000;
    D_8015F974 = 0x801BA000;
    D_8015F978 = 0x801BE000;
    D_8015F97C = 0x801C2000;
    D_8015F980 = 0x801C6000;
}

void func_801554C0(void) {
    D_8015F984 = 0x801B0000;
    D_8015F988 = 0x801B2000;
    D_8015F98C = 0x801B6000;
    D_8015F990 = 0x801BA000;
    D_8015F994 = 0x801BE000;
    D_8015F998 = 0x801C2000;
    D_8015F99C = 0x801C6000;
}

void func_80155530(void) {
    switch (dialogue_work) {
    case 3:
        func_80155D80();
        return;
    case 4:
        func_80155E40();
        return;
    case 5:
        func_801563A8();
        return;
    case 6:
        func_80156494();
        return;
    default:
        game_reset_to_title();
        return;
    }
}

void func_801555C4(void) {
    D_8015E208 = D_8015DF00;
    D_8015E20C = D_8015E03C;
    D_8015E210 = D_8015E178;
    func_801552B0();
    load_palette(D_8015F910, 0x11, 1, 2, 0);
    event_still_show(D_8015F914, D_8015F918, D_8015F91C, D_8015F920, D_8015F924, D_8015F928);
    event_anim_show(D_8015F904, D_8015F908, D_8015F900, (s32) D_8015F90C);
    game_state.girls[5].tokimeki += 2;
    game_state.girls[5].affection += 1;
    game_state.girls[5].syoushin -= 0xA;
    check_para_limit();
    main_step_next();
}

void func_801556D8(void) {
    Actor *m = (Actor *)actor_pool;
    s16 i;
    DateShorts8 sp68;
    DateShorts8 sp58;

    sp68 = D_8015F9B0;
    sp58 = D_8015F9C0;
    D_8015E208 = D_8015DF04;
    D_8015E20C = D_8015E040;
    D_8015E210 = D_8015E17C;
    game_state.girls[5].tokimeki += 2;
    game_state.girls[5].affection += 1;
    game_state.girls[5].syoushin -= 0x14;
    check_para_limit();
    func_80155360();
    load_palette(D_8015F93C, 0x11, 1, 2, 0);
    event_still_show(D_8015F940, D_8015F944, D_8015F948, D_8015F94C, D_8015F950, D_8015F954);
    event_anim_show(D_8015F930, D_8015F934, D_8015F92C, D_8015F938);
    m[96].unk_04 = m[97].unk_04 = 8;
    for (i = 0; i < 8; i++) {
        D_8015F9A0[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        m[98 + i] = m[96];
        m[98 + i].sequence_slot = 5;
        m[98 + i].x = sp68.v[i] - 0xA0;
        m[98 + i].y = sp58.v[i] - 0x78;
        D_8015F9A8[i] = rand() % 120;
    }
    m[98].sequence_slot = 4;
    main_step_next();
}

void func_801559C0(void) {
    D_8015E208 = D_8015DF08;
    D_8015E20C = D_8015E044;
    D_8015E210 = D_8015E180;
    func_80155410();
    load_palette(D_8015F968, 0x11, 1, 2, 0);
    event_still_show(D_8015F96C, D_8015F970, D_8015F974, D_8015F978, D_8015F97C, D_8015F980);
    event_anim_show(D_8015F95C, D_8015F960, D_8015F958, (s32) D_8015F964);
    game_state.girls[5].tokimeki += 3;
    game_state.girls[5].affection += 2;
    game_state.girls[5].syoushin -= 0x14;
    check_para_limit();
    main_step_next();
}

void func_80155AD4(void) {
    D_8015E208 = D_8015DF0C;
    D_8015E20C = D_8015E048;
    D_8015E210 = D_8015E184;
    func_801554C0();
    load_palette(D_8015F984, 0x11, 1, 2, 0);
    event_still_show(D_8015F988, D_8015F98C, D_8015F990, D_8015F994, D_8015F998, D_8015F99C);
    event_anim_show(0, 0, 0, 0);
    game_state.girls[5].tokimeki += 1;
    game_state.girls[5].affection += 1;
    game_state.girls[5].syoushin -= 0x14;
    check_para_limit();
    D_800CA224[0] = 2;
    D_800CA224[1] = 2;
    D_800CA224[2] = 2;
    D_800CA234[0] = 1;
    D_800CA234[1] = 1;
    D_800CA234[2] = 1;
    main_step_next();
}

void func_80155C18(void) {
    bg_read_sub2(0x48EE);
    main_step_next();
}

void func_80155C40(void) {
    bg_read_sub2(0x45D3);
    main_step_next();
}

void func_80155C68(void) {
    bg_read_sub2(0x48B5);
    main_step_next();
}

void func_80155C90(void) {
    bg_read_sub2(0x4591);
    main_step_next();
}

void func_80155CB8(void) {
    bg_read_sub2(0x47E0);
    main_step_next();
}

void func_80155CE0(void) {
    bg_read_sub2(0x42F4);
    main_step_next();
}

void func_80155D08(void) {
    bg_read_sub2(0x488A);
    main_step_next();
}

void func_80155D30(void) {
    bg_read_sub2(0x4562);
    main_step_next();
}

void func_80155D58(void) {
    bg_read_sub2(0x4558);
    main_step_next();
}

void func_80155D80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_8015F9D0;
    idx = game_state.step;
    tbl.f[idx](0x80);
}

void func_80155E08(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8816);
    func_801552B0();
    main_step_next();
}

/* int result, no return statement (K&R): $v0 stays live at the exit, so as1 keeps the second loop's counter clear below the byte test. T-17205 */
s32 func_80155E40(void) {
    GameState *g = &game_state;
    s16 i;
    FnTbl45 tbl;

    tbl = D_8015FA68;
    if (((g->year == 0x5F && g->month == 7 && g->day == 0x17) ||
         (g->year == 0x60 && g->month == 7 && g->day == 0x1C) ||
         (g->year == 0x61 && g->month == 7 && g->day == 0x1B)) &&
        dec_bg_brightness == 0x80 && D_80122D04 == 0 && g->step >= 0x21) {
        light_effect_draw();
    }
    tbl.f[g->step](0x80);
    if (func_800853FC == tbl.f[g->step]) {
        for (i = 0; i < 8; i++) {
            ((Actor *)actor_pool)[98 + i].brightness = ((Actor *)actor_pool)[96].brightness;
        }
        if (((Actor *)actor_pool)[96].brightness < 9) {
            for (i = 0; i < 8; i++) {
                ((Actor *)actor_pool)[98 + i].draw_flags &= 0x7F;
            }
        }
    }
    if (g->step >= 0x11 && g->step < 0x14) {
        func_801560E0();
    }
}

void func_80156024(void) {
    func_8014C5C8();
    if (((u16) D_800CA154 == 2) && (game_state.step_timer.w == 1)) {
        bustup_layout_set(1);
    }
    game_state.step_timer.w += 1;
}

void func_80156080(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8853);
    func_80155360();
    main_step_next();
}

void func_801560B8(void) {
    snd_command(0x502);
    main_step_next();
}

void func_801560E0(void) {
    ActorFixed *m = (ActorFixed *)actor_pool;
    u8 *st;
    DateShorts8 sp98;
    DateShorts8 sp88;
    DatePairs8 sp68;
    s16 i;

    sp98 = D_8015FB1C;
    sp88 = D_8015FB2C;
    sp68 = D_8015FB3C;
    for (i = 0; i < 8; i++) {
        st = i + D_8015F9A0;
        m[98 + i].unk_07 = m[96].unk_07;
        if (*st == 0) {
            D_8015F9A8[i]++;
        }
        if (*st == 0 && D_8015F9A8[i] >= 0x79 && (u32)(0xB4 - D_8015F9A8[i]) < (u32)(rand() % 60)) {
            m[98 + i].draw_flags |= 0x80;
            m[98 + i].flags = 5;
            *st = 1;
        }
        if (i == 0) {
            if (*st == 1 && m[98 + i].frame == 7 && m[98 + i].unk_08 == 0) {
                m[98 + i].dy = 0;
                *st = 2;
            }
        } else if (*st == 1 && m[98 + i].frame == 3 && m[98 + i].unk_08 == 7) {
            m[98 + i].dy = 0;
            *st = 2;
        }
        if (*st == 2) {
            m[98 + i].dy += 0x4000;
            m[98 + i].y.w += m[98 + i].dy;
            if (sp68.v[i][1] - 0x78 < m[98 + i].y.h[1]) {
                m[98 + i].flags = 0;
                m[98 + i].draw_flags &= 0x7F;
                m[98 + i].y.h[1] = sp88.v[i] - 0x78;
                m[98 + i].y.h[0] = 0;
                m[98 + i].frame = 0;
                m[98 + i].unk_08 = 0;
                *st = 0;
                D_8015F9A8[i] = 0;
            }
        }
    }
}

void func_801563A8(void) {
    s32 pad; /* FAKE: unused slot above tbl, the original frame has it (T-3330 idiom, real source unknown). T-8010 */
    FnTbl40 tbl;

    tbl = D_8015FB5C;
    if (game_state.step < 0xB) {
        func_801407D0();
    }
    if (game_state.step == 0xB) {
        func_80140788();
    }
    tbl.f[game_state.step](0x80);
}

void func_8015645C(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8890);
    func_80155410();
    main_step_next();
}

void func_80156494(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl56 tbl;

    tbl = D_8015FBFC;
    if (game_state.step >= 0x1E && game_state.step < 0x2C) {
        light_effect_draw();
    }
    tbl.f[game_state.step](0x80);
}

void func_80156538(void) {
    cd_read_sectors(0x35, 0x801B0000, 0x88CD);
    func_801554C0();
    main_step_next();
}

void func_80156570(void) {
    if (game_state.girls[5].flags.f.b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    main_step_next();
}

void func_801565B4(void) {
    if (!game_state.girls[5].flags.f.b14) {
        D_800CA150 = (u16)D_800CA150 + 1;
    }
    main_step_next();
}
