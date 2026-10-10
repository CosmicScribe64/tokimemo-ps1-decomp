#include "common.h"
#include "game.h"

void initView(void) {
    func_8009AD30(0x3E8);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x3E8;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(0x64);
    func_8009AD60(0x10000);
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", initLight);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", initCoordinate);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", initModelingData_init);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_80058F9C);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_80059048);

void func_8005907C(void) {
    func_80098380();
    func_8009B560(0, 0, 0, 0xF0);
    func_80098530();
    InitGeom();
    D_8011ECA0 = func_80098370();
}

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800590CC);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_800591D8);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_80059308);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_8005938C);

INCLUDE_ASM("asm/nonmatchings/main/80058D20", func_80059688);
