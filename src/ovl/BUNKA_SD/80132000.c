#include "common.h"
#include "game.h"
#include "ovl/BUNKA_SD.h"

s32 func_80132000(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xBEFB, 0xBEE2, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 2) {
        if (func_80044F94(1) == 1) {
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
        }
    } else if (D_800E6280.unk_110D == 3) {
        cd_read_sectors(0x52, 0x80180000, 0x4B82);
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    } else if (D_800E6280.unk_110D == 4) {
        if (cd_read_sync_status() & 1) {
            func_8004284C();
        }
    }
}

void func_80132148(void) {
    switch (D_80122EAC) {
    case 0:
        D_8013B7F0 = D_8013B74C;
        break;
    case 1:
        D_8013B7F0 = D_8013B794;
        D_8013B7F4 = D_8013B7B0;
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1E00004C, 0x10000051, 0xE);
        func_80044750(0x300);
        break;
    case 2:
        D_8013B7F0 = D_8013B670;
        D_8013B7F4 = D_8013B6B4;
        D_800E6280.unk_F5F = 0;
        func_80046290(0x1C0002E9, 0x410002F3, 0);
        func_80044750(0x300);
        break;
    case 3:
        D_8013B7F0 = D_8013B6E4;
        D_8013B7F4 = D_8013B71C;
        D_800E6280.unk_F5F = 1;
        func_80046290(0x1A0001B5, 0x150001CA, 1);
        func_80044750(0x300);
        break;
    case 4:
    default:
        D_8013B7F0 = D_8013B7C8;
        D_8013B7F4 = D_8013B7DC;
        D_800E6280.unk_F5F = 0xB;
        func_80046290(0x1F0000AD, 0x100000B3, 0xB);
        func_80044750(0x300);
        break;
    }
    func_8004284C();
}

void func_801322EC(void) {
    s32 i;

    if (D_800E6280.unk_110A >= 5U && D_800E6280.unk_110A < 15U) {
        if (D_800E6280.unk_F80 & 0x20) {
            if (func_800460EC() != 2) {
                func_80044750(0x74);
            }
            func_80132428();
            for (i = 0; i < 10; i++) {
                D_8011ECD0[(i * 0x44) + 0x1986] = 0x82;
            }
        } else {
            for (i = 0; i < 10; i++) {
                D_8011ECD0[(i * 0x44) + 0x1986] = 1;
            }
        }
    }
    if (D_800E6280.unk_110A >= 5U && D_800E6280.unk_110A < 14U && (D_800E6280.unk_F80 & 0x40)) {
        func_80044750(0xC4);
        func_80042940(13);
    }
    func_80132428();
}

void func_80132428(void) {
    func_80066C08(2);
    func_80064F48();
    func_80066334();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xC1);
        func_80134260();
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.u >= 0x1FU) {
            func_8004284C();
        }
        break;
    case 2:
        func_80132000();
        break;
    case 3:
        func_80132148();
        break;
    case 4:
        func_801325E8();
        func_80044750(0xE4);
        break;
    case 5:
        func_80134418();
        break;
    case 6:
        func_8013261C();
        break;
    case 7:
        func_80132708();
        func_80132DFC();
        break;
    case 8:
        func_80132DB8(0xF);
        break;
    case 9:
        func_80132708();
        func_80132FF0();
        break;
    case 10:
        func_80132DB8(0x1E);
        break;
    case 11:
        func_80132708();
        func_80133204();
        break;
    case 12:
        func_801327E4();
        break;
    case 13:
        func_8004E9F4(1);
        func_8004284C();
        break;
    case 14:
        func_801344A8();
        break;
    case 15:
        func_80134314();
        break;
    default:
        func_80046500();
        break;
    }
    D_800E6280.unk_1104.u += 1;
}


void func_801325E8(void) {
    func_80132940();
    k_speed_set(5);
    D_8013B7EC = 0;
    func_8004284C();
}

void func_8013261C(void) {
    if (D_800E6280.unk_1104.w == 1) {
        D_8013B808 = 0;
        D_801206DB = 0x84;
        D_801206DA = 5;
    }
    /* FAKE: the ^ 0 keeps the constant as the left operand of the bne (decomp-permuter); real source unknown. T-8060 */
    if ((D_80122EAC != 4) && (0x14 == (D_801206F0 ^ 0))) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            func_80044750(0x501);
        }
    }
    if ((D_80122EAC == 4) && (D_801206F0 == 5)) {
        D_8013B808 += 1;
        if (D_8013B808 == 0x14) {
            func_80044750(0x501);
        }
    }
    if (!((u8) D_801206DA & 1)) {
        func_8004284C();
    }
}

s32 func_80132708(void) {
    u8 v;
    u32 q;
    s32 t;

    v = *(u8 *)&D_800E6280.unk_110C;
    D_801206DA = 0x40;
    q = v / 14;
    if (q >= 2) {
        v = 0;
        q = 0;
    }
    t = q * 2;
    /* FAKE: dead test keeps q * 2 computed once before the switch (T-10310) */
    if (t < 0) {
    }
    switch (D_80122EAC) {
    case 0:
        D_801206F0 = 0x19 - t;
        break;
    case 1:
        D_801206F0 = 0x45 - t;
        break;
    case 2:
        D_801206F0 = 0x71 - t;
        break;
    case 3:
        D_801206F0 = 0x9D - t;
        break;
    case 4:
    default:
        D_801206F0 = 0xBD - t;
        break;
    }
    v = v + 1;
    D_800E6280.unk_110C = v;
}

void func_801327E4(void) {
    if (D_800E6280.unk_1104.w == 1) {
        D_8013B80C = 0;
        D_801206DB = 4;
        D_8012071F = 0x84;
        D_8012071E = 5;
    }
    if ((D_80122EAC != 3) && (D_80122EAC != 4)) {
        if (D_80120734 == 0) {
            D_8013B80C += 1;
            if (D_8013B80C == 0x14) {
                func_80044750(0x501);
            }
        }
    }
    if ((D_80122EAC == 3) && (D_80120734 == 0xB)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            func_80044750(0x501);
        }
    }
    if ((D_80122EAC == 4) && (D_80120734 == 5)) {
        D_8013B80C += 1;
        if (D_8013B80C == 1) {
            func_80044750(0x501);
        }
    }
    if (!((u8) D_8012071E & 1)) {
        func_8004284C();
    }
}

void func_80132940(void) {
    u16 frame;
    s32 i;
    DisplayActor44 *record;

    i = 0;
    do {
        load_palette(D_8013B804, (i + 0x10) & 0xFF, 1, 1, i);
        i += 1;
    } while (i < 0x10);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_80120650)[0].unk_01 = 5;
    ((DisplayActor44 *)D_80120650)[0].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[0].unk_03 = 0x84;
    ((DisplayActor44 *)D_80120650)[0].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[0].unk_0C = D_8013B7FC;
    ((DisplayActor44 *)D_80120650)[0].unk_10 = D_8013B800;
    ((DisplayActor44 *)D_80120650)[0].unk_34 = D_8013B7F8;
    ((DisplayActor44 *)D_80120650)[0].unk_18 = 0xC4;
    ((DisplayActor44 *)D_80120650)[0].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[0].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[0].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[0].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[0].unk_04 = 0;
    func_80048F64(0x61);
    ((DisplayActor44 *)D_80120650)[1].unk_01 = 4;
    ((DisplayActor44 *)D_80120650)[1].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[1].unk_03 = 0x84;
    ((DisplayActor44 *)D_80120650)[1].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[1].unk_0C = D_8013B7FC;
    ((DisplayActor44 *)D_80120650)[1].unk_10 = D_8013B800;
    ((DisplayActor44 *)D_80120650)[1].unk_34 = D_8013B7F8;
    ((DisplayActor44 *)D_80120650)[1].unk_18 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[1].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[1].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[1].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[1].unk_04 = 0;
    record = (DisplayActor44 *)D_8011ED58;
    i = 2;
    do {
        func_80048F64(i + 0x60);
        record[96].unk_01 = 4;
        record[96].unk_38 = 0x01000000;
        record[96].unk_03 = 4;
        record[96].unk_0C = D_8013B7FC;
        record[96].unk_10 = D_8013B800;
        record[96].unk_34 = D_8013B7F8;
        record++;
        frame = D_80122EAC * 2 + i;
        i += 1;
        record[95].unk_16 = frame - 2;
        record[95].unk_06 = 1;
        record[95].unk_07 = 0;
        record[95].unk_26 = -0xA0;
        record[95].unk_2A = -0x78;
        record[95].unk_05 = 0x80;
        record[95].unk_43 = 0;
        record[95].unk_04 = 0;
    } while (i != 4);
    func_80048F64(0x64);
    ((DisplayActor44 *)D_80120650)[4].unk_01 = 4;
    ((DisplayActor44 *)D_80120650)[4].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_80120650)[4].unk_03 = 0x84;
    ((DisplayActor44 *)D_80120650)[4].unk_02 = 0x40;
    ((DisplayActor44 *)D_80120650)[4].unk_0C = D_8013B7FC;
    ((DisplayActor44 *)D_80120650)[4].unk_10 = D_8013B800;
    ((DisplayActor44 *)D_80120650)[4].unk_34 = D_8013B7F8;
    switch (D_80122EAC) {
    case 0:
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0xBF;
        break;
    case 1:
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0xC2;
        break;
    case 2:
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0xC0;
        break;
    case 3:
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0xC1;
        break;
    case 4:
    default:
        ((DisplayActor44 *)D_80120650)[4].unk_18 = 0xC3;
        break;
    }
    ((DisplayActor44 *)D_80120650)[4].unk_07 = 0;
    ((DisplayActor44 *)D_80120650)[4].unk_26 = -0xA0;
    ((DisplayActor44 *)D_80120650)[4].unk_2A = -0x78;
    ((DisplayActor44 *)D_80120650)[4].unk_05 = 0x80;
    ((DisplayActor44 *)D_80120650)[4].unk_43 = 0;
    ((DisplayActor44 *)D_80120650)[4].unk_04 = 0;
    load_tpage_buf_lock(5, ((s32 *)D_8013B7F8)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_8013B7F8)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_8013B7F8)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_8013B7F8)[3], 1);
    load_tpage_buf_lock(9, ((s32 *)D_8013B7F8)[4], 1);
    load_tpage_buf_lock(0xA, ((s32 *)D_8013B7F8)[5], 1);
    load_tpage_buf_lock(0x25, ((s32 *)D_8013B7F8)[6], 1);
    load_tpage_buf_lock(0x26, ((s32 *)D_8013B7F8)[7], 1);
    load_tpage_buf_lock(0x27, ((s32 *)D_8013B7F8)[8], 1);
}


void func_80132DB8(u32 arg0) {
    func_80132708();
    if (arg0 < (u32)D_800E6280.unk_1104.u) {
        func_8004284C();
    }
}

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139910);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139920);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139938);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139958);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139970);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139988);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139998);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399B4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399C8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_801399E8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A20);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A40);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A54);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139A9C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139ABC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139AD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139AE4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B00);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B44);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B64);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B78);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139B88);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139BF8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C10);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C38);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139C8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CA4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CBC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CD8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139CF0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D18);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D30);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D48);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D74);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139D8C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DC0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DD0);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139DF4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E0C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E24);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E34);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E4C);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E68);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E80);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139E94);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EA8);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EC4);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EDC);

INCLUDE_RODATA("asm/ovl/BUNKA_SD/data/BUNKA_SD/80132000.rodata", D_80139EF4);

void func_80132DFC(void) {
    if (D_800E6280.unk_1104.w == 1) {
        set_kanji_string(-0x78, 0x30, 0, (s32) "題目", 0);
        switch (D_80122EAC) {
        case 0:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「僕の高校生活」", 0);
            break;
        case 1:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「僕の存在」", 0);
            if (!(D_800E6280.unk_F80 & 0x20)) {
                func_80046290(0x1E00004C, 0x10000051, 0xE);
                func_80044750(0x300);
            }
            break;
        case 2:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「高校生活を振り返って」", 0);
            if (!(D_800E6280.unk_F80 & 0x20)) {
                func_80046290(0x1C0002E9, 0x410002F3, 0);
                func_80044750(0x300);
            }
            break;
        case 3:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「貧血と私」", 0);
            break;
        case 4:
        default:
            set_kanji_string(-0x56, 0x30, 0, (s32) "「届け私の想い」", 0);
            if (!(D_800E6280.unk_F80 & 0x20)) {
                func_80046290(0x1F0000AD, 0x100000B3, 0xB);
                func_80044750(0x300);
            }
            break;
        }
    } else if ((func_800460EC() == 2) && (check_end_k() != 0)) {
        func_8004284C();
    }
    check_k_scroll();
    k_disp_inc();
}

void func_80132FF0(void) {
    /* FAKE: 24-byte capacity reproduces observed stack homes; original capacity unknown. T-12090 */
    u8 sp38[24];
    s32 sp34;

    if (D_800E6280.unk_1104.u == 1) {
        switch (D_80122EAC) {
        case 0:
            sp34 = func_800AE0E0(D_800E6280.unk_0D4);
            sp34 += func_800AE0E0(D_800E6280.unk_0DC);
            func_800AE0F0(sp38, D_800E6280.unk_0D4);
            strcat(sp38, "　");
            strcat(sp38, D_800E6280.unk_0DC);
            func_8004E788((s16) (0x60 - (sp34 * 7)), 0x40, 0, (s32) sp38, 0);
            break;
        case 1:
            if (!(D_800E6280.unk_F80 & 0x20)) {
                func_80046290(0x0B00001B, 0x05000020, 0xE);
                func_80044750(0x300);
            }
            func_8004E788(0x36, 0x40, 0, (s32) "弁　論者", 0);
            break;
        case 2:
            if (!(D_800E6280.unk_F80 & 0x20)) {
                func_80046290(0x1D00005B, 0x0B000061, 0);
                func_80044750(0x300);
            }
            func_8004E788(0x28, 0x40, 0, (s32) "藤崎　詩織", 0);
            break;
        case 3:
            func_8004E788(0x28, 0x40, 0, (s32) "如月　未緒", 0);
            break;
        case 4:
        default:
            func_8004E788(0x44, 0x40, 0, (s32) "女生徒", 0);
            break;
        }
    } else if ((func_800460EC() == 2) && (func_8004ECB4() != 0)) {
        func_8004284C();
    }
    func_8007EDF8();
    func_8004EC24();
}


void func_80133204(void) {
    s32 index;
    u8 command;
    s32 *voice;

    if (D_800E6280.unk_1104.u == 1) {
        func_8004E9F4(1);
        switch (D_80122EAC) {
        case 0:
            func_8004E788((s16) (-0x68 - (func_800AE0E0(D_800E6280.unk_0D4) * 4)), 0x30, 0, (s32) D_800E6280.unk_0D4, 0);
            break;
        case 1:
            func_8004E788(-0x80, 0x30, 0, (s32) "弁論者", 0);
            break;
        case 2:
            func_8004E788(-0x78, 0x30, 0, (s32) "藤崎", 0);
            break;
        case 3:
            func_8004E788(-0x78, 0x30, 0, (s32) "如月", 0);
            break;
        case 4:
        default:
            func_8004E788(-0x80, 0x30, 0, (s32) "女生徒", 0);
            break;
        }
        func_8004E788(-0x56, 0x30, 0, (s32) "「", 0);
        index = D_8013B7EC;
        if (index < 0x64) {
            do {
                func_8004E788(-0x48, (s16) (((index - D_8013B7EC) * 0x10) + 0x30), 0, ((u8 **)D_8013B7F0)[index] + 1, 0);
                command = *((u8 **)D_8013B7F0)[index];
                if (command == 0x67) {
                    if (!(D_800E6280.unk_F80 & 0x20) && (D_80122EAC != 0)) {
                        voice = (s32 *)D_8013B7F4 + D_800E6280.unk_110D;
                        func_80046290(voice[0], voice[1], (s32) D_800E6280.unk_F5F);
                        func_80044750(0x300);
                    }
                    D_800E6280.unk_110D += 2;
                    D_8013B7EC = index + 1;
                    index = 0x64;
                } else if (command == 0x45) {
                    if (!(D_800E6280.unk_F80 & 0x20)) {
                        if (D_80122EAC != 0) {
                            voice = (s32 *)D_8013B7F4 + D_800E6280.unk_110D;
                            func_80046290(voice[0], voice[1], (s32) D_800E6280.unk_F5F);
                            func_80044750(0x300);
                        }
                    }
                    D_800E6280.unk_110D = 0xFF;
                    D_8013B7EC = index + 1;
                    index = 0x64;
                }
                index += 1;
            } while (index < 0x64);
        }
    }
    if ((func_800460EC() == 2) && (func_8004ECB4() != 0)) {
        if (D_800E6280.unk_110D == 0xFF) {
            func_8004E9F4(1);
            func_8004284C();
        }
        D_800E6280.unk_1104.u = 0;
    }
    func_8007EDF8();
    func_8004EC24();
}
