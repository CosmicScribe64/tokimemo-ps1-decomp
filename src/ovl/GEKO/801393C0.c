#include "common.h"
#include "ovl/GEKO.h"

void func_801393C0(void) {
    D_80146280 = (u8 *)0x801CE0FC;
    D_80146284 = (u8 *)0x801CE100;
    D_80146288 = (u8 *)0x801CE120;
    D_8014628C = *(s16 *)0x801CE13C;
    D_80146290 = (u8 *)0x801B0000;
    D_80146294 = (u8 *)0x801B2000;
    D_80146298 = (u8 *)0x801B6000;
    D_8014629C = (u8 *)0x801BA000;
    D_801462A0 = (u8 *)0x801BE000;
    D_801462A4 = (u8 *)0x801C2000;
    D_801462A8 = (u8 *)0x801C6000;
}

void func_80139470(void) {
    if (D_80122CD0 == 1) {
        func_801394B0();
        return;
    }
    func_80046500();
}

void func_801394B0(void) {
    if (D_800E7389 == 0) {
        func_801394EC();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[53])();
} FnTbl53; /* size 0xD4 */
extern FnTbl53 D_801462AC;

void func_801394EC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl53 tbl;

    tbl = D_801462AC;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    D_80120652 = 1;
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801393C0", func_8013957C);

void func_801395C0(void) {
    func_80046318(0x3D, 0x801B0000, 0x7D96);
    func_801393C0();
    func_8004284C();
}

void func_801395F8(void) {
    func_800634FC(get_g_zyotai_h(8));
    func_80044750(0x501);
    func_800847B8(D_800E71DF);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801393C0", func_80139640);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801393C0", func_801397A8);

void func_80139864(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013988C(void) {
    bg_read_sub2(0x408D);
    func_8004284C();
}

void func_801398B4(void) {
    bg_read_sub2(0x46D6);
    func_8004284C();
}

void func_801398DC(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}
