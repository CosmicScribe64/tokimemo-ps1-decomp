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

/* bulk_m2c declarations (T-11190) */
extern s16 D_800F5746;
extern s16 D_80147224;
extern s8 D_80147244;
u32 func_8005EBE0();
u32 func_8005F220();
/* end bulk_m2c declarations */
void func_80138C9C(void) {
    s32 sp4C[16];
    s16 *sp44;
    s16 *temp_t8;
    s16 *temp_v0;
    s16 *temp_v1;
    s16 temp_t7;
    s16 temp_v0_2;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s2;
    s32 temp_hi;
    s32 var_t1;
    s32 var_t1_2;
    s32 var_t2;
    s32 var_t9;
    s32 var_t9_2;
    u8 *temp_s1;

    D_80147220 = 0;
    D_80147244 = 0;
    D_8014723C = 0;
    D_80147240 = 0;
    var_s2 = 0;
    if (!(D_800F5750 & 2) && (D_800F5746 >= 0x14)) {
        D_80147240 = 1;
    }
    var_s0 = 0;
    var_t1 = 0 * 8;
    do {
        temp_s1 = ((var_t1 - var_s0) * 8) + &D_800F53A0;
        if (((*(s32 *)((unsigned char *)temp_s1 + (440)) & 2) || ((var_s0 == 9) && (D_80147240 == 1))) && ((func_8005F220(var_s0) >= 0x32U) || ((D_80147240 == 1) && (var_s0 == 9))) && ((var_t9 = var_s2 * 2, (D_80147240 == 0)) || (var_t9 = var_s2 * 2, (var_s0 != 0)) || (var_t9 = var_s2 * 2, ((func_8005EBE0(0) < 0x46U) == 0)))) {
            *(&sp4C[0] + var_t9) = var_s0;
            D_8014723C += 1;
            var_s2 += 1;
            *(u8 *)((unsigned char *)temp_s1 + (441)) = (u8) (*(u8 *)((unsigned char *)temp_s1 + (441)) | 1);
        }
        var_s0 += 1;
        var_t1 = var_s0 * 8;
    } while (var_s0 < 0xB);
    var_s0_2 = 0;
    if (D_8014723C > 0) {
        var_t1_2 = 0 * 2;
        do {
            func_800BCDF0("%d ", *(&sp4C[0] + var_t1_2));
            var_s0_2 += 1;
            var_t1_2 = var_s0_2 * 2;
        } while (var_s0_2 < D_8014723C);
        var_s0_2 = 0;
    }
    func_800BCDF0("\n");
    if (D_8014723C > 0) {
        do {
            temp_hi = (s32) (var_s0_2 + (s16) ((u32) D_800F6468 % (u32) D_8014723C)) % (s16) D_8014723C;
            temp_t8 = &(&D_80147224)[var_s0_2];
            temp_t7 = var_s0_2 + 1;
            var_s0_2 = temp_t7;
            *temp_t8 = (&sp4C[0])[temp_hi];
        } while (temp_t7 < D_8014723C);
        var_s0_2 = 0;
    }
    if (D_8014723C > 0) {
        var_t2 = var_s0_2 * 2;
        do {
            func_800BCDF0("%d ", *(&D_80147224 + var_t2));
            var_s0_2 += 1;
            var_t2 = var_s0_2 * 2;
        } while (var_s0_2 < D_8014723C);
        var_s0_2 = 0;
    }
    func_800BCDF0("\n");
    var_t9_2 = var_s0_2 * 2;
    if (D_8014723C > 0) {
loop_27:
        temp_v1 = &D_80147224 + var_t9_2;
        if ((*temp_v1 == 0) && (sp44 = temp_v1, ((func_8005EBE0(0) < 0x46U) == 0))) {
            temp_v0 = &(&D_80147224)[D_8014723C];
            *temp_v1 = *(s16 *)((unsigned char *)temp_v0 + (-2));
            *(s16 *)((unsigned char *)temp_v0 + (-2)) = 0;
        } else {
            temp_v0_2 = *temp_v1;
            if (temp_v0_2 == 0) {
                *temp_v1 = D_80147224;
                D_80147224 = 0;
            } else if ((temp_v0_2 == 9) && (D_80147240 != 0)) {
                *temp_v1 = D_80147224;
                D_80147224 = 9;
            } else {
                var_s0_2 += 1;
                var_t9_2 = var_s0_2 * 2;
                if (var_s0_2 < D_8014723C) {
                    goto loop_27;
                }
            }
        }
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80138B50", func_8013907C);

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
