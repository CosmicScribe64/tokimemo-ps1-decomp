/* annual_character_state_helpers.c: ; src/ovl/GYOZI/8013E5B0.c:36-66 updates displayed character state and flags. Original object 0x8013E5B0-0x8013EB10 (config/objects/GYOZI.txt). */
#include "common.h"
#include "ovl/annual_events.h"

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_80148180;

void annual_character_state_mode_step_dispatch(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148180;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

/* FAKE: empty counter test steers allocation; the opaque call keeps only
 * the base argument to avoid a redundant stride reload. T-10850.
 * FAKE: the loop init, the empty test and the `do` share one source line (as1 schedules by line;
 * autotweak group, T-20830). T-21520 */
void annual_character_state_deltas_apply(void)
{
    s16 i;
    i = 0; if (i && i) { } do
    {
        if ((D_800F53A0.girl[i].unk_10[1] & 1) && (D_800F53A0.unk_F2F != i))
        {
            D_800F53A0.girl[i].unk_0A += 1;
        }
        i += 1;
    }
    while (i < 0xB);
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 8;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 8;
    func_8008FB00(&D_800F53A0);
    func_80050D60(0, 0);
    func_80081D30(1);
}

void annual_character_flag_snapshot_setup(void) {
    s16 i;

    annual_hallway_talk_text_table = D_8014808C;
    annual_hallway_talk_command_table = D_801480C0;
    annual_hallway_talk_voice_table = D_801480F4;
    annual_hallway_talk_group = 0;
    annual_hallway_talk_line = 0;
    for (i = 0; i < 0xB; i++) {
        D_800D9388[i] = (D_800F53A0.girl[i].unk_10[1] & 1) >= 1;
    }
    func_8004DE1C();
}

void annual_character_state_resource_select(void) {
    D_800F62CF = (u8) D_8012E66C;
    func_80090960(7, 0);
    func_8008F618(D_800F62CF);
    func_8004DE1C();
}

void annual_character_state_table_select(void) {
    s32 r;
    u8 s;

    r = func_8005E0E0(D_800F62CF);
    s = r & 0x7F;
    annual_hallway_talk_group = 0;
    if (s >= 2U) {
        annual_hallway_talk_group += 2;
    }
    if (s >= 3U) {
        annual_hallway_talk_group += 2;
    }
    if (s == 4) {
        annual_hallway_talk_group += 2;
    }
    switch (D_800F62CF) {
    case 0:
        annual_hallway_talk_text_table = D_80148064;
        annual_hallway_talk_command_table = D_80148098;
        annual_hallway_talk_voice_table = D_801480CC;
        break;
    case 1:
        annual_hallway_talk_text_table = D_80148078;
        annual_hallway_talk_command_table = D_801480AC;
        annual_hallway_talk_voice_table = D_801480E0;
        break;
    case 2:
        annual_hallway_talk_text_table = D_80148068;
        annual_hallway_talk_command_table = D_8014809C;
        annual_hallway_talk_voice_table = D_801480D0;
        break;
    case 3:
        annual_hallway_talk_text_table = D_80148070;
        annual_hallway_talk_command_table = D_801480A4;
        annual_hallway_talk_voice_table = D_801480D8;
        break;
    case 4:
        annual_hallway_talk_text_table = D_80148088;
        annual_hallway_talk_command_table = D_801480BC;
        annual_hallway_talk_voice_table = D_801480F0;
        break;
    case 5:
        annual_hallway_talk_text_table = D_8014807C;
        annual_hallway_talk_command_table = D_801480B0;
        annual_hallway_talk_voice_table = D_801480E4;
        break;
    case 6:
        annual_hallway_talk_text_table = D_80148080;
        annual_hallway_talk_command_table = D_801480B4;
        annual_hallway_talk_voice_table = D_801480E8;
        break;
    case 7:
        annual_hallway_talk_text_table = D_80148074;
        annual_hallway_talk_command_table = D_801480A8;
        annual_hallway_talk_voice_table = D_801480DC;
        break;
    case 8:
        annual_hallway_talk_text_table = D_80148060;
        annual_hallway_talk_command_table = D_80148094;
        annual_hallway_talk_voice_table = D_801480C8;
        break;
    case 9:
        annual_hallway_talk_text_table = D_80148084;
        annual_hallway_talk_command_table = D_801480B8;
        annual_hallway_talk_voice_table = D_801480EC;
        break;
    case 10:
        annual_hallway_talk_text_table = D_80148090;
        annual_hallway_talk_command_table = D_801480C4;
        annual_hallway_talk_voice_table = D_801480F8;
        break;
    }
    func_8004DE1C();
}
