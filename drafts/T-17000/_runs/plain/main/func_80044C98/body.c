s32 func_80044C98(void) {
    u8 temp_t1;

    if ((u32) (((func_800460DC() - D_800B3D20) + 0x100) & 0xFF) >= 5U) {
        if (D_800B3D4C == 0) {
            func_80044D54();
            D_800B3D4C += 1;
            goto block_7;
        }
        temp_t1 = (D_800B3D4C + 1) & 0xFF;
        D_800B3D4C = temp_t1;
        if ((temp_t1 >= 0xC1U) || (func_80044F94((s32) D_800B3D24) != 0)) {
            D_800B3D54[D_800B3D24] = 0;
            return 1;
        }
block_7:
        /* Duplicate return node #8. Try simplifying control flow for better match */
        return -1;
    }
    return -1;
}
