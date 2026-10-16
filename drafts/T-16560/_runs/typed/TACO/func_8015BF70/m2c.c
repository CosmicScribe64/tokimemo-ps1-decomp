void func_8015BF70(s32 arg0) {
    TcActor *var_s2;
    s32 var_s1;
    u32 var_s0;
    u8 *var_s3;
    u8 var_s4;
    void *temp_v0;

    var_s2 = &D_8015EDB4[arg0];
    var_s0 = 0;
    if (var_s2->unk82 >= 0xF) {
        var_s2->unk82 = 0;
        var_s2 = &D_8015EDB4[arg0];
    }
    var_s4 = var_s2->unk84[2];
    var_s3 = D_80161D9C;
    var_s1 = 0;
loop_3:
    if (arg0 == *var_s3) {
        temp_v0 = D_80161E60[var_s4] + (var_s0 * 8);
        func_8015B1B8(var_s1, temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) var_s2->unk82);
        var_s0 += 1;
        var_s2 = &D_8015EDB4[arg0];
        var_s4 = var_s2->unk84[2];
    }
    var_s1 += 1;
    if (var_s0 < (u8) D_80161E20[var_s4]) {
        var_s3 += 1;
        if (var_s1 == 0x40) {

        } else {
            goto loop_3;
        }
    }
}
