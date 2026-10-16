s32 func_800754C0(void) {
    s32 sp4C[1];
    s32 sp2C[1];
    s32 var_v0;

    func_80053418((s32) &sp4C, (s32) &sp2C);
    if (((D_800E6378 & 0xF) != (D_800E644C & 0xF)) || (((u32) (D_800E6378 << 0x17) >> 0x1B) != ((u32) (D_800E644C << 0x17) >> 0x1B))) {
        return 0;
    }
    if ((D_800E62BF != 3) && ((D_800E62BF != 4) || ((u8) D_800E62C0 >= 4U))) {
        return 0;
    }
    if (appointment_find_today((s32) D_800E62BE, (s32) D_800E62BF, (s32) D_800E62C0) > 0) {
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
