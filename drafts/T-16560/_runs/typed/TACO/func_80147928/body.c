void func_80147928(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5) {
    s32 sp34;
    s32 sp30;
    s32 sp2C;
    TcObj50 *temp_v1;
    TcPos *temp_a0_3;
    s32 temp_a0;
    s32 temp_a0_2;
    u8 *temp_v1_2;

    temp_a0 = arg1 + 0x10;
    sp34 = temp_a0;
    func_80143B34(temp_a0);
    temp_a0_2 = arg2 + 4;
    sp30 = temp_a0_2;
    func_8009B3C0(temp_a0_2);
    sp2C = sp34 * 0x88;
    func_8009B430(sp30 + 8, (u8 *) &D_8015EDB4[sp34].unk4, arg0 - 1, sp30);
    D_8015EDB4[sp34].unk4.unk0 = 0x80000000;
    D_8015EDB4[sp34].unk0[1] = 0x80;
    temp_v1 = &D_80127480[arg0];
    D_8015EDB4[sp34].unk74 = (s16) temp_v1->unk18;
    D_8015EDB4[sp34].unk76 = (s16) temp_v1->unk1C;
    D_8015EDB4[sp34].unk78 = (s16) temp_v1->unk20;
    temp_a0_3 = &D_80128880[arg0];
    D_8015EDB4[sp34].unk64 = temp_a0_3->x;
    D_8015EDB4[sp34].unk66 = temp_a0_3->y;
    D_8015EDB4[sp34].unk68 = temp_a0_3->z;
    temp_v1_2 = (arg1 * 2) + ((u8 *)arg4);
    D_8015EDB4[sp34].unk84[1] = (*(s32 *)temp_v1_2);
    D_8015EDB4[sp34].unk7A = (s16) *(u8 *)((unsigned char *)temp_v1_2 + (1));
    D_8015EDB4[sp34].unk82 = 1;
    D_8015EDB4[sp34].unk84[0] = *((arg1 * arg5) + ((u8 *)arg3));
    D_8015EDB4[sp34].unk84[3] = 1;
    D_8015EDB4[sp34].unk4.unk4[0] = (s32) &D_80127480[arg0];
}
