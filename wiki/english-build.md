---
type: tool
updated: 2026-11-01
sources: ["tools/patch/text/retarget.py", "tools/patch/text/reflow.py", "tools/patch/build_english.py", "tools/patch/art/", "tools/test_patch_art.py", "tools/patch/cd_layout.py", "tools/patch/text/english.py", "tools/patch/text/windows.py", "tools/patch/text/wrap.py", "tools/patch/text/layout.py", "tools/patch/text/inplace.py", "src/english/cd.c", "include/english/cd.h", "tools/test_patch_english.py", "tools/patch/movie/build.py"]
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
- Reflow (T-39506; windows `message_speech`, `message_narration`, `telephone_narration`): the translators kept the Japanese row breaks, which left short English rows ("(What'll I do / this / week?)"). Each break is judged on the Japanese row before it (`text/reflow.py`; the record's source segments, glyph cells against the window's original row capacity: 14 for speech and the telephone, 18 for narration):
  - forced (reflowed): the Japanese row had at least capacity - 2 cells, or ended short but not at a natural boundary;
  - kept: the Japanese row ended short after 。！？, a closing 」』）, or an ellipsis, or the next row opens with 「『（. It maps to the English break of the same index when both pages have as many breaks, else to the English sentence end (or opening bracket) of the same rank; if there is none, the break is dropped and counted as ambiguous;
  - categories that keep every break (`reflow.KEEP_ALL_BREAKS`): none of the windowed containers; song lines, letters and poems are C strings or table entries without a window and are not touched;
  - pages (records, option ends) never change; choices are not reflowed.
  - each speech page is measured with its closing quote (drawn by the runtime) and wrapped from its speaker's text column: x -0x80 + label + 6 + mark, for the widest speaker the record can have (inline labels measured, girls with "Tatebayashi", the player with seven wide letters; `text/speech.py`).
  Then each row too wide for the window is set balanced (`wrap._balanced_row`): the fewest rows that fit, the most even fill, no lone short word on the last row. `report.json` `layout` has the totals and `layout.json` each record (rows before and after, kept breaks with reason and Japanese fill %, orphans, pages longer than the window). A page longer than the window scrolls, as long records do in the original; splitting it would need a new page control in the runtime (not done).
- A row that fits is kept. A row that is too wide breaks at the last space that fits.
- A word wider than the window is reported. Choices never wrap, because each option is one selectable row.
- Name codes count at their English width: family and given names 7 letters at 6 px (42), F 88, P measured from a 7-letter name with "-kun", G from "Tatebayashi-san", Y from "Yoshio". An inline speaker label does not count: the encoder writes 0x1C (or 0x1D for a thought), the label, and 0x1F where the translation had `": "` (`english.end_inline_label`), and the runtime draws it left of the text.
- Strings drawn by `sndisp`/`sndi` that the handoff could not tokenise (the NAME_ENT prologue lines) get the telephone window from their proven consumer (`windows.consumer_calls`), and their name letters become codes (`english.untokenized_name_codes`: a standalone `P` or `G` in the English, as often as the original has the code).
- Row buffers are the English parsers' 160 bytes (`ENGLISH_ROW_BYTES`).
- Pagination: the message window and the telephone box scroll, as in the original, which has single records of up to 9 rows. A non-scrolling window, or more rows than `max_rows`, is reported as `too_many_lines`. A page break would need a new script record or a new runtime control, so none is inserted.
- Records outside these contexts (menus, tables, C strings) have no proven window yet. They are measured (`width_px` in the notes), not wrapped.

## Text longer than its slot (`text/retarget.py`, T-39506)

An in-place record whose English does not fit its slot is moved when the code reaches the slot only through relocations: the string goes to a free area of the same binary and every reference is re-pointed (`inplace.patch_built(..., retarget)`).
- References come from the English link: each input section of `build/en/<unit>.map`, its object's relocations (R_MIPS_32 words, HI16/LO16 pairs), target addresses through the map (section symbols) or the English ELF (other symbols).
- A slot moves only when a reference reaches its first byte and none reaches inside it, and when every `%lo` sharing a moved `%hi` targets the same slot (or the `%hi` does not change).
- Free areas: an overlay's zero tail (after its last nonzero byte and its last non-segment symbol, plus 16 bytes; [[ram-map]]), and in the main executable `english_string_pool` (2048 bytes in `src/english/script.c`, sized by `english_string_pool_bytes`). Before allocating, `patch_built` reserves the full capacity of every mapped in-place slot, including retained entries. Zero padding inside the final fixed-stride slot is owned storage, even when it extends beyond the last nonzero byte and symbol (T-39514).
- A typed table is moved entry by entry only when every translated entry has its own reference (a pointer table). A table that the code indexes by a fixed stride is left alone: moving its first entry would move the base the stride is added to.
- Fixed-stride tables changed under `TOKIMEMO_ENGLISH` to pointer views, so their entries have references and move: the New Year fortune results and labels and the wish names (SHOUGATU `omikuji_draw_scene.c`, `shrine_actor_sequences.c`), the summer camp club names (KANGEI `scene_pictures_and_effects.c`), the ending song lines (ENDING `graduation_epilogue_handlers.c`, centred by pixel width), the magazine profile lines (main `hud_screens.c`).
- A slot over capacity in an object the English build compiles again (its C literal is English already) counts as `moved_in_build`, not `needs_relink`.
- Kept original on purpose: the NAME_ENT kana pages and kanji reading labels (`hiragana_grid`, `katakana_grid`, `reading_labels`: the translators gave romaji, which would replace the kana a player picks); ETC `album_background_lba_cat_00`..`_15` (0x8014E0C4-0x8014EA1C), tables of CD sectors that the handoff read as half-width katakana (49 records; writing them loaded wrong album backgrounds); name presets longer than the name limit (`layout.PRESET_LIMITS`).

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

## Results after T-39506 round 2 (2026-11-01)

| Status | Records | Change |
|---|---|---|
| placed | 31,049 | +98 |
| needs_relink | 0 | was 266: 196 moved by `retarget` (TEL 76, main 43, NAME_ENT 24, SHOUGATU 17, ENDING 11, TAIIKU 8, KANGEI 4, RPG_BAT 4, BUNKAKEN 2, BUNKASAI 2, ETC 2, TT 2, MASTER 1); 27 are slots under English C literals (now `moved_in_build`); 30 kana-page entries and 9 long name presets kept on purpose |
| moved_in_build | 288 | +27 |
| kept_original | 762 | +137: 79 kana pages and reading labels, 49 ETC LBA-table bytes, 9 long presets |
| overflow | 8 | JOHO.NBN labels, open (see below) |
| file_kept_original | 4,718 | the DATE files |

Layout (source-judged reflow): 30,573 records, 34,029 line breaks: 21,130 reflowed, 12,907 kept (12,900 after a sentence end, 7 before an opening bracket), 0 ambiguous; rows 64,602 -> 48,999 (14,270 records got shorter); 576 pages still end in a lone short word; 79 pages are longer than their window and scroll.

The 340 `empty_target` records are all empty: 303 talk records whose source is empty or full-width spaces, and 37 table slots marked empty. None is a translation gap. Main executable 686,080 bytes.

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

2026-11-02 current art result: `build/still-fallback-proof` passes final-disc readback and xdelta validation, target SHA-1 `e00ffdf0b44b94ad97168b425c1ce564dad04b9f`, patch 6,533,072 bytes. All 245 art items are verified, with 28 approved exceptions, zero deferred and zero missing. The analytic unclipped IDCT starting basis fixes P_01_02 and D_03_08. If all analytic attempts fail, the previous nonlinear probe is tried as another search seed under the same gates; this preserves a fitting skating-rink D_19_02 result (16,370/16,384 bytes). Fourteen unchanged analytic successes were independently revalidated and reused; the skating-rink fallback was freshly computed. Cache provenance is in `build/still-cache-migration.json`. Assembly/readback took 161.7 seconds. This supersedes the historical deferral counts below; it does not complete off-route visual coverage.

Visual evidence: `build/emu/club-placard-english/shot_800.png` shows the translated tournament placard using a controlled, logged scene fixture; `build/emu/route-en-magazine-v2` opens the first magazine by normal input. The narrowed surname remains verified in `build/emu/route-en-banner-fit/046_prologue_18.png`, clear of the portrait's hair. Full route with font tracing is still running on the preceding analytic-basis build.

Current verification (2026-11-01, supersedes the historical deferral lists below): NAME_ENT panels, ABC, exam header, and movie pills are placed and read back. The birthday digit texture remains byte-identical; the full fresh route `build/emu/route-en-final-layout` confirms it through save/load. The added code-generated Ijuin banner in `tools/patch/art/prologue.py` writes CUT_10.NBN with a source hash guard, original palette and unchanged extent; `build/banner-proof` passes readback, with GPU review pending. The movie gate hashes the final raw extent and reconciles its 29 image IDs into the art report. Catalog-142 is an explicit owner-approved exception. The two compressed stills P_01_02 and D_03_08 remain deferred; magazine, sports, and other off-route coverage remain required.

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
- Relink records: done (T-39506, above).
- JOHO.NBN: 7 club labels and Sagittarius do not fit their fixed slots (12 and 10 bytes). The file is data loaded at 0x801A0000 and TEL reads it at absolute addresses with fixed strides (`phone_profile_details_draw`), and its last 268 bytes are not free. Room needs a new layout of the label tables inside the file (for example packed strings behind a small offset table) and the TEL draw under the flag reading it, or a bigger read whose buffer end is proven free.
- `moved_in_build`: the extents under rewritten C literals. Check after the relink whether any consumer reads them other than through the literal.
- Windows for menus, tables and C strings: add rows to `WINDOWS` as screens are checked in the route.
- Art: the deferred items of the Art section (NAME_ENT panels and the second name-entry page, the exam header, two stills, the movie pills).
- Dialogue display needs the parser work of [[tickets/T-39506-english-script-runtime]]. Until then the control bytes 0x0A-0x1A are skipped by the renderer and the old parsers misread English.

## Compact talk storage (T-39506)

The English build uses `Image.write(..., compact=True)` for talk and supplemental talk containers. Strings are placed longest first in the smallest free region that fits, retaining unused regions for shorter strings. Each record owns distinct bytes, including identical strings, because the game can modify script text at runtime. Structure and pointer locations do not move. Default identity checks retain the original-order writer. The DATE probe fits all eleven files inside 24 sectors without relocating their RAM buffer; the complete build and route must confirm the wrapped payloads.

## Name-entry art extension (T-39506 / T-39508)

`tools/patch/art/name_entry.py` appends seven 4bpp actor pages to NAME_ENT.NBN, leaving the six original background pages and sequence table untouched. Eight actor pages total allow all four panels, selector fallbacks and both ABC states to fit. A marker at file offset 0x1AFF8 selects the extended texture and frame tables in the English overlay; a no-art build still uses the originals. The extended file (including grown frames) is 172,032 bytes in the placement test. The source load begins at 0x80190000 and the next live name-entry data begins at 0x801E0000, giving 0x50000 bytes; the art planner applies this bound to this container only. Runtime validation must check the extra VRAM pages and actor 125 used by ABC. Current sidebar labels remain the 32-pixel fallback art.

The first GPU run of the extended name-entry art exposed direct birthday-digit UVs into the original actor sheet, outside the frame table. The extended profile now protects all of page zero and limits English relocations to the seven added pages; a game-data regression requires the original 0x2000 texture bytes to remain identical. Full-file placement/readback and another emulator run are the gates for this correction.

## Actual-disc script language audit (T-39510)

Executable companion (in progress): `python3 -m tools.patch.audit_executable_disc --build BUILD --link build/en --out REPORT.json` verifies frozen/live ELF, map, linker script and rebuilt object identity, then reads actual executable files from the disc. It checks mapped slots, stage consumer pointers, RPG initial buffers and compiled function references using bounded text-section ownership and decoded MIPS instructions. It intentionally exits 2 while any audit layer remains incomplete. On the ending-stream release it proves 2,827/2,940 mapped slots, three more canonical C payload locations, 47 unhooked function bindings, all 286 stage pointers and five initial buffers. Remaining: 57 records in bypassed original talk-parser bodies, four save-helper labels and two replaced prompts, plus the referenced-byte census. The earlier 104 function-binding diagnostic included the 57 bypassed-body cases and must not be used as active-consumer coverage. Fourteen synthetic executable regressions cover address bounds, relocation, missing consumers, stale links, instruction decoding, function boundaries and hooked bodies.

Hook/helper follow-up: all 23 configured main/NAME_ENT entry jumps and NOP delay slots match their replacement symbols on the actual release disc. Of the 57 bypassed-body records, 37 now have exact canonical payload references in their active replacement functions. Both compiled save/album calls reach the static name helper and that helper references both English labels; both memory-card prompts are referenced by their replacement function. Those six extents are now bound by explicit helper recipes. Twenty parser records remain pending: 14 old `%s ` formats, two old spaces, two `%s%s` formats and two Yoshio substitutions now handled inside a static expansion helper. Their source behavior has been located, but compiled helper/retired-format proof is still needed. Current combined suites pass 31 tests; language-census consolidation also remains open.

Static-helper follow-up resolves those final twenty records. `tools/patch/executable_calls.py` reconstructs local helper regions from named entries and section-relative JAL targets, then verifies every reported call against disc instructions. The two Yoshio substitutions reach their exact payload through the renderer's helper. Eighteen old ASCII templates are retired only with verified hook paths plus separate speaker/mark drawing or bounded append calls; a production C89 positioning test checks visible label/mark separation. All 2,940 slots are accounted for, with 37 tests passing. The executable audit still exits 2 pending the referenced-byte language census.

Source-generation companion: `python3 -m tools.patch.audit_generated_sources --build BUILD --link build/en --out REPORT.json` (Docker only) regenerates every translated source in memory and compares exact bytes with both saved source trees. It rejects missing translation sites, duplicate IDs, missing/altered files and empty verification scope. Unloaded EVENT/GYOZI literals are explicitly reported as excluded. On `build/ending-stream-final-proof`, 1,964 active literals in 171 files pass both snapshots. Seven synthetic tests include swapped call-site payloads that a simple string-presence search would miss. This verifies source provenance only; compiled references and actual-disc language scans remain separate checks.

`python3 -m tools.patch.audit_script_disc --build BUILD --out REPORT.json` (Docker only) reads the ISO and follows the actual script pointer words, verifying encoded target equality and rejecting original-font bytes. Every modeled slot must be checked; fixed-table data slots use their actual offsets, including JOHO's relocated club/zodiac labels. Executable text and artwork are explicitly excluded. Six synthetic tests in `tools/test_script_disc_audit.py` cover relocated pointers/suffix aliases, corrupt pointers/text, fixed slots, profile relocation, original-font bytes and missing coverage.

The first diagnostic census found three retained Japanese fragments caused by malformed original tab bytes; the remaining opaque token was a dangling apostrophe. Four hash-guarded repairs are in `translations/en/anomaly-repairs.json`, applied by `text/anomaly_repairs.py` without changing raw sources or exported rows. `build/anomaly-repair-proof` passes the final-disc audit: 105 text-data containers, 31,471 strings, 31,334 pointers, exact target equality and zero original-font bytes. Report: `build/script-disc-audit-final.json`. Executable text and broader runtime/art coverage remain open in [[tickets/T-39510-no-japanese-left-check]].

## Stage and menu correction release, 2026-11-03

Published target `b579f2ad31288f442436f718eba0cb61f7e359fe` restores 214 source-proven stage commands without changing the visible English. The stage speaker column adapts to full names; the mascot picker uses two columns clear of its preview. Fresh-state captures, actual-disc checks and the 15-check browser run are recorded in `translations/en/reviews/stage-controls-and-menu-spacing-2026-11-02.json`. All 37 served files match the tested package (`published-stage-controls-2026-11-03.json`). The broader quality ticket T-39511 remains open; publication does not claim every route has been played end to end.
