/* The unused K&R parameter holds the stage: as a parameter live in the entry block it cannot take $v0/$v1 (uopt cupcosts), so it gets $a0 as in the original. T-25152 */
s32 func_80138B2C(arg0)
s32 arg0;
{
    s32 var_a1;
    s32 var_v0;
    s32 var_v1;
    u8 temp_v0;

    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    /* FAKE: T-25152, byte view of the stage: the indirect load stops copy propagation, so arg0 stays a variable instead of the promoted global */
    arg0 = *(u8 *)&D_8015EDCC;
    D_8015F490[0] = D_8015EDF0 += arg0 * 0xF4240;
    D_800E6280.unk_038 = 0;
    var_a1 = 0;
    if (arg0 >= 3U) {
        temp_v0 = D_8015EDB4->unk84[1];
        if (temp_v0 == 0xFF) {
            var_v1 = 0x7A120;
        } else {
            var_v1 = temp_v0 * 0x3E8;
        }
        var_a1 = D_8015EDEC * 0x64;
    } else {
        var_v1 = D_8015EDB4->unk84[1] * 0x64;
    }
    if (D_8015EDB4->unk80 != 0) {
        var_v0 = 0x7A120;
    } else if (D_8015EDB4->unk7E != 0) {
        var_v0 = 0x3D090;
    } else if (D_8015EDB4->unk7C != 0) {
        var_v0 = 0x186A0;
    } else {
        var_v0 = 0;
    }
    /* FAKE: T-25152, word view of the score for the same reason: the total stays in arg0 ($a0) and is stored in place, before the time bonus */
    arg0 = *(s32 *)&D_8015EDF0 + var_v1 + var_v0 + var_a1;
    D_8015F490[2] = var_v0;
    D_8015F490[1] = var_v1;
    D_8015F490[4] = arg0;
    D_8015F490[3] = var_a1;
    if (arg0 >= 0x989680) {
        arg0 = 0x98967F;
        D_8015F490[4] = arg0;
    }
    func_80044750(0x24);
    func_80059048();
    func_80143AF4();
    func_8004284C();
}
