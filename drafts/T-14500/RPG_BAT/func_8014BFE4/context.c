#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014ADD0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014B294);

void func_8014B738(s32 arg0, s32 arg1, s32 arg2) {
    k_sub_reset();
    D_8015E740 = arg0;
    D_8015E744 = arg2;
    D_8015E748 = arg1;
    D_8015EE44.unk_00 = 0;
    D_8015EE44.unk_08 = 0;
    D_8015EE44.unk_04 = 1;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014B79C);

void func_8014BAA4(s32 arg0) {
    switch (arg0) {
    case 1:
        D_8015EE44.unk_04 = 0x80;
        return;
    case 2:
        D_8015EE44.unk_04 = 0x81;
        return;
    case 3:
        D_8015EE44.unk_04 = 0x82;
        return;
    case 4:
        D_8015EE44.unk_04 = 0x83;
        return;
    case 5:
        D_8015EE44.unk_04 = 0x84;
        return;
    case 6:
        D_8015EE44.unk_04 = 0x85;
        return;
    case 7:
        D_8015EE44.unk_04 = 0x86;
        return;
    case 8:
        D_8015EE44.unk_04 = 0x87;
        return;
    case 9:
        D_8015EE44.unk_04 = 0x88;
        return;
    case 10:
        D_8015EE44.unk_04 = 0x89;
        return;
    default:
        D_8015EE44.unk_04 = 1;
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014ADD0", func_8014BB80);

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EE5C;
extern s32 D_8015EE68;
extern s32 D_8015EE74;
extern s32 D_8015EE80;
/* end bulk_m2c declarations */
void func_8014BFE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp58;
    s32 sp50;
    s16 var_s1;
    s32 temp_a1;
    s32 temp_hi;
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s3;
    s32 temp_t1;
    s32 temp_t7;
    s32 var_a1;
    s32 var_s2;
    s32 var_s4;
    s32 var_s6;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *var_s0;

    temp_a1 = arg1;
    temp_t7 = temp_a1 - 0xA0;
    arg1 = temp_t7;
    var_s1 = arg2 - 0x78;
    if (arg3 >= 0xA) {
        if (arg3 < 0x64) {
            arg1 = temp_t7 + 4;
        } else if (arg3 < 0x3E8) {
            arg1 += 9;
        } else {
            arg1 += 0xD;
        }
    }
    var_a1 = 1;
    switch (arg0) {
    case 0:
        D_8015EE5C = 0;
        D_8015EE60 = 1;
        D_8015EE64 = 0;
        break;
    case 1:
        D_8015EE68 = 0;
        D_8015EE6C = 1;
        D_8015EE70 = 0;
        break;
    case 2:
        D_8015EE74 = 0;
        D_8015EE78 = 1;
        D_8015EE7C = 0;
        break;
    case 3:
        D_8015EE80 = 0;
        D_8015EE84 = 1;
        D_8015EE88 = 0;
        break;
    }
    temp_t1 = arg0 * 0xC;
    var_s4 = arg1;
    var_s0 = (arg0 * 0xCC * 4) + D_8011ECD0;
    sp50 = temp_t1;
    var_s2 = temp_t1 + 0x40;
    var_s6 = 0;
    do {
        temp_lo = arg3 / var_a1;
        *(s16 *)((unsigned char *)var_s0 + (4390)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4394)) = var_s1;
        *(s16 *)((unsigned char *)var_s0 + (4662)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4666)) = var_s1;
        *(s16 *)((unsigned char *)var_s0 + (4934)) = (s16) var_s4;
        *(s16 *)((unsigned char *)var_s0 + (4938)) = var_s1;
        if (temp_lo != 0) {
            temp_hi = temp_lo % 10;
            arg2 = (s32) var_s1;
            sp58 = var_a1;
            temp_s3 = temp_hi + 0x27;
            func_8013E864(var_s2 + 4, temp_s3, 0);
            func_8013E864(var_s2 + 8, temp_s3, 0);
            if (arg4 == 0) {
                func_8013E864(var_s2, temp_hi + 0x1D, 0);
            } else {
                func_8013E864(var_s2, temp_hi + 0x13, 0);
            }
            var_s1 = (s16) arg2;
            var_a1 = sp58;
        } else {
            temp_v0 = (var_s2 * 0x44) + D_8011ECD0;
            *(s16 *)((unsigned char *)temp_v0 + (38)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (42)) = 0x78;
            *(s16 *)((unsigned char *)temp_v0 + (310)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (314)) = 0x78;
            *(s16 *)((unsigned char *)temp_v0 + (582)) = 0xA0;
            *(s16 *)((unsigned char *)temp_v0 + (586)) = 0x78;
        }
        var_s6 += 9;
        var_s2 += 1;
        var_s0 += 0x44;
        var_s4 -= 9;
        var_a1 *= 0xA;
    } while (var_s6 != 0x24);
    if (arg3 == 0) {
        temp_v0_2 = (sp50 * 0x44) + D_8011ECD0;
        *(s16 *)((unsigned char *)temp_v0_2 + (4394)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4666)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4938)) = var_s1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4390)) = (s16) arg1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4662)) = (s16) arg1;
        *(s16 *)((unsigned char *)temp_v0_2 + (4934)) = (s16) arg1;
        func_8013E864(sp50 + 0x44, 0x27, 0);
        temp_s0 = sp50 + 0x40;
        func_8013E864(temp_s0 + 8, 0x27, 0);
        if (arg4 == 0) {
            func_8013E864(temp_s0, 0x1D, 0);
            return;
        }
        func_8013E864(temp_s0, 0x13, 0);
    }
}

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
