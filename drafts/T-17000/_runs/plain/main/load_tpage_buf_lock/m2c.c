extern ? D_800E74A8;

void load_tpage_buf_lock(s32 arg0, s32 arg1, u8 arg2) {
    *(&D_800E74A8 + (arg0 * 4)) = arg1;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, arg0 & 0x1F, (s32) arg2, 0, /* extra? */ 0, /* extra? */ 0x80, /* extra? */ 0x80);
        return;
    }
    load_csr_tp(arg1, arg0 & 0x1F, (s32) arg2, 0, /* extra? */ 0x80, /* extra? */ 0x80, /* extra? */ 0x80);
}
