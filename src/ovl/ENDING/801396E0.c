#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_801396E0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_801399B4);

void func_80139A58(void) {
    if (D_800E71DF == 0xD) {
        func_80042908(7);
        func_80042940(0xF);
        return;
    }
    func_80042908(8);
}

void func_80139AA0(s32 arg0) {
    func_80139B48(-0x64, -0x50, 0xC8, 0x10, arg0, 0, 1);
    func_80139B48(-0x64, 0x40, 0xC8, 0x10, arg0, 0, 1);
    dtd_on(0);
    func_80139B48(-0x64, -0x40, 0xC8, 0x80, arg0, 0xB, 1);
    dtd_on(0xB);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139B48);

void func_80139D24(void) {
    func_80046318(9, 0x80180000, 0xAE18);
    func_8004284C();
}

void func_80139D54(void) {
    func_80048EB8(0);
    func_8006BD6C(0);
    func_80041584();
    func_8004E58C();
    func_8004E500(3);
    D_80122CE4 = 0;
    D_80122CD4 = 0;
    func_8004284C();
}

void func_80139DA8(void) {
    if (D_800E71DF != 0xD) {
        func_800462C8(9, 0x80162000, 0x497C);
    } else {
        func_800462C8(0xA, 0x80162000, 0x4985);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139E08);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139EC4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139F3C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A004);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A2E8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B510);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B5E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B640);
