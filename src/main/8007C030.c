#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8007C030", SD_GetCDLevel);

void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n) {
    u16 i;
    u16 step;
    s16 a;
    s16 b;

    i = 0;
    if (n != 0) {
        step = 0x100 / n;
        do {
            a = D_80126080[pos];
            b = D_80126080[pos + 0x200];
            if (a & 0x8000) {
                a = ~(a - 1);
            }
            if (b & 0x8000) {
                b = ~(b - 1);
            }
            if (*l < a) {
                *l = a;
            }
            if (*r < b) {
                *r = b;
            }
            pos += step;
            i++;
        } while (i < n);
    }
    *l *= 2;
    *r *= 2;
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", SD_CalcCDAve);

s16 getCDlevel(void) {
    if (D_80125D10.unk_00 & 0x400) {
        return (u32)(D_80125E60 + D_80125E62) >> 1;
    }
    return 0;
}

void addr_init_weekly_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800C9F60 + arg0 % 25 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_holiday_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800C9FF8 + arg0 % 13 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_club_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800CA048 + arg0 % 32 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_else_sd(s32 arg0) {
    u16 *p;

    D_80125CA4 = 0x801E0000;
    p = D_800CA108 + arg0 % 3 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}

void addr_init_bustup(void) {
    D_800CA120 = 0x80180084;
    D_800CA124 = 0x80180098;
    D_800CA128 = 0x8018009C;
    D_800CA130 = 0x801800AC;
}

void func_8007C784(void) {
    s32 pad; /* FAKE: unused slot above the byte locals, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;
    u8 sp2A;
    u8 sp29;

    if (func_800460CC() & 1) {
        func_8004284C();
        return;
    }
    if ((u32)D_800E6280.unk_1104.w++ >= 0x401) {
        func_800452C4();
        sp29 = D_800B5939;
        sp2A = *(u8 *)&D_800B5948;
        sp2B = D_800B593C;
        dec_bg_reset();
        *(u8 *)&D_800B5948 = sp2A;
        D_800B593C = sp2B;
        D_800B5939 = sp29;
        D_800E6280.unk_110A -= 1;
    }
}

void func_8007C844(void) {
    D_80122CF8 = 1;
    D_800B593C = 0x80;
    func_8004284C();
}

void func_8007C878(void) {
    D_80122CF8 = 0;
    D_800B593C = 0;
    func_8004284C();
}

void func_8007C8A4(void) {
    if (func_8007C8D8() == 1) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8007C8D8);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", set_tarao_bg);

void set_tarao_sprt(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u8 arg4, u8 arg5, s16 arg6, u8 arg7, u8 arg8, u8 arg9) {
    u8 *p;

    p = (u8 *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg7;
    p[5] = arg8;
    p[6] = arg9;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0x10) = arg2;
    *(s16 *)(p + 0x12) = arg3;
    p[0xC] = arg4;
    p[0xD] = arg5;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg6 * 4, p);
}

void set_tarao_rect(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7) {
    u8 *p;

    p = (u8 *)GetWorkBase(0x10, D_8011ECA0);
    func_8009F0F4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg5;
    p[5] = arg6;
    p[6] = arg7;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0xC) = arg2;
    *(s16 *)(p + 0xE) = arg3;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg4 * 4, (s32)p);
}

void normal_date_bg_out(void) {
    if (func_8007D8AC() == 1) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8007D8AC);

void normal_date_girl_in(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_in_init();
        break;
    case 1:
        normal_date_girl_in_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_in_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_in_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_suddenin);

void normal_date_girl_out(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_out_init();
        break;
    case 1:
        normal_date_girl_out_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}

void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_out_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_suddenout);

s32 normal_date_bg_fadein(void) {
    D_80122CF8 = 1;
    D_800B593C += 4;
    if (D_800B593C >= 0x80) {
        D_800B593C = 0x80;
    }
    if (D_800B593C >= 0x7D) {
        D_800B593C = 0x80;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bggirl_fadein);

void normal_date_bg_fadeout(void) {
    D_80122CF8 = 1;
    D_800B593C -= 4;
    if (D_800B593C >= 0x81) {
        D_800B593C = 0;
    }
    if (D_800B593C < 4) {
        D_80122CF8 = 0;
        D_800B593C = 0;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bggirl_fadeout);

void normal_date_move_place(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_move_place_init();
        return;
    case 1:
        normal_date_move_place_main();
        return;
    default:
        func_80046500();
        return;
    }
}

void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
    func_80058398(1);
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_move_place_main);

void place_init(void) {
    k_reset(0);
    func_8006612C(D_800CA19C);
    func_8004284C();
}

void place_init2(void) {
    k_reset(0);
    func_8006612C(D_800CA1DC);
    func_8004284C();
}

void change_dec_bg(void) {
    dec_bg_show_switch(1);
    func_8004284C();
}

s32 dec_init(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        D_800B5928 = 0;
        D_800B592C = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_8005751C(1);
    func_8004284C();
}

void bg_read_sub2(s32 arg0) {
    s32 t;

    D_800B3D60 = 0;
    t = dec_bg_cd_read(arg0, 1);
    if (t == D_800B5939) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (t == 1 - D_800B5939) {
        func_8004284C();
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", check_k_scroll);

void wait_sub_sub(s16 arg0) {
    s32 t = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = t;
    if ((u32)t >= (u32)arg0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8007EF48);

void func_80081128(void) {
    if (D_800CA2AC > 0x200) {
        if (D_800E6280.unk_F88 & 0x600060) {
            func_80044750(0xB4);
            D_800CA2A8 = 0;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80081190);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80082764);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", make_three_select);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", junban_init);

void normal_date_three_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_three_select_init();
        return;
    case 1:
        normal_date_three_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_three_select_init);

s32 normal_date_three_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = D_800CA2A4;
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = D_800CA2A5;
            } else if (D_800E6280.unk_F90[5].unk_06 != 0) {
                D_80122CDC = D_800CA2A6;
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}

void normal_date_two_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_two_select_init();
        return;
    case 1:
        normal_date_two_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}

void normal_date_two_select_init(void) {
    s32 pad; /* FAKE: unused word above the arrays, the original frame is 8 bytes bigger (T-9020) */
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];

    x[0] = x[1] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    w[0] = w[1] = 0xFC;
    h[0] = h[1] = 0x10;
    menu_set(1, 2, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}

s32 normal_date_two_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = 0;
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = 1;
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}

void func_80083338(void) {
    func_80083440(get_g_zyotai_h(D_800E6280.unk_F5F) & 0x7F);
    func_8004284C();
}

void func_80083378(void) {
    func_80083440(0);
    func_8004284C();
}

void func_800833A0(void) {
    func_80083440(1);
    func_8004284C();
}

void func_800833C8(void) {
    func_80083440(2);
    func_8004284C();
}

void func_800833F0(void) {
    func_80083440(3);
    func_8004284C();
}

void func_80083418(void) {
    func_80083440(4);
    func_8004284C();
}

void func_80083440(u8 arg0) {
    func_80085CD4(arg0);
    func_800634FC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80083474);

void func_80083628(void) {
    u8 var_v1;

    var_v1 = get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F;
    if ((u8) D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_800836A0);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80083808);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80083A10);

void hizuke_hide(void) {
    hizuke_disp_switch(0);
    func_8004284C();
}

void hizuke_appear(void) {
    hizuke_disp_switch(1);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", read_bustup);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", read_bustup_uniform);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", read_bustup_swimsuit);

void bustup_return(void) {
    read_bustup();
}

void return_step(void) {
    func_80042878(D_800E6280.unk_720);
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}

void k_disp_inc2(void) {
    s32 pad; /* FAKE: unused slot above sp2B, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;

    if (D_80122CFC-- < 1) {
        D_80122CFC = 0;
        sp2B = get_k_speed();
        if (D_800E6280.unk_F80 & 0x600060) {
            k_speed_set(0);
        }
        k_disp_inc();
        k_speed_set(sp2B);
    }
}

void yosi_trans(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}

void change_yoshio(void) {
    D_800E6280.unk_F5F = 0xD;
    func_8004284C();
}

void change_girl(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_8004284C();
}

void func_800847B8(u8 arg0) {
    s32 t;

    strcpy(D_800CA16C, D_800E6280.unk_0D4);
    strcpy(D_800CA174, D_800E6280.unk_0DC);
    if (arg0 < 0xE) {
        t = arg0;
        get_p_name(D_800CA17C, t);
        strcpy(D_800CA188, D_800B35F4[arg0]);
        get_g_name(D_800CA190, t);
    }
}

void select_girl(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(0);
        return;
    case 1:
        select_girl_main(0);
        return;
    default:
        func_80046500();
        return;
    }
}

void select_girl2(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(1);
        return;
    case 1:
        select_girl_main(1);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl_main);

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)D_801217D0 + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}

void day_plus(void) {
    s32 sp28; /* FAKE: unused local, the original frame has one more word below sp2B (T-3330 slot rule); source unknown. T-6040 */
    u8 sp2B;

    D_800CA2F0 = 0;
    sp2B = D_8011F113;
    func_80072338();
    hizuke_init();
    if (sp2B & 0x80) {
        hizuke_disp_switch(1);
    } else {
        hizuke_disp_switch(0);
    }
    hizuke_show();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", check_para_limit);

void func_80084E4C(void) {
    func_8006C848(0);
}

void func_80084E6C(void) {
    D_80122D20 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80084E90);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_800850D4);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085234);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085368);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_800853FC);

void event_face0(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}

void event_face1(void) {
    D_80120666 = 3;
    D_801206AA = 1;
    func_8004284C();
}

void event_face2(void) {
    D_80120666 = 6;
    D_801206AA = 4;
    func_8004284C();
}

void event_face3(void) {
    D_80120666 = 7;
    D_801206AA = 5;
    func_8004284C();
}

void event_face_kuchi0(void) {
    D_80120666 = 2;
    func_8004284C();
}

void event_face_kuchi1(void) {
    D_80120666 = 3;
    func_8004284C();
}

void event_kuchi_ani_no(void) {
    D_80122D10 = 0;
    D_80120652 = 4;
    func_8004284C();
}

void event_kuchi_ani_yes(void) {
    D_80122D10 = 1;
    D_80120652 = 0;
    func_8004284C();
}

void ev_me_on(void) {
    D_80122D0C = 0;
    D_80120696 = 5;
    func_8004284C();
}

void ev_me_on_continue(void) {
    D_80122D0C = 1;
    D_80120696 = 5;
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", _sprite_set_box_shade_tarao);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8008585C);

void don_init2(void) {
    func_80047550();
    func_8004284C();
}

void don_wait(void) {
    if ((u32)D_800E6280.unk_1104.w++ >= 0x81U) {
        k_reset(1);
        func_8004284C();
    }
}

void normal_date_speak_1line(void) {
    D_800CA134 = (u8 *)&D_800CA148;
    D_800CA138 = (u8 *)&D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    if (func_80082764(D_80122CDC, 1, 0) == 1) {
        func_8004284C();
    }
}

void normal_date_speak_012(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(0xFF, 1, 0);
}

void normal_date_speak(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(D_80122CDC, 1, 0);
}

void vram_bustup_clear(void) {
    s16 i;

    func_80048F64(0x1F);
    func_80048F64(0x1E);
    for (i = 5; i < 0xA; i++) {
        D_800E6280.unk_1228[i] = -1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085B3C);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085CD4);
