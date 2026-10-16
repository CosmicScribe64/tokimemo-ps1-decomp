void func_8014265C(void) {
    typedef struct { s32 w[16]; } M2cBlock40; /* m2c memcpy of 64 bytes */
    typedef struct { s32 w[17]; } M2cBlock44; /* m2c memcpy of 68 bytes */
    DisplayActor44 *temp_v0;
    DisplayActor44 *temp_v1;
    s32 temp_a0;

    func_8013D5D8();
    temp_a0 = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = temp_a0;
    if (D_800E6280.unk_1104.w == 0) {
        temp_v1 = &((DisplayActor44 *)D_8011ECD0)[96];
        temp_v0 = &((DisplayActor44 *)D_8011ECD0)[98];
        *(M2cBlock40 *)(temp_v0) = *(M2cBlock40 *)(temp_v1);
        *(s32 *)((unsigned char *)temp_v0->pad_3C + (4)) = (s32) *(s32 *)((unsigned char *)temp_v1->pad_3C + (4));
        *(M2cBlock44 *)(temp_v0) = *(M2cBlock44 *)(temp_v1);
        ((DisplayActor44 *)D_8011ECD0)[98].unk_16 = 4;
    }
    if (((u16) D_800CA150 == 7) && ((u16) D_800CA154 == 1) && (check_end_k(temp_a0) == 0)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 1;
        return;
    }
    ((DisplayActor44 *)D_8011ECD0)[98].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0;
}
