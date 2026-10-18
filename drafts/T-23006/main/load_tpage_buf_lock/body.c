void load_tpage_buf_lock(arg0, arg1, arg2)
s32 arg0;
s32 arg1;
u8 arg2;
{
    D_800E6280.unk_1228[arg0] = arg1;
    if (arg0 < 0x20) {
        load_csr_tp(arg1, (u8)(arg0 & 0x1F), arg2 & 0xFF, 0, 0, 0x80, 0x80);
    } else {
        load_csr_tp(arg1, (u8)(arg0 & 0x1F), arg2 & 0xFF, 0, 0x80, 0x80, 0x80);
    }
}
