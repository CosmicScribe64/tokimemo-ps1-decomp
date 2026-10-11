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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_801493D4);

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80148FE0", func_80149D1C);
