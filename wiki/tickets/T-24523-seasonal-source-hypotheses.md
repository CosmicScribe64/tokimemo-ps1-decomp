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

### GYOZI func_80140F90 verified candidate and review

Recovered a three-local typed loop from the older fifteen-local decompiler foundation. Full-unit resolved costs: initial typed 145; `b1 >= 1` 140; redundant unsigned-byte status conversion 20; sum-operand preservation 18; common `GyoziWork` state-byte accesses 10; integer debug-call view 0. Explicit flag temporaries, signed narrowing, return width variants and empty status tests did not solve the flag register difference and were discarded. No uninitialized-variable tricks retained.

The shared declaration is exactly `void func_800BCDF0();` before and after, now inside `#ifndef MAIN_API_OVERRIDE_func_800BCDF0`. This translation unit defines that guard with its result-lifetime reason and declares `s32 func_800BCDF0();`. Other callers retain the original canonical void view. This patch does not change any other main-exe prototype. GYOZI's existing `GyoziWork` gains offset-documented fields for the directly observed bytes; `func_80141550(void)` is declared in the owning overlay header. Source contains no scratch externs or raw main-address pointer arithmetic.

Docker `python3 tools/funcdiff.py --resolve --version default GYOZI:func_80140F90`: MATCH, privately linked function equals all 788 target bytes. Private `ninja -j1 build/ovl/GYOZI.ok` passes SHA1 `43f92523116e7b6cd772d4397712a52eeda89703`. `check_headers.py`, `migrate_globals.py --check`, and `sync_protos.py --check-branch` pass (0 new/15 known prototype disagreements). Parent standards/source review has no blockers: override convention, field offsets/padding, C89, documented FAKE conversion and operand-order identity, owned prototype, unchanged existing debug text. Self-review agrees. Final 27-hash refresh uses private GYOZI and SHOUGATU plus 25 unchanged baseline binaries.
