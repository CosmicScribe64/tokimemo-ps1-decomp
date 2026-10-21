void func_80140E0C(void) {
    s16 i;

    D_80148750 = 0;
    if (((GyoziBits32 *)D_800F53A0.girl[9].unk_10)->b1 == 0 &&
        D_800F53A0.girl[9].unk_06 >= 20 && (D_800F6468 & 1)) {
        D_80148750 = 1;
    }
    *(u8 *)&D_800F62CF *= 0;
    D_80148620 = (((u8)func_8005E0E0(D_800F53A0.unk_F2F) & 0x7F) == 4) * 4;
    D_80148614 = D_801481D4;
    D_80148618 = D_80148208;
    D_8014861C = D_8014823C;
    func_8008F618(D_800F62CF);
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
