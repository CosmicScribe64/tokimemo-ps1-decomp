void func_80138F60(void) {
    u32 sp28;
    u32 var_t0;
    u8 temp_t7;
    u8 var_t5;
    u8 var_t6;
    u8 var_t8;
    u8 *temp_v1;

    func_80041878();
    func_80138C1C();
    temp_t7 = ((((s32) ((((u32) (D_800E644C << 0x17) >> 0x1B) * 3) + (D_800E644C & 0xF)) % 11) * 0x10) | (D_800E644A & 0xFF0F)) & 0xFF;
    var_t0 = temp_t7 >> 4;
    temp_v1 = ((*(&D_8014D03C + ((((u32) (D_800E6378 << 0x15) >> 0x1E) * 4) + ((u32) (D_800E644C << 0x15) >> 0x1E))) - 1) * 3) + &D_8014D04C;
    D_800E6442 = (s16) (*(s32 *)temp_v1);
    D_800E643E = (s16) *(u8 *)((unsigned char *)temp_v1 + (1));
    D_800E6446 = (s16) *(u8 *)((unsigned char *)temp_v1 + (2));
    D_800E644A = temp_t7;
    if (var_t0 == 0) {
        D_800E6482 = (D_800E6482 & 0xFF0F) | 0x10;
    } else if (var_t0 == 1) {
        D_800E6482 &= 0xFF0F;
    } else {
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E6482 = (D_800E6482 & 0xFF0F) | 0x10;
        } else {
            D_800E6482 &= 0xFF0F;
        }
    }
    D_800E6485 = (D_800E6485 & 0xFFF9) | 2;
    if (var_t0 == 2) {
        D_800E64BA = (D_800E64BA & 0xFF0F) | 0x30;
    } else {
        if (var_t0 == 3) {
            var_t5 = (D_800E64BA & 0xFF0F) | 0x20;
            goto block_14;
        }
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E64BA = (D_800E64BA & 0xFF0F) | 0x30;
        } else {
            var_t5 = (D_800E64BA & 0xFF0F) | 0x20;
block_14:
            D_800E64BA = var_t5;
        }
    }
    D_800E64BD = (D_800E64BD & 0xFFF9) | 2;
    if (var_t0 == 4) {
        D_800E64F2 = (D_800E64F2 & 0xFF0F) | 0x50;
    } else {
        if (var_t0 == 5) {
            var_t6 = (D_800E64F2 & 0xFF0F) | 0x40;
            goto block_22;
        }
        sp28 = var_t0;
        if (func_800AE0D0() & 1) {
            D_800E64F2 = (D_800E64F2 & 0xFF0F) | 0x50;
        } else {
            var_t6 = (D_800E64F2 & 0xFF0F) | 0x40;
block_22:
            D_800E64F2 = var_t6;
        }
    }
    D_800E64F5 = (D_800E64F5 & 0xFFF9) | 4;
    if (var_t0 == 6) {
        D_800E652A = (D_800E652A & 0xFF0F) | 0x70;
    } else {
        if (var_t0 == 7) {
            var_t8 = (D_800E652A & 0xFF0F) | 0x60;
            goto block_30;
        }
        if (func_800AE0D0() & 1) {
            D_800E652A = (D_800E652A & 0xFF0F) | 0x70;
        } else {
            var_t8 = (D_800E652A & 0xFF0F) | 0x60;
block_30:
            D_800E652A = var_t8;
        }
    }
    D_800E652D = (D_800E652D & 0xFFF9) | 2;
    D_800E6562 = (D_800E6562 & 0xFF0F) | 0x80;
    D_800E6565 = (D_800E6565 & 0xFFF9) | 4;
    D_800E659A = (D_800E659A & 0xFF0F) | 0x90;
    D_800E659D = (D_800E659D & 0xFFF9) | 2;
    D_800E65D2 = (D_800E65D2 & 0xFF0F) | 0xB0;
    D_800E65D5 &= 0xFFF9;
    D_800E660A = (D_800E660A & 0xFF0F) | 0xB0;
    D_800E660D = (D_800E660D & 0xFFF9) | 4;
    D_800E6642 = (D_800E6642 & 0xFF0F) | 0xB0;
    D_800E6645 = (D_800E6645 & 0xFFF9) | 2;
    D_800E667A = (D_800E667A & 0xFF0F) | 0xA0;
    D_800E667D &= 0xFFF9;
    D_800E66B2 = (D_800E66B2 & 0xFF0F) | 0xB0;
    D_800E66B5 = (D_800E66B5 & 0xFFF9) | 2;
    D_800E66ED |= 6;
    func_80139440();
}
