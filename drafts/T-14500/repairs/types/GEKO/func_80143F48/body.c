/* NON_MATCHING: T-14700, draft-only type reconstruction; see result.json. */
void func_80143F48(void) {
    GsWord packed;
    RepairBytes21 sp30;
    s32 temp_lo;
    u8 temp_t0;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t9;
    u8 temp_v1;

    sp30 = D_80147C3C;
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
        packed.b[0] = (sp30.b[D_801476E0] & 0x1F) | (packed.b[0] & 0xFFE0);
        packed.h[0] = ((func_80144198(0) << 5) & 0xFE0) | (packed.h[0] & 0xF01F);
        packed.b[1] = (func_80144198(1) * 0x10) | (packed.b[1] & 0xFF0F);
        temp_t4 = (func_80144198(2) & 0x1F) | (packed.b[2] & 0xFFE0);
        packed.b[2] = temp_t4;
        temp_t4_2 = packed.b[3] | 1;
        temp_t9 = ((D_800E71DF * 2) & 0x1E) | (temp_t4_2 & 0xE1);
        packed.b[3] = temp_t4_2;
        temp_t0 = temp_t9 | 0x20;
        packed.b[3] = temp_t9;
        packed.b[3] = temp_t0;
        packed.b[2] = (temp_t4 & 0x1F) | 0xC0;
        packed.b[3] = temp_t0 & 0x3F;
        func_8006B5BC(packed);
        temp_v1 = sp30.b[D_801476E0];
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
