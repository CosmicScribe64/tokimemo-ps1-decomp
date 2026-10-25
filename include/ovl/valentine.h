#ifndef OVL_VALENTINE_H
#define OVL_VALENTINE_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_normal_date_girl_out /* implicit int declaration: valentine_megumi_birthday_gate returns its result (void in main_api.h) */

#include "common.h"
#include "common.h"
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
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/scene.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"

/* VALEN overlay (load address 0x80132000): externs and types. */

void valentine_talk_driver();
extern s16 valentine_candidate_index;
void valentine_birthday_pointer_setup();
void valentine_scene_pointer_setup();
extern s16 valentine_talk_group;
extern s16 valentine_candidate_count;
extern volatile u8 valentine_unmet_megumi_selected; /* FAKE: volatile keeps as1's global scheduler from moving this byte's `lui` out of the valentine_candidate_scene_select `sel == 9` block into the branch delay slot (wiki/as1-scheduler-rule.md); real declaration unknown. T-24903 */
extern s32 D_80134400;
extern s32 D_80134434;
extern s32 D_80134468;
extern s32 valentine_talk_text_table;
extern s32 valentine_talk_command_table;
extern s32 valentine_talk_voice_table;
extern s16 valentine_talk_line;

extern s32 D_801343F0;
extern s32 D_801343F4;
extern s32 D_801343F8;
extern s32 D_801343FC;
extern s32 D_80134404;
extern s32 D_80134408;
extern s32 D_8013440C;
extern s32 D_80134410;
extern s32 D_80134414;
extern s32 D_80134418;
extern s32 D_8013441C;
extern s32 D_80134420;
extern s32 D_80134424;
extern s32 D_80134428;
extern s32 D_8013442C;
extern s32 D_80134430;
extern s32 D_80134438;
extern s32 D_8013443C;
extern s32 D_80134440;
extern s32 D_80134444;
extern s32 D_80134448;
extern s32 D_8013444C;
extern s32 D_80134450;
extern s32 D_80134454;
extern s32 D_80134458;
extern s32 D_8013445C;
extern s32 D_80134460;
extern s32 D_80134464;
extern s32 D_8013446C;
extern s32 D_80134470;
extern s32 D_80134474;
extern s32 D_80134478;
extern s32 D_8013447C;
extern s32 D_80134480;
extern s32 D_80134484;
extern s32 D_80134488;
extern s32 D_80134660;
extern s32 D_80134664;
extern s32 D_80134668;
extern s32 D_8013466C;
extern s32 D_80134670;
extern s32 D_80134674;
extern s32 D_80134678;
extern s32 D_8013467C;
extern s32 D_80134680;
extern s32 D_80134684;
extern s32 D_80134688;
extern s32 D_8013468C;
extern volatile s32 D_80134690; /* FAKE: volatile keeps as1's global scheduler from moving this load's lui out of the valentine_birthday_event_mark default arm (wiki/as1-scheduler-rule.md); real declaration unknown. T-24900 */
extern s32 D_80134694;
extern s32 D_80134698;
extern s32 D_8013469C;
extern s32 D_801346A0;
extern s32 D_801346A4;
extern s32 D_801346A8;
extern s32 D_801346AC;
extern s32 D_801346B0;
extern s32 D_801346B4;
extern s32 D_801346B8;
extern s32 D_801346BC;
extern s32 D_801346C0;
extern volatile s32 D_801346C4; /* FAKE: volatile keeps as1's global scheduler from moving this load's lui out of the valentine_birthday_event_mark default arm (wiki/as1-scheduler-rule.md); real declaration unknown. T-24900 */
extern s32 D_801346C8;
extern s32 D_801346CC;
extern s32 D_801346D0;
extern s32 D_801346D4;
extern s32 D_801346D8;
extern s32 D_801346DC;
extern s32 D_801346E0;
extern s32 D_801346E4;
extern s32 D_801346E8;
extern s32 D_801346EC;
extern s32 D_801346F0;
extern s32 D_801346F4;
extern volatile s32 D_801346F8; /* FAKE: volatile keeps as1's global scheduler from moving this load's lui out of the valentine_birthday_event_mark default arm (wiki/as1-scheduler-rule.md); real declaration unknown. T-24900 */
void valentine_birthday_callback_dispatch(void);
s32 valentine_day_event_complete(void);
void valentine_return_callback_dispatch(void);
void valentine_candidate_order_shuffle(void); /* defined in 80132760.c, called by valentine_girl_candidates_select (T-24138) */

/* T-24138: parameter record at GameState +0x11C is 16 halfwords here
 * (main_api.h's ParamValue covers the shared 4-byte prefix); valentine_scene_place_select
 * adjusts six halfword parameters. Accessed through ValenStateView. */
typedef struct ValenParaRec {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
} ValenParaRec; /* size 0x10 */
typedef struct ValenStateView {
    /* 0x000 */ u8 pad_000[0x11C];
    /* 0x11C */ ValenParaRec unk_11C;
} ValenStateView; /* prefix size 0x12C */

extern s16 valentine_candidate_girls[];

extern u8 valentine_ranked_girls[];
extern u8 valentine_scene_place_index;

/* T-27056 R4 eligibility thresholds and assigned amounts have separate names. */
enum ValentineCandidateRule {
    VALENTINE_UNMET_MEGUMI_TOKIMEKI_MINIMUM = 20, /* Unmet Megumi special eligibility. */
    VALENTINE_MEGUMI_INITIAL_AFFECTION = 50, /* Assigned affection for special unmet Megumi. */
    VALENTINE_RANK_TOKIMEKI_MINIMUM = 70, /* Tokimeki threshold for top-three ranking and Shiori ordering. */
    VALENTINE_CANDIDATE_AFFECTION_MINIMUM = 50 /* Minimum affection for ordinary candidate inclusion. */
};

#endif /* OVL_VALENTINE_H */

