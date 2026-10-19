/* T-24140 attempt at GEKO func_80132B24: m2c baseline + types, 184 diff rows
 * (frame 0x60 vs 0x50, spills, division allocation). Restored to INCLUDE_ASM
 * in src/; continue here. Shifts pre-split (cfe <<>> quirk, see
 * geko_80133308_draft.c note).
 */
u8 func_80132B24(void) {
    GekoThresholds thr;
    GekoMonthDays dys;
    s32 a0;
    s32 a2;
    u32 a1;
    s32 sh;
    s32 dv;

    thr = D_80144CF8;
    dys = D_80144D10;
    if (D_800E6280.unk_03E == 0x60) {
        dys.day[1] = 0x1D;
    }
    sh = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.w << 0x19;
    if (((u32) sh >> 0x1F) == 0) {
        return 0;
    }
    if ((((u32) (D_800E6280.unk_10F8 & 7)) < ((u32) thr.threshold[D_800E6280.unk_F5F])) || (D_80144C50 == 0)) {
        a0 = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_10.w;
        a2 = a0 & 0xF;
        if (D_800E6280.unk_03F == a2) {
            sh = a0 << 0x17;
            a1 = ((u32) sh >> 0x1B) - D_800E6280.unk_040;
            if ((a1 < 0x15U) && (a1 != 0)) {
                D_80144C50 = 0xA;
            }
            return D_800E6280.unk_F5F;
        }
        dv = (a2 - D_800E6280.unk_03F) + 0xC;
        if ((((u32) (dv % 12U)) == 1) && ((u32) (((((u32) (a0 << 0x17) >> 0x1B)) + dys.day[(D_800E6280.unk_03F + 0xA) % 12U]) - D_800E6280.unk_040) < 0x14U)) {
            D_80144C50 = 0xA;
        }
        return D_800E6280.unk_F5F;
    }
    return D_800E6280.unk_F5F;
}
