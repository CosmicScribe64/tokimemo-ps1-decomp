---
id: T-27009
title: Reconcile phase 2 proposal names and executable batches
status: Ready
assignee:
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[rename-tool]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Reconcile reviewed unit proposals with the names actually applied by T-27003/T-27004, then prepare collision-free rename batches. Begin final reconciliation after the missing analysis units are reviewed.

## Acceptance criteria

- [ ] Shared-core applied ledger names take precedence over stale proposals; remaining conflicts resolved from ownership and source evidence.
- [ ] No conflicting (kind, scope, old) keys or target-name collisions, including types/macros visible through header closures.
- [ ] Separate executable rename tables from comment requests, enum proposals, deferred rows and file/header batches. `rename.py` accepts neither `comment` rows nor proposed enum-group literals as ordinary rename rows.
- [ ] Dry-run each supported batch with `rename.py plan`; review before application, and keep file/dir renames separate.
- [ ] Specs use reconciled names; decisions and rejected hypotheses remain documented.
- [ ] Independent standards/spec review before Done.

## Notes

2026-10-24 baseline audit (Docker, Python standard library): 24 TSV files representing 21 unit analyses plus supplemental DATE/EVENT tables; 10,085 rows. Every non-comment row has five tab-separated columns. 135 exact keys have multiple proposed names; four same-kind/same-scope target names refer to different old names. This is an initial count, not a complete C-namespace collision check. Row kinds: func 4,219; data 1,944; field 974; type 328; const 2,007; comment 331; file 281; dir 1. No proposals were applied.

Historical core proposals use names such as `snd_command` and `bustup_read_step`; the applied ledger/current declarations are authoritative. Some apparent duplicates in supplemental tables are deliberately deferred or cross-unit requests and must not silently become executable rows.

## Comments

2026-10-24: Ready for the reconciliation pass after missing analyses; baseline audit recorded during continuation.
