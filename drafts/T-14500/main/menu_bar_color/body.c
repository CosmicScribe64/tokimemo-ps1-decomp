void menu_bar_color(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg0 >= 0x10) {
        D_800E3680[0] = 0x832F7FAF; D_800E3680[1] = 0x822F7F2F;
        return;
    }
    D_800E3680[arg0] = ((arg2 & 0xF) << 0x18) + arg1 + (arg3 << 0x1C);
}
