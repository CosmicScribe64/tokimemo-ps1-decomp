---
id: T-9550
title: "Wave 8: list 5"
status: Done
assignee: w8-5
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Work through wave8-list-5.txt in order, keeping byte-matched C and reverting time-boxed drafts.

## Acceptance criteria

- [x] Each listed function attempted or time-boxed with evidence.
- [x] Final Docker ninja passes all 27 checksums and header/global checks.
- [x] Branch prototype check passes; inline standards review recorded.

## Notes

Follow the shared wave-8 brief. Changes stay uncommitted for the orchestrator.

## Comments

- 2026-10-11: Claimed list 5; read wiki index, workflow, coding standards, shared brief and register-allocation findings.
- 2026-10-11: Baseline main and completed overlays pass SHA1; prepared list-order drafts and explicit array forms for DATE's halfword rows. Scratch scripts and draft evidence live only in scratchpad/w8-5/.
- 2026-10-11: DATE func_801572E4 is within a moved load pair after struct-copy, narrow-index-loop and byte-chain rewrites. Prepared a common D_8011ECD0 view for its two flag updates to keep the second load after the first store; retest pending after the active batch.
- 2026-10-11: Function diff MATCH for `DATE:func_801572E4` (268 bytes).
- 2026-10-11: Function diff MATCH for `TAIIKU:func_80143D24` (172 bytes).
- 2026-10-11: Function diff MATCH for `main:cal_sprite_init` (828 bytes).
- 2026-10-11: Function diff MATCH for `main:initLight` (272 bytes).
- 2026-10-11: Docker completeness audit accounts for all 152/152 listed functions. Four matches currently retained (1,540 bytes); failed candidates and declaration views restored. Final two local-allocation retries and validation follow.
- 2026-10-11: Four-match batch Docker ninja passed 27/27 checksums, headers and globals. The final model/callback local-allocation retries differ and are restored. Added 14 T-0018 evidence rows in total.
- 2026-10-11: Inline code-review skill against HEAD (4607287db1200b1b005233958cb1d4f5e56bbf72), with the shared wave-8 brief as spec. Standards finding: incomplete DATE workspace, TAIIKU coordinate and libgs light aggregate views needed explicit FAKE documentation; comments now explain and ticket all three. No SDK definitions/prototypes added. Existing placeholder names and shared declarations retained. Spec review: all 152 entries accounted for; only owned C files, permitted declarations and wiki files changed; no raw/generated sources, tools or Git state changed. Final verification and reuse audit pending.
- 2026-10-11: Final Docker verification PASS: sync_protos --check-branch reports 0 new disagreements (17 known); migrate_globals --check passes; all four fresh resolved function diffs MATCH after comments; incremental ninja exits 0 with 27 checksum checks, followed by explicit 27/27 SHA1 verification and header/global stamps. Owned-file dupes applied 0 copies. Final progress: grand total 4738/6958 functions, 731152/2279368 bytes (32.1%); SDK 722 functions excluded.
- 2026-10-11: Inline review complete. Standards: three documentation findings resolved, zero outstanding; C89/fixed-width/layout comments, existing SDK declarations, field-based GameState/Rec24 access, explained FAKE views and overlay-only delta declaration checked. Spec: zero outstanding; all 152 entries attempted/time-boxed, four matches (1,540 bytes), fourteen appended T-0018 rows, allowed-file scope and no Git mutations confirmed. Moved to Done; edits remain uncommitted for orchestrator merge. Evidence: scratchpad/w8-5/final-validation.log and results-*.jsonl.
