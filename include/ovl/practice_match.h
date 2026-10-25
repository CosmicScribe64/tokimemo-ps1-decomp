#ifndef OVL_PRACTICE_MATCH_H
#define OVL_PRACTICE_MATCH_H

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game_enums.h"
#include "libapi.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/cd.h"
#include "main/date.h"
#include "main/day_event.h"
#include "main/draw.h"
#include "main/event_log.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/minigame_scenes_school_events.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/scene.h"
#include "main/select_girl.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/vram.h"
#include "main/vsync.h"

/* RENSYU overlay (load address 0x80132000): externs and types. */

extern s32 D_801342B0;
extern s32 D_801342B4;
extern s32 D_801342B8;
extern s32 D_801343D0;
extern s32 D_801343D4;
extern s32 D_801343D8;
extern s16 D_801343DC;
extern s32 D_801343E0;
extern u8 practice_selected_girl;
extern u8 practice_match_result_grade;

/* T-19020: GameState.unk_66C[i] seen as a halfword pair; the low 4 bits of the halfword at +2 are a counter
 * (incremented as lhu, +1, andi 0xF, merged with the lbu of the same byte, sb). */
typedef struct RensyuClubCount {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 count : 4;
    /* 0x02 */ u16 unk_02_4 : 12;
} RensyuClubCount; /* size 0x04 */

/* T-19020: 11 halfwords at D_80134390, copied to the stack by practice_result_dialogue_select (22 bytes: five word moves and one halfword). */
typedef struct RensyuTbl11 {
    u16 v[11];
} RensyuTbl11; /* size 0x16 */
/* T-24122: 13 halfwords at D_801343A8, copied to the stack by practice_result_girl_dialogue (26 bytes: six word moves and one halfword). */
typedef struct RensyuTbl13 {
    s16 v[13];
} RensyuTbl13; /* size 0x1A */
/* T-24138: 13 words at D_801342F4 and D_80134328, copied to the stack by practice_candidate_and_day_scene_select (52 bytes each). */
typedef struct RensyuWords13 {
    s32 v[13];
} RensyuWords13; /* size 0x34 */
extern RensyuWords13 D_801342F4;
extern RensyuWords13 D_80134328;
extern RensyuTbl11 practice_match_point_thresholds;
extern RensyuTbl11 D_80134390;
extern RensyuTbl13 D_8013435C;
extern RensyuTbl13 D_801343A8;
extern u8 practice_round_index;
extern u8 practice_round_win_flags[5];

/* T-11190: declarations added by tools/bulk_m2c.py. */
void practice_intro_talk_pointer_setup(void);
void practice_selected_girl_portrait_select(void);
void practice_club6_actor_tables_setup(void);
void practice_club7_actor_tables_setup(void);
void practice_club8_actor_tables_setup(void);
void practice_club9_actor_tables_setup(void);
void practice_club10_actor_tables_setup(void);

#endif /* OVL_PRACTICE_MATCH_H */
