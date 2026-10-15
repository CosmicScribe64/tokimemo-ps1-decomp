s32 func_80139834(void) {
    /* FAKE: 4 spare bytes reproduce prev's stack home at 0x2A(sp). T-11140 */
    s32 pad;
    s16 prev;
    u8 g;

    if (D_80122CDC == 1) {
        g = D_800E6280.unk_F5F;
        if (g == 7) {
            prev = D_800E6280.unk_110A;
            func_80085A60();
            g = D_800E6280.unk_110A;
            if (prev != g) {
                D_800CA148 = 0xA;
                D_800E6280.unk_110A = g + 0xC;
            }
            return 0;
        }
        D_800CA148 = 9;
        if (g == 9) {
            D_800CA148 = 8;
        } else if ((g == 4) || (g == 8) || (g == 5)) {
            D_800CA148 = 7;
        }
        D_800E6280.unk_110A += 0xC;
    } else {
        g = D_800E6280.unk_F5F;
        if (g == 7) {
            D_800CA148 += 1;
        }
        D_8015B67C = 1;
    }
    func_8004284C();
}
