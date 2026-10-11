#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_80141DE0);

s16 func_801420DC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        D_8015EDB4->unk80 = 0;
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        D_8015EDB4->unk7E = 0;
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        D_8015EDB4->unk7C = 0;
        return D_8015EDB4->unk7C;
    }
    return 0;
}

s16 func_801421AC() {
    if (D_8015EDB4->unk80 == 3 || D_8015EDB4->unk80 == 4) {
        return D_8015EDB4->unk80;
    }
    if (D_8015EDB4->unk7E == 3 || D_8015EDB4->unk7E == 4) {
        return D_8015EDB4->unk7E;
    }
    if (D_8015EDB4->unk7C == 3 || D_8015EDB4->unk7C == 4) {
        return D_8015EDB4->unk7C;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_80142220);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_801423B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80141DE0", func_801424A8);
