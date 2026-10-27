---
id: T-24025
title: Recover GEKO hallway event picker
status: Done
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-26
links: ["[[tickets/T-24020-astra-hard-functions]]", "[[tickets/T-23005-w5-big-p05]]"]
---

## Goal

Recover only `func_80142C9C` (3700 bytes) in `src/ovl/GEKO/80142760.c`.

## Acceptance criteria

- [ ] Read prior evidence and investigate loop-1 live ranges with the existing allocator trace.
- [ ] Verify any promoted C through default linked literal bytes and GEKO SHA1 plus guards.
- [ ] Preserve source/negative evidence and obtain standards review before Done.

## Notes

T-23005 recovered frame 0x558 and stack offsets; 492 printed allocation rows remain. The draft hoists constant 7 and spills the flag pointer. Concrete next hypothesis: recover the original year/selector comparison structure and constant use counts to restore allocation order.

## Comments

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
