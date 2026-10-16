void func_80061A3C(void) {
    s32 sp60;
    s32 *var_s1;
    s32 *var_v0_2;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t9;
    s32 *var_v1;
    s32 *var_v1_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s0_3;
    s32 var_s0_4;
    s32 var_s0_5;
    s32 var_s1_2;
    s32 var_s1_3;
    s32 var_v0;

    switch (D_800E6280.unk_110D) {
    case 0:
        D_800B3CFC = 0;
        D_800B3D00 = 0;
        D_800B3CF8 = 0;
        tpage_buf_clear();
        func_80048EB8(0);
        dec_bg_reset();
        func_80041168(0xFE);
        func_8004E58C();
        func_80065F34(0);
        back_clear_switch(1);
        func_80048DAC(1);
        *(s32 *)((unsigned char *)(&D_800E6280) + (4248)) = 0x9F;
        func_8006BD6C(0);
        D_800E6280.unk_037 = 0;
        D_800E6280.unk_036 = 0;
        D_800E6280.unk_03A = 0;
        D_800E6280.unk_038 = 0;
        func_80044750(0x7F);
        func_80044890(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0);
            cd_read_sectors(1U, 0x801F0000, 0xBE13);
            D_800E6280.unk_110D += 1;
            return;
        }
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            var_s0 = 0x801F0800;
            var_a0 = 0x801F0800;
            do {
                memcpy(var_a0, 0x801F0000, 0x800);
                var_s0 += 0x800;
                var_a0 = var_s0;
            } while (var_s0 != 0x801F6800);
            D_800E6280.unk_110D += 1;
            cd_read_sectors(0xD6U, 0x80180000, 0xA0CC);
            return;
        }
        break;
    case 3:
        var_s0_2 = 1;
        if (cd_read_sync_status() & 1) {
            var_a1 = 0x10;
            do {
                load_palette(0x801CC200, var_a1 & 0xFF, 1, 2, /* extra? */ var_s0_2);
                var_s0_2 += 1;
                var_a1 = var_s0_2 + 0xF;
            } while (var_s0_2 < 0xE);
            load_palette(0x801CC200, 0x1F, 1, 2, /* extra? */ 0xF);
            load_palette(0x801CC200, 0x1E, 1, 2, /* extra? */ 0xE);
            var_s0_3 = 0;
            var_s1 = &D_80184000;
            do {
                load_tpage_buf_lock(var_s0_3 + 0x15, (s32) var_s1, 1);
                var_s0_3 += 1;
            } while (var_s0_3 < 6);
            var_s1_2 = 0x80198000;
            var_s0_4 = 0;
            do {
                load_tpage_buf_lock(var_s0_4 + 0x35, var_s1_2, 1);
                var_s0_4 += 1;
                var_s1_2 += 0x4000;
            } while (var_s0_4 < 4);
            var_s1_3 = 0x801A8000;
            var_s0_5 = 0;
            do {
                load_tpage_buf_lock(var_s0_5 + 0x25, var_s1_3, 1);
                var_s0_5 += 1;
                var_s1_3 += 0x4000;
            } while (var_s0_5 < 3);
            var_v0 = 0x801B4000;
            var_v1 = &sp60;
            do {
                *var_v1 = var_v0;
                var_v0 += 0x4000;
                var_v1 += 1;
            } while (var_v0 < 0x801D4000);
            func_8004A1A0((s32) &sp60, 0, 6, 3, 2, 0x1EF, -0xA0, -0x78, 8, 0xD);
            load_palette(0x801CC000, 0xF, 1, 3, /* extra? */ 0);
            func_80048F64(0x60);
            ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xD;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 1;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x41000000;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0xA4;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = 0x801EAB3C;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = 0x801EAB6C;
            *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = 0x801EAB20;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 8;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_800E6280.unk_10EC[0] = 0x7F;
            D_801220E2 = 0;
            back_clear_switch(0);
            srand(D_800E6280.unk_10F4);
            draw2d3d(1U, 1U);
            func_80061710();
            func_80061790();
            func_800618B0();
            func_8006190C();
            var_v0_2 = &D_800B5A42;
            var_v1_2 = &D_801230D4;
            D_801230D4 = (s32) D_800B5A40;
            do {
                temp_t7 = *(s16 *)((unsigned char *)var_v0_2 + (2));
                temp_t8 = *(s16 *)((unsigned char *)var_v0_2 + (4));
                temp_t9 = *(s16 *)((unsigned char *)var_v0_2 + (6));
                temp_t6 = (*(s32 *)var_v0_2);
                var_v0_2 += 2;
                var_v1_2 += 4;
                *(s32 *)((unsigned char *)var_v1_2 + (-8)) = (s32) temp_t7;
                *(s32 *)((unsigned char *)var_v1_2 + (-4)) = (s32) temp_t8;
                (*(s32 *)var_v1_2) = (s32) temp_t9;
                *(s32 *)((unsigned char *)var_v1_2 + (-12)) = (s32) temp_t6;
            } while (var_v0_2 != &D_800B5A5A);
            func_80042808();
        }
        break;
    }
}
