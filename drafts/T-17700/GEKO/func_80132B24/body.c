u8 func_80132B24(void) {
    GekoMonthDays sp1C;
    GekoThresholds sp34;
    s32 temp_a0;
    s32 temp_a2;
    u32 temp_a1_2;
    u8 temp_a1;
    u8 temp_v0;

    sp34 = D_80144CF8;
    sp1C = D_80144D10;
    if (D_800E6280.unk_03E == 0x60) {
        sp1C.day[1] = 0x1D;
    }
    temp_v0 = D_800E6280.unk_F5F;
    if (((u32) (D_800E6280.unk_1BC[temp_v0].unk_0C.w << 0x19) >> 0x1F) == 0) {
        return 0U;
    }
    if (((u32) (D_800E6280.unk_10F8 & 7) < (u32) sp34.threshold[temp_v0]) || (D_80144C50 == 0)) {
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
        if ((((u32) ((temp_a2 - temp_a1) + 0xC) % 12U) == 1) && ((u32) ((((u32) (temp_a0 << 0x17) >> 0x1B) + sp1C.day[(u32) (temp_a1 + 0xA) % 12U]) - D_800E6280.unk_040) < 0x14U)) {
            D_80144C50 = 0xA;
        }
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return temp_v0;
    }
    return temp_v0;
}
