#include "common.h"
#include "ovl/ETC.h"

void func_80142B80(void) {
    /* FAKE: 12-byte pad places uopt's loop-induction spill temp at sp+0x3C
     * in the 0x50 frame; void * casts on the two tpage loop addresses keep
     * the induction variables in v1 and v0 before copying to a1. T-24129. */
    s32 pad[3];
    s32 i;

    switch (D_800E6280.unk_110D) {
    case 0:
        D_80150148 = 0;
        D_800E6280.unk_03A = 0x80;
        func_80044750(0xD1);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_8015018C = 0;
        D_800E6280.unk_038 = 0;
        draw2d3d(1, 0);
        func_80041584();
        func_80048EB8(0);
        back_clear_switch(1);
        tpage_buf_clear();
        if (D_800E6280.unk_F5F >= 0xDU) {
            if (D_800E6280.unk_1124 != 0) {
                main_state_set(0x12);
                return;
            }
            main_state_set(0xC4);
            return;
        }
        D_8015014C = 0;
        if (D_800E6280.unk_F80 & 0x10) {
            D_8015014C = 1;
        }
        if (func_80143240() != 0) {
            D_8015014C = 1;
        }
        if ((func_80143240() > 0) && (D_800E6280.unk_F80 & 0x10)) {
            D_8015014C = 0;
        }
        D_80150188 = 0;
        cd_read_sectors(0x11, 0x80180000, 0xA0BB);
        D_800E6280.unk_110D = 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_8015018C = 1;
            load_tpage_buf_lock(0x15, 0x80180000, 1);
            load_tpage_buf_lock(0x16, 0x80184000, 1);
            func_8004B590(0x80188000, D_800E6280.unk_163C);
            load_palette(D_800E6280.unk_163C, 0x1F, 1, 1, 0);
            func_80142A4C();
            cd_read_sectors(1, 0x801F9000, 0xBE1D);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x52, 0x801D0000, 0xA069);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            load_palette((void *)0x801F8990, 0xF, 1, 1, 0);
            load_palette(&D_80150190, 0xE, 1, 1, 0);
            for (i = 0; i < 9; i++) {
                load_tpage_buf_lock(i + 0x25, (void *)(i * 0x2000 + 0x801E0000), 2);
            }
            for (i = 0; i < 8; i++) {
                load_tpage_buf_lock(i + 8, (void *)(i * 0x2000 + 0x801D0000), 2);
            }
            if (D_8015014C != 0) {
                cd_read_sectors(5, 0x80180000, 0x7BD0);
            } else {
                cd_read_sectors(1, 0x80180000, 0xB2CC);
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            if (D_8015014C != 0) {
                for (i = 0; i < 0xE; i++) {
                    load_palette((void *)0x80180020, (i + 0x10) & 0xFF, 1, 1, 0);
                }
            } else {
                for (i = 0; i < 0xE; i++) {
                    load_palette((void *)0x80180000, (i + 0x10) & 0xFF, 1, 1, 0);
                }
            }
            if (D_8015014C != 0) {
                cd_read_sectors(0xD, 0x801E8000, 0x7BEF);
                cd_read_sectors(0xD, 0x801E0000, 0x7C23);
                cd_read_sectors(0xD, 0x801D8000, 0x7BE2);
                cd_read_sectors(0xD, 0x801D0000, 0x7C09);
                cd_read_sectors(0xD, 0x801C8000, 0x7C57);
            } else {
                cd_read_sectors(0xD, 0x801E8000, 0xB2DA);
                cd_read_sectors(0xD, 0x801E0000, 0xB30E);
                cd_read_sectors(0xD, 0x801D8000, 0xB2CD);
                cd_read_sectors(0xD, 0x801D0000, 0xB31B);
                cd_read_sectors(0xD, 0x801C8000, 0xB342);
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            if (D_8015014C != 0) {
                cd_read_sectors(0xD, 0x801C0000, 0x7C30);
                cd_read_sectors(0xD, 0x801B8000, 0x7C3D);
                cd_read_sectors(0xD, 0x801B0000, 0x7C16);
                cd_read_sectors(0xD, 0x801A8000, 0x7BD5);
                cd_read_sectors(0xD, 0x801A0000, 0x7C4A);
            } else {
                cd_read_sectors(0xD, 0x801C0000, 0xB301);
                cd_read_sectors(0xD, 0x801B8000, 0xB328);
                cd_read_sectors(0xD, 0x801B0000, 0xB2F4);
                cd_read_sectors(0xD, 0x801A8000, 0xB2BF);
                cd_read_sectors(0xD, 0x801A0000, 0xB335);
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            D_8015018C = -1;
            if (D_8015014C != 0) {
                cd_read_sectors(0xD, 0x80198000, 0x7C64);
                cd_read_sectors(0xD, 0x80190000, 0x7C71);
                cd_read_sectors(0xD, 0x80188000, 0x7BFC);
                cd_read_sectors(0xD, 0x80180000, 0x7C7E);
            } else {
                cd_read_sectors(0xD, 0x80198000, 0xB34F);
                cd_read_sectors(0xD, 0x80190000, 0xB35C);
                cd_read_sectors(0xD, 0x80188000, 0xB2E7);
                cd_read_sectors(0xD, 0x80180000, 0xB369);
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    case 21:
        func_8004284C();
        break;
    default:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
        }
        break;
    }
}
