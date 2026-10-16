void func_80137154(void) {
    s16 var_v0;
    s16 var_v0_2;
    s16 var_v0_3;
    s32 temp_t1;
    s32 temp_t2;
    s32 temp_t8;
    u8 temp_v0;
    u8 var_a0;
    u8 var_a0_2;
    u8 var_a0_3;

    D_8015B690 = (u8) D_80122CDC;
    if (D_80122CDC == 0) {
        func_800AE0F0(D_800CA1DC, "ブティック");
        if (!(*(&D_800E6452 + (D_800E71DF * 0x38)) & 1)) {
            var_a0 = 0;
        } else {
            var_a0 = 1;
        }
        if ((D_800E71DF == 0) || (D_800E71DF == 7)) {
            D_800CA2FC = var_a0;
            var_v0 = D_800CA148 + (var_a0 * 6);
        } else {
            D_800CA2FC = var_a0;
            var_v0 = D_800CA148 + (var_a0 * 2);
        }
        if ((D_800E71DF == 8) || (D_800E71DF == 6)) {
            var_v0 += 1;
        }
        D_800CA148 = var_v0;
        if (D_800E71DF == 5) {
            D_800CA148 = var_v0 + 2;
        }
        temp_t1 = D_800CA2FC * 3;
        D_80122D08 = 2;
        *D_800CA22C = (s16) *((D_800E71DF * 0x12) + D_8015B580 + temp_t1);
        D_800CA22E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t1) + (1));
        D_800CA230 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t1) + (2));
        *D_800CA21C = (s16) *((D_800E71DF * 0x12) + D_8015B584 + temp_t1);
        D_800CA21E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t1) + (1));
        D_800CA220 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t1) + (2));
        bg_read_sub2(0x4529, 5);
    } else if (D_80122CDC == 1) {
        func_800AE0F0(D_800CA1DC, "ファンシーショップ");
        if (!(*(&D_800E6453 + (D_800E71DF * 0x38)) & 1)) {
            var_a0_2 = 0;
        } else {
            var_a0_2 = 1;
        }
        if (D_800E71DF == 0) {
            D_800CA2FC = var_a0_2;
            var_v0_2 = D_800CA148 + (var_a0_2 * 2) + 0xA;
        } else if (D_800E71DF == 7) {
            D_800CA2FC = var_a0_2;
            var_v0_2 = D_800CA148 + (var_a0_2 * 2) + 8;
        } else if ((D_800E71DF == 1) || (D_800E71DF == 4) || (D_800E71DF == 9)) {
            D_800CA2FC = var_a0_2;
            var_v0_2 = D_800CA148 + (var_a0_2 * 2) + 6;
        } else if (D_800E71DF == 0xA) {
            D_800CA2FC = var_a0_2;
            var_v0_2 = D_800CA148 + (var_a0_2 * 2) + 5;
        } else {
            D_800CA2FC = var_a0_2;
            var_v0_2 = D_800CA148 + (var_a0_2 * 2) + 4;
        }
        if ((D_800E71DF == 8) || (D_800E71DF == 6)) {
            var_v0_2 += 1;
        }
        D_800CA148 = var_v0_2;
        if (D_800E71DF == 5) {
            D_800CA148 = var_v0_2 + 2;
        }
        temp_t2 = D_800CA2FC * 3;
        D_80122D08 = 3;
        *D_800CA22C = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t2) + (6));
        D_800CA22E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t2) + (7));
        D_800CA230 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t2) + (8));
        *D_800CA21C = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t2) + (6));
        D_800CA21E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t2) + (7));
        D_800CA220 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t2) + (8));
        bg_read_sub2(0x4532, 5);
    } else {
        func_800AE0F0(D_800CA1DC, "ジャンク屋");
        temp_v0 = *(&D_800E6454 + (D_800E71DF * 0x38));
        if (!(temp_v0 & 1)) {
            var_a0_3 = 0;
        } else {
            var_a0_3 = 1;
        }
        if ((D_800E71DF == 5) && (temp_v0 != 0)) {
            var_a0_3 = 1;
        }
        if (D_800E71DF == 0) {
            var_v0_3 = D_800CA148 + (var_a0_3 * 2) + 0xE;
        } else if (D_800E71DF == 7) {
            var_v0_3 = D_800CA148 + (var_a0_3 * 2) + 0xC;
        } else if ((D_800E71DF == 1) || (D_800E71DF == 4) || (D_800E71DF == 9)) {
            var_v0_3 = D_800CA148 + (var_a0_3 * 2) + 0xA;
        } else if (D_800E71DF == 0xA) {
            var_v0_3 = D_800CA148 + (var_a0_3 * 2) + 9;
        } else {
            var_v0_3 = D_800CA148 + (var_a0_3 * 2) + 8;
        }
        if (D_800E71DF == 6) {
            D_800CA2FC = var_a0_3;
            D_800CA148 = var_v0_3 + 1;
        } else {
            D_800CA148 = var_v0_3;
            D_800CA2FC = var_a0_3;
            if (D_800E71DF == 5) {
                D_800CA148 = var_v0_3;
                D_800CA2FC = var_a0_3;
                if (var_a0_3 == 1) {
                    D_800CA148 = var_v0_3 + 1;
                }
            }
        }
        temp_t8 = D_800CA2FC * 3;
        D_80122D08 = 4;
        *D_800CA22C = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t8) + (12));
        D_800CA22E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t8) + (13));
        D_800CA230 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B580 + temp_t8) + (14));
        *D_800CA21C = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t8) + (12));
        D_800CA21E = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t8) + (13));
        D_800CA220 = (s16) *(u8 *)((unsigned char *)((D_800E71DF * 0x12) + D_8015B584 + temp_t8) + (14));
        bg_read_sub2(0x453B, 5);
    }
    func_80136F2C();
    if (*(&D_800E69D8 + (D_800E71DE * 8)) == 1) {
        D_800E71DE -= 1;
    }
    func_80085B3C(0, D_80122D08);
}
