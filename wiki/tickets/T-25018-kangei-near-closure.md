---
id: T-25018
title: Close or pin KANGEI func_8013852C local schedule
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: `[tickets/T-25016-candidate-reconstruction`, [[as1-scheduler-rule]], [[build-step-gaps]]]
---

## Goal

Reproduce the latest full-unit resolved baseline for KANGEI func_8013852C, then test only a new dependency-backed source hypothesis. Preserve an exact blocker if no justified source lever remains.

## Acceptance criteria

- [x] Full-unit NON_MATCHING C baseline reproduced.
- [x] Existing negatives checked before any experiment; at most three causal experiments.
- [x] Match promoted only with literal default C proof and affected checksum/declaration guards, or guarded source retained with exact blocker.
- [ ] Review against CODING_STANDARDS.md recorded before Done.

## Notes

Isolated codex-t25018-kangei worktree from 7c87f223e. Own src/ovl/KANGEI/80137B90.c only. No compiler changes, repeated sweeps, push, or integration. T-24901 already documents the local readiness gap (lui besttime 15, addu 16) and about 5,000 exhausted source variants.

## Comments

### Bounded result (2026-10-22)

No C promotion. Full-unit `tools/docker.sh python3 tools/funcdiff.py --resolve KANGEI:func_8013852C` explicitly compared NON_MATCHING C and reproduced only rows 35/36: expected `addu t6,t4,v1; lui at`, built `lui at; addu t6,t4,v1`. The initial isolated configure/ninja completed successfully (673 steps). Production body remains the assembly fallback.

Fresh capture with the existing `drafts/T-24900-as1/cap.py`, `cc_cap.py`, and `shim.py` scripts into ignored `build/t25018-cap/` reproduced the stock as1 DAG at preprocessed line 3344. Node 5 (`01837021`) and node 4 (`3c010000`, relocation 282) both have aftercycles 4 and the same source line. At time 16, node 4 has besttime 15 and node 5 has besttime 16, and node 4 is selected first. Their predecessors are the arithmetic chain (lbu node 8, sll node 7, subu node 6) and the $at/store chain (node 11/10 to node 4). This is the same local-scheduler cause previously measured in T-24901, not a global-mover choice.

Zero source variants were run: the T-24901 suggested narrowing/copy variants overlap the earlier recorded cast/temp sweeps; no new form with a demonstrated dependency change absent those notes was established. Spending the three-experiment ceiling on them would repeat known negatives. No compiler edits or permutations were attempted.

Exact escalation condition: return only with an evidenced C form that changes the arithmetic-chain or $at/store-chain DAG while preserving the final instruction stream, or separately justified compiler research. The current two-row guard is pinned with this ticket. The capture is diagnostic output and is not committed. Source body and all declarations remain unchanged.

Awaiting root review against CODING_STANDARDS.md; no Done claim.

### Verification

- Default `funcdiff --resolve --version default`: MATCH, literal linked function bytes equal target (180 bytes). This verifies retained assembly only, not a C match.
- After the source-note edit: `ninja build/ovl/KANGEI.ok`: SHA1 OK. An earlier command named nonexistent `config/checksums.sha1`; the actual configured checksum target above supplied the valid result.
- `check_headers.py`: headers OK.
- `migrate_globals.py --check`: globals OK.
- `sync_protos.py --check-branch`: 0 new disagreements, 15 known; branch check OK.
- `git diff --check`: clean.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
