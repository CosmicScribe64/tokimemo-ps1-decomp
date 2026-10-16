void func_80146F74(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    s32 sp38[8];
    TcObj50 *sp30;
    TcPos *sp2C;
    TcObj50 *temp_v1;
    TcPos *temp_v0;
    s32 temp_t5;

    (*(s32 *)&sp38) = (s32) (*(s32 *)D_80125C60);
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp38) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp38) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp38) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp38) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp38) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    *(s32 *)((unsigned char *)(&sp38) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    temp_v1 = &D_80127480[arg0];
    temp_t5 = temp_v1->unk18 + arg1;
    sp4C = temp_t5;
    sp50 = temp_v1->unk1C + arg2;
    sp54 = temp_v1->unk20 + arg3;
    if ((arg7 != 0) && (arg8 != 0)) {
        sp4C = temp_t5 + ((s32) (D_8015EDB4->unk74 - temp_t5) / arg7);
        sp50 += (s32) (D_8015EDB4->unk76 - sp50) / arg8;
    }
    temp_v0 = &D_80128880[arg0];
    temp_v0->x += arg4;
    sp2C = temp_v0;
    sp30 = temp_v1;
    func_8015D0D0((s32) (temp_v0->x << 0xC) / 360, (s32) &sp38);
    sp2C->z += arg6;
    func_8015CF30((s32) (sp2C->z << 0xC) / 360, (s32) &sp38);
    sp2C->y += arg5;
    func_8015D270((s32) (sp2C->y << 0xC) / 360, (s32) &sp38);
    *(s32 *)((unsigned char *)sp30 + (4)) = (s32) (*(s32 *)&sp38);
    *(s32 *)((unsigned char *)sp30 + (8)) = (s32) *(s32 *)((unsigned char *)(&sp38) + (4));
    *(s32 *)((unsigned char *)sp30 + (12)) = (s32) *(s32 *)((unsigned char *)(&sp38) + (8));
    *(s32 *)((unsigned char *)sp30 + (16)) = (s32) *(s32 *)((unsigned char *)(&sp38) + (12));
    *(s32 *)((unsigned char *)sp30 + (20)) = (s32) *(s32 *)((unsigned char *)(&sp38) + (16));
    sp30->unk18 = *(s32 *)((unsigned char *)(&sp38) + (20));
    sp30->unk1C = *(s32 *)((unsigned char *)(&sp38) + (24));
    sp30->unk0 = 0;
    sp30->unk20 = *(s32 *)((unsigned char *)(&sp38) + (28));
}
