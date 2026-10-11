#include "common.h"
#include "ovl/TACO.h"

void func_80148FE0(void) {
    if (((u32) D_800E6280.unk_1100 < 0x60U) && (D_800E6280.unk_10F8 & 0x10)) {
        func_801432F0("2ST BOSS", -0x30, 0, 2);
    }
    switch (D_800E6280.unk_110A) {
    case 0:
        func_801490B4();
        break;
    case 1:
        func_8014927C();
        break;
    case 2:
        func_801493D4();
        break;
    case 3:
        func_80149538();
        break;
    }
    func_8013F250();
    func_80132FE8();
    func_80133374();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801490B4);

void func_8014927C(void) {
    s32 i;

    func_80151D1C(3);
    func_8015266C(6);
    func_80152808(4);
    func_801532C4();
    for (i = 0; i < 0xC; i++) {
        func_801497EC(D_8015FCA8[i], i, D_8015FCD8, D_8015FCB4, D_8015FCC0);
    }
    if (D_800E6280.unk_1104.u % 240 == 0 && func_800AE0D0() % 3 == 0) {
        func_801335A0(0, 0x500);
    }
    if (D_800E6280.unk_1104.u < 0x12C) {
        func_80146F74(0, 0, -2, -0x28, 0, 0, 0, 0x3C, 0x3C);
    } else {
        func_8004284C();
    }
    D_800E6280.unk_1104.u++;
}

void func_801493D4(void) {
    s32 i;

    for (i = 0; i != 0xC; i++) {
        func_801497EC(D_8015FCA8[i], i, D_8015FCD8, D_8015FCB4, D_8015FCC0);
    }
    func_80149D1C();
    func_80151C30();
    if (D_800E6280.unk_1104.u % 240 == 0) {
        if (func_800AE0D0() % 3 == 0) {
            func_801335A0(0, 0x500);
        }
    }
    if (func_80147B24(0xC, D_8015FCC0) >= 0x55) {
        if (*((u8 *) D_8015EDB4 + 0x905) == 0 && *((u8 *) D_8015EDB4 + 0x98D) == 0 && *((u8 *) D_8015EDB4 + 0x905) == 0) {
            func_80154230(0x80196000, 9);
            D_8015EDCC += 1;
            func_8004284C();
        }
    }
    D_800E6280.unk_1104.u++;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149538);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801497EC);

void func_80149B7C(void) {
    s32 i;

    for (i = 0x10; i < 0x3F; i++) {
        func_80143B34(i);
    }
    for (i = 0; i < 0xC; i++) {
        func_80147928(D_8015FCA8[i], i, 0x80196000, D_8015FCB4, D_8015FCC0, 1);
    }
    *((u8 *) D_8015EDB4 + 0x906) = 0;
    *((u8 *) D_8015EDB4 + 0x883) = 0;
    *((u8 *) D_8015EDB4 + 0x882) = 0;
    *((u8 *) D_8015EDB4 + 0x98E) = 0;
    *((u8 *) D_8015EDB4 + 0x90B) = 0;
    *((u8 *) D_8015EDB4 + 0x90A) = 0;
    *((u8 *) D_8015EDB4 + 0xA16) = 1;
    *((u8 *) D_8015EDB4 + 0x993) = 0;
    *((u8 *) D_8015EDB4 + 0x992) = 0;
    *((u8 *) D_8015EDB4 + 0xA9E) = 1;
    *((u8 *) D_8015EDB4 + 0xA1B) = 0;
    *((u8 *) D_8015EDB4 + 0xA1A) = 0;
    *((u8 *) D_8015EDB4 + 0xB26) = 1;
    *((u8 *) D_8015EDB4 + 0xAA3) = 0;
    *((u8 *) D_8015EDB4 + 0xAA2) = 0;
    *((u8 *) D_8015EDB4 + 0xB2B) = 0;
    *((u8 *) D_8015EDB4 + 0xB2A) = 0;
    *((u8 *) D_8015EDB4 + 0xBB3) = 0;
    *((u8 *) D_8015EDB4 + 0xBB2) = 0;
}

void func_80149D1C(void) {
    s32 v;
    u8 b;

    v = func_80147B24(0xC, D_8015FCC0);
    if (*((u8 *) D_8015EDB4 + 0xBB2) < v) {
        D_80122760 = 0;
        D_80122764 = 0;
        D_80122768 = -0x64;
        D_8012276C = 0xFF;
        D_8012276D = 0x80;
        D_8012276E = 0x80;
        func_8009AD70(0, &D_80122760);
        D_80122770 = 0;
        D_80122774 = 0;
        D_80122778 = -0x64;
        D_8012277C = 0xFF;
        D_8012277D = 0x80;
        D_8012277E = 0x80;
        func_8009AD70(1, &D_80122770);
        D_80122780 = 0;
        D_80122784 = 0;
        D_80122788 = -0x64;
        D_8012278C = 0xFF;
        D_8012278D = 0x80;
        D_8012278E = 0x80;
        func_8009AD70(2, &D_80122780);
        b = *((u8 *) D_8015EDB4 + 0xBB3);
        if (b >= 3) {
            *((u8 *) D_8015EDB4 + 0xBB2) = v;
            D_80122760 = 0x14;
            D_80122764 = 0x64;
            D_80122768 = -0x64;
            D_8012276C = 0xB0;
            D_8012276D = 0xB0;
            D_8012276E = 0xB0;
            func_8009AD70(0, &D_80122760);
            D_80122770 = 0x14;
            D_80122774 = -0x64;
            D_80122778 = 0x64;
            D_8012277C = 0x80;
            D_8012277D = 0x80;
            D_8012277E = 0x80;
            func_8009AD70(1, &D_80122770);
            D_80122780 = -0x14;
            D_80122784 = 0x14;
            D_80122788 = -0x64;
            D_8012278C = 0x60;
            D_8012278D = 0x60;
            D_8012278E = 0x60;
            func_8009AD70(2, &D_80122780);
            *((u8 *) D_8015EDB4 + 0xBB3) = 0;
            return;
        }
        *((u8 *) D_8015EDB4 + 0xBB3) = b + 1;
    }
}
