#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014ADD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014B294);

void func_8014B738(s32 arg0, s32 arg1, s32 arg2) {
    k_sub_reset();
    D_8015E740 = arg0;
    D_8015E744 = arg2;
    D_8015E748 = arg1;
    D_8015EE44 = 0;
    D_8015EE4C = 0;
    D_8015EE48 = 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014B79C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BAA4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BB80);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BFE4);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014C360);

void func_8014C4FC(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE60 == 1) {
        half = D_8015EE64 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0, 0xFF, 1);
            func_8013E8B8(4, 0xFF, 1);
            func_8013E8B8(8, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(1, 0xFF, 1);
            func_8013E8B8(5, 0xFF, 1);
            func_8013E8B8(9, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(2, 0xFF, 1);
            func_8013E8B8(6, 0xFF, 1);
            func_8013E8B8(0xA, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(3, 0xFF, 1);
            func_8013E8B8(7, 0xFF, 1);
            func_8013E8B8(0xB, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE64 & 1)) {
            p = D_8015E5A0[half];
            D_8011FDFA += p[0];
            D_8011FF0A += p[1];
            D_8012001A += p[2];
            D_8011FE3E += p[3];
            D_8011FF4E += p[4];
            D_8012005E += p[5];
            D_8011FE82 += p[6];
            D_8011FF92 += p[7];
            D_801200A2 += p[8];
            D_8011FEC6 += p[9];
            D_8011FFD6 += p[10];
            D_801200E6 += p[11];
        }
        D_8015EE64 += 1;
        if (D_8015EE64 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i, 0xFF, 0);
            }
            D_8015EE60 = 0;
        }
    }
}

void func_8014C804(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE6C == 1) {
        half = D_8015EE70 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0xC, 0xFF, 1);
            func_8013E8B8(0x10, 0xFF, 1);
            func_8013E8B8(0x14, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0xD, 0xFF, 1);
            func_8013E8B8(0x11, 0xFF, 1);
            func_8013E8B8(0x15, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0xE, 0xFF, 1);
            func_8013E8B8(0x12, 0xFF, 1);
            func_8013E8B8(0x16, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0xF, 0xFF, 1);
            func_8013E8B8(0x13, 0xFF, 1);
            func_8013E8B8(0x17, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE70 & 1)) {
            p = D_8015E5A0[half];
            D_8012012A += p[0];
            D_8012023A += p[1];
            D_8012034A += p[2];
            D_8012016E += p[3];
            D_8012027E += p[4];
            D_8012038E += p[5];
            D_801201B2 += p[6];
            D_801202C2 += p[7];
            D_801203D2 += p[8];
            D_801201F6 += p[9];
            D_80120306 += p[10];
            D_80120416 += p[11];
        }
        D_8015EE70 += 1;
        if (D_8015EE70 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0xC, 0xFF, 0);
            }
            D_8015EE6C = 0;
        }
    }
}

void func_8014CB0C(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE78 == 1) {
        half = D_8015EE7C / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0x18, 0xFF, 1);
            func_8013E8B8(0x1C, 0xFF, 1);
            func_8013E8B8(0x20, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0x19, 0xFF, 1);
            func_8013E8B8(0x1D, 0xFF, 1);
            func_8013E8B8(0x21, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0x1A, 0xFF, 1);
            func_8013E8B8(0x1E, 0xFF, 1);
            func_8013E8B8(0x22, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0x1B, 0xFF, 1);
            func_8013E8B8(0x1F, 0xFF, 1);
            func_8013E8B8(0x23, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE7C & 1)) {
            p = D_8015E5A0[half];
            D_8012045A += p[0];
            D_8012056A += p[1];
            D_8012067A += p[2];
            D_8012049E += p[3];
            D_801205AE += p[4];
            D_801206BE += p[5];
            D_801204E2 += p[6];
            D_801205F2 += p[7];
            D_80120702 += p[8];
            D_80120526 += p[9];
            D_80120636 += p[10];
            D_80120746 += p[11];
        }
        D_8015EE7C += 1;
        if (D_8015EE7C / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0x18, 0xFF, 0);
            }
            D_8015EE78 = 0;
        }
    }
}

void func_8014CE14(void) {
    s32 half;
    s32 i;
    s16 *p;

    if (D_8015EE84 == 1) {
        half = D_8015EE88 / 2;
        switch (half) {
        case 0:
            func_8013E8B8(0x24, 0xFF, 1);
            func_8013E8B8(0x28, 0xFF, 1);
            func_8013E8B8(0x2C, 0xFF, 1);
            break;
        case 1:
            func_8013E8B8(0x25, 0xFF, 1);
            func_8013E8B8(0x29, 0xFF, 1);
            func_8013E8B8(0x2D, 0xFF, 1);
            break;
        case 2:
            func_8013E8B8(0x26, 0xFF, 1);
            func_8013E8B8(0x2A, 0xFF, 1);
            func_8013E8B8(0x2E, 0xFF, 1);
            break;
        case 3:
            func_8013E8B8(0x27, 0xFF, 1);
            func_8013E8B8(0x2B, 0xFF, 1);
            func_8013E8B8(0x2F, 0xFF, 1);
            break;
        }
        if (half >= 0x10) {
            half = 0xF;
        }
        if (!(D_8015EE88 & 1)) {
            p = D_8015E5A0[half];
            D_8012078A += p[0];
            D_8012089A += p[1];
            D_801209AA += p[2];
            D_801207CE += p[3];
            D_801208DE += p[4];
            D_801209EE += p[5];
            D_80120812 += p[6];
            D_80120922 += p[7];
            D_80120A32 += p[8];
            D_80120856 += p[9];
            D_80120966 += p[10];
            D_80120A76 += p[11];
        }
        D_8015EE88 += 1;
        if (D_8015EE88 / 2 >= 0x1A) {
            for (i = 0; i != 0xC; i++) {
                func_8013E8B8(i + 0x24, 0xFF, 0);
            }
            D_8015EE84 = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014D11C);
