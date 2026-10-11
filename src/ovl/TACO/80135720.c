#include "common.h"
#include "ovl/TACO.h"

void func_80135720(void) {
    D_8015F3F0 = 0;
    D_8015F3F4 = 0;
    D_8015F3F8 = 0;
    D_8015F3FC = 0;
}

s32 func_80135744(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013587C();
        break;
    case 1:
        func_801359B4();
        break;
    case 2:
        func_80135994();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801357B0);

s32 func_8013587C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_800573AC();
        func_80046318(8, 0x80162000, 0xB378);
        func_80135720();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_800460CC() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_800536AC(0x2C0, 0, 0x140, 0xF0);
        func_80135944();
        func_8004284C();
        break;
    }
}

void func_80135944(void) {
    RECT r;

    r.x = 0x2C0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x14;
    func_8009C7F8(&r, 0, 0, 0);
    func_8009C674(0);
}

void func_80135994(void) {
    func_80042908(2);
}

void func_801359B4(void) {
    if (D_8015F3F4 == 1) {
        func_80135F6C();
    } else {
        func_80136988();
    }
    func_800578F4(2);
    func_80135A04();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80135A04);

void func_80135F6C(void) {
    D_8015F3FC += 1;
    switch (D_8015F3F0) {
    case 0:
        func_80136024();
        break;
    case 1:
        func_801361E8();
        break;
    case 2:
        func_80136438();
        break;
    case 3:
        func_80136688();
        break;
    }
    if (D_8015F3FC >= 0x1F) {
        D_8015F3FC = 0;
        D_8015F3F4 = 0;
    }
}

void func_80136024(void) {
    s32 t;

    if (D_8015F3FC < 0xF) {
        t = (0xF - D_8015F3FC << 0xC) / 15;
        switch (D_8015F3F8 % 2) {
        case 0:
            func_801357B0("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            func_801357B0("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    } else {
        t = ((D_8015F3FC << 0xC) - 0xF000) / 15;
        switch (D_8015EDD8 & 1) {
        case 0:
            func_801357B0("OFF", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        case 1:
            func_801357B0("ON", t, 0x1000, 5, 3, 0x2A, -0x10);
            return;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_801361E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136438);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136688);

void func_801368D8(u8 *arg0, u8 arg1) {
    u8 v;

    if (D_8015F3F0 < 4) {
        D_8015F3F4 = 1;
    }
    v = *arg0;
    D_8015F3F8 = v;
    if (D_800E6280.unk_F88 & 0x2020) {
        *arg0 = ((u32)v + arg1 + 1) % (u32)arg1;
    }
    if (D_800E6280.unk_F88 & 0x8000) {
        *arg0 = (*arg0 + arg1 - 1U) % (u32)arg1;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80135720", func_80136988);
