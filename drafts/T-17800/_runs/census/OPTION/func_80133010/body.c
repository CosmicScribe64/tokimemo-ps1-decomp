s32 func_80133010(void) {
    s32 var_a1;
    s32 var_a1_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 *var_v0;
    u8 temp_t7;
    u8 var_v0_2;
    u8 var_v1;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80048E78();
        D_8013D300 = 1;
        var_v0 = D_80120650;
        do {
            var_v0 += 0x44;
            *(s8 *)((unsigned char *)var_v0 + (-67)) = 5;
            *(s8 *)((unsigned char *)var_v0 + (-66)) = 0x40;
            *(s32 *)((unsigned char *)var_v0 + (-12)) = 0x01000000;
            *(s8 *)((unsigned char *)var_v0 + (-65)) = 0x24;
            *(s32 *)((unsigned char *)var_v0 + (-56)) = 0x801C32D8;
            *(s32 *)((unsigned char *)var_v0 + (-52)) = 0x801C34E0;
            *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x801C32C8;
            *(s8 *)((unsigned char *)var_v0 + (-64)) = 8;
            *(s16 *)((unsigned char *)var_v0 + (-44)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-63)) = 0x80;
            *(s16 *)((unsigned char *)var_v0 + (-30)) = -0x200;
            *(s16 *)((unsigned char *)var_v0 + (-26)) = -0x200;
            *(s8 *)((unsigned char *)var_v0 + (-61)) = 0x80;
        } while ((u32) var_v0 < (u32) D_80121750);
        var_s0 = 0;
        var_a1 = 0x10;
        do {
            load_palette(0x801C34F0, var_a1 & 0xFF, 1, 1, var_s0);
            var_s0 += 1;
            var_a1 = var_s0 + 0x10;
        } while (var_s0 < 4);
        load_palette(0x801B0000, 0x1F, 1, 1, 0);
        var_s0_2 = 0;
        var_a1_2 = 0x1A;
        do {
            load_palette(0x801CBEF0, var_a1_2 & 0xFF, 1, 1, var_s0_2);
            var_s0_2 += 1;
            var_a1_2 = var_s0_2 + 0x1A;
        } while (var_s0_2 != 4);
        load_tpage_buf_lock(5, 0x801C7CF0, 1);
        func_801328C0();
        D_80120668 = 0;
        D_80120676 = -0xA0;
        D_8012067A = -0x78;
        D_80120653 |= 0x80;
        D_80120651 = 8;
        D_801206AC = 0xA;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
        D_80120697 |= 0x80;
        D_80120695 = 3;
        D_80120910 = 7;
        D_8012091E = -0xA0;
        D_80120922 = -0x78;
        D_801208FB = 0x24;
        D_801208F9 = 1;
        D_80121724 = 7;
        D_80121732 = -0xA0;
        D_80121736 = -0x78;
        D_8012170F = 0x24;
        D_8012170D = 1;
    }
    var_v1 = D_800E6280.unk_110D;
    if (var_v1 != 0) {
        temp_t7 = (D_800E6280.unk_03A - 4) & 0xFF;
        var_v0_2 = temp_t7;
        if (temp_t7 < 0x81U) {
            var_v0_2 = 0x80;
            if (var_v1 == 1) {
                var_v1 = 2;
            }
        }
        D_800E6280.unk_03A = var_v0_2;
        D_800E6280.unk_110D = var_v1;
        if (var_v1 == 2) {
            func_8004284C();
        }
    }
    func_8004AC18((s32) D_800E6280.unk_03A);
}
