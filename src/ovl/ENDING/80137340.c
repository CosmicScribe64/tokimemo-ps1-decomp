#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137340);

void func_801374BC(void) {
    if (D_800E6280.unk_1124 != 0) {
        func_80042878(0xC2);
        return;
    }
    func_80042808();
}

void func_801374F8(void) {
    _sprite_set_box_shade_tarao(-0xA0, -0x78, 0x140, 0xF0, 0xE, 0xC0C0FF, 0x4080FF);
    dtd_on(0xE);
    if (D_800E6280.unk_1104.w++ >= 0x81U) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137574);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_8013759C);

void func_801376FC(void) {
    D_80122CE4 = 0;
    D_8013C364 = 0;
    D_8013C360 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137730);

void func_80137AA8(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80137BB4();
        return;
    case 1:
        func_80137E40();
        return;
    case 2:
        func_80137F64();
        return;
    case 3:
        func_801380B0();
        return;
    case 4:
        func_80138278();
        return;
    case 5:
        func_8013855C();
        return;
    case 6:
        func_801387B0();
        return;
    case 7:
        func_80138A9C();
        return;
    case 8:
        func_80138C70();
        return;
    case 9:
        func_80138DC8();
        return;
    case 10:
        func_80138FC8();
        return;
    case 12:
        func_801391E4();
        return;
    default:
        func_80139498();
        return;
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137BB4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137E40);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80137F64);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801380B0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138278);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_8013855C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801387B0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138A9C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138C70);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138DC8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80138FC8);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_801391E4);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80137340", func_80139498);

void func_801396A4(s16 arg0, u8 arg1) {
    D_8011F4DE = arg0;
    D_8011F4E0 = 0;
    D_8011F4D0 = 0;
    if (arg1 != 0xFF) {
        D_8011F4CA = arg1;
    }
}
