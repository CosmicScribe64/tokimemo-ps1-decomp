/* T-24061 source-only unmatched draft. See README.md for reproduction and negatives. */
s32 func_80141870(void) {
    if (check_end_k() && D_800CA154 == 1) {
        if (D_800E6280.unk_110D++ < 0x40U) {
            return 0;
        }
        D_800E6280.unk_110D = 0;
        k_reset(1);
    }
    if (D_800CA154 == 0 || D_800CA154 == 8) {
        func_80138B74();
    } else {
        func_80138AF8();
    }
    if (D_800E6280.unk_1104.w == 1 && D_800CA154 == 1) {
        func_80044750(0x609);
    }
    if (D_800E6280.unk_1104.w++ == 1 && D_800CA154 == 9) {
        func_80044750(0x60B);
    }
}
