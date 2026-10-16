void func_80145C00(void) {
    s32 sp5C;
    s32 *var_v0;
    GameState *var_v0_2;
    s32 *var_a0;
    s32 var_a0_2;
    s32 var_a3;
    s32 var_v1;

    var_a0 = &sp5C;
    var_v0 = &D_80184000;
    do {
        *var_a0 = (s32) var_v0;
        var_a0 += 4;
    } while ((s32) var_v0 < 0x801A0000);
    switch (D_800E738D) {
    case 0:
        if ((u8) D_800E71DF >= 0xDU) {
            main_state_set(0xC4);
            return;
        }
        D_800E62B7 = 0xFF;
        D_800E62B6 = 0xFF;
        D_800E62B8 = 0xFF;
        back_clear_switch(1);
        draw2d3d(1, 0);
        func_80041584();
        func_80048E78();
        var_a0_2 = 0;
        var_a3 = 0xF;
        if (D_800E7200 & 0x800) {
            cd_read_sectors(0x31U, 0x80180000, D_801503C4);
            D_800E738D += 2;
        } else if ((D_800E62BE == 0x62) && (D_800E62BF == 3) && (var_v1 = 0, (D_800E62C0 == 1))) {
            var_v0_2 = &D_800E6280;
            do {
                if ((var_v0_2->unk_53C[0] != 0) || (*(u8 *)((unsigned char *)(&D_800E7D10 + var_v1) + (20)) != 0)) {
                    var_a0_2 += 1;
                } else {
                    var_a3 = var_v1;
                }
                var_v1 += 1;
                var_v0_2 += 1;
            } while (var_v1 != 0xD);
            if ((var_a3 == (D_800E71DF & 0xF)) && (var_a0_2 >= 0xC)) {
                cd_read_sectors(0x31U, 0x80180000, D_801503C4);
                D_800E738D += 2;
            } else {
                cd_read_sectors(0x21U, 0x80180000, (&D_80150390)[D_800E71DF]);
                D_800E738D += 1;
            }
        } else {
            cd_read_sectors(0x21U, 0x80180000, (&D_80150390)[D_800E71DF]);
            D_800E738D += 1;
        }
    default:
block_28:
        func_8004AC18(0xFF);
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800590CC(1);
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FF, -0x80, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            func_80042808();
            func_80042940(0);
        }
        goto block_28;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80198000, 0x1F, 1, 3, 0);
            func_80042808();
            func_80046290(0x1D000077, 0x1000007B, 0);
            func_80044750(0x300);
            func_80042940(1);
        }
        goto block_28;
    }
}
