void func_8007739C(void) {
    s32 temp_v0_2;
    s32 var_s0;
    u32 temp_t6;
    u8 temp_t9;
    u8 *temp_v0;

    func_8004E58C();
    func_80065F34(0);
    var_s0 = 0;
    if (D_800E6280.unk_110D == 0) {
        do {
            if (var_s0 != D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01) {
                icon_can_use_set(var_s0, 0);
            } else {
                icon_can_use_set(var_s0, 1);
            }
            var_s0 += 1;
        } while (var_s0 != 0xE);
        temp_t9 = (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 * 0x10) | (D_800E6280.unk_F68.b[0] & 0xFF0F);
        D_800E6280.unk_F68.b[0] = temp_t9;
        temp_v0 = &D_800E6280 + ((u32) ((u32) (temp_t9 & 0xFF) >> 4) % 9U);
        *(u8 *)((unsigned char *)temp_v0 + (309)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (309)) + 1);
        if (((u8) D_800E6280.unk_F68.b[0] >> 4) == 4) {
            temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            if (temp_t6 < 0xBU) {
                D_800E6280.unk_66C[temp_t6].unk_00 += 6;
            }
        }
        temp_v0_2 = func_80077330();
        if (temp_v0_2 != -1) {
            if ((temp_v0_2 == 0) || (temp_v0_2 == 1)) {
                dec_bg_show_set(0, temp_v0_2);
                D_800E6280.unk_110D = 2;
            }
        } else {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(0, func_8005751C(0));
            if (D_800B594C == -1) {
                dec_bg_reset();
                func_80077330();
            } else {
                D_800E6280.unk_110D = 2;
            }
        }
    } else if (D_800E6280.unk_110D == 2) {
        func_80076FDC();
        if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && !(D_800E6280.unk_0F6.h & 1)) {
            cd_read_sectors(9U, 0x801E0000, 0x7D8A);
            D_800E6280.unk_110D = 3;
        } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 4) {
            cd_read_sectors(0x29U, 0x801E0000, D_800B69E0[D_801230D0]);
            D_800E6280.unk_110D = 3;
        } else {
            cd_read_sectors(0x29U, 0x801E0000, D_800B6810[D_801230D0]);
            D_800E6280.unk_110D = 3;
        }
    } else if ((D_800E6280.unk_110D == 3) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}
