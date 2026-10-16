? func_800A0ECC(? *, s16 *, s16 *, TcObj50 *);      /* extern */
extern s32 D_801270B0;
extern s32 D_801270C0;
extern s32 D_801270F0;
extern s32 D_80127100;
extern s32 D_80127110;
extern s32 D_80127120;
extern s32 D_80127498;
extern ? D_80127610;
extern ? D_801277F0;
extern s16 D_80128882;
extern s16 D_80128884;

void func_8014A480(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5) {
    ? sp60;
    s16 sp5C;
    s16 sp5A;
    s16 sp58;
    s16 sp50;
    s32 sp4C;
    s32 sp48;
    s32 sp44;
    s32 sp3C;
    TcObj50 *sp38;
    TcActor *temp_v0_2;
    TcActor *temp_v0_3;
    TcObj50 *temp_a3;
    s32 temp_v1;
    u8 var_a0;
    void *temp_v0;

    temp_v1 = arg1 * 2;
    sp4C = arg1 + 0x10;
    temp_v0 = arg2 + (temp_v1 * 2);
    sp48 = (s32) temp_v0->unk0;
    sp44 = (s32) *(s16 *)((unsigned char *)temp_v0 + (2));
    *(u8 *)((unsigned char *)(&D_8015EDB4[arg1]) + (2308)) = (u8) *(arg3 + arg1);
    sp60.unk0 = (s32) D_80125C60->unk0;
    *(s32 *)((unsigned char *)(&sp60) + (4)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (4));
    *(s32 *)((unsigned char *)(&sp60) + (8)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (8));
    *(s32 *)((unsigned char *)(&sp60) + (12)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (12));
    *(s32 *)((unsigned char *)(&sp60) + (16)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (16));
    *(s32 *)((unsigned char *)(&sp60) + (20)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (20));
    *(s32 *)((unsigned char *)(&sp60) + (24)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (24));
    *(s32 *)((unsigned char *)(&sp60) + (28)) = (s32) *(s32 *)((unsigned char *)D_80125C60 + (28));
    sp3C = temp_v1;
    func_8015D0D0((s32) (D_80128880->x << 0xC) / 360, (s32) &sp60);
    func_8015CF30((s32) (D_80128884 << 0xC) / 360, (s32) &sp60);
    func_8015D270((s32) (D_80128882 << 0xC) / 360, (s32) &sp60);
    temp_a3 = &D_80127480[arg0];
    sp38 = temp_a3;
    sp58 = (s16) temp_a3->unk18;
    sp5A = (s16) temp_a3->unk1C;
    sp5C = (s16) temp_a3->unk20;
    func_800A0ECC(&sp60, &sp58, &sp50, temp_a3);
    D_8015EDB4[sp4C].unk74 = D_80127498 + sp50 + sp48;
    D_8015EDB4[sp4C].unk76 = D_8012749C + sp52 + sp44;
    D_8015EDB4[sp4C].unk78 = D_801274A0 + sp54;
    temp_v0_2 = &D_8015EDB4[sp4C];
    var_a0 = temp_v0_2->unk84[1];
    if ((var_a0 < 0x15U) && (temp_v0_2->unk82 < temp_v0_2->unk7A)) {
        temp_v0_2->unk84[1] = *(sp3C + arg4);
        temp_v0_3 = &D_8015EDB4[sp4C];
        temp_v0_3->unk82 += 1;
        var_a0 = D_8015EDB4[sp4C].unk84[1];
    }
    if (var_a0 == 0) {
        if (arg0 != 0x19) {
            func_8015163C(arg0, arg5);
        }
        if (sp38 == &D_80127610) {
            D_80127090.unk0 = 0x40000000;
            D_801270A0.unk0 = 0x40000000;
            D_801270B0 = 0x40000000;
            D_801270C0 = 0x40000000;
        } else if (sp38 == &D_801277F0) {
            D_801270F0 = 0x40000000;
            D_80127100 = 0x40000000;
            D_80127110 = 0x40000000;
            D_80127120 = 0x40000000;
        }
    }
}
