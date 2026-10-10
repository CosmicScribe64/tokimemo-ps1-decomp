#include "common.h"
#include "ovl/DATE.h"

void func_80157880(void) {
    D_801600A0 = 0x801CE090;
    D_801600A4 = 0x801CE094;
    D_801600A8 = 0x801CE0AC;
    D_801600AC = *(s16 *)0x801CE0C0;
    D_801600B0 = 0x801B0000;
    D_801600B4 = 0x801B2000;
    D_801600B8 = 0x801B6000;
    D_801600BC = 0x801BA000;
    D_801600C0 = 0x801BE000;
    D_801600C4 = 0x801C2000;
    D_801600C8 = 0x801C6000;
}

void func_80157930(void) {
    D_801600CC = 0x801D2240;
    D_801600D0 = 0x801D2248;
    D_801600D4 = 0x801D2288;
    D_801600D8 = *(s16 *)0x801D22A0;
    D_801600DC = 0x801B0000;
    D_801600E0 = 0x801B2000;
    D_801600E4 = 0x801B6000;
    D_801600E8 = 0x801BA000;
    D_801600EC = 0x801BE000;
    D_801600F0 = 0x801C2000;
    D_801600F4 = 0x801C6000;
}

void func_801579E0(void) {
    D_801600F8 = 0x801CE078;
    D_801600FC = 0x801CE07C;
    D_80160100 = 0x801CE08C;
    D_80160104 = *(s16 *)0x801CE098;
    D_80160108 = 0x801B0000;
    D_8016010C = 0x801B2000;
    D_80160110 = 0x801B6000;
    D_80160114 = 0x801BA000;
    D_80160118 = 0x801BE000;
    D_8016011C = 0x801C2000;
    D_80160120 = 0x801C6000;
}

void func_80157A90(void) {
    D_80160124 = 0x801CE0F4;
    D_80160128 = 0x801CE0F8;
    D_8016012C = 0x801CE118;
    D_80160130 = *(s16 *)0x801CE12C;
    D_80160134 = 0x801B0000;
    D_80160138 = 0x801B2000;
    D_8016013C = 0x801B6000;
    D_80160140 = 0x801BA000;
    D_80160144 = 0x801BE000;
    D_80160148 = 0x801C2000;
    D_8016014C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80157B40);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80157BD4);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80157D68);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80157E88);

void func_80157FE4(void) {
    D_8015E208 = D_8015DF3C;
    D_8015E20C = D_8015E078;
    D_8015E210 = D_8015E1B4;
    D_800E6280.unk_1BC[9].unk_02 += 3;
    D_800E6280.unk_1BC[9].unk_06 += 2;
    D_800E6280.unk_1BC[9].unk_0A -= 0x14;
    func_80084D3C();
    func_80157A90();
    func_80043914(D_80160134, 0x11, 1, 2, 0);
    func_80084E90(D_80160138, D_8016013C, D_80160140, D_80160144, D_80160148, D_8016014C);
    func_800850D4(D_80160128, D_8016012C, D_80160124, D_80160130);
    func_8004284C();
}

void func_801580FC(void) {
    D_800E6280.unk_F5F = 9;
    bg_read_sub2(0x4818);
    func_8004284C();
}

void func_80158130(void) {
    func_80044750(0x200);
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80158160(void) {
    bg_read_sub2(0x47F1);
    func_8004284C();
}

void func_80158188(void) {
    bg_read_sub2(0x48AC);
    func_8004284C();
}

void func_801581B0(void) {
    bg_read_sub2(0x457D);
    func_8004284C();
}

void func_801581D8(void) {
    bg_read_sub2(0x456C);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80158200);

void func_80158288(void) {
    D_800E6280.unk_F5F = 9;
    D_80122D20 = 0;
    func_8014C5C8();
}

void func_801582B8(void) {
    if ((u16) D_800CA154 >= 4U) {
        D_800E6280.unk_F5F = 0xE;
        D_80122D10 = 2;
    }
    func_8014C5C8();
}

void func_80158300(void) {
    func_80046318(0x3D, 0x801B0000, 0x8ACD);
    func_80157880();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80158338);

void func_80158444(void) {
    func_80046318(0x45, 0x801B0000, 0x8B0A);
    func_80157930();
    func_8004284C();
}

void func_8015847C(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801584A4(void) {
    func_80044750(0x503);
    func_8004284C();
}

void func_801584CC(void) {
    func_80044750(0x504);
    func_8004284C();
}

void func_801584F4(void) {
    func_80044750(0x505);
    func_8004284C();
}

void func_8015851C(void) {
    func_80044750(0x506);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80158544);

void func_801585B8(void) {
    func_80046318(0x3D, 0x801B0000, 0x8B4F);
    func_801579E0();
    func_8004284C();
}

typedef struct {
    void (*f[36])();
} FnTbl36; /* size 0x90 */
extern FnTbl36 D_801603B0;

void func_801585F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl36 tbl;

    tbl = D_801603B0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80157880", func_80158664);
