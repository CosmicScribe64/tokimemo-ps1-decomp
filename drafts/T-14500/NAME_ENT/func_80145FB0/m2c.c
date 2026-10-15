extern s8 D_800E736C;
extern ? D_800E78BC;

void func_80145FB0(s32 arg0, s32 arg1) {
    s32 sp58;
    s32 sp54;
    s32 sp4C;
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_v1;

    sp54 = arg0;
    temp_v1 = 2 << 0xE;
    temp_v0 = &(&sp54)[2];
    sp58 = arg0 + 0x4000;
    temp_v0->unk0 = temp_v1 + arg0;
    *(s32 *)((unsigned char *)temp_v0 + (4)) = (s32) (temp_v1 + arg0 + 0x4000);
    *(s32 *)((unsigned char *)temp_v0 + (12)) = (s32) (temp_v1 + arg0 + 0xC000);
    *(s32 *)((unsigned char *)temp_v0 + (8)) = (s32) (temp_v1 + arg0 + 0x8000);
    func_8004A1A0((s32) &sp54, 0, 6, 3, 2, arg1 + 0x1E0, -0xA0, -0x78, 8, 8);
    temp_a0 = arg0 + 0x18000;
    sp4C = temp_a0;
    func_80043914(temp_a0, 0xF, 1, 2, 0);
    func_8004B590(temp_a0, (s32) &D_800E78BC);
    func_80043914((s32) &D_800E78BC, 0xE, 1, 2, 0);
    D_800E736C = 0xD;
    D_801220D0 = 0x41000000;
}
