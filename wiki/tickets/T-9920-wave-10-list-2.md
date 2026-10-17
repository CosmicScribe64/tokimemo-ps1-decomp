---
id: T-9920
title: "Wave 10: list 2"
status: Done
assignee: w10-2
created: 2026-10-11
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match functions in wave10-list-2.txt within the assigned original-object files.

## Acceptance criteria

- [ ] Complete a draft/build/diff first pass, followed by ranked deep attempts.
- [ ] Spend at least four hours, as explicitly requested for this batch.
- [ ] Verify retained C with resolved function diffs and 27 unit SHA1 checks.
- [ ] Pass shared-header/global checks and inline coding-standards review.
- [ ] Leave changes uncommitted for the orchestrator.

## Notes

Started 2026-10-11 20:19 UTC. Four-hour target: 2026-10-12 00:19 UTC.
Scratch: scratchpad/w10-2/ in the shared session scratchpad.
Follow wave10-brief.md; do not change tooling or object configurations.

## Comments

2026-10-11: Read wiki index, brief, standards, workflow, register-allocation
evidence and local agent instructions. Claimed the assigned list.

2026-10-11: Fresh Docker configure/ninja baseline passed all 27 unit SHA1 checks.
Read-only drafts and copies of earlier immutable farm inputs are in the owned
scratch directory; the shared farm and its containers remain untouched.

2026-10-11: Duplicate and near-duplicate tools, scoped to owned paths, applied
no new functions. First-pass compilation now builds the defining object and the
header/global guards per draft, with the complete unit build required before
retaining a match. Rejected drafts are restored immediately. Resolved comparisons
of standalone probes have extra object-end padding; diagnostics remove only
trailing zero rows, and every zero candidate still requires real source/object
diff plus full SHA1 verification before any match is counted.

The initial model-lifetime search tried 256 variants (best 13 core edits); DATE's
loop search tried 192 variants (best 8 core edits). A second search tests chained
assignments for DATE and pointer comparisons through the existing TkPl type for
TAIIKU. No compiler or build-pass change is proposed.

2026-10-11: First verified gain: EVENT func_8010A6E4, +172 bytes. Recovered
native bit-field access with separate read/write views; precise FAKE documents
the incomplete SDK aggregate. Real resolved diff and linked EVENT SHA1 pass.
Historical pass resumes after a safe pause, without repeating completed rows.

2026-10-17: Closing as superseded. The Codex run hit its usage limit with one function merged (EVENT func_8010A6E4, +172 bytes, still C on main); the rest of list 2 is covered by the T-14500 census and the later waves. Review: the FAKE comment gives reason and T-9920, but the function defines two local bit-field structs with a 0x3EC pad and reaches the field through D_800B0860 + 0x3EC instead of the typed root recorded under T-9770. That finding is part of the followup opened from T-9940.
