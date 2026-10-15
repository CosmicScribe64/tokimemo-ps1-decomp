#include "common.h"
#include "ovl/ETC.h"

u8 *func_80140F50(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_80150128;
        }
        return D_8015012C;
    }
    return D_80150108;
}

u8 *func_80140F94(void) {
    if (D_800E6280.unk_F74 == 0) {
        if (D_800E6280.unk_F75 != 0) {
            return D_8015012C;
        }
        return D_80150128;
    }
    return D_80150124;
}

void func_80140FD8(void) {
    GameState *g = &D_800E6280;
    g->unk_1104.w += 1;
    switch ((u8) g->unk_110A) {
    case 0x0:
        func_801413FC();
        break;
    case 0x1:
        func_80141468();
        break;
    case 0x2:
        func_801415A4();
        break;
    case 0x3:
        func_80141700();
        break;
    case 0x4:
        func_8014175C();
        break;
    case 0x5:
        func_80141980();
        break;
    case 0x6:
        func_80141BE8();
        break;
    case 0xA0:
        func_80141154();
        break;
    case 0xF0:
        func_80141E10();
        break;
    case 0xFD:
        func_80142574();
        break;
    case 0xFE:
        func_80142390();
        break;
    case 0xFF:
        func_801421AC();
        break;
    }
    func_801428EC();
}


void func_80141154(void) {
    u8 temp;
    temp = D_800E6280.unk_110D;
    switch (temp) {
    case 0:
        func_8004E58C();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "未初期化のメモリーカードです。", 0);
        k_disp_start(1);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004E58C();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "初期化していいですか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_disp_start(5);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            if (func_80054108() == 1) {
                func_80042940(4);
            } else {
                func_80042940(2);
            }
        } else if (D_800E6280.unk_F88 & 0x40) {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 4:
        func_8004E58C();
        set_kanji_string(-0x78, 0x30, 0xF, (s32) "別のメモリーカードを", 0);
        set_kanji_string(-0x78, 0x3E, 1, (s32) "差し込み口１", 0);
        set_kanji_string(-0x78, 0x3E, 0xF, (s32) "　　　　　　にセットして、", 0);
        set_kanji_string(-0x78, 0x4C, 0xF, (s32) "　ボタンを押してください。", 0);
        set_kanji_string(-0x78, 0x4C, 1, func_80140F50(), 0);
        D_800E6280.unk_110D += 1;
        k_disp_start(5);
        break;
    case 5:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042940(4);
        }
        break;
    }
    func_800578F4(2);
}

void func_801413FC(void) {
    func_800AE0B0("データセーブ");
    if (D_800E6280.unk_1124 != 0) {
        func_8004482C();
        main_state_set(0x12);
        return;
    }
    func_8004E58C();
    func_80057390(0);
    D_800E6280.unk_03A = 0;
    func_8004284C();
}

s32 func_80141468(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        dec_bg_reset();
        cd_read_sectors(9U, 0x80162000, 0xA1A2);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D += 1;
            return;
        }
        return;
    case 2:
        func_800536AC(0x2C0, 0, 0x140, 0xF0);
        MoveSquare(0x2C0, 0x20, 0x140, 0xD0, 0x2C0, 0);
        func_8009C674(0);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        if (D_800E6280.unk_03A++ < 0x80U) {
            set_dec_bri(D_800E6280.unk_03A);
            func_800578F4(2);
            return;
        }
        func_8004284C();
        break;
    }
}

void func_801415A4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EA98();
        func_8004E788(-0x78, 0x30, 0, "今回のアルバムモードのデータを", 0);
        func_8004E788(-0x78, 0x3E, 0, "　ときめきメモリアル・システムファイル", 0);
        func_8004E788(-0x78, 0x4C, 0, "にセーブしますか。", 0);
        func_8004E788(-0x88, 0x5A, 0, D_801500F0, 0);
        func_8004E788(-0x34, 0x5A, 1, func_80140F50(), 0);
        func_8004E788(8, 0x5A, 0, D_8015010C, 0);
        func_8004E788(0x5C, 0x5A, 2, func_80140F94(), 0);
        func_8004EAD4(7);
        func_800AE0B0("データセーブメッセージ");
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if ((u32)D_800E6280.unk_1104.w >= 0x41U) {
            func_8004284C();
        }
        break;
    }
    func_800578F4(2);
}

void func_80141700(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004482C();
        main_state_set(0xC4);
    }
    func_800578F4(2);
}

void func_8014175C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x78, 0x30, 0, (s32) "メモリーカードをチェックしています。", 0);
        func_8004EAD4(1);
        D_800E6280.unk_110D += 1;
        func_80053CE0();
        break;
    case 1:
        if (func_80053F04() != 0) {
            if (D_800E6280.unk_1115 == 1) {
                func_80053D10();
                func_800AE0B0("EXIST_CARD!");
                D_800E6280.unk_110D += 1;
            } else if (D_800E6280.unk_1115 == 0xF1) {
                func_80053D10();
                func_800AE0B0("UNFORMAT CARD!");
                func_80042940(0xA0);
            } else if (D_800E6280.unk_1115 == 0xFF) {
                func_80053D10();
                func_800AE0B0("NO_CARD !");
                func_80042940(0xFF);
            }
        } else if ((u32) D_800E6280.unk_1120 >= 0x201U) {
            func_80053D10();
            func_800AE0B0("NO COMUNICATION!");
            func_80042940(0xFF);
        }
        break;
    case 2:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80054140() == 2) {
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if ((u32) D_800E6280.unk_1120 >= 0x201U) {
            func_80042940(0xFF);
        }
        break;
    case 4:
        if ((u8) D_800E8BEE != 0) {
            func_8004284C();
        } else {
            func_80042940(0xF0);
        }
        break;
    }
    func_800578F4(2);
}

void func_80141980(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x78, 0x30, 0, "ただいま、セーブ中です。", 0);
        func_8004EAD4(2);
        D_800E6280.unk_110D += 1;
        func_80053CE0();
        break;
    case 1:
        if (func_80054284() == 3) {
            func_80053D10();
            func_800AE0B0("LOAD SYSTEM OK");
            D_800E6280.unk_110D += 1;
        } else if ((u32)D_800E6280.unk_1120 >= 0x201U || D_800E6280.unk_1115 == 0xF0) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        if (func_800552DC(0) == -1) {
            func_80042940(0xFD);
        } else {
            func_8004284C();
        }
        break;
    }
    func_800578F4(2);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80140F50", func_80141ABC);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B383C;
extern s32 D_800E67BC;
extern u32 D_800E73A0;
extern u8 D_800E62BA;
extern u8 D_800E71DF;
extern u8 D_800E738D;
extern u8 D_800E7395;
/* end bulk_m2c declarations */
s32 func_80141BE8(void) {
    s32 temp_t3;
    u8 *temp_v0;
    u8 *temp_v0_2;

    switch (D_800E738D) {
    case 0:
        func_80053CE0();
        func_80141ABC();
        D_800E738D += 1;
        break;
    case 1:
        if (func_80054388() == 0x13) {
            func_80053D10();
            D_800E738D += 1;
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 2:
        func_800AE0B0("[C_GIRL]%d \n", D_800E71DF);
        temp_t3 = D_800E71DF & 0xF;
        if (*(&D_800E67BC + temp_t3) & 1) {
            temp_v0 = (temp_t3 * 8) + &D_800B383C;
            func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), temp_t3);
        } else {
            temp_v0_2 = (temp_t3 * 8) + D_800B37D4;
            func_80046290((*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), temp_t3);
        }
        func_80044750(0x300);
        D_800E738D += 1;
        break;
    case 3:
        if (func_800460EC() & 4) {
            D_800E738D += 1;
        }
        break;
    case 4:
        if (func_800460EC() & 2) {
            D_800E738D += 1;
        }
        break;
    case 5:
        D_800E62BA -= 1;
        if ((u8) D_800E62BA < 8U) {
            D_800E62BA = 0;
            func_8004482C();
            main_state_set(0xC4);
            D_800E71DF += 0x10;
        }
        set_dec_bri(D_800E62BA);
        break;
    }
    func_800578F4(2);
}

void func_80141E10(void) {
    GameState *g = &D_800E6280;
    switch (g->unk_110D) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルがありません。", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "（システムファイルがないと", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "アルバムモードのセーブはできません）", 0);
        k_sub_disp_start(3);
        g->unk_110D += 1;
        func_80053CE0();
        break;
    case 1:
        if ((u32) g->unk_1104.u >= 0x41U) {
            g->unk_110D += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成し、", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "アルバムモードをセーブしますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(6);
        g->unk_110D += 1;
        break;
    case 3:
        if (g->unk_F88 & 0x20) {
            g->unk_110D += 1;
        } else if (g->unk_F88 & 0x40) {
            func_8004482C();
            main_state_set(0xC4);
        }
        break;
    case 4:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成しています。", 0);
        set_kanji_string(-0x78, 0x3E, 0, (s32) "（アルバムモードのデータをセーブ後、", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "ファイルセレクト画面にて、", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "システムファイルを作り直してください）", 0);
        k_sub_disp_start(5);
        g->unk_110D += 1;
        break;
    case 5:
        if (func_80054694(0) == 1) {
            g->unk_110D += 1;
        } else {
            func_80042940(0xFE);
        }
        break;
    case 6:
        D_800E7D36 = g->unk_1118;
        *D_800E7D14 = 1;
        g->unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        g->unk_110D += 1;
        break;
    case 7:
        if (func_80054388() == 0x13) {
            func_80053D10();
            g->unk_110D += 1;
        } else if (((u32) g->unk_1120 >= 0x201U) || (g->unk_1115 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 8:
        func_80042940(5);
        break;
    }
    func_800578F4(2);
}

void func_801421AC(void) {
    u8 *state = &D_800E6280.unk_110D;

    switch (*state) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "メモリーカードを認識できません。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "差し込み口１のメモリーカードの", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "接続を確認してください。", 0);
        k_sub_disp_start(3);
        *state += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.u >= 0x41U) {
            *state += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "データのセーブを行ないますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(5);
        *state += 1;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042940(4);
        } else if (D_800E6280.unk_F88 & 0x40) {
            func_8004482C();
            main_state_set(0xC4);
        }
        break;
    }
    func_800578F4(2);
}

void func_80142390(void) {
    u8 *state = &D_800E6280.unk_110D;

    switch (*state) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "メモリーカードに空きブロックがありません。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "他の空きブロックのあるメモリーカードを", 0);
        set_kanji_string(-0x78, 0x5A, 0, (s32) "用意してください。", 0);
        k_sub_disp_start(3);
        *state += 1;
        break;
    case 1:
        if ((u32) D_800E6280.unk_1104.u >= 0x41U) {
            *state += 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "データのセーブを行ないますか？", 0);
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(5);
        *state += 1;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042940(4);
        } else if (D_800E6280.unk_F88 & 0x40) {
            func_8004482C();
            main_state_set(0xC4);
        }
        break;
    }
    func_800578F4(2);
}

void func_80142574(void) {
    GameState *g = &D_800E6280;
    switch (g->unk_110D) {
    case 0:
        k_sub_reset();
        set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルが壊れています。", 0);
        set_kanji_string(-0x78, 0x4C, 0, (s32) "システムファイルを作り直していいですか？", 0);
        k_sub_disp_start(3);
        g->unk_110D += 1;
        break;
    case 1:
        if ((u32) g->unk_1104.u >= 0x41U) {
            g->unk_110D += 1;
        }
        break;
    case 2:
        set_kanji_string(-0x88, 0x5A, 0, (s32) D_801500F0, 0);
        set_kanji_string(-0x34, 0x5A, 1, func_80140F50(), 0);
        set_kanji_string(8, 0x5A, 0, (s32) D_8015010C, 0);
        set_kanji_string(0x5C, 0x5A, 2, func_80140F94(), 0);
        k_sub_disp_start(6);
        g->unk_110D += 1;
        break;
    case 3:
        if (g->unk_F88 & 0x20) {
            k_sub_reset();
            set_kanji_string(-0x78, 0x30, 0, (s32) "システムファイルを作成しています。", 0);
            set_kanji_string(-0x78, 0x3E, 0, (s32) "（アルバムモードのデータをセーブ後、", 0);
            set_kanji_string(-0x78, 0x4C, 0, (s32) "ファイルセレクト画面のシステムを", 0);
            set_kanji_string(-0x78, 0x5A, 0, (s32) "クリックして、システムファイルを", 0);
            set_kanji_string(-0x78, 0x68, 0, (s32) "作り直してください）", 0);
            k_sub_disp_start(5);
            g->unk_110D += 1;
        } else if (g->unk_F88 & 0x40) {
            func_8004482C();
            main_state_set(0xC4);
        }
        break;
    case 4:
        if (func_80054704(0) == 1) {
            g->unk_110D += 1;
        } else {
            func_80042940(0xFF);
        }
        break;
    case 5:
        if (func_80054694(0) == 1) {
            g->unk_110D += 1;
        } else {
            func_80042940(0xFF);
        }
        break;
    case 6:
        D_800E7D36 = g->unk_1118;
        *D_800E7D14 = 1;
        g->unk_1118 = 0;
        func_80054E6C();
        g->unk_110D += 1;
        break;
    case 7:
        if (func_80054388() == 0x13) {
            func_80053D10();
            g->unk_110D += 1;
        } else if (((u32) g->unk_1120 >= 0x201U) || (g->unk_1115 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 8:
        func_80042940(5);
        break;
    }
    func_800578F4(2);
}

s32 func_801428EC(void) {
    s32 color;
    u8 *shade = &D_800E6280.unk_03A;
    color = *shade;
    color = color * 0x10101 + 0x141414;

    func_80049A40(-0x8C, 0x2C, 0x128, 0x40, 4, color, 0x81);
    dtd_on_tpage(0, 0, 4, 2, 0);
}

void func_80142974(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80143644();
        break;
    case 1:
        func_80143FE0();
        break;
    }
    if (func_800460EC() & 4) {
        if (D_800E6280.unk_F88 & 0x200000) {
            if (D_800E6280.unk_1124 != 0) {
                main_state_set(0x11);
            } else {
                main_state_set(0xC4);
            }
        } else if (D_800E6280.unk_F88 & 0x400000) {
            if (D_800E6280.unk_1124 != 0) {
                main_state_set(0x11);
            } else {
                main_state_set(0x91);
            }
        }
    }
}
void func_80142A4C(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_801217D0[i].unk_04 = i * 0x80 - 0x60;
        D_801217D0[i].unk_00 = 0x01000000;
        D_801217D0[i].unk_06 = -0x40;
        D_801217D0[i].unk_08 = 0x80;
        /* s16 view of the u16 member: the constant is then shared with unk_08 */
        *(s16 *)&D_801217D0[i].unk_0A = 0x80;
        D_801217D0[i].unk_0C = i + 0x15;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1FF;
        D_801217D0[i].unk_14 = 0;
        D_801217D0[i].unk_15 = 0;
        D_801217D0[i].unk_16 = 0;
        D_801217D0[i].unk_18 = 0;
        D_801217D0[i].unk_1A = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_800E6280.unk_10A5[i] = 0;
    }
}

void func_80142AE8(void) {
    s32 i;
    u8 v;
    u8 a;

    if (D_8015018C > 0) {
        for (i = 0; i < 2; i++) {
            v = D_801217D0[i].unk_14;
            a = v + 1;
            if (v < 0x80) {
                D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[i].unk_14 = a;
            }
        }
    }
    if (D_8015018C < 0) {
        for (i = 0; i < 2; i++) {
            v = D_801217D0[i].unk_14;
            a = v - 1;
            if (v != 0) {
                D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = D_801217D0[i].unk_14 = a;
            } else {
                D_801217D0[i].unk_00 = 0x80000000;
            }
        }
    }
}
