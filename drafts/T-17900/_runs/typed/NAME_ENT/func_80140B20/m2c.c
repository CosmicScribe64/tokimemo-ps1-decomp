void func_80140B20(void) {
    s8 temp_v0;
    u8 temp_a0;
    u8 temp_t6;

    temp_a0 = D_800E6280.unk_03A;
    if (temp_a0 >= 3U) {
        temp_t6 = (temp_a0 - 2) & 0xFF;
        temp_v0 = temp_t6 & 0xFF;
        *(s8 *)((unsigned char *)D_801217D0 + (2358)) = temp_v0;
        *(s8 *)((unsigned char *)D_801217D0 + (2357)) = temp_v0;
        *(s8 *)((unsigned char *)D_801217D0 + (2356)) = temp_v0;
        *(s8 *)((unsigned char *)D_801217D0 + (2322)) = temp_v0;
        *(s8 *)((unsigned char *)D_801217D0 + (2321)) = temp_v0;
        *(s8 *)((unsigned char *)D_801217D0 + (2320)) = temp_v0;
        D_800E6280.unk_03A = temp_t6;
        if (temp_t6 < 0x80U) {
            D_800E6280.unk_03A = temp_t6;
            func_80057390(temp_t6);
        }
    }
}
