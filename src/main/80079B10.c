#include "common.h"
#include "game.h"
#include "main_only.h"

void func_80079B10(u16 arg0) {
    func_8007B5EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079B34);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079C70);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079D28);

s32 func_80079E00(s32 arg0) {
    if ((D_80125D14 & (0x10000000 << (u16)D_80125D4C)) == 0) {
        if ((D_80125D14 & (0x1000000 << (u16)D_80125D4C)) == 0) {
            return 1;
        }
    }
    return 0;
}

void func_80079E4C(void) {
    if (D_80125D10 & 0x400) {
        func_80079E9C();
    }
    if (D_80125D14 & 0x200) {
        func_80079F00();
    }
}

void func_80079E9C(void) {
    if (func_80046274() >= (u32)D_80125D1C) {
        if (!(D_80125D14 & 0x100)) {
            func_8007AB24(0xB4);
        }
    } else {
        SD_GetCDLevel(&D_80125E60);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079F00);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A008);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A080);

void func_8007A254(u16 arg0) {
    arg0 = arg0 & 0xFF;
    func_8007B99C(arg0);
    switch (arg0) {
    case 17:
        func_8007BF04(0);
        func_8007BE94(0);
        func_8007BF04(1);
        func_8007BE94(1);
        return;
    case 18:
        func_8007BF04(2);
        func_8007BE94(2);
        func_8007BF04(3);
        func_8007BE94(3);
        return;
    case 19:
        func_8007BF04(0);
        func_8007BE94(0);
        return;
    case 20:
        func_8007BF04(1);
        func_8007BE94(1);
        return;
    case 21:
        func_8007BF04(2);
        func_8007BE94(2);
        return;
    case 22:
        func_8007BF04(3);
        func_8007BE94(3);
        return;
    default:
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A354);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A43C);

void func_8007A4DC(void) {
    if (D_80125D14 & 0x100) {
        func_8007A50C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A50C);

void func_8007A5BC(void) {
    func_8007A98C(0x74);
    if ((D_80125D10 & 0x4000) == 0) {
        func_8007A868();
        if (D_80125D18 != 0) {
            func_8007A924(D_80125D18);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A618);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A6AC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A868);

/* FAKE: indexed views of D_80125D58 stop IDO from hoisting the later loads above the stores (see cal_sprite_disp_switch). Real source unknown. T-2090 */
void func_8007A924(s32 arg0) {
    func_8007BA54();
    D_80125D58[0] = D_80125D5E;
    D_80125D58[1] = D_80125D58[4];
    *(s32 *)(D_80125D58 - 0x44) |= 0x200;
    func_80045414(0xD, arg0, D_80125D58);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A98C);

void func_8007AB24(u16 arg0) {
    if (!(D_80125D14 & 0x100)) {
        D_80125D44 = 0;
        D_80125D14 |= 0x100;
    }
    switch (arg0 & 0xFF) {
    default:
        D_80125D48 = 0x100;
        D_80125D46 = 0xA;
        return;
    case 0xB4:
        D_80125D48 = 0x100;
        D_80125D46 = 0xA;
        return;
    case 0xC4:
        D_80125D48 = 0x100;
        D_80125D46 = 1;
        return;
    case 0xD4:
        D_80125D48 = 0x80;
        D_80125D46 = 1;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007ABE0);

void func_8007AD6C(void) {
    if ((D_80125D10 & 0x100000) && !(D_80125D10 & 0x400)) {
        func_8008FD68(0);
        func_8008B750(0, 0);
    }
    func_8007B144(0x71);
    if (D_80125D3A != -1) {
        func_8007AEC0();
    }
}

void func_8007ADD8(s32 arg0) {
    s32 a;

    a = arg0 & 0xFFFF;
    switch (a & 0xF000) {
    case 0x0:
        if (D_80125D14 & 1) {
            D_80125D3C = a & 0xFF;
            func_8007AD6C();
            return;
        }
        if (D_80125D10 & 0x100) {
            D_80125D3C = a & 0xFF;
            func_8007B2B4(0xB1, a);
            return;
        }
        D_80125D3A = a & 0xFF;
        D_80125D3C = -1;
        func_8007AEC0();
        return;
    case 0x1000:
        func_8007B144(0x71, a);
        return;
    case 0x2000:
        func_8007B144(0xE1, a);
        return;
    case 0x4000:
        func_8007B144(0xF1, a);
        return;
    }
}

void func_8007AEC0(void) {
    if (func_80079E00(D_80125D4C) == 0) {
        D_80125D14 |= 2;
    } else {
        func_8007AF0C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AF0C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B144);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B2B4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B358);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B3DC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B460);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B4E4);

void func_8007B568(s32 arg0, s32 arg1) {
    func_80090D20();
    D_80125D10 &= 0xFFEFFFFF;
    func_8008B750(0x7F, 0x7F);
    func_80090D60(arg0, arg1, D_80125D34, D_80125D34);
}

void func_8007B5CC(void) {
    func_80090D20();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B5EC);

void func_8007B640(void) {
    D_80125CC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B64C);

void func_8007B6E0(void) {
    func_8007A98C(0x74);
    D_80125D18 = 0;
    D_80125D1C = 0;
    D_80125D20 = 0;
    D_80125D24 = 0;
    D_80125D28 = 0;
    D_80125D5C = 0;
    D_80125D5D = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B734);

void func_8007B7E0(void) {
    if (func_80079524() == 0 || !(D_80125D10 & 0x100)) {
        func_80090DB0();
        func_8008FD68(0);
        func_8008FED0();
        func_800949B0(3);
        func_8008FEB0();
    }
}

void func_8007B844(void) {
    func_8008B8C4();
    func_8008BAA4(1);
    func_80090DF0(0x18);
    func_8008B904(D_80125E70, 1, 1);
    func_80090E30(1);
    func_8008FEF0(3);
    func_800949B0(3);
    func_8008FEB0();
    func_8008FF5C(0x3C, 0x3C);
    func_8008B750(0x7F, 0x7F);
    func_8007BCFC(2);
    func_8007BAAC();
    func_8007B8F8();
    func_8007BA54();
    SD_InitCDLevelInfo();
    func_8007B640();
    func_8008BE34();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B8F8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B99C);

void func_8007BA54(void) {
    D_80125D54 = 0xC8;
    func_8008BEE0(0, 0, 0);
    func_8008BFB0(0, 0, 0);
    func_80045414(0xE, 0, &D_80125D54);
}

void func_8007BAAC(void) {
    D_80125D14 = 0;
    D_80125D10 = 0x10;
    D_80125D18 = 0;
    D_80125D1C = 0;
    D_80125D20 = 0;
    D_80125D24 = 0;
    D_80125D28 = 0;
    D_80125D2C = 0x64;
    D_80125D2E = 0x64;
    D_80125D30 = 0x50;
    D_80125D32 = 0x50;
    D_80125D34 = 0x5A;
    D_80125D36 = 0x5A;
    D_80125D38 = 0;
    D_80125D3A = -1;
    D_80125D3C = -1;
    D_80125D3E = 0;
    D_80125D40 = 0x80;
    D_80125D42 = 0xFF;
    D_80125D44 = 0;
    D_80125D46 = 0x80;
    D_80125D48 = 0xFF;
    D_80125D50 = 0;
    D_80125D4C = 1;
    D_80125D4E = 2;
    D_80125D52 = 3;
    D_80125D4A = 0;
    D_80125D5C = 0;
    D_80125D5D = 0;
    D_80125D5E = 0;
    D_80125D5F = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BBE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BCFC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BDE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BE94);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BF04);

void func_8007BFA8(void) {
    D_80125D3C = -1;
}

void func_8007BFB8(void) {
    if (D_80125D10 & 0x10000000) {
        D_80125D10 &= 0xEFFFFFFF;
        return;
    }
    D_80125D10 |= 0x10000000;
}

void SD_InitCDLevelInfo(void) {
    D_80125E60 = 0;
    D_80125E62 = 0;
    D_80125E64 = 8;
    D_80125E66 = 0x20;
    D_80125E68 = 0x212;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", SD_GetCDLevel);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", SD_DetectCDPeak);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", SD_CalcCDAve);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C784);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C8D8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", set_tarao_bg);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007D8AC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_in);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_in_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_in_main);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_suddenin);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_out);

void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E7384 = D_800E7384 + 1;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_out_main);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_girl_suddenout);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_bg_fadein);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_bggirl_fadein);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_bg_fadeout);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_bggirl_fadeout);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_move_place);

void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E7384 = D_800E7384 + 1;
    func_80058398(1);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_move_place_main);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", check_k_scroll);

void wait_sub_sub(s16 arg0) {
    s32 t = D_800E7384 + 1;
    D_800E7384 = t;
    if ((u32)t >= (u32)arg0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007EF48);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80081190);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80082764);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", make_three_select);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", junban_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_three_select);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_three_select_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_three_select_main);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_two_select);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_two_select_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_two_select_main);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083338);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083474);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083628);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_800836A0);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083808);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083A10);

void hizuke_hide(void) {
    hizuke_disp_switch(0);
    func_8004284C();
}

void hizuke_appear(void) {
    hizuke_disp_switch(1);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", read_bustup);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", read_bustup_uniform);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", read_bustup_swimsuit);

void bustup_return(void) {
    read_bustup();
}

void return_step(void) {
    func_80042878(D_800E69A0);
    func_80042908(D_800E69A1);
    func_80042940(D_800E69A2);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", k_disp_inc2);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl2);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl_main);

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)&D_801217D0 + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", day_plus);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", check_para_limit);

void func_80084E4C(void) {
    func_8006C848(0);
}

void func_80084E6C(void) {
    D_80122D20 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80084E90);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_800850D4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085234);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085368);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_800853FC);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", _sprite_set_box_shade_tarao);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8008585C);

void don_init2(void) {
    func_80047550();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", don_wait);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_speak_1line);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", vram_bustup_clear);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085B3C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085CD4);
