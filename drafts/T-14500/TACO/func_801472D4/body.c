/* NON_MATCHING: T-10520, frame and body match (local MATRIX at sp+0x2C, `s32 pad` declared first for the 4 spare bytes); ugen temporaries of the block copy are $t7/$t8 against $t8/$t9 and one addiu of the first loop is scheduled a slot apart, about 12 lines */
s32 func_801472D4(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
s32 arg1;
s16 *arg2;
s16 *arg3;
u8 arg4;
u8 arg5;
{
    Tc10 *rec;
    MATRIX m;
    s32 i;
    s32 j;
    s32 pad;

    if (((s32)arg2 & 3) || ((s32)arg3 & 3)) {
        return -1;
    }
    if (arg4 == 0) {
        arg2 += arg1 * 3;
    }
    if (arg5 == 0) {
        arg3 += arg1 * 9;
    }
    for (i = 0; i != 3; i++) {
        for (j = 0; j != 3; j++) {
            m.m[i][j] = *arg3++;
        }
        m.t[i] = *arg2++;
    }
    rec = &D_80127080[arg0];
    *(MATRIX *)((u8 *)rec->unk4[0] + 4) = m;
    *(s32 *)rec->unk4[0] = 0;
    return (s32)rec;
}
