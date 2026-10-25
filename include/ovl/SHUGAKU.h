#ifndef OVL_SHUGAKU_H
#define OVL_SHUGAKU_H

#include "common.h"
#include "game.h"

/* SHUGAKU overlay (load address 0x80132000): externs and types. */

extern u8 D_8013C2E4;
extern u8 D_8013C2EC;

extern u8 D_8013C1C0;
extern s32 D_8013AEF0;
void trip_birthday_script_pointer_table_setup(void);
void trip_scene_pointer_table_setup(void);
void trip_ayako_scene_resource_pointer_setup(void);
void trip_yukari_scene_resource_pointer_setup(void);
void trip_corridor_scene_resource_pointer_setup(void);
void trip_yuina_scene_resource_pointer_setup(void);
void trip_yuina_background_load(void);
void trip_girl_script_pointer_table_setup(void);
void trip_sightseeing_hotel_lobby_background_load(void);
#ifndef SHUGAKU_OVERRIDE_trip_destination_response_talk_run
void trip_destination_response_talk_run(void);
#endif

void trip_corridor_callback_dispatch_saved_mode_stage_three(void);
#ifndef SHUGAKU_OVERRIDE_trip_ayako_response_talk_run
void trip_ayako_response_talk_run(void);
#endif
void trip_corridor_callback_dispatch_and_actor_fade(void);

extern s16 D_8013B084;
extern s32 D_8013C870;
extern s32 D_8013C874;
extern s32 D_8013C878;
extern s32 D_8013C880;
extern u8 D_8013C974;
extern s32 D_8013CAF0;
extern s32 D_8013CAF4;
extern s32 D_8013CAF8;
extern s32 D_8013CB00;
extern s32 D_8013C450;
extern s32 D_8013C484;
extern s32 D_8013C4B8;
extern s32 D_8013C2D0;
extern s32 D_8013C2D4;
extern s32 D_8013C2D8;
extern s16 D_8013C2DC;
extern s32 D_8013C2E0;
extern s16 D_801C26A0;
extern s32 D_8013AEE0;
s32 trip_day_event_complete(void);

extern s32 D_8013B06C;
extern s32 D_8013B070;
extern s32 D_8013B074;
extern s32 D_8013B078;
extern s32 D_8013B07C;
extern s32 D_8013B080;
extern s16 D_8013B08C;
extern s32 D_8013AD70;
void trip_birthday_script_text_copy(void);
void trip_destination_scene_resource_pointer_setup(void);
extern s16 D_8013B088;
extern s32 D_8013BFFC;
extern s32 D_8013C000;
extern s32 D_8013C004;

void trip_saved_mode_stage_restore(void);

extern u8 D_8013C97C;
extern s32 D_8013BE74;
extern s32 D_8013BEA8;
extern s32 D_8013BEDC;

extern s16 D_8013CAFC;

void trip_girl_scene_select();

void trip_destination_audio_transition();
extern s32 D_8013AFD0;
extern s32 D_8013AFD4;
extern s32 D_8013AFD8;
extern s32 D_8013AFDC;
extern s32 D_8013AFE0;
extern s32 D_8013AFE4;
extern s32 D_8013AFE8;
extern s32 D_8013AFEC;
extern s32 D_8013AFF0;
extern s32 D_8013AFF4;
extern s32 D_8013AFF8;
extern s32 D_8013AFFC;
extern s32 D_8013B000;
extern s32 D_8013B004;
extern s32 D_8013B008;
extern s32 D_8013B00C;
extern s32 D_8013B010;
extern s32 D_8013B014;
extern s32 D_8013B018;
extern s32 D_8013B01C;
extern s32 D_8013B020;
extern s32 D_8013B024;
extern s32 D_8013B028;
extern s32 D_8013B02C;
extern s32 D_8013B030;
extern s32 D_8013B034;
extern s32 D_8013B038;
extern s32 D_8013B03C;
extern s32 D_8013B040;
extern s32 D_8013B044;
extern s32 D_8013B048;
extern s32 D_8013B04C;
extern s32 D_8013B050;
extern s32 D_8013B054;
extern s32 D_8013B058;
extern s32 D_8013B05C;
extern s32 D_8013B060;
extern s32 D_8013B064;
extern s32 D_8013B068;
extern s32 D_8013BE70;
extern s32 D_8013BE78;
extern s32 D_8013BE7C;
extern s32 D_8013BE80;
extern s32 D_8013BE84;
extern s32 D_8013BE88;
extern s32 D_8013BE8C;
extern s32 D_8013BE90;
extern s32 D_8013BE94;
extern s32 D_8013BE98;
extern s32 D_8013BE9C;
extern s32 D_8013BEA0;
extern s32 D_8013BEA4;
extern s32 D_8013BEAC;
extern s32 D_8013BEB0;
extern s32 D_8013BEB4;
extern s32 D_8013BEB8;
extern s32 D_8013BEBC;
extern s32 D_8013BEC0;
extern s32 D_8013BEC4;
extern s32 D_8013BEC8;
extern s32 D_8013BECC;
extern s32 D_8013BED0;
extern s32 D_8013BED4;
extern s32 D_8013BED8;
extern s32 D_8013BEE0;
extern s32 D_8013BEE4;
extern s32 D_8013BEE8;
extern s32 D_8013BEEC;
extern s32 D_8013BEF0;
extern s32 D_8013BEF4;
extern s32 D_8013BEF8;
extern s32 D_8013BEFC;
extern s32 D_8013BF00;
extern s32 D_8013BF04;
extern s32 D_8013BF08;
extern s32 D_8013C420;
extern s32 D_8013C424;
extern s32 D_8013C428;
extern s32 D_8013C42C;
extern s32 D_8013C430;
extern s32 D_8013C434;
extern s32 D_8013C438;
extern s32 D_8013C43C;
extern s32 D_8013C440;
extern s32 D_8013C444;
extern s32 D_8013C448;
extern s32 D_8013C44C;
extern s32 D_8013C454;
extern s32 D_8013C458;
extern s32 D_8013C45C;
extern s32 D_8013C460;
extern s32 D_8013C464;
extern s32 D_8013C468;
extern s32 D_8013C46C;
extern s32 D_8013C470;
extern s32 D_8013C474;
extern s32 D_8013C478;
extern s32 D_8013C47C;
extern s32 D_8013C480;
extern s32 D_8013C488;
extern s32 D_8013C48C;
extern s32 D_8013C490;
extern s32 D_8013C494;
extern s32 D_8013C498;
extern s32 D_8013C49C;
extern s32 D_8013C4A0;
extern s32 D_8013C4A4;
extern s32 D_8013C4A8;
extern s32 D_8013C4AC;
extern s32 D_8013C4B0;
extern s32 D_8013C4B4;
extern u8 D_8013C1BC;
extern s32 D_8013C5C4;
extern s32 D_8013C700;
extern s32 D_8013C83C;
void trip_scene_script_resource_pointer_setup();
extern u8 *D_8013BFD0;
extern u8 *D_8013BFD4;
extern u8 *D_8013BFD8;
extern s16 D_8013BFDC;
extern u8 *D_8013BFE0;
extern u8 *D_8013BFE4;
extern u8 *D_8013BFE8;
extern u8 *D_8013BFEC;
extern u8 *D_8013BFF0;
extern u8 *D_8013BFF4;
extern u8 *D_8013BFF8;
extern u8 *D_8013C0C0;
extern u8 *D_8013C0C4;
extern u8 *D_8013C0C8;
extern s16 D_8013C0CC;
extern u8 *D_8013C0D0;
extern u8 *D_8013C0D4;
extern u8 *D_8013C0D8;
extern u8 *D_8013C0DC;
extern u8 *D_8013C0E0;
extern u8 *D_8013C0E4;
extern u8 *D_8013C0E8;
extern u8 *D_8013C190;
extern u8 *D_8013C194;
extern u8 *D_8013C198;
extern s16 D_8013C19C;
extern u8 *D_8013C1A0;
extern u8 *D_8013C1A4;
extern u8 *D_8013C1A8;
extern u8 *D_8013C1AC;
extern u8 *D_8013C1B0;
extern u8 *D_8013C1B4;
extern u8 *D_8013C1B8;
extern s16 D_8013BF18;
extern s16 D_8013BF1C;
extern s32 D_8013BF0C;
extern s32 D_8013BF10;
extern s32 D_8013BF14;
extern s32 D_8013C4BC;
extern s32 D_8013C4C0;
extern s32 D_8013C4C4;
extern s32 D_8013C4C8;
extern s32 D_8013C4CC;
extern s32 D_8013C4D0;
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
extern s32 D_8013C570;
extern s32 D_8013C574;
extern s32 D_8013C578;
extern s32 D_8013C57C;
extern s32 D_8013C580;
extern s32 D_8013C584;
extern s32 D_8013C588;
extern s32 D_8013C58C;
extern s32 D_8013C590;
extern s32 D_8013C594;
extern s32 D_8013C598;
extern s32 D_8013C59C;
extern s32 D_8013C5A0;
extern s32 D_8013C5A4;
extern s32 D_8013C5A8;
extern s32 D_8013C5AC;
extern s32 D_8013C5B0;
extern s32 D_8013C5B4;
extern s32 D_8013C5B8;
extern s32 D_8013C5BC;
extern s32 D_8013C5C0;
extern s32 D_8013C5C8;
extern s32 D_8013C5CC;
extern s32 D_8013C5D0;
extern s32 D_8013C5D4;
extern s32 D_8013C5D8;
extern s32 D_8013C5DC;
extern s32 D_8013C5E0;
extern s32 D_8013C5E4;
extern s32 D_8013C5E8;
extern s32 D_8013C5EC;
extern s32 D_8013C5F0;
extern s32 D_8013C5F4;
extern s32 D_8013C5F8;
extern s32 D_8013C5FC;
extern s32 D_8013C600;
extern s32 D_8013C604;
extern s32 D_8013C608;
extern s32 D_8013C60C;
extern s32 D_8013C610;
extern s32 D_8013C614;
extern s32 D_8013C618;
extern s32 D_8013C61C;
extern s32 D_8013C620;
extern s32 D_8013C624;
extern s32 D_8013C628;
extern s32 D_8013C62C;
extern s32 D_8013C630;
extern s32 D_8013C634;
extern s32 D_8013C638;
extern s32 D_8013C63C;
extern s32 D_8013C640;
extern s32 D_8013C644;
extern s32 D_8013C648;
extern s32 D_8013C64C;
extern s32 D_8013C650;
extern s32 D_8013C654;
extern s32 D_8013C658;
extern s32 D_8013C65C;
extern s32 D_8013C660;
extern s32 D_8013C664;
extern s32 D_8013C668;
extern s32 D_8013C66C;
extern s32 D_8013C670;
extern s32 D_8013C674;
extern s32 D_8013C678;
extern s32 D_8013C67C;
extern s32 D_8013C680;
extern s32 D_8013C684;
extern s32 D_8013C688;
extern s32 D_8013C68C;
extern s32 D_8013C690;
extern s32 D_8013C694;
extern s32 D_8013C698;
extern s32 D_8013C69C;
extern s32 D_8013C6A0;
extern s32 D_8013C6A4;
extern s32 D_8013C6A8;
extern s32 D_8013C6AC;
extern s32 D_8013C6B0;
extern s32 D_8013C6B4;
extern s32 D_8013C6B8;
extern s32 D_8013C6BC;
extern s32 D_8013C6C0;
extern s32 D_8013C6C4;
extern s32 D_8013C6C8;
extern s32 D_8013C6CC;
extern s32 D_8013C6D0;
extern s32 D_8013C6D4;
extern s32 D_8013C6D8;
extern s32 D_8013C6DC;
extern s32 D_8013C6E0;
extern s32 D_8013C6E4;
extern s32 D_8013C6E8;
extern s32 D_8013C6EC;
extern s32 D_8013C6F0;
extern s32 D_8013C6F4;
extern s32 D_8013C6F8;
extern s32 D_8013C6FC;
extern s32 D_8013C704;
extern s32 D_8013C708;
extern s32 D_8013C70C;
extern s32 D_8013C710;
extern s32 D_8013C714;
extern s32 D_8013C718;
extern s32 D_8013C71C;
extern s32 D_8013C720;
extern s32 D_8013C724;
extern s32 D_8013C728;
extern s32 D_8013C72C;
extern s32 D_8013C730;
extern s32 D_8013C734;
extern s32 D_8013C738;
extern s32 D_8013C73C;
extern s32 D_8013C740;
extern s32 D_8013C744;
extern s32 D_8013C748;
extern s32 D_8013C74C;
extern s32 D_8013C750;
extern s32 D_8013C754;
extern s32 D_8013C758;
extern s32 D_8013C75C;
extern s32 D_8013C760;
extern s32 D_8013C764;
extern s32 D_8013C768;
extern s32 D_8013C76C;
extern s32 D_8013C770;
extern s32 D_8013C774;
extern s32 D_8013C778;
extern s32 D_8013C77C;
extern s32 D_8013C780;
extern s32 D_8013C784;
extern s32 D_8013C788;
extern s32 D_8013C78C;
extern s32 D_8013C790;
extern s32 D_8013C794;
extern s32 D_8013C798;
extern s32 D_8013C79C;
extern s32 D_8013C7A0;
extern s32 D_8013C7A4;
extern s32 D_8013C7A8;
extern s32 D_8013C7AC;
extern s32 D_8013C7B0;
extern s32 D_8013C7B4;
extern s32 D_8013C7B8;
extern s32 D_8013C7BC;
extern s32 D_8013C7C0;
extern s32 D_8013C7C4;
extern s32 D_8013C7C8;
extern s32 D_8013C7CC;
extern s32 D_8013C7D0;
extern s32 D_8013C7D4;
extern s32 D_8013C7D8;
extern s32 D_8013C7DC;
extern s32 D_8013C7E0;
extern s32 D_8013C7E4;
extern s32 D_8013C7E8;
extern s32 D_8013C7EC;
extern s32 D_8013C7F0;
extern s32 D_8013C7F4;
extern s32 D_8013C7F8;
extern s32 D_8013C7FC;
extern s32 D_8013C800;
extern s32 D_8013C804;
extern s32 D_8013C808;
extern s32 D_8013C80C;
extern s32 D_8013C810;
extern s32 D_8013C814;
extern s32 D_8013C818;
extern s32 D_8013C81C;
extern s32 D_8013C820;
extern s32 D_8013C824;
extern s32 D_8013C828;
extern s32 D_8013C82C;
extern s32 D_8013C830;
extern s32 D_8013C834;
extern s32 D_8013C838;
extern s32 D_8013C840;
extern s32 D_8013C844;
extern s32 D_8013C848;
extern s32 D_8013C84C;
extern s32 D_8013C850;
extern s32 D_8013C854;
extern s32 D_8013C858;
extern s32 D_8013C85C;
extern s32 D_8013C860;
extern s32 D_8013C864;
extern s32 D_8013C868;
extern s32 D_8013C86C;
void trip_destination_actor_sector_load(void);
void trip_destination_actor_and_palette_setup(void);
s16 trip_destination_actor_slide_update(void);

extern s16 D_8013C2E8;
void trip_destination_exit_callback_dispatch(void);
s32 trip_background_header_validate_and_decode(void);
void trip_sightseeing_mode_step_dispatch(void);
extern u8 D_8013B090;
void trip_departure_mode_step_callback_dispatch(void);
void trip_yukari_mode_step_callback_dispatch(void);

void trip_companion_dialogue_pointer_bind(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
extern s16 D_8013C87C;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void trip_response_talk_run(void);

/* Phase handlers that trip_mode_stage_frame_dispatch switches over. */
void trip_callback_dispatch(void);
void trip_mode_step_callback_dispatch(void);
void trip_ayako_mode_step_callback_dispatch(void);
void trip_saved_mode_stage_corridor_dispatch(void);
s32 trip_yuina_callback_dispatch_and_actor_fade(void);
void trip_destination_one_mode_step_callback_dispatch(void);
void trip_sightseeing_callback_dispatch_alt(void);
void trip_destination_outing_callback_dispatch(void);

extern s16 D_8013C970;
extern u8 D_8013C978;
extern u8 D_8013C980;
void trip_current_girl_destination_pointer_bind(void);

#endif /* OVL_SHUGAKU_H */

#ifndef T9540_SHUGAKU_FN68
#define T9540_SHUGAKU_FN68
typedef struct ShugakuFn68 { /* 0x00 */ void (*f[68])(); } ShugakuFn68; /* size 0x110 */
extern ShugakuFn68 D_8013CB08;
extern u8 D_8013CB04;
void trip_default_destination_talk_run();
#endif

#ifndef OVL_SHUGAKU_CALLBACK_T9610
#define OVL_SHUGAKU_CALLBACK_T9610
/* Complete stack copy in trip_destination_one_mode_step_callback_dispatch. */
typedef struct ShugakuCallbacks71 {
    /* 0x00 */ void (*callback[71])();
} ShugakuCallbacks71; /* size 0x11C */
extern ShugakuCallbacks71 D_8013C984;
#endif

#ifndef OVL_SHUGAKU_DISPATCH_T10020
#define OVL_SHUGAKU_DISPATCH_T10020
/* FAKE: the additional 0x10 bytes reproduce the original 0x158 frame.
 * The source purpose of that unused extent is unknown. T-10020. */
typedef struct ShugakuCallbackLocal12C {
    /* 0x000 */ ShugakuCallbacks71 callbacks;
    /* 0x11C */ u8 pad_11C[0x10];
} ShugakuCallbackLocal12C; /* size 0x12C */
typedef struct ShFnTbl74 { void (*f[74])(); } ShFnTbl74;
extern ShFnTbl74 D_8013C2F0;
s32 trip_yuina_actor_brightness_sync(void);
s32 trip_yuina_actor_effect_and_projectile_update();
#endif

#ifndef OVL_SHUGAKU_FNTBL_T18000
#define OVL_SHUGAKU_FNTBL_T18000
/* T-18000: callback tables. Each dispatcher copies its table whole to the stack and calls one
 * entry through the copy (T-3330). The typedefs were local to the .c files before T-18000;
 * D_8013C008 (trip_ayako_mode_step_callback_dispatch) and D_8013C228 (trip_corridor_callback_dispatch_and_actor_fade) are new. ShFnTbl74 is above. */
typedef struct TripSecondaryCallbacks { void (*f[16])(); } TripSecondaryCallbacks; /* size 0x40 */
typedef struct TripReturnCallbacks { void (*f[18])(); } TripReturnCallbacks; /* size 0x48 */
typedef struct TripSceneCallbacks { void (*f[25])(); } TripSceneCallbacks; /* size 0x64 */
typedef struct TripSceneTransitionCallbacks { void (*f[38])(); } TripSceneTransitionCallbacks; /* size 0x98 */
typedef struct TripAlternateSceneCallbacks { void (*f[41])(); } TripAlternateSceneCallbacks; /* size 0xA4 */
typedef struct TripTravelSceneCallbacks { void (*f[43])(); } TripTravelSceneCallbacks; /* size 0xAC */
typedef struct TripSceneStepCallbacks { void (*f[44])(); } TripSceneStepCallbacks; /* size 0xB0 */
typedef struct TripModeStepCallbacks { void (*f[70])(); } TripModeStepCallbacks; /* size 0x118 */
extern TripModeStepCallbacks D_8013B094;
extern TripTravelSceneCallbacks D_8013BF20;
extern TripSceneStepCallbacks D_8013C008;
extern TripSceneTransitionCallbacks D_8013C0EC;
extern TripSceneCallbacks D_8013C1C4;
extern TripAlternateSceneCallbacks D_8013C228;
extern TripAlternateSceneCallbacks D_8013C884;
extern TripSecondaryCallbacks D_8013C930;
extern TripReturnCallbacks D_8013CAA0;
extern TripSceneCallbacks D_8013CC20;
/* T-27066: trip_girl_scene_select uses month >= 4 to choose the year offset before subtracting 0x5F or 0x60; birthday_detours_and_talk.c:269-274. */
#define TRIP_YEAR_OFFSET_MONTH_BOUNDARY 4

/* T-27075 group 4: SHUGAKU/ayako_scene_and_birthday_detours.c:82,108 compare the liberal-arts parameter value with the proven eligibility threshold 120. */
#define TRIP_LIBERAL_ARTS_THRESHOLD 120 /* Shared >=120 eligibility threshold in the two Ayako trip callbacks. */

/* T-27075 group 5: SHUGAKU/birthday_detours_and_talk.c:303-309; saved_mode_stage == 6 takes the early branch and adds 6 to mode_step. Name only the observed increment. */
#define TRIP_SAVED_MODE_STEP_DELTA 6 /* Observed added mode_step amount when saved_mode_stage equals 6; destination or narrative meaning not inferred. */

#endif
