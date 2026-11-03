---
id: T-39538
title: Restore Yumi's name in mistranslated self-references
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39537-dialogue-break-review]]"]
---

## Goal

Correct confirmed name mistranslations while preserving Yumi's third-person self-reference, reply choices and source meaning.

## Acceptance criteria

- [x] Audit records containing Yumi's Japanese name against the English targets.
- [x] Apply only source-confirmed corrections, preserving protected controls.
- [x] Verify changed payloads, actual-disc readback and layout.
- [x] Inspect affected runtime replies and review against CODING_STANDARDS.md.

## Corrections

The break review exposed malformed self-references in DATE_SA. Audited 446 exported records containing Yumi's Japanese name and reviewed the 55 active-game targets containing `my`. Source comparison confirmed 39 errors. Valid possessives and first-person translations were retained; unused SPEAKTST copies are outside the patch inputs.

`translations/en/reviewed-targets.json` contains the corrections. The loader checks the source and previous-target SHA-256, then requires identical protected markers in the same order. Raw sources and exported translation records remain immutable.

Whole-corpus comparison proves exactly 39 changed payloads, unchanged unrelated records and reply counts, fitting rows, and no added scrolling. Sixty focused tests pass with one optional disc test skipped. Coverage includes source/target drift, missing or reordered controls, input-row preservation and unlisted records. Evidence is in `build/yumi-repair-proof.json` and `build/yumi-repair-tests.log`.

## Build and disc verification

The build completed in 165.8 seconds and is frozen at `build/yumi-reviewed-release`, track SHA-1 `7738fcbe89bee49102c68b025a5f5104ccf6b09f`. The patch is 6,564,441 bytes. Art checks verified 252 placements with no unmapped characters.

Full-disc readback verified 105 containers, 31,471 strings and 31,334 pointers, with zero original-font bytes. The executable audit reports 2,828 exact slots, 291 derived records, 286 pointers and zero pending slots.

## Runtime review

Six receipts under `translations/en/reviews/` cover exactly all 39 corrected records on that frozen track:

- `yumi-date-replies-2026-11-02.json`: 31 changed reply variants across 25 DATE_SA records, inspected on four native-resolution contact sheets.
- `yumi-festival-invitation-2026-11-02.json`: one SA_S6S record with Yumi's actual portrait and disc-loaded invitation tables.
- `yumi-festival-walk-2026-11-02.json`: three SA_S6W records with the actual festival walk tables and portrait.
- `yumi-festival-visit-2026-11-02.json`: two SA_S6 records with the actual visit tables and portrait.
- `yumi-event-2026-11-02.json`: three EVENT_SA records through mode 0x5E's scene talk runner; the controlled replay retains the date backdrop and portrait.
- `yumi-new-year-2026-11-02.json`: five SHOGATU records through the seasonal talk runner; portrait staging is outside this controlled replay.

All inspected replies have complete quotes, fitting rows and no overlapping or clipped glyphs. Table addresses, group/line progression and complete text states were checked. The date collector allows another 120 frames after reveal for the runtime closing quote. `talk_run` advances the line on dispatch, so collectors check the selected line plus one.

These are controlled selections in the actual renderers. They do not establish natural route traversal or whole-game art quality. Broader visual review and publication remain open in [[tickets/T-39511-english-text-quality]].

## Fixture findings

Failed attempts are excluded from approval. The first invitation attempt selected generic dialogue. Mode 0x52 enters Ijuin's scene; `src/main/overlay_loader.c` confirms 0x53 enters the New Year handler. New Year globals move in the English link, so its successful probe reads the current SHOUGATU ELF symbols.

The first festival visit retry paused at frame 65 with an unknown byte read after a forced intermediate dialogue transition. Entering the script-load step directly completed successfully with the expected SA_S6 triad `801ED184/801ED664/801F0F78`. No game code changed for these fixture repairs. Detailed runs and diagnostics stay under `build/`.

## Code review

Reviewed the correction loader, layout integration, manifest and tests against CODING_STANDARDS.md. Hash checks prevent applying repairs to changed inputs; marker checks preserve controls; input rows are copied. Whole-corpus comparison and full-disc audits support the stated scope. No default C binary changes, generated assembly edits, host dependencies or raw-source modifications were introduced. There are no unresolved code findings.
