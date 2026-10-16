s32 file_checksum_ok(arg0, arg1)
u8 arg0;
u8 *arg1;
{
    u32 lo;
    s32 temp_t0;
    u32 var_v0;
    u32 temp_a2;
    u32 var_v1;
    u8 *var_a0;
    u8 *var_a3;
    u8 *temp_a0;
    var_v0 = 0;
    var_v1 = 0;
    if (((arg0 << 0xB) - 4) != 0)
    {
        var_a0 = arg1;
        var_a3 = arg1 + 1;
        do
        {
            var_v1 += 2;
            var_v0 += (*var_a0) + (*var_a3);
            temp_t0 = var_v0 & 0xFFFF;
            var_a0 += 2;
            var_a3 += 2;
            var_v0 = temp_t0;
        }
        while (var_v1 < ((arg0 << 0xB) - 4));
    }
    temp_a0 = var_v1 + arg1;
    temp_a2 = temp_a0[0];
    lo = temp_a0[1];
    if (var_v0 != ((temp_a2 << 8) + lo))
    {
        return 0;
    }
    return 1;
}
