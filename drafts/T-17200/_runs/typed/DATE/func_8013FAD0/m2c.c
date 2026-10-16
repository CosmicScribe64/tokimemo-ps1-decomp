extern s32 D_801217F4;

void func_8013FAD0(void) {
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s16 var_v0_4;
    s16 var_v0_5;
    s16 var_v0_6;
    s32 *temp_v1_3;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_t2;
    s32 temp_t8;
    u8 temp_a0;
    void *temp_v1;
    void *temp_v1_2;
    void *temp_v1_4;
    void *temp_v1_5;

    temp_a0 = D_8015BD3C - 4;
    D_801217E5 = temp_a0;
    D_801217E4 = temp_a0;
    D_80121809 = temp_a0;
    D_80121808 = temp_a0;
    D_8015BD3C = temp_a0;
    D_801217E6 = temp_a0;
    D_8012180A = temp_a0;
    *(u8 *)((unsigned char *)D_8011ECD0 + (2115)) = temp_a0;
    var_v0 = 0;
    do {
        temp_lo = var_v0 * 0x44;
        var_v0 += 1;
        *(u8 *)((unsigned char *)(D_8011ECD0 + temp_lo) + (6535)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115));
    } while (var_v0 < 4);
    var_v0_2 = 0x3A;
    do {
        temp_lo_2 = var_v0_2 * 0x24;
        var_v0_2 += 1;
        temp_v1 = D_801217D0 + temp_lo_2;
        *(u8 *)((unsigned char *)temp_v1 + (22)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115));
        *(u8 *)((unsigned char *)temp_v1 + (21)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115));
        *(u8 *)((unsigned char *)temp_v1 + (20)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115));
    } while (var_v0_2 < 0x40);
    if ((u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115)) < 0x11U) {
        D_80122D14 = 0;
    }
    *(u8 *)((unsigned char *)D_8011ECD0 + (2115)) = (u8) *(u8 *)((unsigned char *)D_8011ECD0 + (2115));
    if (*(u8 *)((unsigned char *)D_8011ECD0 + (2115)) == 0) {
        D_80122D14 = 0;
        D_80122CF4 = 0;
        var_v0_3 = 0;
        do {
            temp_lo_3 = var_v0_3 * 0x44;
            var_v0_3 += 1;
            temp_v1_2 = D_8011ECD0 + temp_lo_3;
            *(u8 *)((unsigned char *)temp_v1_2 + (6531)) = (u8) (*(u8 *)((unsigned char *)temp_v1_2 + (6531)) & 0x7F);
        } while (var_v0_3 < 0xA);
        *(u8 *)((unsigned char *)D_8011ECD0 + (2111)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (2111)) & 0xFF7F);
        D_801217E5 = 0x80;
        D_801217E4 = 0x80;
        D_80121809 = 0x80;
        D_80121808 = 0x80;
        D_801217D0->unk_00 |= 0x80000000;
        D_801217F4 |= 0x80000000;
        D_801217E6 = 0x80;
        D_8012180A = 0x80;
        var_v0_4 = 0x3A;
        do {
            temp_lo_4 = var_v0_4 * 0x24;
            var_v0_4 += 1;
            temp_v1_3 = D_801217D0 + temp_lo_4;
            *temp_v1_3 |= 0x80000000;
        } while (var_v0_4 < 0x40);
        func_80043914(D_800CA130, 0x10, 1, 2, 0);
        var_v0_5 = 0xB;
        do {
            temp_t8 = var_v0_5 * 4;
            var_v0_5 += 1;
            temp_v1_4 = &D_800E6280 + temp_t8;
            *(s32 *)((unsigned char *)temp_v1_4 + (4648)) = 0x77;
            *(s32 *)((unsigned char *)temp_v1_4 + (4776)) = 0x77;
        } while (var_v0_5 < 0x10);
        var_v0_6 = 0x15;
        do {
            temp_t2 = var_v0_6 * 4;
            var_v0_6 += 1;
            temp_v1_5 = &D_800E6280 + temp_t2;
            *(s32 *)((unsigned char *)temp_v1_5 + (4648)) = 0x77;
            *(s32 *)((unsigned char *)temp_v1_5 + (4776)) = 0x77;
        } while (var_v0_6 < 0x1A);
        func_80044750(0x205);
        func_80048E78();
        func_80084E4C();
        D_800B5BD4 = 0xF;
        func_8004284C();
    }
}
