#ifndef OVL_OMIMAI_H
#define OVL_OMIMAI_H

#include "common.h"
#include "game.h"

/* OMIMAI overlay (load address 0x80132000): externs and types. */

void sick_visit_girl_talk_pointer_select(void);
void sick_visit_mode_stage1_girl_talk_pointer_setup();
void sick_visit_mode_stage1_talk_table_select();
void sick_visit_common_mode_girl_talk_pointer_setup();
void sick_visit_mode_stage0_girl_talk_pointer_setup();
extern s32 D_80134A8C;
extern s32 D_80134ABC;
extern s32 D_80134AEC;
void sick_visit_common_mode_step_callback_dispatch();
void sick_visit_day_event_girl_setup();
void sick_visit_common_girl_talk_pointer_select(void);

/* three parallel tables per object (13 words each), indexed by a fixed slot per value of
 * game_state.unk_F5F (sick_visit_girl_talk_pointer_select, sick_visit_mode_stage1_talk_table_select, sick_visit_common_girl_talk_pointer_select; T-0500) */
extern s32 D_80134A60[];
extern s32 D_80134A90[];
extern s32 D_80134AC0[];
extern s32 D_80134B60[];
extern s32 D_80134B94[];
extern s32 D_80134BC8[];
extern s32 D_80134C30[];
extern s32 D_80134C64[];
extern s32 D_80134C98[];
extern s32 D_80134C5C;
extern s32 D_80134C90;
extern s32 D_80134CC4;
extern u8 D_80134CCC;
s32 sick_visit_day_event_complete(void);
s32 sick_visit_common_day_event_complete(void);

/* Girl-indexed value tables of sick_visit_portrait_select (copied to the stack, T-19012). */
typedef struct OmimaiWords11 {
    /* 0x00 */ s32 unk_00[11];
} OmimaiWords11; /* size 0x2C */
typedef struct OmimaiWords2 {
    /* 0x00 */ s32 unk_00[2];
} OmimaiWords2; /* size 0x08 */
extern OmimaiWords11 D_80134D38;
extern OmimaiWords2 D_80134D64;
extern OmimaiWords2 D_80134D6C;
extern OmimaiWords2 D_80134D74;

extern s32 D_80134C34;
extern s32 D_80134C38;
extern s32 D_80134C3C;
extern s32 D_80134C40;
extern s32 D_80134C44;
extern s32 D_80134C48;
extern s32 D_80134C4C;
extern s32 D_80134C50;
extern s32 D_80134C54;
extern s32 D_80134C58;
extern s32 D_80134C60;
extern s32 D_80134C68;
extern s32 D_80134C6C;
extern s32 D_80134C70;
extern s32 D_80134C74;
extern s32 D_80134C78;
extern s32 D_80134C7C;
extern s32 D_80134C80;
extern s32 D_80134C84;
extern s32 D_80134C88;
extern s32 D_80134C8C;
extern s32 D_80134C94;
extern s32 D_80134C9C;
extern s32 D_80134CA0;
extern s32 D_80134CA4;
extern s32 D_80134CA8;
extern s32 D_80134CAC;
extern s32 D_80134CB0;
extern s32 D_80134CB4;
extern s32 D_80134CB8;
extern s32 D_80134CBC;
extern s32 D_80134CC0;
extern s32 D_80134CC8;

#endif /* OVL_OMIMAI_H */
