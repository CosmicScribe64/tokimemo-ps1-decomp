#ifndef ENGLISH_NAME_ART_H
#define ENGLISH_NAME_ART_H

/* T-39506: tools/patch/art/name_entry.py extends NAME_ENT.NBN. The marker
 * stays inside the original load so a no-art build safely keeps its tables. */
#define ENGLISH_NAME_ART_PRESENT (*(u32 *)0x801AAFF8 == 0x4D4E454E)
#define ENGLISH_NAME_ART_TPAGES 0x801B9000
#define ENGLISH_NAME_ART_FRAMES 0x801B9020
#define ENGLISH_NAME_ART_PAGE_COUNT 8

#endif
