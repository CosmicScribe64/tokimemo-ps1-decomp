#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80143AD0);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80143BA0);

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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80144A74);

void func_80144B28(void) {
    func_8004F860(0x280, 0, 0x40, 0x80, D_80148B90);
    D_800F65C0 = 1;
    func_8004DE1C();
}

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_80144B70);

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

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80143AD0", func_801455BC);

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
