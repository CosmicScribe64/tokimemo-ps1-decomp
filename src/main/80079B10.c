#include "common.h"
#include "game.h"

void func_80079B10(u16 arg0) {
    func_8007B5EC(arg0);
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079B34);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079C70);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079D28);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079E00);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079E4C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079E9C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80079F00);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A008);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A080);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A254);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A354);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A43C);

void func_8007A4DC(void) {
    if (D_80125D14 & 0x100) {
        func_8007A50C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A50C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A5BC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A618);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A6AC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A868);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A924);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007A98C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AB24);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007ABE0);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AD6C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007ADD8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AEC0);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007AF0C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B144);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B2B4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B358);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B3DC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B460);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B4E4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B568);

void func_8007B5CC(void) {
    func_80090D20();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B5EC);

void func_8007B640(void) {
    D_80125CC0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B64C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B6E0);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B734);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B7E0);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B844);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B8F8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007B99C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BA54);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BAAC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BBE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BCFC);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BDE8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BE94);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BF04);

void func_8007BFA8(void) {
    D_80125D3C = -1;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007BFB8);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", addr_init_weekly_sd);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", addr_init_holiday_sd);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", addr_init_club_sd);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", addr_init_else_sd);

void addr_init_bustup(void) {
    D_800CA120 = 0x80180084;
    D_800CA124 = 0x80180098;
    D_800CA128 = 0x8018009C;
    D_800CA130 = 0x801800AC;
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C784);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C844);

void func_8007C878(void) {
    D_80122CF8 = 0;
    D_800B593C = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C8A4);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007C8D8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", set_tarao_bg);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", set_tarao_sprt);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", set_tarao_rect);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_bg_out);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_move_place_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_move_place_main);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", place_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", place_init2);

void change_dec_bg(void) {
    dec_bg_show_switch(1);
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80079B10", dec_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", bg_read_sub2);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", check_k_scroll);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", wait_sub_sub);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_8007EF48);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80081128);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80083440);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", return_step);

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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_800847B8);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl2);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl_init);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", select_girl_main);

void sprite_brightness(s16 idx, u8 val) {
    u8 *p = D_801217D0 + idx * 36;
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

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_speak_012);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", normal_date_speak);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", vram_bustup_clear);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085B3C);

INCLUDE_ASM("asm/nonmatchings/main/80079B10", func_80085CD4);
