---
type: design
updated: 2026-11-01
sources: ["src/main/text_system.c", "src/main/scene_library.c", "src/main/menu_speaker_girl.c", "src/english/", "tools/english_link.py", "tools/english_font.py", "translations/en/font/latin12.txt", "config/english_hooks.txt"]
---

# English text runtime

How the English build draws text: build variant, encoding, font, layout, and the work list for full English. Ticket: [[tickets/T-39502-english-text-runtime]] (part of [[tickets/T-39500-english-translation-patch]]). Addresses and routine roles come from [[ref-english-patch]].

The original's text limits are not limits for English. The game draws every character from a 12 x 12 Shift-JIS font at a fixed advance of 14 or 16 px, and its writers count 2 bytes per character. Because we build the code, the English build uses a proportional font, single-byte letters and pixel-width layout.

## Build variant

`ninja english` builds the English variant into `build/en/`. The default targets do not include it, and the default build still prints OK for all 27 binaries.

- **Flagged objects.** A C file that mentions `TOKIMEMO_ENGLISH` is compiled again with `-DTOKIMEMO_ENGLISH` into `build/en/<path>.o`. Every other object is reused from the default build. `configure.py` stops if a header outside `include/english/` mentions the flag, because only flagged `.c` files are rebuilt. Rerun `configure.py` after adding the flag to a new file.
- **Fixed addresses.** Every original address stays the same. Overlays call the main exe at absolute addresses from the symbol files, data in both holds raw code addresses (751 words of `0x8007C784` alone), and `ovl_load_enter` hard-codes the 26 overlay entry points. `tools/english_link.py ld` rewrites the default linker script so that every input section is placed (`. = offset`) at its address from the default map. A flagged object that shrinks is padded. One that grows fails the link with `cannot move location counter backwards`, on the line that names it.
- **Island.** New main-exe code and data go in the island. It is linked to run at `english_island_start` = end of the main bss (0x8012B540), with a limit of 0x80132000, the overlay slot (27,328 bytes; checked by a linker `ASSERT` and in the post-link step). In the file the island sits right after `.data`, so the BIOS loads it at 0x800E3800 and up (bss nothing has used yet). `src/english/boot.s` (`english_boot`) is the new header entry. It copies the island into place, clears the island bss, calls FlushCache (A0:44h) and jumps to `boot_start` (whose `bzero` ends at 0x8012B538, below the island). Current use: 4,352 bytes of code and data plus 768 bytes of bss, ending at 0x8012C940. Island symbols are passed to the overlay links (`build/en/island_syms.ld`), so overlay C can call `english_*` functions.
- **Hooks.** `config/english_hooks.txt` lists `<unit> <original> <replacement>`. The post-link step writes `j replacement; nop` over the original's first two instructions, so every caller, overlays included, reaches the replacement. The original body stays in place and never runs. Current hooks: `load_string`, `set_kanji_string` and `disp_string`, replaced by the `english_*` functions of `src/english/text.c`.
- **Checks** (`tools/english_link.py exe` / `ovl`):
  - Main exe: every symbol of the default ELF keeps its address. The island bss must end at or below 0x80132000. The file must fit the disc slot of 376 sectors (LBA 24 up to KONAMIC3.STR at 400, 0xBC000 bytes; [[tickets/T-39501-disc-rebuild-and-xdelta]] moves files if it ever has to grow further). The step sets header `pc0` = `english_boot` and `t_size`, and pads to 2048 bytes.
  - Overlays: 0x30000 bytes at most. No function may move (data inside a flagged object may). The step pads to 0x30000 and rewrites the checksum trailer: sum16 of all other bytes, big-endian, then 0x12 0x10. This was checked against all 26 default overlays.
- **Outputs:** `build/en/SLPM_86.053.bin` and `build/en/ovl/<NAME>.bin`. Today the English exe differs from the default exe in 6 places: header `pc0` and `t_size`, and 3 hooks; then the island follows. Only NAME_ENT differs among the overlays.

Overlay limit in this layout: a flagged overlay object may not grow. That is enough for the first string, but not for a full translation; see open risks.

## Encoding

Two kinds of strings reach the renderer:
- Display strings go straight to `set_kanji_string`: menus, labels, file select and similar.
- Script strings are parsed first by `talk_render`, `talk_speak_part`, `sndisp` and a few others.

Recommended direction, checked against the code: single-byte ASCII for English, mixed freely with the existing 2-byte codes.

| Byte | Meaning in English strings |
|---|---|
| 0x01-0x1F | control codes of the English script format (below); the renderer skips them |
| 0x20-0x7E | one glyph of the proportional font (ASCII, 0x20 is a real space) |
| 0xA0-0xDF | extra single-byte glyphs: curly quotes, em dash, ellipsis, heart, note, e-acute, star (the half-width katakana range of Shift-JIS, which the game's font never had) |
| 0x81-0x9F, 0xE0-0xFC | lead byte of a 2-byte Shift-JIS character from the game font, at the fixed advance (14 or 16) |

The plain ASCII direction collides with the original script format. A script string uses 0x20 as end of display line, `q` as end of choice option and `n` as an empty option, and `P p m F Y G S O W A M` as name/value codes, all as single bytes in a 2-byte stream (`talk_render` steps 2 bytes and backs up 1 on a code). English letters and spaces cannot keep those meanings. **Script strings therefore get their own control bytes in English**, and the text writers ([[tickets/T-39503-text-container-writers]]) translate readable tags into them:

| Original | English byte | Tag in the source text |
|---|---|---|
| 0x20 (line end) | 0x0A | newline in the record |
| `q` (option end) | 0x0B | `{q}` |
| `n` (empty option) | 0x0C | `{n}` |
| `P p m F Y G S O W A M` | 0x10-0x1A in that order | `{P}` `{p}` ... `{M}` |

Display strings need no control bytes.

## Font

- Source: `translations/en/font/latin12.txt`. This is our own drawing (committed), a text bitmap with one block per glyph (`glyph 0xNN name`, then 12 rows of `.`/`#`). There are 105 glyphs: ASCII 0x20-0x7E and 10 extras.
- Metrics: cap height 9 px (rows 1-9), x-height 6 (rows 4-9), descenders on rows 10-11. The advance is the ink width plus 1 px. Most letters are 5 px wide (advance 6), `i l . ! '` are 1 px, `M W m w` are 7 px.
- `tools/english_font.py` writes `build/en/gen/english_font.s`: `english_font_index[256]` (code to glyph, 0xFF none), `english_font_advance[]`, and `english_font_glyphs[]` in the game's 18-byte 12 x 12 1bpp format. It also writes `build/en/font_metrics.json` (advance per code, `sjis_advance` 14, `line_height` 16) for build-time wrapping.

## Rendering and layout

- **Glyph slots unchanged.** Each character still takes the next VRAM slot (`text_state.total`: 16 x 12 4bpp cells, 8 x 16 per page, pages at x 0x320/0x360/0x3A0, 384 slots). The typewriter (`k_disp_inc`, one glyph per tick), `get_now_k`, `k_reset` and every glyph count keep their meaning; a space is a glyph too.
- **Pixel positions.** `english_load_string` records each slot's x offset from the start of its entry in `english_glyph_pen[384]`. An ASCII glyph adds its font advance and a Shift-JIS character adds `text_state.glyph_advance`. `english_disp_string` is `disp_string` with `x = entry.x + pen[i]` instead of `entry.x + k * advance`. The shadow sprite, CLUTs, texture page switching and OT order are unchanged.
- **One-letter strings.** `english_set_kanji_string` drops the original's check that the second byte is nonzero.
- **Build-time wrap.** Lines are wrapped when the text is built, using the same width table; files carry pre-wrapped text and the runtime only advances pixels. The text writers measure in pixels, not bytes: the box width in px (from the window's x and the frame art), 16 px per line, and the number of lines per box. The original text box shows 3 lines, and scene code waits on line indices, so a translated line may not change the line count of a string unless the scene code changes with it.
- **Glyph cache reuse.** Not needed so far. A 3-line box of about 45 characters per line uses about 135 of 384 slots, and every screen starts from `text_system_reset`. If a screen runs out, map unique glyphs to slots with a slot index per glyph in `english_disp_string`; the typewriter would still count glyphs, not slots.
- **Windows.** Clear and scroll stay as in the original: `text_system_reset` and `text_glyph_pages_clear`, `k_reset`/`k_sub_reset`, `check_k_scroll`. They work on entries and glyph counts, not on pixels.
- **Centering.** Centered and right-aligned strings compute x from byte counts (for example `-strlen(p) * 7 / 2` = half of 14 px per 2 bytes). English needs a pixel width function (`english_text_width(str)`, from the same tables) at each such site.
- **Names.** `player_family_name` and `player_given_name` are 8-byte fields at GameState 0xD4/0xDC, next to each other and part of the save data, so the layout stays. With single-byte letters a field holds 7 letters plus NUL. 8 letters need every reader bounded to 8 (`strncpy`, not `strlen`), because a full field has no NUL. The name entry screen sets advance 16 (`text_set_advance`) and builds its grid from 2-byte kana tables. The English letter page needs its own table and a 1-byte cursor step.
- **Numbers.** The game writes numbers as full-width Shift-JIS digits (`strcat(dst, "０")` in the RPG battle HUD, digit literals in result strings). In English these become ASCII digits, which the mixed encoding draws directly; each helper that builds digit strings changes to write 1-byte digits. Calendar and parameter numbers are sprites (art, [[assets-media]]).

## Functions to change for full English

The census comes from grepping `src/`: 1,172 `set_kanji_string` calls in 52 files, and every `strlen` on display text.

Text system (main, done or hooked):
- `load_string`, `set_kanji_string`, `disp_string`: replaced (hooks). `vload_font` and `text_glyph_expand` are used as they are for Shift-JIS.
- `text_set_advance` and `text_system_reset`: keep. The advance applies to Shift-JIS characters only.

Script parsers (main, `src/main/scene_library.c` and `src/main/menu_speaker_girl.c`):
- `talk_render`: 2-byte stepping, control letters, 64-byte line buffers and 512-byte copy. Rewrite to step 1 byte on the English control bytes, use larger line buffers (names expand to 7 letters), and give the `（` indent test an ASCII rule.
- `talk_speak_part`: splits replies at `q` (0x71), tests `nn` (0x6E) at line ends in five places (lines 1278-1559), and scans a reply loop at line 1638. Move these to 0x0B/0x0C.
- `make_three_select`: scans `P G q ' '` in `source_text` and joins the three options with `sprintf("%sq%sq%s")`; both move to the English control bytes. Recheck `talk_run`, which calls into these, once they change.
- `sndisp`: splits at 0x20 and `P`/`G` with byte parity; `sndi`: `P`/`G` with `(i & 1)`; `gnsx`: `switch (strlen(speaker_name))` picks a label width by byte count.
- `get_g_name`, `get_p_name`: girl short names and honorific suffixes (`sprintf("%sさん")`), 10-cell `girl_name_buf`. Strings come from tables; English names and honorific policy go in the text records.
- `D_80122D1C` test in `talk_render`: an all-punctuation check over 2-byte codes.

Width by byte count (each needs `english_text_width`):
- `src/ovl/amusement_park_date/karaoke_scene_and_return.c`: 35 `strlen` sites, x = `strlen * 7` and menu widths `strlen / 2 * 16`.
- `src/ovl/ending/graduation_epilogue_handlers.c:196`: centered `-strlen(p) * 7 / 2`.
- `src/ovl/culture_stage_shows/literature_speech_contest.c:564-610`: player name length and x offset `strlen * 4`.
- `src/main/hud_panels.c:1182`: `strlen(arg0) / 2`.
- `src/ovl/name_entry/name_entry.c`: kana grid tables, `strlen / 2` column count (line 1094), `text_set_advance(16)`, 8-byte name buffers.
- `src/ovl/options/settings_sound_test_and_extras.c`: `text_set_advance` caller.
- Any other x computed from a character count: grep `strlen(` and `* 0xE`/`* 0x10` next to `set_kanji_string` per file while translating.

Number writers:
- `src/ovl/rpg_battle/hud_messages_and_swimming_effect.c` (digit `strcat`) and the RPG result strings, and any other site that builds full-width digits.

Strings:
- Main `.rodata` strings: 0x800AF340-0x800B3220, game strings first. Overlay strings come either as `INCLUDE_RODATA` pieces or as C literals. All go through T-39503's writers.

## Implemented (T-39502)

- `configure.py` English variant, `tools/english_link.py`, `tools/english_font.py`, `src/english/boot.s`, `src/english/text.c`, `include/english/text.h`, `config/english_hooks.txt`, `translations/en/font/latin12.txt`.
- First English screen: `file_select_memory_card_check_begin` (NAME_ENT, `src/ovl/name_entry/file_select.c`). Under the flag it shows two English lines in place of the memory-card check message. The literals fit the 44 bytes of the two original rodata strings, which the English build leaves out.
- Emulator proof (2026-11-01). The test disc was a copy of the original track 1 with the English exe and NAME_ENT written in place: a temporary script, not committed; T-39501's `tools/patch/disc.py` does this properly. Route: boot, then title, then Game Start, then the file select screen. The screenshot shows the English lines in the proportional font next to the original Japanese labels. Memory watches over the island and the free gap (0x8012B540-0x80132000) saw no writes from game code during the opening, the title and the file select. A watch on `english_glyph_pen` confirmed that the island code runs.

## Open risks

- **Overlay space.** Pinned overlays cannot grow. Most overlays end in a long run of zero bytes (NAME_ENT 82 KB, OPTION 145 KB, DATE 5 KB, TACO 291 bytes), but that run sits inside the last splat data object and may be work memory. Two ways forward:
  - an overlay island: trim that object's zero tail under the flag, after a memory watch proves the space unused;
  - free relinking of overlays: overlays have no raw overlay-range words in their data and no literal overlay addresses in C. The entry table in `ovl_load_enter` would then come from the English maps, and the `PROVIDE` labels of `build/ovl/<NAME>_data_syms.ld` (absolute addresses inside C data) would have to be derived again.
  TACO, DATE, BUNKASAI and RPG_BAT have little zero tail.
- **Main island.** 27 KB in total, 5 KB used. The `talk_render` rewrite and the width helpers fit; main-exe English strings (16 KB of original rodata, roughly the same in English) would not all fit next to code. Options: put them in the overlay islands per screen, or load a string bank from disc.
- **Untested gap use.** The watches cover only the opening, the title and the file select. Run a longer route ([[tickets/T-39504-emulator-screen-route]]) with the same watch before relying on the gap.
- **VRAM.** 384 glyph slots per screen. English uses about 2x the glyphs of the Japanese for the same text (letters plus spaces), so a full-screen list could run out; see glyph cache reuse above. Each glyph cell keeps its 16 x 16 sprite; English glyphs use at most 12 px of it.
- **Performance.** Each glyph upload is `LoadSquare` plus a `DrawSync` (`func_8009C674`), once per string layout, not per frame. About twice as many uploads when a box opens, still well under a frame for a 3-line box. Batching the syncs is possible if a long menu stalls.
- **Encoding collisions.** Any other code that scans strings for `q`, `n`, 0x20 or the name letters must move to the English control bytes. The list above comes from a grep for `== 'q'`-style compares; parsers that test the values through tables would be missed.
- **Disc and EDC.** The temporary test disc kept stale EDC/ECC; the emulator ignores them, but real hardware would not. Use T-39501's disc tool for anything beyond a smoke test.
