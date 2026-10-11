#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133870);

s32 func_80133980(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044750(0x24);
        D_8015EDEC = 0x400;
        func_800438DC(1, 0);
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        func_80041584();
        func_80143E80();
        func_800438F0(1);
        func_8006BD6C(0);
        func_8004E58C();
        func_80048EB8(0);
        func_80143AF4();
        D_8015EDB8 = 0;
        func_80046318(0x21, 0x80180000, 0xB4F3);
        func_80046318(1, 0x801FA800, 0xB514);
        func_80041168(0xFF);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
    case 2:
        if (func_800460CC() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        return;
    case 3:
        D_800E6280.unk_1A6C = 0x8018A000;
        D_800E6280.unk_1A4C = 0x801FAA7C;
        D_800E6280.unk_1A5C = 0x801FAA88;
        D_800E6280.unk_1A3C = 0x801FAA68;
        func_801342CC();
        func_8004284C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133AF4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133C08);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80133E98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801342CC);

void func_80134450(void) {
    func_8004435C(0x100, 0x1E0, 0x100, 0x18, 0x801B8000);
    func_8004435C(0, 0x1F0, 0x100, 0x10, 0x801C0000);
    func_80143730(0x801C8000, 8);
    func_80143730(0x801C8000, 0xA);
    func_80143730(0x801C8000, 0xC);
    func_80143730(0x801C8000, 0xE);
    func_80143730(0x801C8000, 0x18);
    func_80143730(0x801C8000, 0x1A);
    func_8009C674(0);
}

s32 func_80134500(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_038 = 0;
        D_800E6280.unk_10A2 = 0;
        D_800E6280.unk_03A = 0x80;
        D_800E6280.unk_036 = 0;
        func_800438DC(1, 1);
        func_80058D20();
        func_8014394C();
        func_80058EB0();
        func_801345D4();
        func_80058F0C(0x8018A000);
        func_8013474C();
        func_801347F4(0x801AC000);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80042908(1);
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801345D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_8013474C);

void func_801347A8(void) {
    s32 i;

    func_80059048();
    for (i = 0; i < 0x40; i++) {
        D_80127080[i].unk0 = 0x80000000;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_801347F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80133870", func_80134884);
