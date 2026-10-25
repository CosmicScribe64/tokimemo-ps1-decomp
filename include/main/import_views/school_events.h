#ifndef MAIN_SCHOOL_EVENTS_H
#define MAIN_SCHOOL_EVENTS_H

#include "common.h"

extern u8 trip_destination;
extern s32 trip_talk_text_table;
extern s32 trip_talk_command_table;
extern s32 trip_talk_voice_table;
extern s16 trip_talk_group;
extern s16 trip_talk_line;
extern s16 trip_secondary_talk_group;
extern u8 trip_alternate_dialogue_mode;
extern u8 trip_response_branch;
#ifndef MAIN_API_OVERRIDE_D_80120678
extern s16 D_80120678;
#endif
#ifndef MAIN_API_OVERRIDE_D_801206BC
extern s16 D_801206BC;
#endif
extern u8 D_80120767;
extern s16 D_801208CC;
extern s16 D_80122790;
extern s32 D_80122794;
extern s8 D_80122798;
extern s8 D_80122799;
extern s8 D_8012279A;
extern s32 sports_folk_dance_cycle;
extern s32 sports_race_variant;
extern s32 D_80122EC4;
#ifndef MAIN_API_OVERRIDE_sports_entry_phase
extern s32 sports_entry_phase;
#endif
extern s32 sports_scene_variant;

#endif
