? func_800A0ECC(? *, s16 *, s16 *);                 /* extern */

void func_80147C98(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    ? sp48;
    s16 sp44;
    s16 sp42;
    s16 sp40;
    s16 sp38;
    s16 sp34;
    s16 sp32;
    s16 sp30;
    TcPos *sp2C;
    TcObj50 *temp_v0_2;
    TcObj50 *temp_v0_3;
    TcPos *temp_v0;

    sp48.unk0 = (s32) D_80125C60->unk0;
    *(s32 *)((unsigned char *)(&sp48) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp48) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp48) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp48) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp48) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp48) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    *(s32 *)((unsigned char *)(&sp48) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    temp_v0 = &D_80128880[arg0];
    sp2C = temp_v0;
    func_8015D0D0((s32) (temp_v0->x << 0xC) / 360, (s32) &sp48);
    func_8015CF30((s32) (sp2C->z << 0xC) / 360, (s32) &sp48);
    func_8015D270((s32) (sp2C->y << 0xC) / 360, (s32) &sp48);
    temp_v0_2 = &D_80127480[arg1];
    sp40 = (s16) temp_v0_2->unk18;
    sp42 = (s16) temp_v0_2->unk1C;
    sp44 = (s16) temp_v0_2->unk20;
    func_800A0ECC(&sp48, &sp40, &sp38);
    temp_v0_3 = &D_80127480[arg0];
    sp30 = temp_v0_3->unk18 + sp38 + arg2;
    sp32 = temp_v0_3->unk1C + sp3A + arg3;
    sp34 = temp_v0_3->unk20 + sp3C + (s16) arg4;
    func_8013DB80(subroutine_arg0, subroutine_arg1, arg5, arg6);
}
