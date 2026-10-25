#ifndef OVL_MASTER_H
#define OVL_MASTER_H

/* T-24038: event-record logger, called before its definition; result is unused. */
void master_tournament_result_log(void);

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/girl.h"
#include "game/schedule_records.h"
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
#include "main/import_views/entry_ui_scenes_school_events.h"
#include "main/import_views/minigame_scenes_school_events.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/message.h"
#include "main/portrait.h"
#include "main/prim.h"
#include "main/scene.h"
#include "main/schedule.h"
#include "main/select_girl.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/tpage.h"
#include "main/vram.h"
#include "main/vsync.h"

/* MASTER overlay (load address 0x80132000): externs and types. */

extern u32 D_8013C2D0; /* three pointers set by master_set_award_text_assets_1 and siblings */
extern u32 D_8013C2D4;
extern u32 D_8013C2D8;
extern u32 D_8013C4A0; /* same, second group */
extern u32 D_8013C4A4;
extern u32 D_8013C4A8;

extern u32 D_8013C750; /* third group */
extern u32 D_8013C754;
extern u32 D_8013C758;
extern s32 D_8013C710;
extern s32 D_8013C714;
extern s32 D_8013C718;
extern s16 D_8013C71C;
extern s32 D_8013C720;
extern s32 D_8013C724;
extern s32 D_8013C728;
extern s16 D_8013C72C;
extern s32 D_8013C730;
extern s32 D_8013C740;
extern s32 D_8013C744;
extern s32 D_8013C748;
extern s16 D_8013C74C;
extern s32 D_8013C760;
extern s32 D_8013C764;
extern s32 D_8013C768;
extern s16 D_8013C76C;
extern s32 D_8013C770;
void master_player_birthday_scene_run();
extern s32 D_8013C4B0;
extern s32 D_8013C4B4;
extern s32 D_8013C4B8;
extern s32 D_8013C4BC;
extern s32 D_8013C4C0;
extern s32 D_8013C4C4;
extern s16 D_8013C4C8;
extern s16 D_8013C4CC;
extern s32 D_8013C4D0;
void master_set_companion_actor_assets_a();
void master_set_companion_actor_assets_c();
extern s32 D_8013C4D4;
extern s32 D_8013C4D8;
extern s32 D_8013C4DC;
extern s32 D_8013C4E0;
extern s32 D_8013C4E4;
extern s32 D_8013C4E8;
extern s32 D_8013C4EC;
extern s32 D_8013C4F0;
extern s32 D_8013C4F4;
extern s32 D_8013C4F8;
extern s32 D_8013C4FC;
extern s32 D_8013C500;
extern s32 D_8013C504;
extern s32 D_8013C508;
extern s32 D_8013C50C;
extern s32 D_8013C510;
extern s32 D_8013C514;
extern s32 D_8013C518;
extern s32 D_8013C51C;
extern s32 D_8013C520;
extern s32 D_8013C524;
extern s32 D_8013C528;
extern s32 D_8013C52C;
extern s32 D_8013C530;
extern s32 D_8013C534;
extern s32 D_8013C538;
extern s32 D_8013C53C;
extern s32 D_8013C540;
extern s32 D_8013C544;
extern s32 D_8013C548;
extern s32 D_8013C54C;
extern s32 D_8013C550;
extern s32 D_8013C554;
extern s32 D_8013C558;
extern s32 D_8013C55C;
extern s32 D_8013C560;
extern s32 D_8013C564;
extern s32 D_8013C568;
extern s32 D_8013C56C;
extern u8 master_final_tournament_companion_index;
extern s32 master_selected_companion; /* GirlId or 0xFF; selected companion, not the tournament winner. */
extern u8 master_final_tournament_grade;

/* T-9960: called by the preceding state machine in the same object. */
void master_birthday_response_name_select(void);

/* T-9960: eleven word-sized text ids copied into master_player_birthday_scene's frame. */
typedef struct MasterWords11 {
    /* 0x00 */ s32 v[11];
} MasterWords11; /* size 0x2C */
extern MasterWords11 D_8013C64C;

#endif /* OVL_MASTER_H */

#ifndef OVL_MASTER_T10630_H
#define OVL_MASTER_T10630_H

typedef struct MasterPoint2 {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
} MasterPoint2; /* size 0x04 */
extern s32 master_tournament_stage_index;
extern u8 master_tournament_result_grade;
extern u8 D_8013C588;
extern MasterPoint2 D_8013C58C[10];
extern s16 D_8013C5B4[10];
#endif

#ifndef OVL_MASTER_T10630_TABLES_H
#define OVL_MASTER_T10630_TABLES_H
/* Message and threshold tables copied into local frames, T-10630. */
typedef struct MasterHalfwords13 {
    /* 0x00 */ s16 v[13];
} MasterHalfwords13; /* size 0x1A */
typedef struct MasterHalfwords5 {
    /* 0x00 */ u16 v[5];
} MasterHalfwords5; /* size 0x0A */
extern MasterHalfwords13 D_8013C6C0;
extern MasterHalfwords13 D_8013C6DC;
/* T-13001: 0x1A-byte copies, indexed signed halfword reads in each consumer. */
extern MasterHalfwords13 D_8013C7E8;
extern MasterHalfwords13 D_8013C804;
extern MasterHalfwords13 D_8013C820;
extern MasterHalfwords5 master_tournament_grade_thresholds;
extern u8 master_tournament_companion_index;
extern s32 D_8013C574;
extern u8 master_tournament_result_markers[5];
#endif

/* Calls from the large scene handlers to later definitions in this object. */
void master_update_tournament_audience_animation(void);
void master_tournament_player_actor_setup(void);
void master_log_tournament_companion_event(void);
void master_log_tournament_companion_followup(void);
void master_set_companion_actor_assets_b(void);

/* T-10630: eleven resource IDs copied by the neighboring selector. */
extern MasterWords11 D_8013C678;

/* T-18600: called by master_tournament_reward_apply before its definition. */
void master_tournament_event_log_append(void);

/* T-13001: later helpers called by the typed-table scene handlers. */
void master_tournament_companion_actor_setup(void);
void master_log_tournament_followup_event(void);

#ifndef OVL_MASTER_T18600_H
#define OVL_MASTER_T18600_H
/* T-18600: tables copied whole into a local frame and called or indexed there (T-3330 idiom). */
typedef struct FnTbl4 {
    /* 0x00 */ void (*f[4])();
} FnTbl4; /* size 0x10 */
typedef struct FnTbl7 {
    /* 0x00 */ void (*f[7])();
} FnTbl7; /* size 0x1C */
typedef struct FnTbl30 {
    /* 0x00 */ void (*f[30])();
} FnTbl30; /* size 0x78 */
typedef struct PtrTbl3 {
    /* 0x00 */ void *p[3];
} PtrTbl3; /* size 0x0C */
/* Thirteen words indexed by master_selected_companion or master_final_tournament_companion_index: text ids and thresholds. */
typedef struct MasterWords13 {
    /* 0x00 */ s32 v[13];
} MasterWords13; /* size 0x34 */

extern FnTbl4 D_8013C2E4;        /* master_award_mode_stage_dispatch's scene states */
extern FnTbl30 master_tournament_mode_stage_callbacks;       /* master_tournament_scene_mode_stage_dispatch's scene states */
extern PtrTbl3 D_8013C700;
extern FnTbl7 master_final_tournament_mode_stage_callbacks;        /* master_tournament_mode_stage_dispatch's scene states */
/* master_companion_dialogue_scene_run's eight stack tables, in frame order sp1A8..sp3C */
extern MasterWords13 D_8013C2F4;
extern MasterWords13 D_8013C328;
extern MasterWords13 D_8013C35C;
extern MasterWords13 D_8013C390;
extern MasterWords13 D_8013C3C4;
extern MasterWords13 D_8013C3F8;
extern MasterWords13 D_8013C42C;
extern MasterWords13 D_8013C460; /* followed by three zero words of padding */
extern MasterWords13 D_8013C7B4; /* master_companion_portrait_load */
extern MasterHalfwords13 D_8013C6A4; /* master_tournament_companion_dialogue_scene */

/* Second baseball event (master_tournament_final_grade_scene), the twin of D_8013C574..master_tournament_companion_index. */
extern u8 D_8013C780;
extern u8 D_8013C788[5];         /* like master_tournament_result_markers[5]; D_8013C78C is element 4 */
extern s16 D_8013C794;
#endif
