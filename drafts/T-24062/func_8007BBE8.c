void func_8007BBE8(arg0, channel)
AudioBankView50 *arg0;
u16 channel;
{
    s32 i;
    s32 offset;

    for (i = 0; i < 16; i++) {
        offset = arg0->unk_00[i];
        if (offset == 0) {
            offset = arg0->unk_00[0];
        }
        D_80125DA0[channel][i] = (s32)((u8 *)arg0 + offset + 0x10);
        D_80125E20[channel][i] = arg0->unk_40[i];
    }
}
