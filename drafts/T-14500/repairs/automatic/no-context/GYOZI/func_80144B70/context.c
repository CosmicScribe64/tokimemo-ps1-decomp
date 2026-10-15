#include "common.h"
#include "ovl/GYOZI.h"

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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80144018);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_801444CC);

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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_8014494C);

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

/* bulk_m2c declarations (T-11190) */
extern s16 D_8012CAD4;
extern s16 D_80148E48;
extern s16 D_80148EAC;
extern s32 D_800F6415;
extern s32 D_800F6418;
extern s32 D_8012CA40;
extern s32 D_8012E694;
extern s32 D_80148BEC;
extern s32 D_80148BF0;
extern s32 D_80148CB8;
extern s32 D_80148D80;
extern u32 D_80148EB0;
extern u8 D_801317EB;
s32 func_800BCE40();
/* end bulk_m2c declarations */
void func_80144B70(void) {
    s16 *temp_s4;
    s16 *var_v1;
    s16 temp_a0;
    s16 temp_s2;
    s16 temp_v0;
    s16 temp_v0_4;
    s16 var_a0;
    s16 var_s2;
    s16 var_s2_2;
    s16 var_v1_2;
    s32 *temp_s0;
    s32 temp_at;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s6;
    s32 temp_t7;
    s32 temp_v0_3;
    s32 temp_v1;
    s8 temp_t8;
    s8 temp_v0_2;
    u8 *temp_s0_2;
    u8 *temp_s1;
    u8 *temp_s3;
    u8 *temp_s5;

    var_a0 = D_80148EAC;
    if (var_a0 == 0) {
        return;
    }
    if (D_80148EB0 == 0x1E) {
        D_8012CAD4 = 0xB4;
        var_a0 += 1;
        D_80148E48 = 1;
    } else if (D_80148EB0 == 0x5A) {
        var_a0 += 1;
        D_8012CAD4 = 0x28;
    } else if (((u32) D_80148EB0 >= 0xC9U) && (((u32) D_80148EB0 % 20U) == 0) && (var_a0 < 0x32)) {
        D_80148EAC = var_a0 + 1;
        temp_a0 = D_80148EAC;
        temp_t8 = (func_800BCE40(var_a0, D_80148EB0) % 2) * 8;
        *(s8 *)((unsigned char *)(&D_8012CA40 + (temp_a0 * 0x24)) + (122)) = temp_t8;
        if ((temp_t8 & 0xFF) == 8) {
            *(&D_800F6418 + temp_a0) = 0xC;
            D_80148EAC = temp_a0;
            var_a0 = D_80148EAC;
            var_v1 = &(&D_80148E48)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40(temp_a0) % 2) + 3);
        } else {
            *(&D_800F6418 + temp_a0) = 9;
            D_80148EAC = temp_a0;
            var_a0 = D_80148EAC;
            var_v1 = &(&D_80148E48)[var_a0];
            *(s16 *)((unsigned char *)var_v1 + (-2)) = (s16) ((func_800BCE40(temp_a0) % 2) + 1);
        }
        temp_v0 = *(s16 *)((unsigned char *)var_v1 + (-2));
        *(&D_80148BEC + (var_a0 * 4)) = 0x10000 / (s32) (((temp_v0 >= 3) * 2) + temp_v0 + 1);
    }
    if (D_8012E694 == 0) {
        D_80148EAC = var_a0;
        var_v1_2 = var_a0;
    } else {
        D_80148EAC = var_a0;
        var_v1_2 = 0xA;
        var_s2 = 0xE;
        do {
            temp_lo = var_s2 * 0x24;
            temp_s2 = var_s2 + 1;
            temp_at = temp_s2 < 0x36;
            var_s2 = temp_s2;
            temp_s0 = &D_8012CA40 + temp_lo;
            *temp_s0 |= 0x80000000;
        } while (temp_at != 0);
    }
    temp_s6 = var_v1_2 + 4;
    var_s2_2 = 4;
    if (temp_s6 >= 5) {
        do {
            temp_s4 = &(&D_80148E48)[var_s2_2];
            temp_v0_2 = D_801317EB & 0xFF;
            temp_s0_2 = &D_8012CA40 + (var_s2_2 * 0x24);
            *(s8 *)((unsigned char *)temp_s0_2 + (21)) = temp_v0_2;
            *(s8 *)((unsigned char *)temp_s0_2 + (20)) = temp_v0_2;
            *(u8 *)((unsigned char *)temp_s0_2 + (22)) = (u8) D_801317EB;
            temp_v0_3 = var_s2_2 * 4;
            (*(s32 *)temp_s0_2) = (s32) ((*(s32 *)temp_s0_2) & 0x7FFFFFFF);
            temp_s5 = temp_v0_3 + &D_80148BF0;
            temp_s1 = temp_v0_3 + &D_80148CB8;
            temp_s3 = temp_v0_3 + &D_80148D80;
            if ((D_80148EB0 % (u32) *(s16 *)((unsigned char *)temp_s4 + (-8))) == 0) {
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = (s16) (*(s16 *)((unsigned char *)temp_s0_2 + (6)) + 1);
            }
            temp_v1 = *(s32 *)((unsigned char *)temp_s5 + (-16));
            temp_t7 = *(s32 *)((unsigned char *)temp_s1 + (-16)) + temp_v1;
            *(s32 *)((unsigned char *)temp_s1 + (-16)) = temp_t7;
            *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (temp_v1 + ((s32) (*(s32 *)((unsigned char *)temp_s3 + (-16)) - temp_t7) >> 8));
            *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) *(s16 *)((unsigned char *)temp_s1 + (-14));
            if (*(s16 *)((unsigned char *)temp_s0_2 + (6)) >= 0x29) {
                temp_hi = func_800BCE40() % 320;
                *(s32 *)((unsigned char *)temp_s1 + (-16)) = 0;
                *(s16 *)((unsigned char *)temp_s0_2 + (6)) = -0x78;
                *(s16 *)((unsigned char *)temp_s0_2 + (4)) = (s16) (temp_hi - 0x9F);
                *(s16 *)((unsigned char *)temp_s1 + (-14)) = (s16) *(s16 *)((unsigned char *)temp_s0_2 + (4));
                *(s32 *)((unsigned char *)temp_s3 + (-16)) = (s32) *(s32 *)((unsigned char *)temp_s1 + (-16));
                *(u8 *)((unsigned char *)temp_s0_2 + (14)) = (u8) ((func_800BCE40() % 2) * 8);
                if (D_8012E694 == 1) {
                    *(u8 *)((unsigned char *)temp_s0_2 + (14)) = 0U;
                }
                if (*(u8 *)((unsigned char *)temp_s0_2 + (14)) == 8) {
                    *(&D_800F6415 + var_s2_2) = 0xC;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800BCE40() % 2) + 3);
                } else {
                    *(&D_800F6415 + var_s2_2) = 9;
                    *(s16 *)((unsigned char *)temp_s4 + (-8)) = (s16) ((func_800BCE40() % 2) + 1);
                }
                temp_v0_4 = *(s16 *)((unsigned char *)temp_s4 + (-8));
                *(s32 *)((unsigned char *)temp_s5 + (-16)) = (s32) (0x10000 / (s32) (((temp_v0_4 >= 3) * 2) + temp_v0_4 + 1));
            }
            var_s2_2 += 1;
        } while (var_s2_2 < temp_s6);
    }
    D_80148EB0 += 1;
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80145084);

void func_80145350(void) {
    func_8008A0D4(0x46B1);
    func_80090960(4, 3);
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80145384);

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
