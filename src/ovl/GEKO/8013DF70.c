#define MAIN_API_OVERRIDE_D_80122CD0 /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_80146F34;

typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */
extern FnTbl46 D_80146E7C;

void func_8013DF70(void) {
    D_80146E50 = 0x801D22F4;
    D_80146E54 = 0x801D22F8;
    D_80146E58 = 0x801D2318;
    D_80146E5C = *(s16 *)0x801D232C;
    D_80146E60 = 0x801B0000;
    D_80146394 = 0x801D2000;
    D_80146E64 = 0x801B2000;
    D_80146E68 = 0x801B6000;
    D_80146E6C = 0x801BA000;
    D_80146E70 = 0x801BE000;
    D_80146E74 = 0x801C2000;
    D_80146E78 = 0x801C6000;
    D_801463B0 = 0x801CE000;
}

void func_8013E040(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013E0A4();
        return;
    case 2:
        func_8013E0E0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013E0A4(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013E11C();
        return;
    }
    func_80046500();
}

void func_8013E0E0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013E4F0();
        return;
    }
    func_80046500();
}

void func_8013E11C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl46 tbl;

    tbl = D_80146E7C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_8013E950();
}

void func_8013E1A0(void) {
    if (func_80052C88(9) >= 0x50U) {
        D_800E6280.unk_1BC[9].unk_02 += 1;
        D_800E6280.unk_1BC[9].unk_06 += 2;
    } else {
        D_800E6280.unk_1BC[9].unk_0C.f.b14 = 1;
        D_800E6280.unk_1BC[9].unk_06 += 1;
    }
    func_80084D3C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E22C);

void func_8013E410(void) {
    func_80046318(0x45, 0x801B0000, 0x8A88);
    func_8013DF70();
    func_8004284C();
}

void func_8013E448(void) {
    func_8013DF70();
    func_80043914(D_80146E60, 0x11, 1, 2, 0);
    func_80084E90(D_80146E64, D_80146E68, D_80146E6C, D_80146E70, D_80146E74, D_80146E78);
    func_800850D4(D_80146E54, D_80146E58, D_80146E50, D_80146E5C);
    func_8004284C();
}

void func_8013E4F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_80146F34;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E56C);

void func_8013E5C8(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800CA150 = (u16) D_800CA150 + 2;
    }
    func_8004284C();
}

void func_8013E60C(void) {
    if (get_h_yuukou(9) < 0x50U) {
        D_800E6280.unk_110A += 0xE;
    }
    func_8004284C();
}

void func_8013E650(void) {
    if (get_h_yuukou(9) >= 0x50U) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E69C);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E760);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E828);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013E950);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013DF70", func_8013ECA8);

void func_8013EDC4(void) {
    bg_read_sub2(0x4068);
    func_8004284C();
}

void func_8013EDEC(void) {
    bg_read_sub2(0x46B2);
    func_8004284C();
}

void func_8013EE14(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}
