#include "common.h"
#include "ovl/KANGEI.h"

void func_80132E40(void) {
    D_80139A50[0] = 0x801C1400;
    D_80139A50[1] = 0x801C1D6C;
    D_80139A50[2] = 0x801CA47C;
}

void func_80132E74(void) {
    D_80139A5C[0][0] = 0x80199CD4;
    D_80139A5C[0][1] = 0x8019EE30;
    D_80139A5C[0][2] = 0x801A422C;
    D_80139A5C[0][3] = 0x801A82AC;
    D_80139A5C[0][4] = 0x801AB394;
    D_80139A5C[0][5] = 0x801AF5FC;
    D_80139A5C[0][6] = 0x801B1C9C;
    D_80139A5C[0][7] = 0x801B3370;
    D_80139A5C[1][0] = 0x80199EF4;
    D_80139A5C[1][1] = 0x8019F078;
    D_80139A5C[1][2] = 0x801A447C;
    D_80139A5C[1][3] = 0x801A8408;
    D_80139A5C[1][4] = 0x801AB4E0;
    D_80139A5C[1][5] = 0x801AF824;
    D_80139A5C[1][6] = 0x801B1D10;
    D_80139A5C[1][7] = 0x801B3478;
    D_80139A5C[2][0] = 0x8019BC24;
    D_80139A5C[2][1] = 0x801A0FD8;
    D_80139A5C[2][2] = 0x801A63F8;
    D_80139A5C[2][3] = 0x801A96B8;
    D_80139A5C[2][4] = 0x801AC800;
    D_80139A5C[2][5] = 0x801B17AC;
    D_80139A5C[2][6] = 0x801B2090;
    D_80139A5C[2][7] = 0x801B41F4;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80132FF8);

void func_801335A0(void) {
    D_800E6280.unk_71C += 1;
    D_80139AC4 = 0;
    D_80139AC8 = 0;
    func_80072338();
    hizuke_init();
    func_80042808();
}

void func_801335F0(void) {
    s16 var_s0;

    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    D_80139ABC = 0;
    D_80139AC0 = 0;
    D_80139AC4 = 0;
    func_80133D28();
    D_800E6280.unk_040 -= 1;
    func_800649D4();
    D_800E6280.unk_040 += 1;
    func_8006509C();
    for (var_s0 = 0; var_s0 < 15; var_s0++) {
        func_80065048(var_s0, 0);
    }
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    /* FAKE: dead byte test keeps the packed-record address in v0 (T-9580). */
    if (D_800E6280.unk_03A == 0) {}
    ((KangeiStateView *)&D_800E6280)->modes[(u16)D_800E6280.unk_0F4.h >> 12].mode++;
    D_800B593C = 0x80;
    func_80042808();
}

void func_80133734(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC06F, 0xC065, 0xCC33, 0xCBE3, 0xCBD9);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_80044750(0x22);
    func_80044750(0x21);
    func_80044750(0x2F);
    func_8004284C();
}

void func_801337B4(void) {
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80133834(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801338B4(void) {
    if (D_800E6280.unk_1104.w == 0) {
        func_8004500C(0, 0);
    }
    func_8004284C();
}

s32 func_801338EC(void) {
    s32 pad; /* FAKE: unused local puts rect at the original offset; real source unknown. T-9320 */
    RECT rect;

    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_800674B0();
        *(u8 *)&D_800E6280.unk_110D += 1;
    }
    if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            *(u8 *)&D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            *(u8 *)&D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    func_800573AC();
    rect.x = 0x140;
    rect.y = 0x100;
    rect.w = 0x140;
    rect.h = 0xF8;
    func_8009C7F8(&rect, 8, 0, 0);
    rect.x = 0x2C0;
    rect.y = 0;
    func_8009C7F8(&rect, 8, 0, 0);
    func_80072338();
    return func_80072B5C(1);
}

void func_80133A14(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        func_80133A54();
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132E40", func_80133A54);
