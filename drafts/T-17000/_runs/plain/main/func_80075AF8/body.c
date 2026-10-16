void func_80075AF8(void) {
    GameState *var_v0;
    s32 var_v1;
    u8 temp_t8;

    if (appointment_find_today((s32) D_800E62BE, (s32) D_800E62BF, (s32) D_800E62C0) <= 0) {
        icon_can_use_set(9, 0);
    }
    icon_can_use_set(0xD, 0);
    if (D_800E71EC == 2) {
        icon_can_use_set(0xE, 0);
    }
    if (D_800E62C1 == 6) {
        var_v1 = 0;
        var_v0 = &D_800E6280;
        do {
            temp_t8 = var_v0->unk_66C[0].unk_03;
            var_v0 += 4;
            if (temp_t8 & 1) {
                var_v1 += 1;
            }
        } while (var_v0 != &D_800E62AC);
        if (var_v1 >= 0xB) {
            icon_can_use_set(4, 0);
        }
    } else if (((u16) D_800E6374 >> 0xC) == 0xB) {
        icon_can_use_set(4, 0);
    }
}
