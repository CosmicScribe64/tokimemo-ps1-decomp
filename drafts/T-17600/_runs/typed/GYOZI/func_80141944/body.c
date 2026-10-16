s16 func_80141944(void) {
    GzCallbacks68 sp2C;
    s16 var_v0;
    s32 var_at;
    s32 var_at_2;
    u8 temp_v1;

    sp2C = D_80148880;
    var_v0 = sp2C.callbacks[D_800F647A](0x80);
    temp_v1 = D_800F647A;
    if (temp_v1 < 8U) {
        ((DisplayActor44 *)D_80129F40)[96].unk_07 = (u8) D_801317EB;
        var_at = temp_v1 < 0xAU;
        if (((DisplayActor44 *)D_80129F40)[96].unk_18 == 0) {
            var_at = temp_v1 < 0xAU;
            if (((DisplayActor44 *)D_80129F40)[96].unk_08 == 0) {
                var_v0 = ((DisplayActor44 *)D_80129F40)[96].unk_16 + 1;
                ((DisplayActor44 *)D_80129F40)[96].unk_16 = var_v0;
                if (var_v0 >= 3) {
                    ((DisplayActor44 *)D_80129F40)[96].unk_16 = 0;
                    var_v0 = 0;
                }
                goto block_5;
            }
        }
    } else {
block_5:
        var_at = temp_v1 < 0xAU;
    }
    if (var_at == 0) {
        var_at_2 = temp_v1 < 0xAU;
        if (D_80148820 != 0) {
            var_v0 = func_801428EC();
            goto block_9;
        }
    } else {
block_9:
        var_at_2 = temp_v1 < 0xAU;
    }
    if (var_at_2 == 0) {
        if (D_80148824 != 0) {
            var_v0 = func_80142C14();
        }
    }
    return var_v0;
}
