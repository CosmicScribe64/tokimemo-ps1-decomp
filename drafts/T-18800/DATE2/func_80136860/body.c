s32 func_80136860(void) {
    GameState *g = &D_800E6280;
    s32 temp_t5;
    u8 var_v0;

    var_v0 = g->unk_110D;
    if (var_v0 == 0) {
        func_800674B0();
        var_v0 = (g->unk_110D + 1) & 0xFF;
    }
    g->unk_110D = var_v0;
    if (var_v0 == 1) {
        if ((u32) D_800E7384++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            g->unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            g->unk_110D += 1;
            goto block_8;
        }
        return 0;
    }
block_8:
    temp_t5 = (((D_8013A4C4 & 1) != 0) + ((D_8013A4C4 & 2) != 0) + ((D_8013A4C4 & 4) != 0)) & 0xFF;
    if (temp_t5 == 0) {
        g->unk_1BC[0].unk_06 += 0xC;
        g->unk_1BC[g->unk_75D].unk_0A += 7;
    } else if (temp_t5 == 1) {
        g->unk_1BC[0].unk_06 += 3;
        g->unk_1BC[g->unk_75D].unk_0A += 2;
    } else if (temp_t5 == 2) {
        g->unk_1BC[g->unk_75D].unk_06 += 3;
        g->unk_1BC[0].unk_0A += 2;
    } else {
        g->unk_1BC[g->unk_75D].unk_06 += 0xC;
        g->unk_1BC[0].unk_0A += 7;
    }
    check_para_limit();
    return func_80072B5C(1);
}
