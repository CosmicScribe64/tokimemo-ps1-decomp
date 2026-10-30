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
