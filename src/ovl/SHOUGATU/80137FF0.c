#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[24])();
} FnTbl24; /* size 0x60 */
extern FnTbl24 D_801450C0;

typedef struct {
    void (*f[20])();
} FnTbl20; /* size 0x50 */
extern FnTbl20 D_80144F14;

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_80145048;

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80137FF0);

void func_8013808C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F14;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
    if (D_80144E08 == 1) {
        if (D_80144E0C == 8) {
            if (D_800E6280.unk_1104.w++ == 0) {
                func_80044750(0x500);
            }
        }
    }
}

s32 func_80138158(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        if (D_800E6280.unk_1104.w++ == 1) {
            func_80044750(0x24);
            func_80044750(0x501);
        }
    }
}

extern FnTbl20 D_80144F64;

void func_801381B8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80144F64;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138240);

void func_80138354(void) {
    func_80137AB4();
    if (D_800E6280.unk_1104.w++ == 0) {
        if (D_80144E0C == 3) {
            func_80044750(0x500);
        }
    }
}

void func_801383AC(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801383D4(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801383FC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_8013843C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_80145048;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

void func_801384B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl24 tbl;

    tbl = D_801450C0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

extern FnTbl20 D_80145120;

void func_80138524(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl20 tbl;

    tbl = D_80145120;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_801385AC);

void func_801386CC(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

void func_80138710(void) {
    D_80144E08 = 3;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138758);

void func_80138868(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 0xE;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "教室");
    func_800AE0F0(D_800CA1DC, "実験室");
    func_8004284C();
}

void func_801388F0(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 0x12;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "実験室");
    func_8004284C();
}

void func_80138964(void) {
    ((Bits64B8 *)&D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0])->f = 1;
    D_80144E08 = 0x17;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "実験室");
    func_8004284C();
}

void func_801389D8(void) {
    bg_read_sub2(0x405F);
    func_8004284C();
}

void func_80138A00(void) {
    bg_read_sub2(0x403B);
    func_8004284C();
}

void func_80138A28(void) {
    if (D_80144F10 != 0) {
        bg_read_sub2(0x40D0);
    } else {
        bg_read_sub2(0x40C6);
    }
    func_8004284C();
}

void func_80138A6C(void) {
    bg_read_sub2(0x40C6);
    func_8004284C();
}

void func_80138A94(void) {
    bg_read_sub2(0x40D0);
    func_8004284C();
}

void func_80138ABC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/80137FF0", func_80138AFC);
