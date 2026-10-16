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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_8015528C);

void func_80155868(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    u8 *sp28;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_t3;
    s32 temp_t4;
    s32 temp_t5;
    s32 temp_t9;
    void *temp_v0;

    arg0 = 0x2F - arg0;
    temp_a3 = 0x2F - arg3;
    temp_a2 = arg2 - 0x1F;
    arg1 -= 0x1F;
    arg5 -= 0x1F;
    arg4 -= 0x1F;
    arg2 = temp_a2;
    arg3 = temp_a3;
    temp_v0 = GetWorkBase(0x24, D_8011ECA0, temp_a2, temp_a3);
    sp28 = temp_v0;
    func_8009F054((s32) temp_v0);
    SetSemiTrans(sp28, 1);
    temp_t9 = arg6 >> 0x10;
    temp_t3 = arg6 >> 8;
    temp_t4 = arg7 >> 0x10;
    temp_t5 = arg7 >> 8;
    *(s8 *)((unsigned char *)sp28 + (5)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (4)) = (s8) temp_t9;
    *(s8 *)((unsigned char *)sp28 + (6)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (13)) = (s8) temp_t3;
    *(s8 *)((unsigned char *)sp28 + (12)) = (s8) temp_t9;
    *(s8 *)((unsigned char *)sp28 + (14)) = (s8) arg6;
    *(s8 *)((unsigned char *)sp28 + (21)) = (s8) temp_t5;
    *(s8 *)((unsigned char *)sp28 + (20)) = (s8) temp_t4;
    *(s8 *)((unsigned char *)sp28 + (22)) = (s8) arg7;
    *(s8 *)((unsigned char *)sp28 + (28)) = (s8) temp_t4;
    *(s8 *)((unsigned char *)sp28 + (30)) = (s8) arg7;
    *(s8 *)((unsigned char *)sp28 + (29)) = (s8) temp_t5;
    *(s16 *)((unsigned char *)sp28 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg1);
    *(s16 *)((unsigned char *)sp28 + (16)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg0);
    *(s16 *)((unsigned char *)sp28 + (18)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
    *(s16 *)((unsigned char *)sp28 + (24)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg3);
    *(s16 *)((unsigned char *)sp28 + (26)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg4);
    *(s16 *)((unsigned char *)sp28 + (32)) = (s16) (D_800E6280.unk_014[D_8011ECA0] + arg3);
    *(s16 *)((unsigned char *)sp28 + (34)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg5);
    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (arg8 * 4)], sp28);
}
