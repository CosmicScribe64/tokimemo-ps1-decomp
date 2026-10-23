#include "common.h"
#include "game.h"

void func_80053650(s32 arg0, s32 arg1, s32 arg2) {
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    D_80125C08 = arg0;
    D_80125C0C = arg1;
    D_80125C04 = arg2;
}

s32 func_800536AC(arg0, arg1, arg2, arg3)
u16 arg0;
u16 arg1;
u16 arg2;
u16 arg3;
{
    RECT rect;
    s32 pad[2]; /* FAKE: unused 8-byte local reproduces the original frame (0x58); real source unknown. T-10320 */
    u32 end;
    s32 v;

    if ((*(u32 *)D_80125C04 & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        return -1;
    }
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_80086870(D_80125C04, D_80125C08);
    func_800868BC(D_80125C08, 2);
    rect.y = arg1;
    rect.w = 0x10;
    rect.x = arg0;
    rect.h = arg3;
    end = arg0 + arg2;
    if (rect.x < end) {
        v = ((arg3 + 8) >> 4) << 7;
        do {
            func_80086944(D_80125C0C, v);
            func_80086984(0);
            func_8009C884(&rect, D_80125C0C);
            rect.x += 0x10;
        } while (rect.x < end);
    }
    return 1;
}

void card_ev_set(void) {
    Sw_Start();
    Hw_Start();
    InitCARD(1);
    StartCARD();
    _bu_init();
    _card_auto(0);
}

void Sw_Start(void) {
    EnterCriticalSection();
    D_8011ECAC = OpenEvent(0xF4000001, 4, 0x2000, 0);
    D_8011ECB0 = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    D_8011ECB4 = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    D_8011ECB8 = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    EnableEvent(D_8011ECAC);
    EnableEvent(D_8011ECB0);
    EnableEvent(D_8011ECB4);
    EnableEvent(D_8011ECB8);
    ExitCriticalSection();
}

void Hw_Start(void) {
    EnterCriticalSection();
    D_8011ECBC = OpenEvent(0xF0000011, 4, 0x2000, 0);
    D_8011ECC0 = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    D_8011ECC4 = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    D_8011ECC8 = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    EnableEvent(D_8011ECBC);
    EnableEvent(D_8011ECC0);
    EnableEvent(D_8011ECC4);
    EnableEvent(D_8011ECC8);
    ExitCriticalSection();
}

u8 Sw_Test(void) {
    D_800E6280.unk_1115 = 0;
    if (TestEvent(D_8011ECAC) != 0) {
        D_800E6280.unk_1115 = 1;
    } else if (TestEvent(D_8011ECB0) != 0) {
        D_800E6280.unk_1115 = 2;
    } else if (TestEvent(D_8011ECB4) != 0) {
        D_800E6280.unk_1115 = 4;
    } else if (TestEvent(D_8011ECB8) != 0) {
        D_800E6280.unk_1115 = 8;
    }
    if (D_800E6280.unk_1115 != 0) {
        Sw_Clear();
    }
    return D_800E6280.unk_1115;
}

void Sw_Clear(void) {
    TestEvent(D_8011ECAC);
    TestEvent(D_8011ECB0);
    TestEvent(D_8011ECB4);
    TestEvent(D_8011ECB8);
}

u8 Hw_Test(void) {
    D_800E6280.unk_1115 = 0;
    do {
        if (TestEvent(D_8011ECBC) != 0) {
            D_800E6280.unk_1115 = 0x81;
        } else if (TestEvent(D_8011ECC0) != 0) {
            D_800E6280.unk_1115 = 0x82;
        } else if (TestEvent(D_8011ECC4) != 0) {
            D_800E6280.unk_1115 = 0x84;
        } else if (TestEvent(D_8011ECC8) != 0) {
            D_800E6280.unk_1115 = 0x88;
        }
    } while (D_800E6280.unk_1115 == 0);
    Hw_Clear();
    return D_800E6280.unk_1115;
}

void Hw_Clear(void) {
    TestEvent(D_8011ECBC);
    TestEvent(D_8011ECC0);
    TestEvent(D_8011ECC4);
    TestEvent(D_8011ECC8);
}

void Hw_Stop(void) {
    EnterCriticalSection();
    CloseEvent(D_8011ECBC);
    CloseEvent(D_8011ECC0);
    CloseEvent(D_8011ECC4);
    CloseEvent(D_8011ECC8);
    ExitCriticalSection();
}

void func_80053CAC(u8 arg0) {
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111C = arg0;
}

void func_80053CC0(void) {
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111C += 1;
}

void func_80053CE0(void) {
    D_800E6280.unk_035 = 1;
    D_800E6280.unk_111C = 0;
    D_800E6280.unk_1115 = 0;
    D_800E6280.unk_111D = 0;
    D_800E6280.unk_1120 = 0;
}

void func_80053D10(void) {
    D_800E6280.unk_035 = 0;
    D_800E6280.unk_1120 = 0;
}

void func_80053D24(u8 arg0) {
    switch (Sw_Test()) {
    case 1:
        func_80053CC0();
        return;
    case 8:
        func_80053CAC(arg0);
        return;
    case 4:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        if ((u32)D_800E6280.unk_111D >= 3) {
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xFF;
            return;
        }
    case 0:
        return;
    default:
        func_80053CAC(0);
        break;
    }
}

s32 func_80053DDC(void) {
    s32 status;

    status = _card_status(0);
    while (status == 0) {
        status = _card_status(0);
    }
    switch (status) {
    case 1:
        close(D_800B58E4);
        break;
    case 2:
        close(D_800B58E4);
        break;
    case 4:
        close(D_800B58E4);
        break;
    case 8:
        close(D_800B58E4);
        break;
    case 0x11:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        close(D_800B58E4);
        break;
    case 0x21:
        D_800E6280.unk_111D += 1;
        func_80053CAC(0);
        close(D_800B58E4);
        break;
    }
    return status;
}

s32 func_80053F04(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        /* FAKE: byte view of the u8 field avoids the redundant selector copy (move v0,v1), as in
         * TEL (T-10630). T-17000 */
        switch (*(u8 *)&D_800E6280.unk_111C) {
        case 0x0:
            D_800E6280.unk_1115 = 0;
            Sw_Clear();
            if (_card_info(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x1:
            func_80053D24(0x20);
            break;
        case 0x2:
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x3:
            func_80053D24(0x40);
            break;
        case 0x4:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 1;
            break;
        case 0x20:
            func_800AD950(0);
            Hw_Clear();
            if (_card_clear(0) == 0) {
                func_80053CAC(0);
            } else {
                Hw_Test();
                switch (D_800E6280.unk_1115) {
                default:
                case 0x0:
                case 0x81:
                    func_80053CAC(0);
                    break;
                case 0x88:
                    func_80053CAC(0x30);
                    break;
                }
            }
            break;
        case 0x30:
            if (_card_load(0) == 1) {
                func_80053CC0();
            }
            break;
        case 0x31:
            func_80053D24(0x40);
            break;
        case 0x32:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 1;
            break;
        case 0x40:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF1;
            break;
        }
    }
    return (s32) D_800E6280.unk_1115;
}

s32 func_80054108(void) {
    if (format((u8 *)"bu00:") == 1) {
        return 1;
    }
    return 0;
}

s32 func_80054140(void) {
    GameState *g = &D_800E6280;

    g->unk_1120 += 1;
    if (g->unk_035 != 0) {
        if (g->unk_111C == 0) {
            g->unk_1115 = 0;
            g->unk_1118 = g->unk_111C;
            if (func_80054AF4(g->unk_111C) == 1) {
                D_800E8BEE = 1;
                func_80053CC0();
            } else {
                g->unk_111D += 1;
                if (g->unk_111D >= 3U) {
                    D_800E8BEE = 0;
                    func_80053CC0();
                }
            }
        } else if (g->unk_111C < 0xFU) {
            g->unk_1118 = g->unk_111C;
            if (func_80054AF4(g->unk_1118) == 1) {
                D_800E7D14[g->unk_1118] = 1;
            } else {
                D_800E7D14[g->unk_1118] = 0;
            }
            g->unk_111C += 1;
        } else {
            g->unk_035 = 0;
            g->unk_1115 = 2;
        }
    }
    return (s32) g->unk_1115;
}

u8 func_80054284(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            D_800E6280.unk_1118 = 0;
            if (func_8005478C(0) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 3;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

s32 func_80054388(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            D_800E6280.unk_1118 = 0;
            if (func_80054884(0) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 0x13;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return (s32) D_800E6280.unk_1115;
}

u8 func_8005448C(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            if (func_8005478C(D_800E6280.unk_1118) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 4;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

u8 func_80054590(void) {
    D_800E6280.unk_1120 += 1;
    if (D_800E6280.unk_035 != 0) {
        switch (D_800E6280.unk_111C) {
        case 0:
            Sw_Clear();
            if (func_80054884(D_800E6280.unk_1118) == 1) {
                func_80053CC0();
            } else {
                D_800E6280.unk_1115 = 0xD0;
                D_800E6280.unk_035 = 0;
            }
            break;
        case 1:
            if (func_80053DDC() == 1) {
                func_80053CC0();
            }
            break;
        case 2:
            D_800E6280.unk_1115 = 0x14;
            D_800E6280.unk_035 = 0;
            break;
        case 16:
            D_800E6280.unk_035 = 0;
            D_800E6280.unk_1115 = 0xF0;
            break;
        }
    }
    return D_800E6280.unk_1115;
}

s32 func_80054694(s32 arg0) {
    FileReq req;
    s32 fd;

    func_80056070(req.name, arg0);
    close(D_800B58E4);
    fd = open(req.name, 0x10200);
    D_800B58E4 = fd;
    if (fd == -1) {
        return -1;
    }
    close(D_800B58E4);
    return 1;
}

s32 func_80054704(s32 arg0) {
    /* FAKE: retain the observed final stack counter write (T-9510). */
    volatile s32 counter;
    u8 name[32];
    s32 retry;

    func_80056070(name, arg0);
    if (delete(name) != 1) {
        /* Original code reads this counter before initialization. */
        retry = counter;
        do {
            retry++;
            if (retry >= 3) {
                return -1;
            }
        } while (delete(name) != 1);
        counter = retry;
    }
    return 1;
}

s32 func_8005478C(s32 arg0) {
    /* FAKE: unused padding reproduces the 0x78-byte frame and filename
     * buffer at SP+0x4C; retry control flow matches naturally. T-9510. */
    s32 retry;
    s32 pad_before[2];
    u8 name[32];
    s32 pad_after[4];

    retry = 0;
    func_80054C4C();
    func_80056070(name, arg0);
    close(D_800B58E4);
    D_800B58E4 = open(name, 0x8001);
    if (D_800B58E4 == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
        } while ((D_800B58E4 = open(name, 0x8001)) == -1);
        retry = 0;
    }
    if (read(D_800B58E4, D_80123120, 0x2000) == -1) {
        do {
            retry++;
            if (retry >= 16) {
                return -1;
            }
        } while (read(D_800B58E4, D_80123120, 0x2000) == -1);
    }
    return 1;
}

s32 func_80054884(s32 arg0) {
    /* FAKE: pad1/pad2 reproduce the 0x70-byte frame (sp47/sp46 at SP+0x47/0x46,
     * idx spilled at SP+0x50); real locals unknown. T-24214. */
    u8 name[28];
    s32 idx;
    s32 pad1[2];
    u8 sp47;
    u8 sp46;
    s32 pad2[1];
    s32 retry;

    retry = 0;
    func_80054C4C();
    func_80056070(name, arg0);
    close(D_800B58E4);
    D_800B58E4 = open(name, 0x8002);
    if (D_800B58E4 == -1) {
        do {
            retry++;
            if (func_80054694(arg0) == -1) {
                return -1;
            }
            if (retry >= 16) {
                return -1;
            }
        } while ((D_800B58E4 = open(name, 0x8002)) == -1);
        retry = 0;
    }
    strncpy(D_80123120, "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(D_80123124, 0x40);
    strncpy(D_80123124, "ときめきメモリアル", 0x14);
    if (arg0 != 0) {
        strncat(D_80123124, D_800B32CC[arg0 - 1], 6);
    } else {
        strncat(D_80123124, "・システム", 0xC);
    }
    bzero(D_80123164, 0x1C);
    func_8006C988(&sp47, &sp46, arg0);
    /* FAKE: T-24214, the no-op mask on the u8 takes one ugen temporary, so
     * the later temporaries (t2..t9) land on the original's registers. */
    idx = sp47 & 0xFF;
    if (idx >= 0xD) {
        idx = 0;
    }
    memcpy(D_80123180, &D_800B4390[idx << 5], 0x20);
    memcpy(D_801231A0, (&D_800B58B0)[idx], 0x180);
    if (arg0 != 0) {
        func_8005557C(0);
    } else {
        func_80054FC0(0);
    }
    D_80123320[1] = D_80123320[0];
    while (write(D_800B58E4, D_80123120, 0x2000) == -1) {
        retry++;
        if (retry >= 16) {
            return -1;
        }
    }
    return 1;
}

s32 func_80054AF4(s32 arg0) {
    u8 name[0x1C];
    s32 max;
    s32 i;
    s32 fd;

    if (arg0 != 0) {
        max = 8;
    } else {
        max = 0x10;
    }
    func_80056070(name, arg0);
    i = 0;
    fd = open(name, 2);
    D_800B58E4 = fd;
    while (fd == -1) {
        i++;
        if (max < i) {
            return 0;
        }
        fd = open(name, 2);
        D_800B58E4 = fd;
    }
    close(D_800B58E4);
    return 1;
}

void func_80054BA8(void) {
    if (func_80054AF4(0) != 0) {
        D_800E8BEE = 1;
        return;
    }
    D_800E8BEE = 0;
}

s32 func_80054BE4(void) {
    s32 i;
    s32 n;
    u8 *p;
    s32 t;

    n = 0;
    p = D_800E7D11;
    i = 1;
    do {
        t = func_80054AF4(i);
        i++;
        p++;
        p[3] = t;
        n++;
    } while (i != 0xF);
    return n;
}

void func_80054C4C(void) {
    bzero(D_80123120, 0x2000);
}

s32 func_80054C74(s32 arg0) {
    u8 name[0x1C];
    u32 i;
    u8 *p;

    func_80054C4C();
    func_80056070(name, arg0);
    close(D_800B58E4);
    if ((D_800B58E4 = open(name, 2)) == -1) {
        return -1;
    }
    strncpy(D_80123120, "SC", 2);
    D_80123122 = 0x13;
    D_80123123 = 1;
    bzero(D_80123124, 0x40);
    strcpy(D_80123124, "ときめきメモリアル");
    if (arg0 != 0) {
        strcat(D_80123124, D_800B32CC[arg0 - 1]);
    } else {
        strcat(D_80123124, "・システム");
    }
    bzero(D_80123164, 0x1C);
    memcpy(D_80123180, D_800B4390, 0x20);
    memcpy(D_801231A0, D_800B58B0, 0x180);
    if (arg0 != 0) {
        func_8005557C(0);
    } else {
        func_80054FC0(0);
    }
    D_80123320[1] = D_80123320[0];
    i = 0;
    p = D_80123120;
    /* FAKE: a one-pass loop; the u32 counter makes the bound a second 0x2000 register next to the write
       size, as in the original. The real loop shape is unknown. T-17007 */
    for (; i < 0x2000; i += 0x2000) {
        if (write(D_800B58E4, p, 0x2000) != 0x2000) {
            close(D_800B58E4);
            return -1;
        }
        p += 0x2000;
    }
    return 1;
}

void func_80054E6C(void) {
    Work800E7D10 *w = &D_800E7D10;
    GameState *g = &D_800E6280;
    s32 pad[14]; /* FAKE: T-25105, 56 unused bytes of the original 0x80 frame; the real locals are unknown */
    s32 i;
    s32 slot;

    slot = w->selected_slot & 0xF;
    w->year[slot] = g->unk_03E;
    w->month[slot] = g->unk_03F;
    w->day[slot] = g->unk_040;
    memcpy(w->family_name[slot], g->unk_0D4, 8);
    memcpy(w->given_name[slot], g->unk_0DC, 8);
    memcpy(w->nickname[slot], g->unk_0E4, 0x10);
    for (i = 0; i < 9; i++) {
        /* each 4-byte record is copied as one word */
        *(s32 *)&w->metrics[i][slot] = ((s32 *)&g->unk_0FC)[i];
    }
    for (i = 0; i < 16; i++) {
        w->unk_CC8[slot][i] = g->unk_1BC[15].unk_14[i];
    }
    w->club[slot] = g->unk_0F4.h >> 12;
}

s32 func_80054FC0(s32 arg0) {
    SaveImageView *bank;
    u8 *p;
    s32 sum;
    u32 offset;

    bank = (SaveImageView *)D_80123320;
    D_800E7D10.slot_status[D_800E7D10.selected_slot] = 1;
    memcpy(bank->slot_status, D_800E7D10.slot_status, 16);
    memcpy(bank->flags, D_800E7D10.flags, 16);
    bank->unk_24[0] = D_800E7D10.unk_024[0];
    bank->unk_24[1] = D_800E7D10.unk_024[1];
    /* FAKE: T-25105, an empty signed test with a positive bound after the indexed slot store puts
       tools/ugen_pass.py in block-copy state F for the two record copies below, which is the
       original's temporary numbering (one ugen register skipped, not two). The original's state
       comes from its own compiler; the real source has no such test. */
    if ((s32)D_800E7D10.selected_slot > 0) {
    }
    if (D_800E7D10.selected_slot != 0) {
        bank->selected_slot = D_800E7D10.selected_slot % 15U;
    }
    bank->config0 = ((SaveWorkView *)&D_800E7D10)->config0;
    bank->config1 = ((SaveWorkView *)&D_800E7D10)->config1;
    bank->unk_54[0] = D_800E7D10.unk_027[0];
    bank->unk_54[1] = D_800E7D10.unk_027[1];
    memcpy(bank->records, D_800E7D10.unk_058, 0x800);
    memcpy(bank->year, D_800E7D10.year, 16);
    memcpy(bank->month, D_800E7D10.month, 16);
    memcpy(bank->day, D_800E7D10.day, 16);
    memcpy(bank->family_name, D_800E7D10.family_name, 0x80);
    memcpy(bank->given_name, D_800E7D10.given_name, 0x80);
    memcpy(bank->nickname, D_800E7D10.nickname, 0x100);
    memcpy(bank->club, D_800E7D10.club, 16);
    memcpy(bank->metrics, D_800E7D10.metrics, 0x240);
    memcpy(bank->unk_CD8, D_800E7D10.unk_CC8, 0x100);
    if (D_800E6280.unk_1124 == 0) {
        if (D_800E7D10.selected_slot != 0) {
            memcpy(D_800E7D10.unk_DC8, D_80125C10, 0x40);
            memcpy(&D_800E7D10.unk_DC8[0xF0], &D_80125C50, 4);
            D_800E7D10.unk_DC8[0xF4] = D_800B5C08;
            D_800E7D10.unk_DC8[0xF5] = D_80125C54;
            D_800E7D10.unk_DC8[0xF6] = D_80125C55;
            memcpy(bank->unk_DD8, D_800E7D10.unk_DC8, 0x100);
        }
    }
    /* byte sum of the selected bank's data, from the card block base.
       FAKE: T-25105, the checksum store below names D_80123120 in its address: as1 moves `lw ra`
       above a store only when the address names a global, as the original's epilogue shows (a store
       through a pointer local stays before it). The real source form is unknown. */
    p = &D_80123120[arg0 * 0xF00 + 0x204];
    sum = 0;
    for (offset = 4; offset < 0xF00; offset++) {
        sum += *p++;
    }
    ((SaveBank *)&D_80123120[arg0 * 0xF00 + 0x200])->checksum = sum;
}

s32 func_800552DC(s32 arg0) {
    SaveImageView *bank;

    if (func_80055A38(0) == -1) {
        return -1;
    }
    /* FAKE: T-24501, integer address arithmetic keeps the bank offset grouped
     * before adding the image base; pointer arithmetic reassociates +0x200 and
     * changes temporary numbering throughout both aggregate copies. */
    bank = (SaveImageView *)((D_800E6280.unk_1117 * 0xF00 + 0x200) + (u32)D_80123120);
    memcpy(D_800E7D10.flags, bank->flags, 16);
    D_800E7D10.unk_024[0] |= bank->unk_24[0];
    D_800E7D10.unk_024[1] = bank->unk_24[1];
    D_800E7D10.selected_slot = bank->selected_slot;
    if ((((SaveWorkView *)&D_800E7D10)->config0.flags.b[3] & 1) == 0) {
        ((SaveWorkView *)&D_800E7D10)->config0 = bank->config0;
    }
    if (D_800E7D10.bits.flag == 0) {
        ((SaveWorkView *)&D_800E7D10)->config1 = bank->config1;
    }
    D_800E7D10.unk_027[0] = bank->unk_54[0];
    D_800E7D10.unk_027[1] = bank->unk_54[1];
    memcpy(D_800E7D10.unk_058, bank->records, 0x800);
    memcpy(D_800E7D10.year, bank->year, 16);
    memcpy(D_800E7D10.month, bank->month, 16);
    memcpy(D_800E7D10.day, bank->day, 16);
    memcpy(D_800E7D10.family_name, bank->family_name, 0x80);
    memcpy(D_800E7D10.given_name, bank->given_name, 0x80);
    memcpy(D_800E7D10.nickname, bank->nickname, 0x100);
    memcpy(D_800E7D10.club, bank->club, 16);
    memcpy(D_800E7D10.metrics, bank->metrics, 0x240);
    memcpy(D_800E7D10.unk_CC8, bank->unk_CD8, 0x100);
    memcpy(D_800E7D10.unk_DC8, bank->unk_DD8, 0x100);
    memcpy(D_80125C10, D_800E7D10.unk_DC8, 0x40);
    memcpy(&D_80125C50, &D_800E7D10.unk_DC8[0xF0], 4);
    D_800B5C08 = D_800E7D10.unk_DC8[0xF4];
    D_80125C54 = D_800E7D10.unk_DC8[0xF5];
    D_80125C55 = D_800E7D10.unk_DC8[0xF6];
}

/* Writes a game save into bank arg0 of the card image (SaveGameView), then stores the bank's byte sum.
   FAKE: T-25105, the straight-line stores reach the bank through the byte offset
   `&D_80123120[arg0 * 0xF00 + 0x200]` and the loops as an element of the bank array
   `((SaveGameView *)&D_80123120[0x200])[arg0]`. uopt keeps the two spellings as two values: the
   original's two base registers (a2 for the stores, a3 recomputed before the first loop, both
   spilled around get_k_speed). One spelling for both gives one base register. The real source
   form is unknown. */
s32 func_8005557C(s32 arg0) {
    GameState *g;
    u8 *p;
    s32 sum;
    u32 i; /* one counter for every loop: only the last loop (the byte sum) is unrolled */

    g = &D_800E6280;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_004 = g->unk_03D;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_005 = g->unk_03E;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_006 = g->unk_03F;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_007 = g->unk_040;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_008 = g->unk_041;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_00A = g->unk_042;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_00C[0] = g->unk_044[0];
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_00C[1] = g->unk_044[1];
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_08C = *(GsBlockC4 *)g->unk_0C4;
    for (i = 0; i < 16; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_108[i] = g->unk_1BC[i];
    }
    for (i = 0; i < 16; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_488[i] = g->unk_53C[i];
    }
    for (i = 0; i < 8; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_498[i] = g->unk_54C[i];
    }
    for (i = 0; i < 12; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_4B8[i] = ((GsWord *)g->unk_66C)[i];
    }
    g->unk_668[3] = get_k_speed();
    g->unk_668[2] = D_800B6708;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_4E8 = *(GsBlock56C *)g->unk_56C;
    for (i = 0; i < 4; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5E8[i] = g->unk_728[i];
    }
    for (i = 0; i < 16; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5EC[i] = g->unk_72C[i];
    }
    for (i = 0; i < 16; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_5FC[i] = g->unk_73C[i];
    }
    for (i = 0; i < 16; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_60C[i] = g->unk_74C[i];
    }
    for (i = 0; i < 256; i++) {
        ((SaveGameView *)&D_80123120[0x200])[arg0].unk_61C[i] = g->unk_75E[i];
    }
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1C = g->unk_F6C;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1D = g->unk_F6D;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1E = g->unk_1092;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E1F = g->unk_F6E;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E20 = g->unk_03C;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E24 = g->unk_F68;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E28 = g->unk_F6F;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E29 = g->unk_F70;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2A = g->unk_F73;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2B = g->unk_F75;
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->unk_E2C = *(GsWord *)g->unk_F64;
    /* byte sum of the bank's data; the checksum store is a FAKE address form as in func_80054FC0 (T-25105) */
    p = &D_80123120[arg0 * 0xF00 + 0x204];
    sum = 0;
    for (i = 4; i < 0xF00; i++) {
        sum += *p++;
    }
    ((SaveGameView *)&D_80123120[arg0 * 0xF00 + 0x200])->checksum = sum;
}

/* FAKE: T-10850, T-17007: the dead initial assignments set the induction-register numbering, and
   writing all five on one line makes as1 emit the hoisted starts in the original order (q, r, b).
   The bank checksum is read as *(s32 *)(r + 0x200), not D_80123320[b].checksum: the field form
   changes the register allocation (r is the header-side induction pointer). */
s32 func_80055A38(s32 arg0) {
    s32 b;
    s32 n;
    s32 sum;
    s32 j;
    u8 *p;
    u8 *q;
    u8 *r;

    n = 0; b = 0; p = D_80123320[0].data; r = D_80123120; q = D_80123320[0].data;
    for (; b < 2; b++) {
        n = 0;
        p = q;
        sum = 0;
        for (j = 4; j < 0xF00; j++) {
            if (*p != 0) {
                n++;
            }
            sum += *p++;
        }
        if (sum == *(s32 *)(r + 0x200) && n != 0) {
            D_800E6280.unk_1117 = b;
            return 1;
        }
        r += 0xF00;
        q += 0xF00;
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/main/80053650", func_80055AFC);

s32 func_80056070(u8 *buf, s32 arg1) {
    s32 i;

    /* FAKE: same-line zero init and while condition places move v1, zero after li a0, 20. T-24123 */
    i = 0; while (i < 0x14) {
        buf[i] = 0;
        i++;
    }
    strcpy(buf, D_800B3938);
    switch (arg1) {
    case 0:
        strcat(buf, "0");
        break;
    case 1:
        strcat(buf, "1");
        break;
    case 2:
        strcat(buf, "2");
        break;
    case 3:
        strcat(buf, "3");
        break;
    case 4:
        strcat(buf, "4");
        break;
    case 5:
        strcat(buf, "5");
        break;
    case 6:
        strcat(buf, "6");
        break;
    case 7:
        strcat(buf, "7");
        break;
    case 8:
        strcat(buf, "8");
        break;
    case 9:
        strcat(buf, "9");
        break;
    case 10:
        strcat(buf, "A");
        break;
    case 11:
        strcat(buf, "B");
        break;
    case 12:
        strcat(buf, "C");
        break;
    case 13:
        strcat(buf, "D");
        break;
    case 14:
        strcat(buf, "E");
        break;
    case 15:
        strcat(buf, "F");
        break;
    default:
        return 0;
    }
    return 1;
}

void func_80056284(void) {
    s32 i;

    func_80053D10();
    func_80054C4C();
    D_800E7D14[0] = 0;
    D_800E7D15[0] = 0;
    /* FAKE: the store inside the for-init places it after the loop prologue as in the original; real source unknown. T-9140 */
    for (i = 0, D_800E7D16 = 0; i < 3; i++) {
        D_800E7D13[i * 4 + 5] = 0;
        D_800E7D13[i * 4 + 6] = 0;
        D_800E7D13[i * 4 + 7] = 0;
        D_800E7D13[i * 4 + 4] = 0;
    }
    func_80053CE0();
    D_800E8BEE = 0;
}
