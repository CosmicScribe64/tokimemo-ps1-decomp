#define MAIN_API_OVERRIDE_func_8008FF60 /* s32 return (like DATE sibling func_800850D4 in main_api.h): consumes $v0 in ugen for func_801444CC and func_80145084 (T-24098) */
#define GYOZI_OVERRIDE_func_80140050 /* s32 return consumes $v0 in ugen for func_8014494C (T-24098) */
#include "common.h"
#include "ovl/GYOZI.h"

s32 func_8008FF60();
s32 func_80140050();

void func_80143AD0(void) {
    D_80148B60 = 0x801DA680;
    D_80148B64 = 0x801DA68C;
    D_80148B68 = 0x801DA72C;
    /* address literals: the data belongs to another overlay; the original loads it with a separate base register */
    D_80148B6C = *(s16 *)0x801DA754;
    D_80148B70 = 0x801B0000;
    D_80148B74 = 0x801DA000;
    D_80148B78 = 0x801B2000;
    D_80148B7C = 0x801B6000;
    D_80148B80 = 0x801BA000;
    D_80148B84 = 0x801BE000;
    D_80148B88 = 0x801C2000;
    D_80148B8C = 0x801C6000;
    D_80148B90 = 0x801D6000;
}

void func_80143BA0(void) {
    D_80148B94 = 0x801D2430;
    D_80148B98 = 0x801D2434;
    D_80148B9C = 0x801D2498;
    /* address literal, see func_80141780 */
    D_80148BA0 = *(s16 *)0x801D24B4;
    D_80148BA4 = 0x801B0000;
    D_80148B74 = 0x801D2000;
    D_80148BA8 = 0x801B2000;
    D_80148BAC = 0x801B6000;
    D_80148BB0 = 0x801BA000;
    D_80148BB4 = 0x801BE000;
    D_80148BB8 = 0x801C2000;
    D_80148BBC = 0x801C6000;
    D_80148B90 = 0x801CE000;
}

void func_80143C70(void) {
    D_80148BC0 = 0x801D22D0;
    D_80148BC4 = 0x801D22D8;
    D_80148BC8 = 0x801D233C;
    D_80148BCC = *(s16 *)0x801D2358;
    D_80148BD0 = 0x801B0000;
    D_80148BD4 = 0x801B2000;
    D_80148BD8 = 0x801B6000;
    D_80148BDC = 0x801BA000;
    D_80148BE0 = 0x801BE000;
    D_80148BE4 = 0x801C2000;
    D_80148BE8 = 0x801C6000;
}

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80148EB4;

void func_80143D20(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80148EB4;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    if ((u8)D_800F647A < 0x10U) {
        D_8012B8C0[0].unk_07 = *(u8 *)&D_801317EB;
        if (D_8012B8C0[0].unk_18 == 0) {
            if (D_8012B8C0[0].unk_08 == 0) {
                D_8012B8C0[0].unk_16 += 1;
                if (D_8012B8C0[0].unk_16 >= 3) {
                    D_8012B8C0[0].unk_16 = 0;
                }
            }
        }
    }
    func_80144018();
}

void func_80143E08(void) {
    u32 r;
    u8 flag;

    flag = 1;
    r = func_8005B368();
    if (r >= 2U) {
        D_800F6474 += 1;
    }
    if (((u32)D_800F6474 < 0x73U || (u32)D_800F6474 % 60U != 6) && !((u8)D_8012B8C2 & 1)) {
        flag = 0;
    }
    func_80140050();
    if (flag == 0 && D_8012E668 != 0) {
        D_8012B8C2 = 0;
    }
}

void func_80143EB8(void) {
    func_80086AB0(0x206);
    func_8004DE1C();
}

void func_80143EE0(void) {
    D_80148BEC = func_8005B28C();
    D_8012E68C = 0x72;
    func_8005B264(0x1E);
    func_8005AEFC(2);
    func_8004DE1C();
}

void func_80143F2C(void) {
    func_8005B264(D_80148BEC);
    func_8005AEFC(1);
    func_8004DE1C();
}

void func_80143F64(void) {
    func_8004F860(0x280, 0, 0x40, 0x80, D_80148B90);
    D_800F65C0 = 1;
    func_8004DE1C();
}

void func_80143FAC(void) {
    func_80051DD8(0x55, 0x801B0000, 0x7D78);
    func_80143AD0();
    func_8004DE1C();
}

void func_80143FE4(void) {
    D_80148EAC = 1;
    D_8012CAD4 = -0x64;
    func_8004DE1C();
}

/* Fall-through s32 return retains the early-exit return 0. T-24098 */
s32 func_80144018(void) {
    s16 i;

    if (D_80148EAC == 0) {
        return 0;
    }
    if (D_80148EB0 == 0x1E) {
        D_80148EAC += 1;
        D_8012CA40[4].unk_04 = 0xB4;
        D_80148E48[0] = 1;
    } else if (D_80148EB0 == 0x5A) {
        D_80148EAC += 1;
        D_8012CA40[4].unk_04 = 0x28;
    /* FAKE: u16 pointer dereference on s16 D_800D9250 emits lhu instead of lh. T-24098 */
    } else if (*(u16 *)&D_800D9250 >= 6U && (u32)D_80148EB0 % 20U == 0 && D_80148EAC < 0x32) {
        D_80148EAC += 1;
        D_8012CA40[D_80148EAC + 3].unk_0E = (func_800BCE40() % 2) * 8;
        if (D_8012CA40[D_80148EAC + 3].unk_0E == 8) {
            D_800F53A0.unk_F30[0x148 + D_80148EAC] = 0xC;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 3;
        } else {
            D_800F53A0.unk_F30[0x148 + D_80148EAC] = 9;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 1;
        }
        D_80148BF0[D_80148EAC - 1] = 0x10000 / (D_80148E48[D_80148EAC - 1] + (D_80148E48[D_80148EAC - 1] >= 3) * 2 + 1);
    }
    for (i = 4; i < D_80148EAC + 4; i++) {
        D_8012CA40[i].unk_14 = D_8012CA40[i].unk_15 = D_8012CA40[i].unk_16 = D_801317EB;
        D_8012CA40[i].unk_00 &= 0x7FFFFFFF;
        if ((u32)D_80148EB0 % (u32)D_80148E48[i - 4] == 0) {
            D_8012CA40[i].unk_06 += 1;
        }
        ((Fix32 *)D_80148CB8)[i - 4].w += D_80148BF0[i - 4];
        D_80148BF0[i - 4] += (D_80148D80[i - 4] - ((Fix32 *)D_80148CB8)[i - 4].w) >> 8;
        D_8012CA40[i].unk_04 = ((Fix32 *)D_80148CB8)[i - 4].h[1];
        if (D_8012CA40[i].unk_06 >= 0x29) {
            D_8012CA40[i].unk_04 = func_800BCE40() % 320 - 0x9F;
            ((Fix32 *)D_80148CB8)[i - 4].h[0] = 0;
            ((Fix32 *)D_80148CB8)[i - 4].h[1] = D_8012CA40[i].unk_04;
            D_80148D80[i - 4] = ((Fix32 *)D_80148CB8)[i - 4].w;
            D_8012CA40[i].unk_06 = -0x78;
            D_8012CA40[i].unk_0E = (func_800BCE40() % 2) * 8;
            if (D_8012CA40[i].unk_0E == 8) {
                D_800F53A0.unk_F30[0x145 + i] = 0xC;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 3;
            } else {
                D_800F53A0.unk_F30[0x145 + i] = 9;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 1;
            }
            D_80148BF0[i - 4] = 0x10000 / (D_80148E48[i - 4] + (D_80148E48[i - 4] >= 3) * 2 + 1);
        }
    }
    D_80148EB0 += 1;
}

void func_801444CC(void) {
    s16 i;

    func_800BCE90(D_800F6468);
    D_800D9250 = 0;
    D_800D9254 = 0;
    func_80089200();
    func_8008F618(D_800F62CF);
    func_80143AD0();
    func_8013ED84();
    D_80148628 = D_8014829C;
    D_8014862C = D_801483D4;
    D_80148630 = D_8014850C;
    func_8004EE18(D_80148B74, 0x12, 1, 3, 0);
    for (i = 4; i < 0x36; i++) {
        D_8012CA40[i].unk_00 = 0xC1000000;
        D_8012CA40[i].unk_04 = func_800BCE40() % 320 - 0x9F;
        ((Fix32 *)D_80148CB8)[i - 4].h[0] = 0;
        ((Fix32 *)D_80148CB8)[i - 4].h[1] = D_8012CA40[i].unk_04;
        D_80148D80[i - 4] = ((Fix32 *)D_80148CB8)[i - 4].w;
        D_8012CA40[i].unk_06 = -0x78;
        D_8012CA40[i].unk_08 = 8;
        /* Rec24.unk_0A is u8[2] in game.h; original writes a 16-bit halfword sh. T-24098 */
        *(s16 *)&D_8012CA40[i].unk_0A = 8;
        D_8012CA40[i].unk_0C = 0xA;
        D_8012CA40[i].unk_0E = 0;
        D_8012CA40[i].unk_0F = 0;
        D_8012CA40[i].unk_10 = 0;
        D_8012CA40[i].unk_12 = 0x1F2;
        D_8012CA40[i].unk_14 = 0x80;
        D_8012CA40[i].unk_15 = 0x80;
        D_8012CA40[i].unk_16 = 0x80;
        D_8012CA40[i].unk_18 = 0;
        D_8012CA40[i].unk_1A = 0;
        D_8012CA40[i].unk_1C = 0x1000;
        D_8012CA40[i].unk_1E = 0x1000;
        D_8012CA40[i].unk_20 = 0;
        D_800F53A0.unk_F30[0x145 + i] = 9;
        D_80148E48[i - 4] = 2;
        D_80148BF0[i - 4] = 0x8000;
    }
    func_8004EE18(D_80148B70, 0x11, 1, 2, 0);
    func_8008FD1C(D_80148B78, D_80148B7C, D_80148B80, D_80148B84, D_80148B88, D_80148B8C);
    func_8008FF60(D_80148B64, D_80148B68, D_80148B60, D_80148B6C);
    D_8012B8C0[0].unk_04 = D_8012B8C0[1].unk_04 = 8;
    D_80148EAC = 0;
    D_80148EB0 = 0;
    D_800F53A0.girl[0].unk_06 += 1;
    D_800F53A0.girl[0].unk_0A += 2;
    func_8008FB00();
    func_8004DE1C();
}

void func_801447BC(void) {
    func_8008A0D4(0x46B9);
    func_80090960(4, 2);
    func_8004DE1C();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_80148F50;

void func_801447F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_80148F50;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    func_80144B70();
}

void func_8014486C(void) {
    func_80072734(0x5BE4);
    func_8004DE1C();
}

void func_80144894(void) {
    func_80086AB0(0x207);
    func_8004DE1C();
}

void func_801448BC(void) {
    func_8008E408(5);
    func_8004DE1C();
}

void func_801448E4(void) {
    func_80051DD8(0x45, 0x801B0000, 0x825C);
    func_80143BA0();
    func_8004DE1C();
}

void func_8014491C(void) {
    D_8012B94B &= 0x7F;
    func_8004DE1C();
}

void func_8014494C(void) {
    func_80140050();
    if (D_800F6474++ == 0) {
        /* FAKE: duplicate copy advances ugen's block-copy state to State B. T-24092 */
        D_8012B8C0[2] = D_8012B8C0[0];
        D_8012B8C0[2] = D_8012B8C0[0];
        D_8012B8C0[2].unk_16 = 4;
    }
    if ((u16)D_800D9250 == 7 && (u16)D_800D9254 == 1 && func_8005B32C() == 0) {
        D_8012B8C0[2].unk_02 = 1;
    } else {
        D_8012B8C0[2].unk_02 = 0;
        D_8012B8C0[2].unk_18 = 0;
    }
}

void func_80144A74(void) {
    if (D_8012E66C == 0) {
        D_800F647A += 0x18;
        return;
    }
    *(u16 *)&D_800D9250 += 2;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 1;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 2;
    func_8008FB00();
    func_8004DE1C();
}

void func_80144B28(void) {
    func_8004F860(0x280, 0, 0x40, 0x80, D_80148B90);
    D_800F65C0 = 1;
    func_8004DE1C();
}

/* Fall-through s32 return retains the early-exit return 0. T-24098 */
s32 func_80144B70(void) {
    s16 i;
    s16 count;

    if (D_80148EAC == 0) {
        return 0;
    }
    if (D_80148EB0 == 0x1E) {
        D_80148EAC += 1;
        D_8012CA40[4].unk_04 = 0xB4;
        D_80148E48[0] = 1;
    } else if (D_80148EB0 == 0x5A) {
        D_80148EAC += 1;
        D_8012CA40[4].unk_04 = 0x28;
    } else if ((u32)D_80148EB0 >= 0xC9U && (u32)D_80148EB0 % 20U == 0 && D_80148EAC < 0x32) {
        D_80148EAC += 1;
        D_8012CA40[D_80148EAC + 3].unk_0E = (func_800BCE40() % 2) * 8;
        if (D_8012CA40[D_80148EAC + 3].unk_0E == 8) {
            D_800F53A0.unk_F30[0x148 + D_80148EAC] = 0xC;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 3;
        } else {
            D_800F53A0.unk_F30[0x148 + D_80148EAC] = 9;
            D_80148E48[D_80148EAC - 1] = func_800BCE40() % 2 + 1;
        }
        D_80148BF0[D_80148EAC - 1] = 0x10000 / (D_80148E48[D_80148EAC - 1] + (D_80148E48[D_80148EAC - 1] >= 3) * 2 + 1);
    }
    if (D_8012E694 == 0) {
        count = D_80148EAC;
    } else {
        count = 0xA;
        for (i = 0xE; i < 0x36; i++) {
            D_8012CA40[i].unk_00 |= 0x80000000;
        }
    }
    for (i = 4; i < count + 4; i++) {
        D_8012CA40[i].unk_14 = D_8012CA40[i].unk_15 = D_8012CA40[i].unk_16 = D_801317EB;
        D_8012CA40[i].unk_00 &= 0x7FFFFFFF;
        if ((u32)D_80148EB0 % (u32)D_80148E48[i - 4] == 0) {
            D_8012CA40[i].unk_06 += 1;
        }
        ((Fix32 *)D_80148CB8)[i - 4].w += D_80148BF0[i - 4];
        D_80148BF0[i - 4] += (D_80148D80[i - 4] - ((Fix32 *)D_80148CB8)[i - 4].w) >> 8;
        D_8012CA40[i].unk_04 = ((Fix32 *)D_80148CB8)[i - 4].h[1];
        if (D_8012CA40[i].unk_06 >= 0x29) {
            D_8012CA40[i].unk_04 = func_800BCE40() % 320 - 0x9F;
            ((Fix32 *)D_80148CB8)[i - 4].h[0] = 0;
            ((Fix32 *)D_80148CB8)[i - 4].h[1] = D_8012CA40[i].unk_04;
            D_80148D80[i - 4] = ((Fix32 *)D_80148CB8)[i - 4].w;
            D_8012CA40[i].unk_06 = -0x78;
            D_8012CA40[i].unk_0E = (func_800BCE40() % 2) * 8;
            if (D_8012E694 == 1) {
                D_8012CA40[i].unk_0E = 0;
            }
            if (D_8012CA40[i].unk_0E == 8) {
                D_800F53A0.unk_F30[0x145 + i] = 0xC;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 3;
            } else {
                D_800F53A0.unk_F30[0x145 + i] = 9;
                D_80148E48[i - 4] = func_800BCE40() % 2 + 1;
            }
            D_80148BF0[i - 4] = 0x10000 / (D_80148E48[i - 4] + (D_80148E48[i - 4] >= 3) * 2 + 1);
        }
    }
    D_80148EB0 += 1;
}

void func_80145084(void) {
    s16 i;

    D_800D9250 = 0;
    D_800D9254 = 0;
    func_80089200();
    func_8013ED84();
    func_8008F618(D_800F62CF);
    func_80143BA0();
    func_8004EE18(D_80148BA4, 0x11, 1, 2, 0);
    func_8008FD1C(D_80148BA8, D_80148BAC, D_80148BB0, D_80148BB4, D_80148BB8, D_80148BBC);
    func_8008FF60(D_80148B98, D_80148B9C, D_80148B94, D_80148BA0);
    D_8012B8C0[0].unk_04 = D_8012B8C0[1].unk_04 = 8;
    func_8013ED84();
    D_80148628 = D_801482F8;
    D_8014862C = D_80148430;
    D_80148630 = D_80148568;
    func_800BCE10(&D_800D92A0, "伊集院邸");
    func_8004EE18(D_80148B74, 0x12, 1, 3, 0);
    for (i = 4; i < 0x36; i++) {
        D_8012CA40[i].unk_00 = 0xC1000000;
        D_8012CA40[i].unk_04 = func_800BCE40() % 320 - 0x9F;
        ((Fix32 *)D_80148CB8)[i - 4].h[0] = 0;
        ((Fix32 *)D_80148CB8)[i - 4].h[1] = D_8012CA40[i].unk_04;
        D_80148D80[i - 4] = ((Fix32 *)D_80148CB8)[i - 4].w;
        D_8012CA40[i].unk_06 = -0x78;
        D_8012CA40[i].unk_08 = 8;
        /* Rec24.unk_0A is u8[2] in game.h; original writes a 16-bit halfword sh. T-24098 */
        *(s16 *)&D_8012CA40[i].unk_0A = 8;
        D_8012CA40[i].unk_0C = 0xA;
        D_8012CA40[i].unk_0E = 0;
        D_8012CA40[i].unk_0F = 0;
        D_8012CA40[i].unk_10 = 0;
        D_8012CA40[i].unk_12 = 0x1F2;
        D_8012CA40[i].unk_14 = 0x80;
        D_8012CA40[i].unk_15 = 0x80;
        D_8012CA40[i].unk_16 = 0x80;
        D_8012CA40[i].unk_18 = 0;
        D_8012CA40[i].unk_1A = 0;
        D_8012CA40[i].unk_1C = 0x1000;
        D_8012CA40[i].unk_1E = 0x1000;
        D_8012CA40[i].unk_20 = 0;
        D_800F53A0.unk_F30[0x145 + i] = 9;
        D_80148E48[i - 4] = 2;
        D_80148BF0[i - 4] = 0x8000;
    }
    D_80148EAC = 0;
    D_80148EB0 = 0;
    func_8004DE1C();
}

void func_80145350(void) {
    func_8008A0D4(0x46B1);
    func_80090960(4, 3);
    func_8004DE1C();
}

/* Return type preserves the original fall-through schedule. T-20006. */
s32 func_80145384(void) {
    s32 idx; /* Index precedes the copied table (T-3330, T-20006). */
    GzCallbacksBC table;

    table = D_8014901C;
    idx = D_800F647A;
    table.callbacks[idx](0x3C);
    if (D_800F647A < 0x10) {
        D_8012B8C0[0].unk_07 = D_801317EB;
        if (D_8012B8C0[0].unk_18 == 0 && D_8012B8C0[0].unk_08 == 0) {
            if (++D_8012B8C0[0].unk_16 >= 3) {
                D_8012B8C0[0].unk_16 = 0;
            }
        }
    }
    if (D_8012E694 != 0) {
        D_8012B8C0[2].unk_03 |= 0x80;
        D_8012B8C0[2].unk_07 = D_8012B8C0[0].unk_07;
        if ((u32)D_800F6468 % 300U == 0) {
            func_80145520();
            D_8012B8C0[2].unk_02 = 3;
        }
    } else {
        D_8012B8C0[2].unk_03 &= 0xFF7F;
    }
}

void func_801454F8(void) {
    func_80086AB0(0x205);
    func_8004DE1C();
}

void func_80145520(void) {
    func_80086AB0(0x504);
}

void func_80145540(void) {
    func_80051DD8(0x45, 0x801B0000, 0x88B4);
    func_80143C70();
    func_8004DE1C();
}

void func_80145578(void) {
    D_8012B960 = 0;
    D_8012B950 = 0;
    D_8012B94A = 3;
    func_80086AB0(0x503);
    func_8004DE1C();
}

void func_801455BC(void) {
    D_800D9250 = 0;
    D_800D9254 = 0;
    func_8008F618(D_800F62CF);
    func_8013ED84();
    D_80148628 = D_80148358;
    D_8014862C = D_80148490;
    D_80148630 = D_801485C8;
    func_8004DE1C();
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 1;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 2;
    func_8008FB00();
    func_800BCE10(&D_800D92A0, "近所の公園");
}

void func_8014569C(void) {
    D_800F6600 = -1;
    D_800F6680 = -1;
    func_80143C70();
    func_8004EE18(D_80148BD0, 0x11, 1, 2, 0);
    func_8008FD1C(D_80148BD4, D_80148BD8, D_80148BDC, D_80148BE0, D_80148BE4, D_80148BE8);
    func_8008FF60(D_80148BC4, D_80148BC8, D_80148BC0, D_80148BCC);
    D_8012B8C0[1].unk_04 = 8;
    D_8012B8C0[0].unk_04 = 8;
    D_8012B8C0[2] = D_8012B8C0[0];
    D_8012B8C0[2].unk_16 = 4;
    D_8012B8C0[2].unk_18 = 0;
    D_8012B8C0[2].unk_08 = 0;
    D_8012B8C0[2].unk_02 = 0;
    func_8004DE1C();
}

void func_801457D8(void) {
    func_8008A0D4(0x4280);
    func_8004DE1C();
}

void func_80145800(void) {
    func_8008A0D4(0x4718);
    func_80090960(4, 4);
    func_8004DE1C();
}
