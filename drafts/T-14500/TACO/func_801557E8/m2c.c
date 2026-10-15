void func_801557E8(s32 arg0) {
    s32 sp2C;
    s32 sp28;
    TcActor *temp_v1;
    s32 temp_t0;
    u8 *temp_v1_2;
    u8 *temp_v1_3;
    u8 *temp_v1_4;

    temp_v1 = &D_8015EDB4[arg0];
    sp28 = arg0 * 0x88;
    temp_t0 = (temp_v1->unk7A - temp_v1->unk6A) * 7;
    sp2C = temp_t0;
    *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[sp28]) + (120)) = (s16) ((s32) (func_800A0070(temp_t0) * -0x3A98) >> 0xC);
    temp_v1_2 = &D_8015EDB4->pad0[sp28];
    *(s16 *)((unsigned char *)temp_v1_2 + (116)) = (s16) ((((s32) (func_800A0140(sp2C) << 0xB) >> 0xC) * *(s16 *)((unsigned char *)temp_v1_2 + (126))) + *(s16 *)((unsigned char *)temp_v1_2 + (124)));
    temp_v1_3 = &D_8015EDB4->pad0[sp28];
    *(s16 *)((unsigned char *)temp_v1_3 + (118)) = (s16) *(s16 *)((unsigned char *)temp_v1_3 + (128));
    *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[sp28]) + (100)) = 0;
    temp_v1_4 = &D_8015EDB4->pad0[sp28];
    if (*(s16 *)((unsigned char *)temp_v1_4 + (126)) == 1) {
        *(s16 *)((unsigned char *)temp_v1_4 + (102)) = (s16) sp2C;
        *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[sp28]) + (104)) = 0xC00;
    } else {
        *(s16 *)((unsigned char *)temp_v1_4 + (102)) = (s16) (0x1000 - sp2C);
        *(s16 *)((unsigned char *)(&D_8015EDB4->pad0[sp28]) + (104)) = 0x400;
    }
    if (sp2C >= 0xA01) {
        func_8015C208(arg0);
    }
}
