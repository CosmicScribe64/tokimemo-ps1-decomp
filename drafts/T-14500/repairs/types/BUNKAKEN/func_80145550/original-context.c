#include "common.h"
#include "ovl/BUNKAKEN.h"

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80144F70);

void func_80145500(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_800E643E;
extern s16 D_800E6442;
extern s32 D_80155F9C;
extern s32 D_80155FD4;
extern s32 D_80156018;
extern u8 D_800E62BE;
/* end bulk_m2c declarations */
void func_80145550(void) {
    typedef struct { s32 w[13]; } M2cBlock34; /* m2c memcpy of 52 bytes */
    typedef struct { s32 w[16]; } M2cBlock40; /* m2c memcpy of 64 bytes */
    s32 spA4[13];
    s32 sp60[16];
    s32 sp1C[16];
    s32 temp_t8;
    u8 var_v0;
    u8 var_v0_2;

    *(M2cBlock34 *)(&spA4) = *(M2cBlock34 *)(&D_80155F9C);
    *(u16 *)((unsigned char *)(&spA4 + 0x30) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80155F9C + 0x30) + (4));
    *(M2cBlock40 *)(&sp60) = *(M2cBlock40 *)(&D_80155FD4);
    *(u16 *)((unsigned char *)(&sp60 + 0x3C) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80155FD4 + 0x3C) + (4));
    *(M2cBlock40 *)(&sp1C) = *(M2cBlock40 *)(&D_80156018);
    *(u16 *)((unsigned char *)(&sp1C + 0x3C) + (4)) = (u16) *(u16 *)((unsigned char *)(&D_80156018 + 0x3C) + (4));
    temp_t8 = (D_800E62BE - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        D_800E6442 += 2;
        switch (temp_t8) {
        default:
            var_v0 = spDA;
            break;
        case 0:
            var_v0 = *(&spA4 + D_80122EB8);
            break;
        case 1:
            var_v0 = *(&sp60 + D_80122EB8);
            break;
        case 2:
            var_v0 = *(&sp1C + D_80122EB8);
            break;
        }
        if (var_v0 != 0) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        D_800E643E += var_v0;
    } else if (D_80122EBC < 0xB) {
        D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0_2 = spDA;
            break;
        case 0:
            var_v0_2 = *(&spA4 + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 1:
            var_v0_2 = *(&sp60 + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 2:
            var_v0_2 = *(&sp1C + (D_80122EBC * 6) + D_80122EB8);
            break;
        }
        if (var_v0_2 != 0) {
            var_v0_2 = (var_v0_2 - 1) & 0xFF;
        }
        D_800E6280.unk_1BC[D_80122EBC].unk_02 += var_v0_2;
    }
    if (D_80122EA4 != 0xFF) {
        D_800E6280.unk_1BC[D_80122EA4].unk_06 -= 0xA;
        D_800E6280.unk_1BC[D_80122EA4].unk_0A += 0xA;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145844);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    func_8006612C(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    func_8006612C(&D_80154B8C);
    func_80065F34(0);
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145A64);
