extern u8 D_800E62BA;

void func_80140B20(void) {
    u8 temp_t6;
    u8 temp_v0;

    if ((u8) D_800E62BA >= 3U) {
        temp_t6 = (D_800E62BA - 2) & 0xFF;
        temp_v0 = temp_t6 & 0xFF;
        D_80122106 = temp_v0;
        D_80122105 = temp_v0;
        D_80122104 = temp_v0;
        D_801220E2 = temp_v0;
        D_801220E1 = temp_v0;
        D_801220E0 = temp_v0;
        D_800E62BA = temp_t6;
        if (temp_t6 < 0x80U) {
            D_800E62BA = temp_t6;
            func_80057390(temp_t6);
        }
    }
}
