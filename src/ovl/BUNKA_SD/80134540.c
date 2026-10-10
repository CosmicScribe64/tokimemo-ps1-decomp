#include "common.h"
#include "ovl/BUNKA_SD.h"

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134540);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_801345BC);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_801346D4);

void func_801347CC(void) {
    func_8004500C(1, 0x201);
    func_80134938();
    func_8004284C();
}

void func_80134800(void) {
    u32 t;

    func_80134B88();
    func_80134D94();
    t = D_800E7384 + 1;
    D_800E7384 = t;
    if (t >= 0x818) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134850);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134938);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134B88);

INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134540", func_80134D94);
