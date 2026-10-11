#include "common.h"
#include "game.h"

void week_day_main(void);
void week_day_exit(void);

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

void week_day(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        week_day_init();
        break;
    case 1:
        week_day_main();
        break;
    case 2:
        week_day_exit();
        break;
    case 10:
        vacation_day_init();
        break;
    default:
        func_80046500();
        break;
    }
    parameter_show();
    message_window_show();
    hizuke_show();
    func_80065B0C(1);
    func_800578F4(0);
    func_80066C08(1);
    if (D_8011F3FF[0] & 0x80) {
        func_80067870();
    }
}

void week_day_exit(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        week_day_exit0();
    }
}

u8 week_day_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        return func_80073AD8();
    case 1:
        return func_800741B8();
    case 2:
        return func_8007437C();
    }
}

void week_day_main(void) {
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_110A == 0) {
        week_day_main0();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", get_weekly_bg_sector);

s32 func_80073A40(void) {
    s32 ret;

    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3 && !(*(u16 *)&D_800E6280.unk_0F6.b[0] & 1) && (D_800E6280.unk_F68.w & 0xF) == 7) {
        ret = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);
    }
    return ret;
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073AD8);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073CB0);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_800741B8);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007437C);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", week_day_main0);

void week_day_exit0(void) {
    D_800E6280.unk_71C++;
    func_80048F64(0x60);
    func_80048F64(0x61);
    func_80042878(0x31);
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", parameter_up_down);

u8 vacation_day_init(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        return func_80074DE0();
    case 1:
        return func_80074F24();
    case 2:
        return func_8007505C();
    }
}

s32 func_80074D28(void) {
    s32 ret;

    if ((D_800E6280.unk_69C[0].unk_00 == 0x25) || (D_800E6280.unk_69C[0].unk_00 == 0x42)) {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    } else if (((D_800E6280.unk_0F4.b[1] & 0xF) == 3) && ((D_800E6280.unk_F68.w & 0xF) == 7)) {
        ret = dec_bg_cd_read(D_800B5960[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(func_80075FA0(), 0);
    }
    return ret;
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80074DE0);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80074F24);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007505C);
