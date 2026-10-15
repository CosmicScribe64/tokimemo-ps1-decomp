#include "common.h"
#include "game.h"

void func_800789E0(void) {
    D_800B6D30 = 0;
    ovl_load_state = 0;
    ovl_load_sectors = 0;
    ovl_load_buf = 0;
    ovl_load_lba = 0;
}

void func_80078A0C(void) {
    u8 sel = ovl_load_state; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_80078C48();
        break;
    case 1:
        ovl_load_poll();
        break;
    case 2:
        ovl_load_verify();
        break;
    case 3:
        func_80079070();
        break;
    }
    func_80078A94();
}

void func_80078A94(void) {
    s32 pad; /* FAKE: unused local above the spill slot of n, the original has the spill at sp+0x28 (T-3330) */
    s32 n;

    n = 0;
    if (D_8011F113 & 0x80) {
        hizuke_show();
        n = 1;
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
        n += 1;
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
        n += 1;
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
        n += 1;
    }
    if (D_8011ECD3 & 0x80) {
        func_8006BA40();
        n += 1;
    }
    if (D_800E6280.unk_110E == 0x45) {
        cal_base_show();
    }
    if (n != 0) {
        func_80066C08(0);
        switch (D_800B6D30) {
        case 0:
            if (D_80122D44 & 1) {
                func_8006B900();
            }
            if (D_80122D44 & 2) {
                func_80066334();
            }
            func_800578F4(get_last_gamen_mode());
            break;
        case 1:
            func_80057710();
            func_800578F4(1);
            break;
        default:
            back_clear_switch(1);
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80078C48);

void ovl_load_poll(void) {
    if (cd_read_sync_status() & 1) {
        ovl_load_state = 2;
    }
}

void ovl_load_verify(void) {
    if (file_checksum_ok(ovl_load_sectors, ovl_load_buf) != 0) {
        ovl_load_state = 3;
        func_80042458();
    } else {
        ovl_load_state = 0;
        ovl_load_lba = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079070);

s32 func_80079524(void) {
    if (D_800E6280.unk_1108 == 0xD0 || D_800E6280.unk_1108 == 0xD1) {
        return 1;
    }
    return 0;
}

s32 func_80079554(void) {
    if (D_800E6280.unk_1108 == 0x90) {
        return 1;
    }
    return 0;
}

s32 func_80079578(void) {
    if (D_800E6280.unk_1108 == 0x21) {
        return 1;
    }
    return 0;
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8013E850;
extern u8 D_800E69A0;
extern u8 D_800E69A1;
extern u8 D_800E69A2;
extern u8 D_800E7389;
extern u8 D_800E73A4;
/* end bulk_m2c declarations */
void func_8007959C(void) {
    ovl_load_buf = 0x80132000;
    switch (D_800E7389) {
    case 0:
        D_800E7D4F |= 1;
        D_800B3220 = 0;
        func_80041168(0xFF);
        func_80048EB8(0);
        func_80041584();
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        cd_read_sectors(0x60U, ovl_load_buf, 0x9A6A);
        func_80042808();
    default:
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80042808();
        }
        break;
    case 2:
        if (file_checksum_ok((s32) ovl_load_sectors, 0x80132000) != 0) {
            func_80042808();
            func_80042458();
            func_8009C5E0(0);
        } else {
            func_80042908(0);
        }
        break;
    case 3:
        D_80123110 = (u32) &D_8013E850;
        func_8009C674(0);
        func_8009C210(3);
        CloseEvent(D_8011ECA8);
        ((s32 (*)()) D_80123110)();
        func_80041168(0xFF);
        func_80042808();
        break;
    case 4:
        func_8009C674(0);
        func_8009C210(3);
        func_80042488();
        func_8004482C();
        dec_bg_reset();
        func_800452C4();
        func_80044750(0x7F);
        func_800451E0(0x801B8000);
        func_80042808();
        break;
    case 5:
        func_80079800();
        break;
    case 6:
        if (D_800E73A4 == 0) {
            main_state_set((s32) D_800E69A0);
            func_80042908((s32) D_800E69A1);
            func_80042940((s32) D_800E69A2);
        } else {
            main_state_set(0);
            func_80048EB8(0);
            draw2d3d(0U, 0U);
            func_80041584();
            tpage_buf_clear();
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800789E0", func_80079800);
