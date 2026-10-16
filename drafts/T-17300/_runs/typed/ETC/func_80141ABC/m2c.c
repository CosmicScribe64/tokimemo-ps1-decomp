? func_800AE080(? *, ?);                            /* extern */
? func_800AE0B0(? *, u8, u8, u8, s32, s32);         /* extern */
extern ? D_800E6280;
extern ? D_800E7D10;
extern s8 D_800E7D37;
extern s8 D_800E7D38;
extern ? D_800E7D68;

void func_80141ABC(void) {
    ? *var_s0;
    s8 var_s1;
    u8 temp_v0;
    void *temp_t3;

    func_800AE080(&D_800E7D68, 0x800);
    var_s0 = &D_800E6280;
    var_s1 = 0;
    do {
        temp_v0 = *(u8 *)((unsigned char *)var_s0 + (1888));
        func_800AE0B0("%d %d %d %d %d \n", *(u8 *)((unsigned char *)var_s0 + (1890)), *(u8 *)((unsigned char *)var_s0 + (1891)), *(u8 *)((unsigned char *)var_s0 + (1892)), (s32) temp_v0, (s32) temp_v0);
        if ((*(u8 *)((unsigned char *)(&D_800E6280) + (62)) != 0) && (*(u8 *)((unsigned char *)(&D_800E6280) + (63)) != 0) && (*(u8 *)((unsigned char *)(&D_800E6280) + (64)) != 0) && ((*(u8 *)((unsigned char *)(&D_800E6280) + (3935)) & 0xF) == *(u8 *)((unsigned char *)var_s0 + (1886)))) {
            temp_t3 = &D_800E7D10 + (var_s1 * 8);
            *(? *)((unsigned char *)temp_t3 + (88)) = (unaligned s32) *(u8 *)((unsigned char *)var_s0 + (1886));
            var_s1 += 1;
            *(? *)((unsigned char *)temp_t3 + (92)) = (unaligned s32) *(u8 *)((unsigned char *)var_s0 + (1890));
        }
        var_s0 += 8;
    } while (var_s0 != (&D_800E6280 + 0x800));
    D_800E7D38 = var_s1;
    D_800E7D37 = 1;
    func_800AE0B0("ALBUM_MODE IN SAVE %d \n", 1U);
}
