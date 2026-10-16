u8 func_80132B24(void) {
    s16 sp1E;
    s32 temp_a0;
    s32 temp_a2;
    u32 temp_a1_2;
    u8 temp_a1;
    u8 temp_v0;

    sp34[0].unk0 = (s32) D_80144CF8.unk0;
    *(s32 *)((unsigned char *)(&sp34[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (4));
    *(s32 *)((unsigned char *)(&sp34[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (8));
    *(s32 *)((unsigned char *)(&sp34[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (12));
    *(s32 *)((unsigned char *)(&sp34[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80144CF8) + (16));
    *(u16 *)((unsigned char *)(&sp34[0]) + (20)) = (u16) D_80144CF8.threshold[0xA];
    sp1C[0].unk0 = (s32) D_80144D10.unk0;
    *(s32 *)((unsigned char *)(&sp1C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (4));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (8));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (12));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (16));
    *(s32 *)((unsigned char *)(&sp1C[0]) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80144D10) + (20));
    if (D_800E6280.unk_03E == 0x60) {
        sp1E = 0x1D;
    }
    temp_v0 = D_800E6280.unk_F5F;
    if (((u32) (D_800E6280.unk_1BC[temp_v0].unk_0C.w << 0x19) >> 0x1F) == 0) {
        return 0U;
    }
    if (((u32) (D_800E6280.unk_10F8 & 7) < (u32) (&sp34[0])[temp_v0]) || (D_80144C50 == 0)) {
        temp_a0 = D_800E6280.unk_1BC[temp_v0].unk_10.w;
        temp_a1 = D_800E6280.unk_03F;
        temp_a2 = temp_a0 & 0xF;
        if (temp_a1 == temp_a2) {
            temp_a1_2 = ((u32) (temp_a0 << 0x17) >> 0x1B) - D_800E6280.unk_040;
            if ((temp_a1_2 < 0x15U) && (temp_a1_2 != 0)) {
                D_80144C50 = 0xA;
                return temp_v0;
            }
            /* Duplicate return node #13. Try simplifying control flow for better match */
            return temp_v0;
        }
        if ((((u32) ((temp_a2 - temp_a1) + 0xC) % 12U) == 1) && ((u32) ((((u32) (temp_a0 << 0x17) >> 0x1B) + (&sp1C[0])[(u32) (temp_a1 + 0xA) % 12U]) - D_800E6280.unk_040) < 0x14U)) {
            D_80144C50 = 0xA;
        }
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return temp_v0;
    }
    return temp_v0;
}
