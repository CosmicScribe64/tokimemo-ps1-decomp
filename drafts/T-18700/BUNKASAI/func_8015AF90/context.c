#include "common.h"
#include "ovl/BUNKASAI.h"

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

void func_8015AF90(void) {
    GameState *g = &D_800E6280;
    s32 i;
    BunkasaiTexts12 names;
    BunkasaiTexts11 blood_types;
    BunkasaiTexts4 player_blood_types;

    names = D_80160AE0;
    blood_types = D_80160B10;
    player_blood_types = D_80160B3C;
    switch (g->unk_110A) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B80);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->unk_1BC[i].unk_0C.f.b1;
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
            if ((D_80122EBC == 0xFF) || ((((u16) g->unk_0F4.h >> 0xC) != 3) && (func_8008667C(0) == 4) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3)) || ((((u16) g->unk_0F4.h >> 0xC) != 3) && (func_8008667C(2) == 4) && (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3))) {
                D_800CA148 = 0;
                func_8004284C();
            } else {
                D_800CA148 = 1;
                func_80042940(5);
            }
        }
        break;
    case 2:
        func_80085A60();
        break;
    case 3:
        func_800848AC();
        break;
    case 4:
        D_80122EBC = D_80122CDC;
        func_800847B8(D_80122CDC);
        func_8004284C();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) "＊パソコン　ＮＭＳ＿９０００＊", 0);
        func_8004E884(2);
        func_8004284C();
        break;
    case 7:
        func_80044750(0x50A);
        func_8004284C();
        break;
    case 8:
        if (D_80122EBC == 0xB) {
            func_8004E788(-0x80, 0x42, 0, (s32) *girl_short_name_table, 0);
        } else {
            func_8004E788(-0x80, 0x42, 0, (s32) girl_short_name_table[D_80122EBC], 0);
        }
        func_8004E788(-0x48, 0x42, 0, (s32)names.text[D_80122EBC], 0);
        func_8004E884(4);
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
        func_8004E788(-0x10, 0x42, 0, (s32) "血液型", 0);
        func_8004E884(5);
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
            func_8004E788(0x28, 0x42, 0, (s32)player_blood_types.text[(u32) (g->unk_1BC[0].unk_10.w << 0x15) >> 0x1E], 0);
        } else {
            func_8004E788(0x28, 0x42, 0, (s32)blood_types.text[D_80122EBC], 0);
        }
        func_8004E884(6);
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
        func_8004E788(-0x80, 0x52, 0, (s32) g->unk_0D4, 0);
        func_8004E788(-0x48, 0x52, 0, (s32) g->unk_0DC, 0);
        func_8004E884(8);
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
        func_8004E788(-0x10, 0x52, 0, (s32) "血液型", 0);
        func_8004E884(9);
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
        func_8004E788(0x28, 0x52, 0, (s32)player_blood_types.text[(u32) (g->unk_0F8 << 0x15) >> 0x1E], 0);
        func_8004E884(0xA);
        func_8004284C();
        break;
    case 20:
        D_80160B4C++;
        if ((D_80160B4C >= 0x10U) && (func_8004ECB4() != 0)) {
            D_8011ED5B ^= 0x80;
            D_80160B4C = 0;
        }
        if ((g->unk_F88 & 0xF0) && (func_8004ECB4() != 0)) {
            func_8004E9F4(1);
            D_8011ED5B &= 0x7F;
            func_8004284C();
        }
        break;
    case 21:
        func_80085A60();
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
        func_80085A60();
        break;
    case 25:
        if ((func_8008667C(0) == 4) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3)) {
            D_80122EBC = 0;
        } else if ((func_8008667C(2) == 4) && (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3)) {
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
    if (index == 2 && ((u8) D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3) {
        if (((u16) D_800E6280.unk_0F4.h >> 12) != 3) {
            return 3;
        }
    }
    result = table.unk_00[(u32) (D_800E6280.unk_1BC[index].unk_10.w << 21) >> 30][(u32) (D_800E6280.unk_0F8 << 21) >> 30];
    return result;
}
