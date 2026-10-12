---
id: T-10100
title: Wave 9/10 tool fixes
status: Done
assignee: r11-fixes
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Repair global declaration registration, m2c argument recovery, permuter setup limits,
farm worker lifetime, and preserve whole header blocks in three-way merges.

## Acceptance criteria

- [x] Each reported failure has synthetic unit coverage and passes in Docker.
- [x] Header merge tool preserves ours and appends new other-side blocks before the guard.
- [x] Clean build, header/global guards, branch prototype check and progress pass.
- [x] Review against CODING_STANDARDS.md and user requirements; resolve findings.

## Notes

Scope: tools/, tests and wiki only. Leave edits uncommitted for orchestrator review.
Wave-9 list-8 evidence includes GEKO a1 saved in a jal delay slot, RPG_BAT
explicit third arguments, and a stale high-address value passed to k_disp_switch.

## Comments

2026-10-11: Read shared tooling11 brief and project standards. Claimed ticket.

2026-10-11: In Review. Full Docker suite: 644 tests, OK (two existing skips).
Final targeted reruns: 41 m2c tests, 73 permuter/farm tests, ten header merge
tests, 69 prototype tests. Clean Docker configure/ninja: 27/27 SHA1 checks;
build/headers.ok and build/globals.ok exist. Prototype branch check/progress pending.

Generated original assembly confirms GEKO func_80133024 -> func_80132D04 takes
two words; four RPG_BAT dialogue callers -> func_8014B738 take three. New synthetic
coverage guards against intermediate a2/a3 index values and lone high bases.
The reported universal add-global rejection could not be reproduced on HEAD;
all six tested name/type combinations succeed through the CLI. No include/src edits.

2026-10-11: Inline review of the uncommitted diff against HEAD, CODING_STANDARDS.md
and all five user requirements passed. Applied the local code-review skill's
standards/spec axes inline as the shared brief requires. Findings resolved:
helper process groups on setup timeout, consumed caller indexing intermediates,
exact contiguous block duplicate checks, include guards with comments/values,
and the reference's other-side header deletion behavior. Temporary Git tests
trust only their own disposable repo through environment config. No outstanding
findings, new toolchain emulation passes, C/header changes, game data, or commits.

Branch check: zero new disagreements, 17 known. Progress: 4982/6958 functions,
836940/2279368 bytes (36.7%); zero function gains. Build/check logs are under
scratchpad/r11-fixes/.

2026-10-11: Done after inline review and final verification. Reloaded full suite:
649 tests passed (two existing skips, tests-verified.log). Final isolated header
suite: 11 tests passed, including other-side deletion and both-side deletion
(header-final.log). Final m2c suite: 41 tests passed; permuter/farm: 73; prototype:
69. All 27 clean-build checksums, header/global sentinels, branch check and the
4982-function progress floor pass. git diff --check passes. Changes are confined
to tools/tests/wiki, left uncommitted for the orchestrator. The universal global
name rejection was not reproducible on this checkout; exact example/type CLI
tests pass and registration no longer scans sources.
