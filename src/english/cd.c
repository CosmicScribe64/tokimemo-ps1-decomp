/* cd.c: English CD file remap (T-39507). Not part of the original executable: only the English build
 * (`ninja english`) compiles it, into the island after the main bss. The English disc moves the text
 * files that grew to the end of the data track (tools/patch/disc.py), and the game reads files by
 * literal sector numbers and counts: 920 cd_read_sectors calls in src/, LBA tables in the data of main
 * and the overlays, and the 0x1012 end-word check of cd_read_sectors, which needs the read to end at the
 * file's last sector. Rather than change every read site, the two read entry points are redirected
 * here (config/english_hooks.txt); they look the request up in english_cd_remap, a table generated
 * from the disc layout, then do what the originals do. Design: wiki/english-build.md. */
#include "common.h"
#include "main/audio.h"
#include "main/cd.h"
#include "english/cd.h"

#ifdef TOKIMEMO_ENGLISH

/* Rewrites a read request for a file that the English disc moved or resized. */
static void english_cd_map(u8 *sectors, s32 *lba) {
    EnglishCdRemap *entry;
    s32 i;

    for (i = 0; i < english_cd_remap_count; i++) {
        entry = &english_cd_remap[i];
        if (*lba >= entry->lba && *lba < entry->lba + entry->sectors) {
            if (*lba == entry->lba && *sectors >= entry->sectors) {
                *sectors += entry->new_sectors - entry->sectors;
            }
            *lba += entry->new_lba - entry->lba;
            return;
        }
    }
}

/* cd_read_sectors_nowait with the remap: queue a read without the end-word check. */
void english_cd_read_sectors_nowait(u8 sectors, s32 address, s32 lba) {
    u8 params[0x20];

    english_cd_map(&sectors, &lba);
    audio_command(6);
    cd_read_param_pack(sectors, address, (s32 *)params);
    cd_audio_command(6, lba, params);
    cd_read_checked = 0;
}

/* cd_read_sectors with the remap: queue a read whose last word must be a file trailer. */
void english_cd_read_sectors(u8 sectors, s32 address, s32 lba) {
    u8 params[0x20];

    english_cd_map(&sectors, &lba);
    audio_command(6);
    cd_read_param_pack(sectors, address, (s32 *)params);
    cd_audio_command(6, lba, params);
    cd_read_checked = 1;
}

#endif
