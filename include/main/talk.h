#ifndef MAIN_TALK_H
#define MAIN_TALK_H

#include "common.h"

/* T-10730: dialogue voice table leaves, indexed with an eight-byte stride. */
typedef struct TalkVoiceRef {
    /* 0x00 */ s32 clip_start;
    /* 0x04 */ s32 clip_end;
} TalkVoiceRef; /* size 0x08 */

/* T-10730: sixteen byte selectors copied to the dialogue handler's stack. */
typedef struct TalkSelectors {
    /* 0x00 */ u8 value[16];
} TalkSelectors; /* size 0x10 */

/* T-10730: minimum dialogue work range at talk_work. Reset clears words
 * 5..30; retain existing scalar/array declarations as views in older users. */
typedef struct TalkWork {
    /* 0x00 */ s32 scene;
    /* 0x04 */ s32 girl;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 choice;
    /* 0x10 */ s32 result_code;
    /* 0x14 */ s32 line_wait;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 choice_saved;
    /* 0x20 */ s32 voice_state;
    /* 0x24 */ s32 girl_visible;
    /* 0x28 */ s32 bg_visible;
    /* 0x2C */ s32 text_delay;
    /* 0x30 */ s32 line_auto;
    /* 0x34 */ s32 event_still_visible;
    /* 0x38 */ s32 date_spot;
    /* 0x3C */ s32 event_eyes_hold; /* HYPOTHESIS: alias of scene_event_eyes_hold */
    /* 0x40 */ s32 event_mouth_anim;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ s32 line_clear;
    /* 0x4C */ s32 text_silent;
    /* 0x50 */ s32 mouth_lock; /* HYPOTHESIS: alias of scene_mouth_lock */
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s32 row_offset; /* HYPOTHESIS: dialogue rows shift by 16 * this value; no writer outside talk_clear, which sets it to zero */
    /* 0x5C */ s32 cursor_hide;
    /* 0x60 */ s32 menu_active;
    /* 0x64 */ s32 unk_64;
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 wipe_kind;
    /* 0x70 */ s32 girl_selector_plus_one;
    /* 0x74 */ s32 unk_74;
    /* 0x78 */ s32 rank_override;
} TalkWork; /* minimum size 0x7C */

/* T-10730: pointer slots at talk_tables..talk_active_voice_table. The cursor pointers
 * address signed halfwords; legacy scalar declarations remain valid views. */
typedef struct TalkTables {
    /* 0x00 */ s16 *group;
    /* 0x04 */ s16 *line;
    /* 0x08 */ u8 ***text;
    /* 0x0C */ u8 **command;
    /* 0x10 */ TalkVoiceRef ***voice;
} TalkTables; /* size 0x14 on PS1 */

extern u8 *talk_tables;
extern s16 talk_group;  /* also address-taken by the date code */
extern s16 talk_line;
extern s32 talk_text_table;
extern s32 talk_command_table;
extern s32 talk_voice_table;
#ifndef MAIN_API_OVERRIDE_talk_work
extern s32 talk_work;
#endif
void talk_voice_play();
#ifndef MAIN_API_OVERRIDE_talk_render
/* K&R arguments are narrowed by the callee; its renderer result is returned. T-20007. */
s32 talk_render();
#endif
s32 talk_run();
#ifndef MAIN_API_OVERRIDE_talk_names_set
void talk_names_set(u8 girl_id);
#endif
void talk_clear();

extern u8 talk_choice_order[4]; /* [1], [2] are the former D_800CA2A5/6 */

extern s32 D_80122D1C;

extern u8 D_800CA2A5;
extern u8 D_800CA2A6;
void check_k_scroll();
void talk_voice_skip_check(void);
s32 make_three_select();
void don_wait(void);

extern u8 talk_voice_cancelled;
extern s32 D_800CA2C8;

#endif /* MAIN_TALK_H */
