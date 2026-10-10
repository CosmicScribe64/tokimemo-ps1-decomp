#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_80148A7C;

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_80148A4C;

typedef struct {
    s32 w;
} Word4; /* size 0x4 */
extern Word4 D_80148970;

void func_80143110(void) {
    *(Word4 *)&D_800F53A0.girl[D_800F62CF].unk_08 = D_80148970;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143110", func_80143160);

typedef struct {
    void (*f[16])();
} FnTbl16; /* size 0x40 */
extern FnTbl16 D_80148974;

void func_80143314(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl16 tbl;

    tbl = D_80148974;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80143390(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_801433B8(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_801489B4;

void func_801433E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_801489B4;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80143468(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_80143490(void) {
    if (((u32) D_800F55CA >> 4) == 3) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

void func_801434D8(void) {
    if (((u32) D_800F55CA >> 4) != 3) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_801489EC;

void func_80143520(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_801489EC;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_8014359C(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

extern FnTbl14 D_80148A14;

void func_801435C4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80148A14;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_8014364C(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void func_80143674(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        D_80148620 += 3;
    }
    func_8004DE1C();
}

void func_801436BC(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_801436E4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80148A4C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80143758(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_80143780(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148A7C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80143808(void) {
    D_80148620 = 4;
    func_8004DE1C();
}

void func_80143830(void) {
    func_80086AB0(0x505);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143110", func_80143858);

void func_801438E0(void) {
    if (func_8005B32C() != 0) {
        func_8005B06C(1);
        func_8004DE1C();
    }
}

void func_80143918(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

typedef struct {
    void (*f[11])();
} FnTbl11; /* size 0x2C */
extern FnTbl11 D_80148B04;

void func_80143940(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80148B04;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_801439C8(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

extern FnTbl11 D_80148B30;

void func_801439F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80148B30;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80143A78(void) {
    D_80148620 = 3;
    func_8004DE1C();
}

void func_80143AA0(void) {
    func_80072734(0x5617);
    func_8004DE1C();
}
