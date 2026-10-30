---
type: reference
updated: 2026-10-30
---

# English names and presenter English hooks (Phase 4 WP-D, WP-I)

Plan: [[phase4-design-2026-10-29]] (English, WP-D and WP-I). Text layer: [[port-text-layer]].

## WP-D: Symbols audit

The design asked whether NAME_ENT's Symbols page already has Latin letters. It does. Candidate list 38 is drawn by `name_entry_symbol_page_draw` (src/ovl/name_entry/name_entry.c:713). In the expanded original content pack (content SHA-256 `c54066fc...6f8243`), the list has 453 code units:

| Range | Units | Present |
|---|---|---|
| Full-width A-Z, 0x8260-0x8279 | 26 | all |
| Full-width a-z, 0x8281-0x829A | 26 | all |
| Full-width 0-9, 0x824F-0x8258 | 10 | all |

`tests/translation/run_name_entry_latin_tests.gd` checks this audit each time it runs. The design calls for an opt-in Latin page only if the Symbols page lacks these letters, so no port-drawn page was added. `flow/name_entry_latin_page.gd` keeps that file name, but it locates characters on the native Symbols page; it does not draw a page.

## WP-D: how names are stored

Field limits come from the source state machine (`flow/new_game_entry.gd`, `rules/new_game.gd`): family name 3 full-width units, given name 3, nickname 5. The design's statement that each field takes "8 bytes, 4 full-width" is wrong for the editable fields. The 8-byte slots at 0xd4 and 0xdc hold 3 units plus a NUL pair. The nickname slot at 0xe4 is 16 bytes.

- A Latin name is stored exactly as a player would enter it by hand on the Symbols page: big-endian full-width Shift-JIS pairs. `Rules._compact` removes blanks, and the result is padded with NUL bytes. The full name at 0xc4 is the family name followed by the given name.
- No ASCII is stored, there is no sidecar name field, and the save format is unchanged. `profile_*_bytes` round-trip through `services/save_service.gd` as integer arrays, and the tests compare them byte for byte.
- `NameEntryLatinPage.decode()` reads Latin units as ASCII. It is for port UI and logs only. A kana or kanji name decodes to "".
- The original cheat names are compared against raw buffers. A Latin name triggers a cheat only if its full-width bytes are equal to an original cheat name, which is the same behaviour as the original game.

## WP-D: typing

Typing is off unless `enabled` is set (English mode). Each typed character becomes the native actions a player would make: `tab` 2, a series of `scroll` ±1, then `cell`. The inserted bytes are always `entry.cell()` bytes, and none of these actions draws RNG. A full field drops further keys. Leaving the name step drops any queued input. Backspace is three native steps: `cross`, the native space unit `0x8140` (list 38, index 0), then `cross`. The original CROSS only moves the cursor back without erasing, so the space overwrite is what clears the character.

The typed characters are A-Z, a-z, 0-9 and `. , - ' ! ? &`. Each is accepted only if list 38 holds it. The apostrophe is entered as 0x8166 (U+2019).

Space is not typed. Commit (`Rules._compact`) drops every 0x8140 unit, so a typed space would use one of the 3/3/5 slots and vanish from the stored name ("Jo Ann" would be stored as "JoAn"). The Space key therefore keeps its pad meaning (confirm). `decode()` still reads 0x8140 as a space.

Enter and keypad Enter finish the field. `push_event` queues a finish, and `next_action` returns the native `{"kind": "start"}` after the text queued before it has been entered. This is the same action as the Tab (START) pad key, so it draws the one RNG value that `name_entry_name_finish` always draws, and an empty field still gets an original preset. Without this, Enter is the pad confirm (`pointer_confirm`), which presses the cell under the hand. With hook 4 the hand sits on the last typed letter, so "Al" followed by Enter would be stored as "All".

## Boot hook for root (WP-J; `app/new_game_entry_boot.gd`, not edited here)

1. On name-entry setup: `latin = NameEntryLatinPage.new(); latin.configure(entry_content)`. Then set `latin.enabled = <English preset on>`, and update it whenever the language changes.
2. At the top of `_input`, before the pad mapping: `if _name_input_active() and latin.push_event(event): get_viewport().set_input_as_handled(); return`. Key map while English is on and a name field is active:

   | Key | Effect |
   |---|---|
   | Letters, digits, `. , - ' ! ? &` | typed (so the pad letters Q/W/A/D/Z/C do not act there) |
   | Backspace | erase (cross, blank, cross) |
   | Enter, keypad Enter | finish the field: native START |
   | Space | pad confirm, unchanged (presses the cell under the hand) |
   | Arrows, Tab, Escape, F2, Ctrl/Alt/Meta chords, key repeats | pad mapping, unchanged |
3. In `_physics_process`, before the queue is popped: `if queue.is_empty(): var a := latin.next_action(controller); if not a.is_empty(): queue.append(a)`. This queues one action per tick.
4. Optional: `if a.kind == "cell": name_pointer.focus_cell(a.column, a.row)` puts the native hand on the typed cell. Space or a click then presses that cell again, as on the original pad; Enter does not, because hook 2 takes it as the finish.

With `enabled` false, `push_event` and `next_action` do nothing. `presentation/name_entry_image_presenter.gd` is unchanged.

## WP-I: presenter English hooks

`presentation/presenter_text_binding.gd` binds the exact original bytes that a presenter draws to a text-layer record. It accepts two kinds of binding row: `{key, source_hex, container, container_sha256, offset}` goes through `lookup_source`, and `{key, source_hex, record_id, container_sha256?}` goes through `lookup_record`. A row is rejected, and the original drawn, when:

- the source has a 0x20 row break, an odd byte count, or single-byte units;
- the record carries tokens;
- the font lacks a glyph;
- the container hash or source hash differs;
- the translated run is wider than units × the original advance.

Every rejection is listed in `fallback_log()`.

Each presenter gets `bind_english(layer, bindings, image_store = null) -> {text, images, fallbacks}` and `clear_english()`. Every `bind_english` call replaces the earlier binding completely: text is re-bound from scratch, and a call without a store returns HUD images bound by an earlier call to the originals.

| Presenter | Draw seam | Advance | Images |
|---|---|---|---|
| date_appointment, date_overlay | `_set_runs` -> compositor runs with `glyph_x` | 14 | HUD frames through `set_translated_images` |
| telephone_overlay | `_draw_text_hex_checked` wrapper, `_draw_text_hex_native` | 14 | HUD frames |
| calendar_phone | same wrapper | 14 | HUD frames |
| encounters_and_ending | overrides prologue_presenter's `_draw_text_hex_checked` | 14 | HUD frames |
| subgame_overlays | wrapper | 13 | none: open gap, reported in `fallbacks` |

A translated run is drawn as one native glyph call per unit, at its trimmed NativeLatin offset, so shadows, glyph recording and box checks stay native.

### WP-I status: hooks only, acceptance not met

The design's WP-I acceptance asks for text records covering DATE/DATE2/GEKO, TEL.EXN 738, RPG_BAT/TACO/TAIIKU and ENDING, and for sprite and BS keys covering EXAMINE, BATTLE VS_*, TAIIKU, LASTPHOT and the D_/I_ signage. This package ships only the bind/clear seams and their tests. WP-I is therefore not complete and must not move to Done. Recommended split for root (board and ticket numbers are root's):

1. WP-I hooks (this patch): bind/clear seams, strict fallback, triplet tests. Can close after review.
2. WP-I text rows: source-location-verified binding rows for DATE/DATE2/GEKO, TEL.EXN 738, RPG_BAT/TACO/TAIIKU and ENDING. Each row needs proof that the presenter draws from that container offset (decomp source or a traced read), not only byte identity. TACO/TT VRAM text either gets a draw seam or is recorded as a design-approved gap.
3. WP-I image keys: WP-B v2 PAGE/BS keys for EXAMINE, BATTLE VS_*, TAIIKU, LASTPHOT and D_/I_ signage. The subgame sprites first need an identity-keyed draw site (container, frame, palette, PNG hash). Otherwise each one is recorded as a design-approved gap.

Gaps and policy:

- No binding rows are shipped, so the live reach today is 0. The presenter strings carry no container or offset. A byte-identity scan against text-layer v3 found unique physical-record candidates: 10 for date_overlay, 55 for telephone, 19 for calendar_phone, 13 for subgame and 11 for encounters. Matching bytes do not prove that the presenter draws from that location; each candidate needs a source-location check before it becomes a binding.
- These stay original: VRAM and HUD-font subgame text (TACO and TT), the subgame sprites (BATTLE VS_*, TAIIKU, LASTPHOT, the ten 16x8 miss variants), the staff roll, and TEL rows with row breaks.
- These presenters pin HUD SHA `0a884e91...`. No manifest in `.asset-cache` has that hash. The tests open `port/hud-production-v1` with its own hash as the caller capability. Root should repin or regenerate the HUD.

## Tests (Docker, tokimemo-port)

- `tests/translation/run_name_entry_latin_tests.gd`: 206 checks. They include alt, meta and alt+Enter negatives, Space not typed, Enter and keypad Enter finishing without a duplicate letter, the nickname-full drop with no Symbols movement, a backspace on a full nickname that has to scroll back up, and the typed "Ann" actions and frame against a pinned base-snapshot hash (installed-runtime-v13).
- `tests/translation/run_date_phone_subgame_english_tests.gd`: 158 checks. It uses a user:// fixture pack. The fixture contains port-authored Latin only; it is not a translation. It runs a native, English and cleared triplet for every `_set_runs` site: the six date_appointment renders, the title banner, the DATE arrival greeting, three-choice, spot reaction, karaoke and coaster. It checks the reason for each row-break, single-byte and odd-length rejection, and it re-binds with and then without a stub image store on the calendar, appointment and GEKO presenters.
- Mutation check: each of the 11 `_set_runs` sites reverted alone, the row-break guard disabled, the image reset removed (3 presenters), the alt/meta guard removed, Space typed again, Enter not taken, and the field-full drop removed. Every one of these mutants fails a suite.
- An unbound cross-tree probe compared 14 date, phone, subgame and ending frames from the snapshot base against this patch. All 14 hashes were identical.
