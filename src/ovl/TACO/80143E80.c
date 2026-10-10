#include "common.h"
#include "ovl/TACO.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
TacoCam D_8015F5F0 = { 0, 0x400, 0, 0 };
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F5F8);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F5FC);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F604);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F82C);

void func_80143E80(void) {
    D_8015F5F0.a = 0;
    D_8015F5F0.b = 0x400;
    D_8015F5F0.c = 0;
    D_8015F5F0.d = 0;
}

void func_80143E9C(void) {
    if (D_8015EDB0 != 0) {
        func_80143FF4();
        func_80143F40();
        func_80143B58();
        func_80133694();
    }
    if (D_8015EDB0 & 1) {
        func_80144094();
    }
    if (D_8015EDB0 & 2) {
        func_80144430();
    }
    if (D_8015EDB0 & 4) {
        func_801446D0();
    }
}

s32 func_80143F40(void) {
    D_8015F5F0.c += 0x80;
    if (D_8015F5F0.c >= 0x1001) {
        D_8015F5F0.c -= 0x1000;
        D_8015F5F0.d += 1;
    }
    if (D_8015F5F0.c < 0) {
        D_8015F5F0.c += 0x1000;
        D_8015F5F0.d -= 1;
    }
    if (D_8015F5F0.a >= 0x801) {
        D_8015F5F0.a -= 0x800;
    }
    if (D_8015F5F0.a < 0) {
        D_8015F5F0.a += 0x800;
    }
}

void func_80143FF4(void) {
    D_80122740 = D_8015EDB4->unk74;
    D_80122744 = D_8015EDB4->unk76;
    D_80122748 = D_8015EDB4->unk78;
    D_8012274C = D_8015EDB4->unk74;
    D_80122750 = D_8015EDB4->unk76;
    D_80122754 = 0;
    D_80122758 = D_8015EDB4->unk68 * 0x168;
    D_8012275C = 0;
    func_80099540(&D_80122740);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80143E80", func_80144094);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80143E80", func_80144430);

void func_801446D0(void) {
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80143E80", func_801446D8);
