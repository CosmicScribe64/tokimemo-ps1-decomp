---
id: T-10040
title: "Wave 11: list 4"
status: Done
assignee: w11-4
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the 25 assigned large functions (65,612 bytes) in TEL/80132000,
BUNKASAI/801511E0, DATE/801461B0 and EVENT/8011C650.

## Acceptance criteria

- [x] Draft/build/diff first pass, then ranked deep attempts and type repair.
- [x] At least four hours of matching work; started 2026-10-12 00:54:18 UTC.
- [x] Retained matches verified by resolved function diff and all 27 checksums.
- [x] Near misses below about 30 differing lines retained with ticketed NON_MATCHING guards.
- [x] Header/global checks and inline standards/spec review complete.
- [x] Changes remain uncommitted within the assigned scope.

## Notes

Shared wave11-brief.md governs this batch. Scratch lives in scratchpad/w11-4/.
Read the workflow, standards, wave-10 large-function findings, register-shape
notes and allocator trace implications before implementation.

Current verified result: 29 active conversions, 28,220 text bytes. The latest
default build and a separate audit of all 27 binary checksums pass. Header and
global checks pass; `sync_protos.py --check-branch` reports zero new
disagreements (17 known). `ninja progress` reports 5011/6958 functions and
865160/2279368 bytes (38.0%). Five assigned conversions account for 10,432
text bytes; 24 further conversions within the owned files account for 17,788.
These sizes use the progress/assembly-header text sizes, not work-list ranking
sizes.

Twenty-one BUNKASAI routines have exact resolved instruction diffs but remain
guarded because orphan jump tables still reference assembly labels. Four
additional guarded drafts retain bounded differences: TEL 80132FC4,
BUNKASAI 80151364, DATE 801489D0 and EVENT 8011EDFC. BUNKASAI 80151364
has two real resolved differing lines; 80155388 now has an exact resolved
instruction comparison. Data-island/YAML changes
are outside the batch scope. See the T-10040 section of [[matching-notes]].

Explicit FAKE accommodations: TEL 8013226C's 36-byte buffer, TEL 80133D54's
44-byte buffer, BUNKASAI 801527A8's two unused frame words, and the guarded
TEL 80132FC4's 44-byte buffer preserve measured frame or buffer homes. Each
has a nearby ticketed explanation. TEL 8013655C uses recovered halfword
arrays and declaration order without padding.

Three guarded-source FAKE accommodations are also explicit: BUNKASAI 80151364
reassigns its state pointer to split year/base live ranges; 80155388 and
80157860 use empty result tests to supply the needed live ranges without
instructions. All preserve semantics and remain guarded. Two bounded-evidence
T-0018 rows are recorded. The current-source duplicate scan at 5011 matches
finds zero plans within the four owned files.

## Comments

- 2026-10-11: Claimed list 4. Initial worktree is clean. No subagents, commits,
  tool changes or build cleanup are planned, as required by the brief.
- 2026-10-11: Inline CODING_STANDARDS.md review checked the active conversions,
  all retained guards, shared declarations, and changed-file scope. C89 and
  fixed-width types are preserved; native game/SDK aggregates are used, new
  layouts document offsets/sizes, and no SDK type is redefined. All seven
  new FAKE accommodations carry a reason and ticket; the two empty tests use
  already initialized helper results. Guards have original fallbacks and
  ticketed explanations. Header/global/prototype checks pass, and all 27
  binary hashes match. No config/tool/raw/generated source was edited, no
  host dependencies were installed, and nothing is staged or committed.
  Cleanup and guard-comment consistency findings are resolved. The last
  private DATE search and the four-hour minimum are pending; no review
  finding blocks the retained code.

- 2026-10-11: Final review approved; no unresolved findings. Completed at 2026-10-12 04:54:26 UTC after 4h 0m 8s. All 25 assigned functions received first and deep passes. The last 300-second DATE search produced no acceptable match; its best candidate moved a required initialization after a return and was rejected. Final retained result: 29 active matches / 28,220 bytes, 21 instruction-exact orphan-table guards, four near-match guards, and two T-0018 rows. All 27 binary checksums and declaration checks pass. Changes are uncommitted.
