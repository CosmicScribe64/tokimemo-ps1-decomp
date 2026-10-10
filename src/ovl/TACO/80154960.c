#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80154960);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80154F74);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155230);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801553AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801557E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_8015595C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155C64);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155E10);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155F98);

void func_80156178(void) {
    TcPos p0;
    TcPos p2;
    TcPos pos;

    func_800AE120(D_801604C0);
    /* FAKE: x and y on one line so as1 orders their stores y, x like the original (decomp-permuter); real source unknown. T-9180 */
    pos.x = -0x100; pos.y = 0x800;
    pos.z = 0;
    func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80156244);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801563E4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80156924);
