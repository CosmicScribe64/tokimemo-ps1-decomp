#include "common.h"
#include "ovl/EVENT.h"

typedef struct {
    void (*f[56])();
} FnTbl56; /* size 0xE0 */
extern FnTbl56 D_80122A50;

typedef struct {
    void (*f[40])();
} FnTbl40; /* size 0xA0 */
extern FnTbl40 D_801229B0;

typedef struct {
    void (*f[38])();
} FnTbl38; /* size 0x98 */
extern FnTbl38 D_80122848;

typedef struct {
    void (*f[32])();
} FnTbl32; /* size 0x80 */

typedef struct {
    void (*f[19])();
} FnTbl19; /* size 0x4C */
extern FnTbl19 D_801227FC;

void func_801024B0(void) {
    D_80122680 = 0x801CE0F4;
    D_80122684 = 0x801CE0F8;
    D_80122688 = 0x801CE118;
    D_8012268C = *(s16 *)0x801CE12C;
    D_80122690 = 0x801B0000;
    D_80122694 = 0x801B2000;
    D_80122698 = 0x801B6000;
    D_8012269C = 0x801BA000;
    D_801226A0 = 0x801BE000;
    D_801226A4 = 0x801C2000;
    D_801226A8 = 0x801C6000;
}

void func_80102560(void) {
    D_801226AC = 0x801CE07C;
    D_801226B0 = 0x801CE080;
    D_801226B4 = 0x801CE090;
    D_801226B8 = *(s16 *)0x801CE0A4;
    D_801226BC = 0x801B0000;
    D_801226C0 = 0x801B2000;
    D_801226C4 = 0x801B6000;
    D_801226C8 = 0x801BA000;
    D_801226CC = 0x801BE000;
    D_801226D0 = 0x801C2000;
    D_801226D4 = 0x801C6000;
}

void func_80102610(void) {
    D_801226D8 = 0x801CE31C;
    D_801226DC = 0x801CE320;
    D_801226E0 = 0x801CE39C;
    D_801226E4 = *(s16 *)0x801CE3BC;
    D_801226E8 = 0x801B0000;
    D_801226EC = 0x801B2000;
    D_801226F0 = 0x801B6000;
    D_801226F4 = 0x801BA000;
    D_801226F8 = 0x801BE000;
    D_801226FC = 0x801C2000;
    D_80122700 = 0x801C6000;
}

void func_801026C0(void) {
    D_80122704 = 0x801CE124;
    D_80122708 = 0x801CE128;
    D_8012270C = 0x801CE148;
    D_80122710 = *(s16 *)0x801CE15C;
    D_80122714 = 0x801B0000;
    D_80122718 = 0x801B2000;
    D_8012271C = 0x801B6000;
    D_80122720 = 0x801BA000;
    D_80122724 = 0x801BE000;
    D_80122728 = 0x801C2000;
    D_8012272C = 0x801C6000;
}

void func_80102770(void) {
    D_80122730 = 0x801B0000;
    D_80122734 = 0x801B2000;
    D_80122738 = 0x801B6000;
    D_8012273C = 0x801BA000;
    D_80122740 = 0x801BE000;
    D_80122744 = 0x801C2000;
    D_80122748 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_801027E0);

void func_80102890(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80102950();
        return;
    }
    func_80015FE0();
}

void func_801028F0(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80102A24();
        return;
    }
    func_80015FE0();
}

void func_80102950(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl32 tbl;

    tbl = *(FnTbl32 *)&D_8012277C;
    func_80078950("s%d %d\n", D_800B1AF6, D_800E9E63);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_801029EC(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8439);
    func_801024B0();
    func_80011DFC();
}

void func_80102A24(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl19 tbl;

    tbl = D_801227FC;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx]();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80102AB0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80102C0C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80102CC4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80102D40);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80102E54);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_801030F4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80103208);

void func_80103308(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_80103330(void) {
    func_800469F4(0x463D);
    func_80011DFC();
}

void func_80103358(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_80103380(void) {
    func_800469F4(0x4835);
    func_80011DFC();
}

void func_801033A8(void) {
    func_800469F4(0x454C);
    func_80011DFC();
}

void func_801033D0(void) {
    func_800469F4(0x47FC);
    func_80011DFC();
}

void func_801033F8(void) {
    func_800469F4(0x450A);
    func_80011DFC();
}

void func_80103420(void) {
    func_800469F4(0x4726);
    func_80011DFC();
}

void func_80103448(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_80103470(void) {
    func_800469F4(0x47D1);
    func_80011DFC();
}

void func_80103498(void) {
    func_800469F4(0x44DB);
    func_80011DFC();
}

void func_801034C0(void) {
    func_800469F4(0x44D1);
    func_80011DFC();
}

void func_801034E8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl38 tbl;

    tbl = D_80122848;
    func_80078950("%d %d\n", D_800B1AF6, D_800EECD0);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80103584(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8476);
    func_80102560();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_801035BC);

void func_80103714(void) {
    func_80015D28(0x3D, 0x801B0000, 0x84B3);
    func_80102610();
    func_80011DFC();
}

void func_8010374C(void) {
    func_800433D0(0x502);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801024B0", func_80103774);

void func_801039E8(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl40 tbl;

    tbl = D_801229B0;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80103A84(void) {
    func_80015D28(0x3D, 0x801B0000, 0x84F0);
    func_801026C0();
    func_80011DFC();
}

void func_80103ABC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl56 tbl;

    tbl = D_80122A50;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}
