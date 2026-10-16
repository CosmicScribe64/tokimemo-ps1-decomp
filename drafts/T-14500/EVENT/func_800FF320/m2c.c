extern ? D_80121AD8;

void func_800FF320(s16 arg0, s16 arg1, s16 arg2) {
    ? sp50;
    s8 *sp4C;
    s32 temp_a2;
    s32 temp_a3;
    s8 *temp_v1;
    void *temp_v0;
    void *temp_v0_2;
    void *temp_v0_3;
    void *temp_v0_4;

    sp50.unk0 = (s32) D_80121AD8.unk0;
    *(s32 *)((unsigned char *)(&sp50) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (4));
    *(s32 *)((unsigned char *)(&sp50) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (8));
    *(s32 *)((unsigned char *)(&sp50) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (12));
    *(s32 *)((unsigned char *)(&sp50) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (16));
    *(s32 *)((unsigned char *)(&sp50) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (20));
    *(s32 *)((unsigned char *)(&sp50) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (24));
    *(s32 *)((unsigned char *)(&sp50) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80121AD8) + (28));
    if (arg0 < 0x140) {
        temp_a2 = 0xA0 - arg0;
        temp_v0 = &sp50 + ((u8) D_801218DC[arg1] * 8);
        func_800FFA4C(0x140, -0x118, temp_a2, 0x28, temp_a2 + (s16) ((arg0 / 32) + 0xA), 0x28, temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) arg2, 1);
        return;
    }
    if (arg0 < 0x154) {
        temp_v1 = &D_801218DC[arg1];
        temp_v0_2 = &sp50 + ((u8) *temp_v1 * 8);
        sp4C = temp_v1;
        func_800FFA4C(0x140, -0x118, -0xA0, 0x28 - (s16) ((s32) ((arg0 * 7) - 0x8C0) / 8), -0xA0, 0x28, temp_v0_2->unk0, *(s32 *)((unsigned char *)temp_v0_2 + (4)), (s32) arg2, 1);
        temp_v0_3 = &sp50 + ((u8) *temp_v1 * 8);
        func_800FFA4C(0x140, -0x118, -0xA0, 0x28, ((s16) ((arg0 / 32) + 0xA) - arg0) + 0xA0, 0x28, temp_v0_3->unk0, *(s32 *)((unsigned char *)temp_v0_3 + (4)), (s32) arg2, 1);
        return;
    }
    temp_a3 = 0x28 - (s16) ((s32) ((arg0 * 7) - 0x8C0) / 8);
    temp_v0_4 = &sp50 + ((u8) D_801218DC[arg1] * 8);
    func_800FFA4C(0x140, -0x118, -0xA0, temp_a3, -0xA0, temp_a3 + (s16) (0xF - ((s32) (arg0 - 0x140) / 32)), temp_v0_4->unk0, *(s32 *)((unsigned char *)temp_v0_4 + (4)), (s32) arg2, 1);
}
