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

void func_8015461C(void) {
    s32 i;

    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013EA30(255);
        func_8014EBD0();
        return;
    case 1:
        func_8014ED80(5);
        return;
    case 2:
        func_8013EA30(128);
        for (i = 0; i < 6; i++) {
            ((RpgFlagRecord44 *)D_8011ECD0)[i + 136].unk_03 ^= 128;
        }
        if ((D_8015EDC4 & 0x100000) && D_8015EBB0 == 1000) {
            D_8015EBC0 = 0;
        } else {
            ((RpgFlagRecord44 *)D_8011ECD0)[D_8015EC70 + 96].unk_03 ^= 128;
            func_8013D3E4();
        }
        func_8014EBA8();
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s8 D_80121133;
extern s8 D_80121177;
extern s8 D_801211BB;
extern s8 D_801211FF;
extern s8 D_80121243;
extern s8 D_80121287;
extern u8 D_801210F3;
extern u8 D_80121137;
/* end bulk_m2c declarations */
s32 func_80154760(void) {
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_t8;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s3;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_4;
    u8 *var_s0;
    u8 *temp_s0;
    u8 *temp_v0;

    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (D_8015EBAC) {
        default:
            var_v1 = 0x3B;
            break;
        case 0:
            if (*D_8015EBCC == 1) {
                D_8015ED14 = 1;
            }
            var_v1 = 0x3B;
            break;
        case 1:
            if (D_8015EBD0 == 1) {
                D_8015ED14 = 1;
            }
            var_v1 = 0x3C;
            break;
        case 2:
            var_v1 = 0x3D;
            if (D_8015EBD4 == 1) {
                D_8015ED14 = 1;
            }
            break;
        }
        temp_v0 = (var_v1 * 0x44) + D_8011ECD0;
        *(u8 *)((unsigned char *)temp_v0 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (6531)) ^ 0x80);
        D_8015EC70 = var_v1;
        load_palette(D_8015EEE4, 0x18, 1, 3, 0xA);
        load_palette(D_8015EEE8, 0x19, 1, 3, 1);
        load_palette(D_8015EEE8, 0x1A, 1, 3, 0);
        D_80121133 = 0x18;
        D_801211FF = 0x18;
        D_80121177 = 0x19;
        D_80121243 = 0x19;
        D_801211BB = 0x1A;
        D_80121287 = 0x1A;
        var_s0 = D_8011ECD0;
        var_s3 = 0;
        do {
            *(s32 *)((unsigned char *)var_s0 + (9304)) = 0x51000000;
            *(s32 *)((unsigned char *)var_s0 + (9508)) = 0x41000000;
            *(s8 *)((unsigned char *)var_s0 + (9255)) = 0x80;
            *(s8 *)((unsigned char *)var_s0 + (9459)) = 0xFF;
            temp_s1 = var_s3 + 0x28;
            func_8013E97C(temp_s1, D_8015EC28, D_8015EC2C);
            temp_s2 = var_s3 + 0x2B;
            func_8013E97C(temp_s2, D_8015EC28, D_8015EC2C);
            temp_t8 = D_8015EDC4 & 0xFFFF0000;
            switch (temp_t8) {
            case 0x100000:
                func_8013E810(temp_s1, 5, 1, 1);
                func_8013E810(temp_s2, 5, 1, 1);
                break;
            case 0x200000:
                func_8013E810(temp_s1, 6, 1, 1);
                func_8013E810(temp_s2, 6, 1, 1);
                break;
            case 0x400000:
                func_8013E810(temp_s1, 7, 1, 1);
                func_8013E810(temp_s2, 7, 1, 1);
                break;
            default:
            case 0x8000000:
                func_8013E810(temp_s1, 8, 1, 1);
                func_8013E810(temp_s2, 8, 1, 1);
                break;
            }
            var_s3 += 1;
            var_s0 += 0x44;
        } while (var_s3 < 3);
        if (D_8015ED14 == 1) {
            D_801210F3 |= 1;
            temp_s0 = (2 * 0x44) + D_8011ECD0;
            D_80121137 |= 1;
            *(u8 *)((unsigned char *)temp_s0 + (9319)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (9319)) | 1);
            *(u8 *)((unsigned char *)temp_s0 + (9387)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (9387)) | 1);
            *(u8 *)((unsigned char *)temp_s0 + (9455)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (9455)) | 1);
            *(u8 *)((unsigned char *)temp_s0 + (9251)) = (u8) (*(u8 *)((unsigned char *)temp_s0 + (9251)) | 1);
        }
        func_8013F0F4(0x50B, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        var_s0_2 = (s32) (func_800A0070((s32) (D_8015EDB8 << 0xB) / 1000) * 8) >> 0xC;
        if (var_s0_2 >= 8) {
            var_s0_2 = 7;
        }
        func_8013E97C(0x28, D_8015EC28 - var_s0_2, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - var_s0_2, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + var_s0_2, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + var_s0_2, D_8015EC2C);
        var_v1_2 = D_8015EDB8 + 0xA;
        if (var_v1_2 >= 0x1F5) {
            var_v1_2 += 0xA;
        }
        D_8015EDB8 = var_v1_2;
        if (var_v1_2 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 2:
        var_s0_3 = (s32) (func_800A0070((s32) (D_8015EDB8 << 0xB) / 1000) * 0x10) >> 0xC;
        if (var_s0_3 >= 0x10) {
            var_s0_3 = 0xF;
        }
        func_8013E97C(0x28, D_8015EC28 + var_s0_3, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 + var_s0_3, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 - var_s0_3, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 - var_s0_3, D_8015EC2C);
        var_v1_3 = D_8015EDB8 + 0xA;
        if (var_v1_3 >= 0x1F5) {
            var_v1_3 += 0xA;
        }
        D_8015EDB8 = var_v1_3;
        if (var_v1_3 >= 0x3E9) {
            func_8013F0F4(0x50B, 1, 0);
            func_8014EBD0();
        }
        break;
    case 3:
        var_s0_4 = (s32) (func_800A0070((s32) (D_8015EDB8 << 0xB) / 1000) << 5) >> 0xC;
        if (var_s0_4 >= 0x20) {
            var_s0_4 = 0x1F;
        }
        func_8013E97C(0x28, D_8015EC28 - var_s0_4, D_8015EC2C);
        func_8013E97C(0x2B, D_8015EC28 - var_s0_4, D_8015EC2C);
        func_8013E97C(0x2A, D_8015EC28 + var_s0_4, D_8015EC2C);
        func_8013E97C(0x2D, D_8015EC28 + var_s0_4, D_8015EC2C);
        var_v1_4 = D_8015EDB8 + 0xA;
        if (var_v1_4 >= 0x1F5) {
            var_v1_4 += 0xA;
        }
        D_8015EDB8 = var_v1_4;
        if (var_v1_4 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015528C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80155868);
