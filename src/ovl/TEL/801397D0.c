#include "common.h"
#include "ovl/TEL.h"

void func_801397D0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_1109) {
    case 0:
        func_801399CC();
        break;
    case 1:
        func_8013A228();
        break;
    case 2:
        func_8013A40C();
        break;
    case 3:
        func_8013A478();
        break;
    case 4:
        func_8013A79C();
        break;
    case 5:
        func_8013A820();
        break;
    case 6:
        func_8013BF70();
        break;
    case 7:
        func_8013C46C();
        break;
    case 8:
        func_8013C968();
        break;
    case 9:
        func_8013CB34();
        break;
    }
    func_80064DEC();
    func_80064F48();
    func_800646CC();
    func_80065B0C(0);
    func_8006BA40();
    func_80066334();
    func_800578F4(0);
    func_80066C08(0);
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
}

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

s32 func_8013A40C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AA30();
        break;
    case 1:
        func_8013B54C();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

s32 func_8013A478(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013A4FC();
        break;
    case 2:
        func_8013A76C();
        break;
    case 3:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A4FC);

void func_8013A76C(void) {
    D_800E6280.unk_1BC[11].unk_0C.b[3] = D_800E6280.unk_1BC[11].unk_0C.b[3] + 1;
    func_8004284C();
}

s32 func_8013A79C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AA30();
        break;
    case 1:
        func_8013ADC0();
        break;
    case 2:
        func_8013BE38();
        break;
    case 3:
        func_8013BF50();
        break;
    }
}

s32 func_8013A820(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013A88C();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013A88C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013AA30);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013AC0C);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013ADC0);

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013B54C);

s32 func_8013BDDC(void) {
    s32 i;
    s32 found;

    for (i = 0; i != 0xB; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 &&
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

s32 func_8013BF70(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013BFDC();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013BFDC);

s32 func_8013C3E0(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0C.f.b12 &&
            func_80051A68(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 func_8013C46C(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013C4D8();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C4D8);

s32 func_8013C8DC(void) {
    s32 i;

    for (i = 0; i != 0xB; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 && D_800E6280.unk_1BC[i].unk_0C.f.b13 &&
            func_80051A68(i) != 4) {
            return i;
        }
    }
    return 0;
}

s32 func_8013C968(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013C9D4();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013C9D4);

s32 func_8013CB34(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013AC0C();
        break;
    case 1:
        func_8013CBA0();
        break;
    case 2:
        func_8013BF50();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/801397D0", func_8013CBA0);
