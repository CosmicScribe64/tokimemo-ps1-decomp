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
