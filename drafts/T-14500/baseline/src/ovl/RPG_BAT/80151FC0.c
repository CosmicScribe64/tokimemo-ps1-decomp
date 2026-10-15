#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80151FC0(void) {
    if (D_8015EB98 == 0x40000) {
        func_8013E7C0(0x38, 3, 5, 1);
    } else {
        func_8013E7C0(0x38, 4, 5, 1);
    }
    switch (D_8015EB98) {
    case 0x10000:
        func_8013F15C(0x505, 1, 0);
        return;
    case 0x20000:
        func_8013F15C(0x506, 1, 0);
        return;
    case 0x40000:
        func_8013F15C(0x505, 1, 0);
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80151FC0", func_80152080);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7D4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D7FC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D808);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D810);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D820);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D828);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D834);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D840);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/80151FC0.rodata", D_8015D84C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80151FC0", func_80152384);
