/* Display records of the 0x44-byte pool; DISPF reads +0x24/+0x28 as 16.16 words. T-25106. */
#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
#define DISPF(n) (((DisplayFixActor44 *)D_8011ECD0)[n])
/* FAKE: shared player/cpu[3] index, as in matched func_80138680; T-25106. */
#define RUN(n) ((&D_80149208.player)[n])
s32 func_80137784(s32 arg0) {
    s32 i;

    if (DISP(46 + (s32)RUN(arg0).unk_02).unk_02 != 0 && (arg0 != 0 || D_80122E94 == 0xFF)) {
        DISP(46 + (s32)RUN(arg0).unk_02).unk_02 = 0;
        DISP(46 + (s32)RUN(arg0).unk_02).unk_03 = 0;
        if (arg0 == 0) {
            for (i = 0; i < 3; i++) {
                DISP(104 + i).unk_02 = 0;
                DISP(104 + i).unk_03 = 0;
            }
            k_reset(0);
        }
        DISP(107 + arg0).unk_02 = 1;
        DISP(107 + arg0).unk_03 = 0xA4;
        DISP(107 + arg0).unk_26 = (((s16)RUN(arg0).unk_02 % 10) << 5) - D_80149298.halves.high + 0x1E0;
        DISP(107 + arg0).unk_2A = -((s16)RUN(arg0).unk_02 / 10) * 16 - 0x30;
        D_8014982C[arg0] = (DISPF(96 + arg0).unk_24.w - DISPF(107 + arg0).unk_24.w) >> 7;
        D_8014983C[arg0] = (DISPF(96 + arg0).unk_28.w - DISPF(107 + arg0).unk_28.w) >> 7;
        D_8014984C[arg0].w = DISPF(107 + arg0).unk_24.w - DISPF(96 + arg0).unk_24.w;
        if (D_8014982C[arg0] > 0) {
            DISP(107 + arg0).unk_03 |= 1;
        }
        D_8014981C[arg0] = 0;
        RUN(arg0).unk_03 = 0;
        RUN(arg0).unk_04 = 0;
        RUN(arg0).unk_0C = 0;
        RUN(arg0).unk_0E = 0;
        RUN(arg0).unk_10 = 0;
        RUN(arg0).unk_14 = 0;
        RUN(arg0).unk_18 = 0;
        RUN(arg0).unk_1C = 0;
        if (&D_8014984C[arg0] == D_8014984C) {
            D_80149208.player.unk_20.halves.high = -0xA0;
            D_8014929C = 0;
        }
        return 0;
    }
    if ((&DISP(46))[D_801491D8].unk_02 != 0 && D_80122E94 == 1 && arg0 == 0) {
        (&DISP(46))[D_801491D8].unk_02 = 0;
        (&DISP(46))[D_801491D8].unk_03 = 0;
        for (i = 0; i < 3; i++) {
            DISP(104 + i).unk_02 = 0;
            DISP(104 + i).unk_03 = 0;
        }
        k_reset(0);
        DISP(107).unk_16 = DISP(115).unk_16;
        DISP(107).unk_43 = DISP(115).unk_43;
        DISP(107).unk_18 = 0;
        DISP(107).unk_08 = 0;
        DISP(107).unk_02 = 1;
        DISP(107).unk_03 = 0xA4;
        DISP(107).unk_26 = (D_801491D8 << 5) - D_80149298.halves.high + 0x1E0;
        DISP(107).unk_2A = -0x30;
        D_8014982C[0] = (DISPF(96).unk_24.w - DISPF(107).unk_24.w) >> 7;
        D_8014983C[0] = (DISPF(96).unk_28.w - DISPF(107).unk_28.w) >> 7;
        D_8014984C[0].w = DISPF(107).unk_24.w - DISPF(96).unk_24.w;
        if (D_8014982C[0] > 0) {
            DISP(107).unk_03 = 0xA5;
        }
        D_8014981C[0] = 0;
        D_80149208.player.unk_03 = 0;
        D_80149208.player.unk_04 = 0;
        D_80149208.player.unk_0C = 0;
        D_80149208.player.unk_0E = 0;
        D_80149208.player.unk_10 = 0;
        D_80149208.player.unk_14 = 0;
        D_80149208.player.unk_18 = 0;
        D_80149208.player.unk_1C = 0;
        D_80149208.player.unk_20.halves.high = -0xA0;
        D_8014929C = 0;
        return 0;
    }
    D_8014984C[arg0].w += D_8014982C[arg0];
    DISP(107 + arg0).unk_26 = D_8014984C[arg0].h[1] + DISP(96 + arg0).unk_26;
    DISPF(107 + arg0).unk_28.w += D_8014983C[arg0];
    if (++D_8014981C[arg0] >= 0x80) {
        DISP(96 + arg0).unk_18 = arg0 + 0x90;
        if (&D_8014981C[arg0] == D_8014981C && D_80122E94 == 1) {
            DISP(107).unk_18 = D_80149320[D_801491DC] * 6 + 5;
        } else {
            DISP(107 + arg0).unk_18 = D_801491EC * 6 + 5;
        }
        DISP(107 + arg0).unk_26 = DISP(96 + arg0).unk_26 - 8;
        DISP(107 + arg0).unk_02 = 0x40;
        DISP(107 + arg0).unk_03 = 0xA4;
        DISPF(107 + arg0).unk_28.w = DISPF(96 + arg0).unk_28.w;
        return 1;
    }
    return 0;
}
#undef RUN
#undef DISPF
#undef DISP
