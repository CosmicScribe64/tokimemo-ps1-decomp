s32 func_80147B24(s32 arg0, void *arg1) {
    TcActor *var_a1_2;
    s16 temp_t8;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t2;
    s32 var_a2;
    s32 var_v0;
    s32 var_v1;
    u8 *var_t0;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_t9;
    void *var_a1;
    void *var_t0_2;

    var_v1 = 0;
    var_a2 = 0;
    var_v0 = 0;
    if (arg0 > 0) {
        temp_t2 = arg0 & 3;
        if (temp_t2 != 0) {
            var_t0 = arg1 + (0 * 2);
            var_a1 = D_8015EDB4 + (0 * 0x10 * 8);
            do {
                temp_t9 = *(u8 *)((unsigned char *)var_a1 + (2309));
                temp_lo = *var_t0 * *(s16 *)((unsigned char *)var_a1 + (2298));
                var_v0 += 1;
                var_a1 += 0x88;
                var_t0 += 2;
                var_a2 += temp_t9;
                var_v1 += temp_lo;
            } while (temp_t2 != var_v0);
            if (var_v0 != arg0) {
                goto block_5;
            }
        } else {
block_5:
            var_t0_2 = arg1 + (var_v0 * 2);
            var_a1_2 = &D_8015EDB4[var_v0];
            do {
                temp_t7 = *(u8 *)((unsigned char *)var_t0_2 + (2));
                temp_lo_2 = var_t0_2->unk0 * *(s16 *)((unsigned char *)var_a1_2 + (2298));
                temp_t8 = *(s16 *)((unsigned char *)var_a1_2 + (2434));
                temp_t4 = *(u8 *)((unsigned char *)var_t0_2 + (4));
                temp_a2 = var_a2 + *(u8 *)((unsigned char *)var_a1_2 + (2309)) + *(u8 *)((unsigned char *)var_a1_2 + (2445)) + *(u8 *)((unsigned char *)var_a1_2 + (2581));
                var_v0 += 4;
                var_a1_2 += 0x220;
                var_t0_2 += 8;
                var_a2 = temp_a2 + *(u8 *)((unsigned char *)var_a1_2 + (2173));
                var_v1 = var_v1 + temp_lo_2 + (temp_t7 * temp_t8) + (temp_t4 * *(s16 *)((unsigned char *)var_a1_2 + (2026))) + (*(u8 *)((unsigned char *)var_t0_2 + (-2)) * *(s16 *)((unsigned char *)var_a1_2 + (2162)));
            } while (var_v0 != arg0);
        }
    }
    return 0x64 - ((s32) (var_a2 * 0x64) / var_v1);
}
