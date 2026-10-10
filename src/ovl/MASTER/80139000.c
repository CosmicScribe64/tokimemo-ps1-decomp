#include "ovl/MASTER.h"

void func_80139000(void) {
    D_8013C760 = 0x801A42D4;
    D_8013C764 = 0x801A42E0;
    D_8013C768 = 0x801A4330;
    D_8013C76C = *(s16 *)0x801A4338;
    D_8013C770 = 0x80197000;
}

void func_80139054(void) {
    D_8013C760 = 0x801A3E3C;
    D_8013C764 = 0x801A3E48;
    D_8013C768 = 0x801A3E90;
    D_8013C76C = *(s16 *)0x801A3E98;
    D_8013C770 = 0x80197000;
}

void func_801390A8(void) {
    D_8013C760 = 0x801A42E4;
    D_8013C764 = 0x801A42F0;
    D_8013C768 = 0x801A4368;
    D_8013C76C = *(s16 *)0x801A4370;
    D_8013C770 = 0x80197000;
}

void func_801390FC(void) {
    D_8013C760 = 0x801A432C;
    D_8013C764 = 0x801A4338;
    D_8013C768 = 0x801A4394;
    D_8013C76C = *(s16 *)0x801A43A4;
    D_8013C770 = 0x80197000;
}

typedef struct {
    void (*f[7])();
} FnTbl7; /* size 0x1C */
extern FnTbl7 D_8013C798;

void func_80139150(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl7 tbl;

    tbl = D_8013C798;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_801391E4);

typedef struct {
    s32 v[13];
} Tbl13; /* size 0x34 */
extern Tbl13 D_8013C7B4;

void func_80139810(void) {
    s32 pad1; /* FAKE: two unused locals declared before tbl reproduce the original frame (0x68); T-8020 */
    s32 pad2;
    Tbl13 tbl;

    tbl = D_8013C7B4;
    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_8013C790 != 0xFF) {
            func_80062CD0(tbl.v[D_8013C790]);
            func_80042808();
        } else {
            D_8013C790 = 0xFF;
            func_8004284C();
        }
        break;
    case 1:
        func_8007E934();
        break;
    case 2:
        func_80042908(3);
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_80139910);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_80139CC8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013A784);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013ACF0);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B20C);

void func_8013B354(void) {
    func_80138F70();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C744;
    D_80120660 = (u8 *)D_8013C748;
    D_80120684 = (u8 *)D_8013C740;
    D_80120664 = D_8013C74C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B448);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80139000", func_8013B804);
