u8 func_80144198(s32 arg0) {
    GameState *g = &D_800E6280;
    GekoMonthDays sp18;
    s16 temp_t0;
    s32 var_v0;
    s32 var_v1;
    u8 var_a0;
    u8 var_a1;

    sp18 = D_80147C54;
    if (g->unk_03E == 0x60) {
        sp18.day[1] = 0x1D;
    }
    temp_t0 = sp18.day[g->unk0 - 1];
    var_v1 = (g->unk0 * 0) + 6;
    var_a1 = g->unk_03E;
    var_a0 = g->unk0;
    if (temp_t0 < var_v1) {
        var_a0 = g->unk0 + 1;
        var_v1 -= temp_t0;
        if ((s32) var_a0 >= 0xD) {
            var_a0 = 1;
            var_a1 = g->unk_03E + 1;
        }
    }
    if (arg0 == 0) {
        return var_a1;
    }
    var_v0 = var_v1;
    if (arg0 == 1) {
        var_v0 = (s32) var_a0;
    }
    return (u8) var_v0;
}
