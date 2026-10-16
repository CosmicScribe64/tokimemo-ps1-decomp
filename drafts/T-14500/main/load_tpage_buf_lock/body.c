void load_tpage_buf_lock(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
u8 arg2;
{
    u32 idx;
    D_800E6280.unk_1228[arg0] = arg1;
    idx = arg0;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, (u8)(idx & 0x1F), (s32)arg2, 0, 0, 0x80, 0x80);
    } else {
        load_csr_tp(arg1, arg0 & 0x1F, (s32)arg2, 0, 0x80, 0x80, 0x80);
    }
}
