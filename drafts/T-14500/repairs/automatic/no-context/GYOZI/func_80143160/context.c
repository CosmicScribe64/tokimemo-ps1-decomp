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

/* bulk_m2c declarations (T-11190) */
extern s16 D_8012B8C8;
extern s16 D_8012B8D6;
extern s16 D_8012B8D8;
extern s32 D_800F53A0;
extern s32 D_80148970;
extern u8 D_800F62CF;
extern u8 D_800F647A;
extern u8 D_8012B8C7;
extern u8 D_801317EB;
s32 func_80052060();
s32 func_801436E4();
s32 func_80143780();
s32 func_80143858();
void func_801439F0(void);
void func_80143940(void);
void func_801435C4(void);
void func_80143520(void);
void func_801433E0(void);
/* end bulk_m2c declarations */
void func_80143160(void) {
    s16 temp_v0;

    if (D_800F647A == 0) {
        D_80148970 = *(s32 *)((unsigned char *)(&D_800F53A0 + (D_800F62CF * 0x38)) + (432));
        *(s16 *)((unsigned char *)(&D_800F53A0 + (D_800F62CF * 0x38)) + (434)) = 0x32;
    }
    switch (D_800F62CF) {
    case 0:
        func_80052060();
        break;
    case 1:
        func_80143314();
        break;
    case 2:
        func_801433E0();
        break;
    case 3:
        func_80143520();
        break;
    case 4:
        func_801435C4();
        break;
    case 5:
        func_801436E4();
        break;
    case 6:
        func_80143780();
        break;
    case 7:
        func_80143858();
        break;
    case 8:
        func_80143940();
        break;
    case 9:
        func_801439F0();
        break;
    case 10:
        func_80052060();
        break;
    }
    D_8012B8C7 = D_801317EB;
    if ((D_8012B8D8 == 0) && (D_8012B8C8 == 0)) {
        temp_v0 = D_8012B8D6 + 1;
        D_8012B8D6 = temp_v0;
        if (temp_v0 >= 3) {
            D_8012B8D6 = 0;
        }
    }
}

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

extern FnTbl17 D_80148AC0;

void func_80143858(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148AC0;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

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
