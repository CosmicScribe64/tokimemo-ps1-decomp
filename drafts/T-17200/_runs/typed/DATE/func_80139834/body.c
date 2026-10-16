s32 func_80139834(void) {
    s16 sp2A;

    if (D_80122CDC == 1) {
        if (D_800E6280.unk_F5F == 7) {
            sp2A = (s16) D_800E6280.unk_110A;
            normal_date_speak();
            if (D_800E6280.unk_110A != sp2A) {
                D_800CA148 = 0xA;
                D_800E6280.unk_110A += 0xC;
            }
            return 0;
        }
        D_800CA148 = 9;
        if (D_800E6280.unk_F5F == 9) {
            D_800CA148 = 8;
        } else if ((D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5)) {
            D_800CA148 = 7;
        }
        D_800E6280.unk_110A += 0xC;
        goto block_15;
    }
    if (D_800E6280.unk_F5F == 7) {
        D_800CA148 += 1;
    }
    D_8015B67C = 1;
block_15:
    return func_8004284C();
}
