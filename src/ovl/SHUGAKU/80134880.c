#include "common.h"
#include "ovl/SHUGAKU.h"

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_8013C1C4;

void func_80134880(void) {
    D_8013C190 = (u8 *)0x801EA87C;
    D_8013C194 = (u8 *)0x801EA89C;
    D_8013C198 = (u8 *)0x801EA940;
    D_8013C19C = *(s16 *)0x801EA968;
    D_8013C1A0 = (u8 *)0x801B0000;
    D_8013C1A4 = (u8 *)0x801B2000;
    D_8013C1A8 = (u8 *)0x801B6000;
    D_8013C1AC = (u8 *)0x801BA000;
    D_8013C1B0 = (u8 *)0x801BE000;
    D_8013C1B4 = (u8 *)0x801C2000;
    D_8013C1B8 = (u8 *)0x801C6000;
}

void func_80134930(void) {
    if (D_800E6280.unk_721 == 3) {
        func_80134970();
        return;
    }
    func_80134B94();
}

void func_80134970(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_8013C1C4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801349EC(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_8004284C();
}

void func_80134A20(void) {
    if (D_8013C1BC != 0) {
        normal_date_move_place();
        return;
    }
    if (D_800E6280.unk_110D == 0) {
        dec_bg_show_switch(1);
        D_800E6280.unk_110D += 1;
    }
    func_8007C8A4();
}

void func_80134A8C(void) {
    func_800AE0F0(D_800CA19C, "ホテル・廊下");
    func_801365F4();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    if (D_800B593C != 0) {
        D_8013C1BC = 1;
    } else {
        D_8013C1BC = 0;
    }
    func_8004284C();
}

void func_80134B2C(void) {
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}

void func_80134B64(void) {
    func_80042908((D_800E6280.unk_721 + 1) & 0xFF);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134B94);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134C98);

s32 func_80134D8C(void) {
    func_801343B0();
    if ((D_800E6280.unk_1104.w++ == 1) && (D_800CA2E8 == 2)) {
        D_80122D0C = 0;
        func_80067DD4();
    }
}

s32 func_80134DE8(void) {
    if (D_800E6280.unk_110D == 0) {
        func_8004E9F4(0);
        func_8006612C("ホテル・廊下");
        D_800E6280.unk_110D += 1;
    }
    func_801343B0();
    if (D_800CA2E8 == 1) {
        if (D_800E6280.unk_1104.u++ == 1) {
            func_80044750(0x503);
        }
    }
}

void func_80134E74(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80134E9C(void) {
    func_80046318(0x76, 0x801B0000, 0x8D47);
    func_80134880();
    func_8004284C();
}

void func_80134ED4(void) {
    D_800E6280.unk_75D = D_800E6280.unk_F5F;
    D_800E6280.unk_F5F = 0xD;
    func_800847B8(0xD);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_80134F18);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_801350F4);

void func_801351CC(void) {
    func_80137C3C();
    if ((D_800E6280.unk_1BC[0].unk_10.w & 0xF) == D_800E6280.unk_03F && ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B) == D_800E6280.unk_040) {
        D_800E6280.unk_721 += 1;
        D_800E6280.unk_722 = 0;
        D_800E6280.unk_75D = D_800E6280.unk_F5F;
        D_800E6280.unk_F5F = 0;
        func_80042908(6);
        return;
    }
    func_80042908((D_800E6280.unk_721 + 1) & 0xFF);
}

void func_80135278(void) {
    D_800E6280.unk_03C = D_8013C1C0;
    D_800CA2DC = 0x29;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80134880", func_801352B0);
