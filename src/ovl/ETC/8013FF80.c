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
void func_801408F0(void) {
    D_800E6280.unk_1228[10] = 0;
    D_800E6280.unk_1228[11] = 0;
    D_800E6280.unk_1228[12] = 0;
    D_800E6280.unk_1228[13] = 0;
    D_800E6280.unk_1228[14] = 0;
    D_800E6280.unk_1228[42] = 0;
    D_800E6280.unk_1228[43] = 0;
    D_800E6280.unk_1228[44] = 0;
    D_800E6280.unk_1228[45] = 0;
    D_800E6280.unk_1228[46] = 0;
    D_800E6280.unk_1228[0] = 0;
    D_800E6280.unk_1228[1] = 0;
    D_800E6280.unk_1228[2] = 0;
    D_800E6280.unk_1228[3] = 0;
    D_800E6280.unk_1228[4] = 0;
    D_800E6280.unk_1228[32] = 0;
    D_800E6280.unk_1228[33] = 0;
    D_800E6280.unk_1228[34] = 0;
    D_800E6280.unk_1228[35] = 0;
    D_800E6280.unk_1228[36] = 0;
    D_800E6280.unk_1228[16] = 0;
    D_800E6280.unk_1228[17] = 0;
    D_800E6280.unk_1228[18] = 0;
    D_800E6280.unk_1228[19] = 0;
    D_800E6280.unk_1228[20] = 0;
    D_800E6280.unk_1228[48] = 0;
    D_800E6280.unk_1228[49] = 0;
    D_800E6280.unk_1228[50] = 0;
    D_800E6280.unk_1228[51] = 0;
    D_800E6280.unk_1228[52] = 0;
    D_800E6280.unk_1228[5] = -1;
    D_800E6280.unk_1228[6] = -1;
    D_800E6280.unk_1228[7] = -1;
    D_800E6280.unk_1228[8] = -1;
    D_800E6280.unk_1228[9] = -1;
    D_800E6280.unk_1228[37] = -1;
    D_800E6280.unk_1228[38] = -1;
    D_800E6280.unk_1228[39] = -1;
    D_800E6280.unk_1228[40] = -1;
    D_800E6280.unk_1228[41] = -1;
    D_800E6280.unk_1228[21] = -1;
    D_800E6280.unk_1228[22] = -1;
    D_800E6280.unk_1228[23] = -1;
    D_800E6280.unk_1228[24] = -1;
    D_800E6280.unk_1228[25] = -1;
    D_800E6280.unk_1228[53] = -1;
    D_800E6280.unk_1228[54] = -1;
    D_800E6280.unk_1228[55] = -1;
    D_800E6280.unk_1228[56] = -1;
    D_800E6280.unk_1228[57] = -1;
    if ((func_8013F3F8() == 1) && (D_80150060 == 0xFF)) {
        D_800E6280.unk_1228[7] = 0;
    }
}

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
