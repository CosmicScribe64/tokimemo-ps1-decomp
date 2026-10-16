s32 func_801472D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5) {
    s32 sp38[1];
    s32 sp2C[8];
    s32 *var_a0;
    s32 *var_a1;
    s32 *var_v1;
    Tc10 *temp_v0;
    s16 temp_t7;
    s16 temp_t8;
    s32 temp_t4;
    s32 var_a2;
    s32 var_a3;
    s32 var_v0;

    var_a2 = arg2;
    var_a3 = arg3;
    if ((var_a2 & 3) || (var_a3 & 3)) {
        return -1;
    }
    if (arg4 == 0) {
        var_a2 += arg1 * 6;
    }
    if (arg5 == 0) {
        var_a3 += arg1 * 0x12;
    }
    var_a1 = &sp2C;
    var_v1 = var_a1;
    do {
        var_v0 = 0;
        var_a0 = var_v1;
loop_9:
        temp_t7 = *(s16 *)var_a3;
        var_v0 += 1;
        var_a3 += 2;
        var_a0 += 2;
        *(s16 *)((unsigned char *)var_a0 + (-2)) = temp_t7;
        if (var_v0 != 3) {
            goto loop_9;
        }
        temp_t8 = *(s16 *)var_a2;
        var_a1 += 4;
        var_v1 += 6;
        var_a2 += 2;
        *(s32 *)((unsigned char *)var_a1 + (16)) = (s32) temp_t8;
    } while (var_a1 != &sp38);
    temp_v0 = &D_80127080[arg0];
    temp_t4 = temp_v0->unk4[0];
    *(s32 *)((unsigned char *)temp_t4 + (4)) = (s32) (*(s32 *)&sp2C);
    *(s32 *)((unsigned char *)temp_t4 + (8)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (4));
    *(s32 *)((unsigned char *)temp_t4 + (12)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (8));
    *(s32 *)((unsigned char *)temp_t4 + (16)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (12));
    *(s32 *)((unsigned char *)temp_t4 + (20)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (16));
    *(s32 *)((unsigned char *)temp_t4 + (24)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (20));
    *(s32 *)((unsigned char *)temp_t4 + (28)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (24));
    *(s32 *)((unsigned char *)temp_t4 + (32)) = (s32) *(s32 *)((unsigned char *)(&sp2C) + (28));
    temp_v0->unk4[0]->unk0 = 0;
    return (s32) temp_v0;
}
