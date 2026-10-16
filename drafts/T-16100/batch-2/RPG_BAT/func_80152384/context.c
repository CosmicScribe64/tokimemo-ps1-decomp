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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80151FC0", func_80152080);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EFA0;
extern s32 D_8015EFB0;
extern s32 D_8015EFC0;
extern s32 D_8015EFD0;
s32 func_800AE100();
extern s32 sp;
/* end bulk_m2c declarations */
void func_80152384(void) {
    s32 sp64[4];
    s32 spA0[4];
    s32 sp90[4];
    s32 sp80[4];
    s32 *var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s3;
    u8 *temp_s0;

    (*(s32 *)&spA0) = (s32) D_8015EFA0;
    *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (4));
    *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (12));
    *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFA0) + (8));
    (*(s32 *)&sp90) = (s32) D_8015EFB0;
    *(s32 *)((unsigned char *)(&sp90) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (4));
    *(s32 *)((unsigned char *)(&sp90) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (12));
    *(s32 *)((unsigned char *)(&sp90) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFB0) + (8));
    (*(s32 *)&sp80) = (s32) D_8015EFC0;
    *(s32 *)((unsigned char *)(&sp80) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (4));
    *(s32 *)((unsigned char *)(&sp80) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (12));
    *(s32 *)((unsigned char *)(&sp80) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFC0) + (8));
    sp64[0] = D_8015EFD0;
    var_s3 = 0;
    *(s32 *)((unsigned char *)(&sp64[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (4));
    *(s32 *)((unsigned char *)(&sp64[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (8));
    *(s32 *)((unsigned char *)(&sp64[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015EFD0) + (12));
    var_s1 = 2;
    if (func_8013E9A8(5) == 0) {
        var_s1 = 3;
    }
    var_s0 = 0;
    if (var_s1 > 0) {
        do {
            var_s0 += 1;
            (&sp64[0])[func_8013E9A8(4)] = 1;
        } while (var_s0 != var_s1);
    }
    var_s1_2 = 0;
    var_s2 = &sp64[0];
    do {
        temp_s0 = D_8015EB70 + (var_s3 * 4);
        if (*var_s2 == 1) {
            func_800AE0F0(*(void **)((unsigned char *)temp_s0 + (16)), "おたから：");
            switch (D_8015EB98) {
            case 0x10000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), spA0[var_s1_2 / 4]);
                break;
            case 0x20000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), sp90[var_s1_2 / 4]);
                break;
            case 0x40000:
                func_800AE100(*(void **)((unsigned char *)temp_s0 + (16)), sp80[var_s1_2 / 4]);
                break;
            }
            var_s3 += 1;
        }
        var_s2 += 1;
        var_s1_2 += 4;
    } while (var_s2 != &sp64[4]);
    D_8015ED14 = var_s3 + 4;
}
