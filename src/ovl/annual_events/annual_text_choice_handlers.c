/* annual_text_choice_handlers.c: ; src/ovl/GYOZI/80134C10.c:25-56 displays and advances text-choice state. Original object 0x80134C10-0x80134FD0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80145F00;

void annual_text_choice_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80145F00;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_text_choice_sound_play_0603(void) {
    func_80086AB0(0x603);
    func_8004DE1C();
}

INCLUDE_RODATA("asm/ovl/GYOZI/data/annual_events/annual_text_choice_handlers.rodata", D_801458A0);

void annual_text_choice_static_text_install(void) {
    func_800BCE10(&D_800D92E0, &D_801458A0);
    func_8004DE1C();
}

void annual_text_choice_audio_command_play_40e5(void) {
    func_8008A0D4(0x40E5);
    func_8004DE1C();
}

void annual_text_choice_target_branch(void) {
    if (D_8012E66C != 0) {
        if ((D_800F62CF == 2) || (D_800F62CF == 7) || (D_800F62CF == 8) || (D_800F62CF == 9) || (D_800F62CF == 0xA)) {
            func_8008E310();
            return;
        }
        func_8008E2E0();
        return;
    }
    func_8004DE1C();
}

void annual_outcome_values_apply(void) {
    if (D_8012E66C != 0) {
        annual_new_year_talk_group += 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A -= 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 -= 1;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0E += 0x14;
    } else if (D_800F53A0.unk_F2F == 0) {
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 1;
    }
    func_8008FB00();
    func_8004DE1C();
}

void annual_text_choice_zero_route(void) {
    if (D_8012E66C == 0) {
        func_8004DEAC(3);
        return;
    }
    func_8004DE1C();
}

void annual_text_choice_voice_resource_by_girl(void) {
    switch (D_800F62CF) {
    case 0:
        func_80072734(0x5671);
        break;
    case 1:
        func_80072734(0x5D4C);
        break;
    case 2:
        func_80072734(0x5266);
        break;
    case 3:
        func_80072734(0x58E7);
        break;
    case 4:
        func_80072734(0x6643);
        break;
    case 5:
        func_80072734(0x6076);
        break;
    case 6:
        func_80072734(0x620B);
        break;
    case 7:
        func_80072734(0x5B30);
        break;
    case 8:
        func_80072734(0x501D);
        break;
    case 9:
        func_80072734(0x6481);
        break;
    case 10:
        func_80072734(0x688C);
        break;
    }
    func_8004DE1C();
}
