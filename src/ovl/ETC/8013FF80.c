#include "common.h"
#include "ovl/ETC.h"

void func_8013FF80(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8013FFAC();
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_8013FFAC);

void func_801407F0(void) {
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8013FF80();
        break;
    case 1:
        func_8013BF34();
        break;
    case 2:
        func_8013CF2C();
        break;
    case 0xFF:
        func_80140FD8();
        break;
    }
    func_80066334();
    if (D_800E6280.unk_1109 != 0xFF) {
        if (D_801500E0 == 0 && (D_800E6280.unk_F88 & 0x800)) {
            func_8013FC08();
            D_801500D4 = 0x800;
            func_8013FA7C();
            D_801500E0 = 1;
            func_80042908(2);
        }
    }
}
INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_801408F0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140AE4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/8013FF80", func_80140BC4);

s32 func_80140DC8(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 83; i++) {
        if (D_800E6280.unk_56C[i] != 0) {
            n++;
        }
    }
    if (n >= 30) {
        return 1;
    }
    return 0;
}

typedef struct EtcFlagBits {
    u32 pad0 : 1;
    u32 flag : 1;
    u32 rest : 30;
} EtcFlagBits;

typedef struct EtcBits9 {
    u32 pad0 : 9;
    u32 val : 3;
    u32 rest : 20;
} EtcBits9;

s32 func_80140E80(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 0; i < 13; i++) {
        if (((EtcFlagBits *)&D_800E6280.unk_1BC[i].unk_0C)->flag) {
            n++;
        }
    }
    if (((EtcBits9 *)&D_800E6280.unk_1BC[11].unk_0C)->val != 0) {
        n++;
    }
    if (n == 13) {
        return 1;
    }
    return 0;
}
