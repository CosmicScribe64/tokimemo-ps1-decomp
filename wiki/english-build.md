---
type: tool
updated: 2026-11-01
sources: ["tools/patch/build_english.py", "tools/patch/art/", "tools/test_patch_art.py", "tools/patch/cd_layout.py", "tools/patch/text/english.py", "tools/patch/text/windows.py", "tools/patch/text/wrap.py", "tools/patch/text/layout.py", "tools/patch/text/inplace.py", "src/english/cd.c", "include/english/cd.h", "tools/test_patch_english.py", "tools/patch/movie/build.py"]
---

# English build pipeline

One command builds the English disc and its patch. Ticket [[tickets/T-39507-english-disc-build]], part of [[tickets/T-39500-english-translation-patch]]. It builds on the English build variant ([[english-text-runtime]]), the text-container writers ([[english-text-containers]]), the disc rebuild ([[tickets/T-39501-disc-rebuild-and-xdelta]], [[disc-layout]]) and the emulator route ([[emulator]]).

## Running

```
IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english   # about 4 minutes
tools/docker.sh python3 -m unittest tools.test_patch_english -v            # no game data
IMG=tokimemo-patch tools/docker.sh python3 -m unittest tools.test_patch_art -v  # art writers
IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_ENGLISH_DISC=1 python3 -m unittest tools.test_patch_english -v
tools/emu/run.sh route run --label en --build jp --cue build/en/disc/tokimemo-en.cue --relink build/en
```

Inputs:
- the original track in `disc/orig/` (`track1.bin`, `track2.bin`; a copy of `build/disc-image/orig/`);
- `disc/files`;
- `.asset-cache/text/units/units.jsonl` ([[english-text-containers]]);
- `translations/en/`.

Image: `tokimemo-patch` from `tools/Dockerfile.patch` (xdelta3). Outputs in `build/en/disc/`: `track1.bin`, `tokimemo-en.cue`, `track2.bin` (a hard link to the original), `tokimemo-en.xdelta` and `report.json` (ids, offsets, sizes and counts, no game text). All of it is game data and stays out of git.

## Steps

1. `configure.py`, then the font metrics (`build/en/font_metrics.json`).
2. Encode and wrap every record (`text/layout.py`).
3. C literals:
   - The English sources go to `build/en/src/<path>` (`c_literals.rewrite_sources`), and `configure.py` compiles every file found there in place of `src/<path>`, with the flag. The rule is `cctext`, which runs `tools/cc.py --source-text`; every rule that derives something from the path still sees `src/<path>`.
   - The English link relinks every unit freely ([[relink]], T-39505), so an English object may grow and move what follows it. `PINNED_LINK` is `False`; the old fit loop (round 1: a failing file's growing literals back to the original, round 2: the whole file, reported as `needs_relink`) only runs when it is set back to `True`.
   - A literal that initialises a sized array (`u8 name[27] = "..."`) gets a larger array (`c_literals._grow_array`); otherwise IDO stops with "too many initial values".
   - Literals in EVENT and GYOZI stay Japanese (`kept_original`, "overlay never loaded"): the game never loads those overlays, and the English link copies them unchanged.
   - Addresses other code takes inside a rebuilt object are carried over by `english_link.py` when the object's section keeps its size and symbol offsets (English literals change `.rodata`, and in `.data` only the bytes of fixed arrays and pointer addends), or through the object's own relocation at the same site (an anonymous literal); otherwise the link stops and names the address.
4. Data files:
   - Every talk script, supplemental script and `JOHO.NBN` is written with `Image.write` and checked by `cd_layout.plan_file`. It is then put on the disc with `Disc.replace`: in place when it keeps its sector count, otherwise at the end of the data track.
   - A file that may not grow keeps its original bytes.
4b. The English art (section Art below): every registered image into its container, placed like a data file; after step 5 the MAINROM part records go into the built executable, and after track 1 is written the art is read back and verified.
5. `build/en/cd_layout.json` (the moved files), then `configure.py` and `ninja english`. The table `english_cd_remap` (part of the main executable since the free relink) comes from that JSON (`cd_layout.py remap-asm`, `build/en/gen/english_cd_remap.s`). Without the JSON the table is empty, so `ninja english` alone still works.
6. In-place records go into the built executable and overlays (`text/inplace.py`):
   - Each slot is looked up where the free link put it (`english_link.translator`: the anchor of its input section, or a symbol or relocation inside a rebuilt object) and written only where the English binary holds the original bytes; otherwise it counts as `moved_in_build`.
   - EVENT and GYOZI keep their original files.
   - The overlay checksum trailer is recomputed.
   - Then the changed overlays go on the disc (in place), then the executable, which moves to the end because it gained sectors.
   - Then the opening movie: `OPENXA.STR` with the English cast pills, same sectors and layout ([[movie-patch]], T-39509; `--no-movie` keeps the original).
   - Last: track 1, the cue and the xdelta patch.

## Encoder (`text/english.py`)

- Text:
  - ASCII is itself.
  - The extras are 0xA0 “, 0xA1 ”, 0xA2 ‘, 0xA3 ’, 0xA4 em dash, 0xA5 ellipsis, 0xA6 heart, 0xA7 note, 0xA8 e-acute and 0xA9 star.
  - Typographic variants are normalised first (`NORMALISE`): en dash to `-`, open heart and other hearts to the heart, other stars to the star, full-width ASCII and accented letters to plain ASCII.
  - A character the game's font has is kept as 2-byte Shift-JIS and counted in `kept_shift_jis`. Anything else becomes `?` and is counted in `unmapped`.
- Controls, as in [[english-text-runtime]]:

  | Control | Byte |
  |---|---|
  | row end, and the 6 choice options stored as 0x20 | 0x0A |
  | `q` choice option, `q` reply variant | 0x0B |
  | each `n` of the `nn` and skip markers | 0x0C |
  | `P p m F Y G S O W A M` | 0x10-0x1A |
  | the fixed renderer's odd byte before the NUL | dropped |

  Everything else stays byte for byte: inline label markers, printf directives, storage bytes and opaque slots.
- Result on the current records: 0 unmapped and 0 kept-Shift-JIS characters. The returned translations are plain ASCII.

## Windows and wrap (`text/windows.py`, `text/wrap.py`)

There is one table of the windows that the code proves (x is relative to the screen centre):

| Window | x from | x to | width | rows shown | scrolls | row buffer | used for |
|---|---|---|---|---|---|---|---|
| `message_speech` | -0x48 | 0x7C | 196 | 3 | yes | 64 | commands 0-2, 4-6, 0xA-0xF, 0x11, 0x12; command 7 with a label; supplemental scripts |
| `message_narration` | -0x80 | 0x7C | 252 | 3 | yes | 64 | commands 9, 0x10; command 7 without a label |
| `choice` | -0x72 | 0x6E | 224 | 3 | no, no wrap | 64 | commands 3, 8, 0x13 |
| `telephone_narration` | -0x48 | 0x7C | 196 | 3 (at most 6) | yes | 40 | strings with `row_separator` (`sndisp`) |

Sources:
- `message_window_show` draws the frame from -0x85 to 0x85.
- `talk_speak_part` sets the x of each command.
- `check_k_scroll` scrolls rows at y 0x60 or below.
- `talk_render` uses `line[64]` per row.
- `sndisp`/`sndi`: rows from the third on go to y 0x52, the box scrolls through `text_scroll_state`, and `line_break_offsets[5]` allows at most 6 rows.
- The right edge 0x7C is where the widest original rows end in every message-window context.

Rules:
- A row that fits is kept, so explicit breaks stay. A row that is too wide breaks at the last space that fits.
- A word wider than the window is reported. Choices never wrap, because each option is one selectable row.
- Name codes count at their English width: 8 letters at 6 px (48), F 100, Y measured from "Yoshio". An inline speaker label (marker, then text up to `": "`) does not count.
- Pagination: the message window and the telephone box scroll, as in the original, which has single records of up to 9 rows. A non-scrolling window, or more rows than `max_rows`, is reported as `too_many_lines`. A page break would need a new script record or a new runtime control, so none is inserted.
- Records outside these contexts (menus, tables, C strings) have no proven window yet. They are measured (`width_px` in the notes), not wrapped.

## CD remap (`src/english/cd.c`, `tools/patch/cd_layout.py`)

The game names files by literal sectors:
- 920 `cd_read_sectors` calls;
- LBA tables in data, among them the BUNKA tour tables `tour_invite_script_sectors`/`_lba`;
- the 0x1012 end-word check, which needs a read to end on the file's trailer.

So instead of editing read sites, the English build hooks `cd_read_sectors` and `cd_read_sectors_nowait` (`config/english_hooks.txt`). Their island replacements look the request up in `english_cd_remap` (`{lba, new_lba, sectors, new_sectors}`, 12 bytes each, include/english/cd.h):
- A read that starts inside a moved file goes to the new place.
- A read of at least the whole original file grows by the sectors the file gained.
- Then the replacement does what the original does.

Checks before a file moves (`plan_file`):
- The literal read sites (casts and `U` allowed) and the tour table entries, which are read from the original BUNKAKEN bytes: a read that starts inside the file or also covers the next file is a problem.
- The load buffer: the new count must not exceed the largest original read into the same address. The DATE buffer at 0x80198000 is fixed at 24 sectors (`BUFFER_LIMITS`): TAKU3.BIN, the choice tables of every date place, is loaded right after it at 0x801A4000, and another load fills 0x801A5800-0x801AE000 during a date ([[ram-map]]). A file that does not fit keeps its original bytes.
- A literal `cd_file_checksum_ok` with the file's original count is a problem.
- At most 255 sectors (u8).

## Linker

The pinned link of T-39502 is gone: `tools/english_link.py` relinks every unit freely ([[relink]]). The section-label pins that T-39507 added for a shrinking last object are not needed any more, because nothing is pinned.

## Results with the free relink (T-39505, 2026-11-01)

- Command run: 66 s (C literals 40 s in one round, `ninja english` 37 s with the literals compiled, xdelta 13 s).
- Records:

  | Status | Records | Change from the pinned link |
  |---|---|---|
  | placed | 30,951 | +411 |
  | file_kept_original | 4,718 | the 10 DATE talk files (24-sector buffer, [[ram-map]]) |
  | kept_original | 625 | 340 empty targets, 237 C literals and 48 in-place records of EVENT and GYOZI (never loaded) |
  | needs_relink | 266 | 0 C literals (was 734); 266 in-place fixed-size slots over capacity (was 284) |
  | moved_in_build | 257 | extents whose bytes now come from a rewritten C literal |
  | overflow | 8 | JOHO.NBN fixed-stride labels |

  All 1,964 C literals of the loaded units are English, in 171 files. The remaining `needs_relink` records are fixed-stride table entries and fields (TEL 78; dictionary slots 39 in NAME_ENT, among them the hiragana grid, and 38 in the main executable; NAME_ENT 24; overlay tables 20 such as the KANGEI club names; 10 supplemental; the rest in ones and twos): a relink cannot widen them, the code that indexes the table has to change (stride, or pointers to relocated strings), see [[tickets/T-39506-english-script-runtime]].
- Sizes: the executable ends at 0x8012CA60 (21,920 bytes left below the overlay slot), 673,792 bytes, moved to LBA 298740. Overlay room after the English literals: TACO 291 bytes, DATE 4,956, BUNKASAI 5,279, RPG_BAT 13,164, TT 31,962, the rest 47 KB or more.
- Disc: 719 sectors added; xdelta 1,391,556 bytes.

## Results with the pinned link (T-39507, 2026-11-01)

- Command run: 222 s. Steps: C literals 122 s (3 rounds), `ninja english` 41 s, track 1 24 s, xdelta 23 s, loading records 6 s.
- Records (36,825 owners and source-only records, SPEAKTST excluded):

  | Status | Records | Meaning |
  |---|---|---|
  | placed | 30,540 | written in English |
  | file_kept_original | 4,718 | the 10 `SERIFU/DATE_*.BIN` files need 26-31 sectors against the 24-sector DATE buffer |
  | needs_relink | 1,018 | 734 C literals in 25 files (pinned link) and 284 in-place table or extent records over capacity: 49 main, 235 overlays |
  | kept_original | 340 | empty target |
  | moved_in_build | 201 | extents whose bytes in the English binary are no longer the original (their C literal was rewritten) |
  | overflow | 8 | JOHO.NBN fixed-stride club and zodiac labels |

  No record is unwrappable. Only one record needed a break added; the translators' rows already fit.
- Files:
  - 95 data files written, 22 of them moved to the end of track 1 (+41 sectors, +83,968 bytes);
  - 22 overlays replaced in place;
  - the executable is 673,792 bytes (329 sectors) and moved to LBA 298740;
  - 719 sectors added in all;
  - no read-site problem and no warning for any moved file.
- Disc: track 1 SHA-1 c140f667cc7c43855cc43eeef76d7caadc95e7df, the same in two runs (deterministic). xdelta 752,337 bytes.
- Default build 27/27 OK after the change to `tools/cc.py` and `configure.py`.

## Art (`tools/patch/art/`, T-39508)

Step 4b of the build writes every registered English image into its container ([[tickets/T-39508-english-art-on-disc]]). Code: `src/english/hud_frames.c` (room for grown HUD frames) in the English build. `--no-art` leaves the images Japanese. To plan the art alone (no disc): `IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.art.build --report build/en/art-report.json`. Tests: `IMG=tokimemo-patch tools/docker.sh python3 -m unittest tools.test_patch_art -v` (synthetic, plus game-data checks when `disc/files` is there). The image `tokimemo-patch` now has numpy and Pillow (`tools/Dockerfile.patch`).

What ships (`items.py`): for every artwork item of `translations/en/graphics/translation_inventory.json`, its `candidates` (the newest non-superseded candidate per variant), or for an english-reviewed item without candidates its approved historical asset under `artwork/batches`. Each PNG must have its registered SHA-256; its manifest entry says what it covers. Same-size fallbacks come from `T39410-menus-fixes-v1/fallback-ledger.json`.

Indexed sheets (`indexed.py`, `placer.py`, `sheets.py`, `sprites.py`):
- Each container is modelled as views (a frame in one palette variant, one part, a page region, a sports card), built from the asset readers' own `_parse` (`tools/assets/image_packs*.py`). Composing a view from the original bytes gives exactly the reader's export: 400+ frames match the `source_png_sha256` hashes of the inventory (a test).
- Only the palette variants the game draws are targets: a manifest `runtime_variant_scope` with `runtime_applicable: false` or empty `uses` is reported `not_drawn` (176 images, mostly the MAINROM hud/icon variants of frames that never use them).
- In place: an English pixel needs an index whose palette word draws its colour, in every variant of its view; the original index is kept where it fits, else the one with the original texel's STP bit. A colour no palette entry draws is an error. A texel that another view still draws (a frame without English art, a deferred or repacked frame) is never written.
- Repack, where in place cannot work (shared kanji tiles, a frame drawn in several places of one texel, a widened sprite, a colour outside the part's palette): relocate (a changed part whose texels nothing else draws is rewritten where it is; any other changed part gets fresh texels, cut to its opaque box, possibly with another palette of the container) or rebuild (new parts cut from the English image per palette and per word, at most the frame record's part count). The cheaper fits. Frames that draw the same texels with two palettes (OPTION normal and highlight) are one group and keep sharing texels. Free texels: not drawn by any kept part; where code draws pages without frames (MAINROM, FILESEL, NAME_ENT digits, EXAMINE score digits, sports, battle) unreferenced non-zero texels are kept too. A piece may overlap texels that already hold the indices it needs. When the pages are full the group whose dropping frees the most room is deferred and the allocation starts over.
- Records: OPTION, TITLE, FILESEL, NAME_ENT, EXAMINE in the file; sports tile words in the file; MAINROM in the executable (`build.patch_executable` finds the frame-record block by its original bytes, so a relinked executable works, and checks every record still holds its original bytes). A record that another group also draws through is never rewritten.
- Every view is composed again after writing, from the re-parsed container (and executable): targets equal their PNG, other views their original.

BS stills (`stills.py`): decode with the pinned MDEC model at the proven size, composite the crops (RGB rectangles, or RGBA crops where alpha 255 marks the English pixels; crops go to the file their manifest names, so P_01_03B is its own still), re-encode only the macroblocks with a changed pixel (`mdec_splice.encode_macroblock`, from the original levels; a pixel that still moves gets more weight, two rounds), keep every other macroblock's bits. Gate: outside the edited macroblocks bit-identical; unchanged pixels (outside the 2x2 chroma cells of English pixels, which share their Cb/Cr) within 1 RGB555 step and the float IDCT within 2 (the T-39012 gate). When no attempt reaches that, the best attempt within 4 and 4 is taken and `ring_tolerance` 4 is reported. Results are cached in `.asset-cache/art/stills/` (key: source, crops, encoder code); a clean plan takes about 12 minutes, a cached one seconds. All containers run in parallel processes.

Disc: art containers go through `cd_layout.plan_file` and `Disc.replace` like text data files (a grown still that may move is remapped; one that may not keeps its original bytes and its images are deferred). Then, after `write track 1`, `verify art` (`verify.py`) reads every written container and the executable back from the built track, decodes them again and checks each placed image (indexed: exact pixels, no avoidable STP change, other views unchanged, no byte changed outside the planned texels and records; stills: the gate above, plus max error and PSNR of the English pixels). A placed image that fails stops the build. `report.json` `art` lists all 272 items with `placed_verified`, `partly_placed`, `deferred` (reasons per image), `untranslated_by_decision` or `missing`, the images per mode, the containers with their facts, and `route` (the route checkpoints that show the container) with `not_on_route`.

Results (2026-11-01, round 2: on the free relink of [[tickets/T-39505-english-memory-and-relink]]; build 191 s with cached stills, about 28 minutes with an empty still cache):

| Status | Items |
|---|---|
| placed_verified | 235 |
| deferred | 9 |
| untranslated_by_decision | 27 |
| missing | 1 (D_11_11 ride sign) |

Images: 111 in place, 19 relocated, 113 rebuilt, 14 stills re-encoded, 191 palette variants the game never draws, 43 deferred (29 of them movie pills). 29 containers written (one still, D_19_02, grew a sector and moved), 37 MAINROM part records and 2 grown MAINROM frames (60, 88) in the executable.

Grown MAINROM frames (round 2): the uploaded HUD sheets have free 8x8 cells but no free 16x16 one. A changed MAINROM part may become 8x8 tiles (blank tiles left out; when one palette cannot draw a tile, stacked tiles each draw the pixels of their palette) under a new, longer frame record. The English build has room for them: `english_hud_frames` (`src/english/hud_frames.c`, 2 KB, starts with the magic `TKM-EN-HUDFRAMES`). After the link `build.patch_executable` finds the frame-record block by its bytes, the 174-word frame pointer table as the original pointers moved by the same distance, and the array by its magic; it writes the records there and points the frames' table entries at them. `verify art` reads them back from the executable on the disc.

- MAINROM: everything. The stat bar (frame 61), the Save labels (57-59), the date-panel captions YR / MO / DAY (frame 60, grown: its caption cells are tiles, partly in a palette of another frame), all weekdays (Sunday 88 grown), the magazine headlines, banners, tags and covers. The HUD palette overrides count only where the HUD uses them (hud-43/44 on frames 0x44-0x58, icon-51/52/53 on frame 2).
- OPTION: all 108 frames, 102 rebuilt per word, 6 in place. Kiyokawa (0103/0104) and Tatebayashi (0113/0114) ship the family-name fallbacks (no room for the widened labels).
- TITLE 4, FILESEL 13 frames and 3 pages, the EXAMINE suffix (fallback `pt`), the ALL_C banner, the battle labels, the sports cards, the NAME_ENT selector labels (32-texel fallbacks).
- Stills re-encoded (14): P_01_01, P_01_03, P_01_03B, C_01_28, D_01_47, D_01_48, D_01_70, D_15_03, D_19_02 (two signs), D_24_03, D_29_09, I_17_01, I_28_02. Unchanged pixels move at most 1 step on C_01_28, D_29_09 and P_01_03B, at most 3 on the others; English pixels PSNR 23-40 dB.
- Route (`--relink build/en`): the calendar strip reads 95 YR / 4 MO / 4 DAY with English weekdays on the command screen; the Save screen, the stat bar, title, options, file select, name-entry labels, gate plate and roster show English. Not on the route: 86 placed items (40 battle labels, 5 sports cards, the ALL_C banner, the exam suffix, 10 D_/C_/I_ stills, 29 MAINROM magazine frames 0121-0154; the OPTION cast list is on the extras page the route does not open); `report.json` `art.not_on_route` lists them. Extra route stops were not added: the route file belongs to the emulator ticket and the menus to these screens are not mapped yet.

Deferred (9 items) and what each needs:
- NAME_ENT panels 0000, 0001, 0002, 0014 (the 名前 / あだ名 / birthday headers): the one 4bpp page of NAME_ENT.NBN has no free aligned 8x8 cell left (only 2,368 texels are not drawn by a frame, scattered; grown records do not help without texels). Needs a second texture page, which is code in `src/ovl/name_entry/name_entry.c` (owned by [[tickets/T-39506-english-script-runtime]]): the actors take `game_state.ovl_tpage_table = 0x801AAF58` (NAME_ENT.NBN 0x1AF58, one entry, 0x80190200), and a part's texture id selects the entry by its high byte, so a second page needs a two-entry table elsewhere, the page uploaded with the first, and frame records that name page 1. The art step can then place the panels with the same writers (NAME_ENT.NBN is read as 54 sectors to 0x80190000 at `name_entry.c:203`; a grown file needs that read to grow, which the CD remap does).
- EXAMINE results header (exam-0000): fits as a grown frame (one 806-byte record), but its records are in EXAMINE.NBN, read as 13 sectors to 0x801E0000 (`final_exam.c:656`); placing it needs the file and that read to grow by a sector and the frame table entry (file 0x64A0 + 4 x frame) to point past the old end. Not done in this round.
- P_01_02 class signs and the D_03_08 placard: their English is 1-texel black-on-white strokes; at the stills' quantizer (6) the re-encoded blocks overflow the fixed-point IDCT (a pixel meant black decodes white, 31 steps off). Softening the strokes where it overflows removes the overflow but moves the surrounding pixels 7-12 steps, past the codec tier. A finer quantizer is global to a BS file, so it would re-encode every macroblock and change every pixel of the still; left for an owner decision.
- `name-label-abc` and the 56-texel selector labels: for T-39506, see below.
- OPENXA movie pills (29): need a runtime cue player.

For [[tickets/T-39506-english-script-runtime]] (name-entry selector labels and the ABC tab):
- NAME_ENT frames 15-24 are the selector cells in pairs (unlit, lit): 15/16 Hiragana, 17/18 Katakana, 19/20 Symbols, 21/22 Kanji, 23/24 Done. Each is one 32x16 part, origin (17, 8), on page 0 (4bpp, NAME_ENT.NBN 0x200); unlit and lit share texels and differ in the palette column (info 0x5030 / 0x5040). Texels: 15/16 at (32, 32), 17/18 (32, 48), 19/20 (64, 32), 21/22 (64, 48), 23/24 (32, 16). Frame records at NAME_ENT.NBN 0x1ADF8 + 0x18 x (n - 15), pointer table 0x1AF5C (29 words, base 0x80190000).
- Shipped now: the 32-texel fallbacks, written in place at those texels.
- The 56-texel art (`T39410-menus-fixes-v1/menus-fixes-v1/name-label-00NN.png`, and `name-label-abc.unlit` / `.lit`, same face and palette columns as 15/16) needs 56x16 texels per state pair (unlit and lit can share) on a page the actors can use: there is none free on page 0, so it comes with the second page above. The selector actors' positions in `name_entry.c` then move 24 texels apart. The ABC tab needs a new frame pair (e.g. frames 29/30, appended to the pointer table) or two existing frames reused; the art step writes the texels and records once the frame ids exist (`sheets.name_profile`, `sprites.view_for` map an image to its frame).

Other findings: the FILESEL System tab word is cut by the availability mark the code draws next to it (the Japanese draws 有 there; the English text step draws a letter at the same place).

## Opening movie (T-39509, 2026-11-01)

The step "opening movie" (70-80 s) rewrites 2,678 video sectors of `OPENXA.STR` in place (204 frames, 14,179 macroblocks); audio and every other sector stay identical, so the disc layout and the sector count do not change. The report has a `movie` section (frames, PSNR, per-cue opacity fits). The xdelta patch grows to about 6.0 MB. Details in [[movie-patch]].

## Open

- DATE files: the RAM map shows the buffer cannot grow in place (TAKU3.BIN and another load follow it during a date; [[ram-map]]). They need a smaller encoding, a split, or DATE code that loads them elsewhere.
- Relink records: the C literals now place (T-39505). The 266 fixed-size slots over capacity need code changes to their tables.
- `moved_in_build`: the extents under rewritten C literals. Check after the relink whether any consumer reads them other than through the literal.
- Windows for menus, tables and C strings: add rows to `WINDOWS` as screens are checked in the route.
- Art: the deferred items of the Art section (NAME_ENT panels and the second name-entry page, the exam header, two stills, the movie pills).
- Dialogue display needs the parser work of [[tickets/T-39506-english-script-runtime]]. Until then the control bytes 0x0A-0x1A are skipped by the renderer and the old parsers misread English.
