void func_8015BD30(s32 arg0, void *arg1) {
    s32 sp50;
    TcActor *temp_v0;
    TcActor *temp_v0_2;
    s16 temp_s7;
    s16 var_a2;
    s16 var_s2;
    s32 var_s1;
    s32 var_s4;
    u8 *var_s3;
    void *var_v0;

    temp_v0 = &D_8015EDB4[arg0];
    var_a2 = temp_v0->unk82;
    if (var_a2 >= *(s16 *)((unsigned char *)arg1 + (6))) {
        temp_v0->unk82 = 0;
        var_a2 = D_8015EDB4[arg0].unk82;
    }
    var_s4 = 0;
    var_v0 = arg1;
    var_s3 = D_80161D9C;
loop_3:
    if ((var_a2 % (s16) *(s16 *)((unsigned char *)arg1 + (4))) >= *(s16 *)((unsigned char *)var_v0 + (8))) {
        var_v0 += 4;
        goto loop_3;
    }
    var_s1 = 0;
    sp50 = (s32) *(s16 *)((unsigned char *)var_v0 + (10));
    var_s2 = arg1->unk0;
    temp_s7 = *(s16 *)((unsigned char *)arg1 + (2));
loop_7:
    if (arg0 == *var_s3) {
        if (var_s4 >= var_s2) {
            temp_v0_2 = &D_8015EDB4[arg0];
            if (((s16) temp_v0_2->unk82 % (s16) *(s16 *)((unsigned char *)arg1 + (4))) == 0) {
                func_80099E30((s32) &temp_v0_2->pad4[0x10], &D_80127480[var_s1]);
                func_8015B2AC(var_s1, D_80161E60[D_8015EDB4[arg0].unk84[2]][var_s2]);
            } else {
                func_8015B3B0(var_s1, D_80161E60[temp_v0_2->unk84[2]][var_s2], sp50);
            }
            var_s2 += 1;
        }
        var_s4 += 1;
    }
    var_s1 += 1;
    if (temp_s7 >= var_s2) {
        var_s3 += 1;
        if (var_s1 != 0x40) {
            goto loop_7;
        }
    }
}
