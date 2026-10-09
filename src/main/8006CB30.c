#include "common.h"
#include "game.h"

void schedule_init(void) {
    if (D_800E7389 == 0) {
        _schedule_init();
    }
    func_800578F4(get_last_gamen_mode());
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", _schedule_init);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", last_date_spot_timer_dec);

void restore_bgm(void) {
    if (func_8004480C() == 0) {
        func_8004500C(0, 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CD14);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CDD4);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CE84);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CF38);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006CF80);

void func_8006D00C(void) {
    if (D_800E62BF == 9) {
        D_800E6376 &= 0xFFFB;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D038);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D138);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D4A0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8006D52C);

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

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", syoushin_up);

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

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80071280);

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

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80072B5C);

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

void func_800737A0(void) {
    func_80072B5C(0);
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(0);
    func_80067870();
    func_80066C08(1);
    func_800578F4(get_last_gamen_mode());
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", week_day);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", week_day_exit);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", week_day_init);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", week_day_main);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", get_weekly_bg_sector);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80073A40);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80073AD8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80073CB0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_800741B8);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8007437C);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", week_day_main0);

void week_day_exit0(void) {
    D_800E699C++;
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80042878(0x31);
}

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", parameter_up_down);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", vacation_day_init);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80074D28);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80074DE0);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_80074F24);

INCLUDE_ASM("asm/nonmatchings/main/8006CB30", func_8007505C);
