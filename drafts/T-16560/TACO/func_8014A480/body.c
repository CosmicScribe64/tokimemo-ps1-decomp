void func_8014A480(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4, s32 arg5) {
    s32 sp52;
    s32 sp54;
    MATRIX sp60;
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
    u8 *temp_v0;

    temp_v1 = arg1 * 2;
    sp4C = arg1 + 0x10;
    temp_v0 = ((u8 *)arg2) + (temp_v1 * 2);
    sp48 = (s32) (*(s32 *)temp_v0);
    sp44 = (s32) *(s16 *)((unsigned char *)temp_v0 + (2));
    D_8015EDB4[arg1 + 16].unk84[0] = (u8) *(((u8 *)arg3) + arg1);
    sp60 = *(MATRIX *)D_80125C60;
    sp3C = temp_v1;
    func_8015D0D0((s32) (D_80128880->x << 0xC) / 360, (s32) &sp60);
    func_8015CF30((s32) (D_80128880[0].z << 0xC) / 360, (s32) &sp60);
    func_8015D270((s32) (D_80128880[0].y << 0xC) / 360, (s32) &sp60);
    temp_a3 = &D_80127480[arg0];
    sp38 = temp_a3;
    sp58 = (s16) temp_a3->unk18;
    sp5A = (s16) temp_a3->unk1C;
    sp5C = (s16) temp_a3->unk20;
    func_800A0ECC(&sp60, &sp58, &sp50, temp_a3);
    D_8015EDB4[sp4C].unk74 = D_80127480[0].unk18 + sp50 + sp48;
    D_8015EDB4[sp4C].unk76 = D_80127480[0].unk1C + sp52 + sp44;
    D_8015EDB4[sp4C].unk78 = D_80127480[0].unk20 + sp54;
    temp_v0_2 = &D_8015EDB4[sp4C];
    var_a0 = temp_v0_2->unk84[1];
    if ((var_a0 < 0x15U) && (temp_v0_2->unk82 < temp_v0_2->unk7A)) {
        temp_v0_2->unk84[1] = *(sp3C + ((u8 *)arg4));
        temp_v0_3 = &D_8015EDB4[sp4C];
        temp_v0_3->unk82 += 1;
        var_a0 = D_8015EDB4[sp4C].unk84[1];
    }
    if (var_a0 == 0) {
        if (arg0 != 0x19) {
            func_8015163C(arg0, arg5);
        }
        if (sp38 == &D_80127480[5].unk0) {
            D_80127080[1].unk0 = 0x40000000;
            D_80127080[2].unk0 = 0x40000000;
            D_80127080[3].unk0 = 0x40000000;
            D_80127080[4].unk0 = 0x40000000;
        } else if (sp38 == &D_80127480[11].unk0) {
            D_80127080[7].unk0 = 0x40000000;
            D_80127080[8].unk0 = 0x40000000;
            D_80127080[9].unk0 = 0x40000000;
            D_80127080[10].unk0 = 0x40000000;
        }
    }
}
