#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_801504E0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_801505B8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80150AA4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80150B90);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80150CD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80150DE0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80150F84);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80151108);

void func_80151380(s32 arg0) {
    s32 temp_v0;

    D_8012146B = arg0;
    D_801214AF = arg0;
    D_801214F3 = arg0;
    if (arg0 < 0x80) {
        temp_v0 = 0x80 - arg0;
        D_80121603 = temp_v0;
        D_80121647 = temp_v0;
        D_8012168B = temp_v0;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_801513C8);

s32 func_801515F0(void) {
    switch (D_8015EDC4 & 0xFFFF0000) {
    case 0x100000:
        func_8013E810(0x35, 9, 1, 0);
        func_8013E810(0x36, 9, 1, 0);
        func_8013E810(0x37, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        *(u8 *)&D_80121647 = 0x80;
        *(u8 *)&D_8012168B = 0x80;
        break;
    case 0x200000:
        func_8013E810(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    case 0x400000:
        func_8013E810(0x35, 9, 1, 0);
        *(u8 *)&D_80121603 = 0x80;
        break;
    }
}

void func_801516E4(void) {
    switch (D_8015EDB0) {
    case 0:
        func_80150DE0();
        func_8013E97C(0x1D, 0x118, 0x88);
        func_8013E97C(0x32, D_8015EC68 + 0x118, D_8015EC6C + 0x88);
        func_8013E810(0x1D, 0, 1, 1);
        func_8013E810(0x32, 7, 5, 1);
        D_801213B0 = 1;
        D_80120E05 = 4;
        D_80121399 = 3;
        func_8014EBD0();
        break;
    case 1:
        func_8013E97C(0x1D,
                      (D_8015EC60 - D_8015EC58) * D_8015EDB8 / 100 + D_8015EC58,
                      (D_8015EC64 - D_8015EC5C) * D_8015EDB8 / 100 + D_8015EC5C);
        func_8013E97C(0x32,
                      (D_8015EC60 - D_8015EC58) * D_8015EDB8 / 100 + D_8015EC58 + D_8015EC68,
                      (D_8015EC64 - D_8015EC5C) * D_8015EDB8 / 100 + D_8015EC5C + D_8015EC6C);
        D_8015EDB8 = D_8015EDB8 + 4;
        if (D_8015EDB8 >= 0x65) {
            func_8014EBD0();
        }
        break;
    case 2:
        func_8014EBA8();
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801504E0", func_80151984);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1C4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D1F0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D200);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D210);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D220);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D230);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D240);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D254);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D268);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D27C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D290);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2A4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2E0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D2F4);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D308);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D31C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D330);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D348);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D360);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D378);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D38C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3A0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3B8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3D0);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D3E8);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D400);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D41C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D438);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D454);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D470);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D48C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4AC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4CC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D4EC);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D50C);

INCLUDE_RODATA("asm/ovl/RPG_BAT/data/RPG_BAT/801504E0.rodata", D_8015D52C);
