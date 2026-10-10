#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8013F4F0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "紐緒「真・世界征服ロボ！！");
        func_8014B738(D_8015E8F0, 1, 0xF0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A, 0x05000026, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value, as func_8013F4F0 (T-8080) */
s32 func_8013F5C8(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　　服従ドリルパンチ　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xC8);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C000020, 0x05000029, 1);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F694(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　独裁ミサイルシャワー　　　　");
            func_8014B738(D_8015E8F0, 1, 0x118);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1B000020, 0x0300002F, 2);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F760(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　世界征服メガビーム　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xAA);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C00002A, 0x06000036, 0);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_8013F82C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_8013FD8C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_8013FF28);

void func_801401D0(s32 i) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x1AB6) += 1;
    *(s16 *)(p + 0x1ABA) -= 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140204);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140A58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140CC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140E70);
