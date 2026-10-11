#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80154430(void) {
    s16 *state;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　色素分解　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        return;
    case 4:
        state = &D_80121438;
        switch (*state) {
        case 1:
            func_8013F0F4(0x509, 0, 2);
            break;
        case 3:
            func_8013F0F4(0x509, 0, 3);
            break;
        case 5:
            func_8013F0F4(0x509, 0, 4);
            break;
        case 7:
            func_8013F0F4(0x509, 0, 5);
            break;
        }
        func_8014EDBC(0x34);
        return;
    case 5:
        func_8013E7C0(0x34, 4, 1, 1);
        func_8014EBA8();
        return;
    case 6:
        func_80154DD8();
        return;
    case 7:
        func_80154760();
        return;
    case 8:
        func_8015461C();
        return;
    case 9:
        func_8014ED44(0x14);
        return;
    case 10:
        func_8013D59C();
        return;
    case 11:
        func_8014EBEC();

    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015461C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154760);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015528C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80155868);
