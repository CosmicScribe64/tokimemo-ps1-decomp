---
id: T-39201
title: Shared native text flow and English corpus fit runner (Phase 4 English WP-A)
status: In Review
assignee: agent (english-text group)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-text-layer]]", "[[tickets/T-39016-generic-text-layer]]", "[[tickets/T-39207-english-dialogue-hooks]]"]
---

## Goal

A single multi-row layout for English text-layer records in the original full-width glyphs, and a corpus report of how many records fit the original boxes, which overflow, and why.

## Acceptance criteria

- [x] `port/godot/presentation/native_text_flow.gd`: `layout(segments, box_class, font, name_codes)` returns rows of code units with x offsets from `NativeLatin.offsets` (`native_latin_layout.gd` unchanged), or `[]` on overflow or an undrawable token.
- [x] Unit tests (`port/godot/tests/translation/run_native_text_flow_tests.gd`, Docker Godot headless) cover marker rows, word re-wrap, overflow returning empty, P/G expansion with the 4xW worst case, and rejection of `unknown_renderer_bytes` and `inline_label`, plus the original-font checks.
- [x] Corpus runner (`port/godot/tests/translation/run_english_corpus_fit.gd`) uses the runtime path (`lookup_record` + `prepare`/`fit`) and classifies every record against every box class: 51,631 pack records, 0 crashes, all 55,911 inputs accounted for with `coverage.json`.
- [x] Per-screen fit % recorded in [[port-text-layer]].
- [ ] Code review against `CODING_STANDARDS.md` (section 12a applies to the port) with all findings resolved.
- [ ] Root: kanban card, `wiki/index.md`, `wiki/log.md` entry (root-owned files).

## Notes

- Box classes carry a `source` (see [[port-text-layer]]). The `port_runtime` classes (`textbox_line` 224, `textbox_line_label` 192, `textbox_choice` 200, `vn_body_speaker` 176, `vn_body_quoted` 190) come from `NativeTextFlow.runtime_box`, the same formula the presenters call, and the 4:3 headline screens measure them. The C call-site classes (`dialogue_4x3` 260, `dialogue_4x3_speaker` 204, `choice_cell` 206) are kept as `c_reference`. `dialogue_4x3` is 260, not the 264 the design names, because the C talk body starts at x 32. `phone_banner` is provisional.
- Output: `.asset-cache/port/english-fit-v2/fit.json` (schema v2, sha256 `55bcfb79...`), `readability-sample.png`, `readability-sample-3x.png`. The first run (`.asset-cache/port/english-fit/fit.json`, `c14e5b25...`) measured the C boxes and is superseded.
- Overflow reasons name the re-wrap limit: `rewrap_width:*` or `rewrap_rows:marker_rows_exceed_max`. Screen reasons ending in `:fits_reference_box` fit the C box and fail only at the port's narrower width.
- Readability decision: the variable-width font option was not triggered. Talk screens fit 99.12% to 99.50%, and all but 5 of the 4:3 overflows hit the 3-row limit. It is deferred per the design defaults until the human screenshot review in WP-K.
- Kanban: `wiki/kanban.md` is root-owned. Root adds this card to In Review together with the log entry when integrating. Until then, the status here has no card.

## Comments

- 2026-10-30: implemented in the phase4 english-text worktree, delivered as `.asset-cache/phase4-patches/english-text.patch`. Agent review is not human approval.
- 2026-10-30: review fixes (v2 patch, `.asset-cache/phase4-patches/english-text.v2.patch`): the corpus now measures the runtime boxes, defined once in `native_text_flow.gd`. Overflow reasons say whether width or row count failed, and the table in [[port-text-layer]] was re-run. Not Done until a review of v2 resolves its findings.
