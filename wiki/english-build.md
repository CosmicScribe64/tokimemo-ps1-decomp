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

## Open

- DATE files: they need the RAM map (T-39505) to show that the DATE buffer can grow, or a smaller encoding.
- Relink records: C literals in pinned objects, plus main and overlay tables over capacity (T-39505).
- `moved_in_build`: the extents under rewritten C literals. Check after the relink whether any consumer reads them other than through the literal.
- Windows for menus, tables and C strings: add rows to `WINDOWS` as screens are checked in the route.
- Dialogue display needs the parser work of [[tickets/T-39506-english-script-runtime]]. Until then the control bytes 0x0A-0x1A are skipped by the renderer and the old parsers misread English.
