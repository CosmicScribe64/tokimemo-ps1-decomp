extern ? D_80160290;
extern ? D_801603A8;

void func_801521EC(s32 arg0, s32 arg1) {
    s16 **var_s3;
    s32 temp_s4;
    s32 temp_v0;
    s32 var_s0;
    void *var_s1;
    void *var_s2;

    temp_s4 = arg0 + 0x13;
    var_s0 = arg0;
    if (arg0 < temp_s4) {
        temp_v0 = arg0 * 2;
        var_s3 = &D_801603F0[arg1];
        var_s1 = (temp_v0 * 4) + &D_80160290;
        var_s2 = temp_v0 + &D_801603A8;
        do {
            func_801472D4(var_s0, 0, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            func_801478F8(var_s0, (s32) *var_s3);
            var_s0 += 3;
            var_s1 += 0x18;
            var_s2 += 6;
            var_s3 += 4;
        } while (var_s0 < temp_s4);
    }
}
