#ifndef OVL_BUNKAKEN_H
#define OVL_BUNKAKEN_H

#include "common.h"
#include "game.h"

/* BUNKAKEN overlay (load address 0x80132000): externs and types. */

extern u32 tour_script_text; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 tour_script_commands;
extern u32 tour_script_voices;

extern s32 tour_title_backstage;
extern s32 tour_title_blank;

/* defined in C in one object, called from another (T-0500) */
void tour_script_shiori_club(void);
void tour_script_mio_club(void);
void tour_script_yuina_club(void);
void tour_script_ayako_club(void);
void tour_script_saki(void);
void tour_script_nozomi(void);
void tour_script_mira(void);
void tour_script_yuko(void);
void tour_script_megumi(void);
void tour_script_yumi(void);
void tour_script_common(void);
/* T-18500: three 13-word tables of text addresses (in the loaded scene data at 0x801B0000), filled by
 * tour_birthday_tables_init; tour_phase_player_birthday copies row k of each table into talk_text_table/164/168. */
extern s32 tour_birthday_script[3][13];

/* tour_fortune_result copies a 4-by-4 word grid and indexes two two-bit fields. */
typedef struct TourFortuneGrid { /* size 0x40 */
    /* 0x00 */ s32 result_by_blood[4][4];
} TourFortuneGrid;
extern TourFortuneGrid tour_fortune_grid;

/* T-11190: declarations added by tools/bulk_m2c.py. */
void tour_solo_script_begin(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void tour_saki_script_begin(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void tour_ayako_club_script_begin(void);

/* T-12701: forward declaration for the scene handlers. */
void tour_yuina_club_script_begin(void);

#endif

#ifndef OVL_BUNKAKEN_T9960
#define OVL_BUNKAKEN_T9960

void tour_yuko_script_begin(void);

void tour_shiori_club_script_begin(void);

/* Local text-pointer tables copied by tour_stage_fortune_telling (T-9960). */
typedef struct TourFortuneNames {
    /* 0x00 */ u8 *names[12];
} TourFortuneNames; /* size 0x30 */
typedef struct TourFortuneBloodLabels {
    /* 0x00 */ u8 *blood_labels[11];
} TourFortuneBloodLabels; /* size 0x2C */
typedef struct TourFortunePlayerLabels {
    /* 0x00 */ u8 *blood_labels[4];
} TourFortunePlayerLabels; /* size 0x10 */
extern TourFortuneNames tour_fortune_girl_names;
extern TourFortuneBloodLabels tour_fortune_girl_blood_labels;
extern TourFortunePlayerLabels tour_fortune_player_blood_labels;
extern u8 tour_fortune_wait_counter;
s32 tour_fortune_result(void);
#endif


#ifndef OVL_BUNKAKEN_T10860_H
#define OVL_BUNKAKEN_T10860_H
void tour_stage_yukari_computer_game(void);
void tour_yukari_script_begin(void);
void tour_finish_visit(void);
void tour_stage_saki_route(void);
void tour_stage_mira_route(void);

#endif

/* T-10860: forward call within 80132000.c. */
void tour_shiori_companion_script_begin(void);
void tour_yumi_script_begin(void);
void tour_nozomi_script_begin(void);
void tour_mira_script_begin(void);
void tour_megumi_script_begin(void);

/* T-18500: forward calls of tour_entry within 80144F70.c. */
void tour_phase_next_on_step(void);
void tour_apply_points(void);
void tour_event_log_add(void);

/* T-11390: forward calls within 80143DD0.c. */
void tour_club_menu_init(void);
void tour_club_menu_update(void);

#ifndef OVL_BUNKAKEN_T12300_DECLS
#define OVL_BUNKAKEN_T12300_DECLS
/* T-12300: scene handlers call the existing definition in 8013BFE0.c. */
extern u8 tour_scratch_bytes;

void tour_mio_club_script_begin(void);
#endif

#ifndef OVL_BUNKAKEN_T18500_H
#define OVL_BUNKAKEN_T18500_H
/* T-18500: per-heroine tables of the date loaders tour_stage_invitation and tour_stage_ask_companion, indexed by
 * D_80122EBC (12 heroines). */
extern s32 tour_portrait_lba[12];           /* argument of portrait_read_step */
extern void (*tour_invite_script_setter[12])(void); /* called once the scene data is loaded */
extern u8 tour_invite_script_sectors[12];            /* first argument of cd_read_sectors */
extern s32 tour_invite_script_lba[12];           /* sector of cd_read_sectors */
extern u8 tour_portrait_layout[20];            /* argument of portrait_layout_set; the last 8 bytes are zero */

/* T-18500: 11-byte table copied to the stack by tour_entry (lw, lw, lwl/lwr), indexed by D_80122EC0. */
typedef struct TourUnusedBytes {
    /* 0x00 */ u8 b[11];
} TourUnusedBytes; /* size 0x0B */
extern TourUnusedBytes tour_unused_table;

/* T-18500: three byte tables copied to the stack by tour_apply_points and read at
 * [D_80122EBC][D_80122EB8]; the table is chosen by GameState.unk_03E - 0x5F. */
typedef struct TourPoints1995 {
    /* 0x00 */ u8 codes[9][6];
} TourPoints1995; /* size 0x36 */
typedef struct TourPointsLaterYears {
    /* 0x00 */ u8 codes[11][6];
} TourPointsLaterYears; /* size 0x42 */
extern TourPoints1995 tour_points_1995;
extern TourPointsLaterYears tour_points_1996;
extern TourPointsLaterYears tour_points_1997;

/* T-18500: 3 x 3 strings of 33 bytes copied to the stack by tour_birthday_option_name_set and read at
 * [GameState.unk_03E - 0x5F][D_80122CDC]. */
typedef struct TourOptionNames {
    /* 0x00 */ u8 names[3][3][33];
} TourOptionNames; /* size 0x129 */
extern TourOptionNames tour_birthday_option_names;
/* T-27065 reviewed stage names; private-header users and main stage setter */
enum TourStage {
    TOUR_STAGE_INVITATION = 0,
    TOUR_STAGE_ASK_COMPANION = 1,
    TOUR_STAGE_YOSHIO = 2,
    TOUR_STAGE_CLUB_MENU = 3,
    TOUR_STAGE_ROUTE_VISIT = 4,
    TOUR_STAGE_SOLO_ROUTE = 5,
    TOUR_STAGE_SOLO_LITERATURE_COLLECTION = 6,
    TOUR_STAGE_SOLO_LITERATURE_SPEECH = 7,
    TOUR_STAGE_SOLO_DRAMA = 8,
    TOUR_STAGE_SOLO_SCIENCE = 9,
    TOUR_STAGE_SOLO_COMPUTER_FORTUNE = 10,
    TOUR_STAGE_FORTUNE_TELLING = 11,
    TOUR_STAGE_SOLO_COMPUTER_GAME = 12,
    TOUR_STAGE_SOLO_COMPUTER_EXPERIMENT = 13,
    TOUR_STAGE_SOLO_ART_PORTRAIT = 14,
    TOUR_STAGE_SOLO_ART_SKETCH = 15,
    TOUR_STAGE_SOLO_ART_OIL = 16,
    TOUR_STAGE_SOLO_BRASS = 17,
    TOUR_STAGE_SHIORI_COMPANION_ROUTE = 18,
    TOUR_STAGE_SHIORI_COMPANION_LITERATURE_POEMS = 19,
    TOUR_STAGE_SHIORI_COMPANION_LITERATURE_ANTHOLOGY = 20,
    TOUR_STAGE_SHIORI_COMPANION_LITERATURE_SPEECH = 21,
    TOUR_STAGE_SHIORI_COMPANION_DRAMA = 22,
    TOUR_STAGE_SHIORI_COMPANION_SCIENCE = 23,
    TOUR_STAGE_SHIORI_COMPANION_COMPUTER_FORTUNE = 24,
    TOUR_STAGE_SHIORI_COMPANION_COMPUTER_GAME = 25,
    TOUR_STAGE_SHIORI_COMPANION_COMPUTER_EXPERIMENT = 26,
    TOUR_STAGE_SHIORI_COMPANION_ART_PORTRAIT = 27,
    TOUR_STAGE_SHIORI_COMPANION_ART_SKETCH = 28,
    TOUR_STAGE_SHIORI_COMPANION_ART_OIL = 29,
    TOUR_STAGE_SHIORI_COMPANION_BRASS = 30,
    TOUR_STAGE_SHIORI_COMPANION_BRASS_1997 = 31,
    TOUR_STAGE_SHIORI_CLUB_ROUTE = 32,
    TOUR_STAGE_SHIORI_CLUB_LITERATURE_COLLECTION = 33,
    TOUR_STAGE_SHIORI_CLUB_LITERATURE_SPEECH = 34,
    TOUR_STAGE_SHIORI_CLUB_DRAMA = 35,
    TOUR_STAGE_SHIORI_CLUB_SCIENCE = 36,
    TOUR_STAGE_SHIORI_CLUB_COMPUTER_FORTUNE = 37,
    TOUR_STAGE_SHIORI_CLUB_COMPUTER_GAME = 38,
    TOUR_STAGE_SHIORI_CLUB_COMPUTER_EXPERIMENT = 39,
    TOUR_STAGE_SHIORI_CLUB_ART_PORTRAIT = 40,
    TOUR_STAGE_SHIORI_CLUB_ART_SKETCH = 41,
    TOUR_STAGE_SHIORI_CLUB_ART_OIL = 42,
    TOUR_STAGE_SHIORI_CLUB_BRASS = 43,
    TOUR_STAGE_MIO_CLUB_ROUTE = 44,
    TOUR_STAGE_MIO_CLUB_LITERATURE_COLLECTION = 45,
    TOUR_STAGE_MIO_CLUB_LITERATURE_SPEECH = 46,
    TOUR_STAGE_MIO_CLUB_DRAMA = 47,
    TOUR_STAGE_YUINA_CLUB_ROUTE = 48,
    TOUR_STAGE_YUINA_CLUB_SCIENCE = 49,
    TOUR_STAGE_YUINA_CLUB_COMPUTER_FORTUNE = 50,
    TOUR_STAGE_YUINA_CLUB_COMPUTER_GAME = 51,
    TOUR_STAGE_YUINA_CLUB_COMPUTER_EXPERIMENT = 52,
    TOUR_STAGE_AYAKO_CLUB_ROUTE = 53,
    TOUR_STAGE_AYAKO_CLUB_ART_PORTRAIT = 54,
    TOUR_STAGE_AYAKO_CLUB_ART_SKETCH = 55,
    TOUR_STAGE_AYAKO_CLUB_ART_OIL = 56,
    TOUR_STAGE_AYAKO_CLUB_BRASS = 57,
    TOUR_STAGE_SAKI_ROUTE = 58,
    TOUR_STAGE_SAKI_LITERATURE_POEMS = 59,
    TOUR_STAGE_SAKI_LITERATURE_ANTHOLOGY = 60,
    TOUR_STAGE_SAKI_LITERATURE_SPEECH = 61,
    TOUR_STAGE_SAKI_DRAMA = 62,
    TOUR_STAGE_SAKI_SCIENCE = 63,
    TOUR_STAGE_SAKI_COMPUTER_FORTUNE = 64,
    TOUR_STAGE_SAKI_COMPUTER_GAME = 65,
    TOUR_STAGE_SAKI_COMPUTER_EXPERIMENT = 66,
    TOUR_STAGE_SAKI_ART_PORTRAIT = 67,
    TOUR_STAGE_SAKI_ART_SKETCH = 68,
    TOUR_STAGE_SAKI_ART_OIL = 69,
    TOUR_STAGE_SAKI_BRASS = 70,
    TOUR_STAGE_SAKI_BRASS_1997 = 71,
    TOUR_STAGE_YUKARI_ROUTE = 72,
    TOUR_STAGE_YUKARI_LITERATURE_POEMS = 73,
    TOUR_STAGE_YUKARI_LITERATURE_ANTHOLOGY = 74,
    TOUR_STAGE_YUKARI_LITERATURE_SPEECH = 75,
    TOUR_STAGE_YUKARI_DRAMA = 76,
    TOUR_STAGE_YUKARI_SCIENCE = 77,
    TOUR_STAGE_YUKARI_COMPUTER_FORTUNE = 78,
    TOUR_STAGE_YUKARI_COMPUTER_GAME = 79,
    TOUR_STAGE_YUKARI_COMPUTER_EXPERIMENT = 80,
    TOUR_STAGE_YUKARI_ART_PORTRAIT = 81,
    TOUR_STAGE_YUKARI_ART_SKETCH = 82,
    TOUR_STAGE_YUKARI_ART_OIL = 83,
    TOUR_STAGE_YUKARI_BRASS = 84,
    TOUR_STAGE_NOZOMI_ROUTE = 85,
    TOUR_STAGE_NOZOMI_LITERATURE_POEMS = 86,
    TOUR_STAGE_NOZOMI_LITERATURE_ANTHOLOGY = 87,
    TOUR_STAGE_NOZOMI_LITERATURE_SPEECH = 88,
    TOUR_STAGE_NOZOMI_DRAMA = 89,
    TOUR_STAGE_NOZOMI_SCIENCE = 90,
    TOUR_STAGE_NOZOMI_COMPUTER_FORTUNE = 91,
    TOUR_STAGE_NOZOMI_COMPUTER_GAME = 92,
    TOUR_STAGE_NOZOMI_COMPUTER_EXPERIMENT = 93,
    TOUR_STAGE_NOZOMI_ART_PORTRAIT = 94,
    TOUR_STAGE_NOZOMI_ART_SKETCH = 95,
    TOUR_STAGE_NOZOMI_ART_OIL = 96,
    TOUR_STAGE_NOZOMI_BRASS = 97,
    TOUR_STAGE_MIRA_ROUTE = 98,
    TOUR_STAGE_MIRA_LITERATURE_POEMS = 99,
    TOUR_STAGE_MIRA_LITERATURE_ANTHOLOGY = 100,
    TOUR_STAGE_MIRA_LITERATURE_SPEECH = 101,
    TOUR_STAGE_MIRA_DRAMA = 102,
    TOUR_STAGE_MIRA_SCIENCE = 103,
    TOUR_STAGE_MIRA_COMPUTER_FORTUNE = 104,
    TOUR_STAGE_MIRA_COMPUTER_GAME = 105,
    TOUR_STAGE_MIRA_COMPUTER_EXPERIMENT = 106,
    TOUR_STAGE_MIRA_ART_PORTRAIT = 107,
    TOUR_STAGE_MIRA_ART_SKETCH = 108,
    TOUR_STAGE_MIRA_ART_OIL = 109,
    TOUR_STAGE_MIRA_BRASS = 110,
    TOUR_STAGE_YUKO_ROUTE = 111,
    TOUR_STAGE_YUKO_LITERATURE_COLLECTION = 112,
    TOUR_STAGE_YUKO_LITERATURE_SPEECH = 113,
    TOUR_STAGE_YUKO_DRAMA = 114,
    TOUR_STAGE_YUKO_SCIENCE = 115,
    TOUR_STAGE_YUKO_COMPUTER_FORTUNE = 116,
    TOUR_STAGE_YUKO_COMPUTER_GAME = 117,
    TOUR_STAGE_YUKO_ART_PORTRAIT = 118,
    TOUR_STAGE_YUKO_ART_SKETCH = 119,
    TOUR_STAGE_YUKO_ART_OIL = 120,
    TOUR_STAGE_YUKO_BRASS = 121,
    TOUR_STAGE_MEGUMI_ROUTE = 122,
    TOUR_STAGE_MEGUMI_LITERATURE_ANTHOLOGY = 123,
    TOUR_STAGE_MEGUMI_LITERATURE_SPEECH = 124,
    TOUR_STAGE_MEGUMI_DRAMA = 125,
    TOUR_STAGE_MEGUMI_SCIENCE = 126,
    TOUR_STAGE_MEGUMI_COMPUTER_GAME = 127,
    TOUR_STAGE_MEGUMI_ART_SKETCH = 128,
    TOUR_STAGE_MEGUMI_ART_OIL = 129,
    TOUR_STAGE_MEGUMI_BRASS = 130,
    TOUR_STAGE_YUMI_ROUTE = 131,
    TOUR_STAGE_YUMI_LITERATURE_ANTHOLOGY = 132,
    TOUR_STAGE_YUMI_LITERATURE_SPEECH = 133,
    TOUR_STAGE_YUMI_DRAMA = 134,
    TOUR_STAGE_YUMI_SCIENCE = 135,
    TOUR_STAGE_YUMI_COMPUTER_GAME = 136,
    TOUR_STAGE_YUMI_ART_SKETCH = 137,
    TOUR_STAGE_YUMI_ART_OIL = 138,
    TOUR_STAGE_YUMI_BRASS = 139,
    TOUR_STAGE_RESUME = 140,
    TOUR_STAGE_TT_NARRATOR = 141
};

#endif
