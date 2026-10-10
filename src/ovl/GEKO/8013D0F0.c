#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146BEC;

void func_8013D0F0(void) {
    D_80146BC0 = 0x801CE0F4;
    D_80146BC4 = 0x801CE0F8;
    D_80146BC8 = 0x801CE118;
    D_80146BCC = *(s16 *)0x801CE12C;
    D_80146BD0 = 0x801B0000;
    D_80146BD4 = 0x801B2000;
    D_80146BD8 = 0x801B6000;
    D_80146BDC = 0x801BA000;
    D_80146BE0 = 0x801BE000;
    D_80146BE4 = 0x801C2000;
    D_80146BE8 = 0x801C6000;
}

void func_8013D1A0(void) {
    if (D_80122CD0 == 1) {
        func_8013D1E0();
        return;
    }
    func_80046500();
}

void func_8013D1E0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013D21C();
        return;
    }
    func_80046500();
}

void func_8013D21C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146BEC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013D290(void) {
    func_80046318(0x3D, 0x801B0000, 0x8627);
    func_8013D0F0();
    func_8004284C();
}

void func_8013D2C8(void) {
    D_800E6280.unk_F5F = 1;
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
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_800847B8(1);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145F54;
    D_80146278 = D_80146090;
    D_8014627C = D_801461CC;
    func_800AE0F0(D_800CA19C, "廊下");
    D_800E6280.unk_1BC[1].unk_02 += 1;
    D_800E6280.unk_1BC[1].unk_06 += 2;
    func_80084D3C();
    D_800CA368 = 8;
    func_8004284C();
}

void func_8013D408(void) {
    func_8013D0F0();
    func_80043914(D_80146BD0, 0x11, 1, 2, 0);
    func_80084E90(D_80146BD4, D_80146BD8, D_80146BDC, D_80146BE0, D_80146BE4, D_80146BE8);
    func_800850D4(D_80146BC4, D_80146BC8, D_80146BC0, (s32) D_80146BCC);
    func_8004284C();
}
