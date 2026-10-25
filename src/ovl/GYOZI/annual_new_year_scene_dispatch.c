/* annual_new_year_scene_dispatch.c: ; src/ovl/GYOZI/80134520.c:36-52 initializes and enters the New Year scene, including its title label. Original object 0x80134520-0x80134C10 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[13])();
} FnTbl13; /* size 0x34 */
extern FnTbl13 D_80145ECC;

void annual_new_year_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl13 tbl;

    tbl = D_80145ECC;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_new_year_resource_pointer_setup(void) {
    func_800504CC(1, 0xB678, 0xB65A, 0xC2FD, 0xC2D9, 0xC2D4);
    func_8004DE1C();
}

void annual_new_year_conditional_sound(void) {
    if (func_80050AB8() == 1) {
        func_80086AB0(0x200);
        func_8004DE1C();
    }
}

void annual_new_year_scene_resource_command(void) {
    func_80051DD8(0x33, 0x80197000, 0xA400);
    annual_mode_resource_addresses_init();
    func_8004DE1C();
}

void annual_new_year_scene_state_init(void) {
    s16 i;

    D_800F5AAE |= 8;
    func_8004EDE0(1, 0);
    func_80054864(1);
    func_8004C670();
    func_8005493C(0);
    func_8004EDF4(1);
    func_80053EFC();
    func_8005ABC0();
    func_800BCE10(&D_800D92A0, "正月");
    D_800F6412 = 0;
    D_800F6458 = 1;
    func_8009068C();
    D_800F53DA = 0x80;
    D_801317EB = 0;
    D_800C51C4 = 0;
    func_80089200();
    func_800744A0();
    func_80074950();
    func_8008FC10();
    annual_mode_resource_addresses_init();
    func_8008F618(0);
    D_80145EB4 = 0;
    D_80145EB8 = 0;
    D_80145EC8 = 0;
    for (i = 0; i < 0xB; i++) {
        D_800D9388[i] = ((GyoziBits32 *)D_800F53A0.girl[i].unk_10)->b1 >= 1;
    }
    func_8004DE1C();
}

void annual_new_year_audio_command_play_4107(void) {
    func_8008A0D4(0x4107);
    func_8004DE1C();
}

void annual_eligible_girl_select(void) {
    s32 girl_index;
    u8 best_rank_girl;
    u8 best_girl_rank;
    u8 candidate_girl_rank;
    GyoziGirl *best_girl_record;

    best_rank_girl = 0xFF;
    best_girl_rank = 4;
    for (girl_index = 0; girl_index < 11; girl_index++) {
        /* FAKE: empty reads order best_rank_girl ($s3) and 11 ($s4) ahead of &D_800F53A0 ($s5) and 56 ($s6). T-24128. */
        if (best_rank_girl) {}
        if (girl_index == 11) {}
        if (((GyoziBits32 *)D_800F53A0.girl[girl_index].unk_10)->b1 != 0) {
            candidate_girl_rank = func_8005E0E0(girl_index);
            /* FAKE: empty read keeps `move $v1, $v0` before `andi $t9, $v1, 0x7F`. T-24128. */
            if (candidate_girl_rank) {}
            candidate_girl_rank &= 0x7F;
            if (candidate_girl_rank < best_girl_rank ||
                (candidate_girl_rank == best_girl_rank &&
                 (best_girl_record = &D_800F53A0.girl[best_rank_girl],
                  best_girl_record->unk_0A + best_girl_record->unk_06 <
                      D_800F53A0.girl[girl_index].unk_0A + D_800F53A0.girl[girl_index].unk_06))) {
                best_rank_girl = girl_index;
                best_girl_rank = candidate_girl_rank;
            }
        }
    }
    candidate_girl_rank = func_8005E0E0(best_rank_girl) & 0x7F;
    if (best_rank_girl != 0xFF && candidate_girl_rank < 2U) {
        /* FAKE: `(best_rank_girl & 0) +` keeps cfe'candidate_girl_rank CVT J L narrowing so uopt emits `andi $a0, $s3, 0xff` (T-24122). */
        D_800F62CF = (best_rank_girl & 0) + best_rank_girl;
        func_8008F618(D_800F62CF);
        annual_girl_dialogue_tables_select();
        D_80145EB4 = 0;
        D_80145EB8 = 0;
        D_80145EC4 = 1;
        func_8004DEAC(1);
    } else {
        D_80145EC4 = 0;
        func_8004DEAC(2);
    }
}

void annual_girl_dialogue_tables_select(void) {
    u32 temp_v0;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_v0 = (u8)func_8005E0E0(D_800F62CF);
    switch (D_800F62CF) {
    case 0:
        D_80145EA8 = D_80145E08;
        D_80145EAC = D_80145E40;
        D_80145EB0 = D_80145E78;
        return;
    case 1:
        D_80145EA8 = D_80145E18;
        D_80145EAC = D_80145E50;
        D_80145EB0 = D_80145E88;
        return;
    case 2:
        D_80145EA8 = D_80145E0C;
        D_80145EAC = D_80145E44;
        D_80145EB0 = D_80145E7C;
        return;
    case 3:
        D_80145EA8 = D_80145E10;
        D_80145EAC = D_80145E48;
        D_80145EB0 = D_80145E80;
        return;
    case 4:
        D_80145EA8 = D_80145E2C;
        D_80145EAC = D_80145E64;
        D_80145EB0 = D_80145E9C;
        return;
    case 5:
        D_80145EA8 = D_80145E1C;
        D_80145EAC = D_80145E54;
        D_80145EB0 = D_80145E8C;
        return;
    case 6:
        D_80145EA8 = D_80145E24;
        D_80145EAC = D_80145E5C;
        D_80145EB0 = D_80145E94;
        if ((temp_v0 & 0x7F) < 2U) {
            D_80145EA8 = D_80145E20;
            D_80145EAC = D_80145E58;
            D_80145EB0 = D_80145E90;
        }
        return;
    case 7:
        D_80145EA8 = D_80145E14;
        D_80145EAC = D_80145E4C;
        D_80145EB0 = D_80145E84;
        return;
    case 8:
        D_80145EA8 = D_80145E04;
        D_80145EAC = D_80145E3C;
        D_80145EB0 = D_80145E74;
        return;
    case 9:
        D_80145EA8 = D_80145E28;
        D_80145EAC = D_80145E60;
        D_80145EB0 = D_80145E98;
        return;
    case 10:
        D_80145EA8 = D_80145E30;
        D_80145EAC = D_80145E68;
        D_80145EB0 = D_80145EA0;
    }
}
