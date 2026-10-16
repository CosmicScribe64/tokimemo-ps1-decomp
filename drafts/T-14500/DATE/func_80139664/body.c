s32 func_80139664(void) {
    /* FAKE: unused byte local puts prev at 0x28(sp) as in the original. T-11580 */
    u8 pad;
    s32 prev;

    if (D_80122CDC == 1) {
        D_800CA148 = 9;
        if (D_800E6280.unk_F5F == 7) {
            D_800CA148 = 0xA;
        } else if (D_800E6280.unk_F5F == 9) {
            D_800CA148 = 8;
        } else if ((D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5) || (D_800E6280.unk_F5F == 0xA)) {
            D_800CA148 = 7;
        }
        D_800E6280.unk_110A += 0x12;
        return;
    }
    if ((D_800E6280.unk_F5F == 4) || (D_800E6280.unk_F5F == 8) || (D_800E6280.unk_F5F == 5) || (D_800E6280.unk_F5F == 0xA)) {
        D_8015B67C = 1;
        D_800E6280.unk_110A += 6;
        return;
    }
    if ((D_800E6280.unk_F5F == 9) && (D_800E6280.unk_56C[0x3E] == 0)) {
        D_8015B67C = 1;
        D_800CA148 += 1;
        D_800E6280.unk_110A += 6;
        return;
    }
    if ((D_800E6280.unk_F5F == 9) && (D_800E6280.unk_56C[0x3E] != 0)) {
        if (D_800E6280.unk_110D == 0) {
            func_80083440(3);
            D_800E6280.unk_110D = 1;
        }
        prev = D_800E6280.unk_110A;
        func_80085A60();
        if (prev != D_800E6280.unk_110A) {
            D_800CA148 = 8;
            D_800E6280.unk_110A += 0x11;
        }
        return D_800E6280.unk_110A;
    }
    func_8004284C();
}
