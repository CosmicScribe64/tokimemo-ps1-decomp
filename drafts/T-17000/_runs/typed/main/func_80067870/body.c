void func_80067870(void) {
    s16 sp3A;
    s16 temp_t3;
    s16 temp_t7;
    s16 var_t0;
    s32 temp_t2;
    s32 temp_t8_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_t1;
    u32 temp_t8;
    u8 temp_t4;
    u8 temp_t8_3;

    if ((D_800B5C08 == 0) || (D_800B5C08 >= 0xE)) {
        func_800676AC(0);
        return;
    }
    func_800676AC((s32) D_800B5C08);
    if (D_800B5C08 < 0xE) {
        if (((u32) D_800E6280.unk_F88 >> 0x10) != 0) {
            D_80125C55 = (u8) ((u32) (D_80125C54 + 1) >> 1);
        }
        if (((D_800E6280.unk0 & 0x3F) == 0x3F) && (D_80125C55 != 0)) {
            D_80125C55 -= 1;
        }
        temp_t8 = (u32) (rsin((D_800E6280.unk0 & 0x7F) << 5) * D_80125C55) >> 0xC;
        sp3A = (s16) temp_t8;
        *(s32 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1868)) = (s32) (((s32) ((s16) temp_t8 * -2) / 3) << 0xC);
        temp_t8_2 = 0xF - SquareRoot0((D_80125C54 * D_80125C54) - ((s16) temp_t8 * (s16) temp_t8));
        var_t0 = D_80125C50 + sp3A;
        temp_t2 = temp_t8_2 << 0x10;
        ((DisplayActor44 *)D_8011ECD0)[27].unk_26 = var_t0;
        var_t1 = D_80125C52 + D_80125C54;
        ((DisplayActor44 *)D_8011ECD0)[27].unk_2A = (s16) (var_t1 - temp_t8_2);
        ((DisplayActor44 *)D_8011ECD0)[27].unk_18 = (s16) (D_800B5C08 - 1);
        ((DisplayActor44 *)D_8011ECD0)[28].unk_26 = (s16) D_80125C50;
        ((DisplayActor44 *)D_8011ECD0)[28].unk_2A = (s16) D_80125C52;
        if (D_800E6280.unk_F80 & 2) {
            ((DisplayActor44 *)D_8011ECD0)[28].unk_18 = 0xE;
            temp_v0 = D_800E6280.unk_F80 & 0x5000;
            switch (temp_v0) {
            case 0x1000:
                if ((u8) D_80125C54 >= 3U) {
                    temp_t8_3 = (D_80125C54 - 2) & 0xFF;
                    D_80125C55 = (u8) ((u32) (temp_t8_3 + 1) >> 1);
                    D_80125C54 = temp_t8_3;
                    var_t1 = D_80125C52 + temp_t8_3;
                }
                break;
            case 0x4000:
                if ((u8) D_80125C54 < 0xC4U) {
                    temp_t4 = (D_80125C54 + 2) & 0xFF;
                    D_80125C55 = (u8) ((u32) (temp_t4 + 1) >> 1);
                    D_80125C54 = temp_t4;
                    var_t1 = D_80125C52 + temp_t4;
                }
                break;
            }
        } else if (D_800E6280.unk_F80 & 8) {
            ((DisplayActor44 *)D_8011ECD0)[28].unk_18 = 0xE;
            temp_v0_2 = D_800E6280.unk_F80 & 0x5000;
            switch (temp_v0_2) {
            case 0x1000:
                temp_t3 = D_80125C52 - 4;
                var_t1 = temp_t3 + D_80125C54;
                D_80125C52 = temp_t3;
                break;
            case 0x4000:
                temp_t7 = D_80125C52 + 4;
                var_t1 = temp_t7 + D_80125C54;
                D_80125C52 = temp_t7;
                break;
            }
            temp_v0_3 = D_800E6280.unk_F80 & 0xA000;
            if (temp_v0_3 != 0x2000) {
                if (temp_v0_3 == 0x8000) {
                    D_80125C50 -= 4;
                    var_t0 -= 4;
                }
            } else {
                D_80125C50 += 4;
                var_t0 += 4;
            }
        } else {
            (*(s32 *)((DisplayActor44 *)D_8011ECD0)) = 0xD;
        }
        func_80049FF0((s32) D_80125C50, (s32) D_80125C52, (s32) var_t0, (s32) (s16) (var_t1 - (temp_t2 >> 0x10)), 0xF8B0C8, 0xF8B0C8, 1);
        if (D_80125C50 >= 0x201) {
            D_80125C50 = -0x200;
        }
        if (D_80125C50 < -0x200) {
            D_80125C50 = 0x200;
        }
        if (D_80125C52 >= 0x201) {
            D_80125C52 = -0x200;
        }
        if (D_80125C52 < -0x200) {
            D_80125C52 = 0x200;
        }
    }
}
