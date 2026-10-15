/* NON_MATCHING: T-10720, register assignment only: the promoted D_800E6280.unk_03A lands in v1 (plus a copy in a1) and the pointer in v0, the original has the value in a1 and the pointer in v1 with lui v0 for D_80150138 hoisted; the loop is written over D_80121818 so the end folds to a single lui/addiu, about 25 lines */
void func_80143F24(void) {
    s32 i;

    func_80142AE8();
    if (D_800E6280.unk_03A != 0) {
        for (i = 0; i < 3; i++) {
            ((u8 *)&D_80121818)[i * 0x24 + 0x16] = D_800E6280.unk_03A;
            ((u8 *)&D_80121818)[i * 0x24 + 0x15] = D_800E6280.unk_03A;
            ((u8 *)&D_80121818)[i * 0x24 + 0x14] = D_800E6280.unk_03A;
        }
        D_800E6280.unk_03A -= 1;
    }
    D_80150138 -= 0xC8;
    if (D_80150138 < -0x7530) {
        func_80059048();
        if (D_800E6280.unk_F5F >= 0xD) {
            main_state_set(0x12);
        } else {
            main_state_set(0xC3);
        }
    }
}
