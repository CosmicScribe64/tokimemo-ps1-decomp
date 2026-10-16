#include "common.h"
#include "ovl/RPG_BAT.h"

/* NON_MATCHING: T-12300, 557/557 instructions; promoted D_8015EDE0
 * uses v0 instead of v1 at 58 positions. Control flow and scheduling agree. */
#ifdef NON_MATCHING
void func_80145CC0(void) {
    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "　　　　アストラルファング　　　　");
        func_8013E97C(0x38, 0, 0);
        D_80121531 = 4;
        func_8013E7C0(0x38, 1, 1, 1);
        func_8014EF3C();
        break;
    case 1:
        func_8014F080(0x3C);
        break;
    case 2:
        func_8014B738((s32) D_8015E8F0, 1, 0x3C);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8014EF3C();
        break;
    case 3:
        func_8014F080(0x14);
        break;
    case 4:
        func_8013E7C0(0x38, 2, 5, 1);
        func_8013F15C(0x507, 1, 0);
        func_8014EF3C();
        break;
    case 5:
        D_8015EDE0 += 3;
        func_8013E97C(0x38, (s32) (D_8015EDE0 * 0x90) / 100, 0);
        if (D_8015EDE0 >= 0x65) {
            func_8013F15C(0x506, 1, 0);
            func_8013E97C(0x38, 0x90, 0);
            func_8013E7C0(0x38, 3, 1, 1);
            func_8014EF3C();
        }
        break;
    case 6:
        func_8014F080(0xA);
        break;
    case 7:
        func_8013EA60(0, 0x21, D_8015ED94 - 3, 1);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E7C0(0x30, 4, 1, 1);
        D_8015ED98 = 0x1000;
        func_8014EF3C();
        break;
    case 8:
        func_8014F080(0x96);
        break;
    case 9:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「だめだ！　やられてしまう！");
        func_8014B738((s32) D_8015E8F0, 1, 0x78);
        func_8014EF3C();
        break;
    case 10:
        func_8014F178(1);
        break;
    case 11:
        func_800AE0F0(D_8015E828, "？？？「ピキィー！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 12:
        func_8014F080(0xA);
        break;
    case 13:
        func_8013F15C(0x50B, 1, 0);
        func_8014EF3C();
        break;
    case 14:
        func_8014F178(1);
        break;
    case 15:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "「なっ、なんだ！");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 16:
        func_8014F178(1);
        break;
    case 17:
        D_8015EDE0 += 1;
        if (D_8015EDE0 < 0x3D) {

            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x30, 0xFF, 1, 0);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 0);
                }
            } else {
                func_8013E7C0(0x30, 0xFF, 1, 1);
                if (D_8015EB9C != 0xE) {
                    func_8013E7C0(0x1D, 0xFF, 1, 1);
                }
            }
        }
        func_8013E97C(0x28, 0, 0);
        if (D_8015EDE0 >= 0x3D) {

            if (!(D_8015EDE0 & 1)) {
                func_8013E7C0(0x28, 0, 1, 1);
            } else {
                func_8013E7C0(0x28, 0, 1, 0);
            }
        }
        if (D_8015EDE0 >= 0x78) {
            func_8014EF3C();
        }
        break;
    case 18:
        func_800AE0F0(D_8015E828, "正義のマングース参上！");
        func_800AE0F0(D_8015E850, "　　　　　マングース斬鉄剣　　　　　");
        func_8014B738((s32) D_8015E8F0, 2, 0x5A);
        D_8015EE18 ^= 0x10000000;
        func_8014EF3C();
        break;
    case 19:
        func_8014F178(1);
        break;
    case 20:
        func_8013E7C0(0x28, 1, 5, 1);
        func_8013F15C(0x509, 1, 0);
        func_8014EF3C();
        break;
    case 21:
        if (func_8013E90C(0x28) != 0) {
            func_8013E7C0(0x28, 2, 1, 1);
            func_8013F15C(0x50A, 1, 0);
            func_8014EF3C();
        }
        break;
    case 22:
        D_8015EDE0 += 1;
        func_8013E97C(0x28, (-D_8015EDE0) << 3, 0);
        if ((D_8015EDE0 == 0x13) || (D_8015EDE0 == 0x16) || (D_8015EDE0 == 0x19) || (D_8015EDE0 == 0x1C)) {
            func_8013EA30(0xFF);
        } else {
            func_8013EA30(0x80);
        }
        if (D_8015EDE0 >= 0x26) {
            func_8014EF3C();
        }
        break;
    case 23:
        func_8014F080(0x1E);
        break;
    case 24:
        func_8013E97C(0x28, -0xC0, -0xA0);
        func_8013E7C0(0x28, 3, 1, 1);
        func_8013F15C(0x50A, 1, 0);
        func_8014EF3C();
        break;
    case 25:
        D_8015EDE0 += 1;
        func_8013E97C(0x28, -0xC0, (D_8015EDE0 * 8) - 0xA0);
        if ((D_8015EDE0 == 0xD) || (D_8015EDE0 == 0x10) || (D_8015EDE0 == 0x13) || (D_8015EDE0 == 0x16)) {
            func_8013EA30(0xFF);
        } else {
            func_8013EA30(0x80);
        }
        if (D_8015EDE0 >= 0x1F) {
            func_8014EF3C();
        }
        break;
    case 26:
        func_8014F080(0x3C);
        break;
    case 27:
        func_80146578();
        break;
    case 28:
        func_8014F080(0x1E);
        break;
    case 29:
        func_8013E7C0(0x30, 0, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 0, 1, 1);
        }
        k_disp_switch(D_8015EDE8, 0);
        func_800AE0F0(D_8015E9E0, "戦闘に勝った！！");
        func_800AE0F0(D_8015EA08, "ありがとう、正義のマングース");
        func_800AE0F0(D_8015EA30, "経験値　：８２０ＥＸＰ");
        func_800AE0F0(D_8015EA58, "お金　　：１７５ゴールド");
        func_80152384();
        func_8014EF3C();
        break;
    case 30:
        func_8014F080(0x3C);
        break;
    case 31:
        func_8004500C(1, 0x202);
        func_8014EF3C();
        break;
    case 32:
        func_8014B738((s32) D_8015EB70, D_8015ED14, 0x5A);
        func_8013E7C0(0x30, 9, 1, 1);
        if (D_8015EB9C != 0xE) {
            func_8013E7C0(0x1D, 4, 1, 1);
        }
        func_8014EF3C();
        break;
    case 33:
        func_8014F178(0);
        break;
    case 34:
        func_80042808();
        break;
    default:
        func_8014EF8C();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80145CC0", func_80145CC0);
#endif

void func_80146578(void) {
    s32 sp44;
    s32 sp38;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s0_3;
    s32 temp_s0_4;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_v1;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_s1;
    s32 var_s1_2;
    s32 var_s1_3;
    u8 *var_s0;

    func_8013F15C(0x508, 0, 1);
    var_s1 = 0;
    switch (D_8015EDDC) {
    case 0:
        var_s0 = D_8011ECD0;
        do {
            *(s32 *)((unsigned char *)var_s0 + (10076)) = (s32) D_8015EEA8;
            *(s32 *)((unsigned char *)var_s0 + (10080)) = (s32) D_8015EEC0;
            temp_s2 = var_s1 + 0x34;
            *(s32 *)((unsigned char *)var_s0 + (10120)) = 0x41000000;
            *(s8 *)((unsigned char *)var_s0 + (10071)) = 0x80;
            *(s8 *)((unsigned char *)var_s0 + (10131)) = 0x13;
            *(s32 *)((unsigned char *)var_s0 + (10116)) = (s32) D_8015EE90;
            func_8013E97C(temp_s2, 0, 0);
            func_8013E7C0(temp_s2, var_s1 + 5, 1, 1);
            var_s1 += 1;
            var_s0 += 0x44;
        } while (var_s1 != 0xC);
        func_8014EF6C();
        break;
    case 1:
        temp_s1 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_s1;
        temp_lo = (s32) (temp_s1 * -0x30) / 100;
        temp_v1 = func_800A0070((s32) (temp_s1 << 0xB) / 100) >> 8;
        sp44 = temp_v1;
        func_8013E97C(0x34, temp_lo, temp_lo - temp_v1);
        temp_s0 = temp_lo - sp44;
        sp38 = 0;
        func_8013E97C(0x35, 0, temp_s0);
        temp_lo_2 = (s32) (temp_s1 * 0x30) / 100;
        func_8013E97C(0x36, temp_lo_2, temp_s0);
        temp_s0_2 = ((s32) (temp_s1 * -0x10) / 100) - sp44;
        func_8013E97C(0x37, temp_lo, temp_s0_2);
        func_8013E97C(0x38, sp38, temp_s0_2);
        func_8013E97C(0x39, temp_lo_2, temp_s0_2);
        temp_s0_3 = ((s32) (temp_s1 * 0x10) / 100) - sp44;
        func_8013E97C(0x3A, temp_lo, temp_s0_3);
        func_8013E97C(0x3B, sp38, temp_s0_3);
        func_8013E97C(0x3C, temp_lo_2, temp_s0_3);
        func_8013E97C(0x3D, (s32) (temp_s1 * 0x60) / 100, temp_s0_3);
        temp_s0_4 = temp_lo_2 - sp44;
        func_8013E97C(0x3E, sp38, temp_s0_4);
        func_8013E97C(0x3F, temp_lo_2, temp_s0_4);
        if (D_8015EDE0 >= 0x51) {
            D_8015EDE0 = D_8015EDE0;
            if (!(D_8015EDE0 & 1)) {
                var_s1_2 = 0;
                var_a0 = 0x34;
                do {
                    func_8013E7C0(var_a0, var_s1_2 + 5, 1, 1);
                    var_s1_2 += 1;
                    var_a0 = var_s1_2 + 0x34;
                } while (var_s1_2 != 0xC);
            } else {
                var_s1_3 = 0;
                var_a0_2 = 0x34;
                do {
                    func_8013E7C0(var_a0_2, var_s1_3 + 5, 1, 0);
                    var_s1_3 += 1;
                    var_a0_2 = var_s1_3 + 0x34;
                } while (var_s1_3 != 0xC);
            }
        }
        if (D_8015EDE0 >= 0x65) {
            D_8015EE18 ^= 0x10000000;
            func_8014EF3C();
        }
        break;
    }
}
