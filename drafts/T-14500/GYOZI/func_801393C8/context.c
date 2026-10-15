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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_8013907C);

/* bulk_m2c declarations (T-11190) */
extern u8 D_80147244;
u32 func_8005EBE0();
/* end bulk_m2c declarations */
void func_801393C8(void) {
    s32 sp28;
    s32 var_a2;
    s32 var_v1;
    u32 temp_v0;
    u32 temp_v0_2;
    u8 temp_a1;
    u8 temp_v0_3;
    u8 var_v0;

    temp_v0 = D_800F6468 & 7;
    if (temp_v0 == 0) {
        var_v0 = 3;
    } else {
        var_v0 = 1;
        if (temp_v0 < 4U) {
            var_v0 = 0;
        }
    }
    if (D_800F62CF == 0) {
        var_v0 = D_800F6468 & 1;
    }
    if (var_v0 != 0) {
        if (var_v0 != 1) {
            D_80147244 = var_v0;
            if (var_v0 != 2) {
                func_800BCE10(&D_800D92E0, "屋上");
                var_a2 = 0x4023;
            } else {
                func_800BCE10(&D_800D92E0, "校舎裏");
                var_a2 = 0x401A;
            }
        } else {
            D_80147244 = var_v0;
            func_800BCE10(&D_800D92E0, "教室");
            var_a2 = 0x3FFF;
        }
    } else {
        D_80147244 = var_v0;
        func_800BCE10(&D_800D92E0, "廊下");
        var_a2 = 0x3FED;
    }
    var_v1 = D_8014723C - 1;
    if (D_80147220 == var_v1) {
        func_800BCE10(&D_800D92E0, "校門前", var_a2);
        var_a2 = 0x3FDA;
        var_v1 = D_8014723C - 1;
    }
    if ((D_80147220 == var_v1) && (D_800F62CF == 0)) {
        sp28 = var_a2;
        temp_v0_2 = func_8005EBE0(D_800F62CF, D_800F62CF, var_a2);
        if (temp_v0_2 >= 0x46U) {
            func_800BCE10(&D_800D92E0, "自宅前", var_a2);
            var_a2 = 0x40ED;
        }
    }
    temp_a1 = D_800F62CF;
    if ((temp_a1 == 9) && (D_80147240 == 1) && ((u8) D_80147244 >= 2U)) {
        func_800BCE10(&D_800D92E0, "教室", var_a2);
        var_a2 = 0x3FF5;
    }
    temp_v0_3 = D_80147244;
    if (temp_a1 == 9) {
        D_80147244 = temp_v0_3;
        if (D_80147240 == 1) {
            D_80147244 = temp_v0_3;
            if (temp_v0_3 < 2U) {
                sp28 = 0x3FED;
                func_800BCE10(&D_800D92E0, "廊下", 0x3FED);
                var_a2 = 0x3FED;
            }
        }
    }
    switch (var_a2) {
    case 0x3FED:
        sp28 = var_a2;
        func_80090960(6, 0, var_a2);
        break;
    case 0x3FF5:
        sp28 = var_a2;
        func_80090960(6, 1, var_a2);
        break;
    case 0x40ED:
        sp28 = var_a2;
        func_80090960(6, 5, var_a2);
        break;
    case 0x3FDA:
        sp28 = var_a2;
        func_80090960(6, 4, var_a2);
        break;
    case 0x401A:
        sp28 = var_a2;
        func_80090960(6, 3, var_a2);
        break;
    default:
        sp28 = var_a2;
        func_80090960(6, 2, var_a2);
        break;
    }
    func_8008A0D4(var_a2);
    func_8004DE1C();
}

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
