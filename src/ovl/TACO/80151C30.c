#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80151C30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80151D1C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80151F94);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152110);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801521EC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801522B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801523D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_8015266C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152764);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152808);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152944);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152C60);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80152DB4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80153068);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801531E8);

void func_801532C4(void) {
    u8 temp;

    temp = D_8015EDB4[18].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0, 4);
    } else if (temp == 2) {
        func_80151F94(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0, 2);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[18].unk3, &D_8015EDB4[18].unk2, 1, 0);
    }
    temp = D_8015EDB4[19].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7, 3);
    } else if (temp == 2) {
        func_80153068(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7, 4);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[19].unk3, &D_8015EDB4[19].unk2, 2, 7);
    }
    temp = D_8015EDB4[20].unk84[2];
    if (temp == 1) {
        func_80151F94(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14, 2);
    } else if (temp == 2) {
        func_80153068(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14, 3);
    } else if (temp == 3) {
        func_801522B0(&D_8015EDB4[20].unk3, &D_8015EDB4[20].unk2, 3, 14);
    }
    if (D_8015EDB4[17].unk84[2] == 1 && D_8015EDB4[19].unk84[2] == 0 && D_8015EDB4[18].unk84[2] == 0 && D_8015EDB4[20].unk84[2] == 0) {
        func_80152C60(4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_801534E0);

void func_801536BC(u32 arg0) {
    if (arg0 < D_8015EDB4[23].unk3) {
        func_8015131C(D_8015EDB4[23].unk2);
        D_8015EDB4[23].unk2 = D_8015EDB4[23].unk2 + 1;
        D_8015EDB4[23].unk3 = 0;
    }
    if (D_8015EDB4[23].unk2 >= 0x7CU) {
        D_8015EDB4[23].unk2 = 0;
        D_8015EDB4[23].unk3 = D_8015EDB4[23].unk2;
        return;
    }
    D_8015EDB4[23].unk3 = D_8015EDB4[23].unk3 + 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80151C30", func_80153774);
