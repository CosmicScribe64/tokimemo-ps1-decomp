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
