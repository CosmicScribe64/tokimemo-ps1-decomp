void func_80142B80(void) {
    s32 sp3C;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_v0;
    s32 var_v1;
    u8 var_t6;

    switch (D_800E738D) {
    case 0:
        D_80150148 = 0;
        D_800E62BA = 0x80;
        func_80044750(0xD1);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_8015018C = 0;
        D_800E62B8 = 0;
        draw2d3d(1, 0);
        func_80041584();
        func_80048EB8(0);
        back_clear_switch(1);
        tpage_buf_clear();
        if ((u8) D_800E71DF >= 0xDU) {
            if (D_800E73A4 != 0) {
                main_state_set(0x12);
                return;
            }
            main_state_set(0xC4);
            return;
        }
        *D_8015014C = 0;
        if (D_800E7200 & 0x10) {
            *D_8015014C = 1;
        }
        if (func_80143240() != 0) {
            *D_8015014C = 1;
        }
        if ((func_80143240() > 0) && (D_800E7200 & 0x10)) {
            *D_8015014C = 0;
        }
        D_80150188 = 0;
        cd_read_sectors(0x11U, 0x80180000, 0xA0BB);
        var_t6 = 1;
block_51:
        D_800E738D = var_t6;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_8015018C = 1;
            load_tpage_buf_lock(0x15, 0x80180000, 1);
            load_tpage_buf_lock(0x16, 0x80184000, 1);
            func_8004B590(0x80188000, (s32) &D_800E78BC);
            load_palette(&D_800E78BC, 0x1F, 1, 1, 0);
            func_80142A4C();
            cd_read_sectors(1U, 0x801F9000, 0xBE1D);
            D_800E738D += 1;
            return;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x52U, 0x801D0000, 0xA069);
            D_800E738D += 1;
            return;
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            load_palette((s32 *)0x801F8990, 0xF, 1, 1, 0);
            load_palette(&D_80150190, 0xE, 1, 1, 0);
            var_s0 = 0;
            var_v1 = 0x801E0000;
            do {
                sp3C = var_v1;
                load_tpage_buf_lock(var_s0 + 0x25, var_v1, 2);
                var_s0 += 1;
                var_v1 += 0x2000;
            } while (var_s0 < 9);
            var_s0_2 = 0;
            var_v0 = 0x801D0000;
            do {
                sp3C = var_v0;
                load_tpage_buf_lock(var_s0_2 + 8, var_v0, 2);
                var_s0_2 += 1;
                var_v0 += 0x2000;
            } while (var_s0_2 != 8);
            if (*D_8015014C != 0) {
                cd_read_sectors(5U, 0x80180000, 0x7BD0);
            } else {
                cd_read_sectors(1U, 0x80180000, 0xB2CC);
            }
            D_800E738D += 1;
            return;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            var_s0_3 = 0;
            if (*D_8015014C != 0) {
                var_s0_4 = 0;
                var_a1 = 0x10;
                do {
                    load_palette((s32 *)0x80180020, var_a1 & 0xFF, 1, 1, 0);
                    var_s0_4 += 1;
                    var_a1 = var_s0_4 + 0x10;
                } while (var_s0_4 != 0xE);
            } else {
                var_a1_2 = 0x10;
                do {
                    load_palette((s32 *)0x80180000, var_a1_2 & 0xFF, 1, 1, 0);
                    var_s0_3 += 1;
                    var_a1_2 = var_s0_3 + 0x10;
                } while (var_s0_3 != 0xE);
            }
            if (*D_8015014C != 0) {
                cd_read_sectors(0xDU, 0x801E8000, 0x7BEF);
                cd_read_sectors(0xDU, 0x801E0000, 0x7C23);
                cd_read_sectors(0xDU, 0x801D8000, 0x7BE2);
                cd_read_sectors(0xDU, 0x801D0000, 0x7C09);
                cd_read_sectors(0xDU, 0x801C8000, 0x7C57);
            } else {
                cd_read_sectors(0xDU, 0x801E8000, 0xB2DA);
                cd_read_sectors(0xDU, 0x801E0000, 0xB30E);
                cd_read_sectors(0xDU, 0x801D8000, 0xB2CD);
                cd_read_sectors(0xDU, 0x801D0000, 0xB31B);
                cd_read_sectors(0xDU, 0x801C8000, 0xB342);
            }
            D_800E738D += 1;
            return;
        }
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            if (*D_8015014C != 0) {
                cd_read_sectors(0xDU, 0x801C0000, 0x7C30);
                cd_read_sectors(0xDU, 0x801B8000, 0x7C3D);
                cd_read_sectors(0xDU, 0x801B0000, 0x7C16);
                cd_read_sectors(0xDU, 0x801A8000, 0x7BD5);
                cd_read_sectors(0xDU, 0x801A0000, 0x7C4A);
            } else {
                cd_read_sectors(0xDU, 0x801C0000, 0xB301);
                cd_read_sectors(0xDU, 0x801B8000, 0xB328);
                cd_read_sectors(0xDU, 0x801B0000, 0xB2F4);
                cd_read_sectors(0xDU, 0x801A8000, 0xB2BF);
                cd_read_sectors(0xDU, 0x801A0000, 0xB335);
            }
            D_800E738D += 1;
            return;
        }
        break;
    case 16:
        if (cd_read_sync_status() & 1) {
            D_8015018C = -1;
            if (*D_8015014C != 0) {
                cd_read_sectors(0xDU, 0x80198000, 0x7C64);
                cd_read_sectors(0xDU, 0x80190000, 0x7C71);
                cd_read_sectors(0xDU, 0x80188000, 0x7BFC);
                cd_read_sectors(0xDU, 0x80180000, 0x7C7E);
            } else {
                cd_read_sectors(0xDU, 0x80198000, 0xB34F);
                cd_read_sectors(0xDU, 0x80190000, 0xB35C);
                cd_read_sectors(0xDU, 0x80188000, 0xB2E7);
                cd_read_sectors(0xDU, 0x80180000, 0xB369);
            }
            D_800E738D += 1;
            return;
        }
        break;
    case 21:
        func_8004284C();
        return;
    default:
        if (cd_read_sync_status() & 1) {
            var_t6 = D_800E738D + 1;
            goto block_51;
        }
        break;
    }
}
