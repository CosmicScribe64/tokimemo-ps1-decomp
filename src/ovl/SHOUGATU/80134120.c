#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134120);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801341E4);

void func_8013428C(void) {
    func_801323D4();
    if ((D_80143B04 == 2) && (D_800E738D == 0) && (D_8011ED5B & 0x80)) {
        func_80044750(0x202);
        D_800E738D += 1;
    }
    if ((D_800E738D == 0) && (D_80143B04 == 3)) {
        func_80044750(0x202);
        D_800E738D += 1;
    }
}

void func_80134340(void) {
    D_800E69DD = D_800E71DF;
    func_8004284C();
}

void func_8013436C(void) {
    if ((D_80143B20 == 0) && !(D_800E7378 & 1) && (*((u8 *)&D_800E69D8 + D_800E71DE * 8) != 5)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 4);
        } else {
            func_80085B3C(5, 1);
        }
    }
    bg_read_sub2(0x41F6);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134400);

void func_80134554(void) {
    u8 sel = D_80143D20; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 2:
        break;
    case 0:
        D_80143B00 += 3;
        break;
    case 1:
        D_80143B00 += 2;
        break;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801345C8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134648);

void func_801346E8(void) {
    if (D_80122CDC != 2) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_80134730);

void func_801347B4(void) {
    D_80122CEC = D_80122CDC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80134120", func_801347E0);
