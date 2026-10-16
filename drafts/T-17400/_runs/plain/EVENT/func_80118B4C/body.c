void func_80118B4C(void) {
    s16 sp4A;
    s16 sp48;
    s32 sp38[3];
    u8 *temp_v0;
    u8 *temp_v0_2;
    u8 *temp_v0_3;

    (*(s32 *)&sp38) = (s32) D_80125360.unk0;
    *(u16 *)((unsigned char *)(&sp38) + (8)) = (u16) D_80125360.v[4];
    *(s32 *)((unsigned char *)(&sp38) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125360) + (4));
    func_8004A8EC((&D_8012438C)[D_800EECBC]);
    func_80033E88();
    func_80118DAC(3 - (&D_8012439C)[D_800EECBC], (s32) &sp4A, (s32) &sp48);
    temp_v0 = (D_800B1746 * 0x34) + D_800B0860;
    *(s32 *)((unsigned char *)temp_v0 + (420)) = (s32) (*(s32 *)((unsigned char *)temp_v0 + (420)) + ((s32) (((3 - (&D_8012439C)[D_800EECBC]) * sp4A * sp48) << 0x10) / 16));
    func_80118DAC(2 - (&D_8012439C)[D_800EECBC], (s32) &sp4A, (s32) &sp48);
    temp_v0_2 = (D_800B1746 * 0x34) + D_800B0860;
    *(s32 *)((unsigned char *)temp_v0_2 + (424)) = (s32) (*(s32 *)((unsigned char *)temp_v0_2 + (424)) + ((s32) (((2 - (&D_8012439C)[D_800EECBC]) * sp4A * sp48) << 0x10) / 16));
    func_80118DAC(2 - (&D_8012439C)[D_800EECBC], (s32) &sp4A, (s32) &sp48);
    temp_v0_3 = (D_800B1746 * 0x34) + D_800B0860;
    *(s32 *)((unsigned char *)temp_v0_3 + (424)) = (s32) (*(s32 *)((unsigned char *)temp_v0_3 + (424)) + ((s32) ((*(s16 *)((unsigned char *)(sp + ((&D_8012439C)[D_800EECBC] * 2)) + (56)) * sp4A * sp48) << 0x10) / 16));
    func_8004C250();
    func_80011DFC();
}
