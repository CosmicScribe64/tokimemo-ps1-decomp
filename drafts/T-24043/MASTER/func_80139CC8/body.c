/* NON_MATCHING: T-24043, source-only draft; no verified default C bytes.
 * FAKE: scalar probability-byte view controls promotion as in MASTER T-10630. */

void func_80139CC8(void) {
    u8 selected;
    u8 temp_v0_2;
    s32 first;
    s32 value;
    s32 i;
    s32 count;
    u8 *a;
    u8 swap;
    u8 other;
    selected = D_800E6280.unk_110A;
    switch (selected) {
    case 0:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 7:
            dec_bg_cd_read(0x4183, 1);
            break;
        case 8:
            dec_bg_cd_read(0x419E, 1);
            break;
        case 9:
            dec_bg_cd_read(0x4195, 1);
            break;
        case 10:
            dec_bg_cd_read(0x418D, 1);
            break;
        default:
            func_80046500();
            break;
        }
        k_reset(0);
        func_8006612C((s32) "決勝戦");
        D_80125CA0 = 0xC0;
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                func_80046500();
                break;
            }
            func_8004284C();
        }
        break;
    case 2:
        normal_date_bg_fadein();
        break;
    case 3:
        D_800CA148 = 0xC;
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 7:
                func_80139054();
                break;
            case 8:
                func_801390FC();
                break;
            case 9:
                func_801390A8();
                break;
            case 10:
                func_80139000();
                break;
            default:
                func_80046500();
                break;
            }
            i = 0;
            do {
                load_palette(D_8013C770, (i + 0x10) & 0xFF, 1, 1, i);
                i += 1;
            } while (i != 5);
            func_8004284C();
        }
        break;
    case 5:
    case 11:
    case 13:
        normal_date_speak();
        break;
    case 6:
        value = (s16)(D_800E6280.unk_66C[(u16)D_800E6280.unk_0F4.h >> 12].unk_00 - 350);
        if (value < -30) (*(u8 *)&D_8013C784) = 0;
        else if (value < -20) (*(u8 *)&D_8013C784) = 1;
        else if (value < -10) (*(u8 *)&D_8013C784) = 1;
        else if (value < 0) (*(u8 *)&D_8013C784) = 2;
        else if (value >= 30) (*(u8 *)&D_8013C784) = 5;
        else if (value >= 20) (*(u8 *)&D_8013C784) = 4;
        else if (value >= 10) (*(u8 *)&D_8013C784) = 4;
        else if (value >= 0) (*(u8 *)&D_8013C784) = 3;
        func_8004284C();
        break;
    case 7:
        for (a = D_8013C788; a < D_8013C788 + 5; a++) *a = 0;
        i = 0;
        while (i < (*(u8 *)&D_8013C784)) { i++; D_8013C788[i - 1] = 1; }
        i = 0;
        count = func_800AE0D0() & 15;
        if ((*(u8 *)&D_8013C784) >= 3U) {
            swap = D_8013C788[0];
            other = D_8013C788[4];
            D_8013C788[4] = swap;
            D_8013C788[0] = other;
        }
        if (count > 0) {
            do {
                first = func_800AE0D0() & 3;
                value = func_800AE0D0() & 3;
                swap = D_8013C788[first];
                i++;
                other = D_8013C788[value];
                D_8013C788[first] = other;
                D_8013C788[value] = swap;
            } while (i != count);
        }
        if (((u16)D_800E6280.unk_0F4.h >> 12) == 9) {
            if ((*(u8 *)&D_8013C784) >= 3U) D_8013C788[0] = 1;
            else D_8013C788[0] = 0;
        }
        func_8004284C();
        break;
    case 8:
        func_80048F64(0x60);
        ((DisplayFixActor44 *)D_80120650)->unk_01 = 3;
        ((DisplayFixActor44 *)D_80120650)->unk_38 = 0x01000000;
        ((DisplayFixActor44 *)D_80120650)->unk_05 = 0x80;
        ((DisplayFixActor44 *)D_80120650)->unk_1A = 0x1000;
        ((DisplayFixActor44 *)D_80120650)->unk_1C = 0x1000;
        ((DisplayFixActor44 *)D_80120650)->unk_06 = 1;
        ((DisplayFixActor44 *)D_80120650)->unk_07 = 0x80;
        ((DisplayFixActor44 *)D_80120650)->unk_43 = (func_800AE0D0() % 5) + 0x10;
        ((DisplayFixActor44 *)D_80120650)->unk_24.h[1] = -0x60;
        ((DisplayFixActor44 *)D_80120650)->unk_28.h[1] = 0x20;
        ((DisplayFixActor44 *)D_80120650)->unk_0C = (u8 *) D_8013C764;
        ((DisplayFixActor44 *)D_80120650)->unk_10 = (u8 *) D_8013C768;
        ((DisplayFixActor44 *)D_80120650)->unk_34 = (u8 *) D_8013C760;
        ((DisplayFixActor44 *)D_80120650)->unk_14 = D_8013C76C;
        D_8013C780 = 0;
        func_8004284C();
        break;
    case 9:
        if (((DisplayFixActor44 *)D_80120650)->unk_02 == 0) {
            if ((u8) D_8013C780 < 5U) {
                if (D_8013C780 != 0) {
                    if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9) {
                        goto block_81;
                    }
                    goto block_83;
                }
block_83:
                ((DisplayFixActor44 *)D_80120650)->unk_03 = 0x84;
                ((DisplayFixActor44 *)D_80120650)->unk_02 = 5;
                ((DisplayFixActor44 *)D_80120650)->unk_18 = 0;
                ((DisplayFixActor44 *)D_80120650)->unk_08 = 0;
                if (D_8013C788[D_8013C780] == 1) {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 0;
                } else {
                    ((DisplayFixActor44 *)D_80120650)->unk_16 = 1;
                }
                D_8013C780 += 1;
            } else {
block_81:
                func_8004284C();
            }
        }
        break;
    case 10:
        if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
            D_800CA148 = 0xD;
            func_80044750(0x204);
        } else {
            D_800CA148 = D_8013C794 + 0x10;
            func_80044750(0x205);
        }
        func_8004284C();
        break;
    case 12:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 7:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x66;
            }
            break;
        case 8:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x18;
            } else {
                D_800CA148 = 0x67;
            }
            break;
        case 9:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x19;
            } else {
                D_800CA148 = 0x68;
            }
            break;
        case 10:
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800CA148 = 0x1A;
            } else {
                D_800CA148 = 0x69;
            }
            break;
        default:
            func_80046500();
            break;
        }
        func_8004284C();
        break;
    case 14:
        temp_v0_2 = ((DisplayFixActor44 *)D_80120650)->unk_07;
        if (temp_v0_2 < 4U) {
            ((DisplayFixActor44 *)D_80120650)->unk_07 = 0;
        } else {
            ((DisplayFixActor44 *)D_80120650)->unk_07 = temp_v0_2 - 4;
        }
        normal_date_bg_fadeout();
        break;
    case 15:
        func_80048F64(0x60);
        if (D_8013C790 == 0xFF) {
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + 0x19;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x19;
                }
            } else {
                D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + 0x12;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x12;
                }
            }
            func_80042908(6);
        } else {
            k_reset(0);
            if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 7) {
                func_8006612C((s32) "国立競技場");
            } else {
                func_8006612C((s32) "選手控え室");
            }
            D_80125CA0 = 0xC0;
            D_800E6280.unk_1BC[D_8013C790].unk_06 += 2;
            if ((u8) (*(u8 *)&D_8013C784) >= 3U) {
                D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + 0x19;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x19;
                }
                func_80042808();
            } else {
                D_800E6280.unk_1BC[0].unk_02 = D_800E6280.unk_1BC[0].unk_02 + 0x12;
                for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 0x12;
                }
                func_80042908(5);
            }
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
