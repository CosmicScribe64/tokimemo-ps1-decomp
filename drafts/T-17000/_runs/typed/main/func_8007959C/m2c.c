extern ? D_8013E850;

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
        ((? (*)()) D_80123110)();
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
