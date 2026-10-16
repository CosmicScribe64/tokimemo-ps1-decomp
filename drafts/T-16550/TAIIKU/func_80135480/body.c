void func_80135480(void) {
    s32 sp4E[1];
    TaiikuS11 sp38;
    s32 *var_v0_4;
    TaiRunnersA *var_v0_2;
    s16 *var_v0;
    s16 *var_v1;
    s16 temp_t1;
    s16 var_v0_3;
    s16 var_v1_2;
    s32 temp_v1;
    u8 *var_a0;


    sp38 = D_801494F0;
    var_v1 = D_80149320;
    var_v0 = sp38.v;
    *(u16 *)((unsigned char *)(sp38.v) + (20)) = (u16) D_801494F0.v[0xA];
    do {
        temp_t1 = *var_v0;
        var_v0 += 2;
        var_v1 += 2;
        *(s16 *)((unsigned char *)var_v1 + (-2)) = temp_t1;
    } while ((u32) var_v0 < (u32) &sp4E);
    var_v0_2 = &D_80149208;
    var_a0 = D_801491E0;
    var_v1_2 = 0;
    do {
        var_v0_2->player.unk_20.halves.high = var_v1_2;
        var_v1_2 += 8;
        var_a0 += 1;
        var_v0_2 += 0x24;
        *(s8 *)((unsigned char *)var_a0 + (-1)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-36)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-35)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-34)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-30)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-33)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-32)) = 0;
        *(s16 *)((unsigned char *)var_v0_2 + (-24)) = 0;
        *(s16 *)((unsigned char *)var_v0_2 + (-22)) = 0;
        *(s32 *)((unsigned char *)var_v0_2 + (-20)) = 0;
        *(s32 *)((unsigned char *)var_v0_2 + (-16)) = 0;
        *(s32 *)((unsigned char *)var_v0_2 + (-12)) = 0;
        *(s32 *)((unsigned char *)var_v0_2 + (-8)) = 0;
        *(s16 *)((unsigned char *)var_v0_2 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-31)) = 0;
        *(s16 *)((unsigned char *)var_v0_2 + (-28)) = 0;
        *(s16 *)((unsigned char *)var_v0_2 + (-26)) = 0;
        *(s8 *)((unsigned char *)var_v0_2 + (-29)) = 0;
    } while (var_v1_2 < 0x20);
    D_80149208.player.unk_20.halves.high = -0xA0;
    D_80149204 = 0;
    D_801491D4 = 0;
    D_8014929C = 0;
    D_80149298.word = 0;
    temp_v1 = D_800E6280.unk_0F4.b[1] & 0xF;
    var_v0_3 = D_800E6280.unk_10C.unk_02;
    D_801492A0 = 0;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0_3 = (s16) (var_v0_3 >> 1);
    }
    if (var_v0_3 >= 0xA0) {
        D_801491E4 = 0x280;
        D_801491E8 = 0x20000;
    } else if (var_v0_3 >= 0x7D) {
        D_801491E4 = 0x220;
        D_801491E8 = 0x1E000;
    } else if (var_v0_3 >= 0x64) {
        D_801491E4 = 0x1C0;
        D_801491E8 = 0x1C000;
    } else if (var_v0_3 >= 0x4B) {
        D_801491E4 = 0x160;
        D_801491E8 = 0x1A000;
    } else {
        D_801491E4 = 0x100;
        D_801491E8 = 0x18000;
    }
    func_800AE120(D_800E6280.unk0);
    var_v0_4 = &D_801492A4;
    do {
        var_v0_4 += 1;
        *(s8 *)((unsigned char *)var_v0_4 + (-1)) = 0xFF;
    } while (var_v0_4 != &D_801492A6);
    D_801492A8 = 0;
    *(s8 *)((unsigned char *)(&D_801492A8) + (1)) = 0x3C;
    D_80122E94 = 0xFF;
    D_801491DC = 0xFF;
}
