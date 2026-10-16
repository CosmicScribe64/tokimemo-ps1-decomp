void func_801460D8(void) {
    u8 temp_a1;
    u32 temp_v1;

    switch (D_800E6280.unk_110D) {
    case 0:
        D_8014D180 = 3;
        D_801220E1 = 0;
        D_801220E2 = 0;
        D_801220E0 = 0;
        set_dec_bri(0);
        prologue_line_start();
        func_80145FB0(0x801A4000, 0xE);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        temp_a1 = ((u8 *)&D_801220E0)[0];
        if (temp_a1 < 0x81U) {
            D_801220E0 = D_801220E2 = D_801220E1 = temp_v1 = temp_a1 + 4;
            func_80057390(temp_v1 & 0xFF, temp_a1);
        }
        prologue_line_run();
        break;
    case 2:
        func_800462C8(0x31U, 0x801A4000, 0xAA23);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            dec_bg_cd_read(0xACE6, 1);
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            if (D_800B594C == -1) {
                dec_bg_reset();
                dec_bg_cd_read(0xACE6, 1);
            } else {
                func_80042808();
            }
        }
        break;
    }
    func_800578F4(1);
}
