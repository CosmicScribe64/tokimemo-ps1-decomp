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

s32 get_weekly_bg_sector(void) {
    u8 x;
    u16 h;

    x = D_800E6280.unk_71C;
    if (D_800E6280.unk_69C[x].unk_01 != 4) {
        return D_800B6760[D_800E6280.unk_69C[x].unk_01];
    }
    h = D_800E6280.unk_0F4.h;
    return D_800B6780[h >> 0xC];
}

s32 func_80073A40(void) {
    s32 ret;

    if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3 && !(*(u16 *)&D_800E6280.unk_0F6.b[0] & 1) && (D_800E6280.unk_F68.w & 0xF) == 7) {
        ret = dec_bg_cd_read(D_800B5950[func_80066A2C()], 0);
    } else {
        ret = dec_bg_cd_read(get_weekly_bg_sector(), 0);
    }
    return ret;
}

u8 func_80073AD8(void) {
    s32 temp_v0;

    func_8007259C(0);
    if (D_800E6280.unk_03D != 0) {
        func_80041584();
        func_80042908(0xA);
        return 0;
    } else {
        switch (D_800E6280.unk_110D) {                       /* irregular */
        case 0:
            func_80048E78();
            set_dec_bri(0x80U);
            back_clear_switch(1);
            tpage_buf_clear();
            func_80041584();
            hizuke_init();
            func_8006509C();
            icon_disp_switch(1);
            func_8006CE84();
            parameter_show_init();
            func_8006BC28(0);
            parameter_disp_switch(1);
            message_window_init();
            func_8005AD1C();
            if (D_800E6280.unk_0F6.h & 1) {
                D_801217D0->unk_00 = 0x80000000;
            } else {
                func_8005AD70();
            }
            func_8004E58C();
            func_80065F34(0);
            k_disp_start(0);
            temp_v0 = func_80073A40();
            if (temp_v0 != -1) {
                if ((temp_v0 == 0) || (temp_v0 == 1)) {
                    D_800E6280.unk_110D = 2;
                    dec_bg_show_set(0, temp_v0);
                }
            } else {
                D_800E6280.unk_110D = 1;
            }
            break;
        case 1:
            if (func_800460CC() & 1) {
                dec_bg_show_set(0, func_8005751C(0));
                if (D_800B594C == -1) {
                    dec_bg_reset();
                    func_80073A40();
                } else {
                    D_800E6280.unk_110D = 2;
                }
            }
            break;
        default:
        case 2:
            func_8004284C();
            break;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_80073CB0);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_800741B8);

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007437C);

s32 week_day_main0(void) {
    s16 *state;
    s32 sp28;

    if (!(D_80120652 & 1)) {
        if (D_80120658 == 0) {
            state = &D_80120666;
            switch ((u32)(*state & 1)) {                      /* irregular */
            case 0:
                func_80044750(0x501);
                break;
            case 1:
                func_80044750(0x502);
                break;
            }
            sp28 = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
            parameter_up_down(sp28, 0, (s32) D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02);
            parameter_show_init();
            if (sp28 == 4) {
                D_800E6280.unk_66C[(D_800E6280.unk_0F4.h >> 12) % 11].unk_00 += 1;
            }
        }
        if (D_80120658++ >= 0xD) {
            func_80042808();
        }
    }
    if ((u32) D_800E6280.unk_1104.u >= 0x259U) {
        func_800452C4();
        func_80042808();
    }
}

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

u8 func_80074F24(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80076FDC();
        if ((D_800E6280.unk_0F4.b[1] & 0xF) == 3 && !(*(u16 *)&D_800E6280.unk_0F6.b[0] & 1) && (D_800E6280.unk_F68.w & 0xF) == 7) {
            func_80046318(9, 0x801E0000, 0x7D8A);
            D_800E6280.unk_110D = 1;
        } else if (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 != 4) {
            func_80046318(0x29, 0x801E0000, D_800B69E0[D_801230D0]);
            D_800E6280.unk_110D = 1;
        } else {
            func_80046318(0x29, 0x801E0000, D_800B6810[D_801230D0]);
            D_800E6280.unk_110D = 1;
        }
    } else if ((D_800E6280.unk_110D == 1) && (func_800460CC() & 1)) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800737A0", func_8007505C);
