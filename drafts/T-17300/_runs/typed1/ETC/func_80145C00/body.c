void func_80145C00(void) {
    s32 sp5C;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 *var_a0;
    s32 var_a0_2;
    s32 var_a3;
    s32 var_v1;

    var_a0 = &sp5C;
    var_v0 = &D_80184000;
    do {
        *var_a0 = (s32) var_v0;
        var_a0 += 1;
    } while ((s32) var_v0 < 0x801A0000);
    switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
    case 0:
        if ((u8) *(u8 *)((unsigned char *)(&D_800E6280) + (3935)) >= 0xDU) {
            main_state_set(0xC4);
            return;
        }
        *(s8 *)((unsigned char *)(&D_800E6280) + (55)) = 0xFF;
        *(s8 *)((unsigned char *)(&D_800E6280) + (54)) = 0xFF;
        *(s8 *)((unsigned char *)(&D_800E6280) + (56)) = 0xFF;
        back_clear_switch(1);
        draw2d3d(1, 0);
        func_80041584();
        func_80048E78();
        var_a0_2 = 0;
        var_a3 = 0xF;
        if (*(s32 *)((unsigned char *)(&D_800E6280) + (3968)) & 0x800) {
            cd_read_sectors(0x31, 0x80180000, D_801503C4, 0xF);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 2);
        } else if ((*(u8 *)((unsigned char *)(&D_800E6280) + (62)) == 0x62) && (*(u8 *)((unsigned char *)(&D_800E6280) + (63)) == 3) && (var_v1 = 0, (*(u8 *)((unsigned char *)(&D_800E6280) + (64)) == 1))) {
            var_v0_2 = &D_800E6280;
            do {
                if ((*(u8 *)((unsigned char *)var_v0_2 + (1340)) != 0) || (*(u8 *)((unsigned char *)(&D_800E7D10 + var_v1) + (20)) != 0)) {
                    var_a0_2 += 1;
                } else {
                    var_a3 = var_v1;
                }
                var_v1 += 1;
                var_v0_2 += 1;
            } while (var_v1 != 0xD);
            if ((var_a3 == (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) & 0xF)) && (var_a0_2 >= 0xC)) {
                cd_read_sectors(0x31, 0x80180000, D_801503C4, var_a3);
                *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 2);
            } else {
                cd_read_sectors(0x21, 0x80180000, *(&D_80150390 + (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) * 4)), var_a3);
                *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            }
        } else {
            cd_read_sectors(0x21, 0x80180000, *(&D_80150390 + (*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) * 4)), 0xF);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
        }
    default:
block_28:
        func_8004AC18(0xFF);
        return;
    case 1:
        if (cd_read_sync_status(var_a0) & 1) {
            func_800590CC(1);
            func_8004A1A0(&sp5C, 0, 4, 2, 2, 0x1FF, -0x80, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            func_80042808();
            func_80042940(0);
        }
        goto block_28;
    case 2:
        if (cd_read_sync_status(var_a0) & 1) {
            func_8004A1A0(&sp5C, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80198000, 0x1F, 1, 3, 0);
            func_80042808();
            func_80046290(0x1D000077, 0x1000007B, 0);
            func_80044750(0x300);
            func_80042940(1);
        }
        goto block_28;
    }
}
