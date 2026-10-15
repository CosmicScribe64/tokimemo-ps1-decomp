#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8013F4F0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "紐緒「真・世界征服ロボ！！");
        func_8014B738(D_8015E8F0, 1, 0xF0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1A, 0x05000026, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value, as func_8013F4F0 (T-8080) */
s32 func_8013F5C8(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　　服従ドリルパンチ　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xC8);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C000020, 0x05000029, 1);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F694(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　独裁ミサイルシャワー　　　　");
            func_8014B738(D_8015E8F0, 1, 0x118);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1B000020, 0x0300002F, 2);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

s32 func_8013F760(void) {
    if (D_8015ED8C[0] != 3) {
        switch (D_8015ED8C[0]) {
        case 0:
            func_800AE0F0(D_8015E828, "　　　　世界征服メガビーム　　　　　");
            func_8014B738(D_8015E8F0, 1, 0xAA);
            D_8015ED8C[0] += 1;
            break;
        case 1:
            func_8013F1C4(0x1C00002A, 0x06000036, 0);
            D_8015ED8C[0] += 1;
            break;
        case 2:
            func_8013F250(0);
            break;
        case 3:
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_8013F82C);

void func_8013FD8C(void) {
    switch (D_8015EDD8) {
    case 0:
        func_801447E0();
        D_80121311 = 6;
        func_8013E54C(4);
        func_8014EF3C();
        break;
    case 1:
        func_8013F5C8();
        func_8013FF28();
        break;
    case 2:
        func_8014F080(0x3C);
        break;
    case 3:
        func_8013F220();
        func_8014EF3C();
        break;
    case 4:
        func_8013F694();
        func_80140204();
        break;
    case 5:
        func_8013F220();
        func_8014EF3C();
        break;
    case 6:
        func_8013F760();
        func_80140A58();
        break;
    case 7:
        func_8013F220();
        func_8014EF3C();
        break;
    case 8:
        func_8013EA60(0, 0x21, D_8015EBB0, 1);
        func_8014EF3C();
        break;
    case 9:
        func_8014F080(0x1E);
        break;
    case 10:
        func_8013D030();
    case 11:
        func_8014F080(0x14);
        break;
    case 12:
        if (D_8015ED98 & 0x1000) {
            D_8015ED98 = D_8015ED98 & ~0x1000;
            D_8015ED9C = 0xFFFF;
        }
        func_8013E54C(0xFF);
        func_8014EF8C();
        break;
    }
}

void func_8013FF28(void) {
    /* FAKE: case 6 on one line retains the global-store scheduling. T-9750. */
    s32 temp_v0;

    switch (D_8015EDDC) {
    case 0:
        func_8013E810(0x3B, 4, 1, 1);
        func_8014EF6C();

    default:
        break;

    case 1:
        func_8014F0BC(7);
        break;

    case 2:
        func_8013E810(0x3B, 3, 1, 1);
        func_8013E97C(0x3A, 0x70, 0x48);
        func_8013E810(0x3A, 5, 5, 1);
        func_8014EF6C();
        break;

    case 3:
        func_8014F138(0x3A);
        break;

    case 4:
        func_8013E810(0x3A, 6, 1, 1);
        func_8013F15C(0x505, 1, 0);
        func_8014EF6C();
        break;

    case 5:
        func_8013E97C(0x3A, D_8015EDE4 + 0x70, 0x48);
        D_8015EDE4 += 8;
        if ((D_8015EDE4 >= 0x91) && (!(D_8015ED98 & 0x100))) {
            func_8013E7C0(0x30, 0xA, 1, 1);
        }
        if (D_8015EDE4 >= 0x129) {
            func_8013E810(0x3A, 7, 1, 1);
            func_8014EF6C();
        }
        break;

    case 6: func_8013E97C(0x3A, 0x198 - D_8015EDE4, 0x48); temp_v0 = D_8015EDE4 + 8; D_8015EDE4 = temp_v0; if ((0x198 - temp_v0) < 0x70) { func_8013D030(); func_8014EF6C(); } break;

    case 7:
        func_8013E97C(0x39, 0x48, 0x60);
        func_8013E810(0x39, 8, 1, 1);
        func_8014EF6C();
        break;

    case 8:
        func_8014F0BC(0xB);
        break;

    case 9:
        func_8013E810(0x3B, 4, 1, 1);
        func_8013E810(0x3A, 0xFF, 1, 0);
        func_8013E7C0(0x39, 0xFF, 1, 0);
        func_8014EF6C();
        break;

    case 10:
        func_8014F0BC(7);
        break;

    case 11:
        func_8013E810(0x3B, 2, 1, 1);
        func_8014EF3C();
        break;
    }
}

void func_801401D0(s32 i) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x1AB6) += 1;
    *(s16 *)(p + 0x1ABA) -= 1;
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_80121614;
extern s8 D_80121575;
extern s8 D_801215FE;
extern u8 D_80121532;
/* end bulk_m2c declarations */
s32 func_80140204(void) {
    s32 *var_s2;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_t3;
    s32 temp_t8;
    s32 temp_t9;
    s32 var_a0;
    s32 var_s0;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    u8 *temp_s0;
    u8 *temp_s0_4;
    u8 *temp_s0_5;

    switch (D_8015EDDC) {
    case 0:
        func_8013E810(0x3B, 0xA, 5, 1);
        func_8013F15C(0x506, 1, 0);
        func_8014EF6C();
    default:
        break;
    case 1:
        func_8014F138(0x3B);
        break;
    case 2:
        D_801215FE = 0x40;
        D_80121614 = 0x19;
        func_8013E97C(0x39, 0x60, 0x78);
        func_8013E810(0x39, 0xC, 1, 1);
        func_8013E97C(0x38, 0, 0);
        func_8013E7C0(0x38, 9, 5, 1);
        func_8014EF6C();
        break;
    case 3:
        func_8013E97C(0x39, D_8015EDE4 + 0x62, 0x76 - D_8015EDE4);
        D_8015EDE4 += 2;
        if (!(D_80121532 & 1)) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
        }
        if ((D_8015EDE4 + 0x5A) >= 0x77) {
            func_8013E7C0(0x38, 0xFF, 1, 0);
            func_8013E810(0x39, 0xD, 5, 1);
            D_80121575 = 2;
            func_8014EF6C();
        }
        break;
    case 4:
        var_s1 = 0;
        var_s0 = 4;
        do {
            func_8013E97C(var_s0, 0x80, 0x58);
            func_8013E810(var_s0, 0x14, 1, 1);
            var_s1 += 1;
            var_s0 = var_s1 + 4;
        } while (var_s1 != 5);
        func_8014EF6C();
        break;
    case 5:
        temp_t8 = (s32) D_8015EDE4 % 2;
        if (temp_t8 == 0) {
            D_801207CA -= 1;
            D_80120852 -= 2;
            D_80120812 += 1;
            D_8012089A += 2;
        }
        if (temp_t8 == 0) {
            D_80120786 += 1;
            D_8012078A -= 1;
            temp_s0 = (1 * 0x44) + D_8011ECD0;
            *(s16 *)((unsigned char *)temp_s0 + (6906)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6906)) + 1);
            *(s16 *)((unsigned char *)temp_s0 + (6838)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6838)) + 1);
            *(s16 *)((unsigned char *)temp_s0 + (6842)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6842)) - 1);
            *(s16 *)((unsigned char *)temp_s0 + (6974)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6974)) + 1);
            *(s16 *)((unsigned char *)temp_s0 + (6910)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6910)) - 1);
            *(s16 *)((unsigned char *)temp_s0 + (7046)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (7046)) - 1);
            *(s16 *)((unsigned char *)temp_s0 + (6978)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (6978)) - 1);
            *(s16 *)((unsigned char *)temp_s0 + (7042)) = (s16) (*(s16 *)((unsigned char *)temp_s0 + (7042)) + 1);
        }
        temp_t9 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_t9;
        if (temp_t9 >= 0x21) {
            func_8014EF6C();
        }
        break;
    case 6:
        if (D_8015EDE4 >= 0x1B) {
            func_8013F15C(0x507, 0, 1);
            D_80120786 += 3;
            D_8012078A -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(0);
        }
        if (D_8015EDE4 >= 0xE) {
            func_8013F15C(0x507, 0, 2);
            D_801207CA += 3;
            D_801207CE -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(1);
        }
        if (D_8015EDE4 >= 0xE) {
            D_8012080E += 3;
            D_80120812 -= 3;
        } else if (!(D_8015EDE4 & 1)) {
            func_801401D0(2);
        }
        if (D_8015EDE4 > 0) {
            func_8013F15C(0x507, 0, 3);
            D_80120852 += 3;
            D_80120856 -= 3;
        }
        if (D_8015EDE4 > 0) {
            D_80120896 += 3;
            D_8012089A -= 3;
        }
        temp_t3 = D_8015EDE4 + 1;
        D_8015EDE4 = temp_t3;
        if (temp_t3 >= 0x51) {
            func_8014EF6C();
        }
        break;
    case 7:
        func_8014F0BC(0x1E);
        break;
    case 8:
        D_8015EDE4 += 1;
        D_8015ED14 += 1;
        var_s2 = &D_8015EBAC;
        var_s1_2 = 0;
        do {
            if ((*(s32 *)((unsigned char *)var_s2 + (200)) == 3) && !(D_8015ED98 & 0x100)) {
                func_8013E7C0(0x30, 5, 1, 1);
            }
            if ((D_8015EDE4 < 0x78) && (D_8015ED14 >= 3) && (*(s32 *)((unsigned char *)var_s2 + (200)) == 0) && (func_8013E9A8(5) == 0)) {
                func_8013F15C(0x508, 1, 0);
                D_8015ED14 = 0;
                if (func_8013E9A8(0x64) < 0x1E) {
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 1;
                    temp_s0_2 = var_s1_2 + 4;
                    func_8013E97C(temp_s0_2, func_8013E9A8(0x18) + 0x104, 0);
                    func_8013E810(temp_s0_2, 0x16, 1, 1);
                } else {
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 2;
                    temp_s0_3 = var_s1_2 + 4;
                    func_8013E97C(temp_s0_3, func_8013E9A8(0x40) + 0xF0, 0);
                    func_8013E810(temp_s0_3, 0x16, 1, 1);
                }
            }
            if (*(s32 *)((unsigned char *)var_s2 + (200)) == 1) {
                temp_s0_4 = (var_s1_2 * 0x44) + D_8011ECD0;
                *(s16 *)((unsigned char *)temp_s0_4 + (6842)) = (s16) (*(s16 *)((unsigned char *)temp_s0_4 + (6842)) + 5);
                if ((func_8013E9A8(4) - 0x22) < *(s16 *)((unsigned char *)temp_s0_4 + (6842))) {
                    func_8013E810(var_s1_2 + 4, 0xE, 5, 1);
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if (*(s32 *)((unsigned char *)var_s2 + (200)) == 2) {
                temp_s0_5 = (var_s1_2 * 0x44) + D_8011ECD0;
                *(s16 *)((unsigned char *)temp_s0_5 + (6842)) = (s16) (*(s16 *)((unsigned char *)temp_s0_5 + (6842)) + 5);
                if ((func_8013E9A8(4) + 8) < *(s16 *)((unsigned char *)temp_s0_5 + (6842))) {
                    func_8013E810(var_s1_2 + 4, 0xE, 5, 1);
                    *(s32 *)((unsigned char *)var_s2 + (200)) = 3;
                    func_8013F15C(0x509, 1, 0);
                }
            }
            if ((*(s32 *)((unsigned char *)var_s2 + (200)) == 3) && !(*(&D_80120762 + (var_s1_2 * 0x44)) & 1)) {
                func_8013E810(var_s1_2 + 4, 0xFF, 1, 0);
                *(s32 *)((unsigned char *)var_s2 + (200)) = 0;
            }
            var_s1_2 += 1;
            var_s2 += 4;
        } while (var_s1_2 != 0xC);
        if (D_8015EDE4 >= 0xDD) {
            func_8013D030();
            func_8014EF6C();
        }
        break;
    case 9:
        func_8013E810(0x3B, 0xB, 5, 1);
        var_s1_3 = 0;
        var_a0 = 4;
        do {
            func_8013E810(var_a0, 0xFF, 1, 0);
            var_s1_3 += 1;
            var_a0 = var_s1_3 + 4;
        } while (var_s1_3 != 0xC);
        func_8014EF6C();
        break;
    case 10:
        func_8014F138(0x3B);
        break;
    case 11:
        func_8014EF3C();
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140A58);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140CC8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013F4F0", func_80140E70);
