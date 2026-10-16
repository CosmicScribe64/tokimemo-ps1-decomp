void func_8013A034(void) {
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s32 temp_v0;

    switch (D_8015EE30.unk_04) {
    case 0:
        cd_read_sectors(0x2BU, 0x801B1000, 0x3E8C);
        func_8014F524();
    default:
        break;
    case 1:
        if (D_8015EBFC.unk_28 == 0) {
            func_8014F500();
        }
        if (cd_read_sync_status() & 1) {
            D_8015EBFC.unk_28 = 0;
            func_8014F524();
        }
        break;
    case 2:
        D_8015EE8C = 0x801C5DC8;
        D_8015EEA4 = 0x801C5DDC;
        D_8015EEBC = 0x801C5EE4;
        D_800E6280.unk_1228[0x15] = -1;
        D_800E6280.unk_1228[0x16] = -1;
        D_800E6280.unk_1228[0x17] = -1;
        D_800E6280.unk_1228[0x18] = -1;
        D_800E6280.unk_1228[0x19] = -1;
        load_tpage_buf_lock(0x15, *(s32 *)0x801C5DC8, 1);
        load_tpage_buf_lock(0x16, *(s32 *)((unsigned char *)D_8015EE8C + (4)), 1);
        load_tpage_buf_lock(0x17, *(s32 *)((unsigned char *)D_8015EE8C + (8)), 1);
        load_tpage_buf_lock(0x18, *(s32 *)((unsigned char *)D_8015EE8C + (12)), 1);
        load_tpage_buf_lock(0x19, *(s32 *)((unsigned char *)D_8015EE8C + (16)), 1);
        var_v0 = ((DisplayActor44 *)D_8011ECD0);
        do {
            var_v0 += 1;
            *(s32 *)&var_v0[63].unk_0C = (s32) D_8015EEA4;
            *(s32 *)&var_v0[63].unk_10 = (s32) D_8015EEBC;
            *(s32 *)&var_v0[63].unk_34 = (s32) D_8015EE8C;
        } while ((u32) var_v0 < (u32) (&((DisplayActor44 *)D_8011ECD0)[72]));
        if (D_8015EB9C != 0xE) {
            var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[29];
            do {
                var_v0_2 += 1;
                *(s32 *)&var_v0_2[95].unk_0C = (s32) D_8015EEB8;
                *(s32 *)&var_v0_2[95].unk_10 = (s32) D_8015EED0;
                *(s32 *)&var_v0_2[95].unk_34 = (s32) D_8015EEA0;
                var_v0_2[95].unk_43 = 0x1C;
            } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[31]));
        }
        func_8014F524();
        break;
    case 3:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8014F524();
        break;
    case 4:
        func_80044750(0xE4);
        func_8014F524();
        break;
    case 5:
        temp_v0 = D_8015EE30.unk_0C + 1;
        D_8015EE30.unk_0C = temp_v0;
        if (temp_v0 >= 0xB5) {
            func_8014F500();
        }
        if (func_800460EC() & 4) {
            func_8014F500();
        }
        break;
    }
}
