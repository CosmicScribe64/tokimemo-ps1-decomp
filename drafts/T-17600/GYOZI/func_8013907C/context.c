#include "common.h"
#include "ovl/GYOZI.h"

void func_80138B50(void) {
    func_8008A0D4(0x3FED);
    func_80090960(6, 0);
    func_8004DE1C();
}

void func_80138B84(void) {
    func_8008A0D4(0x40C1);
    func_8004DE1C();
}

void func_80138BAC(void) {
    D_80147198 = (D_80147198 + (D_800F53DE * 3)) - 0x120;
    func_8004DE1C();
}

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80147248;

void func_80138BF0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_80147248;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void func_80138C78(void) {
    D_800F62CF = 0;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_80138C9C);

/* bulk_m2c declarations (T-11190) */
extern s32 D_80147224;
u32 func_8005EBE0();
/* end bulk_m2c declarations */
void func_8013907C(void) {
    u8 sp2B;
    s32 var_at;
    s32 var_t8;
    u8 temp_a0;

    if (D_80147220 == D_8014723C) {
        func_8004DDD8();
        return;
    }
    temp_a0 = *(&D_80147224 + (D_80147220 * 2));
    D_800F62CF = temp_a0;
    func_8008F618(temp_a0);
    sp2B = func_8005E0E0(D_800F62CF) & 0x7F;
    D_80147198 = func_8005EBE0(D_800F62CF) >= 0x46U;
    if ((D_800F62CF == 9) && (D_80147240 == 1)) {
        D_80147198 = 3;
    }
    if (D_800F62CF == 0) {
        var_at = (u8) D_800F62CF < 0xBU;
        if (D_80147240 == 1) {
            if (sp2B < 2U) {
                D_80147198 = 3;
            }
            goto block_9;
        }
    } else {
block_9:
        var_at = (u8) D_800F62CF < 0xBU;
    }
    if (var_at != 0) {
        switch (D_800F62CF) {
        case 0:
            D_8014718C = D_801470F8;
            var_t8 = D_80147160;
            D_80147190 = D_8014712C;
block_23:
            D_80147194 = var_t8;
            break;
        case 1:
            D_8014718C = D_8014710C;
            D_80147190 = D_80147140;
            D_80147194 = D_80147174;
            break;
        case 2:
            D_8014718C = D_801470FC;
            D_80147190 = D_80147130;
            D_80147194 = D_80147164;
            break;
        case 3:
            D_8014718C = D_80147104;
            D_80147190 = D_80147138;
            D_80147194 = D_8014716C;
            break;
        case 4:
            D_8014718C = D_8014711C;
            D_80147190 = D_80147150;
            D_80147194 = D_80147184;
            break;
        case 5:
            D_8014718C = D_80147110;
            D_80147190 = D_80147144;
            D_80147194 = D_80147178;
            break;
        case 6:
            D_8014718C = D_80147114;
            D_80147190 = D_80147148;
            D_80147194 = D_8014717C;
            break;
        case 7:
            D_8014718C = D_80147108;
            D_80147190 = D_8014713C;
            D_80147194 = D_80147170;
            break;
        case 8:
            D_8014718C = D_801470F0;
            D_80147190 = D_80147124;
            D_80147194 = D_80147158;
            break;
        case 9:
            D_8014718C = D_80147118;
            D_80147190 = D_8014714C;
            D_80147194 = D_80147180;
            break;
        case 10:
            D_8014718C = D_80147120;
            var_t8 = D_80147188;
            D_80147190 = D_80147154;
            goto block_23;
        }
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_801393C8);

void func_801396D8(void) {
    D_80147220 += 1;
    func_8004DEE4(1);
}

void func_8013970C(void) {
    if (func_800BDC20(&D_800D92A0, &D_800D92E0) == 0) {
        func_8004DE1C();
    }
    func_800BCE10(&D_800D92A0, &D_800D92E0);
    func_8004DE1C();
}

void func_80139764(void) {
    if ((D_80147240 == 1) && (D_800F62CF == 9)) {
        D_800F647A += 6;
        return;
    }
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80138B50.rodata", D_80145A40);

void func_801397BC(void) {
    func_800BCE10(&D_800D92E0, &D_80145A40);
    func_8008A0D4(0x401A);
    func_8004DE1C();
}

void func_801397F8(void) {
    func_8008F618(0);
    func_80072734(0x53A1);
    func_8004DE1C();
}

void func_80139828(void) {
    func_8008F618(9);
    func_80072734(0x63CD);
    func_8004DE1C();
}

s32 func_80139858(void) {
    s32 v;

    v = D_800F5490;
    if (D_800F53E0 == ((u32)(v << 0x17) >> 0x1B) && D_800F53DF == (v & 0xF)
        && func_8005EBE0(D_800F62CF) >= 0x46U) {
        D_800F5AB1 = D_800F6479;
        /* FAKE: D_800F647A read through the D_800F5AB1 base so as1 keeps the load after the store. T-11430 */
        D_800F5AB2 = (&D_800F5AB1)[0x9C9] + 1;
        func_8004DEAC(3);
        return 0;
    }
    if (func_8005EBE0(D_800F62CF) >= 0x46U) {
        func_80138858();
        return;
    }
    func_8004DE1C();
}
