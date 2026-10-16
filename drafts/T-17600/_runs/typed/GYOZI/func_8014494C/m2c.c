void func_8014494C(void) {
    DisplayActor44 *temp_v0;
    DisplayActor44 *temp_v1;
    s32 temp_a0;

    func_80140050();
    temp_a0 = D_800F6474 + 1;
    D_800F6474 = temp_a0;
    if (D_800F6474 == 0) {
        temp_v1 = D_80129F40 + 0x1980;
        temp_v0 = D_80129F40 + 0x1A08;
        M2C_MEMCPY_ALIGNED(temp_v0, temp_v1, 0x40);
        *(s32 *)((unsigned char *)temp_v0->pad_3C + (4)) = (s32) *(s32 *)((unsigned char *)temp_v1->pad_3C + (4));
        M2C_MEMCPY_ALIGNED(temp_v0, temp_v1, 0x44);
        *(s16 *)((unsigned char *)D_80129F40 + (6686)) = 4;
    }
    if (((u16) D_800D9250 == 7) && ((u16) D_800D9254 == 1) && (func_8005B32C(temp_a0) == 0)) {
        *(s8 *)((unsigned char *)D_80129F40 + (6666)) = 1;
        return;
    }
    *(s8 *)((unsigned char *)D_80129F40 + (6666)) = 0;
    *(s16 *)((unsigned char *)D_80129F40 + (6688)) = 0;
}
