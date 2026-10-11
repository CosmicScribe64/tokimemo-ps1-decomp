#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32: selector in $v1 (main_api.h: s32) */
#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    void (*f[61])();
} FnTbl61; /* size 0xF4 */
extern FnTbl61 D_8015FE58;

extern u32 D_80122CD0;

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_8015FF4C;

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 D_8015FFF0;

void func_80156600(void) {
    D_8015FCE0 = 0x801CE0DC;
    D_8015FCE4 = 0x801CE0E0;
    D_8015FCE8 = 0x801CE0F8;
    D_8015FCEC = *(s16 *)0x801CE10C;
    D_8015FCF0 = 0x801B0000;
    D_8015FCF4 = 0x801B2000;
    D_8015FCF8 = 0x801B6000;
    D_8015FCFC = 0x801BA000;
    D_8015FD00 = 0x801BE000;
    D_8015FD04 = 0x801C2000;
    D_8015FD08 = 0x801C6000;
}

void func_801566B0(void) {
    D_8015FD0C = 0x801D2100;
    D_8015FD10 = 0x801D2108;
    D_8015FD14 = 0x801D2128;
    D_8015FD18 = *(s16 *)0x801D2144;
    D_8015FD1C = 0x801B0000;
    D_8015FD20 = 0x801B2000;
    D_8015FD24 = 0x801B6000;
    D_8015FD28 = 0x801BA000;
    D_8015FD2C = 0x801BE000;
    D_8015FD30 = 0x801C2000;
    D_8015FD34 = 0x801C6000;
}

void func_80156760(void) {
    D_8015FD38 = 0x801D2240;
    D_8015FD3C = 0x801D2248;
    D_8015FD40 = 0x801D2288;
    D_8015FD44 = *(s16 *)0x801D22A0;
    D_8015FD48 = 0x801B0000;
    D_8015FD4C = 0x801B2000;
    D_8015FD50 = 0x801B6000;
    D_8015FD54 = 0x801BA000;
    D_8015FD58 = 0x801BE000;
    D_8015FD5C = 0x801C2000;
    D_8015FD60 = 0x801C6000;
}

void func_80156810(void) {
    D_8015FD64 = 0x801CE080;
    D_8015FD68 = 0x801CE084;
    D_8015FD6C = 0x801CE094;
    D_8015FD70 = *(s16 *)0x801CE0A8;
    D_8015FD74 = 0x801B0000;
    D_8015FD78 = 0x801B2000;
    D_8015FD7C = 0x801B6000;
    D_8015FD80 = 0x801BA000;
    D_8015FD84 = 0x801BE000;
    D_8015FD88 = 0x801C2000;
    D_8015FD8C = 0x801C6000;
}

void func_801568C0(void) {
    switch (D_80122CD0) {
    case 2:
        func_8015703C();
        return;
    case 3:
        func_801571D4();
        return;
    case 4:
        func_8015750C();
        return;
    case 5:
        func_8015761C();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80156954(void) {
    D_8015E208 = D_8015DF18;
    D_8015E20C = D_8015E054;
    D_8015E210 = D_8015E190;
    func_80156600();
    func_80043914(D_8015FCF0, 0x11, 1, 2, 0);
    func_80084E90(D_8015FCF4, D_8015FCF8, D_8015FCFC, D_8015FD00, D_8015FD04, D_8015FD08);
    func_800850D4(D_8015FCE4, D_8015FCE8, D_8015FCE0, (s32) D_8015FCEC);
    D_800E6280.unk_1BC[6].unk_02 += 1;
    D_800E6280.unk_1BC[6].unk_06 += 1;
    D_800E6280.unk_1BC[6].unk_0A -= 0x14;
    func_80084D3C();
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_14[0xD] += 1;
    D_800CA21C[0] = 2;
    D_800CA21C[2] = 2;
    D_800CA21C[1] = 2;
    D_800CA224[0] = 2;
    D_800CA224[2] = 2;
    D_800CA224[1] = 2;
    D_800CA22C[0] = 2;
    D_800CA22C[2] = 2;
    D_800CA22C[1] = 2;
    D_800CA234[0] = 1;
    D_800CA234[2] = 1;
    D_800CA234[1] = 1;
    func_8004284C();
}

void func_80156B20(void) {
    D_8015E208 = D_8015DF1C;
    D_8015E20C = D_8015E058;
    D_8015E210 = D_8015E194;
    func_801566B0();
    load_palette(D_8015FD1C, 0x11, 1, 2, 0);
    func_80084E90(D_8015FD20, D_8015FD24, D_8015FD28, D_8015FD2C, D_8015FD30, D_8015FD34);
    func_800850D4(D_8015FD10, D_8015FD14, D_8015FD0C, (s32) D_8015FD18);
    D_800CA360 = 1;
    D_800E6280.unk_1BC[6].unk_02 += 3;
    D_800E6280.unk_1BC[6].unk_06 += 2;
    D_800E6280.unk_1BC[6].unk_0A -= 0x14;
    check_para_limit();
    func_8004284C();
}

void func_80156C40(void) {
    D_8015E208 = D_8015DF20;
    D_8015E20C = D_8015E05C;
    D_8015E210 = D_8015E198;
    func_80156760();
    func_80043914(D_8015FD48, 0x11, 1, 2, 0);
    func_80084E90(D_8015FD4C, D_8015FD50, D_8015FD54, D_8015FD58, D_8015FD5C, D_8015FD60);
    func_800850D4(D_8015FD3C, D_8015FD40, D_8015FD38, (s32) D_8015FD44);
    D_800CA360 = 1;
    D_800E6280.unk_1BC[6].unk_02 += 2;
    D_800E6280.unk_1BC[6].unk_06 += 2;
    D_800E6280.unk_1BC[6].unk_0A -= 0xA;
    func_80084D3C();
    func_8004284C();
}

void func_80156D60(void) {
    D_8015E208 = D_8015DF24;
    D_8015E20C = D_8015E060;
    D_8015E210 = D_8015E19C;
    if ((u8)(func_80051A68(6) & 0x7F) < 2) {
        D_800E6280.unk_1BC[6].unk_02 += 3;
        D_800E6280.unk_1BC[6].unk_06 += 2;
    } else {
        D_800E6280.unk_1BC[6].unk_02 += 1;
        D_800E6280.unk_1BC[6].unk_06 += 3;
    }
    D_800E6280.unk_1BC[6].unk_0A -= 0x14;
    func_80084D3C();
    func_80156810();
    load_palette(D_8015FD74, 0x11, 1, 2, 0);
    func_80084E90(D_8015FD78, D_8015FD7C, D_8015FD80, D_8015FD84, D_8015FD88, D_8015FD8C);
    func_800850D4(D_8015FD68, D_8015FD6C, D_8015FD64, D_8015FD70);
    D_800CA360 = 1;
    func_8004284C();
}

void func_80156EC0(void) {
    bg_read_sub2(0x4860);
    func_8004284C();
}

void func_80156EE8(void) {
    bg_read_sub2(0x44F1);
    func_8004284C();
}

void func_80156F10(void) {
    func_800AE0F0(D_800CA1DC, "無人島");
    func_8007ED84(0x45B6);
    func_8004284C();
}

void func_80156F4C(void) {
    bg_read_sub2(0x48CD);
    func_8004284C();
}

void func_80156F74(void) {
    bg_read_sub2(0x45A3);
    func_8004284C();
}

void func_80156F9C(void) {
    bg_read_sub2(0x48F7);
    func_8004284C();
}

void func_80156FC4(void) {
    bg_read_sub2(0x45D3);
    func_8004284C();
}

void func_80156FEC(void) {
    bg_read_sub2(0x492F);
    func_8004284C();
}

void func_80157014(void) {
    bg_read_sub2(0x4631);
    func_8004284C();
}

typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
extern FnTbl50 D_8015FD90;

void func_8015703C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_8015FD90;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_801570C4(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801570EC(void) {
    func_80046318(0x3D, 0x801B0000, 0x8984);
    func_80156600();
    func_8004284C();
}

void func_80157124(void) {
    if (get_g_zyotai_s(D_800E6280.unk_F5F) < 2U) {
        func_80083440(0);
    } else {
        func_80083440(get_g_zyotai_h(D_800E6280.unk_F5F));
    }
    func_8004284C();
}

void func_80157184(void) {
    u8 s;

    s = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 5;
    }
    func_8004284C();
}

void func_801571D4(void) {
    s32 idx; /* FAKE: never read; declared first so tbl lands at the original frame offset (T-3330) */
    FnTbl61 tbl;

    tbl = D_8015FE58;
    if (D_800E6280.unk_110A < 4 || D_800E6280.unk_110A >= 0x2C) {
        if (D_800B593C == 0x80) {
            func_8006B900();
        }
    }
    tbl.f[D_800E6280.unk_110A](0x80);
}

void func_80157284(void) {
    func_80046318(0x45, 0x801B0000, 0x89C1);
    func_801566B0();
    func_8004284C();
}

void func_801572BC(void) {
    func_80044750(0x500);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_801572E4);

s32 func_801573F0(void) {
    D_800E6280.unk_036 += 0x10;
    D_800E6280.unk_037 += 0x10;
    D_800E6280.unk_038 += 0x10;
    if (D_800E6280.unk_036 >= 0xF0U) {
        func_8004284C();
    }
}

void func_8015745C(void) {
    s16 i;

    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    /* FAKE: the last store and the loop on one source line; as1 then places the store after the
     * loop setup as the original does (line-based scheduling, T-6070). Not a separate symbol. T-7010 */
    D_800E6280.unk_038 = 0; for (i = 0; i < 6; i++) {
        D_801217D0[i + 4].unk_00 &= 0x7FFFFFFF;
    }
    D_80120650[3] |= 0x80;
    D_80120650[0x47] |= 0x80;
    D_80122CF8 = 1;
    func_8004284C();
}

void func_8015750C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8015FF4C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80157594(void) {
    func_80046318(0x45, 0x801B0000, 0x8A06);
    func_80156760();
    func_8004284C();
}

void func_801575CC(void) {
    u8 s;

    s = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 3;
    }
    func_8004284C();
}

void func_8015761C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = D_8015FFF0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/80156600", func_80157698);

void func_801577C4(void) {
    func_80046318(0x3D, 0x801B0000, 0x8A4B);
    func_80156810();
    func_8004284C();
}

void func_801577FC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_80157824(void) {
    u8 s;

    s = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    if (s >= 2U) {
        D_800CA150 = (u16)D_800CA150 + 3;
    }
    func_8004284C();
}
