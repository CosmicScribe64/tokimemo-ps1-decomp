#include "common.h"
#include "game.h"
#include "ovl/BUNKA_SD.h"

s32 func_801388D0(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_800460EC() == 2) {
            D_800E6280.unk_110D += 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else {
        func_80048390();
        func_80048E78();
        func_8006492C(1);
        func_80042908(0x10);
        func_80044750(0x200);
    }
}


s32 func_801389DC(void) {
    if (D_800E6280.unk_110D == 0) {
        cd_read_sectors(0x56, 0x80180000, 0x4F1D);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 1 && (cd_read_sync_status() & 1)) {
        switch (D_800E6280.unk_03E) {
        case 0x5F:
            func_80046290(0x3F, 0x0500039C, 0xF);
            break;
        case 0x60:
            func_80046290(0x0500005B, 0x05000450, 0xF);
            break;
        default:
        case 0x61:
            func_80046290(0x02000062, 0x080003F6, 0xF);
            break;
        }
        func_80044750(0x300);
        func_8004284C();
    }
}

void func_80138ADC(void) {
    if ((u32)D_800E6280.unk_110A >= 3 && (u32)D_800E6280.unk_110A < 6 && (D_800E6280.unk_F80 & 0x40)) {
        func_80042940(5);
        func_80044750(0xC4);
    }
    func_80138B3C();
}

void func_80138B3C(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80134260();
        func_80044750(0x7F);
        return;
    case 1:
        func_801389DC();
        return;
    case 2:
        func_80138C34();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80138CAC();
        return;
    case 5:
        func_8004E9F4(1);
        func_8004284C();
        return;
    case 6:
        func_8013987C();
        return;
    case 7:
        func_801388D0();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80138C34(void) {
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_1100++ >= 1U) {
            func_80138D48();
            func_8004284C();
            func_80045414(9, 0, 0);
        }
    } else {
        D_800E6280.unk_1100 = 0;
    }
}

void func_80138CAC(void) {
    GameState *g;

    func_80139100();
    func_80139290();
    g = &D_800E6280;
    g->unk_1104.u += 1;
    if ((g->unk_1104.u + 0x90000000U) == 0xFA && g->unk_03E != 0x60) {
        func_8004284C();
    }
    if ((g->unk_1104.u + 0x90000000U) == 0xD0 && g->unk_03E == 0x60) {
        func_8004284C();
    }
}

void func_80138D48(void) {
    s32 frame;
    s32 i;
    DisplayActor44 *record;

    i = 0;
    do {
        load_palette(D_8013D9CC, (i + 0x10) & 0xFF, 1, 1, i);
        i += 1;
    } while (i < 0x10);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_80120650)[0].unk_01 = 5;
    ((DisplayActor44 *)D_80120650)[0].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[0].unk_03 = 0x80;
    ((DisplayActor44 *)D_80120650)[0].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[0].unk_0C = D_8013D9C4;
    ((DisplayActor44 *)D_80120650)[0].unk_10 = D_8013D9C8;
    ((DisplayActor44 *)D_80120650)[0].unk_34 = D_8013D9C0;
    ((DisplayActor44 *)D_80120650)[0].unk_18 = 0x2B;
    ((DisplayActor44 *)D_80120650)[0].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[0].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[0].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[0].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_04 = 0;
    func_80048F64(0x61);
    ((DisplayActor44 *)D_80120650)[1].unk_01 = 4;
    ((DisplayActor44 *)D_80120650)[1].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[1].unk_03 = 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[1].unk_0C = D_8013D9C4;
    ((DisplayActor44 *)D_80120650)[1].unk_10 = D_8013D9C8;
    ((DisplayActor44 *)D_80120650)[1].unk_34 = D_8013D9C0;
    ((DisplayActor44 *)D_80120650)[1].unk_18 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[1].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[1].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_04 = 0;
    record = (DisplayActor44 *)D_8011ED58;
    i = 2;
    frame = 0xF;
    for (; i != 6; i++) {
        func_80048F64(i + 0x60);
        record[96].unk_01 = 4;
        record[96].unk_38 = 0x01000000;
        record[96].unk_03 = 0x80;
        record[96].unk_02 = 0x40;
        record[96].unk_0C = D_8013D9C4;
        record[96].unk_10 = D_8013D9C8;
        record[96].unk_34 = D_8013D9C0;
        record[96].unk_18 = frame;
        frame += 4;
        record++;
        record[95].unk_06 = 1;
        record[95].unk_07 = 0;
        record[95].unk_26 = -0xA0;
        record[95].unk_2A = -0x78;
        record[95].unk_05 = 0x80;
        record[95].unk_43 = 0;
        record[95].unk_04 = 0;
    }
    if (D_80122EAC & 1) {
        ((DisplayActor44 *)D_80120650)[3].unk_18 = 0x1F;
    }
    if (D_80122EAC & 2) {
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0x23;
    }
    if (D_80122EAC & 4) {
        ((DisplayActor44 *)D_80120650)[5].unk_18 = 0x27;
    }
    load_tpage_buf_lock(5, ((s32 *)D_8013D9C0)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_8013D9C0)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_8013D9C0)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_8013D9C0)[3], 1);
    load_tpage_buf_lock(9, ((s32 *)D_8013D9C0)[4], 1);
    load_tpage_buf_lock(0xA, ((s32 *)D_8013D9C0)[5], 1);
    load_tpage_buf_lock(0x25, ((s32 *)D_8013D9C0)[6], 1);
    load_tpage_buf_lock(0x26, ((s32 *)D_8013D9C0)[7], 1);
    load_tpage_buf_lock(0x27, ((s32 *)D_8013D9C0)[8], 1);
    load_tpage_buf_lock(0x28, ((s32 *)D_8013D9C0)[9], 1);
}


s32 func_80139100(void) {
    if (D_800E6280.unk_1104.u == 0xBE) {
        D_800E6280.unk_1100 = func_80046274() - 0x1000;
    }
    if (D_800E6280.unk_1104.u == 0xBC) {
        func_80044750(0x300);
    }
    func_8004B19C(func_80046274(), 0, 0);
    func_8004B19C(D_800E6280.unk_1100, 0, 0x10);
    func_8004B19C(func_80046274() - D_800E6280.unk_1100, 0, 0x20);
    if (D_800E6280.unk_1104.u >= 0xC0U) {
        if (D_800E6280.unk_1104.u < 0x70000000U) {
            switch (D_800E6280.unk_03E) {
            case 0x5F:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2609U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x60:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x2AF1U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            case 0x61:
            default:
                if ((u32)(func_80046274() - D_800E6280.unk_1100) >= 0x27E9U) {
                    D_800E6280.unk_1104.u = 0x70000000;
                }
                break;
            }
        }
    }
}

void func_80139290(void) {
    s16 var_t5;
    s32 temp_v1;
    u32 temp_lo;
    u32 temp_lo_2;
    u8 var_t0;

    switch (D_800E6280.unk_1104.w) {
    case 0x1D:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 1;
        break;
    case 0x58:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
        break;
    case 0x66:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 2;
        break;
    case 0x83:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
        break;
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xBFU) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo = (u32) ((u32) (D_800E6280.unk_1104.w - 0xBE) % 242U) / 11U;
        switch (temp_lo) {
        case 0:
            var_t5 = 4;
block_35:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = var_t5;
            break;
        case 1:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 2:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 5;
            break;
        case 3:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 6;
            break;
        case 4:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 5:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xC;
            break;
        case 6:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xB;
            break;
        case 7:
            var_t5 = 0xE;
            goto block_35;
        case 8:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 9:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 8;
            break;
        case 10:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 7;
            break;
        case 11:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 9;
            break;
        case 12:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 13:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 5;
            break;
        case 14:
            var_t5 = 6;
            goto block_35;
        case 15:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 16:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xD;
            break;
        case 17:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xB;
            break;
        case 18:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 4;
            break;
        case 19:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
            break;
        case 20:
            ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0xA;
            break;
        case 21:
            var_t5 = 7;
            goto block_35;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 3;
    }
    temp_v1 = D_800E6280.unk_1104.w - 0x70000000;
    switch (temp_v1) {
    default:
        var_t0 = D_800E6280.unk_03E;
        break;
    case 0x3B:
        var_t0 = D_800E6280.unk_03E;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 2;
        break;
    case 0x49:
        var_t0 = D_800E6280.unk_03E;
block_47:
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 0;
        break;
    case 0x66:
        var_t0 = D_800E6280.unk_03E;
        ((DisplayActor44 *)D_8011ECD0)[97].unk_18 = 1;
        break;
    case 0xA1:
        var_t0 = D_800E6280.unk0;
        if (var_t0 == 0x60) {
            goto block_47;
        }
        break;
    }
    if (D_800E6280.unk_1104.w == 0x95) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1D;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
        }
    }
    if (((u32) D_800E6280.unk_1104.w >= 0xD1U) && ((u32) D_800E6280.unk_1104.w < 0x70000000U)) {
        temp_lo_2 = (u32) ((u32) (D_800E6280.unk_1104.w - 0xC9) % 78U) / 39U;
        switch (temp_lo_2) {
        case 0:
            ((DisplayActor44 *)D_8011ECD0)->unk0 = 0x11;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x15;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x19;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1D;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x21;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x25;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x29;
            }
            break;
        case 1:
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1C;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
            }
            break;
        }
    }
    if (D_800E6280.unk_1104.w == 0x70000000) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x10;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x14;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x18;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1C;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x20;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x24;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x28;
        }
    }
    if (temp_v1 == 0x3B) {
        if (var_t0 != 0x60) {
            ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0xF;
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x13;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x17;
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1B;
            if (D_80122EAC & 1) {
                ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x1F;
            }
            if (D_80122EAC & 2) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x23;
            }
            if (D_80122EAC & 4) {
                ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x27;
            }
        }
    }
    if ((temp_v1 == 0x68) && (var_t0 == 0x60)) {
        ((DisplayActor44 *)D_8011ECD0)[98].unk_18 = 0x12;
        ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x16;
        ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x1A;
        ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x1E;
        if (D_80122EAC & 1) {
            ((DisplayActor44 *)D_8011ECD0)[99].unk_18 = 0x22;
        }
        if (D_80122EAC & 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x26;
        }
        if (D_80122EAC & 4) {
            ((DisplayActor44 *)D_8011ECD0)[101].unk_18 = 0x2A;
        }
    }
}

/* NON_MATCHING: T-12090, 14 differing listing rows; pointer setup order and retained byte-result copy versus clear zero */
#ifdef NON_MATCHING
void func_8013987C(void) {
    u8 v;
    u8 *p;
    s32 i;

    v = D_800E6280.unk_110B;
    p = D_80120650;
    do {
        p[7] = v;
        p += 0x44;
    } while (p < D_80121750);
    if (v == 0) {
        v = 2;
    }
    v -= 1;
    D_800E6280.unk_110B = v;
    if (v == 0) {
        for (i = 0; i < 0x40; i++) {
            D_80120650[i * 0x44 + 7] = v;
        }
        *(u8 *)&D_800E6280.unk_110B = v;
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/801388D0", func_8013987C);
#endif
