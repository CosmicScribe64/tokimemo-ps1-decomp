? func_800AE0B0(? *, s32);                          /* extern */
s32 func_800AE0D0();                                /* extern */
extern ? D_800E6280;
extern ? D_801506F8;

s32 func_80146400(void) {
    s32 *sp2C;
    s32 *temp_v0;
    s32 temp_a0;
    s32 var_v0;
    s32 var_v1;

    if (*(u8 *)((unsigned char *)(&D_800E6280) + (62)) != 0x5F) {
        if (*(u8 *)((unsigned char *)(&D_800E6280) + (62)) != 0x60) {
            var_v1 = 6;
            if (*(u8 *)((unsigned char *)(&D_800E6280) + (62)) != 0x61) {
                var_v1 = 6;
            }
        } else {
            var_v1 = 3;
        }
    } else {
        var_v1 = 0;
    }
    if (*(u8 *)((unsigned char *)(&D_800E6280) + (63)) != 2) {
        if ((*(u8 *)((unsigned char *)(&D_800E6280) + (63)) != 7) && (*(u8 *)((unsigned char *)(&D_800E6280) + (63)) == 0xC)) {
            var_v1 += 1;
        }
    } else {
        var_v1 -= 1;
    }
    temp_v0 = (var_v1 * 4) + &D_801506F8;
    sp2C = temp_v0;
    func_800AE0B0("補正 %d ", *temp_v0);
    temp_a0 = *sp2C;
    var_v0 = 0;
    if ((((func_800AE0D0() & 7) + temp_a0) - 8) > 0) {
        var_v0 = temp_a0;
    }
    return var_v0;
}
