---
id: T-39536
title: Keep long ending continuations within the glyph atlas
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[english-text-runtime]]"]
---

## Goal

Render every line of long continued ending speeches without exhausting glyph storage, preserving scrolling and typewriter timing.

## Acceptance criteria

- [x] Trace the visible failure to the logical glyph count and physical atlas use.
- [x] Keep visible rows intact when a continuation exceeds 384 accumulated glyphs.
- [x] Inspect Nozomi table 10 lines 12 through 14 and other maximum-length continuation chains through final punctuation.
- [x] Verify default matching, focused regressions and actual-disc readback.
- [x] Review the change against CODING_STANDARDS.md.

## Evidence

The published-build replay `build/emu/ending-scoped-nozomi` completed 24,000 frames. Inspected 98 selected captures across 11 sheets. Lines 12 through 14 collapse into overlapping glyphs at the left margin; line 15 restores normal rendering after the speech resets. Native frame 12420 shows the failure clearly.

Sampled totals rise from 332 glyphs at frame 10770 to 396 at 11550, 473 at 12420, 527 at 13080 and 587 at 13800. `english_glyph_pen` holds 384 entries; `english_disp_string` stops adding pen offsets beyond that bound. The upload function also maps every logical slot above 255 into the third 128-slot physical page, so enlarging the pen array alone would not repair atlas reuse. The ending's C/D/F continuation commands retain prior text entries until the complete speech ends.

The existing ending-summary queue solves a related capacity problem for a different renderer. Inspect its design before choosing a fix; the subtitle path must retain typewriter progression. No Nozomi records have been approved from this run yet.

## Capacity audit and candidate repair

`build/ending-capacity-profile.json` audits 381 subtitle records. The longest accumulated chain has 689 glyphs (table 5); retaining the final two rows between continuations reduces the largest load to 307 glyphs (table 9). The candidate compacts completed subtitle entries before commands D/F load their next record. It preserves the reserved first entry and visible rows, copies their VRAM cells in ascending order, and moves pen offsets and counters with them. GPU synchronization prevents queued copies from referencing a reused rectangle. Default code remains behind the existing English-build guard. Build and runtime verification are pending.

The candidate built successfully in 177.1 seconds after fixing English-overlay imports of existing SDK functions in `tools/english_link.py`. Frozen at `build/ending-capacity-release`; track prefix `66d0c190c1b6`, patch 6,564,403 bytes. Art verification covers 252 placements and 28 deliberate retentions. Default ENDING matches its original checksum. Nozomi and the longest-chain route are replaying; no runtime approval yet.

Save-state compatibility check: the candidate main executable has the same size as the prior review build and differs at only two bytes, addresses `0x8007936c`/`0x8007936d`. Probe scripts update exactly those bytes immediately after restoration, with an assertion on the complete difference list. DATE is byte-identical. No live main symbol moved; the only changed ELF alias points into ENDING.

## Late-speech visual check

Inspected `build/ending-capacity-checkpoints/nozomi-late-chain.png`: nine captures from frames 10770 through 14040 cover the original overflow, subsequent scrolling rows, the final speech line and reset into the next reply. Every displayed row has distinct glyphs; no left-margin collapse, atlas corruption or stale rows are visible. This proves the originally failing Nozomi interval, not all endings. Full replay and independent longest-chain review remain running.

Frozen-disc audits pass: 105 script containers, 31,471 strings, 31,334 pointers; executable audit has 2,828 exact slots, 291 derived records, 286 pointers and zero pending slots. The executable audit must use `--link build/en` because the frozen link maps retain original object paths; using the copied directory as the link path failed before producing a result. Default ENDING, globals and 14,449 rename checks passed. Focused linker/text suites pass 64 tests with one optional disc test skipped, including the new test for relocation of newly imported SDK symbols.

## Completed capture review

Nozomi's 24,000-frame run completed. Inspected all 98 selected captures across 11 sheets, including complete long records 3, 13 and 21. Rei's first 24,000 frames completed; inspected 116 captures across 13 sheets through line 38. The longest 689-glyph logical chain reaches its final quote at frame 18750 with intact scrolling rows. Sampled live atlas totals stay at or below 234 for Nozomi and 137 for Rei. Five additional flagged records now have hashed receipts in `translations/en/reviews/ending-capacity-2026-11-02.json`; the alternate Nozomi branch remains pending. Rei's tail is continuing from the saved state to cover line 43.

## Standards review

Reviewed compaction and linking against CODING_STANDARDS.md: C89 declarations, SDK RECT/prototypes, English-only guard, no renamed existing symbols, no modified generated assembly, and unchanged default checksum. Increasing logical indices copy to lower unused atlas slots, preserving unreplaced source cells; each GPU copy completes before the stack rectangle is reused. Text entries, pen offsets, reveal totals and mark move together. Existing imports are not duplicated; the linker test checks actual relocated addresses and rejects an unused import. Full-corpus capacity bounds and the two runtime chains cover atlas-page transitions. No unresolved code-review finding; remaining route regression checks are still pending.

Rei's saved-state continuation completed another 6,000 frames. Inspected all 15 selected tail captures across two sheets, including line 43 through its closing quote at frame 2310. The receipt now covers six additional flagged records; total queue status is 37 approved, 50 pending. Both tested ending dialogues complete without held controller input or glyph overlap. The gallery contains 1,014 captures across 110 sheets.

## Final regression and source publication

Yukari's 18,000-frame regression completed on `build/break-reviewed-release`. Inspected all 71 selected captures across eight sheets, including the six-row subtitle and all six previously approved long records. Maximum sampled atlas use is 309 glyphs, including opening marks (the text-only audit counted 307). The renderer preserves every row and final quote. Receipt: `translations/en/reviews/ending-capacity-yukari-2026-11-02.json`.

The original checksum, focused tests, frozen-disc audits, failing Nozomi chain, longest Rei chain and largest Yukari load all pass. Standards and exact staged writing review found no unresolved issue. Moved to Done. Commit `6c83d3eb8` contains the renderer, linker imports and regression test and is pushed to `patch/english`. The patching page still serves the older review build; package publication and broader game review remain separate.
