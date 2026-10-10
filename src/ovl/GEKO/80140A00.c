#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

void func_80140A00(void) {
    bg_read_sub2(0x4699);
    func_8004284C();
}

void func_80140A28(void) {
    bg_read_sub2(0x4084);
    func_8004284C();
}

void func_80140A50(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80140A78(void) {
    bg_read_sub2(0x4144);
    func_8004284C();
}

void func_80140AA0(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x4144);
    func_8004284C();
}

void func_80140AE0(void) {
    bg_read_sub2(0x415E);
    func_8004284C();
}

void func_80140B08(void) {
    bg_read_sub2(0x4743);
    func_8004284C();
}

void func_80140B30(void) {
    D_80147300 = 0x801B0000;
    D_80147304 = 0x801B2000;
    D_80147308 = 0x801B6000;
    D_8014730C = 0x801BA000;
    D_80147310 = 0x801BE000;
    D_80147314 = 0x801C2000;
    D_80147318 = 0x801C6000;
}

void func_80140BA0(void) {
    D_8014731C = 0x801CE150;
    D_80147320 = 0x801CE154;
    D_80147324 = 0x801CE1A4;
    D_80147328 = (*(s16 *)0x801CE1BC);
    D_8014732C = 0x801B0000;
    D_80147330 = 0x801B2000;
    D_80147334 = 0x801B6000;
    D_80147338 = 0x801BA000;
    D_8014733C = 0x801BE000;
    D_80147340 = 0x801C2000;
    D_80147344 = 0x801C6000;
}

void func_80140C50(void) {
    switch (D_80122CD0) {
    case 1:
        func_80140CCC();
        return;
    case 2:
        func_80140D08();
        return;
    case 4:
        func_80140D44();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80140CCC(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80140D80();
        return;
    }
    func_80046500();
}

void func_80140D08(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80140E40();
        return;
    }
    func_80046500();
}

void func_80140D44(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80141760();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[27])();
} FnTbl27; /* size 0x6C */
extern FnTbl27 D_80147348;

void func_80140D80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl27 tbl;

    tbl = D_80147348;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_80140DF4(void) {
    func_8004E788(-0x80, 0x30, 1, "ドン", 0);
    func_80044750(0x24);
    func_80044750(0x500);
    func_8004284C();
}

typedef struct {
    void (*f[28])();
} FnTbl28; /* size 0x70 */
extern FnTbl28 D_801473B4;

void func_80140E40(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl28 tbl;

    tbl = D_801473B4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80140EBC(void) {
    func_80138AF8();
    if ((u16) D_800CA154 == 3) {
        if (D_800E6280.unk_1104.w++ == 1) {
            func_80044750(0x501);
        }
    }
}

void func_80140F14(void) {
    func_80046318(0x35, 0x801B0000, 0x8F43);
    func_8004284C();
}

void func_80140F44(void) {
    load_palette(D_80147300, 0x11, 1, 2, 0);
    func_80084E90(D_80147304, D_80147308, D_8014730C, D_80147310, D_80147314, D_80147318);
    func_800850D4(0, 0, 0, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_80140FD0);

void func_8014107C(void) {
    func_80065F34(0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_801410A4);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_801411DC);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_80141374);

void func_80141514(void) {
    func_80140BA0();
    func_80043914(D_8014732C, 0x11, 1, 2, 0);
    func_80084E90(D_80147330, D_80147334, D_80147338, D_8014733C, D_80147340, D_80147344);
    func_800850D4(D_80147320, D_80147324, D_8014731C, D_80147328);
    D_80120650[4] = D_80120650[0x48] = 8;
    D_800CA368 = 0x4D;
    func_8004284C();
}

void func_801415D8(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_80141600(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80141628(void) {
    bg_read_sub2(0x4708);
    func_8004284C();
}

void func_80141650(void) {
    D_80122D38 = 0;
    parameter_disp_switch(0);
    icon_disp_switch(0);
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_80141690(void) {
    bg_read_sub2(0x430F);
    func_8004284C();
}

void func_801416B8(void) {
    bg_read_sub2(0x47F7);
    func_8004284C();
}

void func_801416E0(void) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FB6, 0);
    if (temp_v0 == *D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (temp_v0 == (1 - *D_800B5938)) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_80141760);

void func_8014181C(void) {
    if (D_800E6280.unk_110D == 0) {
        k_reset(0);
        func_8006612C(D_800CA1DC);
        D_800E6280.unk_110D += 1;
    }
    normal_date_girl_in();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80140A00", func_80141870);

void func_8014197C(void) {
    func_80044750(0x24);
    func_80044750(0x500);
    func_8004284C();
}

void func_801419AC(void) {
    func_80046318(0x3D, 0x801B0000, 0x8F78);
    func_80140BA0();
    func_8004284C();
}

void func_801419E4(void) {
    func_80044890(1, 0xC043, 0xC01D, 0xCEFD, 0xCEB4, 0xCEB1);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80141A44(void) {
    func_80138AF8();
    D_800CA150 = (u16) D_800CA150 + 1;
    D_800CA154 = 0;
    func_8004284C();
}

void func_80141A84(void) {
    k_reset(1);
    D_80122CE4 = 0;
    D_80122CF0 = 0;
    D_800CA2AC = 0;
    func_8004284C();
}

void func_80141AC0(void) {
    func_80062CD0(0x6C05);
    func_8004284C();
}
