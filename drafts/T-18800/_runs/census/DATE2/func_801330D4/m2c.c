extern u8 D_800E69DD;
extern u8 D_800E71DF;
s32 func_801338F8(void);
void func_801330D4(void) {
    s16 sp38[10];
    s16 *temp_v1;
    s16 *temp_v1_2;
    s16 var_a1;
    s16 var_a2;
    s16 var_v0;
    s32 temp_lo;
    s32 var_t6;
    u8 temp_t3;
    u8 var_a1_2;

    D_8013A4C0 = 0;
    var_a2 = 0x64;
    D_800E71DF = 1;
    var_a1 = 1;
    do {
        temp_v1 = &(&sp38[0])[var_a1];
        *temp_v1 = 0;
        var_v0 = 1;
loop_2:
        temp_t3 = *(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + (var_a1 * 0x38) + var_v0) + (464));
        var_v0 += 1;
        *temp_v1 += temp_t3;
        if (var_v0 < 0x25) {
            goto loop_2;
        }
        temp_lo = var_a1 * 0x38;
        var_a1 += 1;
        *temp_v1 -= *(u8 *)((unsigned char *)(((u8 *)&D_800E6280) + temp_lo) + (490));
    } while (var_a1 < 0xA);
    var_a1_2 = 1;
    var_t6 = 1 * 2;
    do {
        temp_v1_2 = &sp38[var_t6 / 2];
        if ((*temp_v1_2 < var_a2) && (D_800E6280.unk_1BC[var_a1_2].unk_0C.w & 2)) {
            D_800E71DF = var_a1_2;
            var_a2 = *temp_v1_2;
        }
        var_a1_2 = (u8) (s16) (var_a1_2 + 1);
        var_t6 = var_a1_2 * 2;
    } while ((s32) var_a1_2 < 0xA);
    D_800E69DD = D_800E71DF;
    func_800847B8(D_800E71DF);
    D_800E71DF = 0;
    func_80085B3C(0xC, 0);
    D_800E71DF = D_800E69DD;
    func_80085B3C(0xC, 0);
    func_80133258();
    func_801338F8();
    func_8004284C();
}
