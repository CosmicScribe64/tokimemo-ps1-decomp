void func_80075AF8(void) {
    GameState *var_v0;
    s32 var_v1;
    u8 temp_t8;

    if (appointment_find_today((s32) D_800E6280.unk_03E, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) <= 0) {
        icon_can_use_set(9, 0);
    }
    icon_can_use_set(0xD, 0);
    if ((u8) D_800E6280.unk_F6C == 2) {
        icon_can_use_set(0xE, 0);
    }
    if (D_800E6280.unk_041 == 6) {
        var_v1 = 0;
        var_v0 = &D_800E6280;
        do {
            temp_t8 = var_v0->unk_66C[0].unk_03;
            var_v0 += 4;
            if (temp_t8 & 1) {
                var_v1 += 1;
            }
        } while (var_v0 != &D_800E6280.unk_01C[1].unk_04);
        if (var_v1 >= 0xB) {
            icon_can_use_set(4, 0);
        }
    } else if (((u16) D_800E6280.unk_000 >> 0xC) == 0xB) {
        icon_can_use_set(4, 0);
    }
}
