/* T-24061 source-only unmatched draft. See README.md for reproduction and negatives. */
s32 func_80141870(void) {
    s32 ret;
    s32 v;


    if (check_end_k() && ((u16) D_800CA154 == 1)) {
        if (D_800E6280.unk_110D++ < 64U) {
            return 0;
        }
        *(u8 *)&D_800E6280.unk_110D = 0;
        k_reset(1);
    }
    if (((u16) D_800CA154 == 0) || ((u16) D_800CA154 == 8)) {
        func_80138B74();
    } else {
        func_80138AF8();
    }
    v = *(s32 *)&D_800E6280.unk_1104.w;
    if ((v == 1) && ((u16) D_800CA154 == 1)) {
        func_80044750(0x609);
        v = *(s32 *)&D_800E6280.unk_1104.w;
    }
    ret = (v ^ 1) == 0;
    *(s32 *)&D_800E6280.unk_1104.w = v + 1;
    if (ret != 0) {
        if ((u16) D_800CA154 == 9) {
            ret = func_80044750(0x60B);
        }
    }
    return ret;
}
