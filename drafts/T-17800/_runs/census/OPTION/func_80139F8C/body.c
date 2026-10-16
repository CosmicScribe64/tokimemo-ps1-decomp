/* FAKE: width-staged locals preserve raw-byte and call-argument ranges. T-10850 */
void func_80139F8C(void)
{
    s32 t;
    u32 value;
    u16 code;
    t = D_800E6280.unk_F88 & 0x60;
    if (t != 0x20)
    {
        if (t == 0x40)
        {
            D_8013D3A8--;
        }
    }
    else
    {
        D_8013D3A8++;
    }
    if (D_8013D3A8 > 0x10)
    {
        value = D_8013D3A8;
        code = value | 0x1200;
        t = code & 0xFFFF;
        func_80044750(t);
    }
}
