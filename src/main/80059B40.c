#include "common.h"
#include "game.h"

void func_80059B40(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80059E00();
        return;
    case 1:
        func_80059BC0();
        return;
    case 2:
        func_8004ADE4();
        func_8004284C();
        return;
    default:
        func_80042808();
        return;
    }
}

void func_80059BC0(void) {
    func_80059BE8();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059BE8);

/* FAKE: u8 views give the original's global access (LOD): `andi`/`or v0` copy of the increment; the s32 return keeps that copy live (T-5010) */
s32 func_80059E00(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        printf("LDVI");
        back_clear_switch(1);
        func_80041168(0xFF);
        func_80046318(0x35, 0x80180000, 0x9D85);
        *(u8 *)&D_800E6280.unk_110D += 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if (func_800460CC() & 1) {
            func_80059EF0();
            func_800462C8(8, 0x801F0000, 0x9C97);
            *(u8 *)&D_800E6280.unk_110D += 1;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 2 && (func_800460CC() & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059EF0);

void func_8005A06C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80059B40();
        return;
    }
    func_80042878(0x12);
}
