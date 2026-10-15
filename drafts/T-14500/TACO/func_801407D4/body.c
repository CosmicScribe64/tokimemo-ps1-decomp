s32 func_801407D4(s32 *arg0, s32 arg1, s16 arg2) {
    s32 sp24;
    TcActor *temp_s0;
    TcActor *temp_s0_2;
    TcActor *temp_t0;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 var_a1;
    s32 var_t0;
    s32 var_t1;
    s32 var_t5;
    s32 var_v1;
    u32 temp_hi;

    *arg0 = -1;
    temp_hi = (u32) D_800E7378 % 48U;
    var_v1 = 0;
    var_a1 = 0x09C40000;
    temp_hi_2 = (s32) temp_hi % 48;
    if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_2]) + (2177)) & 0x80) {
        temp_t0 = &D_8015EDB4[temp_hi_2 + 0x10];
        if ((temp_t0->unk84[3] & 1) && (D_8015EDB4->unk84[2] != 0xE)) {
            var_v1 = 1;
            if ((temp_t0->unk78 < arg2) && (sp24 < 0x09C40000)) {
                var_a1 = sp24;
                *arg0 = ((s32) temp_hi % 48) + 0x10;
            }
        }
    }
    var_t1 = temp_hi + 1;
    var_t5 = var_t1 + 1;
    var_t0 = 1;
    do {
        temp_hi_3 = var_t1 % 48;
        var_t1 += 2;
        if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_3]) + (2177)) & 0x80) {
            temp_t2 = temp_hi_3 + 0x10;
            temp_s0 = &D_8015EDB4[temp_t2];
            if ((temp_s0->unk84[3] & 1) && (D_8015EDB4[var_t0].unk84[2] != 0xE)) {
                var_v1 += 1;
                if ((temp_s0->unk78 < arg2) && (sp24 < var_a1)) {
                    var_a1 = sp24;
                    *arg0 = temp_t2;
                }
            }
        }
        temp_hi_4 = var_t5 % 48;
        if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_4]) + (2177)) & 0x80) {
            temp_t2_2 = temp_hi_4 + 0x10;
            temp_s0_2 = &D_8015EDB4[temp_t2_2];
            if ((temp_s0_2->unk84[3] & 1) && (*(u8 *)((unsigned char *)(&D_8015EDB4[var_t0]) + (270)) != 0xE)) {
                var_v1 += 1;
                if ((temp_s0_2->unk78 < arg2) && (sp24 < var_a1)) {
                    var_a1 = sp24;
                    *arg0 = temp_t2_2;
                }
            }
        }
        var_t0 += 2;
        var_t5 += 2;
    } while (var_t0 != 0x31);
    return var_v1;
}
