#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80151FC0(void) {
    if (D_8015EB98 == 0x40000) {
        func_8013E7C0(0x38, 3, 5, 1);
    } else {
        func_8013E7C0(0x38, 4, 5, 1);
    }
    switch (D_8015EB98) {
    case 0x10000:
        func_8013F15C(0x505, 1, 0);
        return;
    case 0x20000:
        func_8013F15C(0x506, 1, 0);
        return;
    case 0x40000:
        func_8013F15C(0x505, 1, 0);
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6354;
extern s8 D_8012163C;
/* end bulk_m2c declarations */
void func_80152080(void) {
    s32 sp34;
    s32 temp_v0;
    s32 temp_v1;

    switch (D_8015EDD8) {
    case 0:
        func_801447E0();
        switch (D_8015EB98) {
        case 0x10000:
            func_800AE0F0(D_8015E828, "　　　　　テール・パンチ　　　　　");
            break;
        case 0x20000:
            func_800AE0F0(D_8015E828, "　　　　　暴れん坊頭突き　　　　　");
            break;
        case 0x40000:
            func_800AE0F0(D_8015E828, "　　　　クラッシュ・クロー　　　　");
            break;
        }
        func_8014D11C((s32) D_8015E850, (s32) &D_800E6354, (s32) "は", D_8015EBB0, (s32) "のダメージを受けた");
        func_8014B738((s32) D_8015E8F0, 2, 0x3C);
        func_8014EF3C();
    default:
        break;
    case 1:
        D_8015EE48 = 3;
        temp_v0 = D_8015EDE0 + 1;
        if ((temp_v0 % 3) == 0) {
            D_8012163C = 0xFF;
        } else {
            D_8012163C = 0;
        }
        D_8015EDE0 = temp_v0;
        if (temp_v0 >= 0x10) {
            func_8014EF3C();
        }
        break;
    case 2:
        if (!(D_8015ED98 & 0x100)) {
            func_8013E7C0(0x30, 5, 1, 1);
        }
        D_80121531 = 3;
        func_80151FC0();
        D_8015EC74 = func_8013E9A8(4) + 1;
        sp34 = func_8013E9A8(0x14);
        func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
        func_8014EF3C();
        break;
    case 3:
        if (func_8013E90C(0x38) != 0) {
            func_80151FC0();
            sp34 = func_8013E9A8(0x14);
            func_8013E97C(0x38, sp34 - 5, func_8013E9A8(0x14) - 5);
            temp_v1 = D_8015EC74 - 1;
            D_8015EC74 = temp_v1;
            if (temp_v1 == 0) {
                func_8014EF3C();
            }
        }
        break;
    case 4:
        if (func_8013E90C(0x38) != 0) {
            func_8013E7C0(0x38, 0xFF, 5, 0);
            func_8014EF3C();
            D_8015EE48 = 1;
        }
        break;
    case 5:
        func_8014F178(1);
        break;
    case 6:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        func_8014EF3C();
        break;
    case 7:
        func_8014F178(0);
        break;
    case 8:
        func_8014F080(0x14);
        break;
    case 9:
        func_8013D030();
        func_8014EF8C();
        break;
    }
}

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7D4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7FC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D808);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D810);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D820);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D828);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D834);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D840);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D84C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80151FC0", func_80152384);
