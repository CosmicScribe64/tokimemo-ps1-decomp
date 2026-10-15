#include "common.h"
#include "ovl/ENDING.h"

void func_80132000(void) {
    D_8013C280 = 0x801F00B8;
    D_8013C284 = 0x801F05A8;
    D_8013C288 = 0x801F0C70;
    D_8013C28C = 0x801F0F34;
    D_8013C290 = 0x801F14C4;
    D_8013C294 = 0x801F1FF4;
    D_8013C298 = 0x801F29EC;
    D_8013C29C = 0x801F32F8;
    D_8013C2A0 = 0x801F4100;
    D_8013C2A4 = 0x801F4D18;
    D_8013C2A8 = 0x801F5BD4;
    D_8013C2AC = 0x801F6698;
    D_8013C2B0 = 0x801F6D9C;
    D_8013C2B4 = 0x801F70B4;
    D_8013C2B8 = 0x801F759C;
    D_8013C2BC = 0x801F7A70;
    D_8013C2C0 = 0x801F7F98;
    D_8013C2C4 = 0x801F00D0;
    D_8013C2C8 = 0x801F05C4;
    D_8013C2CC = 0x801F0C8C;
    D_8013C2D0 = 0x801F0F44;
    D_8013C2D4 = 0x801F14E4;
    D_8013C2D8 = 0x801F2028;
    D_8013C2DC = 0x801F2A0C;
    D_8013C2E0 = 0x801F331C;
    D_8013C2E4 = 0x801F4144;
    D_8013C2E8 = 0x801F4D30;
    D_8013C2EC = 0x801F5C1C;
    D_8013C2F0 = 0x801F66B4;
    D_8013C2F4 = 0x801F6DB8;
    D_8013C2F8 = 0x801F70BC;
    D_8013C2FC = 0x801F75B8;
    D_8013C300 = 0x801F7AAC;
    D_8013C304 = 0x801F7FB0;
    D_8013C308 = 0x801F01A0;
    D_8013C30C = 0x801F07F8;
    D_8013C310 = 0x801F0EC0;
    D_8013C314 = 0x801F0F84;
    D_8013C318 = 0x801F17DC;
    D_8013C31C = 0x801F24FC;
    D_8013C320 = 0x801F2CCC;
    D_8013C324 = 0x801F3684;
    D_8013C328 = 0x801F476C;
    D_8013C32C = 0x801F4F2C;
    D_8013C330 = 0x801F62EC;
    D_8013C334 = 0x801F68E8;
    D_8013C338 = 0x801F7008;
    D_8013C33C = 0x801F7114;
    D_8013C340 = 0x801F7808;
    D_8013C344 = 0x801F7CA0;
    D_8013C348 = 0x801F8174;
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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013BF04;
extern s32 D_8013BF08;
extern s32 D_8013BF14;
extern s32 D_8013BF1C;
extern s32 D_8013BF20;
extern s32 D_8013BF28;
extern s32 D_8013BF2C;
extern s32 D_8013BF30;
extern s32 D_8013BF34;
extern s32 D_8013BF38;
extern s32 D_8013BF3C;
extern u8 D_800E71DF;
/* end bulk_m2c declarations */
void func_80132420(void *arg0, u8 arg1, s32 arg2, s32 arg3, u16 arg4, u16 arg5, u8 arg6) {
    s16 sp256;
    s32 sp50[1];
    s32 *temp_v1;
    s32 *temp_v1_2;
    s32 *temp_v1_3;
    s32 *temp_v1_4;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t7;
    s32 temp_t8;
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *temp_v0_3;
    u8 *temp_v0_4;

    if (arg1 != 0xFF) {
        func_800AE0F0(&sp50, arg0);
    }
    D_80122CF0 = 0;
    D_800CA2A8 = 0;
    switch (arg1) {
    case 0x0:
        D_8013C360 = 0;
        k_speed_set(0x10);
        sp256 = -0x66;
        set_kanji_string(-0x66, 0x48, 0, (s32) "『 ", 0);
        dialogue_render(-0x58, 0x48, 0, (s32) &sp50, 0, &D_8013BF04, &D_8013BF08, 0);
        D_80122D00 = 1;
        return;
    case 0x6:
        D_8013C360 = 0;
        k_speed_set(0x10);
        sp256 = -0x66;
        set_kanji_string(-0x66, 0x48, 0, (s32) "（ ", 0);
        dialogue_render(-0x58, 0x48, 0, (s32) &sp50, 0, "）", &D_8013BF14, 0);
        D_80122D00 = 1;
        return;
    case 0x1:
        D_8013C360 = 0;
        k_speed_set(8);
        temp_v1 = arg3 + (arg4 * 4);
        temp_t6 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1 + temp_t6) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0 = *(s32 *)(*temp_v1 + temp_t6);
                func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) D_800E71DF);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_17;
        }
block_17:
        set_kanji_string(-0x90, 0x48, (s32) arg6, (s32) "「 ", 0);
        dialogue_render(-0x82, 0x48, (s32) arg6, (s32) &sp50, 0, &D_8013BF1C, &D_8013BF20, 0);
        D_80122CE4 = 1;
        return;
    case 0xC:
        D_8013C360 = 0;
        k_speed_set(8);
        temp_v1_2 = arg3 + (arg4 * 4);
        temp_t6_2 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_2 + temp_t6_2) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_2 = *(s32 *)(*temp_v1_2 + temp_t6_2);
                func_80046290((*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), (s32) D_800E71DF);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_26;
        }
block_26:
        set_kanji_string(-0x90, 0x48, (s32) arg6, (s32) "「 ", 0);
        dialogue_render(-0x82, 0x48, (s32) arg6, (s32) &sp50, 0, &D_8013BF28, &D_8013BF2C, 0);
        D_80122D18 = 1;
        return;
    case 0xD:
        D_8013C360 = 0;
        if ((u8) D_8013C364 < 0x3CU) {
            D_8013C364 = ((u8) D_8013C364 + 1) & 0xFF;
            return;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        temp_v1_3 = arg3 + (arg4 * 4);
        temp_t7 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_3 + temp_t7) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_3 = *(s32 *)(*temp_v1_3 + temp_t7);
                func_80046290((*(s32 *)temp_v0_3), *(s32 *)((unsigned char *)temp_v0_3 + (4)), (s32) D_800E71DF);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_37;
        }
block_37:
        dialogue_render(-0x82, (s32) D_800CA2C0, (s32) arg6, (s32) &sp50, 0, &D_8013BF30, &D_8013BF34, 0);
        D_80122D18 = 1;
        return;
    case 0xF:
        D_8013C360 = 0;
        if ((u8) D_8013C364 < 0x3CU) {
            D_8013C364 = ((u8) D_8013C364 + 1) & 0xFF;
            return;
        }
        D_8013C364 = 0;
        k_speed_set(8);
        temp_v1_4 = arg3 + (arg4 * 4);
        temp_t8 = arg5 * 4;
        if (*(s32 *)*(s32 *)(*temp_v1_4 + temp_t8) != 0) {
            if ((D_800CA2AC++ == 0) && (D_80122CE4 == 0)) {
                temp_v0_4 = *(s32 *)(*temp_v1_4 + temp_t8);
                func_80046290((*(s32 *)temp_v0_4), *(s32 *)((unsigned char *)temp_v0_4 + (4)), (s32) D_800E71DF);
                func_80044750(0x300);
            }
            if (!(func_800460EC() & 4) && (D_80122CE4 == 0)) {
                D_800CA2A8 = 1;
                return;
            }
            D_80122CF0 = 1;
            D_800CA2AC = 0;
            goto block_48;
        }
block_48:
        dialogue_render(-0x82, (s32) D_800CA2C0, (s32) arg6, (s32) &sp50, 0, &D_8013BF38, &D_8013BF3C, 0);
        D_80122CE4 = 1;
        return;
    case 0xFF:
        func_8004284C();
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132B04);

void func_80132D98(void) {
    D_800CA134 = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C;
    D_800CA140 = D_8013C350;
    D_800CA144 = D_8013C354;
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

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132F18);
