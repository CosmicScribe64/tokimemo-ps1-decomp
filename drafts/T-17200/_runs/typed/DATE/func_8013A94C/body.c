void func_8013A94C(void) {
    DateCounts22 sp3C;
    DateCounts22 sp54;
    u8 sp6B;
    s16 sp42;
    u8 sp3B;
    s16 temp_t1;
    s16 var_t1;
    s32 temp_a0;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_v1;
    u32 temp_v0;

    sp54 = D_8015B75C;
    sp3C = D_8015B774;
    if ((D_800E6280.unk_03E == 0x61) && ((u8) D_800E6280.unk_03F >= 6U)) {
        sp42 += 1;
    } else if (D_800E6280.unk0 == 0x62) {
        sp42 += 1;
    }
    D_8015BF1C = (s16) D_80122CDC;
    D_8015B65C = 0;
    sp3B = 0;
    temp_v1 = *(&D_800E6280 + (D_800E6280.unk0 * 0x38));
    temp_a0 = temp_v1 & 0xF;
    D_8015BF20 = (s16) ((u32) D_800E6280.unk_10F8 % (u32) ((sp3C.value[D_800E6280.unk0] * (D_80122CDC == 1)) + ((D_80122CDC == 0) * sp54.value[D_800E6280.unk0])));
    if (D_800E6280.unk_03F == temp_a0) {
        if ((u8) D_800E6280.unk_040 < (u32) ((u32) (temp_v1 << 0x17) >> 0x1B)) {
            D_8015B65C = 1;
        }
    } else if (((u32) ((temp_a0 - D_800E6280.unk_03F) + 0xC) % 12U) == 1) {
        D_8015B65C = 1;
    }
    D_8015B660 = 0;
    if ((((D_800E6280.unk_03E == 0x61) && ((u8) D_800E6280.unk_03F >= 3U)) || (D_800E6280.unk_03E == 0x62)) && ((func_800AE0D0() % 2) != 0)) {
        D_8015B660 = 1;
    }
    sp6B = get_g_zyotai_s(D_800E6280.unk_F5F) & 0x7F;
    temp_v0 = get_g_zyotai_h(D_800E6280.unk_F5F);
    temp_t5 = temp_v0 & 0xFF;
    if ((sp6B == 0) && (temp_t5 == 0)) {
        if (!(D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.w & 0x20)) {
            sp3B = 1;
            D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 0x20;
        }
    }
    if ((sp3B != 0) && (D_80122CDC == 0)) {
        D_8015BF1C = 4;
        goto block_44;
    }
    if ((D_8015B65C != 0) && (D_80122CDC == 0)) {
        temp_t6 = *(&D_800E6280 + (D_800E6280.unk_F5F * 0x38)) & 0xF;
        D_8015BF1C = 2;
        var_t1 = D_800E6280.unk_03E - 0x5F;
        if (((temp_t6 == 1) && (D_800E6280.unk_03F == 1)) || ((temp_t6 == 2) && ((D_800E6280.unk_03F == 1) || (D_800E6280.unk_03F == 2))) || ((temp_t6 == 3) && ((D_800E6280.unk_03F == 1) || (D_800E6280.unk_03F == 2) || (D_800E6280.unk_03F == 3)))) {
            var_t1 = D_800E6280.unk_03E - 0x60;
        }
        if (D_800E6280.unk_F5F == 6) {
            if (sp6B < 2U) {
                var_t1 = 0;
                if ((u32) (temp_v0 & 0xFF) < 2U) {
                    var_t1 = 1;
                }
            } else {
                var_t1 = 0;
            }
        }
        D_8015BF20 = var_t1;
    } else if ((D_8015B660 != 0) && (D_80122CDC == 0)) {
        D_8015BF1C = 3;
block_44:
        D_8015BF20 = 0;
    }
    temp_t1 = D_8015BF20;
    if (D_800E6280.unk_F5F == 5) {
        D_8015BF20 = temp_t1;
        if (D_8015BF1C == 0) {
            D_8015BF20 = temp_t1;
            if (temp_t1 == 3) {
                D_800E6280.unk_1BC[5].unk_0C.b[1] |= 0x40;
            }
        }
    }
    func_8004284C();
}
