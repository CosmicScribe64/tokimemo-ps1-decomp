---
id: T-9560
title: "Wave 8: list 6"
status: Done
assignee: w8-6
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[register-allocation-wave7]]", "[[matching-notes]]"]
---

## Goal

Time-box all 151 assigned functions in the 35 owned files, retaining verified byte-matching C.

## Acceptance criteria

- [x] Assigned functions matched or time-boxed with unresolved functions restored to INCLUDE_ASM.
- [x] Final Docker ninja passes all 27 unit SHA1 checks, headers and globals checks.
- [x] Prototype branch check passes and inline standards review is recorded.

## Notes

Shared brief and list: scratchpad/wave8-brief.md and scratchpad/wave8-list-6.txt. Scratch artifacts: scratchpad/w8-6/. No commits, tool changes or edits outside the brief's ownership boundaries.

## Comments

2026-10-11: Read wiki index, shared brief, coding standards, local instructions, workflow and register-allocation findings. DATE func_8015A7E0 is a documented handwritten rotation helper and remains assembly.

- `GYOZI:func_80137F48`: time-boxed baseline difference.

- `EVENT:func_8010B8A4`: compile/header draft failure.

- `main:func_8005B040`: compile/header draft failure.

- `TEL:func_80139E3C`: aggregate draft needs hand rewrite.

- `RPG_BAT:func_8014F540`: compile/header draft failure.

- `EVENT:func_80108A24`: compile/header draft failure.

- `EVENT:func_8010B614`: time-boxed baseline difference.

- `BUNKA_SD:func_8013987C`: compile/header draft failure.

- `main:func_8005DDB4`: aggregate draft needs hand rewrite.

- `main:func_8005B2BC`: compile/header draft failure.

- `SHOUGATU:func_8013E330`: draft needs types or struct-copy reconstruction.

- `DATE:func_8015A7E0`: handwritten SDK assembly.

- `EVENT:func_80114B0C`: compile/header draft failure.

- `TACO:func_801423B0`: time-boxed baseline difference.

- `TACO:func_8013DDE4`: compile/header draft failure.

- `TEL:func_8013BE38`: compile/header draft failure.

- `EVENT:func_80118868`: compile/header draft failure.

- `BUNKA_SD:func_80138CAC`: aggregate draft needs hand rewrite.

- `TACO:func_8013E424`: draft needs types or struct-copy reconstruction.

- `ETC:func_80140BC4`: aggregate draft needs hand rewrite.

- Scoped dupes pass: one copy plan, rejected by verification; zero retained.
- Tooling discrepancy: the brief's positional function filter for register_shapes.py is unsupported. Classified through the existing Python inventory API instead, without changing tools.

- Focused `GYOZI:func_80137F48` variants: DIFF (9 candidates).

- Focused `EVENT:func_80108A24` variants: DIFF (3 candidates).

- Focused `RPG_BAT:func_8014F540` variants: DIFF (3 candidates).

- `EVENT:func_80108A24`: short 45-second permuter pass; candidates require source review and real unit verification.

- `GEKO:func_80140040`: compile/header draft failure.

- `TAIIKU:func_8013384C`: draft needs types or struct-copy reconstruction.

- `EVENT:func_8011C470`: time-boxed baseline difference.

- `EVENT:func_80118DAC`: draft needs types or struct-copy reconstruction.

- `TACO:func_80140638`: time-boxed baseline difference.

- `TAIIKU:func_801339C4`: draft needs types or struct-copy reconstruction.

- `main:func_8005B1A8`: aggregate draft needs hand rewrite.

- `RPG_BAT:func_8014E994`: compile/header draft failure.

- `TT:func_80148460`: compile/header draft failure.

- `EVENT:func_801155D4`: time-boxed baseline difference.

- `EVENT:func_8011AD7C`: compile/header draft failure.

- `RPG_BAT:func_8014F278`: compile/header draft failure.

- `TACO:func_801404FC`: compile/header draft failure.

- `TT:func_80148244`: compile/header draft failure.

- `RPG_BAT:func_8014EF8C`: compile/header draft failure.

- `main:func_8005A410`: aggregate draft needs hand rewrite.

- `TACO:func_8013CE78`: compile/header draft failure.

- `EVENT:func_80119370`: compile/header draft failure.

- `TAIIKU:func_80133258`: compile/header draft failure.

- `TAIIKU:func_801330FC`: time-boxed baseline difference.

- Focused `TAIIKU:func_8013384C` variants: DIFF (4 candidates).

- `GYOZI:func_8013506C`: compile/header draft failure.

- `TAIIKU:func_80132BD0`: draft needs types or struct-copy reconstruction.

- `TACO:func_801407D4`: draft needs types or struct-copy reconstruction.

- `ETC:func_80140AE4`: time-boxed baseline difference.

- `EVENT:func_8011A8E4`: draft needs types or struct-copy reconstruction.

- `RPG_BAT:func_80132B34`: time-boxed baseline difference.

- `EVENT:func_8011B60C`: compile/header draft failure.

- `main:join_club_select`: time-boxed baseline difference.

- `EVENT:func_80108B24`: compile/header draft failure.

- `EVENT:func_80110234`: compile/header draft failure.

- `RPG_BAT:func_801328C4`: draft needs types or struct-copy reconstruction.

- `GYOZI:func_80135518`: compile/header draft failure.

- `TACO:func_8013E4FC`: time-boxed baseline difference.

- `main:func_8005D448`: compile/header draft failure.

- `TAIIKU:func_80132E20`: draft needs types or struct-copy reconstruction.

- `ETC:func_80149F48`: compile/header draft failure.

- `main:week_day_main0`: time-boxed baseline difference.

- `TACO:func_80140364`: time-boxed baseline difference.

- `main:func_800603C0`: aggregate draft needs hand rewrite.

- `main:func_80074DE0`: compile/header draft failure.

- Focused `TAIIKU:func_8013384C` variants: MATCH (1 candidates).

- `RPG_BAT:func_8015461C`: compile/header draft failure.

- `TACO:func_8013F874`: time-boxed baseline difference.

- `SHOUGATU:func_80138758`: time-boxed baseline difference.

- `RPG_BAT:func_8014EBEC`: compile/header draft failure.

- `EVENT:func_8011AC18`: draft needs types or struct-copy reconstruction.

- `BUNKA_SD:func_80139100`: time-boxed baseline difference.

- `TACO:func_8015776C`: draft needs types or struct-copy reconstruction.

- `main:func_8005CA94`: compile/header draft failure.

- `main:func_800741B8`: compile/header draft failure.

- `main:func_80073AD8`: time-boxed baseline difference.

- `TACO:func_8013DB80`: draft needs types or struct-copy reconstruction.

- `TACO:func_80140028`: compile/header draft failure.

- `EVENT:func_8011C164`: compile/header draft failure.

- `TACO:func_801428DC`: aggregate draft needs hand rewrite.

- `EVENT:func_801142E8`: compile/header draft failure.

- `EVENT:func_80114784`: compile/header draft failure.

- `EVENT:func_80114948`: compile/header draft failure.

- `TT:func_80142350`: compile/header draft failure.

- `RPG_BAT:func_8015A744`: time-boxed baseline difference.

- `TACO:func_8013F9C0`: aggregate draft needs hand rewrite.

- `EVENT:func_80112750`: compile/header draft failure.

- `TACO:func_801585D0`: compile/header draft failure.

- `TACO:func_80142220`: compile/header draft failure.

- `EVENT:func_80118010`: compile/header draft failure.

- `EVENT:func_80113A40`: compile/header draft failure.

- `main:func_8007437C`: aggregate draft needs hand rewrite.

- `SHOUGATU:func_8013EE30`: MATCH.

- Focused `TEL:func_80139E3C` variants: DIFF (4 candidates).

- `TEL:func_80139E3C`: short 45-second permuter pass; candidates require source review and real unit verification.

- `GEKO:func_801403A0`: time-boxed baseline difference.

- `EVENT:func_80113520`: compile/header draft failure.

- `EVENT:func_801136FC`: compile/header draft failure.

- `TACO:func_8013CC40`: draft needs types or struct-copy reconstruction.

- `EVENT:func_801130F0`: compile/header draft failure.

- `EVENT:func_80109404`: draft needs types or struct-copy reconstruction.

- `GYOZI:func_80137F48`: 25-second single-worker permuter pass after real resolved DIFF; rejected source restored. Candidate evidence in scratchpad/w8-6/permuter-cases.jsonl.

- `main:func_8005F09C`: time-boxed baseline difference.

- `GEKO:func_801406CC`: draft needs types or struct-copy reconstruction.

- `EVENT:func_80113300`: compile/header draft failure.

- `TACO:func_8013C1F4`: compile/header draft failure.

- `main:func_8005BCEC`: compile/header draft failure.

- `TEL:func_8013AC0C`: aggregate draft needs hand rewrite.

- `RPG_BAT:func_8013CC90`: compile/header draft failure.

- `TACO:func_8013C440`: compile/header draft failure.

- `ETC:func_8014A360`: time-boxed baseline difference.

- `TEL:func_8013AA30`: aggregate draft needs hand rewrite.

- `RPG_BAT:func_8014E780`: time-boxed baseline difference.

- `TACO:func_8013FBF0`: aggregate draft needs hand rewrite.

- `SHOUGATU:func_8013E50C`: draft needs types or struct-copy reconstruction.

- `TACO:func_80141DE0`: compile/header draft failure.

- `main:func_8005DC4C`: draft needs types or struct-copy reconstruction.

- `main:func_8005E150`: draft needs types or struct-copy reconstruction.

- `TACO:func_80140ADC`: draft needs types or struct-copy reconstruction.

- `main:func_8005DEA0`: compile/header draft failure.

- `EVENT:func_8011BE00`: compile/header draft failure.

- `main:func_8005FDEC`: draft needs types or struct-copy reconstruction.

- `TACO:func_8013E778`: draft needs types or struct-copy reconstruction.

- `TACO:func_80153B38`: draft needs types or struct-copy reconstruction.

- `RPG_BAT:func_80154DD8`: time-boxed baseline difference.

- `RPG_BAT:func_8015528C`: time-boxed baseline difference.

- `TACO:func_80159090`: compile/header draft failure.

- `TACO:func_8015A948`: compile/header draft failure.

- `main:func_80073CB0`: compile/header draft failure.

- `main:pre_syogatu1`: time-boxed baseline difference.

- `TACO:func_8013F468`: draft needs types or struct-copy reconstruction.

- `TACO:func_801424A8`: draft needs types or struct-copy reconstruction.

- `SHOUGATU:func_8013EA98`: draft needs types or struct-copy reconstruction.

- `BUNKA_SD:func_80138D48`: compile/header draft failure.

- `GEKO:func_801404FC`: compile/header draft failure.

- `EVENT:func_8011B190`: compile/header draft failure.

- `main:func_8005C5BC`: compile/header draft failure.

- `main:func_8005CF6C`: compile/header draft failure.

- `EVENT:func_80111F04`: compile/header draft failure.

- `RPG_BAT:func_80132FE0`: time-boxed baseline difference.

- `main:func_80060178`: aggregate draft needs hand rewrite.

- `main:func_8005D9B4`: compile/header draft failure.

- `main:parameter_up_down`: compile/header draft failure.

- `TACO:func_80159878`: compile/header draft failure.

- `EVENT:func_8011B848`: compile/header draft failure.

- `RPG_BAT:func_801321D0`: compile/header draft failure.

- `RPG_BAT:func_80155CC0`: compile/header draft failure.

- `RPG_BAT:func_80154760`: compile/header draft failure.

- `main:func_8005A668`: aggregate draft needs hand rewrite.

- `RPG_BAT:func_80154430`: time-boxed baseline difference.

- `RPG_BAT:func_8013C890`: time-boxed baseline difference.

- `TACO:func_8013BAC4`: draft needs types or struct-copy reconstruction.

- `TACO:func_80159260`: compile/header draft failure.

- `EVENT:func_80112930`: compile/header draft failure.

- `main:join_club_message`: compile/header draft failure.

- `TACO:func_8013EAA4`: draft needs types or struct-copy reconstruction.

- `MASTER:func_801359F0`: draft needs types or struct-copy reconstruction.

- `RPG_BAT:func_80156064`: compile/header draft failure.

- `TACO:func_80156A80`: compile/header draft failure.

- `RPG_BAT:func_80132348`: compile/header draft failure.

- Main pass complete: all 151 assigned functions received bounded attempts in list order. Two functions are retained so far (TAIIKU `func_8013384C`, 220 bytes; SHOUGATU `func_8013EE30`, 576 bytes). The DATE rotation helper stays handwritten assembly. Four small-function retry sets remain before final guards and review.

- Focused `RPG_BAT:func_8014F540` variants: DIFF (4 candidates).

- Focused `EVENT:func_8010B614` variants: DIFF (3 candidates).

- Focused `TACO:func_801423B0` variants: MATCH (2 candidates).

- Focused `EVENT:func_8011C470` variants: DIFF (4 candidates).

- Focused `RPG_BAT:func_8014F540` variants: DIFF (2 candidates).

- Focused `EVENT:func_8011C470` variants: DIFF (2 candidates).

- `EVENT:func_8010B614`: typed prefix/Rec34/CharFlags view and canonical versus unprototyped callback view: DIFF. The existing callback argument is explicitly s32 in the unprototyped file view. Rejected changes restored.

- `EVENT:func_8010B614`: 25-second single-worker permuter pass after real resolved DIFF; rejected source restored. Candidate evidence in scratchpad/w8-6/permuter-cases.jsonl.

- `EVENT:func_8011C470`: 25-second single-worker permuter pass after real resolved DIFF; rejected source restored. Candidate evidence in scratchpad/w8-6/permuter-cases.jsonl.

- `GEKO:func_801403A0`: 25-second single-worker permuter pass after real resolved DIFF; rejected source restored. Candidate evidence in scratchpad/w8-6/permuter-cases.jsonl.

- Final list audit: 151 assigned functions, 3 retained plain-C matches (1,044 bytes), 148 remaining INCLUDE_ASM functions. Retained: TAIIKU `func_8013384C` (220), SHOUGATU `func_8013EE30` (576), TACO `func_801423B0` (248). Five controlled T-0018 rows added. Final checks and inline review pending.

- Final scoped duplicate pass: one EVENT candidate, `func_8011B5A8 <- func_8013FA6C`; rejected by unit check, source and include snapshot restored. No new match. Final build invalidates only SHA1/header/global stamps to print all 27 results, preserving compiled objects.

- Inline review uses `git diff HEAD` for uncommitted work, the shared wave-8 brief and assigned list as Spec, and CODING_STANDARDS.md plus the local code-review checklist as Standards. The brief explicitly requires inline review, so no reviewer sub-agents were used.
- Standards source review: no findings. C89 block declarations, fixed-width types, original symbol names/order, existing SDK/game types, append-only overlay declarations with layout comments, and matching explanations checked. The TAIIKU scalars represent live promoted values; there is no dummy padding. TACO uses real Tc14 field accesses and stride. No FAKE, NON_MATCHING, SDK redefinition, tool/config edits, copyrighted data, or unrelated source changes retained.
- Spec source review: no findings. All 151 assigned functions have bounded evidence; 148 unresolved functions remain INCLUDE_ASM. Three retained functions total 1,044 bytes. Owned-file duplicate pass completed with no retained copy. Five controlled T-0018 cases and non-register blockers are documented. Final build and post-duplicate prototype checks remain pending.

- Final Docker incremental `ninja -j4` (no `-k`) exited 0: all 27 binary SHA1 checks OK, `headers OK`, `globals OK`. This reverified the retained TAIIKU field naming and source comments. Post-duplicate `sync_protos.py --check-branch`: zero new disagreements, branch check OK (17 known baseline entries). `migrate_globals.py --check` and `git diff --check` pass.
- Inline review concluded: Standards 0 findings, Spec 0 findings, no unresolved findings. Final source scope is exactly three owned C files and appended TAIIKU declarations; wiki updates are confined to the permitted files. No commits, merges or pushes.

- Final `ninja progress`: `grand total   4737/6958   730656/2279368  32.1%`. Gain: 3 functions and 1,044 bytes. All work is left uncommitted for the orchestrator. Top non-T-0018 blocker: untyped records/tables and missing main-global declarations; missing SDK SPRT type also blocks one GPU packet draft. Tooling discrepancy: the brief's positional `register_shapes.py <func>` filter is unsupported; inventory API used instead.

## Second deep pass

User reopened the ticket after committing the first pass as `3a7461b`. Start: 2026-10-11 11:10:13 UTC. Continue until at least 14:10:13 UTC or the list is exhausted. Rank rejected drafts by resolved differing instruction lines, defer large type repair initially, spend roughly 10-15 minutes of real source/assembly experiments per candidate, and use approximately five-minute permuter runs below 20 differing lines. Recover aggregate/local-copy types from neighbours and retry. Keep only resolved function matches with unit SHA1 verification; leave all second-pass changes uncommitted.

- [x] At least three hours of deep iteration and aggregate recovery performed; remaining failures restored.
- [x] Second-pass final guards, 27/27 SHA1 verification and inline review completed.

- Focused `main:func_80073AD8` variants: MATCH (1 candidates).

- Focused `RPG_BAT:func_80132FE0` variants: MATCH (1 candidates).

- Focused `TACO:func_80140638` variants: DIFF (7 candidates).

- Deep `TACO:func_80140638`: 9 source variants, 38.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_80140638`: 2 source variants, 28.6 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:join_club_select`: 16 source variants, 61.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:join_club_select`: 24 source variants, 127.2 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:join_club_select`: 10 source variants, 19.7 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:join_club_select`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `main:week_day_main0`: 0 source variants, 97.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:week_day_main0`: 9 source variants, 156.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:week_day_main0`: 0 source variants, 63.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 8 source variants, 31.1 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:week_day_main0`: 16 source variants, 66.6 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_80154430`: 16 source variants, 56.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_80154430`: 14 source variants, 37.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:week_day_main0`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `RPG_BAT:func_80154430`: 1 source variants, 60.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 12 source variants, 100.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_80154430`: 1 source variants, 69.3 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 8 source variants, 25.7 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8014E780`: 6 source variants, 19.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 8 source variants, 20.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `BUNKA_SD:func_80138CAC`: 8 source variants, 15.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8014E780`: 1 source variants, 73.2 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 7 source variants, 69.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `BUNKA_SD:func_80138CAC`: 12 source variants, 30.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E330`: 12 source variants, 26.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013F874`: 8 source variants, 74.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `BUNKA_SD:func_80138CAC`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `SHOUGATU:func_8013E330`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `SHOUGATU:func_8013E330`: 6 source variants, 44.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013F874`: 10 source variants, 197.6 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `BUNKA_SD:func_80138CAC`: 8 source variants, 81.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `GEKO:func_801403A0`: 4 source variants, 19.6 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:week_day_main0`: 1 source variants, 47.7 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TAIIKU:func_80132BD0`: 6 source variants, 20.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013E424`: 10 source variants, 39.6 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005DDB4`: 8 source variants, 85.3 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TAIIKU:func_80132BD0`: 16 source variants, 14.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8011C470`: 12 source variants, 24.6 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005DDB4`: 1 source variants, 0.6 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TAIIKU:func_80132BD0`: 8 source variants, 13.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005DDB4`: 8 source variants, 19.1 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013E424`: 16 source variants, 10.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E50C`: 6 source variants, 10.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013E424`: 8 source variants, 18.3 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8011C470`: 16 source variants, 22.1 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013E424`: 16 source variants, 17.7 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E50C`: 24 source variants, 31.2 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005DDB4`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `TACO:func_8013E424`: 6 source variants, 23.6 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013E424`: 6 source variants, 26.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8011C470`: 12 source variants, 108.1 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8010B614`: 8 source variants, 23.2 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `SHOUGATU:func_8013E50C`: 24 source variants, 21.7 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005DDB4`: 7 source variants, 71.2 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8010B614`: 24 source variants, 18.3 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8010B614`: 12 source variants, 12.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8011C470`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `TACO:func_8013E424`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `RPG_BAT:func_8014F540`: 16 source variants, 10.7 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8014F540`: 16 source variants, 47.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `EVENT:func_8010B614`: 24 source variants, 58.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TAIIKU:func_801330FC`: 8 source variants, 8.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TAIIKU:func_801330FC`: 4 source variants, 8.6 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8014F540`: 2 source variants, 8.9 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: 8 source variants, 2.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: 8 source variants, 15.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: 8 source variants, 5.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `GYOZI:func_80137F48`: 32 source variants, 11.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: 16 source variants, 8.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `GYOZI:func_80137F48`: 36 source variants, 8.4 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: 36 source variants, 10.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `GYOZI:func_80137F48`: 48 source variants, 8.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `BUNKA_SD:func_80139100`: 5 source variants, 10.6 seconds, MATCH. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:pre_syogatu1`: 8 source variants, 7.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `main:pre_syogatu1`: 8 source variants, 6.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:pre_syogatu1`: 24 source variants, 10.1 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:pre_syogatu1`: 8 source variants, 13.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:pre_syogatu1`: 12 source variants, 16.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005F09C`: 18 source variants, 23.5 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `TACO:func_8013DB80`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `ETC:func_80140AE4`: 20 source variants, 4.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `ETC:func_80140AE4`: 16 source variants, 14.8 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `ETC:func_80140AE4`: 16 source variants, 8.0 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:pre_syogatu1`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `main:func_8005F09C`: 24 source variants, 13.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `main:func_8005F09C`: 12 source variants, 6.2 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8015A744`: 12 source variants, 4.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8015A744`: 18 source variants, 3.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

- Deep `RPG_BAT:func_8015A744`: 27 source variants, 4.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

### Second-pass inline review preparation

Fixed point: first-pass commit `3a7461b59d2605bd9148b3cc46b7298bb96cdedd`; reviewed the uncommitted second-pass diff. Applied the local `.claude/skills/code-review/SKILL.md` Standards and Spec axes inline, as the shared brief explicitly requires no reviewer subagents. Standards sources: CODING_STANDARDS.md and AGENTS instructions. Spec: user deep-pass request and shared wave8-brief.md.

Standards: retained bodies are C89, preserve symbols and address order, use GameState fields and fixed-width types, and include documented layouts for recovered aggregates. Main API changes come from sync_protos; overlay additions append declarations/types only. SDK RECT is reused. The 0x14-byte workspace view is documented as a game packet view in the permitted overlay header. Matching-dependent group expansion, source-line scheduling, no-op index mask, scalar-range view and frame padding have FAKE annotations citing T-9560. No config/tooling/raw/asm changes, no commits. `git diff --check` passes. Final function and 27-unit verification remains pending until the last scratch experiment restores its source.

Spec: source-led iteration replaces the first-pass rejection-only approach. Ranked resolved differences first, repaired missing arguments/result paths and switch cases, recovered exact-copy/record types, and used five-minute permuter runs on near misses. Twelve additional verified matches (4,048 bytes) are retained so far. Remaining first-pass rejects have not all received deep type repair; this pass will stop only after the user's at-least-three-hour condition is met. Failed experiments are restored, with six new controlled T-0018 rows and appended updates to historical cases.

- Deep `RPG_BAT:func_8015A744`: approximately five-minute, two-worker permuter after source-led experiments. Draft source and temporary declarations restored after the run. Detailed outputs in scratchpad/w8-6/.

- Deep `RPG_BAT:func_8015A744`: 2 source variants, 1.9 seconds, DIFF. Each measurement uses funcdiff resolved instruction APIs on a freshly built object; a candidate is retained only after full funcdiff --resolve and unit SHA1. Rejected source/header edits restored.

### Second-pass final result and review

Completed the requested minimum: 2026-10-11 11:10:13–14:10:50 UTC (3 hours, 37 seconds), with 26 distinct functions receiving recorded source/assembly experiments. The list is not exhausted; the pass stops on the explicitly permitted three-hour condition. All failed source, API and aggregate experiments are restored to the pre-experiment versions.

Second-pass gain: **12 functions, 4,048 bytes**. Whole batch including the committed first pass: **15 functions, 5,092 bytes**. Retained second-pass functions:

| Unit | Function | Bytes |
| --- | --- | ---: |
| main | func_80073AD8 | 472 |
| RPG_BAT | func_80132FE0 | 296 |
| TACO | func_80140638 | 412 |
| RPG_BAT | func_80154430 | 492 |
| RPG_BAT | func_8014E780 | 372 |
| TACO | func_8013F874 | 332 |
| GEKO | func_801403A0 | 348 |
| main | week_day_main0 | 320 |
| main | func_8005DDB4 | 236 |
| TAIIKU | func_801330FC | 212 |
| RPG_BAT | func_8014F540 | 156 |
| BUNKA_SD | func_80139100 | 400 |

Final verification: twelve fresh full `funcdiff --resolve` matches, including the documented equivalent scratch PC16 references for two RPG_BAT functions; every match independently covered by the actual unit SHA1. Forced only the 27 checksum stamps and header/global guard stamps to rerun, then incremental `ninja -j2` without `-k`: **27 actual SHA1 OK results**, headers OK, globals OK. `sync_protos --check-branch`: zero new disagreements (17 known), branch check OK. `migrate_globals --check`: globals OK. Evidence: scratchpad/w8-6/deep-final-verify.log and build/w8-6/final-*.log, deep-final27-build.log, deep-final-progress.log. Owned-file scope and `git diff --check` pass; only the 17 authorized files differ from the committed first pass.

Inline review concluded: **Standards: zero unresolved findings. Spec: zero unresolved findings within the user's three-hour stopping condition.** Rechecked the final restored diff, fixed-width layouts, C89, canonical extern placement, FAKE annotations and verification evidence. No nonmatching drafts remain in the default build. The early unspecified result paths deliberately preserve the matching original ABI.

Six new second-pass T-0018 rows added (eleven historical/new rows for the full batch); the historical GEKO row is resolved. Top remaining non-T-0018 blocker: aggregate copy/frame and load/store scheduling after type recovery. Tool gaps: PC16 relocations are unresolved by native funcdiff; permuter type inference fails on some K&R parameters and local aggregate views; register_shapes has no positional function filter. No repository tools changed.

Final `ninja progress`: `grand total   4749/6958   734704/2279368  32.2%`. Second-pass changes remain uncommitted for the orchestrator.
