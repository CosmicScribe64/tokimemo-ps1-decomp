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

/* ovl_load_lba is assigned in every case and read after the switch: uopt promotes it and writes it back in each
 * branch. The s32 result has no return on the second path (K&R), which keeps v0 live at the exit. T-16220, T-16930 */
s32 func_80078C48(void) {
    GameState *g = &D_800E6280;
    s32 old;

    old = ovl_load_lba;
    back_clear_switch(1);
    ovl_load_buf = 0x80132000;
    D_800B6D30 = 0;
    switch (g->unk_110E) {
    case 8:
    case 0x13:
    case 0x35:
    case 0x50:
    case 0x81:
        break;
    case 0x54:
    case 0x55:
    case 0x59:
    case 0x5A:
    case 0x5B:
    case 0x5D:
    case 0x72:
        func_80057640(0xA0);
        D_800B6D30 = 1;
        break;
    }
    if ((g->unk_0F4.b[1] & 0xF) == 3) {
        D_801217D0[0].unk_00 = 0x80000000;
    }
    switch (g->unk_110E) {
    case 0x3A:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x96AA;
        break;
    case 0x82:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x934A;
        break;
    case 0x74:
    case 0x75:
    case 0x76:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x958A;
        break;
    case 0x81:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x982A;
        break;
    case 0x80:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x91CA;
        break;
    case 0x72:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x99AA;
        break;
    case 0x5F:
    case 0x60:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x952A;
        break;
    case 0x73:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x97CA;
        break;
    case 0x70:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x916A;
        break;
    case 0x71:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x910A;
        break;
    case 0x11:
    case 0x13:
    case 0x14:
    case 0x15:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x95EA;
        break;
    case 0x43:
    case 0x45:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A0A;
        break;
    case 0x63:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x92EA;
        break;
    case 0x35:
    case 0x36:
    case 0x37:
    case 0x91:
    case 0xC2:
    case 0xC3:
    case 0xC4:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x93AA;
        break;
    case 0x50:
    case 0x5E:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x922A;
        break;
    case 0x58:
    case 0x5C:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x928A;
        break;
    case 0x61:
    case 0x62:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x970A;
        break;
    case 0x56:
    case 0x57:
    case 0x59:
    case 0x5A:
    case 0x5D:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x946A;
        break;
    case 0x51:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x98EA;
        break;
    case 0x54:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9ACA;
        break;
    case 0x52:
    case 0x53:
    case 0x55:
    case 0x5B:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x988A;
        break;
    case 0x20:
    case 0x21:
    case 0x22:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x976A;
        break;
    case 0xD0:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x9A6A;
        break;
    default:
        ovl_load_sectors = 0x60;
        ovl_load_lba = 0x994A;
        break;
    }
    if (old == ovl_load_lba) {
        ovl_load_state = 3;
        return 0;
    }
    cd_read_sectors(0x60, ovl_load_buf, ovl_load_lba);
    ovl_load_state = 1;
}

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
/* end bulk_m2c declarations */
void func_8007959C(void) {
    ovl_load_buf = 0x80132000;
    switch (D_800E6280.unk_1109) {
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
        if (D_800E6280.unk_1124 == 0) {
            main_state_set((s32) D_800E6280.unk_720);
            func_80042908((s32) D_800E6280.unk_721);
            func_80042940((s32) D_800E6280.unk_722);
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
