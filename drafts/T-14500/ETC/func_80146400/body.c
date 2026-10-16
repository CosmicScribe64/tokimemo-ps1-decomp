/* FAKE: unused padding array preserves the frame layout. T-10850 */
s32 func_80146400(void)
{
    s32 x;
    s32 pad[3];
    s32 *p;
    s32 a;
    s32 r;
    s32 d;
    switch (D_800E6280.unk_03E)
    {
        case 0x5F:
            x = 0;
            break;

        case 0x60:
            x = 3;
            break;

        case 0x61:
            x = 6;
            break;

        default:
            x = 6;
            break;

    }

    if (D_800E6280.unk_03F != 2)
    {
        if (D_800E6280.unk_03F != 7)
        {
            if (D_800E6280.unk_03F == 0xC)
            {
                x += 1;
            }
        }
    }
    else
    {
        x -= 1;
    }
    p = &D_801506F8[x];
    func_800AE0B0("補正 %d ", *p);
    r = func_800AE0D0() & 7;
    a = *p;
    d = r + a;
    d -= 8;
    if (d > 0)
    {
        return a;
    }
    /* FAKE: zero expression preserves the comparison live range. T-10850 */
    return (d > 0) * 0;
}
