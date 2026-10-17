#define MAIN_API_OVERRIDE_func_800AE0F0 /* matched with strcpy returning its destination: with the void prototype the switch on D_8015EB98 in func_80152384 takes v0, not v1 (main_api.h: void). T-17101 */
#include "common.h"
#include "ovl/RPG_BAT.h"

void *func_800AE0F0(void *dst, void *src);

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

void func_80152080(void) {
    /* FAKE: two unused locals reserve the 8 bytes of frame above the sp34 spill; real source unknown. T-17101 */
    s32 home0;
    s32 home1;
    s32 sp34;

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
        func_8014D11C(D_8015E850, D_800E6280.unk_0D4, "は", D_8015EBB0, "のダメージを受けた");
        func_8014B738(D_8015E8F0, 2, 0x3C);
        func_8014EF3C();
    default:
        break;
    case 1:
        D_8015EE48 = 3;
        D_8015EDE0 += 1;
        if ((D_8015EDE0 % 3) == 0) {
            ((DisplayActor44 *)D_8011ECD0)[155].unk_40 = 0xFF;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[155].unk_40 = 0;
        }
        if (D_8015EDE0 >= 0x10) {
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
            if (--D_8015EC74 == 0) {
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

void func_80152384(void) {
    RpgStrTbl4 spA0;
    RpgStrTbl4 sp90;
    RpgStrTbl4 sp80;
    s32 i;
    s32 n;
    s32 count;
    RpgWordTbl4 sp64;

    spA0 = D_8015EFA0;
    sp90 = D_8015EFB0;
    sp80 = D_8015EFC0;
    sp64 = D_8015EFD0;
    count = 0;
    if (func_8013E9A8(5) == 0) {
        n = 3;
    } else {
        n = 2;
    }
    i = 0;
    if (n > 0) {
        do {
            sp64.v[func_8013E9A8(4)] = 1;
            i += 1;
        } while (i != n);
    }
    for (i = 0; i < 4; i++) {
        if (sp64.v[i] == 1) {
            func_800AE0F0(((u8 **)D_8015EB70)[count + 4], "おたから：");
            switch (D_8015EB98) {
            case 0x10000:
                func_800AE100(((u8 **)D_8015EB70)[count + 4], spA0.s[i]);
                break;
            case 0x20000:
                func_800AE100(((u8 **)D_8015EB70)[count + 4], sp90.s[i]);
                break;
            case 0x40000:
                func_800AE100(((u8 **)D_8015EB70)[count + 4], sp80.s[i]);
                break;
            }
            count += 1;
        }
    }
    D_8015ED14 = count + 4;
}
