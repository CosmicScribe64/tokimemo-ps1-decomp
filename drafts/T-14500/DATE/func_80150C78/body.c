void func_80150C78(void) {
    s32 sp64;
    s16 var_s1;
    s32 temp_fp;
    s32 temp_s0;
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_s4;
    s32 var_s7;
    s32 var_t1;
    u32 temp_hi;
    u32 temp_lo;

    temp_lo = (u8) D_800B593C / 10U;
    var_s7 = 0;
    var_s1 = 0;
    temp_hi = (u32) D_800E7378 % 45U;
    temp_fp = (temp_lo * 0x130000) | (temp_lo * 0x1300) | (temp_lo * 0x13);
    sp64 = temp_fp;
    if (temp_hi < 0xFU) {
        var_s7 = 0xAA;
    } else if (temp_hi < 0x1EU) {
        var_s7 = 0x155;
    }
    var_t1 = 0 << 8;
    do {
        temp_s0 = var_t1 + var_s7;
        temp_s2 = func_800A0140(temp_s0);
        temp_s3 = func_800A0070(temp_s0);
        temp_s4 = func_800A0140(temp_s0);
        func_80049FF0((s32) (s16) (temp_s2 / 20), (s32) (s16) ((temp_s3 / 20) - 0x1E), (s32) (s16) (temp_s4 / 60), (s32) (s16) ((func_800A0070(temp_s0) / 60) - 0x1E), temp_fp, sp64, 0xA);
        var_s1 += 1;
        var_t1 = var_s1 << 8;
    } while (var_s1 < 0x10);
}
