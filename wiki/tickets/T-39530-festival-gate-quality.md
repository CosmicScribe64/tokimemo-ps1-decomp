---
id: T-39530
title: Fix festival gate lettering and verify dialogue clearing
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

The owner rejected the festival gate capture. The standing banner is cramped, the school plaque has damaged-looking letters, and dialogue overlaps.

## Acceptance criteria

- [x] Redraw both festival banner variants with readable spacing and weight, preserving their wording and placement.
- [x] Repair the school plaque lettering and remove residual original strokes while preserving its border.
- [x] Inspect the encoded backgrounds and fresh emulator captures at native size and enlarged.
- [x] Verify normal dialogue progression and clearing in this scene. Reject captures produced by incorrect scene-state injection.
- [x] Review against CODING_STANDARDS.md and resolve findings before closing.

## Notes

Affected backgrounds are D_01_47 and D_01_48. The first Saki fixture accidentally omitted the festival phase-7 write and produced overlapping old dialogue; it is rejected as evidence. A corrected fixture is running. Artwork defects remain independently actionable.

2026-11-02: compared the original background, the published encoded background and the old uncompressed plaque drawing. The old drawing has clean erasure, but its encoded output retains dark residue at the left. Therefore crop erasure alone is insufficient; inspect codec error before accepting the replacement. Drafted thinner banner strokes with wider letter gaps and an antialiased plaque. The banner drawing has visibly better separation; the plaque drawing remains too cramped and needs another pass. Three source-erasure, border and identity tests pass. Neither draft is approved or published. The compressed-preview process is still running; its first draft predates the deterministic plaque-fill adjustment.

2026-11-02: the corrected invitation fixture displays Saki normally. Its refusal variant scrolls fully, and normal Circle input clears the box without overlapping letters (frames 800/1000). This proves clearing for the tested page; a complete normal scene progression remains pending. Rejected the antialiased plaque draft as too cramped at five pixels high. Restored the clearer pixel letter shapes with reduced ink contrast for the next codec test. The earlier compressed-preview process remains live and is useful only as an encoding diagnostic.

2026-11-02: inspected the first compressed sports-festival draft. The lighter banner stays separated after encoding; the antialiased plaque remains cramped and is rejected. Its measured English-pixel error is at most 4 RGB555 levels (mean 0.4715), with ring error 2 and fixed/float IDCT difference 1. These numbers do not establish readability. Started an isolated second encoding pass with the clearer five-pixel capitals and ink level 5, checking the cultural-festival version first. All three plaque safeguard tests still pass. Candidate outputs remain local and unpublished.

2026-11-02: the 4,500-frame controller-driven replay enters the festival with date/relationship setup at frame 120, then uses only Circle input. Inspected all 14 captures through invitation, answer choices, club selection and the literature exhibit. No overlapping dialogue appears; pages clear correctly. Receipt: `translations/en/reviews/festival-dialogue-flow-2026-11-02.json`. This closes the scene-clearing check, while sign artwork remains rejected.

2026-11-02: inspected the second compressed D_01_48 candidate at 3x. The pixel plaque remains legible and its former dark residue is absent; the lighter banner has separated letters. English-pixel maximum error is 4 RGB555 levels, mean 0.4656, ring error 2, IDCT cross-check difference 1; untouched macroblocks decode identically. Placed only this background in a local review disc, verified exact readback and unchanged LBA/sector allocation, and started the controller-driven emulator replay. Local review-disc SHA-1: bdfbbbdb5fb68962e6ccc575aec33c618fd1a1bb. The sports variant continues encoding; no publication yet.

2026-11-02: inspected all 14 captures of the revised cultural-festival replay. The signs remain clean in-game, and dialogue progression remains unchanged; dialogue-region bytes at frames 900, 3000 and 4200 exactly match the earlier normal-flow replay. Inspected the compressed sports-festival candidate as well; banner spacing and plaque text are clear. Created `build/festival-gate-quality-v2/before-after.png` from real emulator captures. All 28 artwork tests pass. The first complete artwork build finished; a second combined build includes T-39531's subsequent wrapping fix. Sports-scene replay, combined-disc audits and publication remain pending.

2026-11-02: inspected all 14 captures of the 4,500-frame sports-festival replay, including its entrance signs and the start of the spoon race. Both revised sign variants now have in-game visual evidence. Recorded encoded-background and screenshot hashes, codec metrics and test results in `translations/en/reviews/festival-gate-quality-2026-11-02.json`. Combined-release auditing and publication are still pending.

2026-11-02: combined target b8b5be606a64650203c4b6c452fb8f61cdadef83 passes 252 artwork checks, complete script/pointer and executable audits, and all 15 browser-patcher checks. Inspected desktop/mobile result pages. Both background files exactly match the inspected compressed candidates. Comparing 1,371 raw file extents finds only D_01_47, D_01_48 and NI_S6W changed; audio is identical. Receipt: `translations/en/reviews/festival-wrap-release-2026-11-02.json`. Source and editorial review remain before commit; publication is pending.

2026-11-02: reviewed the artwork source change against CODING_STANDARDS.md. Source hashes guard both originals; exactly one registered banner per variant must be replaced; generated pixels remain ignored; crop geometry and border preservation pass tests; the build verifies encoded output. No game data enters the commit. No unresolved code-review findings. Final staged prose review and publication remain pending.

2026-11-02: implementation, visual checks, regression tests and standards review are complete. Closed this fix ticket; the parent campaign remains open for broader visual coverage. The combined-release receipt tracks publication.
