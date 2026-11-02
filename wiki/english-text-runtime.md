---
type: design
updated: 2026-11-01
sources: ["src/english/script.c", "src/ovl/name_entry/name_entry.c", "include/english/script.h", "tools/english_link.py", "src/main/text_system.c", "src/main/scene_library.c", "src/main/menu_speaker_girl.c", "src/english/", "tools/english_link.py", "tools/english_font.py", "translations/en/font/latin12.txt", "config/english_hooks.txt"]
---

# English text runtime

How the English build draws text: build variant, encoding, font, layout, and the work list for full English. Ticket: [[tickets/T-39502-english-text-runtime]] (part of [[tickets/T-39500-english-translation-patch]]). Addresses and routine roles come from [[ref-english-patch]].

The original's text limits are not limits for English. The game draws every character from a 12 x 12 Shift-JIS font at a fixed advance of 14 or 16 px, and its writers count 2 bytes per character. Because we build the code, the English build uses a proportional font, single-byte letters and pixel-width layout.

## Build variant

`ninja english` builds the English variant into `build/en/`. The default targets do not include it, and the default build still prints OK for all 27 binaries.

- **Flagged objects.** A C file that mentions `TOKIMEMO_ENGLISH` is compiled again with `-DTOKIMEMO_ENGLISH` into `build/en/<path>.o`. Every other object is reused from the default build. `configure.py` stops if a header outside `include/english/` mentions the flag, because only flagged `.c` files are rebuilt. Rerun `configure.py` after adding the flag to a new file.
- **Free relink** (T-39505, [[relink]]). `tools/english_link.py` links every unit from its default script without pinning anything: a flagged object may grow and everything after it moves. Addresses the linker does not see (side-script numbers, the overlay entry points of `ovl_load_enter`, 2,348 raw function pointers in overlay data and a few more) are translated or patched; the room is in [[ram-map]].
- **English code.** `src/english/*.c` and the font are appended to the main executable's `.text`, `.rodata` and `.data` (their bss as zeros in the file); the bss moves up behind them. There is no island and no boot stub any more: `pc0` is `boot_start`. Current use: 4,352 bytes of code and data plus 768 bytes of bss; the executable ends at 0x8012C940 (limit 0x80132000). New main symbols are given to the overlay links, so overlay C can call `english_*` functions.
- **Hooks.** `config/english_hooks.txt` lists `<unit> <original> <replacement>`. The post-link step writes `j replacement; nop` over the original's first two instructions, so every caller, overlays included, reaches the replacement. The original body stays in place and never runs. T-39506 adds the script parsers and name functions (`src/english/script.c`); a hooked original cannot be called any more, so a replacement repeats the whole function. Current hooks: `load_string`, `set_kanji_string` and `disp_string`, replaced by the `english_*` functions of `src/english/text.c`. With the free relink a flagged object may also change a function in place.
- **Checks**: see [[relink]] (anchors, second main link, raw addresses) and [[ram-map]] (main executable up to 0x80132000, overlay data up to 0x80161FFC, image exactly 0x30000 bytes with the checksum trailer: sum16 of all other bytes, big-endian, then 0x12 0x10).
- **Outputs:** `build/en/SLPM_86.053.bin` and `build/en/ovl/<NAME>.bin`. Every overlay differs from the default one now, because the main data they point to moved.
- **Emulator route:** a relinked disc needs `tools/emu/run.sh route run --relink build/en ...`, because GameState and the cursor moved with the bss.

## Encoding

Two kinds of strings reach the renderer:
- Display strings go straight to `set_kanji_string`: menus, labels, file select and similar.
- Script strings are parsed first by `talk_render`, `talk_speak_part`, `sndisp` and a few others.

Recommended direction, checked against the code: single-byte ASCII for English, mixed freely with the existing 2-byte codes.

| Byte | Meaning in English strings |
|---|---|
| 0x01-0x1F | control codes of the English script format (below); the renderer skips them |
| 0x20-0x7E | one glyph of the proportional font (ASCII, 0x20 is a real space) |
| 0xA0-0xAF | extra single-byte glyphs: curly quotes, em dash, ellipsis, heart, note, e-acute, star |
| 0xB0-0xDF | stable player-name letter pairs from `include/english/name_pairs.inc`; one stored byte expands to two ordinary Latin glyphs |
| 0x81-0x9F, 0xE0-0xFC | lead byte of a 2-byte Shift-JIS character from the game font, at the fixed advance (14 or 16) |

The plain ASCII direction collides with the original script format. A script string uses 0x20 as end of display line, `q` as end of choice option and `n` as an empty option, and `P p m F Y G S O W A M` as name/value codes, all as single bytes in a 2-byte stream (`talk_render` steps 2 bytes and backs up 1 on a code). English letters and spaces cannot keep those meanings. **Script strings therefore get their own control bytes in English**, and the text writers ([[tickets/T-39503-text-container-writers]]) translate readable tags into them:

| Original | English byte | Tag in the source text |
|---|---|---|
| 0x20 (line end) | 0x0A | newline in the record |
| `q` (option end) | 0x0B | `{q}` |
| `n` (empty option) | 0x0C | `{n}` |
| `P p m F Y G S O W A M` | 0x10-0x1A in that order | `{P}` `{p}` ... `{M}` |
| inline label `>` `-` `+` (4, 6, 2 label bytes) | 0x1C, the label, 0x1F | `[[inline_label]]Teacher: ...`: the writer splits the label at the first `: ` |
| inline label `<` (4 label bytes, thought) | 0x1D, the label, 0x1F | as above |
| `nn` at the end (branch zero) | 0x0C 0x0C | `{n}{n}` |

Display strings need no control bytes. The constants are in `include/english/script.h` (T-39506). `sndisp` strings (TEL, prologue, holiday hints) are script strings too: 0x0A ends a line, 0x10 and 0x15 are the player and girl names. `F` is now the family name, a space and the given name.

## Font

- Source: `translations/en/font/latin12.txt`. This is our own drawing (committed), a text bitmap with one block per glyph (`glyph 0xNN name`, then 12 rows of `.`/`#`). There are 105 glyphs: ASCII 0x20-0x7E and 10 extras.
- Metrics: cap height 9 px (rows 1-9), x-height 6 (rows 4-9), descenders on rows 10-11. The advance is the ink width plus 1 px. Most letters are 5 px wide (advance 6), `i l . ! '` are 1 px, `M W m w` are 7 px.
- `tools/english_font.py` writes `build/en/gen/english_font.s`: `english_font_index[256]` (code to glyph, 0xFF none), `english_font_advance[]`, and `english_font_glyphs[]` in the game's 18-byte 12 x 12 1bpp format. It also writes `build/en/font_metrics.json` (advance per code, `sjis_advance` 14, `line_height` 16) for build-time wrapping.

## Rendering and layout

Speaker names use the existing pink font palette and leave eight pixels before the opening mark in the English build (T-39522, owner request). Script and telephone paths share this spacing, and `tools/patch/text/speech.py` reserves it during wrapping. Dialogue and punctuation keep their existing colors.

- **Glyph slots unchanged.** Each character still takes the next VRAM slot (`text_state.total`: 16 x 12 4bpp cells, 8 x 16 per page, pages at x 0x320/0x360/0x3A0, 384 slots). The typewriter (`k_disp_inc`, one glyph per tick), `get_now_k`, `k_reset` and every glyph count keep their meaning; a space is a glyph too.
- **Pixel positions.** `english_load_string` records each slot's x offset from the start of its entry in `english_glyph_pen[384]`. An ASCII glyph adds its font advance and a Shift-JIS character adds `text_state.glyph_advance`. `english_disp_string` is `disp_string` with `x = entry.x + pen[i]` instead of `entry.x + k * advance`. The shadow sprite, CLUTs, texture page switching and OT order are unchanged.
- **One-letter strings.** `english_set_kanji_string` drops the original's check that the second byte is nonzero. For an empty string it also sets the entry's glyph count to 0 (the original leaves the count of the slot's previous string), and `english_disp_string` skips any number of glyph-less entries before a glyph (the original moves one entry per glyph). Both were harmless while every name was padded with full-width spaces; with an empty English name the next entry's glyphs went to the inactive one (T-39506).
- **Build-time wrap.** Lines are wrapped when the text is built, using the same width table; files carry pre-wrapped text and the runtime only advances pixels. The text writers measure in pixels, not bytes: the box width in px (from the window's x and the frame art), 16 px per line, and the number of lines per box. The original text box shows 3 lines, and scene code waits on line indices, so a translated line may not change the line count of a string unless the scene code changes with it.
- **Glyph cache reuse.** Not needed so far. A 3-line box of about 45 characters per line uses about 135 of 384 slots, and every screen starts from `text_system_reset`. If a screen runs out, map unique glyphs to slots with a slot index per glyph in `english_disp_string`; the typewriter would still count glyphs, not slots.
- **Windows.** Clear and scroll stay as in the original: `text_system_reset` and `text_glyph_pages_clear`, `k_reset`/`k_sub_reset`, `check_k_scroll`. They work on entries and glyph counts, not on pixels.
- **Centering.** Centered and right-aligned strings compute x from byte counts (for example `-strlen(p) * 7 / 2` = half of 14 px per 2 bytes). English needs a pixel width function (`english_text_width(str)`, from the same tables) at each such site.
- **Names.** See "Names and name entry" below.
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

- `configure.py` English variant, `tools/english_link.py`, `tools/english_font.py`, `src/english/boot.s` (removed by T-39505), `src/english/text.c`, `include/english/text.h`, `config/english_hooks.txt`, `translations/en/font/latin12.txt`.
- First English screen: `file_select_memory_card_check_begin` (NAME_ENT, `src/ovl/name_entry/file_select.c`). Under the flag it shows two English lines in place of the memory-card check message. The literals fit the 44 bytes of the two original rodata strings, which the English build leaves out.
- Emulator proof (2026-11-01). The test disc was a copy of the original track 1 with the English exe and NAME_ENT written in place: a temporary script, not committed; T-39501's `tools/patch/disc.py` does this properly. Route: boot, then title, then Game Start, then the file select screen. The screenshot shows the English lines in the proportional font next to the original Japanese labels. Memory watches over the island and the free gap (0x8012B540-0x80132000) saw no writes from game code during the opening, the title and the file select. A watch on `english_glyph_pen` confirmed that the island code runs.

## Script parsers (T-39506)

`src/english/script.c` replaces, through hooks, `talk_render`, `talk_speak_part`, `make_three_select`, `talk_names_set` (src/main/scene_library.c) and `gnsx`, `sndisp`, `sndi`, `get_g_name`, `get_p_name` (src/main/menu_speaker_girl.c). They keep the originals' control flow, text entries (one per line, label and mark entries as before, so `check_set_k` counts and typewriter starts are unchanged) and voice handling, with these changes:

- One-byte scanning of the control bytes above; no byte parity. A Shift-JIS pair is still copied whole.
- Japanese left in the original format still works: a string with no byte in 0x01-0x1F and at least one Shift-JIS character is converted on entry (`english_script_from_original`, two-byte stepping as the original: 0x20, `q`, `n`, the name letters and the fixed-length inline labels become the English bytes). Without this the English build hung in the prologue, whose NAME_ENT strings are still Japanese (a 5-row page became one row and the scroll step never ended). An English string with a game-font Shift-JIS character and no control byte would be taken as original, so the writers should use the Latin extras (0xA0-0xDF) for typography.
- Line buffer 160 bytes after name expansion (the original 64), script copy 512, choice options 128 each; every copy is bounded (`english_copy`, `english_append`), and never splits a Shift-JIS pair.
- Speaker layout (owner decision, round 2): the speaker label starts at one left margin (x -0x80, where the Japanese names start), the opening mark (`"`, or `(` for a thought) follows the name after 6 px, and every row of the speech starts under the first character after the mark (`english_speaker_draw`). Continuation records (0xD, 0x12, 0x11) use the same column. The telephone does the same: `english_gnsx` draws the name at -0x80 and leaves the mark position for `english_sndi`, whose rows hang under the text (reset after each `sndisp`). Overlay labels drawn by hand (`english_speaker_label_x`) start at the same margin. A line that starts with `(` in narration still hangs its parenthesis.
- Matched quotes (owner decision, round 2): `english_speech_close` adds a closing `"` as the suffix of the last row of each speech record, each reply variant and each speech inline label, unless the next record of the group continues the speech (0xD, 0x12, 0x11) or the text already ends with a closing quote. Drawing it at run time keeps the files at their size; the wrapper measures each page with it (tools/patch/text/speech.py). Thoughts keep their runtime `)`.
- Inline labels have any length (0x1C/0x1D ... 0x1F). The sound-effect lines that the original matched by text (color 1 or 2, centered "And then...") are matched against the English translations of those literals (c:main:8007ef48), so the talk records must use the same wording.
- The all-punctuation flag `D_80122D1C`: 1 when every byte is a space, `.`, `,`, `"`, a curly quote or the ellipsis glyph. No reader of it was found.
- `sndi` expands every 0x10 and 0x15 (the original only the first `P` and `G`, and `G` only for style 15).
- Honorifics: the translators' choices. `get_p_name`: familiar "-kun", polite "-san" (SLPM_86.053:00072e34/38), nickname without one; form 15 is "Family Given". `get_g_name`: "Shiori", Yumi "-chan", Rei none, others family name "-san" (c:main:80051438). The translators also wrote "-kun"/"-san" inside the dialogue text (208 "-san", 92 "-kun", 42 "-chan").
- Long names. `name_code_girl_short` has 8 bytes and `girl_name_buf` 12; "Fujisaki", "Tatebayashi" and "Kisaragi-san" do not fit. The full form stays in an island buffer, the original buffer gets the bounded copy, and readers take the island copy only while the original buffer still holds exactly that bounded copy (an overlay that writes its own text there wins). `get_g_name` bounds its output to 12 bytes for every caller.

## Names and name entry (T-39506)

2026-11-02 verified implementation: `src/english/names.c` and `tools/patch/text/names.py` share immutable pair assignments. Never reorder/reuse codes: they persist in saves. Both preserve full Shift-JIS characters, even when a trail byte equals a token. The font generator rejects the reserved range. Text loading expands pairs to two glyph slots with their normal advances, so glyph counts/typewriter/cell mode remain correct. Editing unpacks, changes one displayed character and repacks; full-width kana still take two bytes. Byte-copy consumers and the save layouts remain unchanged.

- **Length.** Family/given fields still hold seven encoded bytes plus NUL, and nicknames eleven plus NUL, but pair encoding retains full eight/nine-letter presets. All 45 presets were decoded directly from the final disc's linked NAME_ENT table slots and compared to their full English targets (`build/preset-disc-audit.json`). Editor caps are 56 pixels for family/given and 72 for nickname, with label-aware x positions. Build wrapping reserves 56 pixels for each name, 116 for the full name and a conservative 88 for player form/nickname. Corrected entry/confirmation spacing is GPU-verified in `build/emu/route-en-name-confirm-final/027_name_entry_confirm.png`; `build/emu/route-en-name-save-card/019_full_name_load_summary.png` verifies full family/given names after loading a previous build's card, with adaptive summary rows. Editing/save/load route and focused confirmation trace report zero original-font calls, unknown mappings or overflow. These focused name checks passed; whole-game runtime coverage remains under T-39506.
- **Order.** Family name first everywhere, as the script: the name entry asks the family name first, `F` and `player_full_name` are "Family Given" (with a space; 15 bytes + NUL fit the 16-byte field).
- **Name entry** (an `#ifdef TOKIMEMO_ENGLISH` block at the end of `src/ovl/name_entry/name_entry.c`, functions `name_entry_english_*`, reached through NAME_ENT hooks on `name_entry_step_init`, the three `*_input`, `given_name_init`, `nickname_init`, `names_draw`, `frames_draw`, `profile_store`; round 1 ran it from the island). The block adds no initialised data to the object: the free relink carries addresses that other code takes inside `name_entry.o`'s `.data` (`name_entry_nickname_end`, 0x8014CC8E) only while that section keeps its size, so its tables are code and string literals:
  - a sixth tab, the English letter page, first in the tab column and selected when the screen opens and after each name part; the five original tabs move down (frames 0x1A apart: -0x30 letters, -0x16 hiragana, 4 katakana, 0x1E symbols, 0x38 kanji, 0x52 done). The kana, symbol and kanji pages work as before;
  - letter page grid (cells of `name_entry_cell_hit_test`, 16 px): columns 1-13 of rows 0-3 hold A-M, N-Z, a-m, n-z; row 4 holds `-`, `'`, `.` in columns 1-3 and a space key over columns 5-8. Letters are drawn in the Latin font centered in their cells (`english_text_cell`, a cell mode of `english_load_string`);
  - names are NUL-terminated byte strings; a letter is appended at the end (a kana from the original pages appends its two bytes, taken from the original `name_entry_character_put`), CROSS deletes the last character, START or the done tab ends the part (trailing spaces dropped; an empty part takes a preset as before). One insertion marker (actor 98) follows the last letter instead of one marker per slot;
  - tab column: frames 32 px wide from x -0x9F (the original's are 27 px from -0x9D) up to the page frame at -0x7F; the label actors 109-113 stay centred at x -0x8F, y = frame top + 8. A 56 px label does not fit beside the page (its grid starts at -0x78 and its right edge is at the screen edge), so the column takes **32 x 16** label art;
  - second texture page (asked in round 2, not done): the NAME_ENT headers (名前 / あだ名, panels 0, 1, 2, 14), wider selector labels and an ABC label need a second 4bpp page, because the one page of NAME_ENT.NBN has no free 8x8 cell. Needed: a two-entry texture table under the flag in place of `ovl_tpage_table = 0x801AAF58` (`name_entry_overlay_tables_set`), the second page uploaded with the first to free VRAM, frame records whose texture id names page 1, and frames 29/30 for the ABC pair. Even with room in VRAM, 56 px labels do not fit the screen beside the page grid (column -0x9F..-0x7F), so the selector labels stay 32 px; an ABC pair would go on actor 125 at x -0x8F, y -0x28;
  - tab selector: the letter tab's label is the text "ABC" (yellow when selected, because the highlight sprite would cover it). **Integration point** for the `name-label-abc` art (T-39508, `patch/art`, not merged): `name_entry_english_tab_label_draw`. What the art writer needs: one more sprite frame in NAME_ENT's frame table for a 32 x 16 "ABC" label (and, like the other tabs, its lit variant if it has one); the code then puts it on a free actor (125) at x -0x8F, y -0x28 (letter frame top -0x30 + 8), and `name_entry_english_tab_highlight_update` can move highlight actor 114 there too. The other five labels: frames 0xF, 0x11, 0x13, 0x15, 0x17 on actors 109-113 at y -0xE, 0xC, 0x26, 0x40, 0x5A;
  - the confirm screen draws the name buffers as they are, in the Latin font; `name_entry_profile_store` copies them bounded and builds the full name.
- **Places that draw the player name** (grep of the GameState name fields and `name_code_family`):
  - handled: every `gnsx` caller (about 200: telephone, career guidance and choice, final exam, scheduler, holiday, home screens, status, prologue), every script line through `talk_render`/`talk_speak_part`/`sndi` (dialogue, letters and endings that use the talk scripts), name entry and its confirm screen; plain left-aligned draws with room for 7 letters: fortune telling and computer fortune (family at -0x80, given at -0x48), club tournament labels, debug editors;
  - fixed in round 2 (under `TOKIMEMO_ENGLISH`): file select slot summary (`file_select.c`: the given name follows the family name at its pixel width + 4), speaker labels right-aligned by pixel width in `shiori_member.c`, `literature_speech_contest.c` (and its "Family Given" title, right-aligned at 0x60) and `new_year_dialogue.c` (`english_speaker_label_x`), Yumi's "%sさん" (`yumi_scene_handlers.c`, now family name + "-san", bounded; the translated literal had become "-san%s"), the space in the full name of `final_exam.c`, long names copied into `name_code_girl_short`/`girl_name_buf` (`miharu_scenes.c`, `graduation_scene_handlers.c`: `english_girl_short_set`, `english_girl_name_set`), the New Year fortune and wish into `name_code_o`/`name_code_w` (`english_value_set`). The RPG battle messages needed nothing: their literals are English ("Name: 12 damage taken", ASCII digits) and fit the 40-byte buffers;
  - fixed 2026-11-02: `date_ambush_prepare_battle` uses `english_girl_short_set`, retaining the full 11-letter label in its backing buffer and a bounded copy in the original eight-byte field. The English choice loader calls the canonical initializer; the relinker now honors selected input-section definitions over absolute side-script aliases. Controlled GPU replay verifies the complete label, intact neighboring name and English battle HUD with zero original-font uploads. See `build/emu/date-ambush-name` and [[tickets/T-39506-english-script-runtime]].

Emulator proof (2026-11-01): `tools/emu/run.sh route run --build en` on a test disc (English exe and NAME_ENT, and `CDROM/SERIFU/G_BIRTH.BIN` with table-1 g0000 l0000-l0002, Shiori's birthday, hand-encoded with `containers.build_containers` and `Image.write`, same file size) runs the whole route, 169 screenshots: the letter page opens by default, "Smith", "Tom", "Tom" are typed on it, the confirm screen shows them in the Latin font, the prologue (still Japanese) shows the English name labels, and the birthday page shows the girl name code, line breaks, the thought line in parentheses and the two-way choice in English.

## Open risks

- **Overlay space.** Solved by the free relink ([[relink]]): an overlay may grow into its zero tail, which memory watches showed unused ([[ram-map]]). TACO (291 bytes), DATE and BUNKASAI (5 KB) and RPG_BAT (12 KB) have little; the slot end 0x80162000 is a wall (the background still buffer).
- **Main room.** 27 KB in total (0x8012B540-0x80132000), 5 KB used. The `talk_render` rewrite and the width helpers fit; main-exe English strings (16 KB of original rodata, roughly the same in English) would not all fit next to code. Options: put them in the overlay tails per screen, or load a string bank from disc.
- **Gap use.** Write watches over 0x8012B540-0x80132000 in 8 more scenes (name entry to a date) saw no write ([[ram-map]]).
- **VRAM.** 384 glyph slots per screen. English uses about 2x the glyphs of the Japanese for the same text (letters plus spaces), so a full-screen list could run out; see glyph cache reuse above. Each glyph cell keeps its 16 x 16 sprite; English glyphs use at most 12 px of it.
- **Performance.** Each glyph upload is `LoadSquare` plus a `DrawSync` (`func_8009C674`), once per string layout, not per frame. About twice as many uploads when a box opens, still well under a frame for a 3-line box. Batching the syncs is possible if a long menu stalls.
- **Encoding collisions.** Any other code that scans strings for `q`, `n`, 0x20 or the name letters must move to the English control bytes. The list above comes from a grep for `== 'q'`-style compares; parsers that test the values through tables would be missed.
- **Disc and EDC.** The temporary test disc kept stale EDC/ECC; the emulator ignores them, but real hardware would not. Use T-39501's disc tool for anything beyond a smoke test.

Save/load summary (T-39506): the English-only `hud_english_save_name_lines` preserves complete packed family/given fields, inserts a space, and splits beyond the 124-pixel column. Long nicknames can use a separate label row. Detail rows follow at 14-pixel spacing. The original Japanese build retains its original implementation. Production-helper regression checks cover width, row spacing and full-copy behavior. Fresh-boot copied-card run `route-en-name-save-card` verifies album selection, full-name summary layout and reload with zero original-font uploads/unknown/overflow; capture 019 shows the complete names within the column.

## Primitive arena accounting (T-39515)

`GetWorkBase` in `src/main/gpu_primitives.c` records aligned byte usage. The original address calculation multiplies that byte count by four, leaving three quarters of the primitive arena unused between allocations. A fully populated English exam table (335 glyphs, 46 entries) exposed an overrun into `draw_buffer`, although the text-entry and glyph limits were satisfied. The English-only address calculation now adds the aligned byte count directly. Original build arithmetic is preserved. `tools/test_exam_english_layout.py` exercises both arena buffers with full glyph capacity and scene primitives; GPU regression acceptance is recorded in [[tickets/T-39515-exam-ranking-name-layout]].

## Expanded display rows and ending summaries (T-39517)

EPILOG uses the ending summary's x=-72..124 window, rather than generic narration. `english_talk_render` wraps non-choice rows after expanding names and values; explicit rows, suffixes and atomic characters remain intact. Choice rows remain one selectable row. Its block buffer is 1,024 bytes because the longest current EPILOG record is 544 bytes.

Ending summaries can exceed the entire 384-glyph atlas (the controlled Rei fixture reached 670 glyphs with the previous all-at-once setup). The English ending now captures expanded rows into a 4,096-byte/128-row queue in overlay memory through a temporary synchronous `english_text_capture` hook. The hook is cleared after construction. The scene loads only rows in its existing -80..80 viewport and reloads glyphs when that visible range changes; intervening scroll ticks update y positions. Queue/atlas overflow flags are sampled in GPU fixtures. The queue uses nonzero-initialized data because legacy overlay bss lies beyond the image slot. Final GPU fixtures and all-position queue checks pass: maximum 361 live glyphs, all rows within the right edge, and the complete long ending visibly scrolls through its closing sentence. Original 27 hashes remain matching; see [[tickets/T-39517-ending-summary-layout]].
