#include "common.h"
#include "game.h"
#include "ovl/TEL.h"

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_801397D0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_80139920);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_801399CC);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_80139AA4);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_80139E3C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_80139E90);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_80139FEC);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A228);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A2F0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A40C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A478);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A4FC);

void func_8013A76C(void) {
    D_800E66B3 = D_800E66B3 + 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A79C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A820);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A88C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013AA30);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013AC0C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013ADC0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013B54C);

s32 func_8013BDDC(void) {
    s32 i;
    s32 found;

    for (i = 0; i != 0xB; i++) {
        if (((CharFlags *)(D_800E6280 + i * 0x38 + 0x1C8))->b1 &&
            *(s16 *)(D_800E6280 + i * 0x38 + 0x1C6) >= 0x5B) {
            found = i;
            break;
        }
    }
    return found;
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013BE38);

void func_8013BF50(void) {
    func_80042878(0x31);
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013BF70);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013BFDC);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C3E0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C46C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C4D8);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C8DC);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C968);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C9D4);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013CB34);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013CBA0);
