/* FAKE: s32 return view keeps count in $v1 across the call in func_801352B0 because void return lets uopt colour count into $v0. T-24091 */
#define SHUGAKU_OVERRIDE_func_801343B0
#include "common.h"
#include "ovl/SHUGAKU.h"

s32 func_801343B0(void);

void func_80134880(void) {
    D_8013C190 = (u8 *)0x801EA87C;
    D_8013C194 = (u8 *)0x801EA89C;
    D_8013C198 = (u8 *)0x801EA940;
    D_8013C19C = *(s16 *)0x801EA968;
    D_8013C1A0 = (u8 *)0x801B0000;
    D_8013C1A4 = (u8 *)0x801B2000;
    D_8013C1A8 = (u8 *)0x801B6000;
    D_8013C1AC = (u8 *)0x801BA000;
    D_8013C1B0 = (u8 *)0x801BE000;
    D_8013C1B4 = (u8 *)0x801C2000;
    D_8013C1B8 = (u8 *)0x801C6000;
}

void func_80134930(void) {
    if (D_800E6280.saved_stage == 3) {
        func_80134970();
        return;
    }
    func_80134B94();
}

void func_80134970(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    ShFnTbl25 tbl;

    tbl = D_8013C1C4;
    idx = D_800E6280.step;
    tbl.f[idx](0x80);
}

void func_801349EC(void) {
    D_800E6280.current_girl = D_800E6280.date_girl;
    func_800847B8(D_800E6280.date_girl);
    func_8004284C();
}

void func_80134A20(void) {
    if (D_8013C1BC != 0) {
        normal_date_move_place();
        return;
    }
    if (D_800E6280.substep == 0) {
        dec_bg_show_switch(1);
        D_800E6280.substep += 1;
    }
    func_8007C8A4();
}

void func_80134A8C(void) {
    strcpy(D_800CA19C, "ホテル・廊下");
    func_801365F4();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    if (D_800B593C != 0) {
        D_8013C1BC = 1;
    } else {
        D_8013C1BC = 0;
    }
    func_8004284C();
}

void func_80134B2C(void) {
    func_80042908(D_800E6280.saved_stage);
    func_80042940(D_800E6280.saved_step);
}

void func_80134B64(void) {
    func_80042908((D_800E6280.saved_stage + 1) & 0xFF);
}

void func_80134B94(void) {
    u8 index; /* declared above table and reread after the call: same callback idiom as EVENT func_800FA6A0 (T-18000) */
    ShFnTbl41 table;

    table = D_8013C228;
    index = D_800E6280.step;
    table.f[index](0x80);
    index = D_800E6280.step;
    if (D_80085368 == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].draw_flags |= 0x80;
        ((DisplayActor44 *)D_8011ECD0)[98].brightness = ((DisplayActor44 *)D_8011ECD0)[96].brightness;
    }
    if (func_800853FC == table.f[index]) {
        ((DisplayActor44 *)D_8011ECD0)[98].brightness = ((DisplayActor44 *)D_8011ECD0)[96].brightness;
        if (((DisplayActor44 *)D_8011ECD0)[96].brightness < 9) {
            ((DisplayActor44 *)D_8011ECD0)[98].draw_flags &= 0xFF7F;
        }
    }
}

void func_80134C98(void) {
    if (((D_800E6280.girls[0].birthday.w & 0xF) == D_800E6280.month) && (((u32) (D_800E6280.girls[0].birthday.w << 0x17) >> 0x1B) == D_800E6280.day)) {
        D_800E6280.saved_stage = 5;
        D_800E6280.saved_step = 0;
        D_800E6280.date_girl = D_800E6280.current_girl;
        D_800E6280.current_girl = 0;
        func_80042908(6);
        return;
    }
    if ((D_800E6280.month == (D_800E6280.player_birthday & 0xF)) && (D_800E6280.day == ((u32) (D_800E6280.player_birthday << 0x17) >> 0x1B))) {
        D_800E6280.saved_stage = 5;
        D_800E6280.saved_step = 0;
        D_800E6280.date_girl = D_800E6280.current_girl;
        D_800E6280.daily_event_bits |= 8;
        func_80042908(7);
        return;
    }
    func_8004284C();
}

s32 func_80134D8C(void) {
    func_801343B0();
    if ((D_800E6280.step_timer.w++ == 1) && (D_800CA2E8 == 2)) {
        D_80122D0C = 0;
        func_80067DD4();
    }
}

s32 func_80134DE8(void) {
    if (D_800E6280.substep == 0) {
        k_reset(0);
        func_8006612C("ホテル・廊下");
        D_800E6280.substep += 1;
    }
    func_801343B0();
    if (D_800CA2E8 == 1) {
        if (D_800E6280.step_timer.u++ == 1) {
            func_80044750(0x503);
        }
    }
}

void func_80134E74(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80134E9C(void) {
    cd_read_sectors(0x76, 0x801B0000, 0x8D47);
    func_80134880();
    func_8004284C();
}

void func_80134ED4(void) {
    D_800E6280.date_girl = D_800E6280.current_girl;
    D_800E6280.current_girl = 0xD;
    func_800847B8(0xD);
    func_8004284C();
}

void func_80134F18(void) {
    func_801365F4();
    D_8013BFFC = D_8013C5C4;
    D_8013C000 = D_8013C700;
    D_8013C004 = D_8013C83C;
    func_80134880();
    load_palette(D_8013C1A0, 0x11, 1, 2, 0);
    func_80084E90(D_8013C1A4, D_8013C1A8, D_8013C1AC, D_8013C1B0, D_8013C1B4, D_8013C1B8);
    func_800850D4(D_8013C194, D_8013C198, D_8013C190, D_8013C19C);
    D_80120698 = 8;
    D_80120654 = 8;
    D_801206AC = 3;
    /* D_80120650 and D_801206D8 are records 96 and 98 of one table: the loads stay behind the stores. T-9310, T-20900 */
    ((DisplayActor44 *)D_80120650)[2] = ((DisplayActor44 *)D_80120650)[0];
    ((DisplayActor44 *)D_80120650)[2].flags = 0x40;
    ((DisplayActor44 *)D_80120650)[2].unk_18 = 0x28;
    ((DisplayActor44 *)D_80120650)[2].unk_01 -= 1;
    D_800CA2E4 = 3;
    D_800CA2E8 = 0;
    D_800E6280.tpage_buf[5] = -1;
    D_800E6280.tpage_buf[6] = -1;
    D_800E6280.tpage_buf[7] = -1;
    D_800E6280.tpage_buf[8] = -1;
    D_800E6280.tpage_buf[9] = -1;
    D_80122D0C = 1;
    D_8013C1C0 = *(u8 *)&D_800E6280.voice_mode;
    func_8004284C();
}

void func_801350F4(void) {
    if (D_80122CDC != 0) {
        D_800E6280.step += 0x1D;
        D_800CA2DC += 1;
        return;
    }
    func_80085B3C(2, 0x20);
    func_8004284C();
    D_800CA2F4 = 1;
    D_800E6280.event_flags[0x20] = 1;
    D_800E6280.girls[D_800E6280.current_girl].tokimeki += 5;
    D_800E6280.girls[D_800E6280.current_girl].affection += 2;
    check_para_limit();
}

void func_801351CC(void) {
    func_80137C3C();
    if ((D_800E6280.girls[0].birthday.w & 0xF) == D_800E6280.month && ((u32) (D_800E6280.girls[0].birthday.w << 0x17) >> 0x1B) == D_800E6280.day) {
        D_800E6280.saved_stage += 1;
        D_800E6280.saved_step = 0;
        D_800E6280.date_girl = D_800E6280.current_girl;
        D_800E6280.current_girl = 0;
        func_80042908(6);
        return;
    }
    func_80042908((D_800E6280.saved_stage + 1) & 0xFF);
}

void func_80135278(void) {
    D_800E6280.voice_mode = D_8013C1C0;
    D_800CA2DC = 0x29;
    func_8004284C();
}

void func_801352B0(void) {
    s32 pad; /* FAKE: unused local above page and count gives the 0x38 frame and puts page at sp+0x30 and count at sp+0x2C; real source unknown. T-24091 */
    s32 page;
    s32 count;

    if (*(u8 *)&D_800E6280.voice_mode == 2) {
        *(u8 *)&D_800E6280.voice_mode = 1;
    }
    count = D_800E6280.step_timer.w;
    page = D_800E6280.step;
    if (!(D_800E6280.pad_pressed & 0x600060) || (D_801206FE < -0xBF)) {
        func_801343B0();
    }
    D_800E6280.step_timer.w = count + 1;
    if (D_801206FE >= -0xBF) {
        if (!(D_800E6280.step_timer.w & 1) && (check_end_k() != 0)
            && ((*(u8 *)&D_800E6280.voice_mode == 0) || (func_80046094() == 9))) {
            D_801206FE--;
        }
        D_8011ED5B &= 0x7F;
    }
    if (page != D_800E6280.step_timer.w) {
        if (D_801206FE >= -0xBF) {
            /* FAKE: u32 view prevents uopt from CSE-caching the s32 load of page across the branch, reproducing the reload from sp+0x30. T-24091 */
            D_800E6280.step = *(u32 *)&page;
            D_800CA2E4 = 9;
            D_800CA2E8 = 0;
        } else {
            D_800CA2E4 = 9;
            D_800CA2E8 = 1;
        }
    }
}
