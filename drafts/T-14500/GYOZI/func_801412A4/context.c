#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    s32 (*f[26])();
} FnTbl26R; /* size 0x68 */
extern FnTbl26R D_8014877C;

/* The record fields at 0x8012B8C7..0x8012B8D8 are reached through one base: the original keeps the
 * stores and loads in program order (separate scalars let as1 hoist the loads). The result of the
 * last operation is left in $v0 without a return statement (T-4060). */
s16 func_80140C90(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl26R tbl;

    tbl = D_8014877C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    *(s8 *)(&D_8012B8C2 + 5) = *(u8 *)&D_801317EB;
    if ((*(s16 *)(&D_8012B8C2 + 0x16) == 0) && (*(s16 *)(&D_8012B8C2 + 6) == 0)) {
        *(s16 *)(&D_8012B8C2 + 0x14) = *(s16 *)(&D_8012B8C2 + 0x14) + 1;
        if (*(s16 *)(&D_8012B8C2 + 0x14) >= 3) {
            *(s16 *)(&D_8012B8C2 + 0x14) = 0;
        }
    }
}

void func_80140D7C(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_8005E0E0(D_800F62CF) & 0x7F) != 4) {
        func_8004DE1C();
        return;
    }
    func_8013FD20();
}

void func_80140DCC(void) {
    if (D_80148750 != 0) {
        D_80148620 += 1;
    }
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140E0C);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140F90);

/* bulk_m2c declarations (T-11190) */
extern s32 D_8014875C;
extern s32 D_8014876C;
extern u8 D_800F5AB0;
extern u8 D_800F62CE;
extern u8 D_800F6478;
void func_80141550(void);
/* end bulk_m2c declarations */
void func_801412A4(void) {
    s16 temp_t7;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_a0;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s32 var_v1;
    u32 temp_hi;
    u32 temp_t8;
    u8 temp_t1;

    var_a0 = 0;
    var_v0 = 0;
    do {
        temp_v0 = var_v0 + 1;
        var_a0 += ((u32) (*(s32 *)((unsigned char *)(&D_800F53A0.girl[var_v0]) + (16)) << 0x1E) >> 0x1F) != 0;
        var_v0 = temp_v0;
    } while (temp_v0 < 0xB);
    var_v0_2 = 0;
loop_3:
    temp_hi = (u32) (D_800F6468 + var_v0_2) % 10U;
    var_v0_2 += 1;
    temp_t1 = temp_hi + 1;
    D_800F62CF = temp_t1;
    temp_t8 = (u32) (*(s32 *)((unsigned char *)(&D_800F53A0.girl[temp_t1 & 0xFF]) + (16)) << 0x1E) >> 0x1F;
    if (temp_t8 == 0) {
        if ((u32) (temp_t1 & 0xFF) >= 9U) {
            goto loop_3;
        }
    }
    if (D_80148754 == D_800F62CF) {
        goto loop_3;
    }
    if ((temp_t8 == 0) && (var_a0 >= 3)) {
        goto loop_3;
    }
    if (((D_801486E4 == 7) || (D_801486E4 == 9)) && (D_80148754 != D_801486E4)) {
        D_800F62CF = D_801486E4;
    }
    D_80148758 = D_800F62CF;
    D_80148620 = 0;
    func_80141550();
    func_8008F618(D_800F62CF);
    var_v0_3 = 0;
    if (!(*(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (16)) & 2)) {
        D_800F53A0.girl[D_800F62CF].unk_10[0] |= 2;
        D_800F5AB0 = D_800F6478;
        D_800F5AB1 = D_800F6479;
        D_800F5AB2 = 0x18;
        func_80090960(4, 0, &D_800F53A0 + (D_800F62CF * 0x38), &D_800F53A0);
        *(s8 *)((unsigned char *)(&D_800F53A0 + (D_800F62CE * 8)) + (1841)) = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F62CF].unk_0A += 4;
loop_15:
    var_v1 = var_v0_3 * 2;
    temp_v0_2 = var_v0_3 + 1;
    if (D_800F54AE >= *(&D_8014875C + var_v1)) {
        temp_t7 = temp_v0_2;
        var_v0_3 = temp_t7;
        if (temp_t7 >= 7) {
            var_v1 = temp_t7 * 2;
        } else {
            goto loop_15;
        }
    }
    D_800F53A0.girl[D_800F62CF].unk_06 += *(&D_8014876C + var_v1);
    func_80090960(4, 0, &D_800F53A0 + (D_800F62CF * 0x38), &D_800F53A0);
    func_8004DE1C();
}

void func_80141550(void) {
    switch (D_800F62CF) {
    case 1:
        D_80148614 = D_801481E4;
        D_80148618 = D_80148218;
        D_8014861C = D_8014824C;
        return;
    case 2:
        D_80148614 = D_801481D8;
        D_80148618 = D_8014820C;
        D_8014861C = D_80148240;
        return;
    case 3:
        D_80148614 = D_801481DC;
        D_80148618 = D_80148210;
        D_8014861C = D_80148244;
        return;
    case 4:
        D_80148614 = D_801481F4;
        D_80148618 = D_80148228;
        D_8014861C = D_8014825C;
        return;
    case 5:
        D_80148614 = D_801481E8;
        D_80148618 = D_8014821C;
        D_8014861C = D_80148250;
        return;
    case 6:
        D_80148614 = D_801481EC;
        D_80148618 = D_80148220;
        D_8014861C = D_80148254;
        return;
    case 7:
        D_80148614 = D_801481E0;
        D_80148618 = D_80148214;
        D_8014861C = D_80148248;
        return;
    case 8:
        D_80148614 = D_801481D0;
        D_80148618 = D_80148204;
        D_8014861C = D_80148238;
        return;
    case 9:
        D_80148614 = D_801481F0;
        D_80148618 = D_80148224;
        D_8014861C = D_80148258;
        return;
    case 10:
        D_80148614 = D_80148200;
        D_80148618 = D_80148234;
        D_8014861C = D_80148268;
    default:
        return;
    }
}

void func_80141790(void) {
    switch (D_800F62CF) {
    case 0:
        func_80072734(0x5617);
        break;
    case 1:
        func_80072734(0x5DA6);
        break;
    case 2:
        func_80072734(0x52C0);
        break;
    case 3:
        func_80072734(0x5941);
        break;
    case 4:
        func_80072734(0x66CA);
        break;
    case 5:
        func_80072734(0x6049);
        break;
    case 6:
        func_80072734(0x6292);
        break;
    case 7:
        func_80072734(0x5BB7);
        break;
    case 8:
        func_80072734(0x50FE);
        break;
    case 9:
        if (D_80148750 != 0) {
            func_80072734(0x6508);
        } else {
            func_80072734(0x63A0);
        }
        break;
    case 10:
        func_80072734(0x677E);
        break;
    }
    func_8004DE1C();
}
