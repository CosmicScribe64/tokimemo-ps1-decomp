#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/8007C030", SD_GetCDLevel);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", SD_DetectCDPeak);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", SD_CalcCDAve);

s16 getCDlevel(void) {
    if (D_80125D10 & 0x400) {
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

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8007C784);

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

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_in);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_in_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_in_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_suddenin);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_out);

void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E7384 = D_800E7384 + 1;
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_out_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_girl_suddenout);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bg_fadein);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bggirl_fadein);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bg_fadeout);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_bggirl_fadeout);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_move_place);

void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E7384 = D_800E7384 + 1;
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
        D_800E738A -= 3;
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
    s32 t = D_800E7384 + 1;
    D_800E7384 = t;
    if ((u32)t >= (u32)arg0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_8007EF48);

void func_80081128(void) {
    if (D_800CA2AC > 0x200) {
        if (D_800E7208 & 0x600060) {
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

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_three_select);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_three_select_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_three_select_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_two_select);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_two_select_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", normal_date_two_select_main);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80083338);

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

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80083628);

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
    func_80042878(D_800E69A0);
    func_80042908(D_800E69A1);
    func_80042940(D_800E69A2);
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", k_disp_inc2);

void yosi_trans(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}

void change_yoshio(void) {
    D_800E71DF = 0xD;
    func_8004284C();
}

void change_girl(void) {
    D_800E71DF = D_800E69DD;
    func_8004284C();
}

void func_800847B8(u8 arg0) {
    s32 t;

    strcpy(D_800CA16C, D_800E6354);
    strcpy(D_800CA174, D_800E635C);
    if ((u32)arg0 < 0xE) {
        t = arg0;
        get_p_name(D_800CA17C, arg0);
        strcpy(D_800CA188, D_800B35F4[arg0]);
        get_g_name(D_800CA190, t);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl2);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl_init);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", select_girl_main);

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)&D_801217D0 + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}

INCLUDE_ASM("asm/nonmatchings/main/8007C030", day_plus);

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
    if ((u32)D_800E7384++ >= 0x81U) {
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

INCLUDE_ASM("asm/nonmatchings/main/8007C030", vram_bustup_clear);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085B3C);

INCLUDE_ASM("asm/nonmatchings/main/8007C030", func_80085CD4);
