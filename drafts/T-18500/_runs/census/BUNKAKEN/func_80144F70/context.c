#include "common.h"
#include "ovl/BUNKAKEN.h"

/* bulk_m2c declarations (T-11190) */
extern s32 D_800CA2B3;
extern s32 D_800E6328;
extern s32 D_800E64E8;
extern s32 D_80122ED0;
extern s8 D_800CA2B1;
extern s8 D_800CA2B2;
void func_80145500();
/* end bulk_m2c declarations */
void func_80144F70(void) {
    typedef struct { u8 b[11]; } UnalignedBytes11; /* size 0xB */
    BunkakenBytes11 sp3C;
    u8 *var_v0;
    u8 *var_v1;
    s32 temp_a0;
    s32 temp_a0_2;
    u32 temp_t1;
    u32 temp_t4;
    u32 temp_t5;
    u32 temp_t6;
    u8 *var_v0_2;

    sp3C = D_80155F90;
    func_80083808();
    switch (D_80122EC0) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        func_80085E30(1, 0);
        *D_800CA2B0 = (u8) ((u32) (D_800E6280.unk_1BC[0].unk_0C.w << 0x1E) >> 0x1F);
        D_800CA2B1 = (s8) ((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F);
        var_v0 = &D_800E6328;
        var_v1 = &D_800CA2B3;
        D_800CA2B2 = (s8) ((u32) (D_800E6280.unk_1BC[2].unk_0C.w << 0x1E) >> 0x1F);
        do {
            temp_t5 = (u32) (*(s32 *)((unsigned char *)var_v0 + (456)) << 0x1E) >> 0x1F;
            temp_t4 = (u32) (*(s32 *)((unsigned char *)var_v0 + (624)) << 0x1E) >> 0x1F;
            temp_t1 = (u32) (*(s32 *)((unsigned char *)var_v0 + (568)) << 0x1E) >> 0x1F;
            temp_t6 = (u32) (*(s32 *)((unsigned char *)var_v0 + (512)) << 0x1E) >> 0x1F;
            var_v0 += 0xE0;
            *(s8 *)((unsigned char *)var_v1 + (1)) = (s8) temp_t6;
            *(s8 *)((unsigned char *)var_v1 + (2)) = (s8) temp_t1;
            *(s8 *)((unsigned char *)var_v1 + (3)) = (s8) temp_t4;
            var_v1 += 4;
            *(s8 *)((unsigned char *)var_v1 + (-4)) = (s8) temp_t5;
        } while (var_v0 != &D_800E64E8);
        func_8008585C();
        dec_bg_reset();
        message_window_init();
        k_reset(0);
        func_8006612C((s32) "文化祭");
        func_80065F34(0);
        func_8006764C(0);
        hizuke_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        D_80122EA4 = 0xFF;
        D_80122E98 = 0;
        D_80122EC0 += 1;
        D_80122EBC = 0xFF;
        break;
    case 1:
        set_dec_bri(0);
        D_80122EC0 += 1;
        break;
    case 2:
        if (func_80044E8C() == 1) {
            func_80044750(0x200);
            dec_bg_cd_read(0x4029, 1);
            D_80122EC0 += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(5U, 0x801E8000, 0x49BA);
            func_80065F34(1);
            D_80122EC0 += 1;
        }
        break;
    case 4:
        normal_date_bg_fadein();
        func_80145500();
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(0U);
            func_801324E0();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_80155E90;
            D_800CA164 = (s32) D_80155E94;
            D_800CA168 = (s32) D_80155E98;
            D_80122EC0 += 1;
        }
        break;
    case 6:
        normal_date_speak();
        func_80145500();
        break;
    case 7:
        func_80145E40();
        break;
    case 8:
        if ((D_800E6280.unk_03F == (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 1) && ((D_80122EBC == 0) || (D_80122EBC == 0xB))) {
            D_80122EC0 = 9;
            D_800E6280.unk_1109 = 0;
            D_800E6280.unk_110A = 0;
        } else {
            temp_a0 = D_80122EBC;
            if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && !(D_800E6280.unk_71E & 4) && (temp_a0 >= 4) && (temp_a0 < 0xB) && ((D_80122EBC = temp_a0, ((func_8008667C(temp_a0) < 2) != 0)) || ((D_80122EBC == 0xB) && (func_8008667C(0) < 2)))) {
                D_80122EC0 = 0xA;
                D_800E6280.unk_1109 = 0;
                D_800E6280.unk_110A = 0;
            } else if (D_800E6280.unk_1109 == 0) {
                func_80044750(0xBF);
                func_800674B0();
                func_80042808();
            } else if (func_80044E8C() == 1) {
                func_80145550();
                temp_a0_2 = D_80122EBC;
                if (temp_a0_2 != 0xFF) {
                    D_80122EBC = temp_a0_2;
                    if (func_8008667C(temp_a0_2) < 4) {
                        func_80145A64();
                    }
                }
                var_v0_2 = D_80122DD0;
                do {
                    var_v0_2 += 0x10;
                    *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
                    *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
                    *(s32 *)((unsigned char *)var_v0_2 + (-4)) = 0;
                    *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
                } while (var_v0_2 != &D_80122ED0);
                func_8008585C();
                dec_bg_reset();
                set_dec_bri(0);
                func_80072B5C(1);
            }
        }
        break;
    case 9:
        func_80149144();
        break;
    case 10:
        func_8014A0E0();
        break;
    default:
        func_80046500();
        break;
    }
    if (D_80122E98 == 0) {
        Default_Disp();
    }
}

void func_80145500(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145550);

void func_80145844(void) {
    k_reset(0);
    switch (D_80122EB8) {
    case 0:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "詩集展示");
            break;
        case 0x60:
            func_8006612C((s32) "文集展示");
            break;
        case 0x61:
            func_8006612C((s32) "弁論大会");
            break;
        }
        break;
    case 1:
        func_8006612C((s32) "演劇部公演");
        break;
    case 2:
        func_8006612C((s32) "実験ショー");
        break;
    case 3:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "パソコン占い");
            break;
        case 0x60:
            func_8006612C((s32) "ゲーム");
            break;
        case 0x61:
            func_8006612C((s32) "実験ショー");
            break;
        }
        break;
    case 4:
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_8006612C((s32) "似顔絵");
            break;
        case 0x60:
            func_8006612C((s32) "公開デッサン");
            break;
        case 0x61:
            func_8006612C((s32) "油絵展示");
            break;
        }
        break;
    case 5:
        func_8006612C((s32) "演奏会");
        break;
    }
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    func_8006612C(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    func_8006612C(&D_80154B8C);
    func_80065F34(0);
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145A64);
