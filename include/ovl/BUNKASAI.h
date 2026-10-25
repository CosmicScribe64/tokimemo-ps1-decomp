#ifndef OVL_BUNKASAI_H
#define OVL_BUNKASAI_H

#include "common.h"
#include "game.h"

/* BUNKASAI overlay (load address 0x80132000): externs and types. */

/* T-24030: side-effect-only helpers called by culture_festival_mode_handler. */
void culture_phase_next_on_step(void);
void culture_event_log_add(void);

extern u32 culture_script_text_table; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 culture_script_command_table;
extern u32 culture_script_voice_table;

extern s32 culture_backstage_title;

/* T-18700: 33-byte strings by school year (95..97) and D_80122CDC, copied to the stack and
 * passed to strcpy by culture_birthday_choice_name_set (DATE2 func_80137A2C form). */
typedef struct CultureChoiceNames {
    /* 0x000 */ u8 names[3][3][33];
} CultureChoiceNames; /* size 0x129 */
extern CultureChoiceNames culture_choice_names;
void culture_birthday_choice_name_set(void);

/* T-18700: text-pointer tables copied to the stack by culture_fortune_scene (BUNKAKEN func_80152680 form). */
typedef struct CultureFortuneGirlNames {
    /* 0x00 */ u8 *text[12];
} CultureFortuneGirlNames; /* size 0x30 */
typedef struct CultureFortuneBloodLabels {
    /* 0x00 */ u8 *text[11];
} CultureFortuneBloodLabels; /* size 0x2C */
typedef struct CultureBloodTypeLabels {
    /* 0x00 */ u8 *text[4];
} CultureBloodTypeLabels; /* size 0x10 */
extern CultureFortuneGirlNames culture_fortune_girl_names;
extern CultureFortuneBloodLabels culture_fortune_girl_blood_labels;
extern CultureBloodTypeLabels culture_blood_type_labels;
extern u8 culture_fortune_timer; /* frame counter of culture_fortune_scene */
s32 culture_fortune_lookup(void);

/* T-18700: per-girl byte tables copied to the stack. culture_festival_mode_handler reads one byte per girl
 * (0..10); culture_result_apply picks one 13-by-6 table per school year and a column through the
 * six-byte selector. */
typedef struct CultureUnusedBytes {
    /* 0x00 */ u8 v[11];
} CultureUnusedBytes; /* size 0x0B */
typedef struct CultureClubParamIndex {
    /* 0x00 */ u8 param_index[6];
} CultureClubParamIndex; /* size 0x06 */
typedef struct CultureTokimekiGains {
    /* 0x00 */ u8 gain[13][6];
} CultureTokimekiGains; /* size 0x4E */
extern CultureUnusedBytes culture_unused_bytes;
extern CultureClubParamIndex culture_club_param_index;
extern CultureTokimekiGains culture_tokimeki_gain_95; /* first year */
extern CultureTokimekiGains culture_tokimeki_gain_96; /* second year */
extern CultureTokimekiGains culture_tokimeki_gain_97; /* third year */

/* T-18700: eleven words (all 0x80) copied to the stack by culture_visitor_pick, which fills it with the
 * indices of the girls whose GirlRecord flag bit 1 is set and picks one at random. */
typedef struct CultureGirlList {
    /* 0x00 */ s32 girl[11];
} CultureGirlList; /* size 0x2C */
extern CultureGirlList culture_visitor_list_init;

/* defined in C in one object, called from another (T-0500) */
void culture_script_set_shiori_visit(void);
void culture_script_set_shiori_member(void);
void culture_script_set_mio_visit_a(void);
void culture_script_set_mio_visit_b(void);
void culture_script_set_mio_member(void);
void culture_script_set_yuina_visit(void);
void culture_script_set_yuina_member(void);
void culture_script_set_ayako_visit_a(void);
void culture_script_set_ayako_visit_b(void);
void culture_script_set_ayako_visit_c(void);
void culture_script_set_ayako_member(void);
void culture_script_set_saki_visit(void);
void culture_script_set_yukari_visit(void);
void culture_script_set_nozomi_visit(void);
void culture_script_set_mira_visit(void);
void culture_script_set_yuko_visit(void);
void culture_script_set_megumi_visit(void);
void culture_script_set_yumi_visit(void);
extern s32 culture_birthday_text_table_00;
extern s32 culture_birthday_text_table_01;
extern s32 culture_birthday_text_table_02;
extern s32 culture_birthday_text_table_03;
extern s32 culture_birthday_text_table_04;
extern s32 culture_birthday_text_table_05;
extern s32 culture_birthday_text_table_06;
extern s32 culture_birthday_text_table_07;
extern s32 culture_birthday_text_table_08;
extern s32 culture_birthday_text_table_09;
extern s32 culture_birthday_text_table_10;
extern s32 culture_birthday_text_table_11;
extern s32 culture_birthday_text_table_12;
extern s32 culture_birthday_command_table_00;
extern s32 culture_birthday_command_table_01;
extern s32 culture_birthday_command_table_02;
extern s32 culture_birthday_command_table_03;
extern s32 culture_birthday_command_table_04;
extern s32 culture_birthday_command_table_05;
extern s32 culture_birthday_command_table_06;
extern s32 culture_birthday_command_table_07;
extern s32 culture_birthday_command_table_08;
extern s32 culture_birthday_command_table_09;
extern s32 culture_birthday_command_table_10;
extern s32 culture_birthday_command_table_11;
extern s32 culture_birthday_command_table_12;
extern s32 culture_birthday_voice_table_00;
extern s32 culture_birthday_voice_table_01;
extern s32 culture_birthday_voice_table_02;
extern s32 culture_birthday_voice_table_03;
extern s32 culture_birthday_voice_table_04;
extern s32 culture_birthday_voice_table_05;
extern s32 culture_birthday_voice_table_06;
extern s32 culture_birthday_voice_table_07;
extern s32 culture_birthday_voice_table_08;
extern s32 culture_birthday_voice_table_09;
extern s32 culture_birthday_voice_table_10;
extern s32 culture_birthday_voice_table_11;
extern s32 culture_birthday_voice_table_12;


/* T-11190: declarations added by tools/bulk_m2c.py. */
void culture_mio_member_script_start(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void culture_ayako_member_script_start(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void culture_yuina_member_script_start(void);

#endif

#ifndef OVL_BUNKASAI_MODE_T9740
#define OVL_BUNKASAI_MODE_T9740
void culture_yuina_member_entry(void);
#endif

#ifndef BUNKASAI_TABLE40_T9940
#define BUNKASAI_TABLE40_T9940
typedef struct CultureFortuneBloodTable {
    /* 0x00 */ s32 result[4][4];
} CultureFortuneBloodTable; /* size 0x40; copied by culture_fortune_lookup */
extern CultureFortuneBloodTable culture_fortune_blood_table;
#endif


#ifndef OVL_BUNKASAI_T9960
#define OVL_BUNKASAI_T9960

void culture_yuko_visit_script_start(void);
void culture_nozomi_visit_script_start(void);
#endif


#ifndef OVL_BUNKASAI_T10040
#define OVL_BUNKASAI_T10040
/* Byte timer: lbu/sb in culture_shiori_member_computer_game, T-10040. */
extern u8 culture_line_wait_timer;

/* Forward declarations for the original object, T-10040. */
void culture_shiori_member_computer_game(void);
void culture_shiori_member_script_start(void);
void culture_mio_visit_literature_exhibit(void);
void culture_mio_visit_drama_play(void);
void culture_mio_visit_experiment_show_95(void);
void culture_mio_visit_experiment_show_96(void);
void culture_mio_visit_experiment_show_97(void);
void culture_mio_visit_computer_fortune(void);
void culture_mio_visit_computer_game(void);
void culture_mio_visit_oil_painting_exhibit(void);
void culture_mio_visit_concert(void);
void culture_mio_visit_script_a_start(void);
void culture_mio_visit_script_b_start(void);
void culture_megumi_visit_computer_game(void);
void culture_megumi_visit_script_start(void);

/* Forward calls for guarded dispatchers, T-10040. */
void culture_mio_visit_open_sketching(void);
void culture_megumi_visit_speech_contest(void);
void culture_megumi_visit_drama_play(void);
void culture_megumi_visit_experiment_show(void);
void culture_megumi_visit_open_sketching(void);
void culture_megumi_visit_oil_painting_exhibit(void);
void culture_megumi_visit_concert(void);

void culture_shiori_member_entry(void);

void culture_shiori_member_experiment_show(void);

void culture_mio_visit_entry(void);

void culture_mio_visit_speech_contest(void);

void culture_megumi_visit_entry(void);

void culture_megumi_visit_literature_exhibit(void);

void culture_shiori_member_literature_exhibit(void);
#endif

/* T-10630: shared dialogue continuation in the 8013C430 object. */
void culture_ayako_visit_script_b_start(void);
void culture_ayako_visit_script_a_start(void);
void culture_ayako_visit_script_c_start(void);


/* T-10630: scene helper has no return value; callers use its side effects. */
void culture_exhibit_title_show(void);


#ifndef OVL_BUNKASAI_T10830_H
#define OVL_BUNKASAI_T10830_H
/* Side-effect-only scene table setters defined later in their objects. */
void culture_yuina_visit_script_start(void);
void culture_yumi_visit_script_start(void);
void culture_mira_visit_script_start(void);
void culture_yukari_visit_script_start(void);
#endif
#ifndef OVL_BUNKASAI_T10860_H
#define OVL_BUNKASAI_T10860_H
void culture_shiori_visit_computer_game(void);
void culture_saki_visit_computer_game(void);
void culture_shiori_visit_script_start(void);
/* T-27065 reviewed stage names; private-header users and main stage setter */
enum CultureStage {
    CULTURE_STAGE_INTRO = 0,
    CULTURE_STAGE_SHIORI_MEMBER = 1,
    CULTURE_STAGE_FORTUNE_SHIORI_MEMBER = 7,
    CULTURE_STAGE_VISITOR_PICK = 14,
    CULTURE_STAGE_SHIORI_VISIT = 15,
    CULTURE_STAGE_FORTUNE_SHIORI_VISIT = 21,
    CULTURE_STAGE_MIO_VISIT = 28,
    CULTURE_STAGE_FORTUNE_MIO_VISIT = 36,
    CULTURE_STAGE_MIO_MEMBER = 42,
    CULTURE_STAGE_YUINA_VISIT = 46,
    CULTURE_STAGE_FORTUNE_YUINA_VISIT = 52,
    CULTURE_STAGE_YUINA_MEMBER = 59,
    CULTURE_STAGE_FORTUNE_YUINA_MEMBER = 62,
    CULTURE_STAGE_AYAKO_VISIT = 65,
    CULTURE_STAGE_FORTUNE_AYAKO_VISIT = 71,
    CULTURE_STAGE_AYAKO_FOLLOWUP = 79,
    CULTURE_STAGE_AYAKO_MEMBER = 80,
    CULTURE_STAGE_SAKI_VISIT = 85,
    CULTURE_STAGE_FORTUNE_SAKI_VISIT = 91,
    CULTURE_STAGE_YUKARI_VISIT = 97,
    CULTURE_STAGE_FORTUNE_YUKARI_VISIT = 103,
    CULTURE_STAGE_NOZOMI_VISIT = 109,
    CULTURE_STAGE_FORTUNE_NOZOMI_VISIT = 115,
    CULTURE_STAGE_MIRA_VISIT = 121,
    CULTURE_STAGE_FORTUNE_MIRA_VISIT = 127,
    CULTURE_STAGE_YUKO_VISIT = 133,
    CULTURE_STAGE_FORTUNE_YUKO_VISIT = 139,
    CULTURE_STAGE_MEGUMI_VISIT = 145,
    CULTURE_STAGE_YUMI_VISIT = 154,
    CULTURE_STAGE_GAME_RESUME = 163,
    CULTURE_STAGE_GAME_LAUNCH = 164
};

#endif

/* T-10860: forward call into the owned 801362A0 object. */
void culture_saki_visit_script_start(void);
