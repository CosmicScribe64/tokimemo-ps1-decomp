---
id: T-13800
title: Recover remaining TACO actor families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[tickets/T-13801-taco-actor-family]]", "[[tickets/T-12070-codex-cx7]]"]
---

## Goal
Recover verified TACO game bytes with bounded coherent actor/table family reconstruction from baseline 97ec697f. Own only src/ovl/TACO/*.c and include/ovl/TACO.h. Original checkout remains read only.

## Acceptance criteria
- Full-TU literal linked proof and TACO SHA1 for each accepted match.
- Preserve existing C, Docker tools only, no generated assembly edits.
- Standards review before Done; commit verified matches for integration.

## Notes
Prior T-12070 and T-13400 read. Do not repeat ACCC/BD30 broad searches or 32FE8 local lifetime permutations without new evidence. Prefer >=800-byte siblings.

## Comments
Setup: isolated codex/taco-13800 created from origin/main; copied ignored reference inputs from integration.

2026-10-17: This was a research handoff: zero bytes matched and no source change reached main. The same functions were taken up by the wave-3 TACO packages: func_801487A4 matched in T-19013, func_801497EC is a one-row NON_MATCHING draft from T-19019 (the same commuted add found here), func_8014A480 is a guarded draft from T-19003. Marked superseded; taco-family-results stays as the evidence page.

## Diagnostic handoff

No production function accepted: zero new matched bytes. Best 497EC has one commuted pointer-add row; 4A480 has three rows. 87A4 natural full-TU candidate has 62 rows. See [[taco-family-results]] for bounded experiments and ignored build/t13801/reprobe.py for reproduction. All production source/header trial edits restored. Final default overlay verification and independent standards/spec review are recorded below.

### Final verification and review

Docker reprobe.py reproduces 497EC one row and 4A480 three rows, no compile errors, and restores source bytes. Subsequent `ninja -j1 build/ovl/TACO.ok build/headers.ok build/globals.ok` exits 0; TACO SHA1 OK (1419e93cbad6c0df790577701181be04e0747969). Logs build/t13801/{reprobe,final-verified}.log; the earlier full rebuild records headers OK. Production src/include diff versus 97ec697f is empty. No whole-game checksum claim is made.

Independent GPT-6.1 Sol medium standards review: no findings. Spec review: primary matching goal remains unmet (zero accepted bytes); the stale pending-validation statement is now corrected. Tickets remain In Review as a research handoff, not Done. No shared declarations, guards or FAKE workarounds were installed. Original checkout remains unchanged by this task.
