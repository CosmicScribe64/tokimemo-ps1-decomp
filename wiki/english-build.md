---
type: tool
updated: 2026-11-01
sources: ["tools/patch/build_english.py", "tools/patch/art/", "tools/test_patch_art.py", "tools/patch/cd_layout.py", "tools/patch/text/english.py", "tools/patch/text/windows.py", "tools/patch/text/wrap.py", "tools/patch/text/layout.py", "tools/patch/text/inplace.py", "src/english/cd.c", "include/english/cd.h", "tools/test_patch_english.py"]
---

# English build pipeline

One command builds the English disc and its patch. Ticket [[tickets/T-39507-english-disc-build]], part of [[tickets/T-39500-english-translation-patch]]. It builds on the English build variant ([[english-text-runtime]]), the text-container writers ([[english-text-containers]]), the disc rebuild ([[tickets/T-39501-disc-rebuild-and-xdelta]], [[disc-layout]]) and the emulator route ([[emulator]]).

## Running

```
IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english   # about 4 minutes
tools/docker.sh python3 -m unittest tools.test_patch_english -v            # no game data
IMG=tokimemo-patch tools/docker.sh python3 -m unittest tools.test_patch_art -v  # art writers
IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_ENGLISH_DISC=1 python3 -m unittest tools.test_patch_english -v
tools/emu/run.sh route run --label en --build jp --cue build/en/disc/tokimemo-en.cue
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
   - While the English link pins objects to their original addresses, an object must not grow a section or move a symbol. The fit loop compares each English object with the default one. Round 1 sets a failing file's growing literals back to the original; round 2 does the same for the whole file. Those records are reported as `needs_relink`.
   - `PINNED_LINK` in `build_english.py` turns the loop off once [[tickets/T-39505-english-memory-and-relink]] links freely.
4. Data files:
   - Every talk script, supplemental script and `JOHO.NBN` is written with `Image.write` and checked by `cd_layout.plan_file`. It is then put on the disc with `Disc.replace`: in place when it keeps its sector count, otherwise at the end of the data track.
   - A file that may not grow keeps its original bytes.
4b. The English art (section Art below): every registered image into its container, placed like a data file; after step 5 the MAINROM part records go into the built executable, and after track 1 is written the art is read back and verified.
5. `build/en/cd_layout.json` (the moved files), then `configure.py` and `ninja english`. The island table `english_cd_remap` comes from that JSON (`cd_layout.py remap-asm`, `build/en/gen/english_cd_remap.s`). Without the JSON the table is empty, so `ninja english` alone still works.
6. In-place records go into the built executable and overlays (`text/inplace.py`):
   - A slot is written only where the English binary still holds the original bytes; otherwise it counts as `moved_in_build`.
   - The overlay checksum trailer is recomputed.
   - Then the changed overlays go on the disc (in place), then the executable, which moves to the end because it gained sectors.
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
- The load buffer: the new count must not exceed the largest original read into the same address. The DATE buffer at 0x80198000 is fixed at 24 sectors (`BUFFER_LIMITS`); a file that does not fit keeps its original bytes.
- A literal `cd_file_checksum_ok` with the file's original count is a problem.
- At most 255 sectors (u8).

## Linker change (`tools/english_link.py ld`)

A rebuilt object that shrinks was already padded by the pin of the next input section. Nothing pinned the end of the last input of an output section, though, so `X_RODATA_END` and `X_DATA_START` moved with it, and the overlay check failed for BUNKAKEN. The section labels inside output sections are now pinned at their default addresses too (`read_labels`).

## Results (2026-11-01)

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

Step 4b of the build writes every registered English image into its container ([[tickets/T-39508-english-art-on-disc]]). `--no-art` leaves the images Japanese. To plan the art alone (no disc): `IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.art.build --report build/en/art-report.json`. Tests: `IMG=tokimemo-patch tools/docker.sh python3 -m unittest tools.test_patch_art -v` (synthetic, plus game-data checks when `disc/files` is there). The image `tokimemo-patch` now has numpy and Pillow (`tools/Dockerfile.patch`).

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

Results (2026-11-01, build 154 s with cached stills; a clean still cache adds about 12 minutes):

| Status | Items |
|---|---|
| placed_verified | 230 |
| deferred | 14 |
| untranslated_by_decision | 27 |
| missing | 1 (D_11_11 ride sign) |

Images: 111 in place, 16 relocated, 107 rebuilt, 14 stills re-encoded, 176 palette variants the game never draws, 67 deferred (29 of them movie pills). 29 containers written (one still, D_19_02, grew a sector and moved to the end of track 1), 30 MAINROM part records.

- MAINROM: the stat bar (frame 61: Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress; the label blocks rewritten where they are, two shared arrow tiles moved), the magazine headlines, banners, tags and covers, the weekdays except Monday. Deferred: the Save labels (57-59), the date-panel captions (60) and Monday (82): the eight uploaded sheets have no free 16x16 cell (`boot_hud_textures_load` uploads sheets 0-3 and 7-10; sheets 4-6 are not in VRAM, which the first route run showed as garbage), the frame records cannot grow, and frame 60 and the Monday/Sunday weekdays share the 月 and 日 tiles.
- OPTION: all 108 frames; 102 rebuilt per word (the Japanese labels share kanji tiles; normal and highlight frames keep sharing texels), 6 in place. Kiyokawa (0103/0104) and Tatebayashi (0113/0114) ship the family-name fallbacks: no room for the widened labels.
- TITLE 4, FILESEL 13 frames and 3 information pages (all in place), the EXAMINE suffix (fallback `pt`), the ALL_C banner, the five battle files (command and MISS labels, in place), the five sports cards (tile words rewritten).
- NAME_ENT: the ten selector labels ship as the 32-texel fallbacks: the cells sit at fixed positions in `name_entry.c` and the single 4bpp page has no room for 56-texel cells.
- Stills re-encoded: P_01_01 (gate plate), P_01_03 and P_01_03B (roster close-ups), C_01_28 (chalkboard), D_01_47, D_01_48, D_01_70, D_15_03, D_19_02 (both signs), D_24_03, D_29_09, I_17_01, I_28_02. Unchanged pixels move at most 1 step on C_01_28, D_29_09 and P_01_03B and at most 3 on the others; the English pixels have PSNR 23 to 40 dB against the composited crops (max error 1 to 13 steps).
- On the route: title, options, file select, name entry (fallback labels), the command-screen stat bar, calendar, save and load, the gate plate and the roster close-ups. Not on the route (57 items): the battle labels, sports cards, ALL_C banner, exam suffix, every D_/C_/I_ still and the MAINROM magazine frames (0121-0154); they are checked only by `verify art`.

Deferred:
- MAINROM 57-59, 60, 82: see above. Needs room on an uploaded sheet: another sheet upload in the English build, or frame records with more parts after the relink ([[tickets/T-39505-english-memory-and-relink]]).
- NAME_ENT panels 0000, 0001, 0002, 0014: no free 32x64 rectangle on the page (panel 0 repeats a tile with two English contents; 1, 2 and 14 draw where no part is).
- EXAMINE results header (exam-0000): no free 32x64 rectangle once the score digits (direct sprites) are kept.
- P_01_02 class signs (6 crops) and the D_03_08 placard: every attempt has a float IDCT 31 steps off the fixed decode (saturation).
- `name-label-abc` (lit, unlit): needs the English letter page of [[tickets/T-39506-english-script-runtime]]; the 32-texel fallback is ready (same cell as frames 15/16) and needs a frame in NAME_ENT and a selector in `name_entry.c`.
- OPENXA movie pills (29): need a runtime cue player that draws them over the movie.

Other findings: the FILESEL System tab word is cut by the availability mark the code draws next to it (the Japanese draws 有 there; the English text step draws a letter at the same place).

## Open

- DATE files: they need the RAM map (T-39505) to show that the DATE buffer can grow, or a smaller encoding.
- Relink records: C literals in pinned objects, plus main and overlay tables over capacity (T-39505).
- `moved_in_build`: the extents under rewritten C literals. Check after the relink whether any consumer reads them other than through the literal.
- Windows for menus, tables and C strings: add rows to `WINDOWS` as screens are checked in the route.
- Art: the deferred items of the Art section (MAINROM sheet room, NAME_ENT panels, two stills, the ABC label, the movie pills).
- Dialogue display needs the parser work of [[tickets/T-39506-english-script-runtime]]. Until then the control bytes 0x0A-0x1A are skipped by the renderer and the old parsers misread English.
