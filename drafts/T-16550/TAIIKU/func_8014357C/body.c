void func_8014357C(void) {
    s16 var_v0;
    s32 temp_v1;
    s32 var_v0_2;

    *D_8014A120 = 0;
    D_8014A13C->unk_01 = 0;
    D_8014A13C->unk_00 = 0;
    D_8014A13C->unk_04 = 0;
    D_8014A13C->unk_08 = 0;
    D_8014A13C->unk_0C = 0;
    D_8014A13C->unk_10 = 0;
    D_8014A13C->unk_14.halves.low = 0;
    D_8014A13C->unk_1C = 1;
    D_8014A13C->unk_20 = 0;
    D_8014A13C->unk_1A = 0;
    D_8014A13C->unk_18 = 0;
    D_8014A13C->unk_02 = 0;
    D_8014A121 = 0;
    D_8014A13C[1].unk_01 = 0;
    D_8014A13C[1].unk_00 = 0;
    D_8014A13C[1].unk_04 = 0;
    D_8014A13C[1].unk_08 = 0;
    D_8014A13C[1].unk_0C = 0;
    D_8014A13C[1].unk_1C = 1;
    D_8014A13C[1].unk_14.halves.high = 8;
    D_8014A13C[1].unk_10 = 0;
    D_8014A13C[1].unk_14.halves.low = 0;
    D_8014A13C[1].unk_20 = 0;
    D_8014A13C[1].unk_1A = 0;
    D_8014A13C[1].unk_18 = 0;
    D_8014A13C[1].unk_02 = 0;
    D_8014A122 = 0;
    D_8014A13C[2].unk_01 = 0;
    D_8014A13C[2].unk_00 = 0;
    D_8014A13C[2].unk_04 = 0;
    D_8014A13C[2].unk_08 = 0;
    D_8014A13C[2].unk_14.halves.high = 0x10;
    D_8014A13C[2].unk_0C = 0;
    D_8014A13C[2].unk_10 = 0;
    D_8014A13C[2].unk_14.halves.low = 0;
    D_8014A13C[2].unk_1C = 1;
    D_8014A13C[2].unk_20 = 0;
    D_8014A13C[2].unk_1A = 0;
    D_8014A13C[2].unk_18 = 0;
    D_8014A13C[2].unk_02 = 0;
    D_8014A123 = 0;
    D_8014A13C[3].unk_01 = 0;
    D_8014A13C[3].unk_00 = 0;
    D_8014A13C[3].unk_04 = 0;
    D_8014A13C[3].unk_14.halves.high = 0x18;
    D_8014A13C[3].unk_08 = 0;
    D_8014A13C[3].unk_0C = 0;
    D_8014A13C[3].unk_10 = 0;
    D_8014A13C[3].unk_14.halves.low = 0;
    D_8014A13C[3].unk_1C = 1;
    D_8014A13C[3].unk_20 = 0;
    D_8014A13C[3].unk_1A = 0;
    D_8014A13C[3].unk_18 = 0;
    D_8014A13C[3].unk_02 = 0;
    D_8014A13C->unk_14.halves.high = -0xA0;
    D_8014A1D0 = 0;
    D_8014A1CC.w = 0;
    D_8014A118 = 0;
    temp_v1 = D_800E6280.unk_0F4.b[1] & 0xF;
    var_v0 = D_800E6280.unk_10C.unk_02;
    (*(s32 *)D_8014A130) = 0x7E0000;
    *(s32 *)((unsigned char *)D_8014A130 + (4)) = 0x7F0000;
    *(s32 *)((unsigned char *)D_8014A130 + (8)) = 0x7D0000;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0 = (s16) (var_v0 >> 1);
    }
    if (var_v0 >= 0xB4) {
        D_8014A124 = 6;
        var_v0_2 = 0x20000;
    } else if (var_v0 >= 0x64) {
        D_8014A124 = 7;
        var_v0_2 = 0x1E000;
    } else if (var_v0 >= 0x41) {
        D_8014A124 = 8;
        var_v0_2 = 0x1C000;
    } else if (var_v0 >= 0x1E) {
        D_8014A124 = 9;
        var_v0_2 = 0x1A000;
    } else {
        D_8014A124 = 0xA;
        var_v0_2 = 0x18000;
    }
    if ((u8) D_8014A110 != 0) {
        var_v0_2 -= 0x2000;
    }
    D_8014A128 = var_v0_2 >> 3;
    D_8014A12C = var_v0_2;
    func_800AE120(D_800E6280.unk0);
}
