#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80135B90);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80135E04);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136078);

void func_80136148(void) {
    D_800CA148 = 2;
    D_800CA14C = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136178);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136204);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136340);

void func_80136424(void) {
    bg_read_sub2(0x41ED);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_8013644C);

void func_80136590(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80135B90();
    func_8004284C();
}

void func_801365C8(void) {
    D_80122CDC = D_80143FF4;
    func_8004284C();
}

void func_801365F4(void) {
    D_80143FF4 = (u8) D_80122CDC;
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_80135B90();
    D_800CA160 = D_80143EB4;
    D_800CA164 = D_80143EE8;
    D_800CA168 = D_80143F1C;
    if ((D_800E71DF == 0) && (D_80143B24 == 1)) {
        func_80085B3C(0xA, 0x32);
    } else {
        func_80085B3C(0xA, 0x2E);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_801366A4);

void func_8013673C(void) {
    D_800CA148 = (D_80122CDC * 2) + 0xD;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136770);

void func_80136824(void) {
    D_800CA148 = 0x1D;
    D_800CA160 = D_80143FE8;
    D_800CA164 = D_80143FEC;
    D_800CA168 = D_80143FF0;
    func_80136884();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136884);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136948);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_801369EC);

void func_80136B9C(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80135E04();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136BD4);

void func_80136CE0(void) {
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_80136D18();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80135B90", func_80136D18);
