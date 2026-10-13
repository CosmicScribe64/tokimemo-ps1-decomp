#define MAIN_API_OVERRIDE_func_800847B8 /* Caller passes the full word without a prototype (T-9960). */
#include "common.h"
#include "ovl/BUNKAKEN.h"

void func_800847B8();

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D20);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D28);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D30);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D38);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D40);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D48);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D50);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D54);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D5C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D64);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D68);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D70);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D78);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D7C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D84);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D8C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D94);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155D9C);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DA4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DAC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DB4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DBC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DC4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DCC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DD4);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DDC);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80152680.rodata", D_80155DE4);

void func_80152680(void) {
    GameState *g = &D_800E6280;
    s32 i;
    BunkakenTexts12 names;
    BunkakenTexts11 blood_types;
    BunkakenTexts4 player_blood_types;

    names = D_80156470;
    blood_types = D_801564A0;
    player_blood_types = D_801564CC;
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
            func_80132548();
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            if ((D_80122EBC == 0xFF) || ((((u16) g->unk_0F4.h >> 0xC) != 3) && ((func_8008667C(0) == 4) || (D_80122EA4 == 0)) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3)) || ((((u16) g->unk_0F4.h >> 0xC) != 3) && ((func_8008667C(2) == 4) || (D_80122EA4 == 2)) && (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3))) {
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
        D_801564DC = 0;
        func_8004284C();
        break;
    case 9:
        D_801564DC++;
        if (D_801564DC >= 0x15U) {
            D_801564DC = 0;
            func_8004284C();
        }
        break;
    case 10:
        func_8004E788(-0x10, 0x42, 0, (s32) "血液型", 0);
        func_8004E884(5);
        func_8004284C();
        break;
    case 11:
        D_801564DC++;
        if (D_801564DC >= 0x15U) {
            D_801564DC = 0;
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
        D_801564DC++;
        if (D_801564DC >= 0x15U) {
            D_801564DC = 0;
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
        D_801564DC = 0;
        func_8004284C();
        break;
    case 16:
        D_801564DC++;
        if (D_801564DC >= 0x15U) {
            D_801564DC = 0;
            func_8004284C();
        }
        break;
    case 17:
        func_8004E788(-0x10, 0x52, 0, (s32) "血液型", 0);
        func_8004E884(9);
        func_8004284C();
        break;
    case 18:
        D_801564DC++;
        if (D_801564DC >= 0x15U) {
            D_801564DC = 0;
            func_8004284C();
        }
        break;
    case 19:
        func_8004E788(0x28, 0x52, 0, (s32)player_blood_types.text[(u32) (g->unk_0F8 << 0x15) >> 0x1E], 0);
        func_8004E884(0xA);
        func_8004284C();
        break;
    case 20:
        D_801564DC++;
        if ((D_801564DC >= 0x10U) && (func_8004ECB4() != 0)) {
            D_8011ED5B ^= 0x80;
            D_801564DC = 0;
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
        func_80044750(0x50C);
        func_8004284C();
        break;
    case 23:
        D_801564DC++;
        if (D_801564DC >= 0x3DU) {
            D_800CA148 = func_80152F4C() + 3;
            D_80122EA8 = func_80152F4C();
            func_8004284C();
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        if (((func_8008667C(0) == 4) || (D_80122EA4 == 0)) && (((u8) g->unk_1BC[0].unk_0C.b[2] >> 4) == 3)) {
            D_80122EBC = 0;
        } else if (((func_8008667C(2) == 4) || (D_80122EA4 == 2)) && (((u8) g->unk_1BC[2].unk_0C.b[2] >> 4) == 3)) {
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

/* FAKE: unused word fixes the table offset; expression layout retains address/value allocation. T-9550 */
s32 func_80152F4C(void)
{
  s32 result;
  u32 pad;
  BunkakenGrid4 choices;
  choices = D_801564E0;
  if ((((D_80122EBC == 2) && ((D_800E6280.unk_1BC[2].unk_0C.b[2] >> 4) == 3)) && (func_8008667C(2) < 4)) && (D_80122EA4 != 2))
  {
    return 3;
  }
  result = choices.values[(D_800E6280.unk_1BC[D_80122EBC].unk_10.u << 21) >> 30][(((u32) D_800E6280.unk_0F8) << 21) >> 30];
  return result;
}
