#define MAIN_API_OVERRIDE_D_80122760 /* three-light block (3 x 16 bytes) sharing base symbol for as1 load hoisting (T-24107) */
#include "common.h"
#include "ovl/TACO.h"

typedef struct {
    s32 unk00[3];
    u8 r0, g0, b0, pad0;
    s32 unk10[3];
    u8 r1, g1, b1, pad1;
    s32 unk20[3];
    u8 r2, g2, b2, pad2;
} TacoLightBlock;

extern TacoLightBlock D_80122760;

void func_80134930(void) {
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    draw2d3d(1, 1);
    initView();
    func_80146D60(-0xFA0);
    initModelingData_init(0x8019C800);
    func_801471D0(0x8019C800);
    func_80147488(1, D_8015F3E0[0], 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = 0x32;
    D_80128880[1].z = 0x32;
    D_80128880[1].y = 0xD2;
    func_80147488(2, D_8015F3E0[1], 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = 0x4B;
    D_80128880[2].z = -0x32;
    D_80128880[2].y = 0xC3;
    D_8015F3E8 = 0;
}

s32 func_80134A18(void) {
    s32 i;
    s32 frame;

    frame = D_8015F3E8;
    if (frame < 0x20) {
        if (D_80122760.r0 < 0xC0) {
            /* Chained stores: IDO stores the middle and left targets first and the read one last, the original order. */
            D_80122760.r0 = D_80122760.g0 = D_80122760.b0 = D_80122760.b0 + 6;
            D_80122760.r1 = D_80122760.g1 = D_80122760.b1 = D_80122760.b1 + 1;
            /* FAKE: repeated masked sum makes a uopt CSE range in $v0 while keeping the ugen temporary slot (T-22006, T-24107). */
            D_80122760.g2 = (D_80122760.b2 + 1) & 0xFF;
            D_80122760.r2 = D_80122760.b2 = (D_80122760.b2 + 1) & 0xFF;
            func_8009AD70(0, (s32) &D_80122760);
            func_8009AD70(1, (s32) &D_80122770);
            func_8009AD70(2, (s32) &D_80122780);
        }
        if (frame < 0xF) {
            D_800E6280.unk_038 += 1;
        }
    } else if (frame >= 0x20 && frame < 0x54) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 1);
        }
    } else if (frame >= 0x54 && frame < 0x68) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 2);
        }
    } else if (frame >= 0x68 && frame < 0x72) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 3);
        }
    } else if (frame >= 0x72 && frame < 0x7C) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 4);
        }
    } else if (frame >= 0x7C && frame < 0x9A) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 5);
        }
    } else if (frame >= 0x9A && frame < 0xAE) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 6);
        }
    } else if (frame >= 0xAE && frame < 0xEA) {
        for (i = 0; i < 2; i++) {
            func_80147488(i + 1, D_8015F3E0[i], 7);
        }
    } else if (frame >= 0xF0) {
        return 1;
    }
    D_8015F3E8 = D_8015F3E8 + 1;
    return 0;
}


void func_80134D8C(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134EBC();
        break;
    case 1:
        func_80135220();
        break;
    case 2:
        func_80135278();
        break;
    case 3:
        func_801352CC();
        break;
    case 4:
        func_80135330();
        break;
    case 5:
        func_80135350();
        break;
    }
    func_80134A18();
    func_8013506C();
    if (D_800E6280.unk_1100 == 0xA0) {
        func_801335A0(0, 0x504);
    }
    if (D_800E6280.unk_1100 == 0x40) {
        func_801335A0(0, 0x505);
    }
    if (D_800E6280.unk_1100 == 0x80) {
        func_801335A0(0, 0x506);
    }
    if (D_800E6280.unk_1100 == 0xC0) {
        func_801335A0(0, 0x507);
    }
}

void func_80134EBC(void) {
    s32 i;

    D_8015EDB0 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_8015EDE8 = 0;
    D_800E6280.unk_038 = 0;
    func_80059048();
    func_800438DC(1, 1);
    func_80048DAC(1);
    func_8004E9F4(0);
    func_800443F0(0, 0x1EF, 0x100, 1, &D_800E6280.unk_163C[0]);
    for (i = 56; i < 72; i++) {
        D_800E6280.unk_163C[i] = 0;
    }
    for (i = 0; i < 0x80; i++) {
        D_800E6280.unk_163C[128 + i] = 0;
    }
    func_80043914(&D_800E6280.unk_163C[0], 0x1F, 1, 1, 0);
    func_80134FB8();
    func_80134930();
    func_8004284C();
}

void func_80134FB8(void) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E6280.unk_1A4C;
    D_8011ED24 = D_800E6280.unk_1A5C;
    D_8011ED48 = D_800E6280.unk_1A3C;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED3A = -0xA0;
    D_8011ED3E = -0x64;
}

void func_8013506C(void) {
    /* FAKE: unused 4-byte local reserves the top frame word so the $a3 spill sits at 0x3C($sp) in the 0x48 frame (T-24120). */
    s32 pad[1];
    s32 i;
    u32 x;
    u32 y;

    for (i = 0; i < 0x80; i++) {
        if (D_800E6280.unk_163C[128 + i] == 0) {
            x = func_800AE0D0() % 320;
            y = func_800AE0D0() % 240;
            D_800E6280.unk_163C[128 + i] = (x << 16) + y;
            break;
        }
    }
    for (i = 0; i < 0x80; i++) {
        if (D_800E6280.unk_163C[128 + i] != 0) {
            x = (u32)D_800E6280.unk_163C[128 + i] >> 16;
            y = D_800E6280.unk_163C[128 + i] & 0xFFFF;
            func_80143574(x - 0xA0, y - 0x78, (x * y + D_800E6280.unk_10F8) & 0xFFFFFF);
        }
    }
}


s32 func_8013515C(void) {
    s32 i;
    s32 n;

    n = 0;
    for (i = 56; i < 72; i++) {
        if (D_800E6280.unk_163C[i] == 0) {
            if ((func_800AE0D0() & 0xFF) < 0x7F && n == 0) {
                D_800E6280.unk_163C[i] = ((s32 *)D_800E6280.unk_1A6C)[i];
            } else {
                n++;
            }
        }
    }
    func_80043914(&D_800E6280.unk_163C[0], 0x1F, 1, 1, 0);
    return n;
}

void func_80135220(void) {
    if ((u32)D_800E6280.unk_1104.w >= 0xD1U) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(5);
    }
}

void func_80135278(void) {
    if (func_8013515C() == 0) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(5);
    }
}

void func_801352CC(void) {
    if ((u32)D_800E6280.unk_1104.w < 0xFFU) {
        if (D_800E6280.unk_F88 != 0) {
            func_80042940(5);
            return;
        }
        func_8004AC18(0xFF - D_800E6280.unk_1104.w);
        return;
    }
    func_8004284C();
}

void func_80135330(void) {
    func_8004284C();
}

void func_80135350(void) {
    func_80059048();
    load_palette(D_800E6280.unk_1A6C, 0x1F, 1, 1, 0);
    func_80042908(3);
}

void func_80135394(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80135418();
        break;
    case 1:
        func_8013546C();
        break;
    case 2:
        func_80135638();
        break;
    }
    func_8013506C();
}
void func_80135418(void) {
    func_8009C210(0);
    func_80059048();
    func_800591D8(0);
    func_80134FB8();
    D_8011ED1B = 0x40;
    D_8011ED3E = -0x78;
    func_8004284C();
}

void func_8013546C(void) {
    if (D_800E6280.unk_F88 & 0x5100) {
        if (D_800E6280.unk_F88 & 0x1000) {
            D_8015EDE8 -= 1;
        }
        if (D_800E6280.unk_F88 & 0x4000) {
            D_8015EDE8 += 1;
        }
        if (D_800E6280.unk_F88 & 0x100) {
            D_8015EDE8 += 1;
        }
        if (D_8015EDE8 < 0) {
            D_8015EDE8 = 4;
        }
        if (D_8015EDE8 >= 5) {
            D_8015EDE8 = 0;
        }
        func_801335A0(0, 0x500);
    }
    if (D_800E6280.unk_F88 & 0x820) {
        func_801335A0(0, 0x501);
        func_8004284C();
    }
    func_8013555C();
    if ((u32) D_800E6280.unk_1100 >= 0x385U) {
        func_80042908(6);
    }
}

/* The menu index D_8015EDE8 is an s8 elsewhere (compared < 0); these switches read it as the unsigned byte the original uses. T-16560 */
s32 func_8013555C(void) {
    switch (*(u8 *)&D_8015EDE8) {
    case 0:
        func_801432F0("GAME START", -0x40, 0x48, 2);
        break;
    case 1:
        func_801432F0("  OPTION  ", -0x40, 0x48, 3);
        break;
    case 2:
        func_801432F0("  SCORES  ", -0x40, 0x48, 5);
        break;
    case 3:
        func_801432F0("OPERATIONS", -0x40, 0x48, 6);
        break;
    case 4:
        func_801432F0("   EXIT   ", -0x40, 0x48, 0);
        break;
    }
}

/* The switch reads D_8015EDE8 as an unsigned byte, as func_8013555C does. T-16560 */
s32 func_80135638(void) {
    if ((u32) D_800E6280.unk_1104.u >= 0x41U) {
        switch (*(u8 *)&D_8015EDE8) {
        case 0:
            func_80042908(0xA);
            break;
        case 1:
            func_80042908(4);
            break;
        case 2:
            func_80042908(5);
            break;
        case 3:
            func_80042908(7);
            break;
        case 4:
            func_80042908(0x2E);
            break;
        }
    }
    if (D_800E6280.unk_1109 != 3) {
        func_80048DAC(0);
    }
    if (D_800E6280.unk_1104.u & 4) {
        func_8013555C();
    }
}
