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

/* NON_MATCHING: T-24301, 5 rows; the original keeps 0 in $a1 from entry (`or $a1,$zero,$zero; sb $a1,D_800F62CF;
 * or $a0,$a1,$zero`) and has a 0x28 frame (0x30 here). Zero locals of every width, assignment-expression
 * arguments and chained stores all fold to $zero (T-24116, T-24301). */
#ifdef NON_MATCHING
void func_80140E0C(void) {
    s16 i;

    D_80148750 = 0;
    if (((GyoziBits32 *)D_800F53A0.girl[9].unk_10)->b1 == 0 &&
        D_800F53A0.girl[9].unk_06 >= 20 && (D_800F6468 & 1)) {
        D_80148750 = 1;
    }
    D_800F53A0.unk_F2F = 0;
    /* FAKE: the byte view of the girl index makes the later reload use $a1 for its address. T-24301 */
    D_80148620 = (((u8)func_8005E0E0(((u8 *)&D_800F53A0)[0xF2F]) & 0x7F) == 4) * 4;
    D_80148614 = D_801481D4;
    if (i) {} /* FAKE: T-24301 (permuter), keeps i live from entry: the loop counter gets $v1 as in the original */
    D_80148618 = D_80148208;
    D_8014861C = D_8014823C;
    func_8008F618(D_800F53A0.unk_F2F);
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 4;
    for (i = 0; i < 7; i++) {
        if (D_800F54AE < D_8014875C[i]) {
            break;
        }
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += D_8014876C[i];
    func_80090960(4, 0);
    func_8004DE1C();
}
#else
INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140E0C);
#endif

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_80140F90);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_801412A4);

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
