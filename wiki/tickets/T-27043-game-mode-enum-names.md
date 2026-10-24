---
id: T-27043
title: Apply consistent game-mode enum names
status: In Progress
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27042-enum-rename-support]]", "[[tickets/T-27004-shared-core-names]]"]
---

## Goal

Close the shared-core enum naming remainder: MainMode -> GameMode and MODE_* -> GAME_MODE_* using the reviewed rename tool. Namespace normalization and four source-corrected NAME_ENT labels preserve numeric values, fixed-width fields, behavior and existing hypothesis evidence.

## Acceptance criteria

- [ ] Independent table review; no changed mode meanings/values or dropped hypotheses.
- [ ] rename.py plan/apply/check and clean Docker 27/27 hashes, 6,952 matching functions.
- [ ] Independent Standards/Spec reviews resolved before Done.
- [ ] Ledger, living documentation and core handoff updated.

## Comments

2026-10-24: Claimed after enum-tool review fixes were independently verified.

2026-10-24 pre-application: independent Luna review approved all69rows: one tag and68members, exact one-to-one source coverage, unchanged values/meanings, no case-fold target collisions or hypothesis loss. Root Docker rename.py plan exited0,69rows/67files. Application and clean proof remain pending.

2026-10-24 audit correction: the completion lane surfaced four stale NAME_ENT mode meanings (0x11 opening,0x13file select,0x14name entry,0x15prologue). The first prefix-only apply had already flushed when interrupted during check; root restored only its67owned source/header edits from committed HEAD and removed its new69row ledger, with no other source work pending. The table now corrects these four names directly in the fresh namespace, avoiding misleading labels and ledger-name reuse. Fresh independent semantic review and a new plan/apply are required.

2026-10-24 revised review: Sol6.1 independently approved all69rows including four corrected NAME_ENT meanings, exact68valuecoverage, targetcollision scan and preservation of tenexistingHYPOTHESIScomments. Before landing also correct two stale enum definition comments (0x11opening,0x13file selection); these are prose corrections alongside the checked name application.

2026-10-24 application proof: revised69row collision-only plan exited0 (full gcc/prototype proofs retained in apply); fullapply exited0 and renames check reports849ledgerrows. Clean Docker build passed27/27 hashes and6952/6952functions. Corrected two stale NAME_ENT role comments and three MASTER callback-address comments against actualloader; incremental verification after the last comment-only corrections is running. Primary-owned living core/flow/scheduler/weekday specs use GameMode/GAME_MODE_*; other chat-owned specs are coordinated separately. Final independent code review and post-comment proof remain pending.
