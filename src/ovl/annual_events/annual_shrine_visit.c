/* annual_shrine_visit.c: ; src/ovl/GYOZI/80135900.c:18-22 writes the shrine-visit title label. Original object 0x80135900-0x80135CA0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_80146060;

void annual_shrine_visit_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_80146060;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_shrine_visit_title_score_setup(void) {
    s16 v;

    func_800BCE10(&D_800D92A0, "初詣");
    if (D_80145F60 != 0) {
        v = 0;
    } else {
        v = 0xF;
    }
    if (D_80145EC8 == 0) {
        v += 2;
    }
    /* FAKE: `^ 0` keeps IDO from dropping the second, identical store below; real source unknown. T-8010 */
    annual_new_year_talk_group = v ^ 0;
    if (D_80145EC8 == 0) {
        annual_new_year_talk_group = v;
        if (D_800F62CF == 5) {
            v += 2;
            annual_new_year_talk_group = v;
        }
    }
    func_8004DE1C();
}

void annual_shrine_visit_flag_gate(void) {
    if (D_80145F60 != 0) {
        annual_mode_scene_data_setup();
        return;
    }
    func_8004DE1C();
}

void annual_shrine_visit_choice_branch(void) {
    if (D_8012E66C != 0) {
        func_8008E2E0();
        return;
    }
    func_8008E280();
}

void annual_shrine_visit_audio_setup(void) {
    func_80090960(5, 0);
    func_8008A0D4(0x418C);
    func_8004DE1C();
}

void annual_shrine_visit_exit_gate(void) {
    if ((D_800F62CF != 5) || (D_80145F60 != 0) || (D_80145EC8 == 0)) {
        func_8004DDD8();
        return;
    }
    func_8004DE1C();
}

void annual_shrine_visit_special_step_advance(void) {
    if (D_80145F60 != 0) {
        D_800F647A += 4;
    }
    func_8004DE1C();
}

void annual_shrine_visit_state_gate_ec8_zero(void) {
    if (D_80145EC8 == 0) {
        func_8004DE1C();
        return;
    }
    annual_mode_scene_data_setup();
}

void annual_shrine_visit_target_five_gate(void) {
    if ((D_800F62CF == 5) && (D_80145EC8 == 0)) {
        annual_mode_scene_data_setup();
        return;
    }
    func_8004DE1C();
}

void annual_shrine_visit_state_gate_ec8_nonzero(void) {
    if (D_80145EC8 != 0) {
        annual_mode_scene_data_setup();
        return;
    }
    func_8004DE1C();
}

void annual_shrine_visit_selected_girl_stats_adjust(void) {
    D_800F53A0.girl[5].unk_0A -= 1;
    D_800F53A0.girl[5].unk_06 -= 1;
    D_800F53A0.girl[5].unk_0E += 0xA;
    func_8008FB00();
    func_8004DE1C();
}
