extern u8 D_800E71DF;

void func_80143F48(void) {
    u8 sp4B;
    u8 sp4A;
    u8 sp49;
    u8 sp48;                                        /* compiler-managed */
    ? sp30;
    s32 temp_lo;
    u8 temp_t0;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t9;
    u8 temp_v1;

    sp30.unk0 = (s32) D_80147C3C.unk0;
    *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80147C3C) + (4));
    *(s32 *)((unsigned char *)(&sp30) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80147C3C) + (8));
    *(s32 *)((unsigned char *)(&sp30) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80147C3C) + (12));
    *(s32 *)((unsigned char *)(&sp30) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80147C3C) + (16));
    *(u8 *)((unsigned char *)(&sp30) + (20)) = (u8) D_80147C3C.v[0x14];
    if (D_80122CDC != 0) {
        D_801476CC = D_8014765C;
        D_801476D0 = D_80147690;
        D_801476D4 = D_801476C4;
        temp_lo = (s16) D_800E6280.unk_1BC[D_800E71DF].unk_0A / 20;
        D_801476D8 = (s16) temp_lo;
        if ((s16) temp_lo >= 5) {
            D_801476D8 = 4;
        }
    } else {
        sp48 = (*(&sp30 + D_801476E0) & 0x1F) | (sp48 & 0xFFE0);
        sp48 = ((func_80144198(0) << 5) & 0xFE0) | ((u16) sp48 & 0xF01F);
        sp49 = (func_80144198(1) * 0x10) | (sp49 & 0xFF0F);
        temp_t4 = (func_80144198(2) & 0x1F) | (sp4A & 0xFFE0);
        sp4A = temp_t4;
        temp_t4_2 = sp4B | 1;
        temp_t9 = ((D_800E71DF * 2) & 0x1E) | (temp_t4_2 & 0xE1);
        sp4B = temp_t4_2;
        temp_t0 = temp_t9 | 0x20;
        sp4B = temp_t9;
        sp4B = temp_t0;
        sp4A = (temp_t4 & 0x1F) | 0xC0;
        sp4B = temp_t0 & 0x3F;
        func_8006B5BC((GsWord) (s32) sp48);
        temp_v1 = *(u8 *)((unsigned char *)(sp + D_801476E0) + (48));
        if (temp_v1 == 0xE) {
            D_800E6280.unk_74C[D_800E71DF] |= 1;
        }
        if (temp_v1 == 0xF) {
            D_800E6280.unk_74C[D_800E71DF] |= 2;
        }
        if (temp_v1 == 7) {
            D_800E6280.unk_74C[D_800E71DF] |= 4;
        }
    }
    func_8004284C();
}
