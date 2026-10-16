void func_8013E50C(void) {
    ShoShorts7 sp40;
    ShoShorts7 sp30;
    s16 temp_t8;
    s16 temp_v1;
    s16 var_v1;
    s32 var_v0;

    sp40 = D_801460CC;
    sp30 = D_801460DC;
    D_80146050 = 0;
    if (!(D_800E6280.unk_1BC[9].unk_0C.w & 2) && (D_800E6280.unk_1BC[9].unk_02 >= 0x14) && (D_800E6280.unk_10F8 & 1)) {
        D_80146050 = 1;
    }
    D_800E6280.unk_F5F = 0;
    D_80145F2C = ((get_g_zyotai_s(0, (s16 (*)[]) sp40.v, 0) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    func_800847B8(D_800E6280.unk_F5F);
    var_v1 = 0;
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 4;
loop_5:
    var_v0 = var_v1 * 2;
    temp_v1 = var_v1 + 1;
    if (D_800E6280.unk_114.unk_02 >= sp40.v[var_v1]) {
        temp_t8 = temp_v1;
        var_v1 = temp_t8;
        if (temp_t8 >= 7) {
            var_v0 = temp_t8 * 2;
        } else {
            goto loop_5;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk0].unk_02 += *(s16 *)((unsigned char *)(sp + var_v0) + (48));
    check_para_limit(&D_800E6280 + (D_800E6280.unk0 * 0x38), (s16 (*)[]) sp40.v, &D_800E6280, 0x38);
    func_80085B3C(4, 0);
    func_8004284C();
}
