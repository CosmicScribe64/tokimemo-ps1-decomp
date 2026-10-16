void func_80140B20(void) {
    /* FAKE: unk_03A is already u8; the (u8 *) view only moves the global to $a0 like the original (T-11570) */
    if (*(u8 *)&D_800E6280.unk_03A >= 3) {
        *(u8 *)&D_800E6280.unk_03A -= 2;
        D_801220E0 = D_801220E1 = D_801220E2 = D_80122104 = D_80122105 = D_80122106 = *(u8 *)&D_800E6280.unk_03A;
        if (*(u8 *)&D_800E6280.unk_03A < 0x80) {
            func_80057390(*(u8 *)&D_800E6280.unk_03A);
        }
    }
}
