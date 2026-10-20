/* T-24301 draft, ~100 rows; needs `void func_80141550(void);` and `extern u8 D_800F6478;`.
 * The girl count wants `sltiu; xori` (a boolean negation) and the flag word load in a ugen temporary;
 * D_800F6468 must be read as D_800F53A0.unk_10C8 (reloaded every iteration after the F2F store). */
void func_801412A4(void) {
    s16 i;
    s16 count;
    u32 flag;

    count = 0;
    for (i = 0; i < 11; i++) {
        count += !(((GyoziBits32 *)D_800F53A0.girl[i].unk_10)->b1 == 0);
    }
    i = 0;
    do {
        D_800F53A0.unk_F2F = (D_800F53A0.unk_10C8 + i++) % 10U + 1;
        flag = ((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1;
    } while ((flag == 0 && D_800F53A0.unk_F2F >= 9) || D_80148754 == D_800F53A0.unk_F2F ||
             (flag == 0 && count >= 3));
    if ((D_801486E4 == 7 || D_801486E4 == 9) && D_80148754 != D_801486E4) {
        D_800F53A0.unk_F2F = D_801486E4;
    }
    D_80148758 = D_800F53A0.unk_F2F;
    D_80148620 = 0;
    func_80141550();
    func_8008F618(D_800F53A0.unk_F2F);
    if (!((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1) {
        ((GyoziBits32 *)D_800F53A0.girl[D_800F53A0.unk_F2F].unk_10)->b1 = 1;
        D_800F53A0.unk_410[0x300] = D_800F6478;
        D_800F53A0.unk_410[0x301] = D_800F6479;
        D_800F53A0.unk_410[0x302] = 0x18;
        func_80090960(4, 0);
        D_800F53A0.unk_410[0x321 + D_800F53A0.unk_672[0x8BC] * 8] = 2;
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
