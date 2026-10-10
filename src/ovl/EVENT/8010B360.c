#include "common.h"
#include "ovl/EVENT.h"

void func_8010B360(void) {
    D_801240F0 = 0x801CE130;
    D_801240F4 = 0x801CE134;
    D_801240F8 = 0x801CE15C;
    D_801240FC = *(s16 *)0x801CE170;
    D_80124100 = 0x801B0000;
    D_80124104 = 0x801B2000;
    D_80124108 = 0x801B6000;
    D_8012410C = 0x801BA000;
    D_80124110 = 0x801BE000;
    D_80124114 = 0x801C2000;
    D_80124118 = 0x801C6000;
}

void func_8010B410(void) {
    func_8010B430();
}

void func_8010B430(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_8010B490();
    } else {
        func_80015FE0();
    }
}

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80124120;

void func_8010B490(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80124120;
    func_80078950("s%d %d\n", D_800B1AF6, D_80094714);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_8012411C != 0) {
        func_80033E88();
    }
}

void func_8010B538(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8C15);
    func_8010B360();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B570);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B614);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B678);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B6E4);

void func_8010B844(void) {
    D_80094714 = (u16) D_80094714 + D_800B1746;
    func_80011DFC();
}

void func_8010B87C(void) {
    D_80094714 = 0xC;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B8A4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010B360", func_8010B970);

void func_8010BA94(void) {
    func_8010B360();
    func_80012D64(D_80124100, 0x11, 1, 2, 0);
    func_8004C46C(D_80124104, D_80124108, D_8012410C, D_80124110, D_80124114, D_80124118);
    func_8004C6B0(D_801240F4, D_801240F8, D_801240F0, (s32) D_801240FC);
    func_80011DFC();
}

void func_8010BB3C(void) {
    func_800469F4(0x3FF0);
    func_80011DFC();
}

void func_8010BB64(void) {
    func_800469F4(0x4600);
    func_80011DFC();
}
