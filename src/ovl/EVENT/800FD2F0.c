#define MAIN_API_OVERRIDE_D_800EECB0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/EVENT.h"

extern u32 D_800EECB0;

void func_800FD2F0(void) {
    D_801215E0 = 0x801B0000;
    D_801215E4 = 0x801B2000;
    D_801215E8 = 0x801B6000;
    D_801215EC = 0x801BA000;
    D_801215F0 = 0x801BE000;
    D_801215F4 = 0x801C2000;
    D_801215F8 = 0x801C6000;
}

void func_800FD360(void) {
    switch (D_800EECB0) {
    case 1:
        func_800FD3DC();
        return;
    case 2:
        func_800FD43C();
        return;
    case 3:
        func_800FD49C();
        return;
    default:
        func_80015FE0();
        return;
    }
}

void func_800FD3DC(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FD4FC();
        return;
    }
    func_80015FE0();
}

void func_800FD43C(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FD604();
        return;
    }
    func_80015FE0();
}

void func_800FD49C(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FD6E0();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[51])();
} FnTbl51; /* size 0xCC */
extern FnTbl51 D_801215FC;

void func_800FD4FC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl51 tbl;

    tbl = D_801215FC;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FD2F0", func_800FD588);

typedef struct {
    void (*f[37])();
} FnTbl37; /* size 0x94 */
extern FnTbl37 D_801216C8;

void func_800FD604(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl37 tbl;

    tbl = D_801216C8;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FD680(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_800FD6A8(void) {
    func_80015D28(0x35, 0x801B0000, 0x7EF5);
    func_800FD2F0();
    func_80011DFC();
}

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_8012175C;

void func_800FD6E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_8012175C;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FD77C(void) {
    if (D_800EECBC != 0) {
        D_80094714 = (u16) D_80094714 + 5;
    }
    func_80011DFC();
}

void func_800FD7BC(void) {
    func_80011DFC();
    if (D_800EECBC != 0) {
        D_800B1AF6 += 0x13;
    }
}

void func_800FD800(void) {
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FD2F0", func_800FD820);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FD2F0", func_800FD974);

void func_800FDA9C(void) {
    func_800FD2F0();
    func_80012D64(D_801215E0, 0x11, 1, 2, 0);
    func_8004C46C(D_801215E4, D_801215E8, D_801215EC, D_801215F0, D_801215F4, D_801215F8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}

void func_800FDB30(void) {
    D_80120678 = D_80120620;
    D_8012067C = D_80120640;
    D_80120680 = (s32) D_80120660;
    func_80011DFC();
}

void func_800FDB7C(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800FDBA4(void) {
    func_800469F4(0x488A);
    func_80011DFC();
}

void func_800FDBCC(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_800FDBF4(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}
