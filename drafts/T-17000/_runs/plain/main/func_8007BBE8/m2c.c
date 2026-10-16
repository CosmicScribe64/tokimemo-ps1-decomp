void func_8007BBE8(s32 *arg0, s32 arg1) {
    s16 *var_a3;
    s32 *var_a2;
    s32 *var_t0;
    s32 temp_t6;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_t1;
    s32 var_t2;
    s32 var_t3;
    s32 var_v1;
    u8 temp_t8;
    void *var_t4;
    void *var_t6;
    void *var_t6_2;
    void *var_t8;

    temp_t6 = arg1 & 0xFFFF;
    var_a3 = D_80125E20[temp_t6];
    var_a2 = D_80125DA0[temp_t6];
    var_v1 = 0;
    var_t0 = arg0;
    var_t1 = 4;
    var_t2 = 8;
    var_t3 = 0xC;
    do {
        temp_v0 = *(var_v1 + arg0);
        var_v1 += 0x10;
        var_t6 = arg0 + temp_v0;
        if (temp_v0 == 0) {
            var_t6 = arg0 + *arg0;
        }
        var_a2->unk0 = (s32) (var_t6 + 0x10);
        var_a3->unk0 = (s16) *(u8 *)((unsigned char *)var_t0 + (64));
        temp_v0_2 = *(var_t1 + arg0);
        var_t1 += 0x10;
        var_t4 = arg0 + temp_v0_2;
        if (temp_v0_2 == 0) {
            var_t4 = arg0 + *arg0;
        }
        var_a2[1] = (s32) (var_t4 + 0x10);
        var_a3[1] = (s16) *(u8 *)((unsigned char *)var_t0 + (65));
        temp_v0_3 = *(var_t2 + arg0);
        var_t2 += 0x10;
        var_t8 = arg0 + temp_v0_3;
        if (temp_v0_3 == 0) {
            var_t8 = arg0 + *arg0;
        }
        var_a2[2] = (s32) (var_t8 + 0x10);
        var_a3[2] = (s16) *(u8 *)((unsigned char *)var_t0 + (66));
        temp_v0_4 = *(var_t3 + arg0);
        var_t3 += 0x10;
        var_t6_2 = arg0 + temp_v0_4;
        if (temp_v0_4 == 0) {
            var_t6_2 = arg0 + *arg0;
        }
        var_a2[3] = (s32) (var_t6_2 + 0x10);
        temp_t8 = *(u8 *)((unsigned char *)var_t0 + (67));
        var_a2 += 0x10;
        var_a3 += 8;
        var_t0 += 4;
        *(s16 *)((unsigned char *)var_a3 + (-2)) = (s16) temp_t8;
    } while (var_t3 != 0x4C);
}
