#define MAIN_API_OVERRIDE_func_800847B8 /* caller passes the full word without a prototype (BUNKAKEN T-9960 form) */
#include "common.h"
#include "ovl/BUNKASAI.h"

void func_800847B8();

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160380);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160388);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160390);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160398);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603A0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603A8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603B0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603B4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603BC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603C4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603C8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603D0);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603D8);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603DC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603E4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603EC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603F4);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_801603FC);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160404);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_8016040C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160414);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_8016041C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160424);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_8016042C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160434);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_8016043C);

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/8015AF90.rodata", D_80160444);

/* Clone of BUNKAKEN func_80152680 (T-18700). func_80132410 is called without a declaration, an
 * implicit int function as BUNKAKEN's func_80132548: with a void prototype IDO swaps v0 and v1. */
void func_8015AF90(void) {
    GameState *g = &D_800E6280;
    s32 i;
    BunkasaiTexts12 names;
    BunkasaiTexts11 blood_types;
    BunkasaiTexts4 player_blood_types;

    names = D_80160AE0;
    blood_types = D_80160B10;
    player_blood_types = D_80160B3C;
    switch (g->step) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B80);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->girls[i].flags.f.met;
        }
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80132410();
            D_800CA14C = 0;
            D_800CA160 = D_801604F0;
            D_800CA164 = D_801604F4;
            D_800CA168 = D_801604F8;
            if ((D_80122EBC == 0xFF) || ((((u16) g->player_flags.h >> 0xC) != 3) && (func_8008667C(0) == 4) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) || ((((u16) g->player_flags.h >> 0xC) != 3) && (func_8008667C(2) == 4) && (((u8) g->girls[2].flags.b[2] >> 4) == 3))) {
                D_800CA148 = 0;
                func_8004284C();
            } else {
                D_800CA148 = 1;
                func_80042940(5);
            }
        }
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        select_girl2();
        break;
    case 4:
        D_80122EBC = D_80122CDC;
        func_800847B8(D_80122CDC);
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        k_reset(1);
        set_kanji_string(-0x80, 0x32, 0, (s32) "＊パソコン　ＮＭＳ＿９０００＊", 0);
        k_disp_start(2);
        func_8004284C();
        break;
    case 7:
        func_80044750(0x50A);
        func_8004284C();
        break;
    case 8:
        if (D_80122EBC == 0xB) {
            set_kanji_string(-0x80, 0x42, 0, (s32) *girl_short_name_table, 0);
        } else {
            set_kanji_string(-0x80, 0x42, 0, (s32) girl_short_name_table[D_80122EBC], 0);
        }
        set_kanji_string(-0x48, 0x42, 0, (s32)names.text[D_80122EBC], 0);
        k_disp_start(4);
        D_80160B4C = 0;
        func_8004284C();
        break;
    case 9:
        D_80160B4C++;
        if (D_80160B4C >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 10:
        set_kanji_string(-0x10, 0x42, 0, (s32) "血液型", 0);
        k_disp_start(5);
        func_8004284C();
        break;
    case 11:
        D_80160B4C++;
        if (D_80160B4C >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 12:
        if ((D_80122EBC == 0) || (D_80122EBC == 0xB)) {
            set_kanji_string(0x28, 0x42, 0, (s32)player_blood_types.text[(u32) (g->girls[0].birthday.w << 0x15) >> 0x1E], 0);
        } else {
            set_kanji_string(0x28, 0x42, 0, (s32)blood_types.text[D_80122EBC], 0);
        }
        k_disp_start(6);
        func_8004284C();
        break;
    case 13:
        D_80160B4C++;
        if (D_80160B4C >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 14:
        func_80044750(0x50A);
        func_8004284C();
        break;
    case 15:
        set_kanji_string(-0x80, 0x52, 0, (s32) g->player_family_name, 0);
        set_kanji_string(-0x48, 0x52, 0, (s32) g->player_given_name, 0);
        k_disp_start(8);
        D_80160B4C = 0;
        func_8004284C();
        break;
    case 16:
        D_80160B4C++;
        if (D_80160B4C >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 17:
        set_kanji_string(-0x10, 0x52, 0, (s32) "血液型", 0);
        k_disp_start(9);
        func_8004284C();
        break;
    case 18:
        D_80160B4C++;
        if (D_80160B4C >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 19:
        set_kanji_string(0x28, 0x52, 0, (s32)player_blood_types.text[(u32) (g->player_birthday << 0x15) >> 0x1E], 0);
        k_disp_start(0xA);
        func_8004284C();
        break;
    case 20:
        D_80160B4C++;
        if ((D_80160B4C >= 0x10U) && (check_end_k() != 0)) {
            D_8011ED5B ^= 0x80;
            D_80160B4C = 0;
        }
        if ((g->pad_pressed & 0xF0) && (check_end_k() != 0)) {
            k_reset(1);
            D_8011ED5B &= 0x7F;
            func_8004284C();
        }
        break;
    case 21:
        normal_date_speak();
        break;
    case 22:
        func_80044750(0x508);
        func_8004284C();
        break;
    case 23:
        D_80160B4C++;
        if (D_80160B4C >= 0x3DU) {
            D_800CA148 = func_8015B828() + 3;
            D_80122EA8 = func_8015B828();
            func_80044750(0x50C);
            func_8004284C();
        }
        break;
    case 24:
        normal_date_speak();
        break;
    case 25:
        if ((func_8008667C(0) == 4) && (((u8) g->girls[0].flags.b[2] >> 4) == 3)) {
            D_80122EBC = 0;
        } else if ((func_8008667C(2) == 4) && (((u8) g->girls[2].flags.b[2] >> 4) == 3)) {
            D_80122EBC = 2;
        }
        func_80042908((s32) *(u8 *)&D_80122EB0);
        func_80042940((s32) *(u8 *)&D_80122EB4);
        break;
    default:
        func_80046500();
        break;
    }
}

s32 func_8015B828(void) {
    s32 result;
    s32 index;
    BunkasaiTable40 table;

    table = D_80160B50;
    index = D_80122EBC;
    if (index == 2 && ((u8) D_800E6280.girls[2].flags.b[2] >> 4) == 3) {
        if (((u16) D_800E6280.player_flags.h >> 12) != 3) {
            return 3;
        }
    }
    result = table.unk_00[(u32) (D_800E6280.girls[index].birthday.w << 21) >> 30][(u32) (D_800E6280.player_birthday << 21) >> 30];
    return result;
}
