#include "common.h"
#include "ovl/EVENT.h"

void func_800FDC20(void) {
    D_80121800 = 0x801CE11C;
    D_80121804 = 0x801CE120;
    D_80121808 = 0x801CE140;
    D_8012180C = *(s16 *)0x801CE154;
    D_80121810 = 0x801B0000;
    D_80121814 = 0x801B2000;
    D_80121818 = 0x801B6000;
    D_8012181C = 0x801BA000;
    D_80121820 = 0x801BE000;
    D_80121824 = 0x801C2000;
    D_80121828 = 0x801C6000;
}

void func_800FDCD0(void) {
    D_8012182C = 0x801CE0DC;
    D_80121830 = 0x801CE0E0;
    D_80121834 = 0x801CE100;
    D_80121838 = *(s16 *)0x801CE114;
    D_8012183C = 0x801B0000;
    D_80121840 = 0x801B2000;
    D_80121844 = 0x801B6000;
    D_80121848 = 0x801BA000;
    D_8012184C = 0x801BE000;
    D_80121850 = 0x801C2000;
    D_80121854 = 0x801C6000;
}

void func_800FDD80(void) {
    D_80121858 = 0x801D2148;
    D_8012185C = 0x801D2150;
    D_80121860 = 0x801D2170;
    D_80121864 = *(s16 *)0x801D218C;
    D_80121868 = 0x801B0000;
    D_8012186C = 0x801B2000;
    D_80121870 = 0x801B6000;
    D_80121874 = 0x801BA000;
    D_80121878 = 0x801BE000;
    D_8012187C = 0x801C2000;
    D_80121880 = 0x801C6000;
}

void func_800FDE30(void) {
    D_80121884 = 0x801CE11C;
    D_80121888 = 0x801CE120;
    D_8012188C = 0x801CE140;
    D_80121890 = *(s16 *)0x801CE154;
    D_80121894 = 0x801B0000;
    D_80121898 = 0x801B2000;
    D_8012189C = 0x801B6000;
    D_801218A0 = 0x801BA000;
    D_801218A4 = 0x801BE000;
    D_801218A8 = 0x801C2000;
    D_801218AC = 0x801C6000;
}

void func_800FDEE0(void) {
    D_801218B0 = 0x801CE07C;
    D_801218B4 = 0x801CE080;
    D_801218B8 = 0x801CE090;
    D_801218BC = *(s16 *)0x801CE0A4;
    D_801218C0 = 0x801B0000;
    D_801218C4 = 0x801B2000;
    D_801218C8 = 0x801B6000;
    D_801218CC = 0x801BA000;
    D_801218D0 = 0x801BE000;
    D_801218D4 = 0x801C2000;
    D_801218D8 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FDF90);

void func_800FE090(void);

void func_800FE030(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_800FE090();
        return;
    }
    func_80015FE0();
}

typedef struct {
    u8 b[0x80];
} Blob80; /* size 0x80 */
extern Blob80 D_801218E8;

void func_800FE090(void) {
    s32 pad; /* FAKE: unused local above tmp; the original frame is 4 bytes larger, real source unknown. T-4080 */
    Blob80 tmp;

    tmp = D_801218E8;
    func_80078950("s%d\n", D_800B1AF6);
}

void func_800FE100(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F2A);
    func_800FDC20();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FE138);

void func_800FE270(void) {
    func_800FDC20();
    func_80012D64(D_80121810, 0x11, 1, 2, 0);
    func_8004C46C(D_80121814, D_80121818, D_8012181C, D_80121820, D_80121824, D_80121828);
    func_8004C6B0(D_80121804, D_80121808, D_80121800, (s32) D_8012180C);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FE318);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FE494);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FE6C0);

void func_800FE7D4(void) {
    D_80120678 = D_801203C4;
    D_8012067C = D_801203DC;
    D_80120680 = D_801203F4;
    func_800FDEE0();
    func_80012D64(D_801218C0, 0x11, 1, 2, 0);
    func_8004C46C(D_801218C4, D_801218C8, D_801218CC, D_801218D0, D_801218D4, D_801218D8);
    func_8004C6B0(D_801218B4, D_801218B8, D_801218B0, (s32) D_801218BC);
    func_80011DFC();
}

void func_800FE8A8(void) {
    func_800469F4(0x4015);
    func_80011DFC();
}

void func_800FE8D0(void) {
    func_800469F4(0x462C);
    func_80011DFC();
}

void func_800FE8F8(void) {
    func_800469F4(0x4862);
    func_80011DFC();
}

void func_800FE920(void) {
    func_800469F4(0x458C);
    func_80011DFC();
}

void func_800FE948(void) {
    func_800469F4(0x480C);
    func_80011DFC();
}

void func_800FE970(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void func_800FE998(void) {
    func_800469F4(0x479D);
    func_80011DFC();
}

void func_800FE9C0(void) {
    func_800469F4(0x446A);
    func_80011DFC();
}

void func_800FE9E8(void) {
    func_800469F4(0x4847);
    func_80011DFC();
}

void func_800FEA10(void) {
    func_800469F4(0x455F);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEA38);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEAE8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEDC8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FEF74);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FF320);

void func_800FF5E4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7F67);
    func_800FDCD0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FF61C);

void func_800FF710(void) {
    func_80015D28(0x45, 0x801B0000, 0x7FA4);
    func_800FDD80();
    func_80011DFC();
}

void func_800FF748(void) {
    D_800EB02A = 5;
    func_80011DFC();
}

typedef struct {
    void (*f[42])();
} FnTbl42; /* size 0xA8 */
extern FnTbl42 D_80121BB8;

void func_800FF770(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl42 tbl;

    tbl = D_80121BB8;
    func_80078950("%d\n", D_800B1AF6);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FF7F4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x7FE9);
    func_800FDE30();
    func_80011DFC();
}

typedef struct {
    void (*f[45])();
} FnTbl45; /* size 0xB4 */
extern FnTbl45 D_80121C60;

void func_800FF82C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl45 tbl;

    tbl = D_80121C60;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_800FF8C0(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8026);
    func_800FDEE0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FF8F8);

void func_800FF9D4(void) {
    if (D_800EECBC == 0) {
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_800FFA10(void) {
    if (D_800EECBC == 0) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/800FDC20", func_800FFA4C);
