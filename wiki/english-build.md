---
type: tool
updated: 2026-11-01
sources: ["tools/patch/build_english.py", "tools/patch/cd_layout.py", "tools/patch/text/english.py", "tools/patch/text/windows.py", "tools/patch/text/wrap.py", "tools/patch/text/layout.py", "tools/patch/text/inplace.py", "src/english/cd.c", "include/english/cd.h", "tools/test_patch_english.py"]
---

# English build pipeline

One command builds the English disc and its patch. Ticket [[tickets/T-39507-english-disc-build]], part of [[tickets/T-39500-english-translation-patch]]. It builds on the English build variant ([[english-text-runtime]]), the text-container writers ([[english-text-containers]]), the disc rebuild ([[tickets/T-39501-disc-rebuild-and-xdelta]], [[disc-layout]]) and the emulator route ([[emulator]]).

## Running

```
IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english   # about 4 minutes
tools/docker.sh python3 -m unittest tools.test_patch_english -v            # no game data
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
5. `build/en/cd_layout.json` (the moved files), then `configure.py` and `ninja english`. The table `english_cd_remap` (part of the main executable since the free relink) comes from that JSON (`cd_layout.py remap-asm`, `build/en/gen/english_cd_remap.s`). Without the JSON the table is empty, so `ninja english` alone still works.
6. In-place records go into the built executable and overlays (`text/inplace.py`):
   - Each slot is looked up where the free link put it (`english_link.translator`: the anchor of its input section, or a symbol or relocation inside a rebuilt object) and written only where the English binary holds the original bytes; otherwise it counts as `moved_in_build`.
   - EVENT and GYOZI keep their original files.
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

## Open

- DATE files: the RAM map shows the buffer cannot grow in place (TAKU3.BIN and another load follow it during a date; [[ram-map]]). They need a smaller encoding, a split, or DATE code that loads them elsewhere.
- Relink records: the C literals now place (T-39505). The 266 fixed-size slots over capacity need code changes to their tables.
- `moved_in_build`: the extents under rewritten C literals. Check after the relink whether any consumer reads them other than through the literal.
- Windows for menus, tables and C strings: add rows to `WINDOWS` as screens are checked in the route.
- Dialogue display needs the parser work of [[tickets/T-39506-english-script-runtime]]. Until then the control bytes 0x0A-0x1A are skipped by the renderer and the old parsers misread English.
