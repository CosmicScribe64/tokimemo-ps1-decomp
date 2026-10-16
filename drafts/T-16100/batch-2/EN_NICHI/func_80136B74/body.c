void func_80136B74(void) {
    s32 var_s0;

    switch (D_800E738A) {
    case 0:
        func_80085E30(1, 0);
        func_8006BD6C(0);
        hizuke_disp_switch(0);
        message_disp_switch(0);
        func_800AE120(D_800E7374);
        func_80044750(0xCF);
        func_8004284C();
        return;
    case 1:
        cd_read_sectors(0x6CU, 0x801B0000, 0x7D0B);
        func_8004284C();
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            D_800E62BA = 0;
            func_80132040();
            var_s0 = 0;
            do {
                load_palette(D_801391F4, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 2);
            func_8004E44C(0, (u32 *) D_801391FC, D_801391E0);
            srn_init(0, D_801391F8, 0);
            srn_vram_set(0, 5, 0x580, 0xF0);
            Fade_In_Init();
            func_8004284C();
            return;
        }
        return;
    case 3:
        func_80044750(0x203);
        func_8004284C();
        return;
    case 4:
        Fade_In_Main();
        if (D_800E7208 & 0x60) {
            func_80044750(0x74);
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    case 5:
        if (Fade_Out_Main() != 0) {
            D_800E74BC = -1;
            D_800E753C = -1;
            D_800E74C0 = -1;
            D_800E7540 = -1;
            D_800E6280.unk_1228[8] = -1;
            D_800E6280.unk_1228[40] = -1;
            D_800E6280.unk_1228[9] = -1;
            D_800E6280.unk_1228[41] = -1;
            D_800E6280.unk_1228[10] = -1;
            D_800E6280.unk_1228[42] = -1;
            D_800E6280.unk_1228[7] = -1;
            D_800E6280.unk_1228[39] = -1;
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    default:
        func_80046500();
        break;
    }
}
