---
type: reference
updated: 2026-10-13
sources: ["raw/ (none: facts re-derived from a third-party notes file, not stored)", "config/symbol_addrs.txt", "config/symbols_DATE.txt", "config/symbols_NAME_ENT.txt", "config/symbols_SHOUGATU.txt"]
---

# Reference: English-patch internals notes

Credit: the facts below come from `docs/game-internals.md` in https://github.com/gothamor/tokimeki-memorial-english-patch (a fan English patch for the same release, SLPM-86053). That repo has no license, so this page keeps **facts only** (addresses, what a routine or table does, layouts), all in our own words; none of its prose, code or translated text is copied. Ticket: [[tickets/T-11090-ingest-english-patch-notes]].

How to read: addresses are RAM addresses (main exe `0x80041000..`, overlays `0x80132000..`, file offset = RAM - `0x80040800` in the main exe, RAM - `0x80132000` in an overlay). "Symbol" is the name in this repo now; a dagger marks a placeholder that T-11090 renamed (the old `func_XXXXXXXX` / `D_XXXXXXXX` is the address). Names from [[obin]] are hypotheses; where the notes agree with one it says "agrees". Facts were observed by the patch authors through emulator traces and static disassembly; "unverified" items are their own caveats.

## Overlay loader (main exe)

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x80078DC0-0x80078F84 | main | inside `func_80078C48` | per-mode switch that picks a hard-coded overlay LBA (table below) |
| 0x80078FE0 | main | `ovl_load_poll` † | when the CD sync routine reports done, sets loader state 2 |
| 0x80079014 | main | `ovl_load_verify` † | checks the loaded overlay with the file checksum; ok -> state 3, else state 0 and LBA 0 (reload forever) |
| 0x80042458 | main | `func_80042458` | called once after a successful verify (role not stated) |
| 0x800B6D34 | main | `ovl_load_state` † | u8: 0 idle, 1 reading, 2 read done, 3 loaded and verified |
| 0x800B6D38 | main | `ovl_load_sectors` † | u8 sector count, always 0x60 |
| 0x800B6D3C | main | `ovl_load_buf` † | pointer to the overlay load buffer (value 0x80132000) |
| 0x800B6D40 | main | `ovl_load_lba` † | LBA of the overlay currently loaded; equal to the requested one means "already loaded" |
| 0x800B19E8 | main | `jtbl_800B19E8` | jump table for the overlay switch, 0x81 entries, indexed by (`*(u8*)0x800E738E` - 0x11) |
| 0x800E738E | main | `D_800E738E` (GameState) | byte that indexes the overlay switch (value - 0x11) |

Overlay facts: each `EXEDIR/*.EXN` is 0x30000 bytes (96 sectors), uncompressed code+data, loaded at 0x80132000. `EVENT.EXN`, `GYOZI.EXN` and `O.BIN` are never loaded (no code builds their LBA; EVENT looks like an old copy). Hard-coded LBAs: BUNKAKEN 37130, BUNKASAI 37226, BUNKA_SD 37322, DATE 37418, DATE2 37514, ENDING 37610, EN_NICHI 37706, ETC 37802, GEKO 37994, KANGEI 38186, MASTER 38282, NAME_ENT 38378, OLH 38570, OMIMAI 38666, OPTION 38762, RENSYU 38858, RPG_BAT 38954, SHOUGATU 39050, SHUGAKU 39146, TACO 39242, TAIIKU 39338, TEL 39434, TT 39530, VALEN 39626 (instruction addresses of the loads are 0x14 apart from 0x80078DC0). Main state to overlay: BUNKASAI 0x70, BUNKAKEN 0x71, BUNKA_SD 0x80, ENDING 0x63, MASTER 0x74-0x76, OMIMAI 0x61/0x62, DATE2 0x58/0x5C, RENSYU 0x73, TAIIKU 0x72, RPG_BAT 0x81, ETC 0x35-0x37/0x91, EN_NICHI 0x82, DATE 0x50/0x5E, GEKO 0x56/0x57/0x59/0x5A/0x5D, KANGEI 0x5F/0x60, SHOUGATU 0x52/0x53/0x55/0x5B, SHUGAKU 0x51, TEL 0x43/0x45, VALEN 0x54, NAME_ENT 0x11/0x13-0x15, OPTION 0x20-0x22.

## CD read and checksum (main exe)

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x80046318 | main | `cd_read_sectors` † | read(sector count, RAM buffer, LBA); used for every file load |
| 0x800460CC | main | `cd_read_sync_status` † | returns the read status byte; bit 0 = finished |
| 0x8012512A | main | `cd_read_status` † | the status byte itself |
| 0x8004636C | main | `file_checksum_ok` † | (sector count, buffer): sums all bytes except the last 4 into 16 bits, compares with the big-endian u16 stored at end-4 |

File trailer layout: last 4 bytes = sum16 high, sum16 low, 0x12, 0x10. Present on all overlays, SERIFU, BUNKA (45/46), BUSTUP, most EVENT and others; absent in BS stills and the VAB data. Only the overlay loader's use is confirmed.

## Boot and disc

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x80041000 | main | - | load address; t_size 0xA2800 |
| 0x800420D0 | main | `func_800420D0` | entry point |
| 0x8004435C | main | `LoadSquare` | VRAM upload (x, y, w, h, source); x and w in halfwords; used for every texture/CLUT/glyph upload |

Boot reads, in order: `FUKUTEST/MAINROM.NBN` (53 sectors to 0x80180000), `TMD/DUM.TMD` (1 to 0x801F0000), `FUKUTEST/TITLE.NBN` (214 to 0x80180000), `EXEDIR/NAME_ENT.EXN` (96 to 0x80132000), then `FUKUTEST/O_31_*.NBN`. Disc: boot file `SLPM_86.053` at LBA 24, `SYSTEM.CNF` 23, `KONAMIC3.STR` 400, `OPENXA.STR` 2080; SYSTEM.CNF says TCB 8, EVENT 32, STACK 801FFF00. LBA 14515 (start of `/CDROM` directory) has a damaged subheader and its records start at byte 0x10 (the game uses hard-coded LBAs, only an ISO walker trips on it). See also [[disc-layout]].

## Text renderer (main exe)

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x8004E788 | main | `set_kanji_string` | add_text(x, y, attr, string, extra): appends an 8-byte entry (x s16, y s16, attr, glyph count, extra, 1) to the text list; agrees |
| 0x800B3DC0 | main | `text_list` † | the text list, max 52 (0x34) entries |
| 0x800B3F60 | main | `text_list_count` † | number of entries |
| 0x8004F268 | main | `load_string` | layout: reads 2 bytes per character always, stops at NUL in either byte, codes below 0x8140 go to an error routine, others via the range table to a glyph index; agrees |
| 0x8004F364 | main | `vload_font` | decodes one glyph to 16x12 4bpp and uploads it to the VRAM glyph cache (x = 0x320 + (slot&7)*4, y = 0x100 + (slot&0x78)*2); agrees |
| 0x8004F4F0 | main | `glyph_expand_1bpp` † | expands one 18-byte 1bpp glyph to nibbles (upload buffer 0x801255F0) |
| 0x800B3F62 | main | `text_glyph_count` † | next glyph-cache slot / total glyphs |
| 0x800B3F6A | main | `text_glyph_advance` † | u8 glyph advance in pixels, 14 or 16 |
| 0x8004E750 | main | `text_set_advance` † | sets the advance: 16 if the argument is 16, else 14 |
| 0x8004E708 | main | inside `func_8004E58C` | text-system reset; forces an advance other than 14/16 back to 14 |
| 0x801255F0 | main | `D_801255F0` | 4bpp glyph upload buffer |
| 0x8004EC24 | main | `k_disp_inc` | typewriter: reveals one glyph every (speed+1) frames, returns 1 when all shown; agrees |
| 0x8004ECF0 | main | `get_now_k` | returns the text-list entry (display line) being typed; agrees |
| 0x800B3F64..0x800B3F68 | main | `D_800B3F66` and its neighbours | u16 typewriter fields: first dialogue entry (3F64), speed (3F66), glyphs revealed (3F68) |

Glyph layout: cached glyphs are `index x advance` pixels apart (multiplies at 0x8004EF30 and 0x8004F030). Calls that set the advance: OPTION overlay 0x80132CA0 (14), NAME_ENT 0x801321B4 (16), 0x80139724 (14), 0x8013BCFC (14). add_text calls from OPTION appear at 0x80132CF0.

## Font (main exe)

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x800B6D50 | main | `font_glyphs` † | glyph bitmaps: 3418 glyphs x 18 bytes (to 0x855A4 file), 12x12 1bpp MSB first; file offset 0x76550 |
| 0x800B402C-0x800B40E3 | main | `font_range_table` † | 46 entries of (u16 first code, u16 first glyph index), sorted |

Code ranges: 0x8140-0x84BE are glyphs 0-452 (punctuation, digits, full-width Latin, kana, Greek, Cyrillic), 0x889F-0x9872 are glyphs 453-3417 (JIS level-1 kanji); level 2 is absent; plain ASCII draws nothing. The only code that reads the font or the range table is the glyph expander and the layout loop (0x8004F2CC-0x8004F30C, 0x8004F4FC). No BIOS font is used. Two routines the patch authors found unreferenced (static scan plus a 15-minute run): `func_8004443C` (0x8004443C, 708 bytes) and `make_color_bar` (0x8004B778, 948 bytes, name from obin and the notes' description do not obviously agree). Others they list as never called by a `jal` (EXE-only scan): 0x80049140 `sprite_set_gpu_poly_ft4`, 0x80054C74, 0x8008EFE4 `SsUtKeyOn`, 0x8008D8A0, 0x8005A410, 0x8008F934 `SsUtChangePitch`, 0x80058DA0 `initLight` (two of these are PsyQ names in our tree, so the scan was not complete: overlays may call them).

## Script and dialogue

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x80081190 | main | `dialogue_render` † | dialogue renderer (x, y, attr, string...): copies to a 512-byte buffer, splits at control bytes 0x20/`q`, 64-byte line buffers, expands name codes, draws lines 16 px apart with add_text; speaker label and opening bracket are drawn by the game |
| 0x800812D4 | main | inside `dialogue_render` | first pass: clears a flag at 0x80122D1C unless every char is one of a few punctuation marks (purpose unknown) |
| 0x8007EF48 | main | `dialogue_speak_part` † | speaks one script string (string, code, option): copies to a stack buffer, switches on the code via the jump table at 0x800B22A8 (20 cases); reply loop 0x80080F48-0x80081020 cuts at the option-th `q` and skips an option starting with `n` |
| 0x800B22A8 | main | `jtbl_800B22A8` | the 20-case speak-code switch |
| 0x80082764 | main | `func_80082764` | "talk runner" that all four script sections go through |
| 0x80051438 | main | `get_g_name` | builds the speaker/girl name into the {G} buffer (girl 0 fixed name, 10 special, 12 special, others from the short-name table plus an honorific); agrees |
| 0x800CA190 | main | `girl_name_buf` † | {G} buffer, at most 10 cells |
| 0x800B35F4 | main | `girl_short_name_table` † | 16 pointers to the short speaker names (strings at file 0x72D78-0x72DF3, pointer table file 0x72DF4) |
| 0x800AFB7C / 0x800AFB84 / 0x800AFB8C | main | (data strings) | girl-0 fixed name, "chan" suffix, "san" suffix |
| 0x800B2240 / 0x800B2250 | main | `D_800B2240`, `D_800B2250` | fixed expansions of the {Y} code (Yoshio) |
| 0x800CA16C / 0x800CA174 | main | `D_800CA16C`, `D_800CA174` | {p} and {m} buffers (family / given name, unverified) |
| 0x800CA23C / 0x800CA248 / 0x800CA250 / 0x800CA25C / 0x800CA280 | main | `D_800CA23C` etc. | buffers for {S}, {O}, {W}, {A}, {M} (place, item and similar; {A} and {M} also carry club and technique names) |
| 0x80050DFC / 0x80050E8C / 0x80051010 | main | `gnsx`, `sndisp`, `sndi` | speaker-label draw helpers (0x80051010 takes text, line, speaker; speaker 13 = the callee) |
| 0x80051A68 | main | `get_g_zyotai_s` | per-girl state; `& 0x7F` is the rank/mood used by scenes |
| 0x80051B48 | main | `get_g_zyotai_h` | rank 0-4 from two scores (4 if A < 10, 3 if A < 50, 2 if B < 50, 1 if B < 80, else 0) |
| 0x80052C88 | main | `get_h_yuukou` | score A = weight x affection / 100, clamped 0-100 |
| 0x80052648 | main | `get_h_tokimeki` | score B (not decoded) |
| 0x800B41A0 | main | `D_800B41A0` | per-girl weight bytes (girl 0 = 83) |
| 0x800E6442 + 0x38*g | main | in GameState | affection s16 of girl g |
| 0x80042878 | main | `main_state_set` † | sets the main state (byte 0x800E7388) |
| 0x800E7388 / 0x800E7389 / 0x800E738A | main | `D_800E7388` etc. (GameState) | main state / stage / step of the running scene step machine |
| 0x800E738D | main | `D_800E738D` (GameState) | index of the prologue step machine in NAME_ENT |
| 0x800E691C, 0x800E699C | main | `D_800E691C`, `D_800E699C` (GameState) | day-event queue of 4-byte records (type, girl, scene, flags) and its current index |
| 0x800714D0-0x800722B8 | main | inside `func_8007132C` | day handler switching on the record type |
| 0x800B1340 | main | `jtbl_800B1340` | record-type switch (type < 0x4B); type 0x45 -> 0x800721B0 (special event: copies girl/scene to 0x80122CD4 / 0x80122CD0), 0x48 -> 0x80072210 (meeting, state 0x5B) |
| 0x80122CD4 / 0x80122CD0 | main | `D_80122CD4`, `D_80122CD0` | girl id and scene id of the running special event |
| 0x80122CDC | main | `D_80122CDC` | date result/choice counter used by the date flows |
| 0x800E71DF / 0x800E69DD | main | `D_800E71DF`, `D_800E69DD` (GameState) | current girl id for the event overlays; girl byte of the running date |
| 0x8006FB4C | main | `special_event_select` † | picks a (girl, scene) for a special event: loops girls 0-10 requiring met flag, low rank, nonzero counters and a random roll, then jumps through 0x800B1314 by girl |
| 0x80070B80 | main | inside `schedule_girls_gakko`? | queue builder that asks the selector and writes a type-0x45 record |
| 0x800B1314 | main | `jtbl_800B1314` | per-girl case table of the selector |
| 0x8006AEC4 | main | `appointment_find_today` † | (year, month, day): checks the 8 appointment slots against today |
| 0x800E67CC + 4*i | main | in GameState | 8 appointment slots: bits 0-4 place (1-based), 12-15 month, 16-20 day, 24 flag, 25-28 girl, 29 active |
| 0x80076DA0 | main | `appointment_launch` † | copies slot 0 to a queue record [0x80, girl, place] and enters state 0x50 (DATE) |
| 0x800754C0 / 0x80075628 | main | `func_800754C0`, mid-function | birthday-style date check; scene 8 of girl 0 is started directly when it returns 1 |
| 0x80073198 | main | `func_80073198` | injury-message step machine; can start a one-shot event of girl 0x0B |
| 0x800752DC | main | mid-function | day-advance step; calls `next_stage` when the camp byte is 0x42 |
| 0x80066ACC | main | `func_80066ACC` | date-window check used by one selector case |

Script string format (shared by SERIFU, BUNKA, SPEAKTST and overlay text): NUL-terminated; 0x20 ends a display line, `q` ends a choice option, `n` marks an empty option, letters `P p m F Y G S O W A M` are name/value codes; every other byte starts a 2-byte character. Each file holds absolute RAM pointers, so strings are movable if all pointers are updated. Script-file load addresses: SERIFU DATE_*/EVENT_* 0x80198000; SERIFU, G_APPEAR, G_INVITE, GASSHUKU, GEKO, GROUP_D, HAGEMASI, NYUBU, OMIMAI, SHOGATU, SHUGAKU, VALEN, WHITE, XMAS 0x80197000; G_BIRTH, KARADATA, P_BIRTH, ZAKOSEN 0x801B0000; GASSH_FU 0x801B4400; EPILOG and SPEAKTST 0x80180000; ENDING 0x801F0000; BUNKA and MASTER 0x801E8000. Scenes: a master list of pointers to per-part string lists plus a parallel list of per-string code bytes (FF-terminated) and a third per-scene pointer. A text box shows 3 lines; scene code waits on line indices, so line counts must not change.

Overlay symbols (script side):

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x80144B44 | NAME_ENT | `prologue_line_start` † | starts a prologue display line (index in byte 0x8014D180) |
| 0x80144C8C | NAME_ENT | `prologue_line_run` † | runs the current line each frame |
| 0x80144E34- | NAME_ENT | inside `prologue_line_run` | voiced lines auto-advance after the XA clip ends and the text is typed (counter at 0x800E7384 counts 61 frames) |
| 0x80148380 | NAME_ENT | inside `func_8014826C` | step of the Rei entrance; waits for the 4th display line |
| 0x8013272C | DATE | `date_event_choose` † | date-event chooser, called at every date step |
| 0x801326B0 | DATE | `date_event_trigger` † | (scene, one-shot, flag index): stores scene at 0x80122CD0, sets the one-shot flag, enters state 0x5E |
| 0x8014F370 | DATE | `date_shiori_dispatch` † | girl-0 dispatcher mapping scenes 3-6 to step machines 0x8014FAE0/FBA0/FC80/FE90 |
| 0x80141004 | DATE | `date_fill_sections_shiori` † | writes the 81 section-table words of girl 0's script file |
| 0x80141518 | DATE | `date_fill_sections_mio` † | same for girl 1 |
| 0x801419FC | DATE | `date_fill_sections_yuina` † | same for girl 2 |
| 0x80141EE0 | DATE | `date_fill_sections_ayako` † | same for girl 3 |
| 0x801423C4 | DATE | `date_fill_sections_saki` † | same for girl 4 |
| 0x801428D8 | DATE | `date_fill_sections_yukari` † | same for girl 5 |
| 0x80142DEC | DATE | `date_fill_sections_nozomi` † | same for girl 6 |
| 0x80143300 | DATE | `date_fill_sections_mira` † | same for girl 7 |
| 0x80143814 | DATE | `date_fill_sections_yuko` † | same for girl 8 |
| 0x80143D28 | DATE | `date_fill_sections_megumi` † | same for girl 9 |
| 0x8014420C | DATE | `date_fill_sections_yumi` † | same for girl 10 (default case 0x8013E94C of the loader switch) |
| 0x8013EFA0 | DATE | `func_8013EFA0` | the "missed meeting" section, run in every date |
| 0x8015B9DC | DATE | `date_place_names` † | 21 place names, 33-byte records, index = place id (0 = neighbourhood park) |
| 0x8015D108 | DATE | `date_gift_names` † | 93 gift-item names, 33-byte records |
| 0x8015B654 | DATE | `date_arrival` † | u8 arrival state: 1 on time, 0 she is late, 2 player late |
| 0x8014C0C8 / 0x8015B3D0 | DATE | inside `func_8014C08C`, `jtbl_8015B3D0` | top-level switch on the girl id and its jump table |
| 0x8013D180 | SHOUGATU | `shougatu_stage_dispatch` † | stage dispatcher of the Christmas-party flow (stage in byte 0x800E7389) |
| 0x80138F7C | GEKO | mid-function | loader that switches on the girl id and reads that girl's EVENT_xx file to 0x80198000 (girl 0: LBA 44581, 6 sectors) |
| 0x80144910 | GEKO | `jtbl_80144910` | handler table indexed by girl id |
| 0x80145EC0 | GEKO | `D_80145EC0` | 237-word event table (79 masters, 79 code-byte lists, 79 third pointers), shared in layout by GEKO/DATE/GYOZI/SHOUGATU/SHUGAKU |
| 0x8013ADB0 | SHUGAKU | jump table | 12-stage dispatch table of the school-trip overlay |

## Menus and screens

| Address | Binary | Symbol | Meaning |
|---|---|---|---|
| 0x8005DC4C | main | `system_settings_init` † | initialises the in-game system settings screen (strings at file 0x6FA08-0x70290) |
| 0x8006612C | main | `func_8006612C` | takes the settings screen title |
| 0x800E7312 | main | `D_800E7312` (GameState) | menu mode 0-3 (mouse-like, A, B, mouse-like + A) |
| 0x800E7D2C / 0x800E67C4 | main | `D_800E7D2C`, `D_800E67C4` (GameState) | bytes tested (thresholds 9 and 2) to pick one of two cursor-speed label sets |
| 0x8006B648 | main | `club_list_menu` † | club list menu; draws the first 11 entries of the club table |
| 0x800B3908 | main | `club_name_table` † | pointer table of 12 club names (file 0x73108); strings at file 0x730A4-0x73107; Cancel is a separate literal at file 0x70860 |
| 0x800607E0-0x800608B4 | main | mid-function | club-day notice: six separate add_text calls at fixed x,y |
| 0x80069758-0x80069F94 | main | mid-function | save/load slot panel: strcpy label + strncat value into stack buffers 32 bytes apart, y = -64..0 |
| 0x800E7D36 / 0x800E7397 | main | `D_800E7D36`, `D_800E7397` (GameState) | card-metadata index (first branch) and slot index into records |
| 0x80123120 | main | `D_80123120` | per-slot records, stride 0xF00 |
| 0x80064CA0- | main | in `hizuke_init` region | calendar: weekday sprite frame = 0x52 + weekday; colour 0x43 Saturday, 0x44 Sunday or holiday |
| 0x800E62BE-0x800E62C1 | main | `D_800E62BE` etc. (GameState) | year (95 = 1995), month, day, weekday (0 = Monday .. 6 = Sunday) |
| 0x800E62C4 | main | `D_800E62C4` (GameState) | holiday table, indexed by day |
| 0x8005E660-0x8005FD90, 0x8005F380 | main | mid-function | settings submenu draws: icon-mode confirmations at (x -128, y 50/66), menu-mode lines at y 50/66/82 |
| 0x80047804- | main | mid-function | sprite renderer: looks a frame number up in the frame table |
| 0x800C975C | main | `sprite_frame_table` † | 190 pointers to sprite frames (file offset = address - 0x80040800); frame = s16 part count, x, y, 0, then 14-byte parts (x, y, cell, attr, 0, w, h) |
| 0x80120650 | main | (start of a bss array) | sprite objects, 0x44 bytes each (+0x0C frame table, +0x18 frame number) |
| 0x801217A0 | main | `D_801217A0` | tile-layer descriptors, 20 bytes each (sports-day instruction card) |
| 0x8004D1C8-0x8004D2C0, 0x8004D3C8, 0x8004D6B8 | main | `srn_vram_set` area, `mod_trans_vram`, `get_fnt` | layer map walk, draw one block, copy a tile with flips then upload; the obin names for the last two do not match these roles (kept as is) |
| 0x80132CA0 / 0x80132CF0 | OPTION | inside `func_80132BA8` | options page: advance 14 call and add_text call |
| 0x801C32D8 | (OPTION.NBN in RAM) | - | OPTION.NBN frame table: 130 frames, file offset 0x132D8; glyph sheet starts at file 0xC000 |

OPTION.EXN text offsets: Menu Mode choices 0xB30C, descriptions 0xA8F0-0xAA6B, item strings 0xA880-0xA8EF, a debug menu at 0xAC10-0xAFFF. NAME_ENT.EXN text offsets: kana grid 0x19150 (conversion tables 0x192D8), album/memory-card messages 0x197B4-0x1A14B, yes/no 0x19A0C, name questions 0x1ACE4/0x1AD04/0x1AD10, birthday and blood type 0x194C0-0x1955C, default names 0x1956C/0x19574, cheat names 0x19590, prologue strings 0x1B1D0-0x1B957 with pointer table 0x1B958, per-line voice/speaker byte table 0x1BA04 and XA positions 0x1BA30. The title graphics (`TITLE.NBN`, `MAINROM.NBN`, `FILESEL.NBN`, `OPTION.NBN`, `NAME_ENT.NBN`) are raw 8bpp/4bpp pixel data with separate palettes, not TIM.

## Cross-checks against our symbol sources

- Agree with [[obin]] names: `set_kanji_string`, `load_string`, `vload_font`, `k_disp_inc`, `get_now_k`, `get_g_name`, `get_g_zyotai_s/h`, `get_h_yuukou/tokimeki`, the speaker-label helpers.
- Do not obviously agree: `make_color_bar` (0x8004B778), `get_fnt` (0x8004D6B8) and `mod_trans_vram` (0x8004D3C8) versus the roles the notes give; treat both sides as unverified.
- The notes call some routines "unreferenced" that are PsyQ names here (`SsUtKeyOn`, `SsUtChangePitch`, `initLight`): their scan only covered the main exe.
