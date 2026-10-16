s32 func_800754C0(void) {
    ? sp4C;
    ? sp2C;
    s32 var_v0;

    func_80053418((s32) &sp4C, (s32) &sp2C);
    if (((D_800E6280.unk_0F8 & 0xF) != (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) || (((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B) != ((u32) (D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B))) {
        return 0;
    }
    if ((D_800E6280.unk0 != 3) && ((D_800E6280.unk0 != 4) || ((u8) D_800E6280.unk_040 >= 4U))) {
        return 0;
    }
    if (appointment_find_today((s32) D_800E6280.unk0, (s32) D_800E6280.unk0, (s32) D_800E6280.unk_040) > 0) {
        return 0;
    }
    if (birth_day_check(0xF) != 1) {
        return 0;
    }
    var_v0 = 0;
    if (get_g_zyotai_s(0) & 0x80) {
        var_v0 = 1;
    }
    return var_v0;
}
