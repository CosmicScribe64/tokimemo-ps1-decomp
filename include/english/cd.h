#ifndef ENGLISH_CD_H
#define ENGLISH_CD_H

/* English CD file remap (T-39507, wiki/english-build.md). Only the English build compiles and links
 * these symbols, in the island after the main bss. */

#include "common.h"

/* One file that the English disc moved or resized: a read that starts inside the original
 * sectors is sent to the new place, and a read of at least the whole original file grows by the
 * sectors the file gained. */
typedef struct EnglishCdRemap {
    /* 0x00 */ s32 lba;          /* original first sector */
    /* 0x04 */ s32 new_lba;      /* first sector on the English disc */
    /* 0x08 */ u8 sectors;       /* original sector count */
    /* 0x09 */ u8 new_sectors;   /* sector count on the English disc */
    /* 0x0A */ u8 pad[2];
} EnglishCdRemap; /* size 0x0C */

/* Table written by tools/patch/cd_layout.py from the disc layout of tools/patch/build_english.py
 * (build/en/gen/english_cd_remap.s); empty until a disc build has run. */
extern EnglishCdRemap english_cd_remap[];
extern s32 english_cd_remap_count;

/* Replacements reached through the jumps in config/english_hooks.txt. */
void english_cd_read_sectors_nowait(u8 sectors, s32 address, s32 lba);
void english_cd_read_sectors(u8 sectors, s32 address, s32 lba);

#endif
