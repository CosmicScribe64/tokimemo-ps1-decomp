/* Steps the scene dialogue: D_800CA134/138 point at the page and step halfwords (D_8013C358, D_8013C35C),
 * D_800CA13C/140/144 hold the text, kind and voice tables of the scene (func_80132D98). Returns 1 when a
 * message has finished. T-20930 */
s32 func_80132B04(s32 arg0, s32 arg1, s32 arg2) {
    if (D_80122D00 != 0) {
        if (check_end_k() != 0) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                if ((u32)++D_800E6280.unk_1104.w >= 0x3C) {
                    D_80122D00 = 0;
                    k_reset(1);
                    return 1;
                }
            }
        }
    } else if (D_80122D18 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && D_800E6280.unk_F80 == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122D18 = 0;
                return 1;
            }
        }
    } else if (D_80122CE4 != 0) {
        if (check_end_k() != 0 || (check_end_k() == 0 && D_800E6280.unk_F80 == 0)) {
            if (D_80122CF0 == 0 || (D_80122CF0 == 1 && func_80046094() == 9)) {
                D_80122CE4 = 0;
                k_reset(1);
                return 1;
            }
        }
    } else {
        D_800E6280.unk_1104.w = 0;
        func_80132420(((u8 ***)D_800CA13C)[*(s16 *)D_800CA134][*(s16 *)D_800CA138], ((u8 **)D_800CA140)[*(s16 *)D_800CA134][*(s16 *)D_800CA138], (u8)arg0, D_800CA144, *(s16 *)D_800CA134, *(s16 *)D_800CA138, (u8)arg2);
        if (D_8013C364 == 0) {
            if ((u8)arg1 != 0 && D_800CA2A8 == 0) {
                if (((u8 **)D_800CA140)[*(s16 *)D_800CA134][*(s16 *)D_800CA138] == 0xFF) {
                    *(s16 *)D_800CA134 += 1;
                    *(s16 *)D_800CA138 = 0;
                } else {
                    *(s16 *)D_800CA138 += 1;
                }
            }
        }
    }
    return 0;
}
