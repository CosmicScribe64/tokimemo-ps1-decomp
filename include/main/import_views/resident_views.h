#ifndef MAIN_RESIDENT_VIEWS_H
#define MAIN_RESIDENT_VIEWS_H

#include "common.h"
#include "main/talk.h"

extern void *D_800B374C; /* image data passed to LoadSquare (hud_vram_init) */
extern s32 D_800B39C4[];
extern u8 debug_hud_level;
extern u8 memory_card_icon_palettes[];
extern u8 *memory_card_icon_images;
extern u8 D_800B6708;
/* T-17000: word tables walked by boot_hud_init (and hud_icon_bar_init) up to the next symbol: eight words
 * ending at D_800C6220, 32 words ending at D_800C6340. */
extern s32 D_800C6200[8];
extern u16 sd_table_weekday[];
extern u16 sd_table_holiday[];
extern u16 sd_table_club[];
extern u16 sd_table_else[];
extern TalkSelectors talk_voice_speaker_by_girl;
extern s32 timer_event;
extern s16 D_8011ECE8;
extern u8 D_8011F0CD;
extern u8 D_8011F0CE;
extern u8 D_8011F0CF;
extern u8 D_8011F0D0;
extern u8 D_8011F0D1;
extern u8 *D_8011F0D8;
extern u8 *D_8011F0DC;
extern s16 D_8011F0E0;
extern s16 D_8011F0E2;
extern s16 D_8011F0E4;
extern s16 D_8011F0F2;
extern s16 D_8011F0F6;
extern u8 *D_8011F100;
extern s32 D_8011F104;
extern u8 D_8011F10F;
extern u8 D_8011F50E;
extern u8 D_80123122;
extern u8 D_80123123;
extern u8 D_80123124[];
extern u8 D_80123164[];
extern u8 D_80123180[];
extern u8 D_801231A0[];
extern s32 boot_global_pointer;
extern s32 boot_previous_stack_pointer;
extern s16 D_80125C50;
extern s16 D_80125C52;
extern u8 D_80125C54;
extern u8 D_80125C55;

#endif
