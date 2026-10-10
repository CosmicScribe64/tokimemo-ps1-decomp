#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80149F90);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A070);

void func_8014A1D4(void) {
    func_8014F210();
    func_8015185C();
    if (((u32)D_800E7384 % 180U) == 0) {
        func_801335A0(0, 0x500);
    }
    D_800E7384 += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A234);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A390);

void func_8014A42C(void) {
    func_8014A480(D_8015FE95, 1, D_8015FEC4, D_8015FEA0, D_8015FEAC, 1);
    func_8014F820();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A480);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014A79C);

void func_8014A8E0(void) {
    u32 v;
    u8 t;

    v = func_80147B24(0xB, D_8015FEAC);
    if (D_8015EDB4[21].unk2 < v) {
        D_80122760 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0x80;
        D_8012276E = 0x80;
        func_8009AD70(0, &D_80122760);
        D_80122770 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &D_80122770);
        D_80122780 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0x80;
        D_8012278E = 0x80;
        func_8009AD70(2, &D_80122780);
        t = D_8015EDB4[21].unk3;
        if (t >= 3U) {
            D_8015EDB4[21].unk2 = v;
            D_80122760 = 0x64;
            D_80122764 = 0x32;
            D_80122768 = -0x64;
            D_8012276C = 0x70;
            D_8012276D = 0x70;
            D_8012276E = 0x80;
            func_8009AD70(0, &D_80122760);
            D_80122770 = 0;
            D_80122774 = -0x64;
            D_80122778 = -0x64;
            D_8012277C = 0x40;
            D_8012277D = 0x40;
            D_8012277E = 0x40;
            func_8009AD70(1, &D_80122770);
            D_80122780 = -0x64;
            D_80122784 = 0x32;
            D_80122788 = -0x64;
            D_8012278C = 0x80;
            D_8012278D = 0x80;
            D_8012278E = 0x90;
            func_8009AD70(2, &D_80122780);
            D_8015EDB4[21].unk3 = 0;
        } else {
            D_8015EDB4[21].unk3 = t + 1;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014AB50);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B2A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B374);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B4B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B5F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B790);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014B8B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BC80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BCEC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BE14);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BED8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014BF38);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C0D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C1DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C2E4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C3B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C578);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C8DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014C978);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014CB24);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D200);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D2E0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D3B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D530);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014D918);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014DA6C);

void func_8014DC94(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014DF50(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 += 1;
        D_8015EDB4[16].unk3 = 0;
        if (!(D_8015EDB4[16].unk2 & 1)) {
            if (D_8015EDB4[17].unk2 == 1) {
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) + 0xF4, (func_800AE0D0() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) - 0x2F4, (func_800AE0D0() & 0x1FF) - 0x100, 0x7D0, 0xFA, 0x82);
            } else if (func_80147B24(0xB, D_8015FA68) < 0x32) {
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) + 0x2C, (func_800AE0D0() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
                func_80147C98(0, 0x15, (func_800AE0D0() & 0x1FF) - 0x22C, (func_800AE0D0() & 0x1FF) + 0x2E8, 0x7D0, 0xFA, 0x82);
            } else {
                func_80147C98(0, 0x1B, (func_800AE0D0() & 0x3FF) - 0x200, (func_800AE0D0() & 0x3FF) - 0x200, 0x7D0, 0x190, 0x81);
            }
        }
    }
    if (D_8015EDB4[16].unk2 >= 0x14U) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 += 1;
}

void func_8014DF50(s32 arg0) {
    s32 t;

    t = (-0x41A0 - D_801274A0) / 15;
    if (arg0 < 5) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        func_80146F74(0, -0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E048);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E1A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E4A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014E5F8);

void func_8014EA4C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EB24(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x50U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EB24(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, 0, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[24].unk84[2] = 1;
        func_8014C8DC();
    } else if ((arg0 >= 0xF) && (arg0 < 0x41)) {
        if (D_8015EDB4[24].unk84[2] == 1) {
            func_8014C3B4(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x41) && (arg0 < 0x50)) {
        func_80146F74(0, 0, 0xF, 0x1E, 0, 0, 0, 0x32, 0x1F4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014EC4C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014EDCC);

void func_8014EE3C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EF14(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x73U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EF14(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, -1, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[25].unk84[2] = 1;
        func_8014D200();
    } else if ((arg0 >= 0xF) && (arg0 < 0x64)) {
        if (D_8015EDB4[25].unk84[2] == 1) {
            func_8014C978(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x64) && (arg0 < 0x72)) {
        func_80146F74(0, 0, 0xF, 0x1E, 1, 0, 0, 0x32, 0x1F4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F044);

void func_8014F210(void) {
    func_80151264(5);
    func_8014FBD4(7);
    func_80146F74(0, 0, 1, 0, 0, 0, 0, 0x32, 0x1F4);
    if (func_800AE0C0(D_8012749C - D_8015EDB4->unk76) < 0x64) {
        func_8014A79C();
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F2A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F5F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014F9B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FBD4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FDC0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8014FFDC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801508D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150B3C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150DE8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_80150F44);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801511C0);

void func_80151264(u32 arg0) {
    if (arg0 < D_8015EDB4[16].unk3) {
        func_8015131C(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if (D_8015EDB4[16].unk2 >= 0x7CU) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8015131C);

void func_8015163C(s32 arg0, s32 arg1) {
    if (!(arg1 & 1)) {
        D_80127080[arg0].unk0 = 0x40000000;
        return;
    }
    D_80127080[arg0].unk0 = 0x80000000;
}

void func_80151678(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x32;
    D_80122768 = -0x64;
    D_8012276C = 0x70;
    D_8012276D = 0x70;
    D_8012276E = 0x80;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = -0x64;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0x32;
    D_80122788 = -0x64;
    D_8012278C = 0x80;
    D_8012278D = 0x80;
    D_8012278E = 0x90;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_801517AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80149F90", func_8015185C);
