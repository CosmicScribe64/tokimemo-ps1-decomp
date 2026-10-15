extern s32 D_800E7384;

void func_8014265C(void) {
    s32 temp_a0;

    func_8013D5D8();
    temp_a0 = D_800E7384 + 1;
    D_800E7384 = temp_a0;
    if (D_800E7384 == 0) {
        M2C_MEMCPY_ALIGNED(D_801206D8, D_80120650, 0x40);
        *(s32 *)((unsigned char *)(D_801206D8 + 0x3C) + (4)) = (s32) *(s32 *)((unsigned char *)(D_80120650 + 0x3C) + (4));
        M2C_MEMCPY_ALIGNED(D_801206D8, D_80120650, 0x44);
        D_801206EE = 4;
    }
    if (((u16) D_800CA150 == 7) && ((u16) D_800CA154 == 1) && (func_8004ECB4(temp_a0) == 0)) {
        D_801206DA = 1;
        return;
    }
    D_801206DA = 0;
    D_801206F0 = 0;
}
