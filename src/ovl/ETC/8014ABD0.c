#include "common.h"
#include "ovl/ETC.h"

void func_8014ABD0(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_8014AE20();
        break;
    case 1:
        func_8014B750();
        break;
    case 2:
        func_8014B978();
        break;
    case 3:
        func_8014BB94();
        break;
    case 4:
        func_8014C018();
        break;
    case 5:
        func_8014C250();
        break;
    case 6:
        func_8014C2A4();
        break;
    }
    func_8006BA40();
    func_80064DEC();
    func_80064F48();
    func_80066334();
    func_80066C08(0);
    func_800578F4(1);
    func_80067870();
}

void func_8014ACB4(void) {
    s32 temp_v0;
    s32 sp28;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80048E78();
    func_800438F0(1);
    func_80048390();
    func_80041584();
    func_8006BC28(0);
    func_800649D4();
    func_80064FA4(0);
    func_80063930(0);
    func_80064E84();
    func_8004E58C();
    func_80065F34(0);
    func_80057390(0);
    temp_v0 = func_8005742C(0x4055, 1);
    sp28 = temp_v0;
    if (temp_v0 != -1) {
        if ((temp_v0 == 0) || (temp_v0 == 1)) {
            func_8004284C();
            func_8004284C();
            func_80057418(1, sp28);
        }
    } else {
        func_8004284C();
    }
}

void func_8014AD94(void) {
    if (cd_read_sync_status() & 1) {
        dec_bg_show_set(1, func_8005751C(1));
        func_8004284C();
    }
}

void func_8014ADDC(void) {
    func_8006612C("進路指導");
    func_8004EA98();
    func_8004EAD4(0);
    func_80057390(0x80);
    func_80042808();
}

s32 func_8014AE20(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014ACB4();
        break;
    case 1:
        func_8014AD94();
        break;
    case 2:
        func_8014ADDC();
        break;
    }
}

s32 func_8014AE8C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80150EE0, 0, 0xF);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        func_8004284C();
        break;
    }
}

s32 func_8014AF28(void) {
    s32 index;
    s32 count;

    D_80151870 = 0;
    D_80151874 = 0;
    D_80151878 = 0;
    count = 0;
    D_8015186C = 0;
    /* The nine contiguous parameter records start at GameState +0xFC. */
    for (index = 1; index != 8; index++) {
        func_800AE0B0("param %d %d \n", index, ((GsRec0FC *)((u8 *)&D_800E6280 + 0xFC))[index].unk_02);
        if (((GsRec0FC *)((u8 *)&D_800E6280 + 0xFC))[index].unk_02 >= 0x65) {
            count++;
        }
    }
    if (D_800E6280.unk_100.unk_02 < 0x65) {
        D_8015186C += 1;
    }
    if (D_800E6280.unk_104.unk_02 < 0x65) {
        D_8015186C++;
    }
    if (D_800E6280.unk_108.unk_02 < 0x65) {
        D_8015186C++;
    }
    if (D_800E6280.unk_10C.unk_02 < 0x65) {
        D_80151870 += 1;
    }
    if (D_800E6280.unk_118.unk_02 < 0x65) {
        D_80151870 += 2;
    }
    if (D_800E6280.unk_110.unk_02 < 0x65) {
        D_80151874 += 1;
    }
    if (D_800E6280.unk_114.unk_02 < 0x65) {
        D_80151878 += 1;
    }
    return count;
}

s32 func_8014B0D0(void) {
    s32 var_s7;
    u8 *var_s1;
    u8 temp_v0;
    s32 var_s2;
    u8 sp70[16];
    s16 sp50[16];
    s32 var_s0;

    func_80053418(sp70, sp50);
    var_s2 = 0x3C;
    var_s7 = 0;
    for (var_s0 = 1, var_s1 = &sp70[1]; var_s0 != 11; var_s0++, var_s1++) {
        func_800AE0B0("%d %d \n", var_s0, *var_s1);
        temp_v0 = *var_s1;
        if ((var_s2 < temp_v0) && D_800E6280.unk_1BC[var_s0].unk_0C.f.b1) {
            var_s7 = var_s0;
            var_s2 = temp_v0;
        }
    }
    return var_s7;
}

void func_8014B1B4(void) {
    GameState *g = &D_800E6280;
    s32 sp28;
    s32 temp_v0;
    s32 temp_v0_2;

    switch (g->unk_110D) {
    case 0x0:
        temp_v0 = func_8014AF28();
        if (temp_v0 == 7) {
            g->unk_110D = 0x10;
            return;
        }
        if (temp_v0 == 0) {
            g->unk_110D = 0x20;
            return;
        }
        g->unk_110D = 0x30;
        return;
    case 0x10:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(&D_80151674, 0, 0xF);
        g->unk_110D += 1;
        return;
    case 0x11:
        xa_wait();
        return;
    case 0x12:
        func_80042808();
        return;
    case 0x20:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(&D_801516F0, 0, 0xF);
        g->unk_110D += 1;
        return;
    case 0x21:
        xa_wait();
        return;
    case 0x22:
        g->unk_110D = 0x40;
        return;
    case 0x30:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(&D_801516BC, 0, 0xF);
        g->unk_110D += 1;
        return;
    case 0x31:
        xa_wait();
        return;
    case 0x32:
        k_sub_reset();
        if (D_8015186C == 0) {
            g->unk_110D += 2;
        } else if (D_8015186C == 1) {
            gnsx(g->unk_0D4);
            sndisp(&D_80151738, 0, 0xF);
            g->unk_110D += 1;
        } else if (D_8015186C == 2) {
            gnsx(g->unk_0D4);
            sndisp(&D_80151758, 0, 0xF);
            g->unk_110D += 1;
        } else {
            gnsx(g->unk_0D4);
            sndisp(&D_80151778, 0, 0xF);
            g->unk_110D += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x33:
        xa_wait();
        return;
    case 0x34:
        k_sub_reset();
        if (D_80151870 == 0) {
            g->unk_110D += 2;
        } else if (D_80151870 == 1) {
            gnsx(g->unk_0D4);
            sndisp(&D_801517B4, 0, 0xF);
            g->unk_110D += 1;
        } else if (D_80151870 == 2) {
            gnsx(g->unk_0D4);
            sndisp(&D_801517D4, 0, 0xF);
            g->unk_110D += 1;
        } else {
            gnsx(g->unk_0D4);
            sndisp(&D_80151794, 0, 0xF);
            g->unk_110D += 1;
        }
        get_k_work(3)[4] = 1;
        return;
    case 0x35:
        xa_wait();
        return;
    case 0x36:
        k_sub_reset();
        if (D_80151874 != 0) {
            gnsx(g->unk_0D4);
            sndisp(&D_801517F4, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->unk_110D += 1;
            return;
        }
        g->unk_110D += 2;
        return;
    case 0x37:
        xa_wait();
        return;
    case 0x38:
        k_sub_reset();
        if (D_80151878 != 0) {
            gnsx(g->unk_0D4);
            sndisp(&D_80151810, 0, 0xF);
            get_k_work(3)[4] = 1;
            g->unk_110D += 1;
            return;
        }
        g->unk_110D += 2;
        return;
    case 0x39:
        xa_wait();
        return;
    case 0x3A:
        g->unk_110D = 0x40;
        return;
    case 0x40:
        temp_v0_2 = func_8014B0D0();
        sp28 = temp_v0_2;
        if (temp_v0_2 == 0) {
            g->unk_110D = 0x50;
            return;
        }
        k_sub_reset();
        gnsx(g->unk_0D4);
        g->unk_F5F = (s8) sp28;
        sndisp(&D_80151844, 0, 0xF);
        g->unk_110D += 1;
        return;
    case 0x41:
        xa_wait();
        return;
    case 0x42:
        g->unk_110D = 0x50;
        return;
    case 0x50:
        k_sub_reset();
        gnsx(g->unk_0D4);
        sndisp(&D_8015182C, 0, 0xF);
        g->unk_110D += 1;
        return;
    case 0x51:
        xa_wait();
        return;
    case 0x52:
        func_80042808();
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014B750(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014AE8C();
        break;
    case 1:
        func_8014B1B4();
        break;
    }
}

void func_8014B7A4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(&D_800E6280.unk_0D4[0]);
        func_80050E8C(&D_80150EFC, 0, 0xF);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        func_80051DBC();
        return;
    case 2:
        func_8004EAAC();
        func_80050DFC(&D_800E6280.unk_0D4[0]);
        func_80050E8C(&D_80150F38, 0, 0x1F);
        D_800E6280.unk_110D += 1;
        /* fallthrough */
    case 3:
        func_80051DBC();
        return;
    case 4:
        func_8004EAAC();
        func_80050C24(0, 3);
        func_80050D78(&D_80150F58, &D_80150F6C, &D_80150F80, 3);
        func_8004EAD4(3);
        D_800E6280.unk_1093 = 0;
        func_8004284C();
        /* fallthrough */
    default:
        return;
    }
}

s32 func_8014B8CC(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
        case 0:
            func_80042908(4);
            break;
        case 1:
            func_80042908(5);
            break;
        case 2:
            func_80042908(3);
            break;
        }
    }
}

s32 func_8014B978(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014B7A4();
        break;
    case 1:
        func_8014B8CC();
        break;
    }
}

s32 func_8014B9CC(void) {
    s32 sp34; /* FAKE: unused local, the original frame is 8 bytes larger (T-3330 slot rule); source unknown. T-6040 */
    s32 sp30;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80150F98, 0, 0xF);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        xa_wait();
        break;
    case 2:
        k_sub_reset();
        menu_girl_taku_set(0, &sp30);
        func_8004E884(sp30);
        D_800E6280.unk_1093 = 0;
        func_8004284C();
        break;
    }
}

void func_8014BA8C(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if ((D_800E6280.unk_F88 & 0x20) && D_800E6280.unk_1093 != -1) {
        k_sub_reset();
        gnsx(&D_800E6280.unk_0D4);
        sndisp(D_80151358[D_800E6280.unk_1093], 0, 0xF);
        func_8004284C();
    }
}

s32 func_8014BB34(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        xa_wait();
        break;
    case 1:
        func_80042908(2);
        D_800E6280.unk_110D = 2;
        break;
    }
}

s32 func_8014BB94(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014B9CC();
        break;
    case 1:
        func_8014BA8C();
        break;
    case 2:
        func_8014BB34();
        break;
    }
}

void func_8014BC00(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp((u8 *) &D_80150F58, 0, 0xF);
        D_800E6280.unk_0F6.b[1] = 0;
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(&D_80151398, 0, 0x1F);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(D_801514A4[func_8014BDE0() & 3], 0, 0xE);
        func_80052060((s32) &D_801514B4, (s32) &D_801514D4, func_8014BDE0() & 3, 0xE);
        D_800E6280.unk_110D += 1;
        return;
    case 5:
        func_8005215C();
        return;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80151664[func_8014BDE0() & 3], 0, 0xF);
        D_800E6280.unk_110D += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        func_80042908(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014BDE0(void) {
    s32 pad; /* FAKE: unused local, makes the frame 0x38 and puts the spill at sp+0x2C as in the original; real source unknown. T-4010 */
    s32 sum;
    s32 n;

    sum = 0;
    n = 0;
    if (D_800E6280.unk_100.unk_02 >= 0x79) {
        n = 1;
        sum = D_800E6280.unk_100.unk_02 - 0x78;
    }
    if (D_800E6280.unk_104.unk_02 >= 0x79) {
        n += 1;
        sum += D_800E6280.unk_104.unk_02;
        sum -= 0x78;
    }
    if (D_800E6280.unk_108.unk_02 >= 0x79) {
        n += 1;
        sum += D_800E6280.unk_108.unk_02;
        sum -= 0x78;
    }
    if ((n == 3) && (sum >= 0xC9)) {
        n = 3;
    } else if (n != 0) {
        n -= 1;
    }
    func_800AE0B0("thl %d \n", n);
    return 3 - n;
}

s32 func_8014BE98(void) {
    s32 n;

    n = -func_8014BDE0() * 2 + 6;
    if (D_800E6280.unk_110.unk_02 >= 0x3D) {
        n += 1;
    }
    if (D_800E6280.unk_114.unk_02 >= 0x3D) {
        n += 1;
    }
    if (n >= 7) {
        return 0;
    }
    if (n >= 5) {
        return 1;
    }
    if (n >= 3) {
        return 2;
    }
    return 3;
}

/* The fallthrough s32 form preserves the selector/result copies. T-10020. */
s32 func_8014BF2C(void) {
    s32 result;
    GameState *state;

    state = &D_800E6280;
    switch (state->unk_110D) {
    case 0:
        result = dec_bg_cd_read(0x408D, 1);
        if (result != -1) {
            if (result == 0 || result == 1) {
                D_800E6280.unk_110D += 2;
                dec_bg_show_set(1, result);
            }
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            dec_bg_show_set(1, func_8005751C(1));
            D_800E6280.unk_110D += 1;
        }
        break;
    case 2:
        func_8004284C();
        break;
    }
}

s32 func_8014C018(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014BF2C();
        break;
    case 1:
        func_8014BC00();
        break;
    }
}

void func_8014C06C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp((u8 *) &D_80150F6C, 0, 0xF);
        D_800E6280.unk_0F6.b[1] = 1;
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(&D_80151398, 0, 0x1F);
        D_800E6280.unk_110D += 1;
        return;
    case 3:
        xa_wait();
        return;
    case 4:
        k_sub_reset();
        gnsx((u8 *)"先生");
        sndisp(D_801515E4[func_8014BE98() & 3], 0, 0xE);
        func_80052060((s32) &D_801515F4, (s32) &D_80151614, func_8014BE98() & 3, 0xE);
        D_800E6280.unk_110D += 1;
        return;
    case 5:
        func_8005215C();
        return;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80151664[func_8014BE98() & 3], 0, 0xF);
        D_800E6280.unk_110D += 1;
        return;
    case 7:
        xa_wait();
        return;
    case 8:
        func_80042908(6);
        /* fallthrough */
    default:
        return;
    }
}


s32 func_8014C250(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014BF2C();
        break;
    case 1:
        func_8014C06C();
        break;
    }
}

void func_8014C2A4(void) {
    func_80072B5C(1);
}
