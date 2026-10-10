#include "common.h"
#include "ovl/GYOZI.h"

void func_8013BD50(void) {
    func_8008A0D4(0x4080);
    func_8004DE1C();
}

void func_8013BD78(void) {
    func_8008A0D4(0x4078);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013BD50", func_8013BDA0);

void func_8013BEA0(void) {
    D_801479B0 = (u8 *)0x801E8088;
    D_801479B4 = (u8 *)0x801E808C;
    D_801479B8 = (u8 *)0x801E80A0;
    D_801479BC = *(s16 *)0x801E80B4;
    D_801479C0 = (u8 *)0x801B0000;
    D_801479C4 = (u8 *)0x801B2000;
    D_801479C8 = (u8 *)0x801B4000;
    D_801479CC = (u8 *)0x801B8000;
    D_801479D0 = (u8 *)0x801BC000;
    D_801479D4 = (u8 *)0x801C0000;
    D_801479D8 = (u8 *)0x801C4000;
    D_801479DC = (u8 *)0x801C8000;
    D_801479E0 = (u8 *)0x801D0000;
    D_801479E4 = (u8 *)0x801D4000;
    D_801479E8 = (u8 *)0x801D8000;
    D_801479EC = (u8 *)0x801DC000;
    D_801479F0 = (u8 *)0x801E0000;
    D_801479F4 = (u8 *)0x801E4000;
}

void func_8013BFB4(void) {
    func_8013BFD4();
}

typedef struct {
    void (*f[54])();
} FnTbl54; /* size 0xD8 */
extern FnTbl54 D_801479F8;

void func_8013BFD4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl54 tbl;

    tbl = D_801479F8;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    if (D_801474A8 == 0) {
        if (D_801474AC == 3) {
            D_8012E684 = 0;
        }
    }
}

void func_8013C070(void) {
    func_80051DD8(0x71, 0x801B0000, 0x82A1);
    func_8013BEA0();
    func_8004DE1C();
}

void func_8013C0A8(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_8013C0D0(void) {
    func_8013A820();
    if (D_801474AC == 3) {
        D_8012E6B0 = 1;
        return;
    }
    D_8012E6B0 = 0;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/8013BD50", func_8013C118);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/8013BD50.rodata", D_80145BA0);

void func_8013C310(void) {
    D_801474A8 = 0;
    D_801474AC = 0;
    func_800BCE10(&D_800D92A0, &D_80145BA0);
    func_8004DE1C();
}

void func_8013C354(void) {
    func_8008A0D4(0x3FED);
    func_8004DE1C();
}

void func_8013C37C(void) {
    func_8008A0D4(0x466F);
    func_8004DE1C();
}
