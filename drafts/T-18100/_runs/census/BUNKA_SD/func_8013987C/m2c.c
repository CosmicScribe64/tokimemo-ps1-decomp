
void func_8013987C(void) {
    u8 v;
    u8 *p;
    s32 i;

    v = D_800E6280.unk_110B;
    p = D_80120650;
    do {
        p[7] = v;
        p += 0x44;
    } while (p < D_80121750);
    if (v == 0) {
        v = 2;
    }
    v -= 1;
    D_800E6280.unk_110B = v;
    if (v == 0) {
        for (i = 0; i < 0x40; i++) {
            D_80120650[i * 0x44 + 7] = v;
        }
        *(u8 *)&D_800E6280.unk_110B = v;
        func_8004284C();
    }
}
