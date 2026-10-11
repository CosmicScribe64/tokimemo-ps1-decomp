#include "common.h"
#include "ovl/ENDING.h"

void func_801396E0(void) {
    D_8013CAC0 = 0x80180308;
    D_8013CAC4 = 0x80180728;
    D_8013CAC8 = 0x80180BCC;
    D_8013CACC = 0x80180ED4;
    D_8013CAD0 = 0x801812E4;
    D_8013CAD4 = 0x80181648;
    D_8013CAD8 = 0x80181A84;
    D_8013CADC = 0x80181ED0;
    D_8013CAE0 = 0x80182258;
    D_8013CAE4 = 0x80182684;
    D_8013CAE8 = 0x80182AB4;
    D_8013CAEC = 0x80182C74;
    D_8013CAF0 = 0x801835E4;
    D_8013CAF4 = 0x80183CC4;
    D_8013CAF8 = 0x80184008;
    D_8013CAFC = 0x80180314;
    D_8013CB00 = 0x80180734;
    D_8013CB04 = 0x80180BD8;
    D_8013CB08 = 0x80180EE0;
    D_8013CB0C = 0x801812F0;
    D_8013CB10 = 0x80181654;
    D_8013CB14 = 0x80181A90;
    D_8013CB18 = 0x80181EDC;
    D_8013CB1C = 0x80182264;
    D_8013CB20 = 0x80182690;
    D_8013CB24 = 0x80182AC0;
    D_8013CB28 = 0x80182C80;
    D_8013CB2C = 0x8018360C;
    D_8013CB30 = 0x80183CD4;
    D_8013CB34 = 0x80184014;
    D_8013CB38 = 0x801803DC;
    D_8013CB3C = 0x801807FC;
    D_8013CB40 = 0x80180CA0;
    D_8013CB44 = 0x80180FA8;
    D_8013CB48 = 0x801813B8;
    D_8013CB4C = 0x8018171C;
    D_8013CB50 = 0x80181B58;
    D_8013CB54 = 0x80181FA4;
    D_8013CB58 = 0x8018232C;
    D_8013CB5C = 0x80182758;
    D_8013CB60 = 0x80182B88;
    D_8013CB64 = 0x80182D48;
    D_8013CB68 = 0x801839E4;
    D_8013CB6C = 0x80183DB8;
    D_8013CB70 = 0x801840DC;
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_8013CB74;

void func_801399B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_8013CB74;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_80122CD4 != 0) {
        func_80139AA0(0x404040);
    }
}

void func_80139A58(void) {
    if (D_800E6280.unk_F5F == 0xD) {
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
    if (D_800E6280.unk_F5F != 0xD) {
        func_800462C8(9, 0x80162000, 0x497C);
    } else {
        func_800462C8(0xA, 0x80162000, 0x4985);
    }
    func_8004284C();
}

s32 func_80139E08(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        func_800AE0B0("DEC_BS header Error \n");
        func_800AE0B0("%x %x\n", ((u32 *)D_80125C04)[0], ((u32 *)D_80125C04)[1]);
        D_800B5920[2] = 0;
        D_800B5920[3] = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_800536AC(0x140, 0x100, 0x140, 0xF0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139EC4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_80139F3C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A004);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013A2E8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B510);

s32 func_8013B5E4(void) {
    if ((u32)D_800E6280.unk_1104.w++ < 0x80U) {
        return 0;
    }
    func_8004E884(D_80122CD0);
    D_80122CD4 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/801396E0", func_8013B640);
