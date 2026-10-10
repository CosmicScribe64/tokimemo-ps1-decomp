#include "common.h"
#include "ovl/DATE.h"

void func_8014A1B0(void) {
    D_8015DE28 = 0x80195000;
    D_8015DE2C = 0x80195060;
    D_8015DE30 = 0x801950C0;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A1E4);

void func_8014A280(void) {
    if (D_8015DE44 == 0) {
        func_8014A4DC();
    }
}

void func_8014A2AC(void) {
    switch (D_8015DE44) {
    case 0:
        func_8014A810();
        return;
    case 1:
        func_8014A8FC();
        return;
    case 2:
        func_8014AD70();
        return;
    }
}

void func_8014A314(void) {
    func_80044750(0x200);
    read_bustup();
    D_800E6280.unk_110A -= 1;
    D_8015DE40 += 1;
}

void func_8014A360(void) {
    if (func_800460CC() & 1) {
        D_8015DE40 += 1;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A39C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A4DC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A810);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014A8FC);

void func_8014AD70(void) {
    draw2d3d(1, 0);
    func_8014B194();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014AD9C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014AF88);

void func_8014B080(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x32;
    D_8012276C = 0;
    D_8012276D = 0;
    D_8012276E = 0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x14;
    D_80122774 = 0xA;
    D_80122778 = -0x64;
    D_8012277C = 0;
    D_8012277D = 0;
    D_8012277E = 0;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0;
    D_8012278D = 0;
    D_8012278E = 0;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

void func_8014B194(void) {
    D_8015DE40 += 1;
    D_8015DE44 = 0;
    D_8015DE4C = 0;
    D_8015DE48 = 0;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/8014A1B0", func_8014B1C4);
