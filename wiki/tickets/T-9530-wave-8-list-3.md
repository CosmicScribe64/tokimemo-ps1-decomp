---
id: T-9530
title: "Wave 8: list 3"
status: Done
assignee: w8-3
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Work through the 136 functions in wave8-list-3.txt, within the 36 owned C files.

## Acceptance criteria

- [x] Every listed function matched or time-boxed and restored to INCLUDE_ASM.
- [x] Matches verified with Docker funcdiff and all 27 linked-unit SHA1 checks.
- [x] Shared declarations and GameState access checks pass.
- [x] Inline review against CODING_STANDARDS.md completed; findings resolved.

## Notes

Follow the shared wave-8 brief. No commits, merges, pushes, tooling edits, or unowned C edits.

## Comments

2026-10-11: Read the wiki index, workflow, standards, wave-8 brief, and T-9410 register-allocation findings. Claimed list 3.

## Results

9 functions / 4,040 bytes adopted. All 136 functions received a fresh, explicit-path m2c attempt in list order; recoverable drafts were probed with the current Docker compiler. Attempts were bounded (90-second draft/compiler limits; 60-second permutation runs on selected near misses). Non-matches remain INCLUDE_ASM; no failed C or trial padding was retained.

| Unit/file | Function | Bytes |
|---|---|---:|
| GYOZI/8013D470 | `func_8013D764` | 288 |
| TAIIKU/80133C80 | `func_801345A4` | 344 |
| KANGEI/80137B90 | `func_80138D40` | 252 |
| TAIIKU/80133C80 | `func_80134D94` | 516 |
| TACO/80151C30 | `func_80153774` | 812 |
| GYOZI/8013C3B0 | `func_8013CB18` | 452 |
| RPG_BAT/80141C30 | `func_80141C70` | 456 |
| RPG_BAT/80141C30 | `func_80141E38` | 456 |
| RPG_BAT/8013E300 | `func_8013E300` | 464 |

Eleven observed register retries were appended to [[data/t0018-cases]]. Other time boxes need record/table/type recovery or broader C/control-flow changes; they are not labelled impossible or attributed to T-0018 without evidence. Own-data/layout work requiring config YAML edits is outside this batch's allowed edits.

Scratch evidence: the shared scratchpad `w8-3/` directory, including `probes-refined-0.jsonl`, `probes-45.jsonl`, `probes-75.jsonl`, `probes-105.jsonl`, `custom-*.jsonl`, and the three `permute-*.log` files. The JSONL records contain draft text, status, compiler diagnostics, and resolved instruction differences.

Tooling findings: m2c omitted argument 3 (`500`) of RPG_BAT `func_8014B738` despite its three-parameter matched definition. The ENDING permutation search emitted internal AST type-resolution failures on implicit renamed callees, but continued and stopped at its limit. Scoped duplicate/near-duplicate scans found no applicable copies. They classify matches from assembly directories and still printed the baseline 4734 after configure, while current source progress is 4743.

## Attempt manifest

Each non-match below was time-boxed. Category totals: T-0018 retry: 11; Draft record/table/control recovery: 58; C/code generation difference: 28; Type/declaration recovery: 30; Matched: 9.

| # | Owned file | Function | Outcome / remaining work |
|---:|---|---|---|
| 1 | EVENT/800F9680 | `func_800FA194` | T-0018 retry |
| 2 | EVENT/800F9680 | `func_800FA0D0` | T-0018 retry |
| 3 | ENDING/801396E0 | `func_80139B48` | Draft record/table/control recovery |
| 4 | TT/80148DB0 | `func_80149B20` | C/code generation difference |
| 5 | TAIIKU/80133C80 | `func_80136B18` | T-0018 retry |
| 6 | GEKO/80142760 | `func_80144198` | Type/declaration recovery |
| 7 | SHUGAKU/80132000 | `func_801322F0` | T-0018 retry |
| 8 | EVENT/800F9680 | `func_800FAA04` | Draft record/table/control recovery |
| 9 | TT/80148DB0 | `func_80149400` | Draft record/table/control recovery |
| 10 | BUNKA_SD/80134260 | `func_80134418` | C/code generation difference |
| 11 | TAIIKU/80133C80 | `func_801368B8` | Type/declaration recovery |
| 12 | EVENT/800F9680 | `func_800FA25C` | Type/declaration recovery |
| 13 | TT/80148DB0 | `func_8014A814` | T-0018 retry |
| 14 | GYOZI/80136220 | `func_80137188` | Type/declaration recovery |
| 15 | GYOZI/80136220 | `func_80136488` | Type/declaration recovery |
| 16 | SHUGAKU/80132000 | `func_80132BCC` | C/code generation difference |
| 17 | TAIIKU/80133C80 | `func_80137228` | C/code generation difference |
| 18 | ENDING/801396E0 | `func_80139EC4` | T-0018 retry |
| 19 | TAIIKU/80133C80 | `func_80136DA4` | Type/declaration recovery |
| 20 | TACO/8015D0D0 | `func_8015D0D0` | Type/declaration recovery |
| 21 | TT/80148DB0 | `func_80149EA0` | C/code generation difference |
| 22 | TT/80144D70 | `func_80145370` | T-0018 retry |
| 23 | ENDING/80133C10 | `func_80134B18` | C/code generation difference |
| 24 | TAIIKU/80133C80 | `func_80138680` | Draft record/table/control recovery |
| 25 | TT/8013E290 | `func_8013E498` | Draft record/table/control recovery |
| 26 | TT/801487D0 | `func_801487D0` | Draft record/table/control recovery |
| 27 | EVENT/800F9680 | `func_800FA6A0` | C/code generation difference |
| 28 | TAIIKU/80133C80 | `func_80138800` | T-0018 retry |
| 29 | TAIIKU/80133C80 | `func_80137090` | Draft record/table/control recovery |
| 30 | TAIIKU/80133C80 | `func_80135DF8` | Draft record/table/control recovery |
| 31 | TT/8013E850 | `func_8013EC64` | Draft record/table/control recovery |
| 32 | RPG_BAT/80144010 | `func_80144480` | C/code generation difference |
| 33 | TAIIKU/80133C80 | `func_80137440` | Type/declaration recovery |
| 34 | 8004E500 | `func_8004F4F0` | Draft record/table/control recovery |
| 35 | TT/80148DB0 | `func_801498C8` | C/code generation difference |
| 36 | ENDING/801396E0 | `func_8013B510` | T-0018 retry |
| 37 | TAIIKU/80133C80 | `func_80135D24` | Draft record/table/control recovery |
| 38 | BUNKA_SD/80132000 | `func_80132708` | C/code generation difference |
| 39 | TT/80148DB0 | `func_80149108` | C/code generation difference |
| 40 | TT/80148DB0 | `func_80148E7C` | Draft record/table/control recovery |
| 41 | TT/80148DB0 | `func_80149704` | Draft record/table/control recovery |
| 42 | TACO/80151C30 | `func_801522B0` | T-0018 retry |
| 43 | TAIIKU/80133C80 | `func_80137C44` | Draft record/table/control recovery |
| 44 | ENDING/801396E0 | `func_80139F3C` | C/code generation difference |
| 45 | SHOUGATU/801330C0 | `func_8013315C` | Draft record/table/control recovery |
| 46 | TT/80148DB0 | `func_801491F0` | Draft record/table/control recovery |
| 47 | TT/80148DB0 | `func_8014AB48` | C/code generation difference |
| 48 | TACO/80151C30 | `func_80152808` | Type/declaration recovery |
| 49 | TACO/80139170 | `func_8013A580` | Type/declaration recovery |
| 50 | 8004C3F0 | `srn_set` | Draft record/table/control recovery |
| 51 | TAIIKU/80133C80 | `func_80135480` | Draft record/table/control recovery |
| 52 | DATE/8014CC40 | `func_8014DCCC` | Draft record/table/control recovery |
| 53 | TAIIKU/80133C80 | `func_80136EC0` | Draft record/table/control recovery |
| 54 | GYOZI/80136220 | `func_801362A0` | Draft record/table/control recovery |
| 55 | TAIIKU/80133C80 | `func_80135830` | Draft record/table/control recovery |
| 56 | TACO/801480B0 | `func_80148B58` | Type/declaration recovery |
| 57 | TAIIKU/80133C80 | `func_80137EA8` | Draft record/table/control recovery |
| 58 | TT/80145E60 | `func_80146000` | Draft record/table/control recovery |
| 59 | EVENT/800F9680 | `func_800F9730` | Draft record/table/control recovery |
| 60 | 80049FF0 | `func_80049FF0` | C/code generation difference |
| 61 | 8004E500 | `load_string` | Draft record/table/control recovery |
| 62 | TAIIKU/80133C80 | `func_801365C8` | Draft record/table/control recovery |
| 63 | SHOUGATU/801330C0 | `func_80133760` | Type/declaration recovery |
| 64 | GYOZI/8013D470 | `func_8013D764` | Matched |
| 65 | TT/80148DB0 | `func_80149BB0` | C/code generation difference |
| 66 | TACO/80137A60 | `func_80137D00` | Type/declaration recovery |
| 67 | TACO/80151C30 | `func_80152C60` | C/code generation difference |
| 68 | ENDING/801396E0 | `func_8013A004` | Draft record/table/control recovery |
| 69 | TAIIKU/80133C80 | `func_801345A4` | Matched |
| 70 | KANGEI/80137B90 | `func_80138D40` | Matched |
| 71 | TACO/801480B0 | `func_801483B0` | Type/declaration recovery |
| 72 | TAIIKU/80133C80 | `func_80135AD0` | Draft record/table/control recovery |
| 73 | BUNKA_SD/80132000 | `func_801322EC` | Type/declaration recovery |
| 74 | GYOZI/80134520 | `func_801347D4` | C/code generation difference |
| 75 | TACO/801480B0 | `func_801487A4` | Draft record/table/control recovery |
| 76 | TT/80148DB0 | `func_801494D8` | Draft record/table/control recovery |
| 77 | SHUGAKU/80132000 | `func_8013243C` | C/code generation difference |
| 78 | TACO/80133870 | `func_801342CC` | Type/declaration recovery |
| 79 | TACO/801480B0 | `func_80148184` | Type/declaration recovery |
| 80 | TT/8013E290 | `func_8013E6A0` | Draft record/table/control recovery |
| 81 | TACO/801480B0 | `func_801484E4` | Type/declaration recovery |
| 82 | TAIIKU/80133C80 | `func_80137784` | Draft record/table/control recovery |
| 83 | TACO/801480B0 | `func_80148D6C` | C/code generation difference |
| 84 | TACO/80139170 | `func_8013AD64` | Type/declaration recovery |
| 85 | 8004E500 | `func_8004E58C` | Type/declaration recovery |
| 86 | TAIIKU/80133C80 | `func_80134D94` | Matched |
| 87 | TACO/80133870 | `func_801345D4` | Type/declaration recovery |
| 88 | TACO/801430B0 | `func_801430D0` | Draft record/table/control recovery |
| 89 | 80049FF0 | `func_8004A1A0` | Draft record/table/control recovery |
| 90 | TACO/80151C30 | `func_80153774` | Matched |
| 91 | TACO/80139170 | `func_8013920C` | T-0018 retry |
| 92 | TAIIKU/80133C80 | `func_80137544` | C/code generation difference |
| 93 | 8004C3F0 | `get_fnt` | Draft record/table/control recovery |
| 94 | SHOUGATU/801330C0 | `func_80133B44` | Draft record/table/control recovery |
| 95 | TACO/80151C30 | `func_80151D1C` | Type/declaration recovery |
| 96 | GYOZI/8013C3B0 | `func_8013CB18` | Matched |
| 97 | TACO/801430B0 | `func_801432F0` | Draft record/table/control recovery |
| 98 | TACO/80139170 | `func_8013A790` | Draft record/table/control recovery |
| 99 | 8004C3F0 | `mod_trans_vram` | Draft record/table/control recovery |
| 100 | TACO/80151C30 | `func_801523D0` | C/code generation difference |
| 101 | TACO/80151C30 | `func_80152DB4` | C/code generation difference |
| 102 | TACO/801447D0 | `func_801449D8` | Draft record/table/control recovery |
| 103 | 8004C3F0 | `srn_vram_set` | Draft record/table/control recovery |
| 104 | BUNKA_SD/80132000 | `func_80132148` | Type/declaration recovery |
| 105 | RPG_BAT/80144010 | `func_80144010` | Draft record/table/control recovery |
| 106 | TAIIKU/80133C80 | `func_80133CFC` | Draft record/table/control recovery |
| 107 | 8004C3F0 | `mod_set` | Type/declaration recovery |
| 108 | TT/80145E60 | `func_80146348` | Draft record/table/control recovery |
| 109 | TACO/801447D0 | `func_80145DC4` | Type/declaration recovery |
| 110 | 80049FF0 | `func_8004A414` | Draft record/table/control recovery |
| 111 | 80049FF0 | `func_8004A814` | C/code generation difference |
| 112 | TACO/801447D0 | `func_8014616C` | Type/declaration recovery |
| 113 | TT/80144D70 | `func_80144D70` | Draft record/table/control recovery |
| 114 | TACO/801447D0 | `func_80146754` | C/code generation difference |
| 115 | 8004E500 | `disp_string` | C/code generation difference |
| 116 | TAIIKU/80133C80 | `func_80138224` | Type/declaration recovery |
| 117 | TACO/80139170 | `func_80139424` | Draft record/table/control recovery |
| 118 | SHUGAKU/80133430 | `func_801337FC` | C/code generation difference |
| 119 | TACO/801447D0 | `func_80145650` | Draft record/table/control recovery |
| 120 | SHUGAKU/80132000 | `func_8013282C` | C/code generation difference |
| 121 | TACO/80139170 | `func_80139BCC` | Type/declaration recovery |
| 122 | DATE/8014CC40 | `func_8014D298` | Draft record/table/control recovery |
| 123 | TACO/80139170 | `func_80139FB8` | Type/declaration recovery |
| 124 | KANGEI/80137B90 | `func_80138F88` | Type/declaration recovery |
| 125 | TAIIKU/80133C80 | `func_80135F3C` | Draft record/table/control recovery |
| 126 | RPG_BAT/80141C30 | `func_80141C70` | Matched |
| 127 | RPG_BAT/80141C30 | `func_80141E38` | Matched |
| 128 | TAIIKU/80133C80 | `func_801346FC` | Draft record/table/control recovery |
| 129 | RPG_BAT/8013E300 | `func_8013E300` | Matched |
| 130 | BUNKA_SD/80132000 | `func_80132DFC` | C/code generation difference |
| 131 | TACO/801447D0 | `func_80144E3C` | Draft record/table/control recovery |
| 132 | TT/8013E850 | `func_8013E850` | Draft record/table/control recovery |
| 133 | BUNKA_SD/80132000 | `func_80132FF0` | Draft record/table/control recovery |
| 134 | 8004C3F0 | `mod_set_gpu` | Draft record/table/control recovery |
| 135 | TT/80144D70 | `func_801455C8` | Draft record/table/control recovery |
| 136 | TT/80148DB0 | `func_8014A2B4` | Draft record/table/control recovery |

## Review findings and fixes

- Removed the obsolete KANGEI `func_80138D40` NON_MATCHING alternative after its array-access version matched.
- Marked the redundant byte-lvalue cast in GYOZI `func_8013D764` FAKE: it keeps the byte store and call argument in a0; original spelling unknown. This is the only new fakematch.
- No trial padding, unused locals, pointer tricks from failed probes, or prototype overrides were retained.

## Verification and inline review

2026-10-11: reviewed the uncommitted diff against HEAD with the local `.claude/skills/code-review/SKILL.md` standards/spec axes, inline as required by the shared wave-8 brief.

Standards: all findings resolved. C89 declarations, fixed-width types, symbol placeholders, existing aggregate/bit-field views, source order, and generated main API declarations checked. The one new FAKE is labelled and tracked above. No SDK types, compiler passes, tools, config YAML, raw sources, unowned C, or staged files changed. Repeated scene switches and unknown names are preserved for byte matching, as the project standards require.

Spec: all 136 entries matched or explicitly time-boxed, with nine retained functions / 4,040 bytes. Failed candidates stay INCLUDE_ASM; scratch drafts remain outside the worktree. Owned-file duplicate and near-duplicate scans found no applicable copies. No commits, merges, pushes, or reviewer sub-agents.

Final Docker incremental ninja succeeds. All 27 current binary SHA1 values were then checked against config hashes: 27/27 OK, including actual string and jump-table contents. All nine functions pass resolved funcdiff; the two review-touched functions were rechecked after corrections. `build/headers.ok` and `build/globals.ok` exist. `sync_protos.py --check-branch`: zero new disagreements (17 known baseline); `migrate_globals.py --check`: globals OK. `git diff --check` passes; staging is empty.

Progress: `grand total   4743/6958   733632/2279368  32.2%`. Evidence logs in scratch `w8-3/`: `resolved-batch.log`, `final-build.log`, `final-guards.log`, `review-build.log`, `final-proof.log`, `dupes-regenerated.log`, `neardupes-initial.log`.
