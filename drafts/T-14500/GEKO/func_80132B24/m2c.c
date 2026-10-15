extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E71DF;
extern s32 D_800E7378;
extern ? D_80144CF8;
extern ? D_80144D10;

u8 func_80132B24(void) {
    s16 sp1E;
    s32 temp_a0;
    s32 temp_a2;
    u32 temp_a1;

    sp34[0].unk0 = (s32) D_80144CF8.unk0;
    *(s32 *)((unsigned char *)(&sp34[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (4));
    *(s32 *)((unsigned char *)(&sp34[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (8));
    *(s32 *)((unsigned char *)(&sp34[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (12));
    *(s32 *)((unsigned char *)(&sp34[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (16));
    *(u16 *)((unsigned char *)(&sp34[0]) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80144CF8) + (20));
    sp1C[0].unk0 = (s32) D_80144D10.unk0;
    *(s32 *)((unsigned char *)(&sp1C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (4));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (8));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (12));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (16));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (20));
    if (D_800E62BE == 0x60) {
        sp1E = 0x1D;
    }
    if (((u32) (D_800E6280.unk_1BC[D_800E71DF].unk_0C.w << 0x19) >> 0x1F) == 0) {
        return 0U;
    }
    if (((u32) (D_800E7378 & 7) < (u32) (&sp34[0])[D_800E71DF]) || (D_80144C50 == 0)) {
        temp_a0 = D_800E6280.unk_1BC[D_800E71DF].unk_10.w;
        temp_a2 = temp_a0 & 0xF;
        if (D_800E62BF == temp_a2) {
            temp_a1 = ((u32) (temp_a0 << 0x17) >> 0x1B) - D_800E62C0;
            if ((temp_a1 < 0x15U) && (temp_a1 != 0)) {
                D_80144C50 = 0xA;
                return D_800E71DF;
            }
            /* Duplicate return node #13. Try simplifying control flow for better match */
            return D_800E71DF;
        }
        if ((((u32) ((temp_a2 - D_800E62BF) + 0xC) % 12U) == 1) && ((u32) ((((u32) (temp_a0 << 0x17) >> 0x1B) + (&sp1C[0])[(u32) (D_800E62BF + 0xA) % 12U]) - D_800E62C0) < 0x14U)) {
            D_80144C50 = 0xA;
        }
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return D_800E71DF;
    }
    return D_800E71DF;
}
