#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80141780);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80141850);

void func_80141920(void) {
    D_80146510 = (u8 *)0x801D22D0;
    D_80146514 = (u8 *)0x801D22D8;
    D_80146518 = (u8 *)0x801D233C;
    D_8014651C = *(s16 *)0x801D2358;
    D_80146520 = (u8 *)0x801B0000;
    D_80146524 = (u8 *)0x801B2000;
    D_80146528 = (u8 *)0x801B6000;
    D_8014652C = (u8 *)0x801BA000;
    D_80146530 = (u8 *)0x801BE000;
    D_80146534 = (u8 *)0x801C2000;
    D_80146538 = (u8 *)0x801C6000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801419D0);

void func_80141AC0(void) {
    D_800B5A60 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80141AE8);

void func_80141B98(void) {
    func_80044750(0x208);
    func_8004284C();
}

void func_80141BC0(void) {
    D_8014653C = get_k_speed();
    D_80122CFC = 0x72;
    k_speed_set(0x1E);
    k_disp_start(2);
    func_8004284C();
}

void func_80141C0C(void) {
    k_speed_set(D_8014653C);
    k_disp_start(1);
    func_8004284C();
}

void func_80141C44(void) {
    func_8004435C(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E74D0 = 1;
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}

void func_80141CA0(void) {
    func_80046318(0x55, 0x801B0000, 0x8065);
    func_80141780();
    func_8004284C();
}

void func_80141CD8(void) {
    D_801467FC = 1;
    if (D_800E71DF == 0) {
        func_80044750(0x206);
    }
    D_80121864 = -0x64;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80141D20);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801421D4);

void func_801424CC(void) {
    bg_read_sub2(0x475A);
    func_80085B3C(2, 6);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80142500);

void func_8014257C(void) {
    func_80062CD0(0x5C8D);
    func_8004284C();
}

void func_801425A4(void) {
    func_80044750(0x207);
    func_8004284C();
}

void func_801425CC(void) {
    func_80083440(5);
    func_8004284C();
}

void func_801425F4(void) {
    func_80046318(0x45, 0x801B0000, 0x8571);
    func_80141850();
    func_8004284C();
}

void func_8014262C(void) {
    D_801206DB &= 0x7F;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_8014265C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80142784);

void func_80142844(void) {
    func_8004435C(0x280, 0, 0x40, 0x80, D_801464E0);
    D_800E74D0 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_8014288C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80142DA0);

void func_8014306C(void) {
    bg_read_sub2(0x4752);
    func_80085B3C(2, 0x34);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801430A0);

void func_80143200(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80143228(void) {
    func_80044750(0x504);
}

void func_80143248(void) {
    func_80046318(0x45, 0x801B0000, 0x8BC9);
    func_80141920();
    func_8004284C();
}

void func_80143280(void) {
    D_801206F0 = 0;
    D_801206E0 = 0;
    D_801206DA = 3;
    func_80044750(0x503);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801432C4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_801433A4);

void func_801434E0(void) {
    bg_read_sub2(0x42E2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80141780", func_80143508);
