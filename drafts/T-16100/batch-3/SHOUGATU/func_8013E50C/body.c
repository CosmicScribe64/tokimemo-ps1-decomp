void func_8013E50C(void) {
    s16 sp40[7];
    s32 sp30[4];
    s16 temp_t8;
    s16 temp_v1;
    s16 var_v1;
    s32 var_v0;

    *(s32 *)sp40 = (s32) D_801460CC;
    *(s32 *)((unsigned char *)(&sp40[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460CC) + (4));
    *(s32 *)((unsigned char *)(&sp40[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460CC) + (8));
    *(u16 *)((unsigned char *)(&sp40[0]) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460CC) + (12));
    (*(s32 *)&sp30) = (s32) D_801460DC;
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_801460DC) + (8));
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_801460DC) + (4));
    *(u16 *)((unsigned char *)(&sp30) + (12)) = (u16) *(u16 *)((unsigned char *)(&D_801460DC) + (12));
    D_80146050 = 0;
    if (!(D_800E6640 & 2) && (D_800E6636 >= 0x14) && (D_800E7378 & 1)) {
        D_80146050 = 1;
    }
    D_800E71DF = 0;
    D_80145F2C = ((get_g_zyotai_s(0, (s16 (*)[]) &sp40[0], 0) & 0x7F) == 4) * 4;
    D_80145F20 = D_80145AD4;
    D_80145F24 = D_80145B08;
    D_80145F28 = D_80145B3C;
    func_800847B8(D_800E71DF);
    var_v1 = 0;
    D_800E6280.unk_1BC[D_800E71DF].unk_06 += 4;
loop_5:
    var_v0 = var_v1 * 2;
    temp_v1 = var_v1 + 1;
    if (D_800E6396 >= sp40[var_v1]) {
        temp_t8 = temp_v1;
        var_v1 = temp_t8;
        if (temp_t8 >= 7) {
            var_v0 = temp_t8 * 2;
        } else {
            goto loop_5;
        }
    }
    D_800E6280.unk_1BC[D_800E71DF].unk_02 += ((s16 *)sp30)[var_v0 / 2];
    check_para_limit((u8 *)(u8 *)&D_800E6280 + (D_800E71DF * 0x38), (s16 (*)[]) &sp40[0], &D_800E6280, 0x38);
    func_80085B3C(4, 0);
    func_8004284C();
}
