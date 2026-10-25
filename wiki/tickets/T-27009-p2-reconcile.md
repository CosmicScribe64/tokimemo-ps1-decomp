---
id: T-27009
title: Reconcile phase 2 proposal names and executable batches
status: Done
assignee: sol-6.1
created: 2026-10-24
updated: 2026-10-25
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Reconcile reviewed unit proposals with the names actually applied by T-27003/T-27004, then prepare collision-free rename batches. Begin final reconciliation after the missing analysis units are reviewed.

## Acceptance criteria

- [x] Shared-core applied ledger names take precedence over stale proposals; remaining conflicts resolved from ownership and source evidence.
- [x] No conflicting (kind, scope, old) keys or target-name collisions, including types/macros visible through header closures.
- [x] Separate executable rename tables from comment requests, enum proposals, deferred rows and file/header batches. `rename.py` accepts neither `comment` rows nor proposed enum-group literals as ordinary rename rows.
- [x] Dry-run each supported batch with `rename.py plan`; review before application, and keep file/dir renames separate.
- [x] Specs use reconciled names; decisions and rejected hypotheses remain documented.
- [x] Independent standards/spec review before Done.
## Notes

2026-10-24 baseline audit (Docker, Python standard library): 24 TSV files representing 21 unit analyses plus supplemental DATE/EVENT tables; 10,085 rows. Every non-comment row has five tab-separated columns. 135 exact keys have multiple proposed names; four same-kind/same-scope target names refer to different old names. This is an initial count, not a complete C-namespace collision check. Row kinds: func 4,219; data 1,944; field 974; type 328; const 2,007; comment 331; file 281; dir 1. No proposals were applied.

Historical core proposals use names such as `snd_command` and `bustup_read_step`; the applied ledger/current declarations are authoritative. Some apparent duplicates in supplemental tables are deliberately deferred or cross-unit requests and must not silently become executable rows.

## Comments

2026-10-24: Ready for the reconciliation pass after missing analyses; baseline audit recorded during continuation.

2026-10-24 continuation: config/renames/school-day.txt is now an applied, reviewed ledger; its school_day_ names override the old m-weekday proposal aliases. Four new analysis drafts remain In Progress. GEKO has 975 active rows; TAIIKU38, SHOUGATU/GYOZI15 and TEL/OPTION15. Their structural validity is not application approval.

2026-10-24 scheduler reconciliation: prepared `config/renames/proposals/m-scheduler-apply.tsv` (40 rows: 33 remaining owned placeholder functions, the non-O.BIN `special_event_select`, six owned globals). Read every selected function body; normalized prefixes to `schedule_`/`day_event_`, retained applied-core and O.BIN names, deferred cross-unit globals/fields/comments/constants/file work. `D_800B6740` tracks only the last selector result, so the proposal's whole-week no-repeat claim was corrected. [[spec/reconcile]] records ownership, current-name lookup evidence, deferrals and the next-unit recipe. Final Docker plan after name refinements exited 0 (40 rows, 10 files); awaiting independent review. No source, header or symbol file edited; no application or commit.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

