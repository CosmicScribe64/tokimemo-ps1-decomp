/* annual_character_action_dispatch.c: ; src/ovl/GYOZI/80143110.c:34-59 dispatches callbacks by the current character selector. Original object 0x80143110-0x80143AD0 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_80148A7C;

typedef struct {
    void (*f[12])();
} FnTbl12; /* size 0x30 */
extern FnTbl12 D_80148A4C;

typedef struct {
    s32 w;
} Word4; /* size 0x4 */
extern Word4 annual_saved_girl_state_word;

void annual_character_state_word_store(void) {
    *(Word4 *)&D_800F53A0.girl[D_800F62CF].unk_08 = annual_saved_girl_state_word;
    func_8004DE1C();
}

void annual_character_action_group16_dispatch(void);
void annual_character_action_group14_dispatch(void);
void annual_character_action_group10_dispatch(void);
void annual_character_action_group14_secondary_dispatch(void);
void annual_character_action_group12_dispatch(void);
void annual_character_action_group17_dispatch(void);
void annual_character_action_group17_secondary_dispatch(void);
void annual_character_action_group11_dispatch(void);
void annual_character_action_group11_secondary_dispatch(void);

s16 annual_character_action_mode_step_dispatch(void) {
    if (D_800F647A == 0) {
        annual_saved_girl_state_word = *(Word4 *)&D_800F53A0.girl[D_800F53A0.unk_F2F].unk_08[0];
        D_800F53A0.girl[D_800F62CF].unk_0A = 0x32;
    }
    switch (D_800F53A0.unk_F2F) {
    case 0:
        talk_voice_play();
        break;
    case 1:
        annual_character_action_group16_dispatch();
        break;
    case 2:
        annual_character_action_group14_dispatch();
        break;
    case 3:
        annual_character_action_group10_dispatch();
        break;
    case 4:
        annual_character_action_group14_secondary_dispatch();
        break;
    case 5:
        annual_character_action_group12_dispatch();
        break;
    case 6:
        annual_character_action_group17_dispatch();
        break;
    case 7:
        annual_character_action_group17_secondary_dispatch();
        break;
    case 8:
        annual_character_action_group11_dispatch();
        break;
    case 9:
        annual_character_action_group11_secondary_dispatch();
        break;
    case 10:
        talk_voice_play();
        break;
    }
    *(s8 *)(&D_8012B8C2 + 5) = *(u8 *)&D_801317EB;
    if ((*(s16 *)(&D_8012B8C2 + 0x16) == 0) && (*(s16 *)(&D_8012B8C2 + 6) == 0)) {
        *(s16 *)(&D_8012B8C2 + 0x14) = *(s16 *)(&D_8012B8C2 + 0x14) + 1;
        if (*(s16 *)(&D_8012B8C2 + 0x14) >= 3) {
            *(s16 *)(&D_8012B8C2 + 0x14) = 0;
        }
    }
}

typedef struct {
    void (*f[16])();
} FnTbl16; /* size 0x40 */
extern FnTbl16 D_80148974;

void annual_character_action_group16_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl16 tbl;

    tbl = D_80148974;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_1(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

void annual_character_action_sound_play_0500(void) {
    func_80086AB0(0x500);
    func_8004DE1C();
}

typedef struct {
    void (*f[14])();
} FnTbl14; /* size 0x38 */
extern FnTbl14 D_801489B4;

void annual_character_action_group14_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_801489B4;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_2(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

void annual_character_action_counter_increment_flag3(void) {
    if (((u32) D_800F55CA >> 4) == 3) {
        annual_result_talk_group += 1;
    }
    func_8004DE1C();
}

void annual_character_action_counter_increment_not_flag3(void) {
    if (((u32) D_800F55CA >> 4) != 3) {
        annual_result_talk_group += 1;
    }
    func_8004DE1C();
}

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_801489EC;

void annual_character_action_group10_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_801489EC;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_3(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

extern FnTbl14 D_80148A14;

void annual_character_action_group14_secondary_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl14 tbl;

    tbl = D_80148A14;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_sound_play_0501(void) {
    func_80086AB0(0x501);
    func_8004DE1C();
}

void annual_character_action_counter_increment_flag7(void) {
    if (((u32) D_800F563A >> 4) == 7) {
        annual_result_talk_group += 3;
    }
    func_8004DE1C();
}

void annual_character_action_counter_set_3_variant_4(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

void annual_character_action_group12_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl12 tbl;

    tbl = D_80148A4C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_5(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

void annual_character_action_group17_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148A7C;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_4(void) {
    annual_result_talk_group = 4;
    func_8004DE1C();
}

void annual_character_action_sound_play_0505(void) {
    func_80086AB0(0x505);
    func_8004DE1C();
}

extern FnTbl17 D_80148AC0;

void annual_character_action_group17_secondary_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148AC0;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_confirm_input_gate(void) {
    if (func_8005B32C() != 0) {
        func_8005B06C(1);
        func_8004DE1C();
    }
}

void annual_character_action_counter_set_3_variant_6(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

typedef struct {
    void (*f[11])();
} FnTbl11; /* size 0x2C */
extern FnTbl11 D_80148B04;

void annual_character_action_group11_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80148B04;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_7(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

extern FnTbl11 D_80148B30;

void annual_character_action_group11_secondary_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl11 tbl;

    tbl = D_80148B30;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

void annual_character_action_counter_set_3_variant_8(void) {
    annual_result_talk_group = 3;
    func_8004DE1C();
}

void annual_character_action_voice_resource_play_5617(void) {
    func_80072734(0x5617);
    func_8004DE1C();
}
