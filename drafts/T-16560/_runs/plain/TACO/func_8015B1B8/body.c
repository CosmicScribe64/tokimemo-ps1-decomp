void func_8015B1B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp2C[1];
    s32 sp20[8];
    s32 *var_a0;
    s32 *var_t0;
    s32 *var_v1;
    TcObj50 *temp_v0;
    s16 *var_a1;
    s16 *var_a2;
    s16 temp_t3;
    s16 temp_t4;
    s32 var_v0;

    var_a2 = arg2 + (arg3 * 0x12);
    var_t0 = &sp20;
    var_v1 = var_t0;
    var_a1 = arg1 + (arg3 * 6);
    do {
        var_v0 = 0;
        var_a0 = var_v1;
loop_2:
        temp_t3 = *var_a2;
        var_v0 += 1;
        var_a0 += 2;
        var_a2 += 2;
        *(s16 *)((unsigned char *)var_a0 + (-2)) = temp_t3;
        if (var_v0 != 3) {
            goto loop_2;
        }
        temp_t4 = *var_a1;
        var_t0 += 4;
        var_v1 += 6;
        var_a1 += 2;
        *(s32 *)((unsigned char *)var_t0 + (16)) = (s32) temp_t4;
    } while (var_t0 != &sp2C);
    temp_v0 = &D_80127480[arg0];
    *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) (*(s32 *)&sp20);
    *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) *(s32 *)((unsigned char *)(&sp20) + (4));
    *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) *(s32 *)((unsigned char *)(&sp20) + (8));
    *(s32 *)((unsigned char *)temp_v0 + (16)) = (s32) *(s32 *)((unsigned char *)(&sp20) + (12));
    *(s32 *)((unsigned char *)temp_v0 + (20)) = (s32) *(s32 *)((unsigned char *)(&sp20) + (16));
    temp_v0->unk18 = *(s32 *)((unsigned char *)(&sp20) + (20));
    temp_v0->unk1C = *(s32 *)((unsigned char *)(&sp20) + (24));
    temp_v0->unk0 = 0;
    temp_v0->unk20 = *(s32 *)((unsigned char *)(&sp20) + (28));
}
