#include "common.h"
#include "ovl/BUNKASAI.h"

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_80142700);

void func_80142B98(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80142BC8(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

void func_80142BE8(void) {
    s32 i;
    u32 bonus;
    u8 amount;
    BunkasaiColumns sel;
    BunkasaiGirlColumns first;
    BunkasaiGirlColumns second;
    BunkasaiGirlColumns third;
    u32 col;

    sel = D_8016052C;
    first = D_80160534;
    second = D_80160584;
    third = D_801605D4;
    col = D_800E6280.unk_0F4.h >> 12;
    bonus = ((&D_800E6280.unk_0FC)[sel.v[col]].unk_02 / 20 +
             (u32) (D_800E6280.unk_66C[col].unk_00 >> 3) / (u32) (D_800E6280.unk_03E - 94)) & 0xFF;
    if (bonus >= 11) {
        bonus = 10;
    }
    switch ((D_800E6280.unk_03E - 95) & 0xFF) {
    case 0:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(first.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(first.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = first.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    case 1:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(second.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(second.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = second.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    case 2:
        D_800E6280.unk_1BC[0].unk_02 += bonus + (u8)(third.v[0][col] >> 1);
        for (i = 1; i < 13; i++) {
            D_800E6280.unk_1BC[i].unk_02 += bonus;
            D_800E6280.unk_1BC[i].unk_02 += (u8)(third.v[i][D_800E6280.unk_0F4.h >> 12] >> 1);
        }
        amount = third.v[D_80122EBC][D_800E6280.unk_0F4.h >> 12];
        break;
    }
    if (amount != 0) {
        amount--;
    }
    D_800E6280.unk_1BC[D_80122EBC].unk_06 += 2;
    D_800E6280.unk_1BC[D_80122EBC].unk_02 += amount;
}

void func_801431D4(void) {
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

INCLUDE_RODATA("asm/ovl/BUNKASAI/data/BUNKASAI/80142700.rodata", D_8015D774);

void func_8014338C(void) {
    k_reset(0);
    func_8006612C(&D_8015D774);
    k_disp_start(1);
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI/80142700", func_801433C0);
