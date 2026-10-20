---
id: T-24523
title: Match seasonal overlay tails using new source hypotheses
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: []
---

## Goal

Match remaining GYOZI, VALEN, and SHOUGATU functions from baseline dba33c4f5 in an isolated worktree. Avoid exhausted scheduler/type grids and compiler patches.

## Acceptance criteria

- Literal default C matches verified with private linked byte proof, 27 binary checksums and declaration guards.
- Standards review resolves findings before Done.

## Notes

Scope: 16 remaining functions. Source/loop/aggregate hypotheses take priority over repeated broad grids. Docker image tokimemo-decomp-arm64, one compiler worker.

## Comments

- Started with wiki/playbook, prior function notes and standards review. No new matches yet.

### SHOUGATU func_80133B44 candidate

A signed-halfword cast of the first string-width product lowers full-unit resolved score from 82 to zero. The conversion is redundant before the surrounding signed-halfword narrowing, so as1 deletes its instructions while ugen spends two temporary slots. All later ring assignments match as a consequence. Neither u8/u16/u32/s32 variants nor s8 variants match; s8 gives 7/8 rather than zero. The two inherited unused scalar slots are now explicitly documented FAKE frame padding. Default fallback removed only for this function. Literal linked proof and full 27 binary checks underway.

GYOZI func_80140E0C: a bounded 24-variant alias-store grid (plain/byte/signed rereads; zero through byte or signed store, redundant store/re-read chains, xor-self, &=0, source from the earlier flag) leaves the base resolved score 9 unchanged or worse. No source change retained for this target.

### Verified match and review

SHOUGATU `func_80133B44`: **408 bytes matched** by literal default C with no INCLUDE_ASM fallback. Docker `python3 tools/funcdiff.py --resolve --version default SHOUGATU:func_80133B44` reports `MATCH (linked function bytes equal target (408 bytes))`. Private `ninja -j1 build/ovl/SHOUGATU.ok` passes SHA1 `d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9`.

All 27 binary hashes pass against committed config hashes: the modified SHOUGATU binary is privately rebuilt here; the other 26 use unchanged verified binaries from the root checkout mounted read-only at `/baseline`. This is explicit checksum provenance, not a claim of recompiling all 27 in this lane. A full fresh build was stopped at the coordinator's request to prioritize the private target proof. The root integration independently repeats full verification.

`check_headers.py`: headers OK. `migrate_globals.py --check`: globals OK; four TACO generated original-asm proof files use individual symlinks to the read-only baseline, DATE's proof was generated locally. `sync_protos.py --check-branch`: 0 new disagreements, 15 known, branch check OK. `git diff --check` passes.

Parent standards/source review: no blockers. C89 declarations, unchanged existing strings, no assembly/config/header/compiler edits, FAKE padding and redundant narrowing documented. Product narrowing retained as the modulo-equivalent form. Self-review agrees. Remaining 15 seasonal targets continue under this ticket.
