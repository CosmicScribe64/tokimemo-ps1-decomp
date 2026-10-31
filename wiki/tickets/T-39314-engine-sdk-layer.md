---
id: T-39314
title: Give the engine an SDK layer for the PsyQ library calls (M2)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[psyq-sdk]]", "[[ps1-renderer]]"]
---

## Goal

Own the PsyQ SDK calls the game makes, so ported engine code reaches the generic renderer, the vsync and pad model, the memory card service and libc. Today every SDK call refuses by name.

## Acceptance criteria

- [ ] libgpu calls used by `src/main` are backed by the generic renderer: primitive setup (Set*/AddPrim and the sorters), GetTPage/GetClut, SetDrawMode, ordering tables (ClearOTag/ClearOTagR/DrawOTag), Load/Store/Move/ClearImage, PutDrawEnv/PutDispEnv and DrawSync.
- [ ] VSync and the other blocking waits follow the contract's rule for blocking waits.
- [ ] libapi events and the card calls (`_card_*`, `_bu_init`, InitCARD/StartCARD, open/read/write/close/format/delete) are backed by `services/memory_card_service.gd`.
- [ ] libc gains `rand` (BIOS LCG as the port already models it), `abs` and the other calls the packages reported.
- [ ] Each call has a function-differential test from captures, or is listed as untested with a reason.

## Comments
