void func_80138F60(void) {
    u32 sp28;
    s32 temp_a3;
    s8 var_t5;
    s8 var_t6;
    s8 var_t8;
    u32 var_t0;
    u8 temp_t7;
    u8 *temp_v1;

    func_80041878();
    func_80138C1C();
    temp_a3 = D_800E6280.unk_1BC[0].unk_10.w;
    temp_t7 = ((((s32) ((((u32) (temp_a3 << 0x17) >> 0x1B) * 3) + (temp_a3 & 0xF)) % 11) * 0x10) | (D_800E6280.unk_1BC[0].unk_0C.h[1] & 0xFF0F)) & 0xFF;
    var_t0 = temp_t7 >> 4;
    temp_v1 = ((*(&D_8014D03C + ((((u32) (D_800E6280.unk_0F8 << 0x15) >> 0x1E) * 4) + ((u32) (temp_a3 << 0x15) >> 0x1E))) - 1) * 3) + &D_8014D04C;
    D_800E6280.unk_1BC[0].unk_06 = (s16) (*(s32 *)temp_v1);
    D_800E6280.unk_1BC[0].unk_02 = (s16) *(u8 *)((unsigned char *)temp_v1 + (1));
    D_800E6280.unk_1BC[0].unk_0A = (s16) *(u8 *)((unsigned char *)temp_v1 + (2));
    D_800E6280.unk_1BC[0].unk_0C.h[1] = temp_t7;
    if (var_t0 == 0) {
        D_800E6280.unk_1BC[1].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[1].unk_0C.h[1] & 0xFF0F) | 0x10);
    } else if (var_t0 == 1) {
        D_800E6280.unk_1BC[1].unk_0C.h[1] = (u8) (D_800E6280.unk_1BC[1].unk_0C.h[1] & 0xFF0F);
    } else {
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E6280.unk_1BC[1].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[1].unk_0C.h[1] & 0xFF0F) | 0x10);
        } else {
            D_800E6280.unk_1BC[1].unk_0C.h[1] = (u8) (D_800E6280.unk_1BC[1].unk_0C.h[1] & 0xFF0F);
        }
    }
    D_800E6280.unk_1BC[1].unk_10.b[1] = (D_800E6280.unk_1BC[1].unk_10.b[1] & 0xFFF9) | 2;
    if (var_t0 == 2) {
        D_800E6280.unk_1BC[2].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[2].unk_0C.h[1] & 0xFF0F) | 0x30);
    } else {
        if (var_t0 == 3) {
            var_t5 = (D_800E6280.unk_1BC[2].unk_0C.h[1] & 0xFF0F) | 0x20;
            goto block_14;
        }
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E6280.unk_1BC[2].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[2].unk_0C.h[1] & 0xFF0F) | 0x30);
        } else {
            var_t5 = (D_800E6280.unk_1BC[2].unk_0C.h[1] & 0xFF0F) | 0x20;
block_14:
            D_800E6280.unk_1BC[2].unk_0C.h[1] = var_t5;
        }
    }
    D_800E6280.unk_1BC[2].unk_10.b[1] = (D_800E6280.unk_1BC[2].unk_10.b[1] & 0xFFF9) | 2;
    if (var_t0 == 4) {
        D_800E6280.unk_1BC[3].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[3].unk_0C.h[1] & 0xFF0F) | 0x50);
    } else {
        if (var_t0 == 5) {
            var_t6 = (D_800E6280.unk_1BC[3].unk_0C.h[1] & 0xFF0F) | 0x40;
            goto block_22;
        }
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E6280.unk_1BC[3].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[3].unk_0C.h[1] & 0xFF0F) | 0x50);
        } else {
            var_t6 = (D_800E6280.unk_1BC[3].unk_0C.h[1] & 0xFF0F) | 0x40;
block_22:
            D_800E6280.unk_1BC[3].unk_0C.h[1] = var_t6;
        }
    }
    D_800E6280.unk_1BC[3].unk_10.b[1] = (D_800E6280.unk_1BC[3].unk_10.b[1] & 0xFFF9) | 4;
    if (var_t0 == 6) {
        D_800E6280.unk_1BC[4].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[4].unk_0C.h[1] & 0xFF0F) | 0x70);
    } else {
        if (var_t0 == 7) {
            var_t8 = (D_800E6280.unk_1BC[4].unk_0C.h[1] & 0xFF0F) | 0x60;
            goto block_30;
        }
        if (func_800AE0D0() & 1) {
            D_800E6280.unk_1BC[4].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[4].unk_0C.h[1] & 0xFF0F) | 0x70);
        } else {
            var_t8 = (D_800E6280.unk_1BC[4].unk_0C.h[1] & 0xFF0F) | 0x60;
block_30:
            D_800E6280.unk_1BC[4].unk_0C.h[1] = var_t8;
        }
    }
    D_800E6280.unk_1BC[4].unk_10.b[1] = (D_800E6280.unk_1BC[4].unk_10.b[1] & 0xFFF9) | 2;
    D_800E6280.unk_1BC[5].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[5].unk_0C.h[1] & 0xFF0F) | 0x80);
    D_800E6280.unk_1BC[5].unk_10.b[1] = (D_800E6280.unk_1BC[5].unk_10.b[1] & 0xFFF9) | 4;
    D_800E6280.unk_1BC[6].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[6].unk_0C.h[1] & 0xFF0F) | 0x90);
    D_800E6280.unk_1BC[6].unk_10.b[1] = (D_800E6280.unk_1BC[6].unk_10.b[1] & 0xFFF9) | 2;
    D_800E6280.unk_1BC[7].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[7].unk_0C.h[1] & 0xFF0F) | 0xB0);
    D_800E6280.unk_1BC[7].unk_10.b[1] = D_800E6280.unk_1BC[7].unk_10.b[1] & 0xFFF9;
    D_800E6280.unk_1BC[8].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[8].unk_0C.h[1] & 0xFF0F) | 0xB0);
    D_800E6280.unk_1BC[8].unk_10.b[1] = (D_800E6280.unk_1BC[8].unk_10.b[1] & 0xFFF9) | 4;
    D_800E6280.unk_1BC[9].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[9].unk_0C.h[1] & 0xFF0F) | 0xB0);
    D_800E6280.unk_1BC[9].unk_10.b[1] = (D_800E6280.unk_1BC[9].unk_10.b[1] & 0xFFF9) | 2;
    D_800E6280.unk_1BC[0xA].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[0xA].unk_0C.h[1] & 0xFF0F) | 0xA0);
    D_800E6280.unk_1BC[0xA].unk_10.b[1] = D_800E6280.unk_1BC[0xA].unk_10.b[1] & 0xFFF9;
    D_800E6280.unk_1BC[0xB].unk_0C.h[1] = (u8) ((D_800E6280.unk_1BC[0xB].unk_0C.h[1] & 0xFF0F) | 0xB0);
    D_800E6280.unk_1BC[0xB].unk_10.b[1] = (D_800E6280.unk_1BC[0xB].unk_10.b[1] & 0xFFF9) | 2;
    D_800E6280.unk_1BC[0xC].unk_10.b[1] = D_800E6280.unk_1BC[0xC].unk_10.b[1] | 6;
    func_80139440();
}
