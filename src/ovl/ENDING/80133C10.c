#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80133C10);

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_8013C5BC;

void func_80133F1C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_8013C5BC;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

s32 func_80133F98(void) {
    D_800E7384 += 1;
    if ((u32)D_800E7384 < 2U) {
        func_80132000();
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8008585C();
        D_800E62BA = 0x80;
        D_800B593C = 0;
        D_800B5940 = 0;
        return 0;
    }
    if (D_800E7384 == 5) {
        func_8009C5E0(0);
    } else if ((u32)D_800E7384 < 0xBU) {
        return 0;
    }
    func_80134140();
    func_8004284C();
}

s32 func_8013408C(void) {
    D_800E7384 += 1;
    if ((u32)D_800E7384 < 2U) {
        func_800438DC(1, 0);
        func_80048DAC(1);
        func_800438F0(1);
        func_80048390();
        func_80041584();
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004E58C();
        func_8004E9F4(1);
        return 0;
    }
    if ((u32)D_800E7384 < 0xBU) {
        return 0;
    }
    func_801341D8();
    func_8004284C();
}

void func_80134140(void) {
    func_80097D90(0x100, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280 = 0x100;
}

void func_801341D8(void) {
    func_80097D90(0x140, 0xF0, 0, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, 0xF0);
    D_800E8C70 = 8;
    D_800E8C74 = D_800E8CA0;
    D_800E8C84 = 8;
    D_800E8C88 = D_800E90A0;
    func_80098530();
    InitGeom();
    D_800E6280 = 0x140;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134270);

void func_801349A8(void) {
    func_80046318(0x78, 0x80180000, 0x7B58);
    func_80133C10();
    func_8004284C();
}

void func_801349E0(void) {
    s16 t;
    s16 i;

    t = D_80120676 - 1;
    for (i = 0; i < 6; i++) {
        *(s16 *)((u8 *)D_801217D0 + i * 0x24 + 0x94) -= 1;
    }
    D_80120676 = t;
    if (t < -0x9E) {
        D_8011ECD0[0x1982] = 1;
        *(s16 *)&D_8011ECD0[0x1996] = 1;
        *(s16 *)&D_8011ECD0[0x1998] = 0;
        *(s16 *)&D_8011ECD0[0x1988] = 0;
        D_8011ECD0[0x19C7] |= 0x80;
        D_8011ECD0[0x19C6] = 5;
        func_8004284C();
    }
}

void func_80134AA8(void) {
    if ((D_80120696 == 0) && (D_80120668 == 1) && (D_80120658 == 0x14)) {
        D_80120666 = 2;
        D_80120668 = 0;
        D_80120658 = 0;
        D_80120652 = 5;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134B18);

void func_80134BE8(void) {
    if (D_80120668 == 2) {
        if (D_80120658 == 0x2F) {
            func_8004284C();
        }
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80133C10", func_80134C2C);
