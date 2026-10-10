#include "common.h"
#include "game.h"

void schedule_init(void) {
    if (D_800E6280.unk_1109 == 0) {
        _schedule_init();
    }
    func_800578F4(get_last_gamen_mode());
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", _schedule_init);

void last_date_spot_timer_dec(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_73C[i] != 0) {
            D_800E6280.unk_73C[i] -= 1;
        }
    }
}

void restore_bgm(void) {
    if (func_8004480C() == 0) {
        func_8004500C(0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CD14);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CDD4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CE84);

void func_8006CF38(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        *(s32 *)&D_800E6280.unk_69C[i] = 0;
        D_800E6280.unk_69C[i].unk_00 = 0xFF;
    }
    D_800E6280.unk_71C = 0;
}

void func_8006CF80(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.unk_1BC[i].unk_0C.b[0] &= 0xFFFE;
    }
}

void func_8006D00C(void) {
    if (D_800E6280.unk_03F == 9) {
        D_800E6280.unk_0F6.b[0] &= 0xFFFB;
    }
}

void func_8006D038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_72C[i] == 0x23 || D_800E6280.unk_72C[i] == 0x24) {
            D_800E6280.unk_72C[i] = 0;
        }
        D_800E6280.unk_74C[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);

void func_8006D4A0(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280.unk_66C[i].unk_03 &= 0xFFFE;
    }
}

void func_8006D52C(s32 *arg0, s32 arg1, s32 arg2) {
    s32 i;

    if (birth_day_check_days(0, D_800E6280.unk_03F + arg1, arg2) == 1) {
        D_800E6280.unk_69C[*arg0].unk_00 = 0x32;
        D_800E6280.unk_69C[*arg0].unk_01 = 0;
        *arg0 += 1;
    }
    for (i = 1; i != 0xB; i++) {
        if (((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b1 && ((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b6) {
            if (birth_day_check_days(i, D_800E6280.unk_03F + arg1, arg2) == 1) {
                D_800E6280.unk_69C[*arg0].unk_00 = 0x32;
                D_800E6280.unk_69C[*arg0].unk_01 = i;
                *arg0 += 1;
            }
        }
    }
    if ((D_800E6280.unk_0F4.b[1] & 0xF) != 3) {
        if (birth_day_check_days(0xF, D_800E6280.unk_03F + arg1, arg2) == 1) {
            D_800E6280.unk_69C[*arg0].unk_00 = 0x33;
            *arg0 += 1;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D6E0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D7EC);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006EA40);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006EB64);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006EEC4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006F218);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006F524);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006FB4C);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", geko_judge);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", date_sasoi_judge);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", schedule_girls_gakko);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", yuukou_down);

void syoushin_up(void) {
    s32 i;

    for (i = 0; i < 0xB; i++) {
        if (((CharFlags *)&D_800E6280.unk_1BC[i].unk_0C)->b1 && (s32)get_h_yuukou(i) < 0xA) {
            D_800E6280.unk_1BC[i].unk_0A += 5;
        }
    }
}

void schdeule_girls_param(void) {
    if (D_800E6280.unk_03D == 0) {
        yuukou_down();
        syoushin_up();
    }
    func_8004284C();
}

void func_80070ECC(void) {
    func_80072338();
    if (func_800460EC() & 4) {
        func_80045414(9, 0, 0);
    }
    func_80042878(0x31);
}

void func_80070F14(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80070F80();
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_800578F4(get_last_gamen_mode());
    func_80066C08(1);
}

void func_80070F80(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8007132C();
        break;
    case 1:
        func_80070FD0();
        break;
    }
}
void func_80070FD0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80067438();
        D_800E6280.unk_110D++;
    } else if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_80042878(0x63);
    }
}

void func_80071038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_02 > 0x80) {
            D_800E6280.unk_1BC[i].unk_02 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_02 < 0) {
            D_800E6280.unk_1BC[i].unk_02 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_06 > 0x80) {
            D_800E6280.unk_1BC[i].unk_06 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_06 < 0) {
            D_800E6280.unk_1BC[i].unk_06 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A > 0x80) {
            D_800E6280.unk_1BC[i].unk_0A = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0) {
            D_800E6280.unk_1BC[i].unk_0A = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0x32) {
            D_800E6280.unk_1BC[i].unk_0C.b[2] &= 0xFFFE;
            D_800E6280.unk_1BC[i].unk_0C.b[2] &= 0xFD;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80071110);

void func_80071280(void) {
    s32 i;

    if (D_800E6280.unk_03F == 5 && D_800E6280.unk_040 == 6) {
        for (i = 0; i < 11; i++) {
            D_800E6280.unk_1BC[i].unk_0C.b[0] &= 0xFFEF;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8007132C);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800722C4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072338);

void func_8007259C(u8 arg0) {
    D_800E6280.unk_71E = arg0;
    D_800E6280.unk_71D = arg0;
}

void func_800725B0(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_800726F0();
        break;
    case 1:
        D_800E6280.unk_71C += 1;
        func_80042878(0x31);
        break;
    }
    if (D_8011F113 & 0x80) {
        hizuke_show();
    }
    if (D_8011ED17 & 0x80) {
        message_window_show();
    }
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
    if (*D_8011F3FF & 0x80) {
        func_80067870();
    }
    if (D_8011ECD3 & 0x80) {
        func_8006BA40();
    }
    func_800578F4(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(0);
}
INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800726F0);

void func_800728A4(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80072944();
        break;
    case 1:
        D_800E6280.unk_71C += 1;
        func_80042878(0x31);
        break;
    }
    func_800578F4(get_last_gamen_mode());
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
}
INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072944);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072998);

void func_80072B20(void) {
    if (func_80044E8C() == 1) {
        func_8004500C(0, 0);
        func_80042808();
    }
}

s32 func_80072B5C(s32 arg0) {
    if (D_800B3C6C != 0) {
        func_80042878(0x22);
        return 0;
    }
    if (arg0 == 1) {
        func_8004500C(0, 0);
    }
    D_800E6280.unk_71C++;
    func_80042878(0x31);
    /* no return on this path: the original leaves $v0 as the call result */
}

void func_80072BC0(void) {
    switch (D_800E6280.unk_1109) {
    case 0x0:
        func_80072C68();
        break;
    case 0x1:
        func_80072CA0();
        break;
    default:
    case 0xFF:
        func_800737A0();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
    func_800578F4(get_last_gamen_mode());
}
void func_80072C68(void) {
    parameter_show_init();
    hizuke_init();
    message_window_init();
    func_80042808();
}

void func_80072CA0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80072D70();
        return;
    case 1:
        func_80073198();
        return;
    case 2:
        func_800732F8();
        return;
    case 3:
        func_800733D8();
        return;
    case 5:
        func_800734C4();
        return;
    case 6:
        func_800735B8();
        return;
    case 7:
        func_800736AC();
        return;
    default:
        func_80042908(0xFF);
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072D70);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80073198);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800732F8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800733D8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800734C4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800735B8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800736AC);
