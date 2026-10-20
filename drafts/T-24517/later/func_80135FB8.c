/* FAKE: scalar probability-byte views suppress a promoted live range,
 * retained from the matched twin func_80133F80 (T-10630). T-24514 */

void func_80135FB8(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;

    switch (D_800E6280.unk_110A) {
    case 0:
        value = (s16)(((s16) D_800E6280.unk_118.unk_02 >> 4) - (0x140 - D_800E6280.unk_66C[6].unk_00));
        if (value < -0x1E) {
            (*(u8 *)&D_8013C578) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_8013C578) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_8013C578) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_8013C578) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_8013C578) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_8013C578) = 3;
        }
        for (i = 0; i < 5; i++) {
            D_8013C57C[i] = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_8013C578); i++) {
            D_8013C57C[i] = 1;
        }
        i = 0;
        swap_count = func_800AE0D0() & 0xF;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            swap = D_8013C57C[0];
            other = D_8013C57C[4];
            D_8013C57C[0] = other;
            D_8013C57C[4] = swap;
        }
        a = D_8013C57C;
        if (swap_count > 0) {
            do {
                first = func_800AE0D0() & 3;
                value = func_800AE0D0() & 3;
                swap = a[first];
                i++;
                other = a[value];
                a[first] = other;
                a[value] = swap;
            } while (i != swap_count);
        }
        ((DisplayFixActor44 *)D_80120650)->unk_43 = 0x12;
        D_8013C574 = 0;
        func_8004284C();
        break;
    case 1:
        if (((DisplayFixActor44 *)D_80120650)->unk_02 == 0) {
            if (D_8013C574 >= 5) {
                func_8004284C();
            } else {
                ((DisplayFixActor44 *)D_80120650)->unk_03 = 0x84;
                ((DisplayFixActor44 *)D_80120650)->unk_02 = 5;
                ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
                ((DisplayFixActor44 *)D_80120650)->unk_08 = 0;
                if (D_8013C57C[D_8013C574] == 1) {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 0;
                } else {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 1;
                }
                D_8013C574 += 1;
            }
        }
        break;
    case 2:
        ((DisplayFixActor44 *)D_80120650)->unk_03 = 0x84;
        ((DisplayFixActor44 *)D_80120650)->unk_02 = 1;
        ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
        ((DisplayFixActor44 *)D_80120650)->unk_08 = 0;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            func_80044750(0x204);
            ((DisplayFixActor44 *)D_80120650)->unk_16 = 2;
            D_800CA148 = 0xE;
        } else {
            func_80044750(0x205);
            ((DisplayFixActor44 *)D_80120650)->unk_16 = 3;
            D_800CA148 = 0xF;
        }
        func_8004284C();
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            D_800CA148 = 0x16;
        } else {
            D_800CA148 = 0x1B;
        }
        func_8004284C();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        normal_date_bg_fadeout();
        ((DisplayFixActor44 *)D_80120650)->unk_07 -= 4;
        if (((DisplayFixActor44 *)D_80120650)->unk_07 < 4U) {
            ((DisplayFixActor44 *)D_80120650)->unk_07 = 4;
        }
        break;
    case 7:
        ((DisplayFixActor44 *)D_80120650)->unk_03 = 0;
        ((DisplayFixActor44 *)D_80120650)->unk_02 = 0;
        func_80042808();
        if (((ScheduleClubFlags *)&D_800E6280.unk_66C[6] )->unk_25 < 3U) {
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
