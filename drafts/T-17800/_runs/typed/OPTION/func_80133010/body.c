s32 func_80133010(void) {
    OptActor44 *var_v0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 temp_t7;
    u8 var_v0_2;
    u8 var_v1;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80048E78();
        D_8013D300 = 1;
        var_v0 = &((OptActor44 *)D_8011ECD0)[96];
        do {
            var_v0 += 1;
            *(s8 *)((unsigned char *)var_v0 + (-67)) = 5;
            *(s8 *)((unsigned char *)var_v0 + (-66)) = 0x40;
            *(s32 *)((unsigned char *)var_v0 + (-12)) = 0x01000000;
            *(s8 *)((unsigned char *)var_v0 + (-65)) = 0x24;
            *(s32 *)((unsigned char *)var_v0 + (-56)) = 0x801C32D8;
            *(s32 *)((unsigned char *)var_v0 + (-52)) = 0x801C34E0;
            *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x801C32C8;
            *(s8 *)((unsigned char *)var_v0 + (-64)) = 8;
            *(s16 *)((unsigned char *)var_v0 + (-44)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-63)) = 0x80;
            *(s16 *)((unsigned char *)var_v0 + (-30)) = -0x200;
            *(s16 *)((unsigned char *)var_v0 + (-26)) = -0x200;
            *(s8 *)((unsigned char *)var_v0 + (-61)) = 0x80;
        } while ((u32) var_v0 < (u32) D_80121750);
        var_s0 = 0;
        var_a1 = 0x10;
        do {
            load_palette(0x801C34F0, var_a1 & 0xFF, 1, 1, var_s0);
            var_s0 += 1;
            var_a1 = var_s0 + 0x10;
        } while (var_s0 < 4);
        load_palette(0x801B0000, 0x1F, 1, 1, 0);
        var_s0_2 = 0;
        var_a1_2 = 0x1A;
        do {
            load_palette(0x801CBEF0, var_a1_2 & 0xFF, 1, 1, var_s0_2);
            var_s0_2 += 1;
            var_a1_2 = var_s0_2 + 0x1A;
        } while (var_s0_2 != 4);
        load_tpage_buf_lock(5, 0x801C7CF0, 1);
        func_801328C0();
        ((OptActor44 *)D_8011ECD0)[96].unk_18 = 0;
        ((OptActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[96].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[96].unk_03 | 0x80);
        ((OptActor44 *)D_8011ECD0)[96].unk_01 = 8;
        ((OptActor44 *)D_8011ECD0)[97].unk_18 = 0xA;
        ((OptActor44 *)D_8011ECD0)[97].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[97].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[97].unk_03 = (u8) (((OptActor44 *)D_8011ECD0)[97].unk_03 | 0x80);
        ((OptActor44 *)D_8011ECD0)[97].unk_01 = 3;
        ((OptActor44 *)D_8011ECD0)[106].unk_18 = 7;
        ((OptActor44 *)D_8011ECD0)[106].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[106].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[106].unk_03 = 0x24;
        ((OptActor44 *)D_8011ECD0)[106].unk_01 = 1;
        ((OptActor44 *)D_8011ECD0)[159].unk_18 = 7;
        ((OptActor44 *)D_8011ECD0)[159].unk_26 = -0xA0;
        ((OptActor44 *)D_8011ECD0)[159].unk_2A = -0x78;
        ((OptActor44 *)D_8011ECD0)[159].unk_03 = 0x24;
        ((OptActor44 *)D_8011ECD0)[159].unk_01 = 1;
    }
    var_v1 = D_800E6280.unk_110D;
    if (var_v1 != 0) {
        temp_t7 = (D_800E6280.unk_03A - 4) & 0xFF;
        var_v0_2 = temp_t7;
        if (temp_t7 < 0x81U) {
            var_v0_2 = 0x80;
            if (var_v1 == 1) {
                var_v1 = 2;
            }
        }
        D_800E6280.unk_03A = var_v0_2;
        D_800E6280.unk_110D = var_v1;
        if (var_v1 == 2) {
            func_8004284C();
        }
    }
    func_8004AC18((s32) D_800E6280.unk_03A);
}
