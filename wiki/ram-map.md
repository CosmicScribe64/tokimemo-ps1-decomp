---
type: reference
updated: 2026-11-01
sources: ["src/main/main_loop.c", "src/main/overlay_loader.c", "src/main/background_images.c", "src/main/memory_card.c", "src/main/frame_render_audio.c", "src/main/sound_driver.c", "src/main/title_screen.c", "src/main/scene_library.c", "src/main/boot_assets.c", "src/ovl/TT/player_and_asset_uploads.c", "src/ovl/shooter_3d/", "asm/data/bss.bss.s", "disc/files/SYSTEM.CNF"]
---

# RAM map

Who owns each part of the 2 MB of main RAM, for how long, and whether the English build may use it. Ticket: [[tickets/T-39505-english-memory-and-relink]]. Sources: the constants in the code (CD read destinations, buffer pointers, `SetSp`, `InitHeap`), the linker maps, and emulator memory dumps and write watches on the original disc ([[emulator]]).

## Summary: room for English

| Region | Size | Use for English |
|---|---|---|
| 0x8012B540-0x80132000 | 27,328 bytes | **Free.** Growth of the main executable and its bss (the free relink moves the bss up into it). |
| overlay slot after each overlay's last data | 291 bytes (TACO) to 187 KB (RENSYU); table below | **Free while that overlay is loaded.** An overlay may grow into its own zero tail. |
| 0x80176000-0x80180000 | 40 KB | Not proven free: no write in the scenes sampled, but TT and the TACO shooter load there and the still-image buffer below has no fixed end. Do not use. |
| everything else | | Owned by the game at some point; see the map. |

The overlay slot ends at a hard wall: 0x80162000 is the background still buffer, which the main executable fills in nearly every scene. An overlay therefore cannot grow past 0x30000 bytes, and the loader's sector count (0x60) never needs to change (see limits below).

## Map

| Range | Owner | Lifetime and evidence |
|---|---|---|
| 0x80000000-0x80010000 | BIOS kernel | always |
| 0x80010000-0x80041000 | sound driver: VAB headers and bodies, sequence packs | always. `sound_driver.c` (`audio_bank_header_address` 0x80010000, 0x80012000, 0x80016000, 0x8001A000); `frame_render_audio.c` CD reads of 3, 6 and 0x25 sectors to 0x80010000, 0x80012000, 0x80016000, 0x8001A000, 0x8001C000 and 0x8002E800 (the second pack ends at 0x80040F00) |
| 0x80041000-0x800AF340 | main `.text` (game, then PsyQ libraries) | always |
| 0x800AF340-0x800B3220 | main `.rodata` | always |
| 0x800B3220-0x800E3800 | main `.data` (includes the 12x12 font `text_font_glyphs`) | always |
| 0x800E3800-0x8012B540 | main bss: one splat object, `asm/data/bss.bss.s` | always. `boot_start` clears 0x800E60A0-0x8012B538; 0x800E3800-0x800E60A0 is not cleared. Large parts: `draw_work_buffers` 0x800E94A0 (0x901E), `D_800F24BE` (0xAFE2), `prim_arena_buffers` 0x800FD4A0, the two packet buffers `D_80100882` and `D_8010FE8F` (0xF60D and 0xEE11), `D_80126480` (0x2600). |
| 0x80100000-0x80120000 (inside the bss) | packet buffers; TT's work areas while TT runs | TT's `tt_sys` and 20 more pointers name 0x80110000-0x8011FD00. The tails 0x80108000-0x8010E000 and 0x8011A000-0x8011E000 stayed zero in every dump, but they are the unused ends of the packet buffers: not free. |
| 0x8012B540-0x80132000 | **nothing** | No code names an address here (only the unloaded EVENT overlay does, `D_8012B8C2`), every dump is zero, and write watches in 8 scenes saw no write (also T-39502). |
| 0x80132000-0x80162000 | overlay slot: one `.EXN` at a time, 0x60 sectors | from `ovl_load_start` to the next overlay load. The file is always read whole (0x30000 bytes), checksummed by `cd_file_checksum_ok`. |
| 0x80162000-0x80167000 | background still: BS bitstream (CD read of 0xA sectors in `background_images.c`), then the MDEC output strips | per screen. `bg_still_buffers_set(0x80167000, 0x80162000, 0x80162000)` |
| 0x80167000-~0x80176000 | still image run-length buffer (`DecDCTvlc` writes it, `MDEC_vlc_brk`/`func_80086E80` at 0x80086FEC) | per screen. Highest byte written in the samples: 0x8017418A. The size depends on the picture; nothing bounds it. |
| 0x80164000-0x80180000 | TT: palettes and tiles (`tt_vram_upload_*` from 0x80164000 to 0x8017E000) | while TT runs |
| 0x80170000-0x80182000 | TACO shooter: texture page loads (`cd_read_sectors_nowait` 8 sectors to 0x80170000, 0x20 sectors to 0x80172000) | while the shooter runs |
| 0x80172000-0x80199D60 | TT: one 0x27D60-byte read (`tt_cd_read_blocking`) | while TT runs |
| 0x80176000-0x80180000 | no write in the samples (above) | see the TT and TACO rows |
| 0x80180000-0x801A0000 | MAINROM.NBN (0x35 sectors at boot, `boot_assets.c`); face data (0x2D sectors, `hud_panels.c`, `scene_library.c`: `face_images` 0x80180084 ...); `movie_context` (movie player); TITLE.NBN on the title (0xD6 sectors to 0x801EB000) | per scene. Overlays also load scene data here (the DATE, GEKO and SHOUGATU families name 0x80194000-0x801F8000 thousands of times). |
| 0x801A0000-0x801EA800 | CALROM.NBN (0x95 sectors, calendar and home screens) | per screen |
| 0x801B8000-0x801E0000 | sound staging for SPU uploads (`cd_load_destination_base_set(0x801B8000)`, reads of 0x20 and 0x50 sectors to 0x801B8000 and 0x801C8000) | per scene |
| 0x801E0000-0x801F5000 | scene scripts: SD files (9 or 0x29 sectors, `school_day.c`, `holiday.c`), the base `D_80125CA4 = 0x801E0000` in `scene_library.c`; 0x35-sector reads in `frame_render_audio.c` | per scene |
| 0x801F0000-0x801F8000 | EFONT162.TIM (8 sectors at boot), title work copies (0x801F0000-0x801F6800), debug palette 0x801F0020 | boot and title |
| 0x801FD000-0x801FEFF0 | stack (`SetSp(0x801FEFF0)` in `boot_start`; SYSTEM.CNF says 0x801FFF00, the game moves it) | always. Used down to about 0x801FB000. |
| 0x801FF800-0x801FFFF0 | heap (`InitHeap(0x801FF800, 0x7F0)`) | always |
| 0x801FFFF0 | stack for `LoadExec("cdrom:PSX.EXE;1")` | at exit only |

EVENT (0x800F6000) and GYOZI (0x80134000) are never loaded: no code names their LBAs ([[overlays]]).

## Overlay tails

Where each default overlay's data ends (`used_end` in `tools/english_link.py`): its last nonzero byte before the 4-byte checksum trailer, or 16 bytes past the last label in the zero run after it (ETC, OLH, RENSYU and RPG_BAT keep zero-valued variables of up to 16 bytes there). Past that the file holds only zeros, inside the last data object. No symbol and no address points there (`tools/raw_addrs.py`, the maps), and write watches saw no CPU write there while the overlay ran (DATE, NAME_ENT, TEL, GEKO; the only hits were the CD DMA of the load itself). English may use the tail of the overlay it changes.

| Overlay | Data ends | Free tail |
|---|---|---|
| TACO | 0x80161ED9 | 291 |
| DATE | 0x80160C20 | 5,084 |
| BUNKASAI | 0x80160B8D | 5,231 |
| RPG_BAT | 0x8015F010 | 12,268 |
| TT | 0x8015A322 | 31,962 |
| BUNKAKEN | 0x8015651D | 47,839 |
| ETC | 0x80151888 | 67,444 |
| NAME_ENT | 0x8014DCD0 | 82,732 |
| TAIIKU | 0x8014A675 | 96,647 |
| TEL | 0x8014700D | 110,575 |
| SHOUGATU | 0x80146A30 | 112,076 |
| GEKO | 0x80147C81 | 107,387 |
| OPTION | 0x8013E884 | 145,272 |
| BUNKA_SD | 0x8013D9D0 | 149,036 |
| OLH | 0x8013D6F0 | 149,772 |
| ENDING | 0x8013CDBA | 152,130 |
| SHUGAKU | 0x8013CC84 | 152,440 |
| MASTER | 0x8013C839 | 153,539 |
| DATE2 | 0x8013B84E | 157,614 |
| KANGEI | 0x8013B218 | 159,204 |
| EN_NICHI | 0x80139E39 | 164,291 |
| VALEN | 0x8013472C | 186,576 |
| RENSYU | 0x801343F0 | 187,404 |
| OMIMAI | 0x80134D7A | 184,962 |

## Limits the build checks

`tools/english_link.py` ([[relink]]):
- main executable: `main_VRAM_END` (end of the bss) at most 0x80132000;
- overlay: data up to 0x80161FFC at most (the slot end minus the trailer); the image is cut or padded to exactly 0x30000 bytes, so the loader's sector count stays right;
- the PS-X EXE file: 376 sectors (the in-place disc slot up to KONAMIC3.STR; `tools/patch/variant_disc.py` moves it to the end of the track anyway).

Where the sizes live, should an overlay ever need more than 0x60 sectors: `ovl_load_start` stores 0x60 in `ovl_load_sectors` for every mode and passes the literal 0x60 to `cd_read_sectors`; `game_mode_tt_game_mode_handler` does the same for TT; `ovl_load_verify` and the TT handler pass `ovl_load_sectors` to `cd_file_checksum_ok`, whose `blocks` is a `u8` (at most 255 sectors) and which reads the checksum from the last 4 bytes of `blocks * 0x800`. The disc slot of each overlay is 0x60 sectors and the next file follows, so a larger overlay would also need its LBA (also a literal in `ovl_load_start`) moved. None of this is needed while the slot end is a wall.

## Method

- Code: literal census of every 0x801xxxxx constant in `src/` by 16 KB bucket and by file; CD read destinations and sector counts; the file at each literal LBA from `tools/patch/disc.py list`.
- Dumps: 2 MB RAM at the start and after 1,800 frames (CIRCLE every 30 frames) of 11 save states of the original disc (title menu, name entry, prologue, first week, command screen, holiday, week 2, phone date spot, holiday before a date, date, week end; states captured by the T-39504 route runs on the original disc), plus a power-on boot to the title. A page that is zero in all 23 dumps was never written with a nonzero value.
- Watches: CPU-write watches (`tools/emu/run.sh watch ADDR:LEN --no-stop`) over the overlay tails, 0x8016C000-0x80176000, 0x80176000-0x80180000 and 0x8012B540-0x80132000 in 8 of those scenes. Writes of width 2048 are the CD DMA of a load, not code.
- Not covered by the samples: TT, the TACO shooter, RPG_BAT, the culture festival and ending families. Their static references are in the table.
