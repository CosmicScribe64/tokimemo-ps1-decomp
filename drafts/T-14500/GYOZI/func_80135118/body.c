void func_80135118(void) {
    s16 temp_v0;
    s32 temp_a0;

    if ((func_8005B32C() != 0) && (D_80145EB8 < 5)) {
        if (D_80145EB8 == 0) {
            temp_v0 = D_80145F64 + 1;
            if (D_80145F64 < 0x5A) {
                D_80145F64 = temp_v0;
                if (temp_v0 == 1) {
                    func_80086AB0(0x60A);
                }
                if (D_80145F64 == 0x1E) {
                    func_80086AB0(0x604);
                }
                return;
            }
            if (!(D_800F647D & 7)) {
                temp_a0 = (((*(&D_800C3370 + ((D_800F62CF * 7) + ((u8) D_800F647D >> 3))) + 0x505) & 0xFF) | 0x600) & 0xFFFF;
                D_800F647D = D_800F647D;
                func_80086AB0(temp_a0);
            }
            goto block_11;
        }
block_11:
        if ((u8) D_800F647D < 0x40U) {
            D_800F647D = (D_800F647D + 1) & 0xFF;
            return;
        }
        D_800F647D = 0;
        func_8005B06C(1);
        goto block_14;
    }
block_14:
    func_801343A4();
    if (D_80145EB8 < 5) {
        D_800F6474 += 1;
        if (D_800F6474 == 1) {
            func_80086AB0(0x608);
        }
    }
}
