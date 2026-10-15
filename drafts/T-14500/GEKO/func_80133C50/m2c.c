extern ? D_800CA280;
extern u16 D_800E6374;
extern u8 D_800E71DF;
extern ? D_80144D8C;

void func_80133C50(void) {
    ? sp30;
    s32 temp_v0;
    s32 var_at;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t8;
    u32 temp_t9;

    M2C_MEMCPY_ALIGNED(&sp30, &D_80144D8C, 0xF0);
    func_800AE0F0(&D_800CA280, &sp30 + (((u16) D_800E6374 >> 0xC) * 0x14));
    D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 1;
    if (D_80144C50 == 0xA) {
        D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 0x40;
    }
    D_80144C3C = 0x16;
    temp_v0 = func_80051A68(D_800E71DF, &D_800E6280, 0x38);
    if (D_80144C50 == 2) {
        D_80144C50 = 0;
        if (D_800E71DF == 0) {
            temp_t9 = (u16) D_800E6374 >> 0xC;
            temp_t1 = (u8) D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[2] >> 4;
            if ((temp_t9 == temp_t1) && (temp_t9 < 6U)) {
                func_80132A4C(2);
            } else if ((temp_t9 != temp_t1) && (temp_t9 < 6U)) {
                var_at = temp_t9 < 6U;
                if (temp_t1 < 6U) {
                    func_80132A4C(3);
                } else {
                    goto block_12;
                }
            } else {
                var_at = temp_t9 < 6U;
block_12:
                if ((var_at != 0) && (temp_t1 >= 6U)) {
                    func_80132A4C(4);
                } else {
                    func_80132A4C(5);
                }
            }
        } else if ((D_800E71DF == 1) || (D_800E71DF == 2) || (D_800E71DF == 3)) {
            temp_t2 = (u16) D_800E6374 >> 0xC;
            temp_t8 = (u8) D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[2] >> 4;
            if (temp_t2 == temp_t8) {
                func_80132A4C(2);
            } else if ((temp_t2 != temp_t8) && (temp_t2 < 6U)) {
                func_80132A4C(3);
            } else {
                func_80132A4C(5);
            }
        } else if ((D_800E71DF == 4) || (D_800E71DF == 5) || (D_800E71DF == 0xA)) {
            if ((u32) ((u16) D_800E6374 >> 0xC) < 6U) {
                func_80132A4C(3);
            } else if ((u32) (temp_v0 & 0x7F) < 2U) {
                func_80132A4C(4);
            } else {
                func_80132A4C(5);
            }
        } else if ((D_800E71DF == 6) || (D_800E71DF == 7) || (D_800E71DF == 8) || (D_800E71DF == 9)) {
            if ((u32) ((u16) D_800E6374 >> 0xC) < 6U) {
                func_80132A4C(2);
            } else {
                func_80132A4C(3);
            }
        }
    }
    func_8004284C();
}
