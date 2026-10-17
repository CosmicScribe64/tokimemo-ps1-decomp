---
id: T-20710
title: "True compiler-gap census and uniform allocator rule search"
status: In Progress
assignee: bs-gap-census agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[ido-52-evaluation]]", "[[uopt-register-allocation]]", "[[speedup-research-2026-10-16]]", "[[data/t0018-cases]]", "[[toolchain]]"]
---

## Goal

(1) Census: list the still-unmatched non-TT functions whose remaining difference is registers or instruction order
only, after the known source recipes have been tried (register-cheatsheet, T-20600 recipes, T-20610 truth tests,
T-20620 line grouping, implicit-int views through `regdiff`/`funcdiff --proto`). For each: the first divergence (uopt
colour, ugen temporary or as1 order), the allocator decision involved, and a minimal C reproducer where feasible.
Saved as `wiki/data/true-gaps.md`.

(2) Rule search: with the T-9800 traced uopt and the T-20610 offline method (dump cfe ucode once, replay, recompile
only files whose uopt input changes), look for one uniform tie-break or ordering difference in the allocator (or in
the 5.2/4.1 components) that explains several census cases and changes 0 matched functions. The eight T-9800
candidates are not repeated.

## Acceptance criteria

- [ ] `wiki/data/true-gaps.md` with one row per census function (first divergence, decision, reproducer if any)
- [ ] rule search results with counts (targets gained, matched functions changed) in the ticket and in
      [[uopt-register-allocation]] or [[ido-52-evaluation]]
- [ ] IDO 5.2/4.1 only from the recorded URLs, sha256 checked, kept in the gitignored `tools/local-compilers/`
- [ ] a build step only if it is one uniform rule, evidenced, keeps every matched function byte-identical
      (27/27 plus per-function compare), documented in [[toolchain]] and tested in `tools/`
- [ ] clean rebuild 27/27, check_headers, migrate_globals --check, sync_protos --check-branch

## Notes

Started 2026-10-17 01:48 UTC, worktree `bs-gap-census` from `w2-good`, time box about 3 hours.

## Comments
