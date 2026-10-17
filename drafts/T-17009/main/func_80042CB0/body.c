void func_80042CB0(void) {
    s16 x;
    s16 y;
    s32 keys;
    u8 cnt;

    D_800E6280.unk_F84 = D_800E6280.unk_F80;
    x = D_800E6280.unk_F7A + (s16)((s32)(*(s8 *)&D_800E8BF0[4] * D_800E6280.unk_F6F) / 3);
    y = D_800E6280.unk_F7C + (s16)((s32)(*(s8 *)&D_800E8BF0[5] * D_800E6280.unk_F6F) / 3);
    if (x >= 0x8D) {
        x = 0x8C;
    }
    if (x < -0x96) {
        x = -0x96;
    }
    if (y < -0x64) {
        y = -0x64;
    }
    if (y >= 0x65) {
        y = 0x64;
    }
    D_8011ECF6 = x;
    D_8011ECFA = y;
    D_800E6280.unk_F76 = ~D_800E8BF0[3];
    D_800E6280.unk_F7A = x;
    D_800E6280.unk_F77 = D_800E6280.unk_F76 & ~D_800E6280.unk_F78;
    D_800E6280.unk_F7C = y;
    if (D_800E6280.unk_F70 != 0 && (D_800E6280.unk_F71 == 0 || D_800E6280.unk_F77 != D_800E6280.unk_F72) && D_800E6280.unk_F77 != 0) {
        if (D_800E6280.unk_F77 & 4) {
            D_800E6280.unk_F72 = D_800E6280.unk_F77;
            D_800E6280.unk_F71 = (u8)D_800E6280.unk_F73 + 8;
        } else if (D_800E6280.unk_F77 & 8) {
            D_800E6280.unk_F72 = D_800E6280.unk_F77;
            D_800E6280.unk_F71 = (u8)D_800E6280.unk_F73 + 8;
        }
    }
    cnt = D_800E6280.unk_F71;
    keys = 0;
    if (D_800E6280.unk_F70 == 0 || (D_800E6280.unk_F70 != 0 && cnt != 0 && (u8)D_800E6280.unk_F73 + 8 != cnt && D_800E6280.unk_F77 == D_800E6280.unk_F72)) {
        D_800E6280.unk_F72 = 0;
        cnt = 0;
        if (D_800E6280.unk_F75 != 0) {
            if (D_800E6280.unk_F76 & 4) {
                keys |= 0x20;
            } else if (D_800E6280.unk_F76 & 8) {
                keys |= 0x40;
            }
        } else if (D_800E6280.unk_F76 & 8) {
            keys |= 0x20;
        } else if (D_800E6280.unk_F76 & 4) {
            keys |= 0x40;
        }
    }
    if (cnt != 0) {
        D_800E6280.unk_F71 = cnt - 1;
    } else {
        D_800E6280.unk_F71 = cnt;
        D_800E6280.unk_F72 = 0;
    }
    if (D_800E8C30[0] == 0 && (D_800E8C30[1] & 0xF0) == 0x40) {
        keys |= (~D_800E8C30[2] << 24) | ((~D_800E8C30[3] & 0xFF) << 16);
    }
    D_800E6280.unk_F88 = ~D_800E6280.unk_F84 & keys;
    D_800E6280.unk_F8C = D_800E6280.unk_F84 & ~keys;
    D_800E6280.unk_F78 = D_800E6280.unk_F76;
    D_800E6280.unk_F80 = keys;
}
