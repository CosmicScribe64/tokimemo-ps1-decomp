#ifndef OVL_TEL_H
#define OVL_TEL_H

#include "common.h"
#include "common.h"
#include "game/display_records.h"
#include "game/game_state.h"
#include "game/girl.h"
#include "game/player_flags.h"
#include "game/schedule_records.h"
#include "game/word_views.h"
#include "game_enums.h"
#include "libapi.h"
#include "libgte.h"
#include "main/actor.h"
#include "main/audio.h"
#include "main/bg.h"
#include "main/calendar.h"
#include "main/cd.h"
#include "main/cursor.h"
#include "main/day_event.h"
#include "main/game_mode.h"
#include "main/girl.h"
#include "main/hud.h"
#include "main/import_views/entry_ui.h"
#include "main/import_views/entry_ui_scenes.h"
#include "main/import_views/scenes.h"
#include "main/import_views/scenes_school_events.h"
#include "main/menu.h"
#include "main/message.h"
#include "main/speaker.h"
#include "main/sprite.h"
#include "main/state.h"
#include "main/talk.h"
#include "main/text.h"
#include "main/vram.h"

extern u8 phone_selected_target_girl;
extern u8 *D_80140AD8[];
extern u8 D_80140B04[];
extern u8 D_80140B5C[];

void phone_selected_girl_report_single_line_a(void);

void phone_selected_girl_report_single_line_b(void);
extern u8 *D_80140D18[];
extern u8 D_80140D44[];
extern u8 D_80140D9C[];
void phone_selected_girl_report_single_line_c(void);
extern u8 *D_80140F0C[];
extern u8 D_80140F38[];
extern u8 D_80140F90[];
void phone_selected_girl_report_single_line_d(void);
void phone_call_step_dispatch(void);
void phone_call_mode_dispatch(void);
void phone_mode_handler(void);
s32 phone_home_call_scene_init(void);
s32 phone_missed_appointment_result_dialogue_run(void);
void phone_return_to_day_screen(void);
s32 phone_outgoing_call_mode_dispatch_a(void);
s32 phone_answering_message_scene_init(void);
s32 phone_fireworks_call_dialogue(void);
s32 phone_outgoing_call_mode_dispatch_c(void);
s32 phone_high_syoushin_call_response(void);
s32 phone_high_syoushin_call_dispatch(void);
s32 phone_b13_girl_response(void);
s32 phone_lost_gift_response_dispatch(void);
s32 phone_club_leave_state_apply(void);
s32 phone_club_absence_exit_dispatch(void);
s32 phone_culture_festival_club_notice_dialogue(void);
s32 phone_culture_festival_club_notice_dispatch(void);
void phone_miharu_progress_increment(void);
s32 phone_outgoing_call_mode_dispatch_b(void);
void phone_relationship_deltas_apply(void);
s32 phone_special_call_mode_dispatch(void);

void phone_answering_machine_scene(void);

#endif /* OVL_TEL_H */
void phone_ring_animation_step(void);
void phone_call_target_route(void);

#ifndef OVL_TEL_W9_2_TABLES_H
#define OVL_TEL_W9_2_TABLES_H
/* Signed halfword changes indexed by the status nibble. T-9720. */
extern s16 phone_missed_appointment_stat_delta[];
#endif

#ifndef OVL_TEL_W9_2_BYTE_TABLES_H
#define OVL_TEL_W9_2_BYTE_TABLES_H
/* Per-girl byte penalties read by phone_relationship_deltas_apply, eleven entries, T-9720. */
extern u8 phone_high_syoushin_tokimeki_penalty[11];
extern u8 phone_high_syoushin_affection_penalty[11];
#endif


#ifndef OVL_TEL_T10040
#define OVL_TEL_T10040
/* Dialogue pointer tables and packed word streams, T-10040. */
extern s32 D_80140554;
extern s32 D_80140558;
extern s32 D_8014055C;
extern u8 *D_801434D4[];
extern u32 D_80143500[];
extern u32 D_80143558[];
extern u8 *D_801437BC[];
extern u8 *D_80143958[];
extern u32 D_80143984[];
extern u32 D_801439DC[];
extern u8 *D_80143BA0[];
extern u32 D_80143BCC[];
extern u32 D_80143C24[];
extern u32 D_80144E3C[];
extern u32 D_80144E44[];
extern u32 D_80144E4C[];
extern u32 D_80144EAC[];

/* Forward declarations for the original object, T-10040. */
void phone_incoming_call_answer_scene(void);
void phone_relationship_information_menu_scene(void);
void phone_selected_girl_information_menu_scene(void);
void phone_selected_girl_information_dialogue_scene(void);
void phone_reply_route(void);
void phone_selected_girl_report_opening_dialogue(void);
void phone_date_invitation_resolve(void);

/* Greeting text pointers and XA tables for phone_reply_route, T-24126. */
extern u8 *D_801406E0[];
extern u32 D_80140710[];
extern u32 D_80140770[];


/* Dialogue text pointers and XA tables, T-10040. */
extern u8 *D_801410BC[];
extern u32 D_801410D4[];
extern u32 D_80141104[];
extern u8 *D_801410C8;
extern u8 *D_801410CC;

/* Forward calls within 80132000.c, T-10040. */
void phone_selected_girl_report_followup_dialogue_a(void);
void phone_selected_girl_progress_report_dialogue(void);
void phone_selected_girl_report_followup_dialogue_b(void);
void phone_selected_girl_report_followup_dialogue_c(void);

/* Sixteen day/month entries, 0x40-byte tables at +0x0564/+0x05A4. */
extern s32 phone_candidate_date_day_values[16];
extern s32 phone_candidate_date_month_values[16];
s32 phone_available_date_slots_build(void);

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014109C[];
extern u32 D_801410AC[];
extern u8 *D_80141094;
extern u8 *D_80141098;

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014104C[];
extern u32 D_80141064[];
extern u8 *D_80141040;
extern u8 *D_80141044;
extern u8 *D_80141048;

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144210[];
extern u32 D_80144240[];
extern u8 *D_801441F8;
extern u8 *D_801441FC;
extern u8 *D_80144200;
extern u8 *D_80144208;
extern u8 *D_8014420C;
extern u8 *D_80144204;
void phone_rei_call_followup_dialogue(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144298[];
extern u32 D_801442E8[];
extern u8 *D_80144270;
extern u8 *D_80144274;
extern u8 *D_80144278;
extern u8 *D_80144280;
extern u8 *D_80144284;
extern u8 *D_80144288;
extern u8 *D_8014428C;
extern u8 *D_80144290;
extern u8 *D_80144294;
extern u8 *D_8014427C;
void phone_rei_extended_call_dialogue(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144350[];
extern u32 D_80144380[];
extern u8 *D_80144338;
extern u8 *D_8014433C;
extern u8 *D_80144340;
extern u8 *D_80144348;
extern u8 *D_8014434C;
extern u8 *D_80144344;
void phone_rei_call_final_dialogue(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_801443C8[];
extern u32 D_801443F8[];
extern u8 *D_801443B0;
extern u8 *D_801443B4;
extern u8 *D_801443B8;
extern u8 *D_801443C0;
extern u8 *D_801443C4;
extern u8 *D_801443BC;
void phone_rei_call_additional_dialogue(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144440[];
extern u32 D_80144470[];
extern u8 *D_80144428;
extern u8 *D_8014442C;
extern u8 *D_80144430;
extern u8 *D_80144438;
extern u8 *D_8014443C;
extern u8 *D_80144434;
void phone_rei_call_additional_dialogue_b(void);

/* T-10040: dialogue and XA data used by phone_selected_girl_report_followup_dialogue_a. */
extern u32 D_80141084[];
extern u8 *D_8014107C;
extern u8 *D_80141080;
void phone_date_slot_menu_open(void);

/* T-10040: dialogue tables for the record-counter state. */
extern u32 D_801441A8[];
extern u32 D_801441D0[];
extern u8 *D_80144194;
extern u8 *D_80144198;
extern u8 *D_8014419C;
extern u8 *D_801441A0;
extern u8 *D_801441A4;
void phone_rei_call_dialogue_and_progress_gate(void);
#endif

#ifndef OVL_TEL_T10630_H
#define OVL_TEL_T10630_H
/* Four-byte calendar entry passed by value to hud_appointment_add. T-10630. */
typedef union TelDateEntry {
    /* 0x00 */ WordView word;
    /* 0x00 */ struct {
        u32 place : 5;  /* bits 0..4 */
        u32 year : 7;   /* bits 5..11 */
        u32 month : 4;  /* bits 12..15 */
        u32 day : 5;    /* bits 16..20 */
        u32 unk_21 : 3;
        u32 unk_24 : 1;
        u32 girl : 4;   /* bits 25..28 */
        u32 active : 1; /* bit 29 */
        u32 unk_30 : 2;
    } bits;
} TelDateEntry; /* size 0x04 */
extern s32 D_80140550;
extern s32 D_80140560;
/* One 21-place byte row per girl. */
extern u8 phone_invitation_base_adjustments[][21];
extern u8 *D_801413C8[];
extern u32 D_801413F8[];
extern u32 D_80141458[];
extern u8 *D_80141680[];
extern u32 D_801416B0[];
extern u32 D_80141710[];
extern u8 *D_801418D0[];
extern u8 *D_8014197C[];
extern u32 D_801419AC[];
extern u32 D_80141A0C[];
extern u8 *D_80141AB0[];
extern u8 *D_80141D80[];
extern u32 D_80141DB4[];
extern u32 D_80141E1C[];
extern u8 *D_80141FE0[];
extern u8 *D_80142254[];
extern u32 D_80142288[];
extern u32 D_801422F0[];
extern u8 *D_80142570[];
extern u8 *D_80142798[];
extern u32 D_801427C8[];
extern u32 D_80142828[];
extern u8 *D_801429E0[];
extern u8 *D_80142B40[];
extern u32 D_80142B70[];
extern u32 D_80142BD0[];
extern u8 *D_80142CB0[];
extern u8 *D_80142E70[];
extern u32 D_80142EA0[];
extern u32 D_80142F00[];
extern u8 *D_8014306C[];
extern u8 *D_801431E8[];
extern u32 D_80143218[];
extern u32 D_80143278[];
extern u8 *D_8014333C[];
#endif

void phone_date_place_availability_build();

#ifndef OVL_TEL_T10860_H
#define OVL_TEL_T10860_H
/* Dialogue pointer and XA command tables, T-10860. */
extern u8 *D_801450C8;
extern u8 *D_801450CC;
extern u8 *D_801450D0;
extern u8 *D_801450D4;
extern u8 *D_801450D8;
extern u8 *D_801450DC;
extern u8 *D_801450E0;
extern u8 *D_80145454;
extern u8 *D_80145458;
extern u8 *D_8014545C;
extern u8 *D_80145460;
extern u8 *D_80145464;
extern u8 *D_801452C8[10];
extern u32 D_801450E4[];
extern u32 D_8014511C[];
extern u32 D_801452F0[];
extern u32 D_80145340[];
extern u32 D_80145468[];
extern u32 D_801454A0[];
#endif

#ifndef OVL_TEL_T10860_ADC_H
#define OVL_TEL_T10860_ADC_H
s32 phone_yoshio_relationship_report_run(void);
s32 phone_high_syoushin_girl_find(void);
extern u8 *D_80144FB8;
extern u8 *D_80144FBC;
extern u8 *D_80144FC0;
extern u8 *D_80144FC4;
extern u32 D_80144FC8[];
extern u32 D_80144FE8[];
#endif

#ifndef OVL_TEL_T10860_REPLIES_H
#define OVL_TEL_T10860_REPLIES_H
extern u8 *D_80146904[12];
extern u8 *D_80146BF4[12];
extern u8 *D_80146EE4[12];
extern u32 D_80146934[];
extern u32 D_8014698C[];
extern u32 D_80146C24[];
extern u32 D_80146C7C[];
extern u32 D_80146F14[];
extern u32 D_80146F6C[];
extern u8 *D_80146008[12];
extern u8 *D_801462F0[12];
extern u8 *D_801465B0[12];
extern u32 D_80146038[];
extern u32 D_80146090[];
extern u32 D_80146320[];
extern u32 D_80146378[];
extern u32 D_801465E0[];
extern u32 D_80146638[];
s32 phone_b12_girl_find(void);
s32 phone_b13_girl_find(void);
#endif

#ifndef OVL_TEL_T10860_GREETING_TABLES_H
#define OVL_TEL_T10860_GREETING_TABLES_H
/* Text pointer loads and word-indexed XA tables in 80139AA4/80139FEC. */
extern u8 *D_8014544C;
extern u8 *D_80145450;
extern u8 *D_80144D6C[12];
extern u32 D_80144D9C[];
extern u32 D_80144DEC[];
#endif

#ifndef OVL_TEL_T10860_GREETING_TABLES2_H
#define OVL_TEL_T10860_GREETING_TABLES2_H
extern u8 *D_80144738[12];
extern u8 *D_80144AA4[12];
extern u32 D_80144768[];
extern u32 D_801447B8[];
extern u32 D_80144AD4[];
extern u32 D_80144B24[];
#endif

#ifndef OVL_TEL_T10860_REPLY_TABLES_13
#define OVL_TEL_T10860_REPLY_TABLES_13
/* T-10860: 13 pointer entries each, bounded by the following data symbol. */
extern u8 **D_80145C44[13];
extern u32 *D_80145C78[13];
extern u32 *D_80145CAC[13];
#endif

#ifndef OVL_TEL_T10860_A4FC_VIEW
#define OVL_TEL_T10860_A4FC_VIEW
/* T-10860: preceding dispatcher used the legacy integer return view. */
s32 phone_miharu_call_dialogue(void);
#endif

#ifndef OVL_TEL_T10860_DISPATCH_DECLS
#define OVL_TEL_T10860_DISPATCH_DECLS
void phone_opening_call_dialogue_run(void);
void phone_appointment_result_dialogue_run(void);
#endif

#ifndef OVL_TEL_RECORD_FLAGS_H
#define OVL_TEL_RECORD_FLAGS_H
/* T-10860: bits cleared in a GirlRecord +0x0C flag word by phone_relationship_deltas_apply. */
typedef struct TelRecordFlags {
    /* 0x00, bits 0..11 */ u32 low : 12;
    /* 0x00, bit 12 */ u32 b12 : 1;
    /* 0x00, bit 13 */ u32 b13 : 1;
    /* 0x00, bits 14..15 */ u32 middle : 2;
    /* 0x00, bit 16 */ u32 b16 : 1;
    /* 0x00, bits 17..31 */ u32 high : 15;
} TelRecordFlags; /* size 0x04 */
#endif

#ifndef OVL_TEL_T24045_SIGN_DAYS
#define OVL_TEL_T24045_SIGN_DAYS
/* Star-sign boundaries copied as sixteen words by phone_profile_details_draw. T-24045. */
typedef struct TelSignDays {
    /* 0x00 */ u32 w[16];
} TelSignDays; /* size 0x40 */
extern TelSignDays D_80146FD0;
#endif
