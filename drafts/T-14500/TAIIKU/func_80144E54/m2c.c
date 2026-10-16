extern s16 D_801207BC;
extern s16 D_80120800;
extern u8 D_8014A121;
extern u8 D_8014A122;
extern u8 D_8014A123;
extern ? D_8014A124;
extern ? D_8014A228;

void func_80144E54(void) {
    s16 var_a0;
    u8 *var_v0;
    u8 *var_v0_2;
    u8 *var_v1;

    func_80048E78();
    var_v0 = D_8011ECD0;
    do {
        var_v0 += 0x44;
        *(s8 *)((unsigned char *)var_v0 + (6463)) = 0xA4;
        *(s8 *)((unsigned char *)var_v0 + (6462)) = 0x40;
        *(s8 *)((unsigned char *)var_v0 + (6465)) = 0x80;
        *(s8 *)((unsigned char *)var_v0 + (6461)) = 3;
        *(s32 *)((unsigned char *)var_v0 + (6516)) = 0x01000000;
        *(s8 *)((unsigned char *)var_v0 + (6467)) = 0x80;
        *(s32 *)((unsigned char *)var_v0 + (6472)) = (s32) D_80149FC8;
        *(s32 *)((unsigned char *)var_v0 + (6476)) = (s32) D_8014A000;
        *(? **)((unsigned char *)var_v0 + (6512)) = &D_8014A228;
        *(s16 *)((unsigned char *)var_v0 + (6480)) = (s16) D_8014A038;
        *(s16 *)((unsigned char *)var_v0 + (6486)) = 0x1000;
        *(s16 *)((unsigned char *)var_v0 + (6488)) = 0x1000;
        *(s8 *)((unsigned char *)var_v0 + (6466)) = 2;
    } while ((u32) var_v0 < (u32) D_8011EEF0);
    var_v1 = D_8014A120;
    var_v0_2 = D_8011ECD0;
    var_a0 = -0x78;
    do {
        *(s16 *)((unsigned char *)var_v0_2 + (6566)) = var_a0;
        *(s16 *)((unsigned char *)var_v0_2 + (6552)) = (s16) ((*var_v1 * 8) + 0x1A);
        var_a0 += 0x40;
        *(s16 *)((unsigned char *)var_v0_2 + (6570)) = -8;
        if ((*(s16 *)((unsigned char *)var_v0_2 + (6552)) == 0x22) && ((u8) D_8014A110 == 1)) {
            *(s16 *)((unsigned char *)var_v0_2 + (6552)) = 0x42;
        }
        var_v1 += 1;
        var_v0_2 += 0x44;
    } while ((u32) var_v1 < (u32) &D_8014A124);
    D_80120786 = -0x78;
    D_8012078A = -8;
    D_80120778 = (*D_8014A120 * 5) + 0xD;
    D_801207CA = -0x38;
    D_801207CE = -8;
    D_801207BC = (D_8014A121 * 5) + 0xD;
    D_8012080E = 8;
    D_80120812 = -8;
    D_80120800 = (D_8014A122 * 5) + 0xD;
    D_80120852 = 0x48;
    D_80120856 = -8;
    D_80120844 = (D_8014A123 * 5) + 0xD;
}
