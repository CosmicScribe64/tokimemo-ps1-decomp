#include "common.h"
#include "ovl/TAIIKU.h"

void func_80141C00(void) {
    if ((func_8008667C(6) < 2) && (((u32) (D_800E6280.unk_1BC[6].unk_0C.w << 0x1E) >> 0x1F) == 1) && (D_800E6280.unk_56C[45] == 0)) {
        D_800E6280.unk_56C[45] = 1;
        func_80042808();
    } else {
        D_800E6280.unk_1109 = 0;
        D_800E6280.unk_110A = 0;
        D_80122ECC = 1;
    }
    Default_Disp();
}

void func_80141C88(void) {
    s32 temp_v0;

    temp_v0 = func_80141CDC();
    if (temp_v0 != 0) {
        if (temp_v0 == 1) {
            D_800E6280.unk_1109 = 0;
            D_800E6280.unk_110A = 0;
            D_80122ECC = 1;
            return;
        }
        func_80046500();
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8014A0C0;
/* end bulk_m2c declarations */
s32 func_80141CDC(void) {
    DisplayActor44 *var_v1;
    u8 temp_t0;

    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
        D_8014A0C0 = 0;
        func_8008585C();
        func_8004E58C();
        tpage_buf_clear();
        dec_bg_reset();
        func_80048E78();
        k_reset(0);
        func_8006612C((s32) "中庭");
        func_80065F34(0);
        message_window_init();
        D_800B593C = 0;
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            dec_bg_cd_read(0x4071, 1);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_80062CD0(0x6476);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            addr_init_bustup();
            D_800E6280.unk_F5F = 6;
            func_80083474();
            cd_read_sectors(1U, 0x801E8000, 0xB7BF);
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80084E4C();
            func_800847B8(6U);
            func_80142140();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = D_8014A0D0;
            D_800CA164 = D_8014A0D4;
            D_800CA168 = D_8014A0D8;
            func_8004284C();
        }
        break;
    case 5:
        if (check_end_k() != 0) {
            func_80065F34(1);
        }
        /* fallthrough */
    case 7:
    case 9:
    case 10:
    case 12:
    case 18:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadein();
        if (D_800E6280.unk_110A == 7) {
            func_8004500C(0, 0x202);
        }
        break;
    case 8:
        func_80044750(0x60A);
        func_80047550();
        func_8004284C();
        break;
    case 11:
        normal_date_girl_in();
        break;
    case 13:
        cd_read_sectors(0x3DU, 0x80197000, 0xBB5A);
        func_8004284C();
        break;
    case 14:
        normal_date_bggirl_fadeout();
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80145210();
            load_palette(D_8014A3A0, 0x11, 1, 2, 0);
            func_80084E90(D_8014A3A4, D_8014A3A8, D_8014A3AC, D_8014A3B0, D_8014A3B4, D_8014A3B8);
            func_800850D4(D_8014A394, D_8014A398, D_8014A390, (s32) D_8014A39C);
            var_v1 = ((DisplayActor44 *)D_8011ECD0);
            do {
                temp_t0 = *(u8 *)((unsigned char *)var_v1 + (6532));
                var_v1 += 1;
                *(s8 *)((unsigned char *)var_v1 + (6464)) = (s8) (temp_t0 | 8);
            } while (var_v1 != (((DisplayActor44 *)D_8011ECD0) + 0x88));
            dec_bg_cd_read(0x4728, 1);
            func_8004284C();
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            func_8004284C();
        }
        break;
    case 17:
        func_80085368();
        break;
    case 19:
        func_80044750(0xD1);
        func_8004284C();
        break;
    case 20:
        func_800853FC();
        break;
    case 21:
        D_8014A0C0 = 1;
        func_80085B3C(2, 0x2D);
        D_800E6280.unk_1BC[6].unk_06 += 2;
        D_800E6280.unk_1BC[6].unk_02 += 1;
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
    return D_8014A0C0;
}
