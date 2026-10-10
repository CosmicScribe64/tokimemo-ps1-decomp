#define MAIN_API_OVERRIDE_D_80122EC8 /* EVENT table of 40 function pointers (main_api.h: s32, used by TAIIKU) */
#include "common.h"
#include "ovl/EVENT.h"

void func_80103B60(s16 dx, s16 dy) {
    s16 i;

    for (i = 0; i < 6; i++) {
        *(s16 *)(D_800EC190 + i * 0x24 + 0x94) += dx;
        *(s16 *)(D_800EC190 + i * 0x24 + 0x96) += dy;
    }
}

void func_80103BC0(void) {
    func_80015D28(0x35, 0x801B0000, 0x852D);
    func_80102770();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80103BF8);

/* D_800B0B14 read as a bit-field (bit 14): IDO tests it with sll 17 / bltz, as the original does (T-4070). */
typedef struct {
    unsigned pad:14;
    unsigned f:1;
    unsigned rest:17;
} BitsB14;

void func_80103C3C(void) {
    if (!((BitsB14 *)&D_800B0B14)->f) {
        D_80094714 += 1;
    }
    func_80011DFC();
}

void func_80103C80(void) {
    D_80122B30 = 0x801CE124;
    D_80122B34 = 0x801CE128;
    D_80122B38 = 0x801CE148;
    D_80122B3C = *(s16 *)0x801CE15C;
    D_80122B40 = 0x801B0000;
    D_80122B44 = 0x801B2000;
    D_80122B48 = 0x801B6000;
    D_80122B4C = 0x801BA000;
    D_80122B50 = 0x801BE000;
    D_80122B54 = 0x801C2000;
    D_80122B58 = 0x801C6000;
}

void func_80103D30(void) {
    D_80122B5C = 0x801CE0DC;
    D_80122B60 = 0x801CE0E0;
    D_80122B64 = 0x801CE0F8;
    D_80122B68 = *(s16 *)0x801CE10C;
    D_80122B6C = 0x801B0000;
    D_80122B70 = 0x801B2000;
    D_80122B74 = 0x801B6000;
    D_80122B78 = 0x801BA000;
    D_80122B7C = 0x801BE000;
    D_80122B80 = 0x801C2000;
    D_80122B84 = 0x801C6000;
}

void func_80103DE0(void) {
    D_80122B88 = 0x801D20F8;
    D_80122B8C = 0x801D2100;
    D_80122B90 = 0x801D2120;
    D_80122B94 = *(s16 *)0x801D213C;
    D_80122B98 = 0x801B0000;
    D_80122B9C = 0x801B2000;
    D_80122BA0 = 0x801B6000;
    D_80122BA4 = 0x801BA000;
    D_80122BA8 = 0x801BE000;
    D_80122BAC = 0x801C2000;
    D_80122BB0 = 0x801C6000;
}

void func_80103E90(void) {
    D_80122BB4 = 0x801D2240;
    D_80122BB8 = 0x801D2248;
    D_80122BBC = 0x801D2288;
    D_80122BC0 = *(s16 *)0x801D22A0;
    D_80122BC4 = 0x801B0000;
    D_80122BC8 = 0x801B2000;
    D_80122BCC = 0x801B6000;
    D_80122BD0 = 0x801BA000;
    D_80122BD4 = 0x801BE000;
    D_80122BD8 = 0x801C2000;
    D_80122BDC = 0x801C6000;
}

void func_80103F40(void) {
    D_80122BE0 = 0x801CE080;
    D_80122BE4 = 0x801CE084;
    D_80122BE8 = 0x801CE094;
    D_80122BEC = *(s16 *)0x801CE0A8;
    D_80122BF0 = 0x801B0000;
    D_80122BF4 = 0x801B2000;
    D_80122BF8 = 0x801B6000;
    D_80122BFC = 0x801BA000;
    D_80122C00 = 0x801BE000;
    D_80122C04 = 0x801C2000;
    D_80122C08 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80103FF0);

void func_80104090(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_801040F0();
        return;
    }
    func_80015FE0();
}

typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
extern FnTbl29 D_80122C0C;

void func_801040F0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl29 tbl;

    tbl = D_80122C0C;
    idx = D_800B1AF6;
    func_80078950("s%d %d\n", idx, D_800E9E63);
    tbl.f[D_800B1AF6]();
}

void func_8010418C(void) {
    func_80015D28(0x3D, 0x801B0000, 0x85A7);
    func_80103C80();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_801041C4);

void func_80104310(void) {
    func_80103C80();
    func_80012D64(D_80122B40, 0x11, 1, 2, 0);
    func_8004C46C(D_80122B44, D_80122B48, D_80122B4C, D_80122B50, D_80122B54, D_80122B58);
    func_8004C6B0(D_80122B34, D_80122B38, D_80122B30, (s32) D_80122B3C);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_801043B8);

void func_801044CC(void) {
    D_80120678 = D_801204A0;
    D_8012067C = D_801204B4;
    D_80120680 = D_801204C8;
    func_80103DE0();
    func_80012D64(D_80122B98, 0x11, 1, 2, 0);
    func_8004C46C(D_80122B9C, D_80122BA0, D_80122BA4, D_80122BA8, D_80122BAC, D_80122BB0);
    func_8004C6B0(D_80122B8C, D_80122B90, D_80122B88, D_80122B94);
    /* FAKE: the two later fields reached through D_800B0B3E; separate names let as1 hoist their loads above the stores. T-4100 */
    D_800B0B3E += 3;
    (&D_800B0B3E)[2] += 2;
    (&D_800B0B3E)[4] -= 0x14;
    func_8004C250();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_801045E0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_801046F4);

void func_8010484C(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_80104874(void) {
    func_800469F4(0x47A7);
    func_80011DFC();
}

void func_8010489C(void) {
    func_800469F4(0x446A);
    func_80011DFC();
}

void func_801048C4(void) {
    func_800469F4(0x4814);
    func_80011DFC();
}

void func_801048EC(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void func_80104914(void) {
    func_800469F4(0x483E);
    func_80011DFC();
}

void func_8010493C(void) {
    func_800469F4(0x454C);
    func_80011DFC();
}

void func_80104964(void) {
    func_800469F4(0x4876);
    func_80011DFC();
}

void func_8010498C(void) {
    func_800469F4(0x45A0);
    func_80011DFC();
}

typedef struct {
    void (*f[50])();
} FnTbl50; /* size 0xC8 */
extern FnTbl50 D_80122C80;

void func_801049B4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl50 tbl;

    tbl = D_80122C80;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void func_80104A48(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

void func_80104A70(void) {
    func_80015D28(0x3D, 0x801B0000, 0x85E4);
    func_80103D30();
    func_80011DFC();
}

void func_80104AA8(void) {
    if (func_8002328C(D_800B1746) < 2U) {
        func_8004A8EC(0);
        return;
    }
    func_8004A8EC(func_8002336C(D_800B1746));
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80104B00);

typedef struct {
    void (*f[55])();
} FnTbl55; /* size 0xDC */
extern FnTbl55 D_80122D48;

void func_80104B50(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl55 tbl;

    tbl = D_80122D48;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void func_80104BDC(void) {
    func_80015D28(0x45, 0x801B0000, 0x8621);
    func_80103DE0();
    func_80011DFC();
}

void func_80104C14(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80104C3C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80104D48);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80104DB4);

typedef struct {
    void (*f[41])();
} FnTbl41; /* size 0xA4 */
extern FnTbl41 D_80122E24;

void func_80104E64(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl41 tbl;

    tbl = D_80122E24;
    idx = D_800B1AF6;
    func_80078950("%d\n", idx);
    tbl.f[D_800B1AF6](0x80);
}

void func_80104EF8(void) {
    func_80015D28(0x45, 0x801B0000, 0x8666);
    func_80103E90();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_80104F30);

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_80122EC8;

void func_80104F80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_80122EC8;
    idx = D_800B1AF6;
    func_80078950("%d %d %d\n", idx, D_80094714, D_80094718);
    tbl.f[D_800B1AF6](0x80);
}

void func_8010501C(void) {
    func_80015D28(0x3D, 0x801B0000, 0x86AB);
    func_80103F40();
    func_80011DFC();
}

void func_80105054(void) {
    func_800433D0(0x501);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/80103B60", func_8010507C);
