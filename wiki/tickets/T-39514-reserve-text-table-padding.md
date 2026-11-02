---
id: T-39514
title: Reserve complete text table slots before relocating strings
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Prevent relocated strings from consuming zero padding still owned by fixed-stride text tables.

## Acceptance criteria

- [x] Reserve mapped table/extent slot storage before any relocation allocation.
- [x] Synthetic regression covers an early overflowing string and a final padded table slot.
- [x] Final-disc audit verifies every translated dictionary slot, including KANGEI gift_names index 98.
- [x] Review implementation against CODING_STANDARDS and verify the rebuilt patch/browser package.

## Evidence

Actual-disc table census of `build/anomaly-repair-proof` verifies 815 translated dictionary slots but finds KANGEI gift_names index 98 still in Japanese. Its mapped address is 0x8015DE2A. The pool begins 24 bytes into this 33-byte slot, after its short nonzero payload plus the 16-byte tail margin. An earlier overflow is placed there; the subsequent whole-slot identity comparison rejects the gift translation as moved in build. The remaining bytes are owned table padding, not free storage. Raw sources are unchanged.

## Progress

- Implemented pre-allocation reservation of every mapped in-place slot capacity, including untranslated/retained entries. Pool reservations advance the aligned cursor beyond owned storage.
- Synthetic regression covers an earlier overflowing string, an actual rewritten pointer, and the final zero-padded slot both translated and retained. All 36 English patch tests pass, one opt-in end-to-end test skipped; full build is running separately.
- Code review against CODING_STANDARDS finds no unresolved implementation issue: Python-only change, unchanged raw sources and original-game code, preserved allocator bounds/alignment. Final-disc and browser verification are still pending.

- Final verification: `build/table-padding-proof` completes in 215.9 seconds, SHA-1 `72cd7ff7ef559bd7362c26c6ffba7ce9297110ee`, xdelta 6,563,325 bytes; 246 artwork items verified. All 816 translated dictionary slots match their mapped final-disc addresses. Comparing all loaded executable containers shows only KANGEI changed; main ELF is byte-identical. Browser package `build/patcher-table-padding-review` passes all nine checks, and its downloaded disc hash equals the CLI build. Reviewed completion screenshot; no layout issue. All scoped acceptance criteria and code review are satisfied; moved to Done.
