#include "ovl/MASTER.h"

void func_80133680(void) {
    D_8013C4A0 = 0x801E8660;
    D_8013C4A4 = 0x801E8740;
    D_8013C4A8 = 0x801E8AF8;
}

void func_801336B4(void) {
    D_8013C4A0 = 0x801EAA54;
    D_8013C4A4 = 0x801EAE50;
    D_8013C4A8 = 0x801ED1C4;
}

void func_801336E8(void) {
    D_8013C4A0 = 0x801E8A10;
    D_8013C4A4 = 0x801E8B7C;
    D_8013C4A8 = 0x801E93D4;
}

void func_80133724(void) {
    D_8013C4B0 = 0x801EAFC4;
    D_8013C4B4 = 0x801EB144;
    D_8013C4B8 = 0x801EAFF0;
    D_8013C4BC = 0x801EB14C;
    D_8013C4C0 = 0x801EB030;
    D_8013C4C4 = 0x801EB174;
    D_8013C4C8 = *(s16 *)0x801EB17C;
    D_8013C4CC = *(s16 *)0x801EB180;
    D_8013C4D0 = 0x801B4400;
}

void func_801337B8(void) {
    D_8013C4D4 = 0x801B00DC;
    D_8013C4D8 = 0x801B02AC;
    D_8013C4DC = 0x801B03C8;
    D_8013C4E0 = 0x801B04C4;
    D_8013C4E4 = 0x801B0668;
    D_8013C4E8 = 0x801B0808;
    D_8013C4EC = 0x801B09DC;
    D_8013C4F0 = 0x801B0BE8;
    D_8013C4F4 = 0x801B0D8C;
    D_8013C4F8 = 0x801B0F5C;
    D_8013C4FC = 0x801B1164;
    D_8013C500 = 0x801B12D8;
    D_8013C504 = 0x801B1454;
    D_8013C508 = 0x801B00F4;
    D_8013C50C = 0x801B02C8;
    D_8013C510 = 0x801B03D0;
    D_8013C514 = 0x801B04DC;
    D_8013C518 = 0x801B0680;
    D_8013C51C = 0x801B0820;
    D_8013C520 = 0x801B09F8;
    D_8013C524 = 0x801B0C00;
    D_8013C528 = 0x801B0DA4;
    D_8013C52C = 0x801B0F78;
    D_8013C530 = 0x801B1180;
    D_8013C534 = 0x801B12E0;
    D_8013C538 = 0x801B1470;
    D_8013C53C = 0x801B01A8;
    D_8013C540 = 0x801B0398;
    D_8013C544 = 0x801B03F0;
    D_8013C548 = 0x801B0590;
    D_8013C54C = 0x801B0734;
    D_8013C550 = 0x801B08D4;
    D_8013C554 = 0x801B0AE4;
    D_8013C558 = 0x801B0CB4;
    D_8013C55C = 0x801B0E58;
    D_8013C560 = 0x801B1064;
    D_8013C564 = 0x801B126C;
    D_8013C568 = 0x801B1338;
    D_8013C56C = 0x801B155C;
}

typedef struct {
    void (*f[30])();
} FnTbl30; /* size 0x78 */
extern FnTbl30 D_8013C5C8;

void func_80133A2C(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl30 tbl;

    tbl = D_8013C5C8;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

/* FAKE: scalar byte view of D_800B593C preserves its target live range
 * across the case-1 calls; ordinary access selects v0 instead (T-10630). */
void func_80133AA0(void) {
    GameState *g = &D_800E6280;
    s32 i;

    switch (g->unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_8008585C();
        dec_bg_reset();
        set_dec_bri(0);
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        k_reset(0);
        func_8006612C((s32) "甲子園");
        func_80065F34(0);
        func_800AE120(g->unk_10F4);
        for (i = 0; i < 11; i++) {
            D_800CA2B0[i] = g->unk_1BC[i].unk_0C.f.b1;
        }
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            dec_bg_cd_read(0x41A8, 1);
            (*(u8 *)&D_800B593C) = 0;
            func_8004284C();
        }
        if ((u8) (*(u8 *)&D_800B593C) >= 4U) {
            (*(u8 *)&D_800B593C) -= 4;
        } else {
            (*(u8 *)&D_800B593C) = 0;
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_80044750(0x200);
            func_80065F34(1);
            dec_bg_show_set(1, func_8005751C(1));
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            func_8004284C();
        }
        break;
    case 3:
        normal_date_bg_fadein();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            g->unk_F5F = 0;
            func_800847B8(0U);
            func_80084E4C();
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        dec_bg_cd_read(0x417A, 1);
        func_8004284C();
        break;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_8005751C(1);
            func_8004284C();
        }
        break;
    case 8:
        cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
        func_8004284C();
        break;
    case 9:
        normal_date_move_place();
        break;
    case 10:
        if (cd_read_sync_status() & 1) {
            func_80138F10();
            i = 0;
            do {
                load_palette(D_8013C730, (i + 0x10) & 0xFF, 1, 1, i);
                i += 1;
            } while (i != 5);
            func_80048F64(0x60);
            ((MasterSprite44 *)D_80120650)->unk_01 = 3;
            ((MasterSprite44 *)D_80120650)->unk_38 = 0x01000000;
            ((MasterSprite44 *)D_80120650)->unk_07 = 0x80;
            ((MasterSprite44 *)D_80120650)->unk_05 = 0x80;
            ((MasterSprite44 *)D_80120650)->unk_1A = 0x1000;
            ((MasterSprite44 *)D_80120650)->unk_1C = 0x1000;
            ((MasterSprite44 *)D_80120650)->unk_06 = 1;
            ((MasterSprite44 *)D_80120650)->unk_26 = -0x60;
            ((MasterSprite44 *)D_80120650)->unk_28.h[1] = 0x20;
            ((MasterSprite44 *)D_80120650)->unk_0C = (u8 *) D_8013C724;
            ((MasterSprite44 *)D_80120650)->unk_10 = (u8 *) D_8013C728;
            ((MasterSprite44 *)D_80120650)->unk_34 = (u8 *) D_8013C720;
            ((MasterSprite44 *)D_80120650)->unk_14 = D_8013C72C;
            func_8004284C();
        }
        break;
    case 11:
        D_8013C570 = 0;
        ((MasterClubBits *)&g->unk_66C[6])->count += 1;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}


/* FAKE: the scalar byte view prevents promotion of the probability byte
 * into a conflicting live range. value is reused after the threshold tests
 * as the second shuffle index, preserving the frame layout (T-10630). */
void func_80133F80(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;
    MasterHalfwords5 thresholds;

    thresholds = D_8013C640;
    switch (D_800E6280.unk_110A) {
    case 0:
        D_800CA148 = (D_8013C570 * 3) + 1;
        func_8004284C();
        break;
    case 1:
        normal_date_speak();
        break;
    case 2:
        if ((func_800AE0D0() % 100) == 0) {
            func_80042940(0xA);
        } else {
            func_8004284C();
        }
        break;
    case 3:
        value = (s16)((D_800E6280.unk_66C[6].unk_00 + ((s16) D_800E6280.unk_118.unk_02 >> 4)) - thresholds.v[D_8013C570]);
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
        for (a = D_8013C57C; a < D_8013C57C + 5; a++) {
            *a = 0;
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
        if (swap_count > 0) {
            do {

                first = func_800AE0D0() & 3;
                value = func_800AE0D0() & 3;
                swap = D_8013C57C[first];
                i++;
                other = D_8013C57C[value];
                D_8013C57C[first] = other;
                D_8013C57C[value] = swap;
            } while (i != swap_count);
        }
        ((MasterSprite44 *)D_80120650)->unk_43 = ((s32) (D_8013C570 + 2) % 5) + 0x10;
        D_8013C574 = 0;
        func_8004284C();
        break;
    case 4:
        if (((MasterSprite44 *)D_80120650)->unk_02 == 0) {
            if (D_8013C574 >= 5) {
                func_8004284C();
            } else {
                ((MasterSprite44 *)D_80120650)->unk_03 = 0x84;
                ((MasterSprite44 *)D_80120650)->unk_02 = 5;
                ((MasterSprite44 *)D_80120650)->unk_18 = 0;
                ((MasterSprite44 *)D_80120650)->unk_08 = 0;
                if (D_8013C57C[D_8013C574] == 1) {
                    ((MasterSprite44 *)D_80120650)->unk_16 = 0;
                } else {
                    ((MasterSprite44 *)D_80120650)->unk_16 = 1;
                }
                D_8013C574 += 1;
            }
        }
        break;
    case 5:
        ((MasterSprite44 *)D_80120650)->unk_03 = 0x84;
        ((MasterSprite44 *)D_80120650)->unk_02 = 1;
        ((MasterSprite44 *)D_80120650)->unk_18 = 0;
        ((MasterSprite44 *)D_80120650)->unk_08 = 0;
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            ((MasterSprite44 *)D_80120650)->unk_16 = 2;
        } else {
            ((MasterSprite44 *)D_80120650)->unk_16 = 3;
            if (D_8013C570 == 3) {
                D_800CA148 = 0x17;
            } else {
                D_800CA148 = 0x19;
            }
        }
        if ((u8) (*(u8 *)&D_8013C578) >= 3U) {
            func_80044750(0x204);
        } else {
            func_80044750(0x205);
        }
        func_8004284C();
        break;
    case 6:
        normal_date_speak();
        break;
    case 7:
        k_reset(1);
        func_8004284C();
        break;
    case 8:
        normal_date_speak();
        break;
    case 9:
        ((MasterSprite44 *)D_80120650)->unk_03 = 0;
        ((MasterSprite44 *)D_80120650)->unk_02 = 0;
        func_80042808();
        break;
    case 10:
        D_800CA148 = 0x10;
        func_8004284C();
        break;
    case 11:
        normal_date_speak();
        break;
    case 12:
        k_reset(1);
        D_800CA148 += D_8013C570;
        func_8004284C();
        break;
    case 13:
        normal_date_speak();
        break;
    case 14:
        func_80044750(0x204);
        D_800CA148 = 0x16;
        func_8004284C();
        break;
    case 15:
        normal_date_speak();
        break;
    case 16:
        (*(u8 *)&D_8013C578) = 3;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}


INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801344E0);


void func_80135040(void) {
    if (((D_800E6280.unk_1BC[0].unk_10.w & 0xF) == D_800E6280.unk_03F) && (((u32)(D_800E6280.unk_1BC[0].unk_10.w << 0x17) >> 0x1B) == D_800E6280.unk_040)) {
        func_80138490();
        return;
    }
    func_80042808();
    Default_Disp();
}

void func_801350A8(void) {
    s32 index;
    MasterWords11 text_ids;
    u8 flags;

    text_ids = D_8013C64C;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        if ((D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF)) && (D_800E6280.unk_040 == ((u32) (D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && !((flags = D_800E6280.unk_71E) & 4)) {
            func_80044750(0xC1);
            flags = D_800E6280.unk_71E;
            flags |= 4;
            D_800E6280.unk_71E = flags;
            func_8004284C();
        } else {
            func_80042940(0xB);
        }
        break;
    case 1:
        D_800E6280.unk_71C += 1;
        if (func_8008667C(func_8008647C()) < 2) {
            D_800E6280.unk_F5F = func_8008647C();
            func_80085B3C(0x15, 6);
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_800E6280.unk_F5F], D_800B36C8[D_800E6280.unk_F5F], D_800B3708[D_800E6280.unk_F5F]);
            func_8004284C();
        } else {
            func_80042940(0x10);
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            index = D_800E6280.unk_F5F;
            func_80062CD0(text_ids.v[index]);
            func_8004284C();
        }
        break;
    case 3:
        if ((flags = D_800E6280.unk_71E) & 1) {
            func_8004284C();
        } else {
            func_8007EAAC();
        }
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8007C740();
            func_80083474();
            cd_read_sectors(3U, 0x801B0000, 0xAF43);
        }
        break;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_801337B8();
            func_800847B8(D_800E6280.unk_F5F);
            D_800CA148 = 0;
            D_800CA14C = 0;
            switch (D_800E6280.unk_F5F) {
            case 0:
                D_800CA160 = D_8013C4D8;
                D_800CA164 = D_8013C50C;
                D_800CA168 = D_8013C540;
                break;
            case 1:
                D_800CA160 = D_8013C4EC;
                D_800CA164 = D_8013C520;
                D_800CA168 = D_8013C554;
                break;
            case 2:
                D_800CA160 = D_8013C4E0;
                D_800CA164 = D_8013C514;
                D_800CA168 = D_8013C548;
                break;
            case 3:
                D_800CA160 = D_8013C4E4;
                D_800CA164 = D_8013C518;
                D_800CA168 = D_8013C54C;
                break;
            case 4:
                D_800CA160 = D_8013C4FC;
                D_800CA164 = D_8013C530;
                D_800CA168 = D_8013C564;
                break;
            case 5:
                D_800CA160 = D_8013C4F0;
                D_800CA164 = D_8013C524;
                D_800CA168 = D_8013C558;
                break;
            case 6:
                D_800CA160 = D_8013C4F4;
                D_800CA164 = D_8013C528;
                D_800CA168 = D_8013C55C;
                break;
            case 7:
                D_800CA160 = D_8013C4E8;
                D_800CA164 = D_8013C51C;
                D_800CA168 = D_8013C550;
                break;
            case 8:
                D_800CA160 = D_8013C4D4;
                D_800CA164 = D_8013C508;
                D_800CA168 = D_8013C53C;
                break;
            case 9:
                D_800CA160 = D_8013C4F8;
                D_800CA164 = D_8013C52C;
                D_800CA168 = D_8013C560;
                break;
            case 10:
                D_800CA160 = D_8013C504;
                D_800CA164 = D_8013C538;
                D_800CA168 = D_8013C56C;
                break;
            }
            func_80044750(0x200);
            func_8004284C();
        }
        break;
    case 6:
        func_80085A60();
        break;
    case 7:
        func_8007E390();
        break;
    case 8:
        func_80085A60();
        break;
    case 9:
        func_8007E5A0();
        if (D_800E6280.unk_110A == 0xA) {
            func_80044750(0xC1);
        }
        break;
    case 10:
        func_80085A60();
        break;
    case 11:
        if (((flags = D_800E6280.unk_71E) & 1) || ((flags = D_800E6280.unk_71E) & 4)) {
            func_80044750(0xBF);
            func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
            func_8004284C();
        } else {
            func_80042808();
        }
        break;
    case 12:
        if (func_80044E8C() == 1) {
            cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
            func_8004284C();
        }
        break;
    case 13:
        func_8007EAAC();
        break;
    case 14:
        if (cd_read_sync_status() & 1) {
            func_80138F10();
            func_80043914(D_8013C730, 0x10, 1, 1, 0);
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            func_8004284C();
        }
        break;
    case 15:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            D_800CA148 = 0;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_80042808();
        }
        break;
    case 16:
        if ((flags = D_800E6280.unk_71E) & 1) {
            cd_read_sectors(2U, 0x801E8000, 0xA9BA);
            func_8004284C();
        } else {
            D_800CA148 = 0x1B;
            func_80042940(0x12);
        }
        break;
    case 17:
        if (cd_read_sync_status() & 1) {
            func_80133680();
            D_800E6280.unk_F5F = 0;
            func_800847B8(0U);
            D_800CA148 = 0x1B;
            D_800CA14C = 0;
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            func_8004284C();
        }
        break;
    case 18:
        func_80085A60();
        break;
    case 19:
        func_80042940(0xB);
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

void func_801358C8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:
        func_80085A60();
        break;
    case 3:
        D_800CA148 = 5;
        func_8004284C();
        break;
    case 4:
        func_80085A60();
        break;
    case 5:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

void func_801359F0(void) {
    s32 pad[2]; /* FAKE: keep the original 0x68-byte frame without
                 * moving the table or call spill (T-10630). */
    s32 selection;
    MasterWords11 messages;

    messages = D_8013C678;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004284C();
        selection = func_8008647C();
        if ((u8) D_800E6280.unk_1BC[12].unk_0C.b[3] >= 0x41U) {
            D_8013C584 = 0xC;
            func_80062CD0(0x57FB);
        } else {
            if (func_8008667C(selection) < 2) {
                D_8013C584 = (u8) selection;
                if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 6) && (func_8008667C(0) < 2)) {
                    D_8013C584 = 0;
                }
                if ((((u8) D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6) && (func_8008667C(4) < 2) && (D_800E6280.unk_1BC[4].unk_0C.f.b1 == 1)) {
                    D_8013C584 = 4;
                }
                func_80062CD0(messages.v[D_8013C584]);
            } else if ((u8) D_800E6280.unk_1BC[11].unk_0C.b[3] >= 2U) {
                D_8013C584 = 0xB;
                func_80062CD0(0x6B51);
            } else {
                D_8013C584 = 0xFF;
                func_80042940(4);
            }
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80044890(0, 0xC043, 0xC01D, D_800B3688[D_8013C584], D_800B36C8[D_8013C584], D_800B3708[D_8013C584]);
            func_8004284C();
        }
        break;
    case 2:
        if (func_80044E8C() == 1) {
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            func_8004284C();
        }
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_80042808();
        }
        break;
    case 4:
        func_80042808();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135CF4);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_80135FB8);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", func_801363F4);

void func_80136E24(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (((MasterClubBits *)&D_800E6280.unk_66C[6])->count < 3U) {
            func_80042940(2);
        } else {
            cd_read_sectors(0xBU, 0x801E8000, 0xA9C5);
            func_8004284C();
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_801336B4();
            D_800CA160 = (s32) D_8013C4A0;
            D_800CA164 = (s32) D_8013C4A4;
            D_800CA168 = (s32) D_8013C4A8;
            D_800CA148 = 0;
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 2:
        if ((u8) D_8013C578 >= 3U) {
            D_800E6280.unk_1BC[0].unk_02 += 5;
            for (i = 1; i < 13; i++) {
                D_800E6280.unk_1BC[i].unk_02 += 5;
            }
            ((MasterClubBits *)&D_800E6280.unk_66C[6])->b30 = 1;
        }
        if (D_8013C584 == 0xFF) {
            func_80042808();
            func_80042808();
            func_80042808();
        } else {
            D_800E6280.unk_1BC[D_8013C584].unk_06 += 2;
            func_80044750(0x200);
            if ((u8) D_8013C578 >= 3U) {
                func_80042808();
            } else {
                func_80042808();
                func_80042808();
            }
        }
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

/* FAKE: scalar byte views preserve the target live ranges, and
 * the cached girls pointer preserves the copied table offset (T-10630).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_80137044(void) {
    Rec38 *girls;
    u8 index;
    MasterHalfwords13 messages;

    messages = D_8013C6C0;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80083474();
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            func_80083440(0U);
            break;
        case 6:
            func_80083440(1U);
            break;
        case 7:
            func_80083440(1U);
            break;
        case 9:
            func_80083440(5U);
            D_801217D0[0].unk_0C = 7;
            D_801217D0[0].unk_0E = 0x70;
            break;
        case 12:
            func_80083440(0U);
            break;
        }
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        dec_bg_show_switch(1);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            girls = D_800E6280.unk_1BC;
            func_80137F94();
            index = (*(u8 *)&D_8013C584);
            if ((index == 4) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != ((u8) girls[4].unk_0C.b[2] >> 4))) {
                D_800CA148 = 0x40;
            } else if (index == 6) {
                D_800CA148 = 0x4B;
            } else {
                D_800CA148 = messages.v[index];
            }
            if (index == 9) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 8;
            }
            func_8004284C();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if ((*(u8 *)&D_8013C584) == 9) {
            (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) += 4;
            if ((*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) >= 0x7DU) {
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
            }
        }
        if (((*(u8 *)&D_8013C584) != 0xB) && (D_800E6280.unk_110A == 3)) {
            func_8004500C(0, 0);
        }
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            if (D_800CA148 == 0x2A) {
                func_80083440(1U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 1:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 2:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 4:
            if (D_800CA148 == 0x3F) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 4;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80083440(5U);
                D_801217D0[0].unk_0C = 7;
                D_801217D0[0].unk_0E = 0x70;
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 5:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 6:
            if (D_800CA148 == 0x4C) {
                func_80083440(0U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 7:
            if (D_800CA148 == 0x52) {
                func_80083440(0U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x21;
                func_8004284C();
            }
            break;
        case 8:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 9:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 10:
            D_800CA148 = 0x21;
            func_8004284C();
            break;
        case 11:
            func_80042940(6);
            break;
        case 12:
            D_800CA148 = 0x23;
            func_8004284C();
            break;
        }
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        if ((u8) (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) < 4U) {
            (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0;
        } else {
            (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) -= 4;
        }
        break;
    case 7:
        func_80138088();
        func_80042808();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}


/* FAKE: scalar byte views preserve the target live ranges, and
 * the constant value local preserves the copied table offset (T-10630).
 * The sprite aggregate keeps flag loads after neighboring field stores. */
void func_801375E8(void) {
    s32 value;
    u8 index;
    MasterHalfwords13 messages;

    messages = D_8013C6DC;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80083474();
        func_80083440(5U);
        if ((*(u8 *)&D_8013C584) == 0xC) {
            func_80083440(0U);
        }
        dec_bg_show_switch(1);
        cd_read_sectors(0xEU, 0x80197000, 0xA9D0);
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80137F94();
            index = (*(u8 *)&D_8013C584);
            D_800CA148 = messages.v[index];
            D_800CA14C = 0;
            if (index == 9) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                value = 8;
                ((MasterSprite44 *)D_80120650)->unk_16 = value;
            }
            if (index == 0xA) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 0xA;
            }
            func_8004284C();
        }
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadein();
        } else {
            normal_date_bggirl_fadein();
        }
        if (((*(u8 *)&D_8013C584) == 0xA) || ((*(u8 *)&D_8013C584) == 9)) {
            if ((u8) (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) >= 0x81U) {
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
            } else {
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) += 4;
            }
        }
        if (((*(u8 *)&D_8013C584) != 0xB) && (D_800E6280.unk_110A == 3)) {
            func_8004500C(0, 0);
        }
        break;
    case 3:
    case 5:
        normal_date_speak();
        break;
    case 4:
        switch ((*(u8 *)&D_8013C584)) {
        case 0:
            if (D_800CA148 == 0x2C) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 0;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 1:
            if (D_800CA148 == 0x31) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 2;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 2:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 3:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 4:
            if (D_800CA148 == 0x42) {
                ((MasterSprite44 *)D_80120650)->unk_16 = 4;
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 5:
            if (D_800CA148 == 0x47) {
                func_80083440(2U);
                func_80063668();
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 6:
            if (D_800CA148 == 0x4E) {
                ((MasterSprite44 *)D_80120650)->unk_03 |= 0x80;
                ((MasterSprite44 *)D_80120650)->unk_16 = 6;
                (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0x80;
                func_80042940(3);
            } else {
                D_800CA148 = 0x24;
                func_8004284C();
            }
            break;
        case 7:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 8:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 9:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 10:
            D_800CA148 = 0x24;
            func_8004284C();
            break;
        case 11:
            func_80042940(6);
            break;
        case 12:
            D_800CA148 = 0x26;
            func_8004284C();
            break;
        }
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            normal_date_bg_fadeout();
        } else {
            normal_date_bggirl_fadeout();
        }
        if ((u8) (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) < 4U) {
            (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) = 0;
        } else {
            (*(u8 *)&((MasterSprite44 *)D_80120650)->unk_07) -= 4;
        }
        break;
    case 7:
        func_80138374();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}


void func_80137B5C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
    } else if (func_80044E8C() == 1) {
        func_80048F64(0x60);
        D_800E6280.unk_71E |= 0x10;
        func_8008585C();
        set_dec_bri(0);
        func_8006D6E0();
        func_80072B5C(1);
    }
    Default_Disp();
}

void func_80137BF4(void) {
    MasterPoint2 *temp_s0_2;
    s16 *temp_s1;
    /* FAKE: wide X local plus a narrow third argument preserves the
     * original sign-extension sequence. T-10630. */
    s32 temp_a0;
    s16 temp_a1;
    s16 temp_v0;
    s16 temp_v0_2;
    s16 var_s2;
    s32 temp_hi;
    MasterSprite44 *temp_s0;

    if ((((MasterSprite44 *)D_80120650)->unk_16 == 0) && (((MasterSprite44 *)D_80120650)->unk_18 == 7) && (((MasterSprite44 *)D_80120650)->unk_08 == 2)) {
        ((MasterSprite44 *)D_80120650)->unk_16 = 1;
        ((MasterSprite44 *)D_80120650)->unk_18 = 0;
        ((MasterSprite44 *)D_80120650)->unk_08 = 0;
        ((MasterSprite44 *)D_80120650)->unk_02 = 1;
    }
    var_s2 = 0x61;
    if ((((MasterSprite44 *)D_80120650)->unk_16 == 2) && (((MasterSprite44 *)D_80120650)->unk_18 == 1) && (((MasterSprite44 *)D_80120650)->unk_08 == 4)) {
        ((MasterSprite44 *)D_80120650)->unk_16 = 3;
        ((MasterSprite44 *)D_80120650)->unk_18 = 0;
        ((MasterSprite44 *)D_80120650)->unk_08 = 0;
        ((MasterSprite44 *)D_80120650)->unk_02 = 1;
    }
    do {
        temp_s0 = &((MasterSprite44 *)D_8011ECD0)[var_s2];
        temp_v0 = temp_s0->unk_18;
        if ((temp_v0 < 2) || (temp_v0 >= 8)) {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((MasterSprite44 *)D_80120650)->unk_16 == 3) * 0x50000) - 0x50000;
        } else if ((temp_v0 == 4) || (temp_v0 == 7)) {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((MasterSprite44 *)D_80120650)->unk_16 == 3) * 0x30000) - 0x30000;
        } else {
            temp_s0->unk_28.w = temp_s0->unk_28.w - ((((MasterSprite44 *)D_80120650)->unk_16 == 3) * 0x20000) - 0x20000;
        }
        if (temp_s0->unk_28.h[1] < -0x9F) {
            temp_hi = func_800AE0D0() % 320;
            temp_s0->unk_26 = (s16) (temp_hi - 0xA0);
            temp_s0->unk_28.h[1] = 0x3C;
            temp_s0->unk_18 = (s16) (func_800AE0D0() % 10);
        }
        var_s2 += 1;
    } while (var_s2 < 0x74);
    var_s2 = 0;
    if ((u8) ((MasterSprite44 *)D_80120650)->unk_07 >= 0x78U) {
        do {
            temp_s0_2 = &D_8013C58C[var_s2];
            temp_s1 = &D_8013C5B4[var_s2];
            temp_a0 = temp_s0_2->x;
            temp_v0_2 = *temp_s1;
            temp_s0_2->y -= temp_v0_2 * 4 * ((((MasterSprite44 *)D_80120650)->unk_16 == 3) + 1);
            temp_a1 = temp_s0_2->y;
            func_80049FF0((s32) temp_a0, (s32) temp_a1, (s16) temp_a0, (s32) (s16) ((temp_v0_2 * 0x14) + temp_a1), 0xFFFFFF, 0xFFFFFF, 0xB);
            if (temp_s0_2->y < -0xAA) {
                temp_s0_2->y = 0x3C;
                temp_s0_2->x = (func_800AE0D0() % 320) - 0xA0;
                *temp_s1 = (func_800AE0D0() % 5) + 1;
            }
            var_s2 += 1;
        } while (var_s2 < 0xA);
    }
}

void func_80137F94(void) {
    func_80138EC0();
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120657 = 0;
    D_80120655 = 0x80;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
    D_80120693 = 0x10;
    D_80120676 = 0;
    D_8012067A = -0x20;
    D_8012065C = (u8 *)D_8013C714;
    D_80120660 = (u8 *)D_8013C718;
    D_80120684 = (u8 *)D_8013C710;
    D_80120664 = D_8013C71C;
    D_80120652 = 1;
    D_80120653 = 4;
    D_80120668 = 0;
}

void func_80138088(void) {
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 6;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 1:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 2:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 3:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 4:
        if ((D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6) {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        } else {
            D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        }
        break;
    case 5:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 6:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        break;
    case 7:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
        break;
    case 8:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 9:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = 0xFF;
        break;
    case 10:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 11:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    case 12:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 1;
        break;
    }
    if ((D_800E6280.unk_F5E = D_800E6280.unk_F5E + 1) >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}

void func_80138374(void) {
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 6;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    D_800E6280.unk_F5E += 1;
    if (D_800E6280.unk_F5E >= 0xFFU) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
