void func_801412A4(void) {
    s16 temp_t7;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_a0;
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s32 var_v1;
    u32 temp_hi;
    u32 temp_t8;
    u8 temp_t1;

    var_a0 = 0;
    var_v0 = 0;
    do {
        temp_v0 = var_v0 + 1;
        var_a0 += ((u32) (*(s32 *)((unsigned char *)(&D_800F53A0.girl[var_v0]) + (16)) << 0x1E) >> 0x1F) != 0;
        var_v0 = temp_v0;
    } while (temp_v0 < 0xB);
    var_v0_2 = 0;
loop_3:
    temp_hi = (u32) (D_800F6468 + var_v0_2) % 10U;
    var_v0_2 += 1;
    temp_t1 = temp_hi + 1;
    D_800F62CF = temp_t1;
    temp_t8 = (u32) (*(s32 *)((unsigned char *)(&D_800F53A0.girl[temp_t1 & 0xFF]) + (16)) << 0x1E) >> 0x1F;
    if (temp_t8 == 0) {
        if ((u32) (temp_t1 & 0xFF) >= 9U) {
            goto loop_3;
        }
    }
    if (D_80148754 == D_800F62CF) {
        goto loop_3;
    }
    if ((temp_t8 == 0) && (var_a0 >= 3)) {
        goto loop_3;
    }
    if (((D_801486E4 == 7) || (D_801486E4 == 9)) && (D_80148754 != D_801486E4)) {
        D_800F62CF = D_801486E4;
    }
    D_80148758 = D_800F62CF;
    D_80148620 = 0;
    func_80141550();
    func_8008F618(D_800F62CF);
    var_v0_3 = 0;
    if (!(*(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (16)) & 2)) {
        D_800F53A0.girl[D_800F62CF].unk_10[0] |= 2;
        D_800F5AB0 = D_800F6478;
        D_800F5AB1 = D_800F6479;
        D_800F5AB2 = 0x18;
        func_80090960(4, 0, &D_800F53A0 + (D_800F62CF * 0x38), &D_800F53A0);
        *(s8 *)((unsigned char *)(&D_800F53A0 + (D_800F62CE * 8)) + (1841)) = 2;
        func_8004DEAC(5);
        return;
    }
    D_800F53A0.girl[D_800F62CF].unk_0A += 4;
loop_15:
    var_v1 = var_v0_3 * 2;
    temp_v0_2 = var_v0_3 + 1;
    if (D_800F54AE >= *(&D_8014875C + var_v1)) {
        temp_t7 = temp_v0_2;
        var_v0_3 = temp_t7;
        if (temp_t7 >= 7) {
            var_v1 = temp_t7 * 2;
        } else {
            goto loop_15;
        }
    }
    D_800F53A0.girl[D_800F62CF].unk_06 += *(&D_8014876C + var_v1);
    func_80090960(4, 0, &D_800F53A0 + (D_800F62CF * 0x38), &D_800F53A0);
    func_8004DE1C();
}
