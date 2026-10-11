#include "common.h"
#include "ovl/GYOZI.h"

void func_80140740(void) {
    D_801486D0 = 0x801A6558;
    D_801486D4 = 0x801A655C;
    D_801486D8 = 0x801A6580;
    D_801486E0 = 0x801A0000;
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_801486E8;

s32 func_80140780(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_801486E8;
    idx = D_800F647A;
    tbl.f[idx](0x80);
    D_8012B8C0[0].unk_07 = *(u8 *)&D_801317EB;
    if (D_8012B8C0[0].unk_18 == 0) {
        if (D_8012B8C0[0].unk_08 == 0) {
            D_8012B8C0[0].unk_16 += 1;
            if (D_8012B8C0[0].unk_16 >= 3) {
                D_8012B8C0[0].unk_16 = 0;
            }
        }
    }
}

void func_8014086C(void) {
    func_80086AB0(0x204);
    func_8004DE1C();
}

void func_80140894(void) {
    D_8012B8C3 |= 0x80;
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80140740.rodata", D_80145D20);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80140740.rodata", D_80145D28);

INCLUDE_RODATA("asm/ovl/GYOZI/data/GYOZI/80140740.rodata", D_80145D30);

void func_801408C4(void) {
    func_80090960(4, 0);
    D_800F594E += 1;
    func_800BCE10(&D_800D9280, &D_80145D20);
    func_800BCE10(&D_800D9288, &D_80145D28);
    func_800BCE10(&D_800D92E0, &D_80145D30);
    D_80148620 = 9;
    func_8004DE1C();
    func_80140740();
    func_8004EE18(D_801486E0, 0x11, 1, 3, 0);
    func_800549E8(0x60);
    D_8012B8C1 = 0xC;
    D_8012B8C2 = 1;
    D_8012B8F8 = 0x41000000;
    D_8012B8C3 = 4;
    D_8012B8CC = D_801486D4;
    D_8012B8D0 = D_801486D8;
    D_8012B8F4 = D_801486D0;
    D_8012B8D4 = D_801486DC;
    D_8012B8D6 = 0;
    D_8012B8D8 = 0;
    D_8012B903 = 0x11;
    D_8012B8E6 = -0xA0;
    D_8012B8EA = -0x78;
    D_8012B8C7 = 0;
    D_8012B8C5 = 0;
    D_8012B8C4 = 8;
}

void func_80140A30(void) {
    func_8008A0D4(0x4173);
    func_8004DE1C();
}

void func_80140A58(void) {
    D_80148620 = (D_80148620 + (D_800F594E * 3)) - 3;
    func_8004DE1C();
}

void func_80140A9C(void) {
    func_80072734(0x57AC);
    func_8004DE1C();
}

void func_80140AC4(void) {
    func_80051DD8(0xD, 0x801A0000, 0xB0B8);
    func_80140740();
    func_8004DE1C();
}

/* Bit 1 of a flag word, tested as a bit-field read (sll 30; bgez) */
typedef struct {
    u32 pad : 1;
    u32 f : 1;
    u32 rest : 30;
} Bit1Flag;

void func_80140AFC(void) {
    s32 r;
    u8 a;
    u8 pad; /* FAKE: unused byte between a and b, real source unknown (frame layout) */
    u8 b;

    a = func_8005E0E0(7);
    r = func_8005E1C0(7);
    b = 0xFF;
    if (((Bit1Flag *)&D_800F56E0)->f && D_800F592F == 0 && a == 0x80 && r == 0) {
        b = 7;
    }
    a = func_8005E0E0(0);
    r = func_8005E1C0(0);
    if (D_800F5902 == 0 && a == 0x80 && r == 0 && (b == 0xFF || (b == 7 && D_800F5556 < D_800F56DE))) {
        b = 0;
    }
    a = func_8005E0E0(9);
    r = func_8005E1C0(9);
    if (((Bit1Flag *)&D_800F5750)->f && D_800F593D == 0 && a == 0x80 && r == 0 &&
        (b == 0xFF || (b == 7 && D_800F574E < D_800F56DE) || (b == 0 && D_800F574E < D_800F5556))) {
        b = 9;
    }
    D_801486E4 = b;
    func_8004DE1C();
}
