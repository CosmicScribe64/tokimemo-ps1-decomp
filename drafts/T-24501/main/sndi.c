/* T-24501 source-recovery checkpoint, not a production match. */
void sndi(u8 *arg0, s32 arg1, s32 arg2) {
    u8 parity;
    u8 original[64];
    /* FAKE: capacities below64 inferred from adjacent target buffer homes,
     * not original declarations; diagnostic only. T-24501 */
    u8 quoted[40];
    u8 stage[40];
    u8 result[40];
    u8 name[20];
    u8 *p;
    s32 i;
    s32 found;

    strncpy(original, arg0, 64);
    D_800E6280.unk_F62 = 0;
    found = 0;
    if ((arg2 & 15) == 15) {
        i = 0;
        while (original[i] != 0) {
            /* FAKE: distinct integer address class prevents cursor IV.
             * Requires 32-bit target pointer representation. T-24501 */
            p = (u8 *)((u32)original + (u32)i);
            parity = i & 1;
            i++;
            if (*p == 'P' && parity == 0) {
                *p = 0;
                found = 1;
                strcpy(stage, original);
                get_p_name(name, arg2 & 15);
                strcat(stage, name);
                i = 0;
                strcat(stage, p + 1);
                break;
            }
        }
        if (found == 0) {
            i = 0;
            strcpy(stage, original);
        }
        found = 0;
        while (stage[i] != 0) {
            p = (u8 *)((u32)stage + (u32)i);
            parity = i & 1;
            i++;
            if (*p == 'G' && parity == 0) {
                *p = 0;
                found = 1;
                strcpy(result, stage);
                get_g_name(name, D_800E6280.unk_F5F);
                strcat(result, name);
                strcat(result, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(result, stage);
        }
    } else {
        i = 0;
        while (original[i] != 0) {
            p = (u8 *)((u32)original + (u32)i);
            parity = i & 1;
            i++;
            if (*p == 'P' && parity == 0) {
                *p = 0;
                found = 1;
                strcpy(result, original);
                get_p_name(name, D_800E6280.unk_F5F);
                strcat(result, name);
                strcat(result, p + 1);
                break;
            }
        }
        if (found == 0) {
            strcpy(result, original);
        }
    }
    switch (arg1) {
    case 0:
        if (arg2 == 0xF) {
            strcpy(quoted, "『");
            strcat(quoted, result);
        } else if (arg2 == 0x1F) {
            strcpy(quoted, "（");
            strcat(quoted, result);
        } else if (arg2 == 0x2F) {
            strcpy(quoted, result);
        } else {
            strcpy(quoted, "「");
            strcat(quoted, result);
        }
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x32, 0U, quoted, 0);
            return;
        }
        set_kanji_string(-0x56, 0x32, 0U, quoted, 0);
        return;
    case 1:
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x42, 0U, result, 0);
            return;
        }
        set_kanji_string(-0x48, 0x42, 0U, result, 0);
        return;
    default:
    case 2:
        if (arg2 == 0xFF) {
            set_kanji_string(-0x80, 0x52, 0U, result, 0);
            return;
        }
        set_kanji_string(-0x48, 0x52, 0U, result, 0);
        return;
    }
}
