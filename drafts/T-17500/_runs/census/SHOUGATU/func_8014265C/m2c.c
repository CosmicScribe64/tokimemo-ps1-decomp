extern s32 D_800E7384;
void func_8014265C(void) {
    typedef struct { s32 w[16]; } M2cBlock40; /* m2c memcpy of 64 bytes */
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    s32 temp_a0;

    func_8013D5D8();
    temp_a0 = D_800E7384 + 1;
    D_800E7384 = temp_a0;
    if (D_800E7384 == 0) {
        *(M2cBlock40 *)(D_801206D8) = *(M2cBlock40 *)(D_80120650);
        *(s32 *)((unsigned char *)(D_801206D8 + 0x3C) + (4)) = (s32) *(s32 *)((unsigned char *)(D_80120650 + 0x3C) + (4));
        *(M2cBlock44 *)(D_801206D8) = *(M2cBlock44 *)(D_80120650);
        D_801206EE = 4;
    }
    if (((u16) D_800CA150 == 7) && ((u16) D_800CA154 == 1) && (check_end_k(temp_a0) == 0)) {
        D_801206DA = 1;
        return;
    }
    D_801206DA = 0;
    D_801206F0 = 0;
}
