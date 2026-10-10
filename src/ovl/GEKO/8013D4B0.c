#define MAIN_API_OVERRIDE_D_80122CD0 /* matched as u32 (main_api.h: s32), switch selector in $v1 (T-6010) */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_80146CBC;

void func_8013D4B0(void) {
    bg_read_sub2(0x46E7);
    func_8004284C();
}

void func_8013D4D8(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_8013D500(void) {
    D_80146C90 = 0x801CE0F4;
    D_80146C94 = 0x801CE0F8;
    D_80146C98 = 0x801CE118;
    D_80146C9C = (*(s16 *)0x801CE12C);
    D_80146CA0 = 0x801B0000;
    D_80146CA4 = 0x801B2000;
    D_80146CA8 = 0x801B6000;
    D_80146CAC = 0x801BA000;
    D_80146CB0 = 0x801BE000;
    D_80146CB4 = 0x801C2000;
    D_80146CB8 = 0x801C6000;
}

void func_8013D5B0(void) {
    switch (D_80122CD0) {
    case 1:
        func_8013D614();
        return;
    case 2:
        func_8013D650();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_8013D614(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013D68C();
        return;
    }
    func_80046500();
}

void func_8013D650(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013D74C();
        return;
    }
    func_80046500();
}

void func_8013D68C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_80146CBC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013D714(void) {
    func_80046318(0x3D, 0x801B0000, 0x87D9);
    func_8013D500();
    func_8004284C();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80146D3C;

void func_8013D74C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80146D3C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013D7D4(void) {
    D_800E6280.unk_F5F = 5;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_80137990();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F68;
    D_80146278 = D_801460A4;
    D_8014627C = D_801461E0;
    func_800AE0F0(D_800CA19C, "廊下");
    if (D_80122CD0 == 1) {
        D_800E6280.unk_1BC[5].unk_06 += 1;
        D_800E6280.unk_1BC[5].unk_0A -= 1;
        func_80084D3C();
    }
    D_800CA368 = 0x21;
    func_8004284C();
}

void func_8013D91C(void) {
    func_8013D500();
    load_palette(D_80146CA0, 0x11, 1, 2, 0);
    func_80084E90(D_80146CA4, D_80146CA8, D_80146CAC, D_80146CB0, D_80146CB4, D_80146CB8);
    func_800850D4(D_80146C94, D_80146C98, D_80146C90, D_80146C9C);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/8013D4B0", func_8013D9C4);

void func_8013DB04(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013DB2C(void) {
    bg_read_sub2(0x46CD);
    func_8004284C();
}

void func_8013DB54(void) {
    func_8007ED84(0x4045);
    func_8004284C();
}
