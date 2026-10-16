void func_80134418(void) {
    s32 i;
    s32 sel;

    sel = *(u8 *)&D_800E6280.unk_110B;
    for (i = 0; i < 64; i++) {
        D_80120650[i * 0x44 + 7] = sel;
    }
    sel = (u8)(sel + 2);
    D_800E6280.unk_110B = sel;
    if (sel > 0x80) {
        for (i = 0; i < 64; i++) {
            D_80120650[i * 0x44 + 7] = 0x80;
        }
        *(u8 *)&D_800E6280.unk_110B = 0x80;
        func_8004284C();
    }
}
