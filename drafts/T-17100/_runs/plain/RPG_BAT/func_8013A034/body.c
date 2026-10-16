void func_8013A034(void) {
    s32 *var_v0_2;
    s32 temp_v0;
    u8 *var_v0;

    switch (D_8015EE34) {
    case 0:
        cd_read_sectors(0x2BU, 0x801B1000, 0x3E8C);
        func_8014F524();
    default:
        break;
    case 1:
        if (D_8015EC24 == 0) {
            func_8014F500();
        }
        if (cd_read_sync_status() & 1) {
            D_8015EC24 = 0;
            func_8014F524();
        }
        break;
    case 2:
        D_8015EE8C = 0x801C5DC8;
        D_8015EEA4 = 0x801C5DDC;
        D_8015EEBC = 0x801C5EE4;
        D_800E74FC = -1;
        D_800E7500 = -1;
        D_800E7504 = -1;
        D_800E7508 = -1;
        D_800E750C = -1;
        load_tpage_buf_lock(0x15, *(s32 *)0x801C5DC8, 1);
        load_tpage_buf_lock(0x16, *(s32 *)((unsigned char *)D_8015EE8C + (4)), 1);
        load_tpage_buf_lock(0x17, *(s32 *)((unsigned char *)D_8015EE8C + (8)), 1);
        load_tpage_buf_lock(0x18, *(s32 *)((unsigned char *)D_8015EE8C + (12)), 1);
        load_tpage_buf_lock(0x19, *(s32 *)((unsigned char *)D_8015EE8C + (16)), 1);
        var_v0 = D_8011ECD0;
        do {
            var_v0 += 0x44;
            *(s32 *)((unsigned char *)var_v0 + (4296)) = (s32) D_8015EEA4;
            *(s32 *)((unsigned char *)var_v0 + (4300)) = (s32) D_8015EEBC;
            *(s32 *)((unsigned char *)var_v0 + (4336)) = (s32) D_8015EE8C;
        } while ((u32) var_v0 < (u32) &D_8011FFF0);
        if (D_8015EB9C != 0xE) {
            var_v0_2 = &D_8011F484;
            do {
                var_v0_2 += 0x44;
                *(s32 *)((unsigned char *)var_v0_2 + (6472)) = (s32) D_8015EEB8;
                *(s32 *)((unsigned char *)var_v0_2 + (6476)) = (s32) D_8015EED0;
                *(s32 *)((unsigned char *)var_v0_2 + (6512)) = (s32) D_8015EEA0;
                *(s8 *)((unsigned char *)var_v0_2 + (6527)) = 0x1C;
            } while (var_v0_2 != &D_8011F50C);
        }
        func_8014F524();
        break;
    case 3:
        D_800E71DF = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8014F524();
        break;
    case 4:
        func_80044750(0xE4);
        func_8014F524();
        break;
    case 5:
        temp_v0 = D_8015EE3C + 1;
        D_8015EE3C = temp_v0;
        if (temp_v0 >= 0xB5) {
            func_8014F500();
        }
        if (func_800460EC() & 4) {
            func_8014F500();
        }
        break;
    }
}
