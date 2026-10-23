void func_801412A4(void) {
    s16 i;
    s16 count;


    count = 0;
    for (i = 0; i < 11; i++) { count += ((GyoziBits32 *)D_800F53A0.girl[i].unk_10)->b1 >= 1; }
    i = 0;
    do {
        D_800F53A0.unk_F2F = ((s32)D_800F53A0.unk_10C8 + i) % 10U + 1;
        i++;
    } while ((!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && D_800F53A0.unk_F2F >= 9) ||
        D_800F53A0.unk_F2F == D_80148754 ||
        (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 && count >= 3));
    i = D_801486E4;
    if ((i == 7 || i == 9) && i != D_80148754) {
        D_800F53A0.unk_F2F = i;
    }
    D_80148758 = D_800F53A0.unk_F2F;
    D_80148620 = 0;
    func_80141550();
    func_8008F618(D_800F62CF);
    if (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1) {
        ((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 = 1;
        D_800F53A0.unk_710 = D_800F53A0.unk_10D8;
        D_800F53A0.unk_711 = D_800F53A0.unk_10D9;
        D_800F53A0.unk_712 = 0x18;
        func_80090960(4, 0);
        D_800F53A0.unk_713[D_800F53A0.unk_F2E * 8 + 0x1E] = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 4;
    for (i = 0; i < 7; i++) {
        if (D_800F54AE < D_8014875C[i]) {
            break;
        }
    }
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += D_8014876C[i];
    func_80090960(4, 0);
    func_8004DE1C();
}
