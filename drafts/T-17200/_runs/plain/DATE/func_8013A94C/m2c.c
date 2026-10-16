extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern ? D_800E644C;
extern u8 D_800E6561;
extern u8 D_800E71DF;
extern u32 D_800E7378;

void func_8013A94C(void) {
    u8 sp6B;
    s16 sp42;
    u8 sp3B;
    s16 temp_t1;
    s16 var_t1;
    s32 temp_a0;
    s32 temp_t5;
    s32 temp_t6;
    s32 temp_v1;
    u32 temp_v0;

    sp54[0].unk0 = (s32) D_8015B75C.unk0;
    *(s32 *)((unsigned char *)(&sp54[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015B75C) + (4));
    *(s32 *)((unsigned char *)(&sp54[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015B75C) + (8));
    *(s32 *)((unsigned char *)(&sp54[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015B75C) + (12));
    *(s32 *)((unsigned char *)(&sp54[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8015B75C) + (16));
    *(u16 *)((unsigned char *)(&sp54[0]) + (20)) = (u16) D_8015B75C.value[0xA];
    sp3C[0].unk0 = (s32) D_8015B774.unk0;
    *(s32 *)((unsigned char *)(&sp3C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8015B774) + (4));
    *(s32 *)((unsigned char *)(&sp3C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8015B774) + (8));
    *(s32 *)((unsigned char *)(&sp3C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8015B774) + (12));
    *(s32 *)((unsigned char *)(&sp3C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8015B774) + (16));
    *(u16 *)((unsigned char *)(&sp3C[0]) + (20)) = (u16) D_8015B774.value[0xA];
    if ((D_800E62BE == 0x61) && ((u8) D_800E62BF >= 6U)) {
        sp42 += 1;
    } else if (D_800E62BE == 0x62) {
        sp42 += 1;
    }
    D_8015BF1C = (s16) D_80122CDC;
    D_8015B65C = 0;
    sp3B = 0;
    temp_v1 = *(&D_800E644C + (D_800E71DF * 0x38));
    temp_a0 = temp_v1 & 0xF;
    D_8015BF20 = (s16) ((u32) D_800E7378 % (u32) (((&sp3C[0])[D_800E71DF] * (D_80122CDC == 1)) + ((D_80122CDC == 0) * (&sp54[0])[D_800E71DF])));
    if (D_800E62BF == temp_a0) {
        if ((u8) D_800E62C0 < (u32) ((u32) (temp_v1 << 0x17) >> 0x1B)) {
            D_8015B65C = 1;
        }
    } else if (((u32) ((temp_a0 - D_800E62BF) + 0xC) % 12U) == 1) {
        D_8015B65C = 1;
    }
    D_8015B660 = 0;
    if ((((D_800E62BE == 0x61) && ((u8) D_800E62BF >= 3U)) || (D_800E62BE == 0x62)) && ((func_800AE0D0() % 2) != 0)) {
        D_8015B660 = 1;
    }
    sp6B = func_80051A68(D_800E71DF) & 0x7F;
    temp_v0 = func_80051B48(D_800E71DF);
    temp_t5 = temp_v0 & 0xFF;
    if ((sp6B == 0) && (temp_t5 == 0)) {
        if (!(D_800E6280.unk_1BC[D_800E71DF].unk_0C.w & 0x20)) {
            sp3B = 1;
            D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 0x20;
        }
    }
    if ((sp3B != 0) && (D_80122CDC == 0)) {
        D_8015BF1C = 4;
        goto block_44;
    }
    if ((D_8015B65C != 0) && (D_80122CDC == 0)) {
        temp_t6 = *(&D_800E644C + (D_800E71DF * 0x38)) & 0xF;
        D_8015BF1C = 2;
        var_t1 = D_800E62BE - 0x5F;
        if (((temp_t6 == 1) && (D_800E62BF == 1)) || ((temp_t6 == 2) && ((D_800E62BF == 1) || (D_800E62BF == 2))) || ((temp_t6 == 3) && ((D_800E62BF == 1) || (D_800E62BF == 2) || (D_800E62BF == 3)))) {
            var_t1 = D_800E62BE - 0x60;
        }
        if (D_800E71DF == 6) {
            if (sp6B < 2U) {
                var_t1 = 0;
                if ((u32) (temp_v0 & 0xFF) < 2U) {
                    var_t1 = 1;
                }
            } else {
                var_t1 = 0;
            }
        }
        D_8015BF20 = var_t1;
    } else if ((D_8015B660 != 0) && (D_80122CDC == 0)) {
        D_8015BF1C = 3;
block_44:
        D_8015BF20 = 0;
    }
    temp_t1 = D_8015BF20;
    if (D_800E71DF == 5) {
        D_8015BF20 = temp_t1;
        if (D_8015BF1C == 0) {
            D_8015BF20 = temp_t1;
            if (temp_t1 == 3) {
                D_800E6561 |= 0x40;
            }
        }
    }
    func_8004284C();
}
