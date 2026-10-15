/* NON_MATCHING: T-10520, only the base register of the argument load differs ($a0 here, $v0 in the original; ugen temporary), 2 lines */
s32 func_80133670(void) {
    if ((D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x46U) && ((D_80134540 == D_800E6280.unk_F5F) || (D_80134541 == D_800E6280.unk_F5F) || (D_80134542 == D_800E6280.unk_F5F)) && (D_800E6280.unk_F5F == 9)) {
        func_8004284C();
        return 0;
    }
    return normal_date_girl_out();
}
