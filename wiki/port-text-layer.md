---
type: reference
updated: 2026-10-29
---

# Port text layer (source-keyed English text)

[[tickets/T-39016-generic-text-layer]]. The text layer swaps English text in for an original string at the moment a screen decodes that string. It looks the string up by its original source location. If anything fails to match, the screen keeps the original bytes. The existing prologue dialogue pack (`services/text_translation_store.gd`, `tokimemo.text-translation-pack.v1`) is separate and does not change.

## Pieces

- `tools/port/build_text_pack.py` builds the pack in Docker from `translations/en/returned-v3.jsonl` and `translations/en/supplemental.jsonl`. It reads the `text_approval` block in `translations/en/graphics/inventory_decisions.json`, the original files under `disc/files` and the original font. The font comes from the canonical `tools/assets/text_font.py` reader. The builder only reads its inputs, and the output directory must be new. It writes `text_layer.json` (schema `tokimemo.text-layer-pack.v1`) and `coverage.json`.
- `port/godot/services/text_layer_store.gd` opens the pack with a pinned SHA-256, checks every entry and provides the lookups.
- `port/godot/presentation/native_latin_layout.gd` places a translated single-row run with proportional spacing. It uses the same metric as the prologue translation renderer.
- `services/translation_pack_service.gd` has `select_text_layer`. `services/settings_runtime.gd` has `select_ui_text_pack` and `translated_text`. The selection persists in the preferences as `language.ui_text_manifest` and `language.ui_text_sha256`. Both keys are optional, so settings saved before T-39016 still load.

## Keys

- Physical ids are `<container>:<offset:08x>:<sha12>`, where `sha12` is SHA-256 over the source bytes through the first NUL (the scheme of `tools/assets/text_binary.py`). The builder rereads those bytes from the original container and rejects any mismatch. Ids that `tools/assets/text_source.py` derived from an assembler extent without a NUL, or from a NUL-appended alias, do not hash this way and are rejected as `source_bytes_mismatch`. At runtime, `lookup_source(container, container_sha256, offset, text_bytes, font)` checks the id, offset, full source hash, length and container hash. The id binds only the first 12 hex digits, so the full-hash and length guard is tested on its own.
- Logical ids (`talk:`, `c:`, `dictionary:`, `speaktst-layout:`, `supplemental:` and others) go through `lookup_record(id, container_sha256, font)`. When the id names an original container, the pack records that container's SHA-256, and the caller must present the same hash.
- Main-executable offsets come from data-label addresses: `offset = address - 0x80041000 + 0x800`. The load address and header size are from `FONT_PROFILE` in `tools/assets/text_font.py`.

## Glyphs

The builder maps each English character to the original font's full-width Latin or punctuation code unit. A record is representable only if every character resolves through the original font range table to a non-blank 12x12 bitmap. The full-width space 0x8140 is the one blank glyph accepted. At runtime, every glyph is checked again against the `NativeGlyphs` instance the screen opened. Protected markers are restored to the source bytes recorded for them and returned as `token` segments. A record with tokens has no flat `codes`, so the screen has to handle the tokens itself.

Three mappings are deliberate substitutions. JIS X 0208 has no straight quotes and no en dash; CP932 places the full-width straight quotes only in the NEC extension, 0xFA56/0xFA57. So ASCII '"' becomes the right double quote 0x8168, both opening and closing; ASCII "'" becomes the right single quote 0x8166; and the en dash becomes the minus sign 0x817C, which is also CP932's full-width hyphen-minus. `coverage.json` counts each substitution.

## Renderer controls

The original renderers give single bytes their own meaning. In `sndisp` and `sndi` (`src/main/menu_speaker_girl.c`), ' ' at even byte phase breaks the line, and 'P' and 'G' expand names. Format strings carry `%s` and `%d`. A full-width glyph run cannot stand in for these bytes, so the builder rejects:

- a physical source whose Shift-JIS walk finds a single-byte unit outside the record's protected tokens (`source_single_byte_control_untokenized`), or a token whose bytes do not occur in the source at its recorded offset (`protected_token_not_in_source`);
- English with a printf directive outside a marker (`format_directive_untokenized`);
- a record whose review status says `renderer_tokenization_unavailable`. The exception is an id that the builder's `RENDERER_PROVEN` table cites from the source, and the other checks still run on it. The table holds only `D_800AFB74`, the "(" decoration that `sndi` adds for speaker style 0x1F and that the command HUD narration draws.

Logical ids carry no source bytes, so the walk cannot run on them. They rely on their recorded tokens and the printf check.

## Coverage (pack v3 `2f2db836...`, 2026-10-29 inputs)

| | Records |
| --- | ---: |
| English records read | 55,911 |
| Representable with original glyphs | 51,631 (92.3%) |
| ...physical ids re-verified against original bytes | 1,087 |
| ...bound to an original container hash | 49,725 |
| Rejected: renderer tokenization unavailable | 2,430 |
| Rejected: ASCII/format literal (not glyph text) | 1,018 |
| Rejected: no English text | 617 |
| Rejected: supplemental marker without source bytes | 190 |
| Rejected: untokenized single-byte source control | 13 |
| Rejected: markers only, no glyph text | 9 |
| Rejected: printf directive outside a marker | 3 |
| Rejected: character without an original glyph | 0 |

Substitutions in representable records: 239 straight double quotes and 28,064 apostrophes. Pack v2 (`c6fed345...`, 54,059 representable) held 800 physical entries with untokenized single-byte controls; v3 replaces it.

"Representable" does not mean the record is placed. The pack does not prove where a record is drawn or that it fits a screen; each consumer has to prove both. Every entry carries the record's original `review_status` as `context_status`.

## How a screen adopts the layer

1. Find where the screen reads the original string bytes, and the source identity of those bytes: container path, file offset and container hash from the screen's own verified manifest.
2. Call `layer.lookup_source(...)` there, or `lookup_record(...)` for a logical id, and pass the screen's open `NativeGlyphs`. If the result is empty, draw the original bytes. `layer.last_rejection` says why the lookup failed.
3. Use `result.codes` for single-row glyph text. If the result has `segments` with tokens, handle each token the way the original renderer treats those bytes, or reject the record.
4. Lay the text out inside the original extent with `NativeLatin.offsets(font, codes, max_width)`. If it does not fit, keep the original.
5. Add a setter like `set_translated_text(layer)` that binds or clears the translation, call it next to `set_translated_images`, and test three things: the binding works, the original frame comes back exactly after clearing, and a rejected layer leaves the original frame.

The first consumer is the command HUD narration (`presentation/command_hud_presenter.gd`, `set_translated_text`). It looks up the executable strings `D_800AFB74` and `D_800AFF24` (the `sndisp` call in `settings_command_screen_build`, `src/main/home_screens.c`) by their executable location. It binds both strings or neither, and the translated row has to fit inside the cells of the original 14-cell row. `app/playable_boot.gd` binds the HUD to `ModernSettings.translated_text`.

## Tests

- `tools/port/test_build_text_pack.py`: synthetic builder contract (11 tests), including sources with 'P', ' ' and '%s' at even phase.
- `port/godot/tests/translation/run_text_layer_tests.gd`: synthetic store, layout, service and preference checks, including negative cases and entries that share the id hash prefix but differ in full hash or length.
- `port/godot/tests/translation/run_text_layer_hud.gd`: original HUD content and the built pack. Set `TEXT_LAYER_PACK`/`TEXT_LAYER_SHA256` and `HUD_MANIFEST`/`HUD_SHA256`/`HUD_FONT_ROOT`.

Limits: the HUD test uses caller-supplied rule and context state, as `tests/hud/run_fresh_content_tests.gd` does. It is not an emulator capture or a played route, and English layout has no emulator reference. Opening the 24.5 MB pack takes about 1.2 to 2.3 s on the arm64 Docker image.

## Multi-row flow and corpus fit (Phase 4 English WP-A, [[tickets/T-39201-english-native-text-flow]])

`port/godot/presentation/native_text_flow.gd` lays a text-layer record out in rows of original full-width code units. The x offsets come from `NativeLatin.offsets`, and `native_latin_layout.gd` is unchanged. `layout(segments, box_class, font, name_codes)` returns `[{codes, x, width}]` per row, or `[]` when the record does not fit or carries a token the flow cannot draw. `classify` returns the same result plus the outcome and its reason. The fit policy is the Phase 4 default: the translator's row markers first, then a word re-wrap that keeps the marker breaks, then a re-wrap that joins all rows. Anything still too big overflows, and the caller keeps the original. `' '`, `row_separator`, `line_feed` and a terminal `' '` break rows. Name tokens (`P`, `G`, `F`, ... from `NativeTextTokens.NAME_CODES`) expand from caller-supplied full-width code units before measuring. `q` separators need `split_variants()` first. Every other token rejects the record: `unknown_renderer_bytes`, `inline_label`, `format_directive`, `conditional_*`, `reply_skip_marker` and `unknown`.

Box classes (native x = 160 + the C argument). Each class has a `source`. The `port_runtime` classes are the boxes the English hooks actually fit against. `NativeTextFlow.runtime_box(body_x, right_x, reserve_close_cell, rows)` is the one formula: the presenters call it with their own body origin, and the runtime classes apply it to the geometry constants in `native_text_flow.gd`. `run_native_text_flow_tests.gd` and `run_text_layer_dialogue.gd` check that the two agree.

| Class | Source | Width x rows | Origin and evidence |
| --- | --- | --- | --- |
| `textbox_line` | port_runtime | 224 x 3 | `DialogueTextboxPresenter.render_dialogue_line`: body x 54 (40 + opening bracket) to `TEXTBOX_INNER` x 292, less the closing bracket cell |
| `textbox_line_label` | port_runtime | 192 x 3 | the same after the shiori/yoshio label: body x 86 |
| `textbox_choice` | port_runtime | 200 x 1 | `render_choice_menu`: body x 60 to the highlight end x 274, less the closing bracket cell |
| `vn_body_speaker` | port_runtime | 176 x 3 | `VnScenePresenter.draw_vn_dialogue` with a speaker and an opening quote: body x 102 |
| `vn_body_quoted` | port_runtime | 190 x 3 | `draw_vn_dialogue` without a speaker, opening quote at x 74: body x 88 |
| `dialogue_4x3` | c_reference | 260 x 3 | `talk_render(-0x80)` in `talk_speak_part` (x 32) to the window's inner edge (x 292) |
| `dialogue_4x3_speaker` | c_reference | 204 x 3 | `talk_render(-0x48)` after the speaker label and bracket (x 88) |
| `choice_cell` | c_reference | 206 x 1 | `talk_render(-0x72, ..., "』", "『", 1)`: body from x 60 to the cursor at x 280, less the closing bracket |
| `dialogue_wide` / `_speaker` | design | 380 / 324 x 3 | opt-in widescreen box (T-30001 Class B) |
| `hud_14_cell` | port_runtime | 196 x 1 | the command HUD narration's 14 cells |
| `phone_banner` | provisional | 256 x 2 | the port's TEL gradient box `Rect2i(28, 23, 264, 42)`, not yet tied to a C call site |

The design text names `dialogue_4x3` as 264x3. The C talk body never starts at the inner edge (x 28). The port presenters start later still (x 54 and x 86, against the C x 32 and x 88) and reserve the closing bracket cell, so their boxes are narrower than the C references. The C reference classes are kept so the gap can be measured. No presenter draws in them today.

`port/godot/tests/translation/run_english_corpus_fit.gd` sends every record of a built pack through `lookup_record` and then the same `prepare`/`fit` calls that `layout` makes. Its 4:3 headline screens use the `port_runtime` classes. Each screen also names its C reference class, and an overflow that would fit that reference box is counted as `overflow_fits_reference`. Names are the worst case: 4 full-width W per 8-byte field, and 8 for `F`. Run on pack v3 (`2f2db836...`) with the original font, it classified all 51,631 pack records in 266 s with 0 crashes. Together with the builder's 4,280 rejections from `coverage.json`, that covers all 55,911 input records. The output is `.asset-cache/port/english-fit-v2/fit.json` (schema `tokimemo.english-fit.v2`, `55bcfb79...`, ids and counts only) plus `readability-sample.png`. The first run (`.asset-cache/port/english-fit/fit.json`, `c14e5b25...`) measured the C reference boxes and overstated the 4:3 fit, so it is superseded.

| Screen (box, records measured) | Fit | Markers | Re-wrap | Overflow | of which fit the C box | Rejected token |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| talk, labelled textbox (`textbox_line_label` 192, 29,418) | 99.20% | 28,884 | 300 | 89 | 15 | 145 |
| talk, unlabelled textbox (`textbox_line` 224, 29,418) | 99.33% | 28,891 | 331 | 51 | 31 | 145 |
| talk, VN with speaker (`vn_body_speaker` 176, 29,418) | 99.12% | 28,837 | 321 | 115 | 41 | 145 |
| talk, VN quoted (`vn_body_quoted` 190, 29,418) | 99.20% | 28,884 | 299 | 90 | 70 | 145 |
| talk, speaker wide (`dialogue_wide_speaker` 324, 29,418) | 99.49% | 28,891 | 377 | 5 | n/a | 145 |
| talk, narration wide (`dialogue_wide` 380, 29,418) | 99.50% | 28,891 | 379 | 3 | n/a | 145 |
| talk choices, per option (`textbox_choice` 200, 1,129) | 99.38% | 1,122 | 0 | 7 | 0 | 0 |
| speaktst-layout (`textbox_line` 224, 15,363) | 99.40% | 15,031 | 240 | 7 | 3 | 85 |

Every talk record is measured in every talk box, so each row is the fit if all lines used that box. Only shiori/yoshio lines (commands 10/11) draw a textbox label. The labelled row is therefore the worst case for the textbox, not its typical case.

Overflow reasons name the limit the last re-wrap hit at that width. `rewrap_width:word_wider_than_box` means one word is wider than the box. `rewrap_width:rows_exceed_max` means the translator's rows fit the row count but wrapping them at this width needs more rows. `rewrap_rows:marker_rows_exceed_max` means the translator wrote more rows than the box holds and joining them at this width still needs more than 3. The result's `limit` field is `width` or `rows`. A screen reason ending in `:fits_reference_box` is an overflow that the wider C reference box would fit, so it fails only because of the port's narrower body.

Why records overflow or are rejected:

- Labelled textbox: 74 of the 89 overflows fail in the C box too. They have more than 3 marker rows and still need more than 3 rows after joining, and in these lines the original scrolls with `check_k_scroll`. The other 15 also have more than 3 marker rows, but they join into 3 rows at the C width (204) and not at 192. Unlabelled textbox: 20 of 51 fail in the C box too, and 31 fail only at 224. Over the 89 labelled-box overflows, 64 are in `EPILOG.BIN`, 6 in `ENDING.BIN`, 5 in `G_APPEAR.BIN` and 14 elsewhere. With the 3-row rule they stay Japanese. Allowing as many rows as the original and scrolling them is a later opt-in (the design's continuation-page clause). Moving the port's textbox body origin to the C origin would recover the width-only overflows, but that changes native drawing and is outside this package.
- The VN with-speaker box has 5 `rewrap_width:rows_exceed_max` overflows that fit the C box: their marker rows fit 3, but at 176 px the re-wrap needs 4.
- The 145 talk rejections are 127 `inline_label`, 7 `conditional_choice_zero`, 7 `reply_skip_marker` and 4 `unknown`. All 557 `overlay-table` records carry `unknown_renderer_bytes`.
- The 4 choice overflows that are not marker-driven need 2 rows in a 1-row cell.
- Only talk records have a proven draw site. For the other families, `fit.json` lists outcomes per box class without picking a screen. The 14-cell HUD row fits 984 of 1,087 physical records.

Readability: in English mode, a code unit averages 6.09 px against the original 14. Original lowercase letters advance 3 to 9 px with 6 to 10 px of ink, capitals 3 to 11 px, and the space advances 4 px. The sample renders are legible at 1x and 3x. This is an agent judgment, not a human review. Overflow is rare (at most 115 of 29,418 talk records, 0.39%, in the narrowest runtime box). All but 5 of the 4:3 overflows hit the 3-row limit. Part of each runtime-box overflow comes from the port's body origins being narrower than the C ones, not from glyph width. So the variable-width "Modern text" font option was not triggered and stays deferred, as in the design defaults. The WP-K screenshot review (human) can still trigger it.

## Dialogue hooks (Phase 4 English WP-G, [[tickets/T-39207-english-dialogue-hooks]])

- `dialogue/talk_player.gd`: `present_line` and `present_choice` views carry `source_sha256` (the trusted SERIFU hash) and `name_codes` (`name_codes_for(context)`). For commands 2 and 5 a line also carries `variant_index`, but only when the caller passes its live `talk_choice` as an int of 0 or more in that call's `present_line(context)`. `talk_speak_part` case 0x2/0x5 (`src/main/scene_library.c`) draws the variant chosen by its `choice` argument, which `talk_run` callers pass as the global `talk_choice` (`normal_date_speak`, `normal_date_speak_1line`) or as 0xFF (`normal_date_speak_012`). That global is also written outside talk menus, for example `talk_choice = girl_index` in the girl-select menu. So the player's own last `choose()` is not proof of the variant, and it is not used. Without the caller's value the view has no index and the reply stays Japanese. An out-of-range value such as 0xFF also stays Japanese. WP-J must pass the live `talk_choice`.
- `presentation/dialogue_textbox_presenter.gd`: `set_translated_text(layer)` binds or clears the layer. A line draws English only when its `talk:` id resolves with the source hash and the whole record fits the `NativeTextFlow.runtime_box` from the presenter's own body origin to x 292, minus the closing bracket cell, in 3 rows (`textbox_line`/`textbox_line_label`; menus `textbox_choice`). `last_text_box` records the box used. One code unit is revealed per tick. Menus use the record's `q` variants by `source_index`. The variant count has to equal the option count and every option must fit in one row, or the whole menu stays Japanese. `text_rejection` and `last_text_source` report the outcome.
- `presentation/vn_scene_presenter.gd`: `draw_vn_dialogue(..., translation = {})` takes `record_id`, `source_sha256`, `name_codes` and `variant_index`. It keeps the original speaker label and any original opening or closing quote glyph, and reserves the closing quote's cell in its `runtime_box` (`vn_body_speaker`/`vn_body_quoted`; `last_dialogue_box`). The per-glyph drawing moved into `_draw_glyph_checked`, and the native frames are unchanged (see the tests below). `set_translated_text` also binds the inherited command-HUD narration.
- Speaker labels stay Japanese, because no presenter has a source-proved `dictionary:`/`c:` id for them.
- Live reach today is 0 call sites: outside tests, nothing calls `NativeTalkPlayer`, `DialogueTextboxPresenter`, `render_dialogue_line`/`render_choice_menu` or `draw_vn_dialogue`. The hooks start to matter once the core port drives SERIFU talk through these classes and WP-J binds them.
- Tests: `tests/translation/run_text_layer_dialogue.gd` (synthetic records, original font and HUD actors) checks native, English and cleared frames for lines, reply variants, menus and the VN body. It also checks rejections (container hash, missing id, overflow, unknown token, missing name, no id, option-count mismatch, unopened layer) and reveal counts. Further cases: a reply after a non-talk choice stays Japanese, a VN body that fits only without the closing-quote cell falls back to native with the quote inside `Rect2i(27, 166, 266, 52)`, and each presenter's box equals its runtime class. A scratch digest script showed that native textbox, menu, VN and title-badge frames match the base snapshot byte for byte.
- `DialogueTextboxPresenter.DEFAULT_HUD_SHA` (`0a884e91...`) matches no manifest in the current `.asset-cache`, while `hud-production-v1` is `c19f4306...`. So `open_font()` fails unless a HUD manifest is opened first, and `tests/dialogue/run_dialogue_presentation_tests.gd` fails the same way on the base snapshot. The new test opens the HUD manifest explicitly.
