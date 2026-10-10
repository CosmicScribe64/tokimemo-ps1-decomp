#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80137FF0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_8013808C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138158);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_801381B8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138240);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138354);

void func_801383AC(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801383D4(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801383FC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_8013843C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_801384B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138524);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_801385AC);

void func_801386CC(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

void func_80138710(void) {
    D_80144E08 = 3;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138758);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138868);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_801388F0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138964);

void func_801389D8(void) {
    bg_read_sub2(0x405F);
    func_8004284C();
}

void func_80138A00(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80138A28(void) {
    if (D_80144F10 != 0) {
        bg_read_sub2(0x40D0);
    } else {
        bg_read_sub2(0x40C6);
    }
    func_8004284C();
}

void func_80138A6C(void) {
    bg_read_sub2(0x40C6);
    func_8004284C();
}

void func_80138A94(void) {
    bg_read_sub2(0x40D0);
    func_8004284C();
}

void func_80138ABC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138AFC);
