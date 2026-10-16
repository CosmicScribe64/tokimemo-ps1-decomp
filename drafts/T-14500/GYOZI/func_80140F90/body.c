void func_80140F90(void) {
    s16 temp_s0;
    s16 temp_s0_2;
    s16 temp_t1;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s1;
    s32 temp_at;
    s32 temp_lo;
    s32 temp_v0;
    s32 var_v0;
    u32 temp_v1;
    u8 temp_t9;

    var_s1 = 0;
    var_s0 = 0;
    do {
        temp_lo = var_s0 * 0x38;
        temp_s0 = var_s0 + 1;
        temp_at = temp_s0 < 0xB;
        var_s0 = temp_s0;
        var_s1 += ((u32) (*(s32 *)((unsigned char *)(&D_800F53A0.unk_000[temp_lo]) + (440)) << 0x1E) >> 0x1F) != 0;
    } while (temp_at != 0);
    var_s0_2 = 0;
loop_3:
    temp_t9 = ((u32) (D_800F6468 + var_s0_2) % 10U) + 1;
    D_800F62CF = temp_t9;
    func_800BCDF0("num%d girl%d\n", var_s1, temp_t9 & 0xFF);
    var_s0_2 += 1;
    temp_v1 = (u32) (*(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (16)) << 0x1E) >> 0x1F;
    if (temp_v1 == 0) {
        if ((u8) D_800F62CF >= 9U) {
            goto loop_3;
        }
    }
    if ((temp_v1 == 0) && (var_s1 >= 2)) {
        goto loop_3;
    }
    if (D_80148750 != 0) {
        D_800F62CF = 9;
    }
    D_80148754 = D_800F62CF;
    D_80148620 = 0;
    temp_v0 = func_8005E0E0(D_800F62CF);
    if ((D_800F62CF == 6) && ((u32) (temp_v0 & 0x7F) >= 2U)) {
        D_80148620 += 1;
    }
    if ((temp_v0 & 0x7F) == 4) {
        D_80148620 += 1;
    }
    func_80141550();
    func_8008F618(D_800F62CF);
    if (!(*(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (16)) & 2) || ((var_s0_3 = 0, (D_80148750 != 0)) && (D_800F62CF == 9))) {
        D_800F53A0.girl[D_800F62CF].unk_10[0] |= 2;
        D_800F5AB0 = D_800F6478;
        D_800F5AB1 = D_800F6479;
        D_800F5AB2 = 0x11;
        func_80090960(4, 0);
        *(s8 *)((unsigned char *)(&D_800F53A0 + (D_800F62CE * 8)) + (1841)) = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F62CF].unk_0A += 4;
loop_19:
    var_v0 = var_s0_3 * 2;
    temp_s0_2 = var_s0_3 + 1;
    if (D_800F54AE >= *(&D_8014875C + var_v0)) {
        temp_t1 = temp_s0_2;
        var_s0_3 = temp_t1;
        if (temp_t1 >= 7) {
            var_v0 = temp_t1 * 2;
        } else {
            goto loop_19;
        }
    }
    D_800F53A0.girl[D_800F62CF].unk_06 += *(&D_8014876C + var_v0);
    func_80090960(4, 0);
    func_8004DE1C();
}
