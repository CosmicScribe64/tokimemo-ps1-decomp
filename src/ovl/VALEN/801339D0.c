#include "ovl/VALEN.h"

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_80134630;

void func_801339D0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_80134630;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80133A4C(void) {
    if (func_800AEEB0(D_800CA1DC, "自宅前") == 0) {
        func_8004284C();
        return;
    }
    func_800AE0F0(D_800CA1DC, "自宅前");
    func_8007EC9C();
}

s32 func_80133AA8(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

/* Bit 1 of a Rec38 flag word: IDO loads its byte into another register than the lui (T-7010). */
typedef struct {
    u32 pad0 : 1;
    u32 f : 1;
    u32 pad1 : 30;
} Rec38Bit1;

void func_80133B6C(void) {
    if (D_80134544 != 0) {
        ((Rec38Bit1 *)&D_800E6280.unk_1BC[9].unk_0C)->f = 1;
    }
    if (D_8013453C >= 7) {
        D_80134498 = 0;
    } else if (D_8013453C >= 5) {
        D_80134498 = 1;
    } else if (D_8013453C >= 3) {
        D_80134498 = 2;
    } else if (D_8013453C > 0) {
        D_80134498 = 3;
    } else {
        D_80134498 = 4;
    }
    D_8013448C = D_801343F4;
    D_80134490 = D_80134428;
    D_80134494 = D_8013445C;
    func_8004284C();
}

void func_80133C44(void) {
    bg_read_sub2(0x414C);
    func_8004284C();
}
