#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[35])();
} FnTbl35; /* size 0x8C */
extern FnTbl35 D_80146FBC;

void func_8013EE40(void) {
    D_80146F90 = 0x801CE124;
    D_80146F94 = 0x801CE128;
    D_80146F98 = 0x801CE148;
    D_80146F9C = (*(s16 *)0x801CE15C);
    D_80146FA0 = 0x801B0000;
    D_80146FA4 = 0x801B2000;
    D_80146FA8 = 0x801B6000;
    D_80146FAC = 0x801BA000;
    D_80146FB0 = 0x801BE000;
    D_80146FB4 = 0x801C2000;
    D_80146FB8 = 0x801C6000;
}

void func_8013EEF0(void) {
    if (D_80122CD0 == 1) {
        func_8013EF30();
        return;
    }
    func_80046500();
}

void func_8013EF30(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013EF6C();
        return;
    }
    func_80046500();
}

void func_8013EF6C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl35 tbl;

    tbl = D_80146FBC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_8013EFF4(void) {
    func_80046318(0x3D, 0x801B0000, 0x8C0E);
    func_8013EE40();
    func_8004284C();
}

void func_8013F02C(void) {
    if (D_800E6280.unk_56C[28] == 1 && D_800E6280.unk_56C[27] == 1) {
        D_800CA150 = (u16) D_800CA150 + 4;
        D_800CA154 = 0;
    } else if (D_800E6280.unk_56C[27] >= 2U) {
        D_800CA150 = (u16) D_800CA150 + 2;
        D_800CA154 = 0;
    }
    func_8004284C();
}

void func_8013F0BC(void) {
    D_800E6280.unk_F5F = 4;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
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
    D_80146274 = D_80145FB4;
    D_80146278 = D_801460F0;
    D_8014627C = D_8014622C;
    func_800AE0F0(D_800CA19C, "廊下");
    if (D_800E6280.unk_56C[27] >= 3U) {
        D_800E6280.unk_56C[27] = 2;
    }
    if (D_800E6280.unk_56C[28] == 0 && D_800E6280.unk_56C[27] == 1) {
        D_800E6280.unk_1BC[4].unk_02 += 1;
    }
    D_800E6280.unk_1BC[4].unk_06 += 1;
    D_800E6280.unk_0FC.unk_02 += 0xA;
    func_80084D3C();
    D_800CA368 = 0x1B;
    func_8004284C();
}

void func_8013F25C(void) {
    func_8013EE40();
    load_palette(D_80146FA0, 0x11, 1, 2, 0);
    func_80084E90(D_80146FA4, D_80146FA8, D_80146FAC, D_80146FB0, D_80146FB4, D_80146FB8);
    func_800850D4(D_80146F94, D_80146F98, D_80146F90, D_80146F9C);
    D_800CA360 = 1;
    func_8004284C();
}

void func_8013F30C(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_8013F334(void) {
    func_8007ED84(0x46E0);
    func_8004284C();
}
