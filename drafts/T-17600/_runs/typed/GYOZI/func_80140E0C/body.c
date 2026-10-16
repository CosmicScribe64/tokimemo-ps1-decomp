void func_80140E0C(void) {
    s16 temp_t9;
    s16 temp_v1;
    s16 var_v1;
    s32 var_v0;

    D_80148750 = 0;
    if (!(D_800F53A0.girl[9].unk_10[0] & 2) && (D_800F53A0.girl[9].unk_06 >= 0x14) && (D_800F6468 & 1)) {
        D_80148750 = 1;
    }
    D_800F62CF = 0;
    D_80148620 = ((func_8005E0E0(0, 0) & 0x7F) == 4) * 4;
    D_80148614 = D_801481D4;
    D_80148618 = D_80148208;
    D_8014861C = D_8014823C;
    func_8008F618(D_800F62CF, &D_800F62CF);
    var_v1 = 0;
    D_800F53A0.girl[D_800F62CF].unk_0A += 4;
loop_5:
    var_v0 = var_v1 * 2;
    temp_v1 = var_v1 + 1;
    if (D_800F54AE >= D_8014875C[var_v1]) {
        temp_t9 = temp_v1;
        var_v1 = temp_t9;
        if (temp_t9 >= 7) {
            var_v0 = temp_t9 * 2;
        } else {
            goto loop_5;
        }
    }
    D_800F53A0.girl[D_800F62CF].unk_06 += *(D_8014876C + var_v0);
    func_80090960(4, 0, &D_800F53A0 + (D_800F62CF * 0x38), &D_800F53A0);
    func_8004DE1C();
}
