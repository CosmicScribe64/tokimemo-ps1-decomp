---
id: T-39207
title: English dialogue and choice hooks on talk, textbox and VN presenters (Phase 4 English WP-G)
status: In Review
assignee: agent (english-text group)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-text-layer]]", "[[tickets/T-39201-english-native-text-flow]]"]
---

## Goal

Let the native talk player, dialogue textbox and VN dialogue draw English records from the generic text layer through the shared flow, leaving the native frames byte-identical whenever English is cleared or rejected.

## Acceptance criteria

- [x] `NativeTalkPlayer` passes each row's `talk:<path>:<section>:gNNNN:lNNNN` id, `source_sha256`, `name_codes`, and for reply commands 2/5 a `variant_index`, only when the caller passes its live `talk_choice` in that `present_line(context)` call. The player's own `choose()` is not used, because the global is also written outside talk menus (`scene_library.c` girl select: `talk_choice = girl_index`).
- [x] `DialogueTextboxPresenter.set_translated_text(layer)` and `VnScenePresenter.draw_vn_dialogue(..., translation)` look up `lookup_record(id, sha, font)` and draw through `native_text_flow`. Menus use the record's option variants by source index.
- [x] Triplet tests (native, English, cleared) and rejection cases in `port/godot/tests/translation/run_text_layer_dialogue.gd`. Cleared and rejected frames are byte-identical to native, and reveal is one code unit per tick.
- [x] Native frames match the base snapshot (scratch digest comparison of textbox lines, menus, VN bodies and the title badge).
- [x] Live reach reported: 0 non-test call sites today.
- [ ] Code review against `CODING_STANDARDS.md` with all findings resolved.
- [ ] Root: kanban card, index and log entries; WP-J binds the hooks.

## Notes

- Speaker labels stay Japanese (no source-proved dictionary/c id).
- Presenter boxes come from `NativeTextFlow.runtime_box`, the same formula as the corpus's `port_runtime` classes. The VN body reserves the closing quote cell, and a test covers that.
- WP-J must pass the live `talk_choice` global to `present_line({"talk_choice": n})` for reply rows. Otherwise English replies stay Japanese.
- Kanban: root adds this card to In Review together with the log entry when integrating (`wiki/kanban.md` is root-owned).
- Pre-existing: `DialogueTextboxPresenter.DEFAULT_HUD_SHA` and `VnScenePresenter.HUD_SHA` (`0a884e91...`) match no manifest in the current cache (`hud-production-v1` is `c19f4306...`). `open_font()` and `init_resources()` therefore fail without an explicit HUD open, and `tests/dialogue/run_dialogue_presentation_tests.gd` fails the same way on the base snapshot.

## Comments

- 2026-10-30: implemented in the phase4 english-text worktree, delivered as `.asset-cache/phase4-patches/english-text.patch`. Agent review is not human approval.
- 2026-10-30: review fixes (v2 patch): the reply variant comes only from the caller's `talk_choice`, with a test for a reply after a non-talk choice. The presenters use the shared runtime box, and a VN closing-quote-cell test was added. Not Done until a review of v2 resolves its findings.
