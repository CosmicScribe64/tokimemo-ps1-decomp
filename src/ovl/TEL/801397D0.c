#include "common.h"
#include "ovl/TEL.h"

/* Flag word at +0x1C8 of the 0x38-byte entries at GameState +0x1BC (Rec38.unk_0C), tested as bit-fields (sll; bgez in the original). */
typedef struct {
    u32 b0 : 1;
    u32 b1 : 1;
    u32 pad : 10;
    u32 b12 : 1;
    u32 b13 : 1;
    u32 rest : 18;
} CharFlagsTel; /* size 4 */

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_801397D0);

/* Flag word at +0x54C of the 4-byte entries at GameState +0x54C (GsWord unk_54C[8]): nibble in bits 25..28, flag in bit 29. */
typedef struct {
    u32 pad : 25;
    u32 nib : 4;
    u32 b29 : 1;
    u32 rest : 2;
} TelEntryFlags; /* size 4 */

void func_80139920(void) {
    s32 i;
    u32 sel;

    sel = ((TelEntryFlags *)&D_800E6280.unk_54C[0].w)->nib;
    for (i = 0; i != 8; i++) {
        if (((TelEntryFlags *)&D_800E6280.unk_54C[i])->b29 && sel == ((TelEntryFlags *)&D_800E6280.unk_54C[i])->nib) {
            D_800E6280.unk_54C[i].b[3] &= 0xFFDF;
            func_800AE0B0("canceled %d \n", i);
        }
    }
    func_8006B0C8();
}

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
    D_800E6280.unk_1BC[11].unk_0C.b[3] = D_800E6280.unk_1BC[11].unk_0C.b[3] + 1;
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
        if (((CharFlagsTel *)&D_800E6280.unk_1BC[i].unk_0C)->b1 &&
            D_800E6280.unk_1BC[i].unk_0A >= 0x5B) {
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

s32 func_8013C3E0(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (((CharFlagsTel *)&D_800E6280.unk_1BC[i].unk_0C)->b1 && ((CharFlagsTel *)&D_800E6280.unk_1BC[i].unk_0C)->b12 &&
            func_80051A68(i) != 4) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C46C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C4D8);

s32 func_8013C8DC(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (((CharFlagsTel *)&D_800E6280.unk_1BC[i].unk_0C)->b1 && ((CharFlagsTel *)&D_800E6280.unk_1BC[i].unk_0C)->b13 &&
            func_80051A68(i) != 4) {
            return i;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C968);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C9D4);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013CB34);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013CBA0);
