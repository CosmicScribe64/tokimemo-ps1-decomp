#include "common.h"
#include "ovl/ENDING.h"
#include "game.h"

void func_80132000(void) {
    D_8013C280[0][0] = 0x801F00B8;
    D_8013C280[0][1] = 0x801F05A8;
    D_8013C280[0][2] = 0x801F0C70;
    D_8013C280[0][3] = 0x801F0F34;
    D_8013C280[0][4] = 0x801F14C4;
    D_8013C280[0][5] = 0x801F1FF4;
    D_8013C280[0][6] = 0x801F29EC;
    D_8013C280[0][7] = 0x801F32F8;
    D_8013C280[0][8] = 0x801F4100;
    D_8013C280[0][9] = 0x801F4D18;
    D_8013C280[0][10] = 0x801F5BD4;
    D_8013C280[0][11] = 0x801F6698;
    D_8013C280[0][12] = 0x801F6D9C;
    D_8013C280[0][13] = 0x801F70B4;
    D_8013C280[0][14] = 0x801F759C;
    D_8013C280[0][15] = 0x801F7A70;
    D_8013C280[0][16] = 0x801F7F98;
    D_8013C280[1][0] = 0x801F00D0;
    D_8013C280[1][1] = 0x801F05C4;
    D_8013C280[1][2] = 0x801F0C8C;
    D_8013C280[1][3] = 0x801F0F44;
    D_8013C280[1][4] = 0x801F14E4;
    D_8013C280[1][5] = 0x801F2028;
    D_8013C280[1][6] = 0x801F2A0C;
    D_8013C280[1][7] = 0x801F331C;
    D_8013C280[1][8] = 0x801F4144;
    D_8013C280[1][9] = 0x801F4D30;
    D_8013C280[1][10] = 0x801F5C1C;
    D_8013C280[1][11] = 0x801F66B4;
    D_8013C280[1][12] = 0x801F6DB8;
    D_8013C280[1][13] = 0x801F70BC;
    D_8013C280[1][14] = 0x801F75B8;
    D_8013C280[1][15] = 0x801F7AAC;
    D_8013C280[1][16] = 0x801F7FB0;
    D_8013C280[2][0] = 0x801F01A0;
    D_8013C280[2][1] = 0x801F07F8;
    D_8013C280[2][2] = 0x801F0EC0;
    D_8013C280[2][3] = 0x801F0F84;
    D_8013C280[2][4] = 0x801F17DC;
    D_8013C280[2][5] = 0x801F24FC;
    D_8013C280[2][6] = 0x801F2CCC;
    D_8013C280[2][7] = 0x801F3684;
    D_8013C280[2][8] = 0x801F476C;
    D_8013C280[2][9] = 0x801F4F2C;
    D_8013C280[2][10] = 0x801F62EC;
    D_8013C280[2][11] = 0x801F68E8;
    D_8013C280[2][12] = 0x801F7008;
    D_8013C280[2][13] = 0x801F7114;
    D_8013C280[2][14] = 0x801F7808;
    D_8013C280[2][15] = 0x801F7CA0;
    D_8013C280[2][16] = 0x801F8174;
}

void func_80132334(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80133030();
        return;
    case 1:
        func_80133540();
        return;
    case 2:
        func_80133AD0();
        return;
    case 3:
        func_80133F1C();
        return;
    case 4:
        func_80135DD0();
        return;
    case 5:
        func_80137340();
        return;
    case 6:
        func_801399B4();
        return;
    case 7:
        func_8013B760();
        return;
    default:
        func_80132400();
        return;
    }
}

void func_80132400(void) {
    main_state_set(0xC2);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132420);

/* Steps the scene dialogue: D_800CA134/138 point at the page and step halfwords (D_8013C358, D_8013C35C),
 * D_800CA13C/140/144 hold the text, kind and voice tables of the scene (func_80132D98). Returns 1 when a
 * message has finished. T-20930 */
s32 func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    if (D_80122D00 != 0) {
        if (check_end_k() != 0) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                if ((u32)++D_800E6280.unk_1104.w >= 0x3C) {
                    D_80122D00 = 0;
                    k_reset(1);
                    return 1;
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && D_800E6280.unk_F80 == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122D18 = 0;
                return 1;
            }
        }
    } else if (D_80122CE4 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && D_800E6280.unk_F80 == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122CE4 = 0;
                k_reset(1);
                return 1;
            }
        }
    } else {
        D_800E6280.unk_1104.w = 0;
        func_80132420(((u8 ***)D_800CA13C)[*(s16 *)D_800CA134][*(s16 *)D_800CA138], ((u8 **)D_800CA140)[*(s16 *)D_800CA134][*(s16 *)D_800CA138], (u8)arg0, D_800CA144, *(s16 *)D_800CA134, *(s16 *)D_800CA138, (u8)arg2);
        if (D_8013C364 == 0) {
            if ((u8)arg1 != 0 && D_800CA2A8 == 0) {
                if (((u8 **)D_800CA140)[*(s16 *)D_800CA134][*(s16 *)D_800CA138] == 0xFF) {
                    *(s16 *)D_800CA134 += 1;
                    *(s16 *)D_800CA138 = 0;
                } else {
                    *(s16 *)D_800CA138 += 1;
                }
            }
        }
    }
    return 0;
}

void func_80132D98(void) {
    D_800CA134 = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C.unk_00;
    D_800CA140 = D_8013C34C.unk_04;
    D_800CA144 = D_8013C34C.unk_08;
    func_80132B04(0xFF, 1, 1);
}

void func_80132E10(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132E70(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80132EF0(void) {
    func_80044750(0x200);
    func_8004284C();
}

/* Port of main check_k_scroll (src/main/8007C030.c), with ENDING's own limits. The unsigned
 * compare keeps the operand order of the bne (as in check_k_scroll). */
/* FAKE: sp40 keeps the unused result of the second get_k_work call; without it the frame is 8
 * bytes smaller and both spill homes move. Real source unknown. T-18300 */
void func_80132F18(void) {
    s32 sp3C;
    Entry8 *sp50;
    s32 sp40;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    Entry8 *temp_v0_2;
    Entry8 *temp_v0_3;

    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp50 = (Entry8 *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp3C = temp_a0;
    sp40 = get_k_work(temp_a0);
    if (sp50->unk_02 >= 0x68) {
        var_a0 = sp3C;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (Entry8 *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->unk_02;
                    if ((u32) temp_v1 == 0x48U) {
                        temp_v0_2->unk_02 = (s16) (temp_v1 - 0x10);
                        temp_v0_2->unk_07 = 0;
                    } else {
                        temp_v0_2->unk_02 = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            D_800CA2BC = 0;
            sp50->unk_02 = 0x58;
            sp50->unk_07 = 1;
            var_a0 = sp3C;
        }
    }
    temp_v0_3 = (Entry8 *) get_k_work(var_a0);
    if (temp_v0_3->unk_02 >= 0x68) {
        D_800CA2BC = 1;
        temp_v0_3->unk_07 = 0;
    }
}
