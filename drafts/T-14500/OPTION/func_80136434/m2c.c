extern u8 D_800E738D;
extern s16 D_80120914;

s32 func_80136434(void) {
    s16 temp_t8;
    s16 var_v0;
    void *temp_v0;
    void *temp_v0_2;

    if (D_800E738D == 0) {
        D_8012093F &= 0xFF7F;
        temp_v0 = (1 * 0x44) + D_8011ECD0;
        *(u8 *)((unsigned char *)temp_v0 + (7483)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7483)) & ~0x80);
        *(u8 *)((unsigned char *)temp_v0 + (7415)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7415)) & ~0x80);
        *(u8 *)((unsigned char *)temp_v0 + (7347)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7347)) & ~0x80);
        *(u8 *)((unsigned char *)temp_v0 + (7279)) = (u8) (*(u8 *)((unsigned char *)temp_v0 + (7279)) & ~0x80);
        temp_t8 = ((s16) *D_80120912 / 9) * 8;
        *D_80120912 = temp_t8;
        var_v0 = temp_t8;
        if (temp_t8 < 9) {
            var_v0 = 1;
            D_800E738D = 1;
        }
    } else if (D_800E738D == 2) {
        D_8012093F &= 0xFF7F;
        temp_v0_2 = (1 * 0x44) + D_8011ECD0;
        *(u8 *)((unsigned char *)temp_v0_2 + (7483)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7483)) & 0xFF7F);
        *(u8 *)((unsigned char *)temp_v0_2 + (7415)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7415)) & 0xFF7F);
        *(u8 *)((unsigned char *)temp_v0_2 + (7347)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7347)) & 0xFF7F);
        *(u8 *)((unsigned char *)temp_v0_2 + (7279)) = (u8) (*(u8 *)((unsigned char *)temp_v0_2 + (7279)) & 0xFF7F);
        var_v0 = (s16) ((s32) (*D_80120912 * 9) / 8);
        D_80120914 = (s16) ((s32) (D_80120914 * 9) / 8);
        *D_80120912 = var_v0;
        if (var_v0 >= 0x4000) {
            var_v0 = 1;
            D_800E738D = 1;
        }
    } else {
        func_80042908(0);
        var_v0 = func_80042940(2);
    }
    D_801208FB = 0;
    return (s32) var_v0;
}
