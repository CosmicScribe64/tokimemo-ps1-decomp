#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_80135160(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x61, 0x80180000, 0x4E4E);
        D_800E6280.unk_110D += 1;
    } else if ((D_800E6280.unk_110D == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

void func_801351DC(void) {
    s32 i;

    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 9)) {
        if (D_800E6280.unk_F80 & 0x20) {
            func_801352F4();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((D_800E6280.unk_110A >= 3) && (D_800E6280.unk_110A < 8) && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(7);
    }
    func_801352F4();
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801352F4);

void func_8013540C(void) {
    func_8004500C(1, 0x201);
    func_80135630();
    func_8004284C();
}

void func_80135440(void) {
    if (D_800E6280.unk_1104.u == 0) {
        D_80120652 = 5;
    }
    D_800E6280.unk_1104.u += 1;
    if (D_80122EAC == 3) {
        if (D_800E6280.unk_1104.u >= 0x1C2) {
            func_8004284C();
        }
    } else {
        if (!(D_80120652 & 1)) {
            func_8004284C();
        }
    }
}

s32 func_801354C8(void) {
    if (D_800E6280.unk_1104.u == 0) {
        *(u8 *)&D_800E6280.unk_110B = 0x80;
        D_80120653 = 0;
        D_801206DB = 0;
        D_8012069B = 0x80;
        D_80120697 = 0x80;
        D_80120696 = 5;
    }
    func_80066334();
    D_800E6280.unk_1104.u += 1;
    if (D_800E6280.unk_1104.u % 9U == 0) {
        if ((D_801206D7 += 1) >= 0x15U) {
            D_801206D7 = 0x11;
        }
    }
    if (!(D_80120696 & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_801355AC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80135160", func_80135630);
