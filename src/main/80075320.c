#define MAIN_API_OVERRIDE_func_80053418 /* this object calls it as void (no v0 result in the call block, func_800754C0); main_api.h has s32 */
#include "common.h"
#include "game.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
u8 D_800B6990[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0 };
u8 D_800B6998 = 0;
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B699C);
u8 D_800B69BC[0x14] = { "で、待ち合わせ』" };
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B69D0);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B69D8);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B69E0);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6A14);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6A58);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6A8C);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6AD8);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6B10);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6B4C);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6B8C);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6BCC);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6C00);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6C28);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6C50);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6C80);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6CB8);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6CE0);
INCLUDE_RODATA("asm/data/main/80075320.data", D_800B6CF8);
/* pointer-only label D_800B6A14 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6A58 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6A8C may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6AD8 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6B10 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6B4C may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6B8C may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6BCC may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6C00 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6C28 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6C50 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6C80 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6CB8 may be inside D_800B69E0; replace its include too. */
/* pointer-only label D_800B6CE0 may be inside D_800B69E0; replace its include too. */

void func_80053418();

void func_80075320(void) {
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630). T-17000 */
    switch (*(u8 *)&D_800E6280.stage) {
    case 0x0:
        func_80075468();
        break;
    case 0x1:
        func_80075BE8();
        break;
    case 0x5:
        func_80076F48();
        break;
    case 0x6:
        holiday_club();
        break;
    case 0x7:
        func_8007894C();
        break;
    case 0x10:
        holiday_tel();
        break;
    case 0x20:
        func_80077BE0();
        break;
    case 0x30:
        func_8005B798();
        break;
    case 0x40:
        func_8005D9B4();
        break;
    case 0xFE:
        func_80076EC0();
        break;
    }
    if (((Actor *)D_8011ECD0)[27].draw_flags & 0x80) {
        func_80067870();
    }
    message_window_show();
    func_800578F4(0);
    hizuke_show();
    parameter_show();
    func_8006BA40();
    func_80066C08(1);
}

void func_80075468(void) {
    switch (D_800E6280.step) {
    case 0:
    default:
        func_800755B4();
        break;
    case 1:
        func_80075A64();
        break;
    }
    func_80066334();
    func_80065B0C(0);
}

/* FAKE: T-24108, dead entry read of unk_03F satisfies cvt_pass's entry condition and keeps m's stack slot (placing rank/points at sp+0x4C/0x2C) so unk_03F is promoted to $v0 across the month tests and copied to $a1 in the appointment_find_today delay slot. */
s32 func_800754C0(void) {
    s32 m;
    u8 rank[16];
    s16 points[16];

    m = D_800E6280.month;
    func_80053418(rank, points);
    if (((D_800E6280.player_birthday & 0xF) != (D_800E6280.girls[0].birthday.w & 0xF)) ||
        (((u32)(D_800E6280.player_birthday << 0x17) >> 0x1B) != ((u32)(D_800E6280.girls[0].birthday.w << 0x17) >> 0x1B))) {
        return 0;
    }
    if ((D_800E6280.month != 3) && ((D_800E6280.month != 4) || ((u8)D_800E6280.day >= 4U))) {
        return 0;
    }
    if (appointment_find_today(D_800E6280.year, D_800E6280.month, D_800E6280.day) > 0) {
        return 0;
    }
    if (birth_day_check(0xF) != 1) {
        return 0;
    }
    if (get_g_zyotai_s(0) & 0x80) {
        return 1;
    }
    return 0;
}

void func_800755B4(void) {
    s32 i;
    s32 r;
    s32 n;
    u8 buf[0x104];
    u8 name[0x20];

    /* FAKE: T-20940, the clear on one source line (as1 schedules a line as a unit) gives the original's order
     * of the unrolled stores. */
    D_800B6998 = 0;
    for (i = 0; i < 7; i++) { D_800B6990[i] = 0xFF; }
    hizuke_init();
    if (func_800754C0() != 0) {
        D_80122CD4 = 0;
        D_80122CD0 = 8;
        parameter_show_init();
        parameter_show_init();
        main_state_set(0x5D);
        return;
    }
    func_80041584();
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_80066104("休日コマンド画面");
    k_disp_start(1);
    D_800E6280.icon_count = 0xE;
    func_8006509C();
    func_800654D0((s32) (u8) D_800E6280.icon_count);
    func_80075AF8();
    gnsx(D_800E6280.player_family_name);
    if (D_800E6280.player_flags_hi.h & 1) {
        D_800E6280.icon_count = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.icon_count);
        func_8005AC70(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        sndisp("一週間強制補習を しなければならない）", 0, 0x1F);
    } else if ((D_800E6280.player_flags.b[1] & 0xF) == 3) {
        D_800E6280.icon_count = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.icon_count);
        func_8005AC70(1);
        icon_can_use_set(0xD, 0);
        icon_can_use_set(7, 1);
        D_800E6280.menu_cursor_0 = 7;
        sndisp("病気で身体が動かない）", 0, 0x1F);
    } else {
        n = 0;
        if (appointment_find_today((s32) D_800E6280.year, (s32) D_800E6280.month, (s32) D_800E6280.day) > 0) {
            strcpy(buf, "今日は、");
            get_g_name(name, (s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C));
            strcat(buf, name);
            strcat(buf, "とデートだ）");
            sndisp(buf, 0, 0x1F);
            D_800E6280.menu_cursor_0 = 9;
            n = 1;
        }
        if ((func_8005352C() == 1) && (((u32) (D_800E6280.day + 6) / 7U) == 3) && (D_800E6280.weekday == 6)) {
            if (((u8) D_800E6280.month % 3U) == 0) {
                if (n != 0) {
                    strcpy(buf, "（");
                    strcat(buf, "今日は、部活の練習試合だ）");
                    set_kanji_string(-0x56, 0x42, 0U, buf, 0);
                } else {
                    strcpy(buf, "今日は、部活の練習試合だ）");
                    sndisp(buf, n, 0x1F);
                }
                n += 1;
            } else {
                if (n != 0) {
                    strcpy(buf, "（");
                    strcat(buf, "今日は、部活の練習日だ）");
                    set_kanji_string(-0x56, 0x42, 0U, buf, 0);
                } else {
                    strcpy(buf, "今日は、部活の練習日だ）");
                    sndisp(buf, n, 0x1F);
                }
                n += 1;
            }
            D_800E6280.menu_cursor_0 = 4;
        }
        if (n == 0) {
            sndisp("今日は何をしようかな？）", 0, 0x1F);
        }
    }
    k_disp_start(5);
    func_80048E78();
    func_8006BC28(0);
    parameter_show_init();
    parameter_show_init();
    message_window_init();
    r = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    if (r != -1) {
        if ((r == 0) || (r == 1)) {
            set_dec_bri(0x80U);
            dec_bg_show_set(0, r);
            func_80042808();
        }
    } else {
        func_8004284C();
    }
}

void func_80075A64(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(0, func_8005751C(0));
        if (D_800B594C == -1) {
            dec_bg_reset();
            dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
        } else {
            set_dec_bri(0x80);
            func_80042808();
        }
    }
}

void func_80075AF8(void) {
    s32 i;
    s32 n;

    if (appointment_find_today(D_800E6280.year, D_800E6280.month, D_800E6280.day) <= 0) {
        icon_can_use_set(9, 0);
    }
    icon_can_use_set(0xD, 0);
    /* the original reads the s8 field unsigned here (lbu), as 8005A0B0.c does */
    if (*(u8 *)&D_800E6280.icon_mode == 2) {
        icon_can_use_set(0xE, 0);
    }
    if (D_800E6280.weekday == 6) {
        n = 0;
        for (i = 0; i < 11; i++) {
            if (D_800E6280.clubs[i].unk_03 & 1) {
                n++;
            }
        }
        if (n >= 11) {
            icon_can_use_set(4, 0);
        }
    } else if ((D_800E6280.player_flags.h >> 12) == 11) {
        icon_can_use_set(4, 0);
    }
}

void func_80075BE8(void) {
    if (D_800E6280.step == 0) {
        func_80075C24();
    }
    func_80066334();
    func_80065B0C(0);
}

void func_80075C24(void) {
    k_disp_inc();
    func_8006BA40();
    if (func_80076000() == 0 && func_80075C84() == 0 && func_8005B0F4() == 0) {
        func_8005B1A8();
    }
}

s32 func_80075C84(void) {
    s32 i;
    s32 j;
    u32 k;
    s32 random;

    if ((((Actor *)D_8011ECD0)->x < -0x40) && (((Actor *)D_8011ECD0)->x >= -0x4F)
        && (((Actor *)D_8011ECD0)->y < 0x18) && (((Actor *)D_8011ECD0)->y >= -0xF)) {
        /* FAKE T-25050: the raw input use keeps the word and mask in
         * separate promoted registers. */
        if (D_800E6280.pad_pressed) {}
        switch (D_800E6280.pad_pressed & 0x70) {
        case 0x20:
            D_800B6990[D_800B6998] = 0;
            D_800B6998++;
            func_80044750(0x505);
            break;
        case 0x40:
            D_800B6990[D_800B6998] = 1;
            D_800B6998++;
            func_80044750(0x506);
            break;
        case 0x10:
            D_800B6990[D_800B6998] = 2;
            D_800B6998++;
            func_80044750(0x507);
            break;
        }
        if (D_800B6998 >= 7) {
            D_800B6998 = 0;
        }
        if (D_800E6280.pad_pressed & 0x70) {
            for (i = 0; i < 13; i++) {
                k = D_800B6998 + 7;
                for (j = 0; j < 7; k++, j++) {
                    if (D_800B6990[k % 7] != D_800B3778[i][j]) {
                        break;
                    }
                    if ((j == 6) && ((u32)(D_800E6280.girls[i].flags.w << 28) >> 31)) {
                        if (i != 11) {
                            if (i == 12) {
                                D_801230D0 = 11;
                                D_800E6280.current_girl = 11;
                                D_800E6280.menu_cursor_0 = 12;
                            } else {
                                D_801230D0 = i;
                                D_800E6280.current_girl = i;
                                D_800E6280.menu_cursor_0 = i + 1;
                            }
                        }
                        random = rand();
                        /* The original adds to the full first record word. */
                        *(s32 *)&D_800E6280.girls[i].tokimeki_frac += random & 0x7FFF;
                        if (D_800E6280.player_flags_hi.h & 1) {
                            D_800E6280.param_stress.value += 0x20;
                        }
                        func_80042908(0x10);
                        func_80042940(2);
                        D_800E6280.substep = 11;
                        func_8004E58C();
                        icon_disp_switch(0);
                        func_80065F34(0);
                        telephone_class_init();
                        k_sub_reset_point_set();
                        k_sub_disp_start(0);
                        return 1;
                    }
                }
                /* FAKE T-25050: this impossible unsigned branch keeps the
                 * position load inside the outer loop during optimization.
                 * The branch and store disappear from the emitted code. */
                if ((u32)i < 0) {
                    D_800B6998 = 0;
                }
            }
        }
    }
    return 0;
}

s32 func_80075FA0(void) {
    u8 x;
    u16 h;

    x = D_800E6280.day_event_index;
    if (D_800E6280.day_events[x].girl != 4) {
        return D_800B699C[D_800E6280.day_events[x].girl];
    }
    h = D_800E6280.player_flags.h;
    return D_800B6780[h >> 0xC];
}

s32 func_80076000(void) {
    s32 pad[4]; /* FAKE: preserves the 0x40 frame and handled at sp+0x2C. T-24501 */
    s32 handled;
    s32 girl;
    s32 other_girl;

    /* FAKE: unsigned read of the signed state byte reproduces the original lbu. */
    if (((u8)D_800E6280.icon_mode == 2) && (D_800E6280.pad_held & 1)) {
        return 1;
    }
    /* FAKE: explicit signed-byte views preserve the selector range across packed stores. */
    handled = 0;
    if (D_800E6280.pad_pressed & 0x20) {
        if (*(s8 *)&D_800E6280.menu_cursor_0 >= 0) {
            handled = 1;
            func_80045414(9, 0, 0);
            func_80044750(3);
            if (func_8004480C() == 0) {
                func_8004500C(0, 0);
            }
        }
        switch (*(s8 *)&D_800E6280.menu_cursor_0) {
        case 4:
            if (get_icon_can_use((s32) *(s8 *)&D_800E6280.menu_cursor_0) != 0) {
                func_80044750(0x500);
                if (D_800E6280.weekday == 6) {
                    if (((u16) D_800E6280.player_flags.h >> 0xC) == 0xB) {
                        ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                        D_800E6280.day_events[D_800E6280.day_event_index].girl = *(s8 *)&D_800E6280.menu_cursor_0;
                        D_800E6280.day_events[D_800E6280.day_event_index].scene = func_8006BDA8(1);
                        func_80042908(7);
                    } else {
                        D_800E6280.day_events[D_800E6280.day_event_index].girl = *(s8 *)&D_800E6280.menu_cursor_0;
                        D_800E6280.day_events[D_800E6280.day_event_index].scene = func_8006BDA8(1);
                        ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                        girl = func_80053564((s32) ((u16) D_800E6280.player_flags.h >> 0xC));
                        /* Shift back extracts flag bit 2 and preserves ugen temporary numbering. T-24501 */
                        if ((girl != -1) && (((s32)(D_800E6280.girls[girl].flags.u << 29) >> 31) != 0)) {
                            D_800E6280.girls[girl].affection += 1;
                        }
                        func_80042908(6);
                    }
                } else {
                    other_girl = func_80053564((s32) ((u16) D_800E6280.player_flags.h >> 0xC));
                    /* Keep the shift-back extraction here for the same temporary sequence. T-24501 */
                    if ((other_girl != -1) && (((s32)(D_800E6280.girls[other_girl].flags.u << 29) >> 31) != 0)) {
                        D_800E6280.girls[other_girl].affection += 1;
                    }
                    D_800E6280.day_events[D_800E6280.day_event_index].girl = *(s8 *)&D_800E6280.menu_cursor_0;
                    D_800E6280.day_events[D_800E6280.day_event_index].scene = func_8006BDA8(1);
                    ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                    func_80042908(5);
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 5:
        case 6:
        case 7:
            if (get_icon_can_use((s32) *(s8 *)&D_800E6280.menu_cursor_0) != 0) {
                ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                func_80044750(0x500);
                D_800E6280.day_events[D_800E6280.day_event_index].girl = *(s8 *)&D_800E6280.menu_cursor_0;
                D_800E6280.day_events[D_800E6280.day_event_index].scene = func_8006BDA8(1);
                func_80042908(5);
            }
            break;
        case 9:
            if ((get_icon_can_use((s32) *(s8 *)&D_800E6280.menu_cursor_0) != 0) && (appointment_find_today((s32) D_800E6280.year, (s32) D_800E6280.month, (s32) D_800E6280.day) > 0)) {
                ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                func_80044750(0x500);
                func_80042908(0xFE);
            }
            break;
        case 8:
            if (get_icon_can_use((s32) *(s8 *)&D_800E6280.menu_cursor_0) != 0) {
                ((LastCommandBits *)&D_800E6280.last_command)->icon = *(s8 *)&D_800E6280.menu_cursor_0;
                func_80044750(0x50D);
                func_80042908(0x10);
            }
            break;
        case 10:
            if (get_icon_can_use((s32) *(s8 *)&D_800E6280.menu_cursor_0) != 0) {
                func_80044750(0x50D);
                func_80042908(0x20);
            }
            break;
        case 12:
            func_80044750(0x50D);
            func_80042908(0x30);
            break;
        case 11:
            func_80044750(0x50D);
            func_80042908(0x40);
            break;
        }
    }
    return handled;
}


s32 func_800764AC(u32 *arg0) {
    /* FAKE: T-10910, T-24135, padA places cnt at sp+0x2C within the 0xB0 frame, and if (1) around cnt[...]++ reproduces the original branch/delay-slot layout. */
    s32 padA[1];
    s32 cnt[32];
    s32 i;
    s32 n;
    s32 m;
    s32 idx;

    switch (D_800E6280.unk_54C[0].w & 0x1F) {
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return 1;
    default:
        for (i = 0; i < 32; i++) {
            cnt[i] = 0;
        }
        for (i = 0; i < 8; i++) {
            if (!((AppointmentBits *)&D_800E6280.unk_54C[i])->active) {
                break;
            }
            if (1) {
                cnt[((AppointmentBits *)&D_800E6280.unk_54C[i])->place]++;
            }
        }
        cnt[16] = 0;
        cnt[17] = 0;
        cnt[19] = 0;
        cnt[20] = 0;
        cnt[18] = 0;
        n = 0;
        m = 0;
        for (i = 0; i < 32; i++) {
            if (cnt[i] != 0) {
                n++;
            }
        }
        if (n < 2) {
            *arg0 = (u32)(D_800E6280.unk_54C[0].w << 2) >> 31;
            return 1;
        }
        for (i = 0; i < 8; i++) {
            if (!((AppointmentBits *)&D_800E6280.unk_54C[i])->active) {
                break;
            }
            if (cnt[idx = ((AppointmentBits *)&D_800E6280.unk_54C[i])->place] != 0) {
                arg0[m] = idx;
                m++;
                cnt[((AppointmentBits *)&D_800E6280.unk_54C[i])->place] = 0;
                if (m >= 3) {
                    break;
                }
            }
        }
        return n;
    }
}

void func_80076630(void) {
    /* FAKE: T-24135, 4-byte stack slot above buf[32] places buf at sp+0x2C as in the original. */
    s32 pad[1];
    u32 buf[32];

    D_800E6280.appt_place_0 = 0xFF;
    D_800E6280.appt_place_1 = 0xFF;
    D_800E6280.appt_place_2 = 0xFF;
    D_800E6280.appt_place_3 = 0xFF;
    func_8006B640();
    if (func_800764AC(buf) >= 2) {
        k_reset(1);
        k_sub_reset_point_set();
        gnsx(D_800E6280.player_family_name);
        sndisp((u8 *)"どこで、 待ち合わせだったかな？）", 0, 0x1F);
        k_sub_disp_start(1);
        func_8004284C();
        return;
    } else {
        D_800E6280.appt_place_0 = D_800E6280.unk_54C_0.unk_00;
        D_800E6280.appt_place_1 = D_800E6280.unk_54C_0.unk_00;
        D_800E6280.appt_place_2 = 0xFF;
        D_800E6280.appt_place_3 = 0xFF;
        D_800E6280.day_events[D_800E6280.day_event_index].type = 0x80;
        D_800E6280.day_events[D_800E6280.day_event_index].girl = ((AppointmentBits *)&D_800E6280.unk_54C[0])->girl;
        D_800E6280.day_events[D_800E6280.day_event_index].scene = ((AppointmentBits *)&D_800E6280.unk_54C[0])->place;
        D_800E6280.current_girl = D_800E6280.unk_54C_0.unk_25;
        D_800E6280.date_girl = D_800E6280.unk_54C_0.unk_25;
        D_800E6280.appointment_flag = ((AppointmentBits *)&D_800E6280.unk_54C[0])->flag;
    }
    func_80042940(2);
}

/* T-24501: 932-byte literal match. The split stack padding preserves the
 * original 0x198 frame: selected IDs at 0x188/0x18C/0x190, candidate IDs at
 * 0x108, and string buffers at 0x3C/0x7C/0xBC. */
void func_800767A4(void) {
    s32 top_gap; /* FAKE: reserve the original top stack word. T-24501. */
    s32 third;
    s32 second;
    s32 first;
    s32 choices[32];
    s32 list_gap; /* FAKE: reserve the original word between IDs and buffers. T-24501. */
    s32 temp_hi; /* FAKE: unused original draft scalar retained for the diagnosed stack layout. T-24501. */
    s32 var_v1_2;
    u8 text1[64];
    u8 text2[64];
    u8 text3[64];
    s32 i;
    for (i = 0; i < 64; i++) {
        text1[i] = text2[i] = text3[i] = 0;
    }
    if (func_800764AC(choices) >= 3) {
        var_v1_2 = rand() % 3;
        if (var_v1_2 < 0) {
            var_v1_2 = 0;
        }
        first = choices[var_v1_2 % 3];
        second = choices[(s32) (var_v1_2 + 1) % 3];
        third = choices[(s32) (var_v1_2 + 2) % 3];
        D_800E6280.appt_place_0 = 2;
        x_taku_menu_set(1, 3);
        strcpy(text1, "『");
        strcat(text1, D_800B342C[first]);
        strcat(text1, &D_800B69BC);
        strcpy(text2, "『");
        strcat(text2, D_800B342C[second]);
        strcat(text2, &D_800B69BC);
        strcpy(text3, "『");
        strcat(text3, D_800B342C[third]);
        strcat(text3, &D_800B69BC);
        x_taku_string_set((s32) text1, (s32) text2, (s32) text3, 3);
    } else {
        var_v1_2 = rand() & 1;
        if (var_v1_2 < 0) {
            var_v1_2 = 0;
        }
        first = choices[var_v1_2 % 2];
        second = choices[(s32) (var_v1_2 + 1) % 2];
        third = 0xFF;
        D_800E6280.appt_place_0 = 1;
        x_taku_menu_set(1, 2);
        strcpy(text1, "『");
        strcat(text1, D_800B342C[first]);
        strcat(text1, &D_800B69BC);
        strcpy(text2, "『");
        strcat(text2, D_800B342C[second]);
        strcat(text2, &D_800B69BC);
        x_taku_string_set((s32) text1, (s32) text2, 0, 2);
    }
    D_800E6280.appt_place_1 = (u8) first;
    D_800E6280.appt_place_2 = (u8) second;
    D_800E6280.appt_place_3 = (u8) third;
}

s32 func_80076B48(void) {
    GameState *g;
    g = &D_800E6280;
    switch (g->substep) {                           /* switch 1; irregular */
    case 0:                                         /* switch 1 */
        xa_wait();
        break;
    case 1:                                         /* switch 1 */
        k_sub_reset();
        func_800767A4();
        g->substep += 1;
        k_sub_disp_start(3);
        break;
    case 2:                                         /* switch 1 */
        menu_check(1, (s32) D_8011ECF6, (s32) D_8011ECFA);
        func_8004FC10(1);
        menu_bar_show(1);
        if (g->pad_pressed & 0x20) {
            switch (g->menu_cursor_1) {                   /* switch 2; irregular */
            case 0:                                 /* switch 2 */
            case 1:                                 /* switch 2 */
                /* FAKE: index the two adjacent scalar choice bytes as a table. T-9910. */
                g->appt_place_0 = *(&g->appt_place_1 + g->menu_cursor_1);
                func_8004284C();
                break;
            case 2:                                 /* switch 2 */
                if (g->appt_place_0 == 2) {
                    g->appt_place_0 = g->appt_place_3;
                    func_8004284C();
                }
                break;
            }
        }
        break;
    }
}

/* Implicit int in the original (no return value): the switch selector then stays live in $v0 (or v0,v1,zero in the
 * delay slot of the first beqz). T-17006 */
s32 func_80076C7C(void) {
    s32 pad; /* FAKE: T-17006, unused local that reproduces the original stack layout (a 4-byte slot above sp4C; the
              * buffers are otherwise 4 bytes too high); its real use is unknown. */
    u8 sp4C[0x100];
    u8 sp2C[0x20];

    switch (D_800E6280.substep) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx(D_800E6280.player_family_name);
        strcpy(sp4C, "それじゃ、 ");
        strcat(sp4C, D_800B342C[D_800E6280.appt_place_0]);
        strcat(sp4C, "で、 ");
        get_g_name(sp2C, (u32)(D_800E6280.unk_54C[0].w * 8) >> 0x1C);
        strcat(sp4C, sp2C);
        strcat(sp4C, "を待つか。）");
        sndisp(sp4C, 0, 0x1F);
        k_sub_disp_start(4);
        D_800E6280.substep += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        func_8004284C();
        return;
    }
}

void appointment_launch(void) {
    GameState *g;
    s32 i;
    s32 j;
    s32 arr[3];

    g = &D_800E6280;
    arr[0] = D_800E6280.appt_place_1;
    arr[1] = D_800E6280.appt_place_2;
    arr[2] = D_800E6280.appt_place_3;
    D_800E6280.appt_place_1 = D_800E6280.unk_54C_0.unk_00;
    for (i = 0, j = 2; i < 3; i++) {
        if (arr[i] != (g->unk_54C[0].w & 0x1F)) {
            (&D_800E6280.appt_place_0)[j] = arr[i];
            j++;
        }
    }
    D_800E6280.day_events[D_800E6280.day_event_index].type = 0x80;
    D_800E6280.day_events[D_800E6280.day_event_index].girl = D_800E6280.unk_54C_0.unk_25;
    D_800E6280.day_events[D_800E6280.day_event_index].scene = D_800E6280.unk_54C_0.unk_00;
    D_800E6280.current_girl = D_800E6280.unk_54C_0.unk_25;
    D_800E6280.date_girl = D_800E6280.unk_54C_0.unk_25;
    D_800E6280.appointment_flag = ((AppointmentBits *)&D_800E6280.unk_54C[0])->flag;
    main_state_set(0x50);
}

void func_80076EC0(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80076630();
        break;
    case 1:
        func_80076B48();
        break;
    case 2:
        func_80076C7C();
        break;
    case 3:
        appointment_launch();
        break;
    }
    func_80066334();
}

void func_80076F48(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
    case 0:
        func_8007739C();
        break;
    case 1:
        func_80077694();
        break;
    case 2:
        func_80077900();
        break;
    }
    parameter_show();
    func_80066334();
    func_80065B0C(1);
}

void func_80076FDC(void) {
    s32 activity;
    s32 col;

    D_801230D0 = 0xFF;
    if (func_80066A84() == 1) {
        activity = D_800E6280.day_events[D_800E6280.day_event_index].girl;
        if (activity == 4) {
            D_801230D0 = 0xFF;
        } else {
            D_801230D0 = D_800B69D0[activity];
        }
    } else {
        activity = D_800E6280.day_events[D_800E6280.day_event_index].girl;
        if (activity == 4) {
            D_801230D0 = 0xFF;
        } else {
            D_801230D0 = D_800B69D8[activity];
        }
    }
    if (D_801230D0 == 0xFF) {
        col = func_8006F524(4);
        if (col != 0) {
            col = 1;
        }
        if (func_80066A84() == 1) {
            switch (D_800E6280.player_flags.h >> 12) {
            case 0:
                D_801230D0 = D_800B6890[0][col];
                break;
            case 1:
                D_801230D0 = D_800B6890[1][col];
                break;
            case 2:
                D_801230D0 = D_800B6890[2][col];
                break;
            case 3:
                D_801230D0 = D_800B6890[3][col];
                break;
            case 4:
                D_801230D0 = D_800B6890[4][col];
                break;
            case 5:
                D_801230D0 = D_800B6890[5][col];
                break;
            case 6:
                D_801230D0 = D_800B6890[6][col];
                break;
            case 7:
                D_801230D0 = D_800B6890[7][col];
                break;
            case 8:
                D_801230D0 = D_800B6890[8][col];
                break;
            case 9:
                D_801230D0 = D_800B6890[9][col];
                break;
            case 10:
                D_801230D0 = D_800B6890[10][col];
                break;
            }
        } else {
            switch (D_800E6280.player_flags.h >> 12) {
            case 0:
                D_801230D0 = D_800B68A8[0][col];
                break;
            case 1:
                D_801230D0 = D_800B68A8[1][col];
                break;
            case 2:
                D_801230D0 = D_800B68A8[2][col];
                break;
            case 3:
                D_801230D0 = D_800B68A8[3][col];
                break;
            case 4:
                D_801230D0 = D_800B68A8[4][col];
                break;
            case 5:
                D_801230D0 = D_800B68A8[5][col];
                break;
            case 6:
                D_801230D0 = D_800B68A8[6][col];
                break;
            case 7:
                D_801230D0 = D_800B68A8[7][col];
                break;
            case 8:
                D_801230D0 = D_800B68A8[8][col];
                break;
            case 9:
                D_801230D0 = D_800B68A8[9][col];
                break;
            case 10:
                D_801230D0 = D_800B68A8[10][col];
                break;
            }
        }
    }
}

s32 func_80077330(void) {
    s32 r;

    if ((D_800E6280.player_flags.b[1] & 0xF) == 3) {
        r = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        r = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return r;
}

void func_8007739C(void) {
    GameState *gs = &D_800E6280;
    s32 i;
    s32 r;

    func_8004E58C();
    func_80065F34(0);
    if (gs->substep == 0) {
        for (i = 0; i < 14; i++) {
            if (i != gs->day_events[gs->day_event_index].girl) {
                icon_can_use_set(i, 0);
            } else {
                icon_can_use_set(i, 1);
            }
        }
        ((NibbleView *)&gs->last_command.b[0])->high = gs->day_events[gs->day_event_index].girl;
        gs->progress_state[0x15 + (gs->last_command.b[0] >> 4) % 9]++;
        if ((gs->last_command.b[0] >> 4) == 4) {
            if (((PlayerFlagBits *)&gs->player_flags)->club < 11) {
                gs->clubs[((PlayerFlagBits *)&gs->player_flags)->club].exp += 6;
            }
        }
        r = func_80077330();
        if (r != -1) {
            if ((r == 0) || (r == 1)) {
                dec_bg_show_set(0, r);
                gs->substep = 2;
            }
        } else {
            gs->substep = 1;
        }
    } else if (gs->substep == 1) {
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, func_8005751C(0));
            if (D_800B594C == -1) {
                dec_bg_reset();
                func_80077330();
            } else {
                gs->substep = 2;
            }
        }
    } else if (gs->substep == 2) {
        func_80076FDC();
        if (((gs->player_flags.b[1] & 0xF) == 3) && !(gs->player_flags_hi.h & 1)) {
            cd_read_sectors(9, 0x801E0000, 0x7D8A);
            gs->substep = 3;
        } else if (gs->day_events[gs->day_event_index].girl != 4) {
            cd_read_sectors(0x29, 0x801E0000, D_800B69E0[D_801230D0]);
            gs->substep = 3;
        } else {
            cd_read_sectors(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            gs->substep = 3;
        }
    } else if (gs->substep == 3) {
        if (cd_read_sync_status() & 1) {
            func_8004284C();
        }
    }
}

void func_80077694(void) {
    /* FAKE: T-20940, unused local declared first, as the sibling func_8007437C declares its record pointer; IDO gives
     * every local a slot, so temp spills at sp+0x30 as in the original (sp+0x34 without it). */
    DayEvent *rec;
    s32 temp;

    func_80048F64(0x60);
    temp = D_800E6280.day_events[D_800E6280.day_event_index].scene;
    if (((D_800E6280.player_flags.b[1] & 0xF) == 3) && !(D_800E6280.player_flags_hi.h & 1)) {
        addr_init_else_sd(2);
        temp = 0;
    } else if (D_800E6280.day_events[D_800E6280.day_event_index].girl == 4) {
        addr_init_club_sd(D_801230D0);
        if (func_8006F524(4) != 0xF) {
            ((Actor *)D_8011ECD0)[97].unk_01 = 1;
            ((Actor *)D_8011ECD0)[97].flags = 5;
            ((Actor *)D_8011ECD0)[97].quad_flags = 0x01000000;
            ((Actor *)D_8011ECD0)[97].draw_flags = 0x84;
            ((Actor *)D_8011ECD0)[97].frame_table = (u8 *)D_80125CA8;
            ((Actor *)D_8011ECD0)[97].sequence_table = (u8 *)D_80125CAC;
            ((Actor *)D_8011ECD0)[97].tpage_table = (u8 *)D_80125CB0;
            ((Actor *)D_8011ECD0)[97].unk_05 = 2;
            /* FAKE: u8 view of the s8 flag unk_F6E (both reads in this function; the original loads it with lbu).
             * Same view as in func_8007437C (800737A0.c, T-17016). Matches; real source unknown. T-20940 */
            if (*(u8 *)&D_800E6280.sd_anim_fast != 0) {
                ((Actor *)D_8011ECD0)[97].blink_period = 0x82;
            }
            ((Actor *)D_8011ECD0)[97].unk_43 = 0x10;
            ((Actor *)D_8011ECD0)[97].sequence_slot = (s16)(temp + 2);
            ((Actor *)D_8011ECD0)[97].x = -0x18;
            ((Actor *)D_8011ECD0)[97].y = -0x40;
        }
    } else {
        if ((D_801230D0 == 6) || (D_801230D0 == 7)) {
            D_800E6280.day_events[D_800E6280.day_event_index].scene = 0;
            temp = 0;
        }
        addr_init_holiday_sd(D_801230D0);
    }
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
    ((Actor *)D_8011ECD0)[96].unk_01 = 1;
    ((Actor *)D_8011ECD0)[96].flags = 5;
    ((Actor *)D_8011ECD0)[96].quad_flags = 0x01000000;
    ((Actor *)D_8011ECD0)[96].draw_flags = 0x84;
    ((Actor *)D_8011ECD0)[96].frame_table = (u8 *)D_80125CA8;
    ((Actor *)D_8011ECD0)[96].sequence_table = (u8 *)D_80125CAC;
    ((Actor *)D_8011ECD0)[96].tpage_table = (u8 *)D_80125CB0;
    ((Actor *)D_8011ECD0)[96].unk_05 = 2;
    if (*(u8 *)&D_800E6280.sd_anim_fast != 0) {
        ((Actor *)D_8011ECD0)[96].blink_period = 0x82;
    }
    ((Actor *)D_8011ECD0)[96].unk_43 = 0x10;
    ((Actor *)D_8011ECD0)[96].sequence_slot = temp;
    ((Actor *)D_8011ECD0)[96].x = -0x50;
    ((Actor *)D_8011ECD0)[96].y = -0x40;
    func_8004284C();
}

/* The original passes club to main_state_set as a second argument (the definition takes one; main_api.h keeps
 * the `()` declaration): club lives in a1 from its second read on. */
void func_80077900(void) {
    GameState *gs = &D_800E6280;
    u8 b;
    s32 girl;
    u32 club;

    if (gs->step_timer.w == 0x20) {
        switch (((Actor *)D_8011ECD0)[96].sequence_slot & 1) {
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        b = gs->day_events[gs->day_event_index].scene;
        parameter_up_down(gs->day_events[gs->day_event_index].girl, 1, b);
        parameter_show_init();
        return;
    }
    if (gs->step_timer.u <= 0x40) {
        return;
    }
    func_80048F64(0x60);
    func_80048F64(0x61);
    if ((gs->weekday == 6) && (gs->day_events[gs->day_event_index].girl == 4)
        && ((club = gs->player_flags.h >> 12) != 11) && (gs->clubs[club].exp == 6)) {
        girl = func_80053564(club);
        gs->date_girl = girl;
        if ((girl != -1) && (gs->day_count < 0x2DA)) {
            club = gs->player_flags.h >> 12;
            /* Club 10 tests the condition twice: under girl == 0, then again for every girl (the first copy falls
             * through into the second). Both copies are in the original. */
            if (club == 10) {
                if (girl == 0) {
                    if (gs->girls[girl].flags.f.b2 && gs->girls[girl].flags.f.met
                        && !((ClubFlagBits *)&gs->clubs[club])->b29
                        && !gs->girls[girl].flags.f.b18) {
                        gs->girls[girl].flags.f.b18 = 1;
                        main_state_set(0x60, club);
                        return;
                    }
                }
                if (gs->girls[girl].flags.f.b2 && gs->girls[girl].flags.f.met
                    && !((ClubFlagBits *)&gs->clubs[club])->b29
                    && !gs->girls[girl].flags.f.b18) {
                    gs->girls[girl].flags.f.b18 = 1;
                    main_state_set(0x60, club);
                    return;
                }
            } else if (gs->girls[girl].flags.f.met && gs->girls[girl].flags.f.b2
                       && !((ClubFlagBits *)&gs->clubs[club])->b29
                       && !gs->girls[girl].flags.f.b18) {
                gs->girls[girl].flags.f.b18 = 1;
                main_state_set(0x60, club);
                return;
            }
        }
    }
    func_80072B5C(0);
}

void func_80077BE0(void) {
    switch (D_800E6280.step) {
    case 0:
        func_80077C50();
        break;
    case 1:
        func_80077E30();
        break;
    case 2:
        magazine_exit();
        break;
    }
    func_80066334();
}

void func_80077C50(void) {
    func_8006A044(0, 0);
    func_8004E58C();
    func_8006612C("情報誌");
    func_8006A2CC();
    icon_disp_switch(0);
    hizuke_disp_switch(0);
    func_8004284C();
}

/* FAKE: T-24108, omitting default return keeps the selector copy in the first beq delay slot; the always-true >= 0 check in case 0x62 promotes unk_03F to $v0 across the branch and is folded away by uopt. */
u8 func_80077CA8(void) {
    switch (D_800E6280.year) {
    case 0x5F:
        if (D_800E6280.month < 5U) {
            return 0;
        }
        if (D_800E6280.month < 8U) {
            return 1;
        }
        if (D_800E6280.month < 0xBU) {
            return 2;
        }
        return 3;
    case 0x60:
        if (D_800E6280.month < 2U) {
            return 3;
        }
        if (D_800E6280.month < 5U) {
            return 4;
        }
        if (D_800E6280.month < 8U) {
            return 5;
        }
        if (D_800E6280.month < 0xBU) {
            return 6;
        }
        return 7;
    case 0x61:
        if (D_800E6280.month == 8) {
            return 0xD;
        }
        if (D_800E6280.month < 2U) {
            return 7;
        }
        if (D_800E6280.month < 5U) {
            return 8;
        }
        if (D_800E6280.month < 8U) {
            return 9;
        }
        if (D_800E6280.month < 0xBU) {
            return 0xA;
        }
        return 0xB;
    case 0x62:
        if (D_800E6280.month < 2U) {
            return 0xB;
        }
        if (D_800E6280.month >= 0) {
            return 0xC;
        }
        break;
    }
}

void func_80077E30(void) {
    magazine_base_show();
    /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in TEL
     * (T-10630) on the same field. T-17000 */
    switch (*(u8 *)&D_800E6280.substep) {
    case 0:
        xa_wait();
        return;
    case 1:
        if (D_800B3CFC != 0) {
            D_800E6280.substep += 1;
            return;
        }
        func_8004284C();
        return;
    case 2:
        k_sub_reset();
        k_sub_disp_start(0);
        gnsx(D_800E6280.player_family_name);
        sndisp(D_800B6CF8[func_80077CA8()], 0, 0x1F);
        D_800E6280.substep += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
    default:
        func_8004284C();
        return;
    }
}

void magazine_exit(void) {
    func_8006D138();
    hizuke_disp_switch(1);
    func_80042908(0);
}

void holiday_tel(void) {
    D_800E6280.step_timer.w += 1;
    switch (D_800E6280.step) {
    case 0:
        holiday_tel_init();
        break;
    case 1:
        holiday_tel_call();
        break;
    case 2:
        holiday_tel_exit();
        break;
    }
    cal_base_show();
}

void holiday_tel_init(void) {
    func_8004E58C();
    func_80048E78();
    func_80065F34(0);
    icon_disp_switch(0);
    telephone_class_init();
    k_sub_reset_point_set();
    gnsx(D_800E6280.player_family_name);
    sndi((u8 *)"誰に電話かけようかな？）", 0, 0x1F);
    k_sub_disp_start(2);
    func_8004284C();
}

void holiday_tel_call(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    k_disp_inc();
    if (D_800E6280.pad_pressed & 0x40) {
        func_80042908(0);
        return;
    }
    if ((D_800E6280.pad_pressed & 0x20) && D_800E6280.menu_cursor_0 != -1) {
        if (D_800E6280.menu_cursor_0 == 0xD) {
            func_80042908(0);
            return;
        }
        func_80044750(0x50A);
        if (D_800E6280.menu_cursor_0 == 0) {
            D_800E6280.current_girl = 0xD;
        } else if (D_800E6280.menu_cursor_0 == 0xC) {
            D_800E6280.current_girl = 0xB;
        } else {
            D_800E6280.current_girl = D_800E6280.menu_cursor_0 - 1;
        }
        func_8004284C();
    }
}

/* Implicit int: holiday_tel calls it before this definition. */
s32 holiday_tel_exit(void) {
    GameState *g = &D_800E6280;
    s32 k;

    if (g->substep != 11) {
        if ((g->step_timer.w & 7) == 7) {
            if (g->substep == 0) {
                func_80044750(0x504);
            } else if (g->substep >= 4) {
                k = g->substep - 4;
                if (g->current_girl == 13) {
                    func_80044750((u16)(D_800B3778[10][k] + 0x505));
                } else {
                    func_80044750((u16)(D_800B3778[g->current_girl][k] + 0x505));
                }
            }
            g->substep++;
        }
    } else if (g->current_girl == 13) {
        g->menu_cursor_0 = 0;
        k_sub_reset();
        restore_bgm();
        main_state_set(0x45);
        func_80042908(0);
    } else if (g->current_girl == 11) {
        k_sub_reset();
        g->menu_cursor_0 = 12;
        D_801230D0 = 11;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    } else {
        k_sub_reset();
        g->menu_cursor_0 = g->current_girl + 1;
        D_801230D0 = g->current_girl;
        restore_bgm();
        main_state_set(0x45);
        func_80042908(1);
    }
}

void telephone_class_init(void) {
    s32 i;
    /* FAKE: T-20940, a second local above the four row arrays puts them at sp+0xBC/0xA0/0x84/0x68 as in the
     * original (IDO gives every local a slot); its real use is unknown, here it holds the first row's text. */
    u8 *str;
    s16 x[14];
    s16 y[14];
    s16 w[14];
    s16 h[14];

    set_kanji_string(-0x80, -0x54, 3, "電話番号", 0);
    for (i = 0; i < 14; i++) {
        x[i] = (i / 5) * 0x58 - 0x88;
        y[i] = (i % 5) * 0x10 - 0x38;
        w[i] = 0x48;
        h[i] = 0xE;
        if (i == 0) {
            str = D_800B3538[13];
            set_kanji_string(x[i], y[i], 3, str, 0);
        } else if (i == 13) {
            y[i] += 0x10;
            set_kanji_string(x[i], y[i], 3, "取り消し", 0);
        } else if (i == 12) {
            set_kanji_string(x[i], y[i], 3, D_800B3538[12], 0);
        } else if (D_800E6280.girls[i - 1].flags.f.b3) {
            set_kanji_string(x[i], y[i], 3, D_800B3538[i - 1], 0);
        } else {
            w[i] = 0;
        }
    }
    menu_set(0, 14, x, y, w, h);
    D_800E6280.menu_cursor_0 = 0;
    func_80048F64(0x60);
    ((Actor *)D_8011ECD0)[96].unk_01 = 0;
    ((Actor *)D_8011ECD0)[96].flags = 0x40;
    ((Actor *)D_8011ECD0)[96].quad_flags = 0x40000000;
    ((Actor *)D_8011ECD0)[96].draw_flags = 0x84;
    ((Actor *)D_8011ECD0)[96].frame_table = sprite_frame_table;
    ((Actor *)D_8011ECD0)[96].sequence_table = D_800C9A14;
    ((Actor *)D_8011ECD0)[96].tpage_table = D_800C9730;
    ((Actor *)D_8011ECD0)[96].unk_14 = D_800C9A54;
    ((Actor *)D_8011ECD0)[96].unk_04 = 8;
    ((Actor *)D_8011ECD0)[96].unk_05 = 1;
    ((Actor *)D_8011ECD0)[96].sequence_slot = 0;
    ((Actor *)D_8011ECD0)[96].blink_period = 2;
    ((Actor *)D_8011ECD0)[96].unk_18 = 0x3F;
    ((Actor *)D_8011ECD0)[96].x = -0x10;
    ((Actor *)D_8011ECD0)[96].y = -0x13;
}

void holiday_club_init(void) {
    func_80065F34(0);
    func_8004E58C();
    x_taku_string_set("クラブ活動をする", "クラブをやめる", 0, 2);
    x_taku_menu_set(1, 2);
    D_800E6280.menu_cursor_1 = 0;
    k_disp_start(2);
    func_8004284C();
}

void func_8007866C(void) {
    u8 buf[72];
    GameState *gs;

    menu_check(1, (s32) ((Actor *)D_8011ECD0)->x, (s32) ((Actor *)D_8011ECD0)->y);
    menu_bar_show(1);
    func_8004FC10(1);
    if (D_800E6280.pad_pressed & 0x40) {
        func_80042908(0);
        return;
    }
    if (D_800E6280.pad_pressed & 0x20) {
        switch (D_800E6280.menu_cursor_1) {
        case 0:
            func_8004284C();
            return;
        case 1:
            func_80042940((D_800E6280.step + 2) & 0xFF);
            func_8004E58C();
            strcpy(buf, club_name_table[(u16) D_800E6280.player_flags.h >> 0xC]);
            strcat(buf, "部を退部した。");
            sndi(buf, 0, 0xFF);
            gs = &D_800E6280;
            /* FAKE: T-17006, a bit-field store through the borrowed GirlFlagBits view (one ugen temporary more than
             * `|= 1`, so the original's t6..t0 numbering); the record's real bit-field layout is unknown. */
            ((GirlFlagBits *)&gs->clubs[D_800E6280.player_flags.h >> 12].unk_03)->b0 = 1;
            gs->player_flags.b[1] = (gs->player_flags.b[1] & 0xFF0F) | 0xB0;
            k_disp_start(1);
            break;
        }
    }
}

void holiday_club_exit(void) {
    func_80048E78();
    if (func_8005352C() == 1 && (u32)(D_800E6280.day + 6) / 7 == 3 && D_800E6280.month % 3U == 0 && D_800E6280.weekday == 6) {
        main_state_set(0x73);
        return;
    }
    func_80042908(5);
}

void holiday_taibu_club_exit(void) {
    func_80048E78();
    if (D_800E6280.pad_pressed & 0x60) {
        func_80042908(0);
    }
}

s32 holiday_club(void) {
    switch (D_800E6280.step) {
    case 0:
        holiday_club_init();
        break;
    case 1:
        func_8007866C();
        break;
    case 2:
        holiday_club_exit();
        break;
    case 3:
        holiday_taibu_club_exit();
        break;
    }
}

void holiday_club_join_exit(void) {
    func_80048E78();
    icon_disp_switch(1);
    func_80042908(5);
}

void func_8007894C(void) {
    switch (D_800E6280.step) {
    case 0:
        join_club_init();
        break;
    case 1:
        join_club_select();
        break;
    case 2:
        join_club_message();
        break;
    case 3:
        holiday_club_join_exit();
        break;
    }
    cal_base_show();
}
