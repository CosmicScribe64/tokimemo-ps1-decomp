#include "common.h"
#include "ovl/GEKO.h"

void func_8013C4C0(void) {
    D_801469F0 = 0x801CE120;
    D_801469F4 = 0x801CE124;
    D_801469F8 = 0x801CE144;
    D_801469FC = (*(s16 *)0x801CE158);
    D_80146A00 = 0x801B0000;
    D_80146A04 = 0x801B2000;
    D_80146A08 = 0x801B6000;
    D_80146A0C = 0x801BA000;
    D_80146A10 = 0x801BE000;
    D_80146A14 = 0x801C2000;
    D_80146A18 = 0x801C6000;
}

void func_8013C570(void) {
    if (D_80122CD0 == 1) {
        func_8013C5B0();
        return;
    }
    func_80046500();
}

void func_8013C5B0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013C5EC();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */
extern FnTbl32 D_80146A1C;

void func_8013C5EC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = D_80146A1C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013C674(void) {
    D_80122D10 = 2;
    D_80120652 = 0;
    func_8004284C();
}

void func_8013C6A4(void) {
    if ((u16) D_800CA154 == 0) {
        D_800E6280.unk_F5F = 0xE;
    } else if ((u16) D_800CA154 == 1) {
        D_800E6280.unk_F5F = 0xD;
    } else {
        D_800E6280.unk_F5F = 3;
    }
    func_80138AF8();
}

void func_8013C704(void) {
    func_80046318(0x3D, 0x801B0000, 0x829A);
    func_8013C4C0();
    func_8004284C();
}

void func_8013C73C(void) {
    D_800E6280.unk_F5F = 3;
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
    D_80146274 = D_80145F20;
    D_80146278 = D_8014605C;
    D_8014627C = D_80146198;
    func_800AE0F0(D_800CA19C, "校舎裏");
    D_800E6280.unk_1BC[3].unk_06 += 2;
    func_80084D3C();
    D_800CA368 = 0x15;
    func_8004284C();
}

void func_8013C87C(void) {
    func_8013C4C0();
    load_palette(D_80146A00, 0x11, 1, 2, 0);
    func_80084E90(D_80146A04, D_80146A08, D_80146A0C, D_80146A10, D_80146A14, D_80146A18);
    func_800850D4(D_801469F4, D_801469F8, D_801469F0, D_801469FC);
    D_800CA360 = 1;
    func_8004284C();
}

void func_8013C92C(void) {
    bg_read_sub2(0x407A);
    func_8004284C();
}

void func_8013C954(void) {
    func_8007ED84(0x46BC);
    func_8004284C();
}
