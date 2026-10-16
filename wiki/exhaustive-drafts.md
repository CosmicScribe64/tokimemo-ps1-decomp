---
type: research
updated: 2026-10-15
---

# Exhaustive remaining-function drafts (T-14500)

Frozen baseline: 48362009. The configured game-source census contains **1,066 INCLUDE_ASM functions / 866,424 progress bytes**, including **125 retained NON_MATCHING guards**. All units, including TT, are inventoried. SDK library assembly is outside this game-code census. Later independent wins must be reconciled before reporting new bytes.

`tools/exhaustive_drafts.py` runs one bounded subprocess at a time (120-second process-group timeout). Existing guarded C is retained; missing bodies use default m2c context, then mechanical type/declaration repair from existing bulk_m2c helpers. Each candidate compiles as the full translation unit with other guarded functions remaining assembly and is scored by resolved funcdiff rows. No mutation search or production-source changes occur. Zero is an unverified candidate.

`drafts/T-14500/` retains inventory, catalog, source/header baseline snapshots and per-function raw m2c C, cleaned body, declarations, full translation unit, diagnostics, hashes and outcome. No original assembly, executables, assets or raw game bytes enter the archive. Redundant compiler outputs are removed after each attempt.

Historical provenance contains 180 matching broad/family attempt rows. Their scores lack full current context, so fresh generation recovers omitted declarations; guarded drafts are reused directly.

Historical smoke validation: three full-TU zero candidates in BUNKAKEN. The initial serial run subsequently completed; this section records its starting evidence. Log: /private/tmp/t14500-run.log.

See [[tickets/T-14500-exhaustive-drafts]] and [[broad-coverage-results]].

## Census execution

Checkpoint: **1066/1,066 default attempts**; 937 compiled/scored drafts, 129 failures, 0 timeouts. 29 zero candidates are diagnostic scores, separate from production proof records. Initial outcomes remain immutable; externally owned type repairs and automatic retries use separate directories.

| Unit | Completed / total | Zero candidates | Failed / timeout |
|---|---:|---:|---:|
| BUNKAKEN | 25 / 25 | 12 | 5 |
| BUNKASAI | 20 / 20 | 6 | 3 |
| BUNKA_SD | 21 / 21 | 1 | 5 |
| DATE | 55 / 55 | 2 | 9 |
| DATE2 | 10 / 10 | 0 | 1 |
| ENDING | 15 / 15 | 0 | 1 |
| EN_NICHI | 19 / 19 | 0 | 4 |
| ETC | 45 / 45 | 0 | 8 |
| EVENT | 43 / 43 | 0 | 7 |
| GEKO | 38 / 38 | 0 | 4 |
| GYOZI | 37 / 37 | 0 | 6 |
| KANGEI | 17 / 17 | 3 | 1 |
| MASTER | 11 / 11 | 0 | 1 |
| NAME_ENT | 55 / 55 | 0 | 3 |
| OLH | 2 / 2 | 0 | 0 |
| OMIMAI | 3 / 3 | 0 | 0 |
| OPTION | 44 / 44 | 0 | 5 |
| RENSYU | 7 / 7 | 0 | 0 |
| RPG_BAT | 83 / 83 | 2 | 6 |
| SHOUGATU | 39 / 39 | 0 | 9 |
| SHUGAKU | 28 / 28 | 2 | 3 |
| TACO | 126 / 126 | 0 | 18 |
| TAIIKU | 81 / 81 | 0 | 11 |
| TEL | 5 / 5 | 0 | 2 |
| TT | 51 / 51 | 0 | 1 |
| VALEN | 6 / 6 | 0 | 0 |
| main | 180 / 180 | 1 | 16 |

The default census includes all guarded drafts and all game units. Failed generation or compilation is recorded rather than skipped; a failed entry does not yet constitute a compilable C draft. The post-pass targets these failures with bounded repairs. Nine independent DATE source/type repairs already compile with nonzero full-TU scores (separate artifacts from 1308ed4a); no new byte match follows from those repairs.

## Earlier publishing checkpoint

All **1,066/1,066 initial scopes have an explicit outcome**, with 29 zero candidates, 908 other compiled scores and 129 initial failures. All initial bodies/context/provenance are retained. Independent DATE/type/unaligned repairs recover 33 failures as compiling alternatives. The serial automatic repair remains active; 136 repair attempts are captured at this checkpoint.

Current quality snapshot: **843 compiling drafts without listed residual patterns; 139 compiling drafts requiring reconstruction review; 84 scopes without a compiling alternative.** Audit-clear is not proof of semantic equivalence. Residual placeholders, synthesized stack locals, scaled address arithmetic and generic copy layouts remain explicitly listed in `quality-audit.jsonl`. The all-usable-drafts target remains open.

`best-catalog.snapshot.jsonl` and `coverage-snapshot.json` pin this publication checkpoint while the live serial repair continues. Encoding analysis found ten generated literals with hexadecimal byte escapes; two have separately owned proof repairs. A tested hex-to-octal helper is retained, but no additional encoding sweep was started after the user requested immediate publication.

## Earlier stopping checkpoint, superseded by compilation repair

The authorized serial repair is finished and no further experiment was started. **All 1,066 initial scopes / 866,424 progress bytes have explicit outcomes; zero are unattempted.** Initial outcomes remain 29 zero candidates, 908 other compiled scores, and 129 failures. The three bounded automatic passes retained 257 attempts; 33 independently reconstructed alternatives plus automatic repairs recover **52** initial failures.

Compiler coverage now has **989 compiling/scored scopes** (30 diagnostic zero candidates and 959 nonzero scores), with **77 scopes still lacking a compiling alternative**. The separate quality audit selects **849 compiling drafts without listed residual patterns**, **140 compiling drafts needing reconstruction review**, and the same **77 uncompilable scopes**. Audit-clear does not prove semantic equivalence; the all-usable-drafts target remains incomplete. No draft-count increase is counted as matching bytes.

Restart from `drafts/T-14500/quality-audit.jsonl` and `best-quality-catalog.jsonl`, which list every remaining flag and complete selected context. `best-catalog.jsonl` preserves the compiler-score choices; quality choices can differ. `stopping-point.json` pins final counts. Existing attempts are immutable. Create a new, separately named repair alternative for a selected unresolved scope; never delete or overwrite its initial result to force a retry.

Read-only/resumable status commands in Docker:

```sh
tools/docker.sh python3 tools/exhaustive_drafts.py --out drafts/T-14500
tools/docker.sh python3 tools/exhaustive_repair.py
tools/docker.sh python3 tools/audit_drafts.py --archive drafts/T-14500
```

The first two commands reuse completed outcomes and do not repeat existing compiles. The audit refreshes quality metadata. Unresolved families include callback-table/stack layout recovery, scaled stack/GameState address expressions, missing aggregate views/prototypes, and decompiler placeholders. Eight unowned encoding cases remain noted; no encoding sweep ran after the publication/stop request. Production candidates require independent default literal linked proof and affected-unit SHA1.

## Resumed compiler completion (T15900)

The later user instruction to get all drafts compiling supersedes the earlier stopping checkpoint. The thirteen-scope mixed-overlay batch now has thirteen full-TU compiling alternatives at `drafts/T-14500/repairs/t15900`; every score was replayed with full diagnostics and preprocessed context saved. EVENT17D80 reuses current verified production source (zero already counted by integration); twelve alternatives remain nonzero. Other assigned repair batches continue, so this batch alone does not close the77-scope manifest. Compilation does not remove the residual semantic/type review requirement.

## Completed compilation repair

All **1,066 / 1,066** scopes in the frozen unmatched-function census now have a compiling full-translation-unit C alternative. The final 77 failures were repaired in T15700–T16000: 28 TACO/TAIIKU, 23 other overlays, 13 mixed overlays and 13 MAIN functions. Selected compiler outcomes are 31 diagnostic zero candidates and 1035 nonzero scores, with **zero missing or uncompilable scopes**. All 129 initial compiler failures now have compiling alternatives.

Compilation is separate from correctness and matching: the quality audit selects **901 drafts without listed residual patterns and 165 requiring reconstruction review**. Audit-clear does not establish semantic equivalence. Initial failed attempts remain immutable, and every final repair retains source/context/diagnostics and provenance. No additional verified production bytes are claimed from these compilation repairs.

Regenerate the final joined compiler and quality ledgers with `tools/docker.sh python3 tools/refresh_draft_ledgers.py`; this reads saved evidence without launching generation or compilation. The registry references T15700, T15800 and T16000 external archives plus T15900 within T14500. Read [[stopping-point-2026-10-15]] before resuming.

## Subsequent Sol medium quality pass

[[flagged-draft-pass]] records review of all165 flagged scopes,93 repaired alternatives and165 full-TU compiler proofs. Current quality selection is950 audit-clear/116 review-needed; all1066 scopes still compile. Audit logic unchanged; no production matching gain. Original attempts remain immutable.
