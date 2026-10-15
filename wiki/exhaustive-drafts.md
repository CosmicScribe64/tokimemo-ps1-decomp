---
type: research
updated: 2026-10-15
---

# Exhaustive remaining-function drafts (T-14500)

Frozen baseline: 48362009. The configured game-source census contains **1,066 INCLUDE_ASM functions / 866,424 progress bytes**, including **125 retained NON_MATCHING guards**. All units, including TT, are inventoried. SDK library assembly is outside this game-code census. Later independent wins must be reconciled before reporting new bytes.

`tools/exhaustive_drafts.py` runs one bounded subprocess at a time (120-second process-group timeout). Existing guarded C is retained; missing bodies use default m2c context, then mechanical type/declaration repair from existing bulk_m2c helpers. Each candidate compiles as the full translation unit with other guarded functions remaining assembly and is scored by resolved funcdiff rows. No mutation search or production-source changes occur. Zero is an unverified candidate.

`drafts/T-14500/` retains inventory, catalog, source/header baseline snapshots and per-function raw m2c C, cleaned body, declarations, full translation unit, diagnostics, hashes and outcome. No original assembly, executables, assets or raw game bytes enter the archive. Redundant compiler outputs are removed after each attempt.

Historical provenance contains 180 matching broad/family attempt rows. Their scores lack full current context, so fresh generation recovers omitted declarations; guarded drafts are reused directly.

Initial smoke validation: three full-TU zero candidates in BUNKAKEN. Complete serial run in progress; no new verified bytes claimed. Log: /private/tmp/t14500-run.log.

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

## Publishing checkpoint

All **1,066/1,066 initial scopes have an explicit outcome**, with 29 zero candidates, 908 other compiled scores and 129 initial failures. All initial bodies/context/provenance are retained. Independent DATE/type/unaligned repairs recover 33 failures as compiling alternatives. The serial automatic repair remains active; 136 repair attempts are captured at this checkpoint.

Current quality snapshot: **843 compiling drafts without listed residual patterns; 139 compiling drafts requiring reconstruction review; 84 scopes without a compiling alternative.** Audit-clear is not proof of semantic equivalence. Residual placeholders, synthesized stack locals, scaled address arithmetic and generic copy layouts remain explicitly listed in `quality-audit.jsonl`. The all-usable-drafts target remains open.

`best-catalog.snapshot.jsonl` and `coverage-snapshot.json` pin this publication checkpoint while the live serial repair continues. Encoding analysis found ten generated literals with hexadecimal byte escapes; two have separately owned proof repairs. A tested hex-to-octal helper is retained, but no additional encoding sweep was started after the user requested immediate publication.
