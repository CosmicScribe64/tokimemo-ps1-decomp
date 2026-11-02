---
type: design
updated: 2026-11-01
sources: ["src/english/script.c", "src/english/name_entry.c", "include/english/script.h", "tools/english_imports.py", "src/main/text_system.c", "src/main/scene_library.c", "src/main/menu_speaker_girl.c", "src/english/", "tools/english_link.py", "tools/english_font.py", "translations/en/font/latin12.txt", "config/english_hooks.txt"]
---

# English text runtime

How the English build draws text: build variant, encoding, font, layout, and the work list for full English. Ticket: [[tickets/T-39502-english-text-runtime]] (part of [[tickets/T-39500-english-translation-patch]]). Addresses and routine roles come from [[ref-english-patch]].

The original's text limits are not limits for English. The game draws every character from a 12 x 12 Shift-JIS font at a fixed advance of 14 or 16 px, and its writers count 2 bytes per character. Because we build the code, the English build uses a proportional font, single-byte letters and pixel-width layout.

## Build variant

`ninja english` builds the English variant into `build/en/`. The default targets do not include it, and the default build still prints OK for all 27 binaries.

- **Flagged objects.** A C file that mentions `TOKIMEMO_ENGLISH` is compiled again with `-DTOKIMEMO_ENGLISH` into `build/en/<path>.o`. Every other object is reused from the default build. `configure.py` stops if a header outside `include/english/` mentions the flag, because only flagged `.c` files are rebuilt. Rerun `configure.py` after adding the flag to a new file.
- **Fixed addresses.** Every original address stays the same. Overlays call the main exe at absolute addresses from the symbol files, data in both holds raw code addresses (751 words of `0x8007C784` alone), and `ovl_load_enter` hard-codes the 26 overlay entry points. `tools/english_link.py ld` rewrites the default linker script so that every input section is placed (`. = offset`) at its address from the default map. A flagged object that shrinks is padded. One that grows fails the link with `cannot move location counter backwards`, on the line that names it.
- **Island.** New main-exe code and data go in the island. It is linked to run at `english_island_start` = end of the main bss (0x8012B540), with a limit of 0x80132000, the overlay slot (27,328 bytes; checked by a linker `ASSERT` and in the post-link step). In the file the island sits right after `.data`, so the BIOS loads it at 0x800E3800 and up (bss nothing has used yet). `src/english/boot.s` (`english_boot`) is the new header entry. It copies the island into place, clears the island bss, calls FlushCache (A0:44h) and jumps to `boot_start` (whose `bzero` ends at 0x8012B538, below the island). Current use: 4,352 bytes of code and data plus 768 bytes of bss, ending at 0x8012C940. Island symbols are passed to the overlay links (`build/en/island_syms.ld`), so overlay C can call `english_*` functions.
- **Hooks.** `config/english_hooks.txt` lists `<unit> <original> <replacement>`. The post-link step writes `j replacement; nop` over the original's first two instructions, so every caller, overlays included, reaches the replacement. The original body stays in place and never runs. Current hooks: `load_string`, `set_kanji_string` and `disp_string` (`src/english/text.c`); the script parsers and name functions (`src/english/script.c`, T-39506); nine NAME_ENT functions (`src/english/name_entry.c`, T-39506). A hooked original cannot be called any more, so a replacement repeats the whole function.
- **Island imports.** Island code that replaces overlay functions uses that overlay's symbols. `tools/english_imports.py` writes `build/en/island_imports.ld` with every symbol the island objects leave undefined that a default overlay ELF defines, at its default address, and the main English link reads it. This holds because the English overlays keep their default addresses; island code may use them only from a hook of that overlay (the overlay is then in the slot). Free relinking (T-39505) breaks this, because the English NAME_ENT no longer keeps the default addresses: then `src/english/name_entry.c` should move into NAME_ENT itself (an `#ifdef TOKIMEMO_ENGLISH` block or a new NAME_ENT object) and its hooks become plain functions there, and `tools/english_imports.py` with its `-T` in the main link goes away.
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
| inline label `>` `-` `+` (4, 6, 2 label bytes) | 0x1C, the label, 0x1F | `[[inline_label]]Teacher: ...`: the writer splits the label at the first `: ` |
| inline label `<` (4 label bytes, thought) | 0x1D, the label, 0x1F | as above |
| `nn` at the end (branch zero) | 0x0C 0x0C | `{n}{n}` |

Display strings need no control bytes. The constants are in `include/english/script.h` (T-39506). `sndisp` strings (TEL, prologue, holiday hints) are script strings too: 0x0A ends a line, 0x10 and 0x15 are the player and girl names. `F` is now the family name, a space and the given name.

## Font

- Source: `translations/en/font/latin12.txt`. This is our own drawing (committed), a text bitmap with one block per glyph (`glyph 0xNN name`, then 12 rows of `.`/`#`). There are 105 glyphs: ASCII 0x20-0x7E and 10 extras.
- Metrics: cap height 9 px (rows 1-9), x-height 6 (rows 4-9), descenders on rows 10-11. The advance is the ink width plus 1 px. Most letters are 5 px wide (advance 6), `i l . ! '` are 1 px, `M W m w` are 7 px.
- `tools/english_font.py` writes `build/en/gen/english_font.s`: `english_font_index[256]` (code to glyph, 0xFF none), `english_font_advance[]`, and `english_font_glyphs[]` in the game's 18-byte 12 x 12 1bpp format. It also writes `build/en/font_metrics.json` (advance per code, `sjis_advance` 14, `line_height` 16) for build-time wrapping.

## Rendering and layout

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

- `configure.py` English variant, `tools/english_link.py`, `tools/english_font.py`, `src/english/boot.s`, `src/english/text.c`, `include/english/text.h`, `config/english_hooks.txt`, `translations/en/font/latin12.txt`.
- First English screen: `file_select_memory_card_check_begin` (NAME_ENT, `src/ovl/name_entry/file_select.c`). Under the flag it shows two English lines in place of the memory-card check message. The literals fit the 44 bytes of the two original rodata strings, which the English build leaves out.
- Emulator proof (2026-11-01). The test disc was a copy of the original track 1 with the English exe and NAME_ENT written in place: a temporary script, not committed; T-39501's `tools/patch/disc.py` does this properly. Route: boot, then title, then Game Start, then the file select screen. The screenshot shows the English lines in the proportional font next to the original Japanese labels. Memory watches over the island and the free gap (0x8012B540-0x80132000) saw no writes from game code during the opening, the title and the file select. A watch on `english_glyph_pen` confirmed that the island code runs.

## Script parsers (T-39506)

`src/english/script.c` replaces, through hooks, `talk_render`, `talk_speak_part`, `make_three_select`, `talk_names_set` (src/main/scene_library.c) and `gnsx`, `sndisp`, `sndi`, `get_g_name`, `get_p_name` (src/main/menu_speaker_girl.c). They keep the originals' control flow, text entries (one per line, label and mark entries as before, so `check_set_k` counts and typewriter starts are unchanged) and voice handling, with these changes:

- One-byte scanning of the control bytes above; no byte parity. A Shift-JIS pair is still copied whole.
- Japanese left in the original format still works: a string with no byte in 0x01-0x1F and at least one Shift-JIS character is converted on entry (`english_script_from_original`, two-byte stepping as the original: 0x20, `q`, `n`, the name letters and the fixed-length inline labels become the English bytes). Without this the English build hung in the prologue, whose NAME_ENT strings are still Japanese (a 5-row page became one row and the scroll step never ended). An English string with a game-font Shift-JIS character and no control byte would be taken as original, so the writers should use the Latin extras (0xA0-0xDF) for typography.
- Line buffer 160 bytes after name expansion (the original 64), script copy 512, choice options 128 each; every copy is bounded (`english_copy`, `english_append`), and never splits a Shift-JIS pair.
- x from pixel widths: the speaker label is right-aligned at -0x58 (clamped at -0x9E), the opening mark (`"` or `(`, the translations of the original's brackets) right before the text column at -0x48; a line that starts with `(` hangs it left of the column; `gnsx` right-aligns at -0x58; `sndi` puts the row-0 mark left of the column in the same entry, so all rows start at -0x48 (the original started row 0 of style 0x2F 14 px further left).
- Inline labels have any length (0x1C/0x1D ... 0x1F). The sound-effect lines that the original matched by text (color 1 or 2, centered "And then...") are matched against the English translations of those literals (c:main:8007ef48), so the talk records must use the same wording.
- The all-punctuation flag `D_80122D1C`: 1 when every byte is a space, `.`, `,`, `"`, a curly quote or the ellipsis glyph. No reader of it was found.
- `sndi` expands every 0x10 and 0x15 (the original only the first `P` and `G`, and `G` only for style 15).
- Honorifics: the translators' choices. `get_p_name`: familiar "-kun", polite "-san" (SLPM_86.053:00072e34/38), nickname without one; form 15 is "Family Given". `get_g_name`: "Shiori", Yumi "-chan", Rei none, others family name "-san" (c:main:80051438). The translators also wrote "-kun"/"-san" inside the dialogue text (208 "-san", 92 "-kun", 42 "-chan").
- Long names. `name_code_girl_short` has 8 bytes and `girl_name_buf` 12; "Fujisaki", "Tatebayashi" and "Kisaragi-san" do not fit. The full form stays in an island buffer, the original buffer gets the bounded copy, and readers take the island copy only while the original buffer still holds exactly that bounded copy (an overlay that writes its own text there wins). `get_g_name` bounds its output to 12 bytes for every caller.

## Names and name entry (T-39506)

- **Length.** The family and given name fields are 8 bytes (GameState 0xD4, 0xDC, saved as they are), and every reader uses `strcpy`/`strcat` into 8-byte buffers (`name_code_family`, `name_code_given`), so a name keeps its NUL: **7 letters at most** (the original allows 3 kana, 6 bytes). The nickname goes to `name_code_player_form` (12 bytes): 11 letters. Family name + "-kun" is 11 bytes and fits. The save layout is unchanged.
- **Order.** Family name first everywhere, as the script: the name entry asks the family name first, `F` and `player_full_name` are "Family Given" (with a space; 15 bytes + NUL fit the 16-byte field).
- **Name entry** (`src/english/name_entry.c`, hooks on NAME_ENT `name_entry_step_init`, the three `*_input`, `given_name_init`, `nickname_init`, `names_draw`, `frames_draw`, `profile_store`):
  - a sixth tab, the English letter page, first in the tab column and selected when the screen opens and after each name part; the five original tabs move down (frames 0x1A apart: -0x30 letters, -0x16 hiragana, 4 katakana, 0x1E symbols, 0x38 kanji, 0x52 done). The kana, symbol and kanji pages work as before;
  - letter page grid (cells of `name_entry_cell_hit_test`, 16 px): columns 1-13 of rows 0-3 hold A-M, N-Z, a-m, n-z; row 4 holds `-`, `'`, `.` in columns 1-3 and a space key over columns 5-8. Letters are drawn in the Latin font centered in their cells (`english_text_cell`, a cell mode of `english_load_string`);
  - names are NUL-terminated byte strings; a letter is appended at the end (a kana from the original pages appends its two bytes, taken from the original `name_entry_character_put`), CROSS deletes the last character, START or the done tab ends the part (trailing spaces dropped; an empty part takes a preset as before). One insertion marker (actor 98) follows the last letter instead of one marker per slot;
  - tab selector: the letter tab's label is the text "ABC" (yellow when selected, because the highlight sprite would cover it). **Integration point** for the `name-label-abc` art (art branch, not merged): `english_tab_label_draw`; draw it as a label actor like actors 109-113 and let `english_name_entry_tab_highlight_update` highlight it like the others;
  - the confirm screen draws the name buffers as they are, in the Latin font; `name_entry_profile_store` copies them bounded and builds the full name.
- **Places that draw the player name** (grep of the GameState name fields and `name_code_family`):
  - handled: every `gnsx` caller (about 200: telephone, career guidance and choice, final exam, scheduler, holiday, home screens, status, prologue), every script line through `talk_render`/`talk_speak_part`/`sndi` (dialogue, letters and endings that use the talk scripts), name entry and its confirm screen; plain left-aligned draws with room for 7 letters: fortune telling and computer fortune (family at -0x80, given at -0x48), club tournament labels, debug editors;
  - still to do (each needs an overlay change, so the free relink of T-39505 or hooks): file select slot summary (`file_select.c` 121-130: family at 0x20, given at 0x4E, 46 px apart, so 7 wide letters overlap), centering by `strlen * 4` in `shiori_member.c:882`, `literature_speech_contest.c:564-610` and `new_year_dialogue.c:287`, the Japanese suffix in `yumi_scene_handlers.c:49` (`"%sさん"`, a C literal for T-39507), the full name without a space in `final_exam.c:708`, the RPG battle labels and messages (`rpg_label_create`, `rpg_message_format_damage` with Japanese particles), and overlays that `strcpy` English names longer than 7 bytes into `name_code_girl_short` (e.g. "Mysterious woman" in `miharu_scenes.c`) or `girl_name_buf`.

Emulator proof (2026-11-01): `tools/emu/run.sh route run --build en` on a test disc (English exe and NAME_ENT, and `CDROM/SERIFU/G_BIRTH.BIN` with table-1 g0000 l0000-l0002, Shiori's birthday, hand-encoded with `containers.build_containers` and `Image.write`, same file size) runs the whole route, 169 screenshots: the letter page opens by default, "Smith", "Tom", "Tom" are typed on it, the confirm screen shows them in the Latin font, the prologue (still Japanese) shows the English name labels, and the birthday page shows the girl name code, line breaks, the thought line in parentheses and the two-way choice in English.

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
