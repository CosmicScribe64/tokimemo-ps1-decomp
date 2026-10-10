#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_80145ECC;

void func_80134520(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_80145ECC;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_8013459C(void) {
    func_800504CC(1, 0xB678, 0xB65A, 0xC2FD, 0xC2D9, 0xC2D4);
    func_8004DE1C();
}

void func_801345E0(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x200);
        func_8004DE1C();
    }
}

void func_8013461C(void) {
    func_80051DD8(0x33, 0x80197000, 0xA400);
    func_80134000();
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134520", func_80134658);

void func_801347AC(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134520", func_801347D4);

void func_80134938(void) {
    u32 temp_v0;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_v0 = (u8)func_8005E0E0(D_800F62CF);
    switch (D_800F62CF) {
    case 0:
        D_80145EA8 = D_80145E08;
        D_80145EAC = D_80145E40;
        D_80145EB0 = D_80145E78;
        return;
    case 1:
        D_80145EA8 = D_80145E18;
        D_80145EAC = D_80145E50;
        D_80145EB0 = D_80145E88;
        return;
    case 2:
        D_80145EA8 = D_80145E0C;
        D_80145EAC = D_80145E44;
        D_80145EB0 = D_80145E7C;
        return;
    case 3:
        D_80145EA8 = D_80145E10;
        D_80145EAC = D_80145E48;
        D_80145EB0 = D_80145E80;
        return;
    case 4:
        D_80145EA8 = D_80145E2C;
        D_80145EAC = D_80145E64;
        D_80145EB0 = D_80145E9C;
        return;
    case 5:
        D_80145EA8 = D_80145E1C;
        D_80145EAC = D_80145E54;
        D_80145EB0 = D_80145E8C;
        return;
    case 6:
        D_80145EA8 = D_80145E24;
        D_80145EAC = D_80145E5C;
        D_80145EB0 = D_80145E94;
        if ((temp_v0 & 0x7F) < 2U) {
            D_80145EA8 = D_80145E20;
            D_80145EAC = D_80145E58;
            D_80145EB0 = D_80145E90;
        }
        return;
    case 7:
        D_80145EA8 = D_80145E14;
        D_80145EAC = D_80145E4C;
        D_80145EB0 = D_80145E84;
        return;
    case 8:
        D_80145EA8 = D_80145E04;
        D_80145EAC = D_80145E3C;
        D_80145EB0 = D_80145E74;
        return;
    case 9:
        D_80145EA8 = D_80145E28;
        D_80145EAC = D_80145E60;
        D_80145EB0 = D_80145E98;
        return;
    case 10:
        D_80145EA8 = D_80145E30;
        D_80145EAC = D_80145E68;
        D_80145EB0 = D_80145EA0;
    }
}
