---
id: T-39531
title: Fix the split Literature Club question
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

The owner flagged the festival question breaking between "Literature" and "Club". Preserve the wording and give it a natural layout.

## Acceptance criteria

- [x] Identify why the displayed question retains this break.
- [x] Correct the layout without changing approved dialogue wording.
- [x] Check duplicate questions and related break handling for regressions.
- [x] Verify the corrected line in the emulator.
- [x] Review the change against CODING_STANDARDS.md before closing.

## Notes

The translated NI_S6W groups 2 and 4, line 0, contain the inherited separator between "Literature" and "Club". The whole quoted question measures 192 pixels against the static speech window's 196 pixels. The original Japanese break is mid-sentence. Trace the encoded bytes and runtime selection before choosing a fix.

2026-11-02: the break is newly chosen during wrapping, not retained from Japanese. `speech_window` reserves Tatebayashi's label width for every heroine, leaving 179 pixels; the question's body plus closing quote needs 187. `tour_saki_script_begin` binds NI_S6W and calls `talk_names_set(4)`, so the heroine label in this container is Nijino. Added a proven-speaker override for this container's heroine speech commands only; player, continuation, inline-label and other-container safeguards remain unchanged. All five copies of the question now encode on one line. Compared every NI_S6W payload: 22 records change whitespace only, all remain placeable. Wrapping tests pass (41 tests, one optional skip). The isolated review disc changes only the five exact question separators and is replaying; its SHA-1 is 53853bcd1e1ba6bf217472fb8d6c1ce99f1145a4. Full rebuilt-disc verification remains required for all 22 changes.

2026-11-02: the isolated replay completed 4,500 frames. Inspected frame 3300: the full question and closing quote fit on one line beside Nijino. The first complete build (artwork only) has finished. Started the combined build at `build/festival-wrap-release/disc` for all 22 layout changes plus the two sign revisions. Do not publish the earlier artwork-only build as the combined fix.

2026-11-02: the combined target b8b5be606a64650203c4b6c452fb8f61cdadef83 passes exact auditing of 31,471 strings and 31,334 pointers across 105 text containers, plus executable auditing and all 15 browser-patcher checks. The whole-disc comparison confines dialogue changes to NI_S6W; its 22 changed payloads preserve wording. Receipt: `translations/en/reviews/festival-wrap-release-2026-11-02.json`. Source and editorial review remain before commit; publication is pending.

2026-11-02: the runtime display-wrap regression test also passes. Reviewed the source change against CODING_STANDARDS.md: the known-speaker override applies only to heroine speech commands in the proven NI_S6W binding; player, continuation, inline-label, phone and unknown-container handling keep their previous limits. No C/game-code change or toolchain dependency was introduced. No unresolved code-review findings. Final staged prose review and publication remain pending.

2026-11-02: implementation, visual checks, regression tests and standards review are complete. Closed this fix ticket; the parent campaign remains open for broader visual coverage. The combined-release receipt tracks publication.
