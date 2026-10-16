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

void func_80145550(void) {
    typedef struct ByteTable66 { /* 0x00 */ u8 b[66]; } ByteTable66; /* size 0x42 */
    typedef struct ByteTable54 { /* 0x00 */ u8 b[54]; } ByteTable54; /* size 0x36 */
    BunkakenBytes9x6 spA4;
    BunkakenBytes11x6 sp60;
    BunkakenBytes11x6 sp1C;
    u8 spDA; /* Original default arm reads an uninitialized stack byte. */
    s32 temp_t8;
    u8 var_v0;
    u8 var_v0_2;

    spA4 = D_80155F9C;
    sp60 = D_80155FD4;
    sp1C = D_80156018;
    temp_t8 = (D_800E6280.unk_03E - 0x5F) & 0xFF;
    if (D_80122EBC == 0xB) {
        D_800E6280.unk_1BC[0].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0 = spDA;
            break;
        case 0:
            var_v0 = *(spA4.b + D_80122EB8);
            break;
        case 1:
            var_v0 = *(sp60.b + D_80122EB8);
            break;
        case 2:
            var_v0 = *(sp1C.b + D_80122EB8);
            break;
        }
        if (var_v0 != 0) {
            var_v0 = (var_v0 - 1) & 0xFF;
        }
        D_800E6280.unk_1BC[0].unk_02 += var_v0;
    } else if (D_80122EBC < 0xB) {
        D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
        switch (temp_t8) {
        default:
            var_v0_2 = spDA;
            break;
        case 0:
            var_v0_2 = *(spA4.b + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 1:
            var_v0_2 = *(sp60.b + (D_80122EBC * 6) + D_80122EB8);
            break;
        case 2:
            var_v0_2 = *(sp1C.b + (D_80122EBC * 6) + D_80122EB8);
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

void func_80145844(void) {
    k_reset(0);
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "詩集展示");
            break;
        case 0x60:
            func_8006612C((s32) "文集展示");
            break;
        case 0x61:
            func_8006612C((s32) "弁論大会");
            break;
        }
        break;
    case 1:
        func_8006612C((s32) "演劇部公演");
        break;
    case 2:
        func_8006612C((s32) "実験ショー");
        break;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "パソコン占い");
            break;
        case 0x60:
            func_8006612C((s32) "ゲーム");
            break;
        case 0x61:
            func_8006612C((s32) "実験ショー");
            break;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "似顔絵");
            break;
        case 0x60:
            func_8006612C((s32) "公開デッサン");
            break;
        case 0x61:
            func_8006612C((s32) "油絵展示");
            break;
        }
        break;
    case 5:
        func_8006612C((s32) "演奏会");
        break;
    }
    k_disp_start(1);
}

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
