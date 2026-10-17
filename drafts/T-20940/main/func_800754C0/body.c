s32 func_800754C0(void) {
    s32 m;
    u8 rank[16];
    s16 points[16];

    func_80053418(rank, points);
    if (((D_800E6280.unk_0F8 & 0xF) != (D_800E6280.unk_1BC[0].unk_10.w & 0xF)) ||
        (((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) != ((u32)(D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B))) {
        return 0;
    }
    m = D_800E6280.unk_03F;
    if ((m != 3) && ((m != 4) || ((u8)D_800E6280.unk_040 >= 4U))) {
        return 0;
    }
    if (appointment_find_today(D_800E6280.unk_03E, m, D_800E6280.unk_040) > 0) {
        return 0;
    }
    if (birth_day_check(0xF) != 1) {
        return 0;
    }
    if (get_g_zyotai_s(0) & 0x80) {
        return 1;
    }
    return 0;
}
