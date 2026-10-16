void func_80147488(s32 arg0, s32 arg1, s32 arg2) {
    s32 sp54;
    s32 sp50;
    s32 sp4C;
    MATRIX sp38;
    TcObj50 *sp30;
    TcPos *sp2C;
    TcObj50 *temp_v1;
    TcPos *temp_v0;
    u8 *temp_a3;

    sp38 = *(MATRIX *)D_80125C60;
    temp_a3 = arg1 + (arg2 * 0xC);
    temp_v1 = &D_80127480[arg0];
    sp4C = (*(s32 *)temp_a3) + temp_v1->unk18;
    sp50 = *(s16 *)((unsigned char *)temp_a3 + (2)) + temp_v1->unk1C;
    sp54 = *(s16 *)((unsigned char *)temp_a3 + (4)) + temp_v1->unk20;
    temp_v0 = &D_80128880[arg0];
    temp_v0->x += *(s16 *)((unsigned char *)temp_a3 + (6));
    arg1 = (s32) (temp_a3 + 8);
    sp30 = temp_v1;
    sp2C = temp_v0;
    func_8015D0D0((s32) (temp_v0->x << 0xC) / 360, (s32) &sp38);
    sp2C->z += *(s32 *)arg1;
    arg1 += 2;
    func_8015CF30((s32) (sp2C->z << 0xC) / 360, (s32) &sp38);
    sp2C->y += *(s32 *)arg1;
    func_8015D270((s32) (sp2C->y << 0xC) / 360, (s32) &sp38);
    *(MATRIX *)sp30->unk4 = sp38;
    sp30->unk0 = 0;
}
