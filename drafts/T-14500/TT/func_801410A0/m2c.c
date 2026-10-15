s32 func_800A1130(s32, s16, void *, void *);        /* extern */

void func_801410A0(void *arg0) {
    s16 temp_a0;
    s16 temp_a0_3;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v1_2;
    s32 temp_a1_2;
    s32 temp_lo;
    s32 temp_s0;
    s32 temp_s1;
    s32 temp_s3;
    s32 temp_t3;
    s32 temp_t7;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s2;
    s32 var_t0;
    u32 var_a0;
    u8 temp_s4;
    void *temp_a0_2;
    void *temp_a3;
    void *var_a1;
    void *var_a3;
    void *var_s0;
    void *var_s0_2;
    void *var_s1;
    void *var_s1_2;
    void *var_t0_2;
    void *var_t1;
    void *var_v0;
    void *var_v1;

    temp_s4 = *(u8 *)((unsigned char *)arg0 + (26));
    temp_s0 = arg0->unk0 - *(s16 *)((unsigned char *)arg0 + (4));
    temp_s1 = *(s16 *)((unsigned char *)arg0 + (2)) - *(s16 *)((unsigned char *)arg0 + (6));
    *(s16 *)((unsigned char *)arg0 + (18)) = func_8014D260(temp_s1, temp_s0);
    temp_a3 = arg0 + (temp_s4 * 2);
    *(s16 *)((unsigned char *)arg0 + (18)) = (s16) (*(s16 *)((unsigned char *)arg0 + (18)) & 0xFFF);
    *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (*(s16 *)((unsigned char *)temp_a3 + (2460)) & 0xFFF);
    temp_a0 = *(s16 *)((unsigned char *)temp_a3 + (2460));
    temp_a1 = *(s16 *)((unsigned char *)arg0 + (18));
    temp_v1 = temp_a0 - temp_a1;
    var_t0 = temp_v1;
    if (temp_v1 < 0) {
        var_t0 = -temp_v1;
    }
    temp_v0 = *(s16 *)((unsigned char *)arg0 + (24));
    if (temp_v0 >= var_t0) {
        *(s16 *)((unsigned char *)temp_a3 + (2460)) = temp_a1;
        goto block_17;
    }
    if ((temp_a1 - temp_a0) < 0) {
        temp_v1_2 = temp_v0 * -1;
        if (((temp_a1 >= 0xC01) && (temp_a0 < 0x400)) || ((temp_a1 < 0x400) && (temp_a0 >= 0xC01))) {
            *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (temp_a0 + -temp_v1_2);
            return;
        }
        *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (temp_a0 + temp_v1_2);
        goto block_17;
    }
    if (((temp_a1 >= 0xC01) && (temp_a0 < 0x400)) || ((temp_a1 < 0x400) && (temp_a0 >= 0xC01))) {
        *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (temp_a0 + -temp_v0);
        return;
    }
    *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (temp_a0 + temp_v0);
block_17:
    *(s16 *)((unsigned char *)temp_a3 + (2460)) = (s16) (*(s16 *)((unsigned char *)temp_a3 + (2460)) & 0xFFF);
    *(s32 *)((unsigned char *)arg0 + (28)) = (s32) (arg0->unk0 << 0x10);
    *(s32 *)((unsigned char *)arg0 + (156)) = (s32) (*(s16 *)((unsigned char *)arg0 + (2)) << 0x10);
    temp_a0_2 = arg0 + (temp_s4 * 4);
    temp_t3 = (s32) (func_800A1130((temp_s0 * temp_s0) + (temp_s1 * temp_s1), temp_a1, arg0, temp_a3) << 0x10) / 4096;
    *(s32 *)((unsigned char *)temp_a0_2 + (284)) = temp_t3;
    temp_a1_2 = *(s32 *)((unsigned char *)arg0 + (8));
    var_s1 = arg0 + 2;
    var_a3 = arg0 + 0x40;
    if (temp_t3 < temp_a1_2) {
        *(s32 *)((unsigned char *)temp_a0_2 + (284)) = temp_a1_2;
    }
    temp_v0_2 = *(s32 *)((unsigned char *)arg0 + (12));
    var_a1 = arg0 + 0x20;
    var_s0 = arg0 + 4;
    if (temp_v0_2 < *(s32 *)((unsigned char *)temp_a0_2 + (284))) {
        *(s32 *)((unsigned char *)temp_a0_2 + (284)) = temp_v0_2;
    }
    var_s2 = 1;
    if ((s32) temp_s4 > 0) {
        var_t1 = arg0 + 0x20;
        var_t0_2 = arg0 + 0x40;
        do {
            *(s16 *)((unsigned char *)(var_a1 + (*(u8 *)((unsigned char *)arg0 + (27)) * 2)) + (2524)) = (s16) *(s16 *)((unsigned char *)var_s1 + (2460));
            *(s32 *)((unsigned char *)(var_a3 + (*(u8 *)((unsigned char *)arg0 + (27)) * 4)) + (412)) = (s32) *(s32 *)((unsigned char *)var_s0 + (284));
            var_a0 = 0;
            var_v0 = var_a1;
            if (*(u8 *)((unsigned char *)arg0 + (27)) != 0) {
                var_v1 = var_a3;
                do {
                    var_a0 += 1;
                    *(s16 *)((unsigned char *)var_v0 + (2524)) = (s16) *(s16 *)((unsigned char *)var_v0 + (2526));
                    var_v0 += 2;
                    *(s32 *)((unsigned char *)var_v1 + (412)) = (s32) *(s32 *)((unsigned char *)var_v1 + (416));
                    var_v1 += 4;
                } while (var_a0 < (u8) *(u8 *)((unsigned char *)arg0 + (27)));
            }
            var_t0_2 += 0x40;
            *(s16 *)((unsigned char *)var_s1 + (2458)) = (s16) *(s16 *)((unsigned char *)var_t1 + (2524));
            var_a1 += 0x20;
            var_s1 += 2;
            var_a3 += 0x40;
            var_s0 += 4;
            var_t1 += 0x20;
            *(s32 *)((unsigned char *)var_s0 + (276)) = (s32) *(s32 *)((unsigned char *)var_t0_2 + (348));
        } while (((temp_s4 << 6) + arg0 + 0x40) != var_t0_2);
        var_s2 = 1;
    }
    var_s1_2 = arg0 + 2;
    if ((s32) temp_s4 >= 2) {
        var_s0_2 = arg0 + 4;
        temp_s3 = temp_s4 - 1;
        do {
            temp_a0_3 = *(s16 *)((unsigned char *)var_s1_2 + (2460));
            *(s16 *)((unsigned char *)var_s1_2 + (3548)) = (s16) (temp_a0_3 - *(s16 *)((unsigned char *)var_s1_2 + (2458)));
            *(s32 *)((unsigned char *)var_s0_2 + (28)) = (s32) (*(s32 *)((unsigned char *)var_s0_2 + (24)) - ((s32) (*(s32 *)((unsigned char *)var_s0_2 + (284)) * func_800A0140((s32) temp_a0_3)) / temp_s3));
            temp_t7 = *(s32 *)((unsigned char *)var_s0_2 + (152));
            temp_lo = *(s32 *)((unsigned char *)var_s0_2 + (284)) * func_800A0070((s32) *(s16 *)((unsigned char *)var_s1_2 + (2460)));
            var_s2 += 1;
            var_s1_2 += 2;
            var_s0_2 += 4;
            *(s32 *)((unsigned char *)var_s0_2 + (152)) = (s32) (temp_t7 - (temp_lo / temp_s3));
        } while (var_s2 != temp_s4);
    }
}
