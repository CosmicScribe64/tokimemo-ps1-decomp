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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154760);

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EC5C;
extern s32 D_8015EC64;
extern s32 D_8015EC9C;
/* end bulk_m2c declarations */
s32 func_80154DD8(void) {
    s32 sp48;
    s32 sp44;
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_a1;
    s32 temp_a1_2;
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v0;

    func_8013F0F4(0x50A, 0, 1);
    temp_v0 = D_8015EDB0.unk_00;
    if (((temp_v0 == 1) && (D_8015EC9C >= 0x3D)) || (temp_v0 >= 2)) {
        func_8013F0F4(0x50C, 0xA, 7);
    }
    switch (temp_v0) {
    case 0:
        D_8015EC9C = 1;
        D_8015EC74 = 0xA;
        D_8015EC78 = 0;
        D_8015EC7C = 0xA;
        D_8015EC80 = 0;
        switch (D_8015EBAC) {
        default:
            D_8015EC68 = 0xB8;
            D_8015EC5C = 0x48;
            D_8015EC64 = -0x50;
            break;
        case 0:
            D_8015EC68 = 0x70;
            D_8015EC5C = 0x10;
            D_8015EC64 = -0x30;
            break;
        case 1:
            D_8015EC68 = 0xA8;
            D_8015EC5C = 0x24;
            D_8015EC64 = -0x1C;
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        case 2:
            D_8015EC68 = 0xB0;
            D_8015EC5C = -0x18;
            D_8015EC64 = -0x50;
            break;
        }
        func_8014EBD0();
        break;
    case 1:
        temp_a1 = -((s32) (D_8015EC5C * D_8015EC9C) / (s32) D_8015EC68);
        sp48 = temp_a1;
        temp_a2 = -((s32) (D_8015EC64 * D_8015EC9C) / (s32) D_8015EC68);
        sp44 = temp_a2;
        func_8015528C(D_8015EC9C, temp_a1, temp_a2, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC9C, temp_a1, temp_a2, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0;
        if (D_8015EC68 < temp_a0) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 2:
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, -D_8015EC5C, -D_8015EC64, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0_2 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0_2;
        if (temp_a0_2 >= 0x79) {
            func_8014EBD0();
            D_8015EC9C = 1;
        }
        break;
    case 3:
        temp_a2_2 = -(D_8015EC9C + D_8015EC64);
        temp_a1_2 = -(D_8015EC5C - D_8015EC9C);
        sp48 = temp_a1_2;
        sp44 = temp_a2_2;
        func_8015528C(D_8015EC68, temp_a1_2, temp_a2_2, 0x30, 5, 5, 0x34, 0x35, 0xBF);
        func_8015528C(D_8015EC68, temp_a1_2, temp_a2_2, 0xC, 0x14, 4, 0x32, 0x33, 0xDF);
        temp_a0_3 = D_8015EC9C + 1;
        D_8015EC9C = temp_a0_3;
        if ((D_8015EC5C - temp_a0_3) < (temp_a0_3 + D_8015EC64)) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015528C);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80155868);
