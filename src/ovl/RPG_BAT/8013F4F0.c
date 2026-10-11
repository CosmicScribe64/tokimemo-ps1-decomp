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

void func_8013FD8C(void) {
    switch (D_8015EDD8) {
    case 0:
        func_801447E0();
        D_80121311 = 6;
        func_8013E54C(4);
        func_8014EF3C();
        break;
    case 1:
        func_8013F5C8();
        func_8013FF28();
        break;
    case 2:
        func_8014F080(0x3C);
        break;
    case 3:
        func_8013F220();
        func_8014EF3C();
        break;
    case 4:
        func_8013F694();
        func_80140204();
        break;
    case 5:
        func_8013F220();
        func_8014EF3C();
        break;
    case 6:
        func_8013F760();
        func_80140A58();
        break;
    case 7:
        func_8013F220();
        func_8014EF3C();
        break;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        func_8014EF3C();
        break;
    case 9:
        func_8014F080(0x1E);
        break;
    case 10:
        func_8013D030();
    case 11:
        func_8014F080(0x14);
        break;
    case 12:
        if (D_8015ED98 & 0x1000) {
            D_8015ED98 = D_8015ED98 & ~0x1000;
            D_8015ED9C = 0xFFFF;
        }
        func_8013E54C(0xFF);
        func_8014EF8C();
        break;
    }
}

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
