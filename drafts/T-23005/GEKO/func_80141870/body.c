/* T-23005 hand draft (not built in the tree): 51 rows. Reads of unk_110D and unk_1104 go through scalar views; the original loads D_800CA154 into a0 each time, this form into temporaries. */
s32 func_80141870(void) {
    s32 ret;
    s32 v;
    u8 sel;

    if (check_end_k() && ((u16) D_800CA154 == 1)) {
        sel = *(u8 *)&D_800E6280.unk_110D;
        if (sel < 0x40U) {
            *(u8 *)&D_800E6280.unk_110D = sel + 1;
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
