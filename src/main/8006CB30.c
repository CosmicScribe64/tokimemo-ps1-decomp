#include "common.h"
#include "game.h"

void schedule_init(void) {
    if (D_800E7389 == 0) {
        _schedule_init();
    }
    func_800578F4(get_last_gamen_mode());
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", _schedule_init);

void last_date_spot_timer_dec(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280[0x73C + i] != 0) {
            D_800E6280[0x73C + i] -= 1;
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
        *(s32 *)(D_800E6280 + 0x69C + i * 4) = 0;
        *(D_800E6280 + 0x69C + i * 4) = 0xFF;
    }
    D_800E699C = 0;
}

void func_8006CF80(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280[0x1C8 + i * 0x38] &= 0xFFFE;
    }
}

void func_8006D00C(void) {
    if (D_800E62BF == 9) {
        D_800E6376 &= 0xFFFB;
    }
}

void func_8006D038(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (D_800E6280[0x72C + i] == 0x23 || D_800E6280[0x72C + i] == 0x24) {
            D_800E6280[0x72C + i] = 0;
        }
        D_800E6280[0x74C + i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);

void func_8006D4A0(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        D_800E6280[0x66F + i * 4] &= 0xFFFE;
    }
}

void func_8006D52C(s32 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    u8 *p;

    if (birth_day_check_days(0, D_800E62BF + arg1, arg2) == 1) {
        D_800E6280[*arg0 * 4 + 0x69C] = 0x32;
        D_800E6280[*arg0 * 4 + 0x69D] = 0;
        *arg0 += 1;
    }
    for (i = 1; i != 0xB; i++) {
        p = D_800E6280 + i * 0x38;
        if (((CharFlags *)(p + 0x1C8))->b1 && ((CharFlags *)(p + 0x1C8))->b6) {
            if (birth_day_check_days(i, D_800E62BF + arg1, arg2) == 1) {
                D_800E6280[*arg0 * 4 + 0x69C] = 0x32;
                D_800E6280[*arg0 * 4 + 0x69D] = i;
                *arg0 += 1;
            }
        }
    }
    if ((D_800E6375 & 0xF) != 3) {
        if (birth_day_check_days(0xF, D_800E62BF + arg1, arg2) == 1) {
            D_800E6280[*arg0 * 4 + 0x69C] = 0x33;
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
    u8 *p;

    p = D_800E6280;
    for (i = 0; i < 0xB; i++) {
        if (((CharFlags *)(p + 0x1C8))->b1 && (s32)get_h_yuukou(i) < 0xA) {
            *(s16 *)(p + 0x1C6) += 5;
        }
        p += 0x38;
    }
}

void schdeule_girls_param(void) {
    if (D_800E62BD == 0) {
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
    if (D_800E7389 == 0) {
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

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80070F80);

void func_80070FD0(void) {
    if (D_800E738D == 0) {
        func_80067438();
        D_800E738D++;
    } else if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_80042878(0x63);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80071038);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80071110);

void func_80071280(void) {
    s32 i;

    if (D_800E62BF == 5 && D_800E62C0 == 6) {
        for (i = 0; i < 11; i++) {
            D_800E6280[0x1C8 + i * 0x38] &= 0xFFEF;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8007132C);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800722C4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072338);

void func_8007259C(u8 arg0) {
    D_800E699E = arg0;
    D_800E699D = arg0;
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800725B0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800726F0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800728A4);

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
    D_800E699C++;
    func_80042878(0x31);
    /* no return on this path: the original leaves $v0 as the call result */
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072BC0);

void func_80072C68(void) {
    parameter_show_init();
    hizuke_init();
    message_window_init();
    func_80042808();
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072CA0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072D70);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80073198);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800732F8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800733D8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800734C4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800735B8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800736AC);
