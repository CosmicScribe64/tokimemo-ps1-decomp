#include "common.h"
#include "ovl/ENDING.h"

void func_8013B6A0(void) {
    D_8013CBB0 = 0x801C22CC;
    D_8013CBB4 = 0x801C22D4;
    D_8013CBB8 = 0x801C22F0;
    D_8013CBBC = *(s16 *)0x801C2300;
    D_8013CBC0 = 0x801A0000;
    D_8013CBC4 = 0x801C2000;
    D_8013CBC8 = 0x801AA000;
    D_8013CBCC = 0x801AE000;
    D_8013CBD0 = 0x801B2000;
    D_8013CBD4 = 0x801B6000;
    D_8013CBD8 = 0x801BA000;
    D_8013CBDC = 0x801BE000;
}

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_8013CBE4;

void func_8013B760(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_8013CBE4;
    if (D_800E6280.unk_1109 == 7 && D_800E6280.unk_110A < 0x10) {
        func_80083808();
    }
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_1109 == 7) {
        if (D_800E6280.unk_110A < 0x10) {
            func_8007EDF8();
            func_800846C0();
            func_80066C08(2);
            func_80064F48();
            func_80066334();
            func_80064DEC();
            func_80083A10();
        }
    }
}

void func_8013B868(void) {
    func_80042908(6);
}

void func_8013B888(void) {
    func_80042908(8);
}

void func_8013B8A8(void) {
    if (D_800E6280.unk_56C[82] != 0) {
        func_8004284C();
        return;
    }
    normal_date_girl_out();
}

void func_8013B8E4(void) {
    func_80046318(0x45, 0x801A0000, 0x727A);
    func_8013B6A0();
    func_8004284C();
}

s32 func_8013B91C(void) {
    if (D_800E6280.unk_56C[82] != 0) {
        if ((u32) D_800E6280.unk_1104.w++ < 0x78U) {
            return 0;
        }
    }
    D_800E6280.unk_F5F = 0xD;
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_800847B8(D_800E6280.unk_56C[82]);
    func_80132000();
    D_800CA160 = D_8013C280;
    D_800CA164 = D_8013C2C4;
    D_800CA168 = D_8013C308;
    if (D_800E6280.unk_56C[82] != 0) {
        D_800CA148 = 2;
        D_800E6280.unk_110A += 0xA;
    }
    func_8004284C();
}

void func_8013B9F0(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_8006BD6C(0);
    func_800438F0(1);
    func_800482FC();
    D_800E6280.unk_1228[28] = 1;
    D_800E6280.unk_1228[29] = 1;
    D_800E6280.unk_1228[30] = 1;
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8013B6A0();
    func_80043914(D_8013CBC4, 0x11, 1, 2, 0);
    func_80043914(D_8013CBC0, 0x12, 1, 2, 0);
    func_80084E90(D_8013CBC8, D_8013CBCC, D_8013CBD0, D_8013CBD4, D_8013CBD8, D_8013CBDC);
    func_800850D4(D_8013CBB4, D_8013CBB8, D_8013CBB0, D_8013CBBC);
    D_801217D0[61].unk_0A = D_801217D0[62].unk_0A = D_801217D0[63].unk_0A = 0x70;
    func_80048F64(0x61);
    D_80120666 = 0;
    D_80120654 = 8;
    D_80120693 = 0x12;
    D_80120652 = 0;
    D_80120653 = 4;
    func_8004284C();
}

typedef struct {
    s32 w[4];
} Tw4; /* size 0x10 */
extern Tw4 D_8013CD90;

void func_8013BB9C(void) {
    s32 r;
    Tw4 v;

    v = D_8013CD90;
    func_80044750(0xBF);
    if (D_800E6280.unk_56C[82] != 0) {
        func_80046290(v.w[2], v.w[3], 0xF);
    } else {
        func_80046290(v.w[0], v.w[1], 0xF);
    }
    func_80044750(0x300);
    r = func_8004E788(-0x50, 0x50, 0, "『女々しい野郎どもの詩』", 0);
    func_8004E884(r);
    D_8013CBE0 = 0;
    func_8004284C();
}

typedef struct {
    s16 h[13];
} Th13; /* size 0x1A */
extern Th13 D_8013CDA0;

void func_8013BC58(void) {
    Th13 tbl;
    s16 i;
    s16 sel;

    tbl = D_8013CDA0;
    sel = 0xFF;
    for (i = 0; i < 13; i++) {
        if (D_8013CBE0 >= tbl.h[i]) {
            sel = i;
        }
    }
    if (D_8013CBE0 == 0x3E8) {
        func_8004E9F4(1, sel, &tbl);
    }
    if (sel != 0xFF) {
        if (D_8013CBE0 == tbl.h[sel]) {
            func_8004E9F4(1, sel, &tbl);
            func_8013BDD8(sel);
        }
    }
    if (D_8013CBE0 == 0x1536) {
        func_8004E9F4(1);
    }
    if (func_800460EC() & 4) {
        D_8013CBE0 += 1;
    }
    if (func_80046094() == 9) {
        func_8004284C();
    }
}

void func_8013BDD8(s16 arg0) {
    u8 *p;

    p = D_8013CC48 + arg0 * 0x19;
    k_disp_start(set_kanji_string((s16) (-func_800AE0E0(p) * 7 / 2), 0x50, 0, p, 0));
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013BE64);
