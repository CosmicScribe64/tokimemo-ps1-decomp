#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F190);

void func_8013F1E0(void) {
    D_80146144 = 0x801DDBB4;
    D_80146148 = 0x801DDBB8;
    D_8014614C = 0x801DDC70;
    D_80146154 = 0x801DB000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F224);

void func_8013F360(void) {
    if ((D_80146168[D_80146158] == 0xB) && (D_800E62BE == 0x60)) {
        func_8013D324();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F3C0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F494);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013F9C4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013FA80);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8013FB28);

void func_8013FFA8(void) {
    bg_read_sub2(0x41E4);
    func_8004284C();
}

void func_8013FFD0(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_8011ECD0[i * 0x44 + 0x1983] = 0;
    }
    D_80146160 = 0;
    D_80146164 = 0;
    func_8004284C();
}

s32 func_80140030(void) {
    D_80122D2C = 0;
    if (D_80145FE4 == 0) {
        D_800E71DF = 0;
        D_80122CD4 = 0;
        D_80122CD0 = 7;
        func_80042908(6);
        D_800E67F2 += 1;
        return 0;
    }
    if (D_80145FE4 == 7) {
        D_800E71DF = 7;
        D_80122CD4 = 7;
        D_80122CD0 = 7;
        func_80042908(7);
        D_800E6820 += 1;
        return 0;
    }
    if (D_80145FE4 == 9) {
        D_800E71DF = 9;
        D_80122CD4 = 9;
        D_80122CD0 = 7;
        func_80042908(8);
        D_800E682E += 1;
        return 0;
    }
    func_8004284C();
}

void func_80140144(void) {
    if (D_80146168[D_80146158] == 0xB) {
        func_80044750(0x200);
    } else {
        func_80044750(0x201);
        *(s16 *)(D_800E6280 + D_80146168[D_80146158] * 0x38 + 0x1C2) += 1;
    }
    check_para_limit();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_801401DC);

void func_80140220(void) {
    func_80046318(0x4D, 0x801B0000, 0xBDAC);
    func_8004284C();
}

void func_80140250(void) {
    func_80046318(0xE, 0x801D7000, 0xBDF9);
    func_8004284C();
}

void func_80140284(void) {
    D_80145F30 = 0;
    if (D_80146168[D_80146158] == 0xB) {
        D_80145F2C = 0x27;
        D_800E639E += 0xA;
    } else {
        D_80145F2C = 0x26;
        D_800E639E -= 0xA;
    }
    check_para_limit();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140314);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140510);

void func_80140698(void) {
    s16 i;
    u8 *p;

    for (i = 0; i < 3; i++) {
        p = D_8011ECD0 + i * 0x44;
        if (*(s16 *)(p + 0x1A20) == 0xE && *(s16 *)(p + 0x1A10) == 9) {
            *(s16 *)(p + 0x1A1E) = func_800AE0D0() % 3;
        }
    }
}

void func_80140780(void) {
    D_80146230 = 0x801B21E0;
    D_80146234 = 0x801B21E4;
    D_80146238 = 0x801B2228;
    D_80146240 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_801407C0);

void func_8014088C(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, D_800B36AC, D_800B36EC, D_800B372C);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

void func_80140908(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_80140990(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_801409F8(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_80140A78(void) {
    func_80046318(0xD, 0x801B0000, 0xBCBB);
    func_80140780();
    func_8004284C();
}

void func_80140AB0(void) {
    func_80044750(0x203);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140AD8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140BD8);

void func_80140D30(void) {
    bg_read_sub2(0x41DC);
    func_8004284C();
}

void func_80140D58(void) {
    D_80120652 = 9;
    func_8004284C();
}

void func_80140D80(void) {
    *(s16 *)((u8 *)&D_800E6442 + D_800E71DF * 0x38) = 0x28;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140DC0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80140F54);

void func_80140FDC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141004(void) {
    func_80044750(0x506);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8014102C);

typedef struct {
    u8 pad:2;
    u8 f:1;
    u8 rest:5;
} Bits64B8;

void func_801410B4(void) {
    ((Bits64B8 *)&D_800E64B8)->f = 1;
    D_80145F2C = 3;
    func_8004284C();
}

void func_801410F0(void) {
    if (((u32) D_800E64BA >> 4) == 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141138(void) {
    if (((u32) D_800E64BA >> 4) != 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80141180);

void func_801411FC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80141224);

void func_801412AC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801412D4(void) {
    if (((u32) D_800E652A >> 4) == 7) {
        D_80145F2C += 3;
    }
    func_8004284C();
}

void func_8014131C(void) {
    ((Bits64B8 *)&D_800E6528)->f = 1;
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80141358);

void func_801413CC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_801413F4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_8014147C);

void func_801414B8(void) {
    func_80044750(0x505);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_801414E0);

void func_80141568(void) {
    if (check_end_k() != 0) {
        k_reset(1);
        func_8004284C();
    }
}

void func_801415A0(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_801415C8);

void func_80141650(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013F190", func_80141678);

void func_801416EC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141714(void) {
    func_80062CD0(0x56C0);
    func_8004284C();
}

void func_8014173C(void) {
    func_80042908(D_800E69A1);
    func_80042940(D_800E69A2);
}
