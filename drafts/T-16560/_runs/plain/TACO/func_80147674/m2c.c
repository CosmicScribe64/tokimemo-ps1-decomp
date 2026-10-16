void func_80147674(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    ? sp40;
    TcObj50 *sp38;
    TcPos *sp34;
    TcObj50 *temp_v1;
    TcPos *temp_v0;
    s32 temp_t1;
    void *temp_s0;
    void *temp_s0_2;

    sp40.unk0 = (s32) D_80125C60->unk0;
    *(s32 *)((unsigned char *)(&sp40) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp40) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp40) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp40) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp40) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp40) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    *(s32 *)((unsigned char *)(&sp40) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    temp_s0 = arg1 + (arg2 * 0xC);
    temp_v1 = &D_80127480[arg0];
    temp_t1 = temp_s0->unk0 + temp_v1->unk18;
    sp54 = temp_t1;
    sp58 = *(s16 *)((unsigned char *)temp_s0 + (2)) + temp_v1->unk1C;
    temp_s0_2 = temp_s0 + 6;
    sp5C = *(s16 *)((unsigned char *)temp_s0 + (4)) + temp_v1->unk20;
    if ((arg3 != 0) && (arg4 != 0)) {
        sp54 = temp_t1 + ((s32) (D_8015EDB4->unk74 - temp_t1) / arg3);
        sp58 += (s32) (D_8015EDB4->unk76 - sp58) / arg4;
    }
    temp_v0 = &D_80128880[arg0];
    temp_v0->x += *(s16 *)((unsigned char *)temp_s0 + (6));
    sp38 = temp_v1;
    sp34 = temp_v0;
    func_8015D0D0((s32) (temp_v0->x << 0xC) / 360, (s32) &sp40);
    sp34->z += *(s16 *)((unsigned char *)temp_s0_2 + (2));
    func_8015CF30((s32) (sp34->z << 0xC) / 360, (s32) &sp40);
    sp34->y += *(s16 *)((unsigned char *)(temp_s0_2 + 2) + (2));
    func_8015D270((s32) (sp34->y << 0xC) / 360, (s32) &sp40);
    *(s32 *)((unsigned char *)sp38 + (4)) = (s32) sp40.unk0;
    *(s32 *)((unsigned char *)sp38 + (8)) = (s32) *(s32 *)((unsigned char *)(&sp40) + (4));
    *(s32 *)((unsigned char *)sp38 + (12)) = (s32) *(s32 *)((unsigned char *)(&sp40) + (8));
    *(s32 *)((unsigned char *)sp38 + (16)) = (s32) *(s32 *)((unsigned char *)(&sp40) + (12));
    *(s32 *)((unsigned char *)sp38 + (20)) = (s32) *(s32 *)((unsigned char *)(&sp40) + (16));
    sp38->unk18 = *(s32 *)((unsigned char *)(&sp40) + (20));
    sp38->unk1C = *(s32 *)((unsigned char *)(&sp40) + (24));
    sp38->unk0 = 0;
    sp38->unk20 = *(s32 *)((unsigned char *)(&sp40) + (28));
}
