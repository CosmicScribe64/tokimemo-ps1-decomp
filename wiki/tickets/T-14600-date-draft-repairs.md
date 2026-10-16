---
id: T-14600
title: Repair nine DATE census draft failures
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-14500-exhaustive-drafts]]", "[[matching-notes]]"]
---

## Goal

Recover compiling C for nine reserved DATE census failures without editing production C or replacing the immutable initial attempts.

## Acceptance criteria

- [x] Retain alternative body, declarations, complete TU, provenance and full-TU comparison for every repair.
- [x] Classify every residual failure and difference; send any zero candidate to independent literal-byte proof.
- [ ] Standards review before Done.

## Notes

Frozen source base 48362009. Original archives are in the census worktree at drafts/T-14500/DATE. T-14100 historical callback-table drafts are reusable evidence, not matches. Docker only; no production C changes; hardlinked immutable disc/assembly inputs are not regenerated.

## Comments

## Results

All nine compile in Docker with the normal full-TU IDO 5.3 path. No zero scores; no production C changes or claimed matching coverage. Repaired alternatives are archived at drafts/T-14500/repairs/DATE; manifest and reproduction details at drafts/T-14500/repairs/README.md.

| Function | Difference rows | Repair |
| --- | ---: | --- |
| func_8013C190 | 288 | Recover stack array indexing and explicit first-word access to struct. |
| func_8013C410 | 114 | Recover 11x21 byte aggregate from 228-byte copy plus unaligned three-byte tail. |
| func_8013FD6C | 471 | Absolute-address mask symbol denotes integer constant; existing scalar union fields retained. |
| func_801449D4 | 312 | Reuse T-14100 449-u32.c real 76-callback aggregate; unsigned index historical 315-row draft. |
| func_80144E20 | 628 | Reuse T-14100 full typed 21-aggregate callback selector, historical 631-row draft. |
| func_801511B4 | 12 | Use existing TU FnTbl40 declaration and genuine 40-callback copy. |
| func_80152268 | 318 | Existing earlier unprototyped call has implicit int ABI; preserve s32 definition. |
| func_80152E34 | 152 | Existing earlier unprototyped call has implicit int ABI; preserve s32 definition. |
| func_80155004 | 66 | Recover genuine 53-callback table instead of undersized integer buffer. |

The callback dispatch func_801511B4 is the nearest at 12 diagnostic rows (frame size/table offset and register scheduling). Others retain broad instruction or allocation differences. Reused T-14100 drafts compare at 312/628 rows in the frozen census context, versus historical 315/631; neither is a match. C410 recovers the genuine 231-byte aggregate (eleven 21-byte rows); FD6C repairs integer mask arithmetic without changing scalar union fields. Earlier unprototyped calls require s32 return on 52268 and 52E34.

## Review

Self-check: all nine compiled and compared again serially; C89 aggregate/table types; no generated ASM edits; no assets or binaries in tracked changes. Independent standards review remains pending; ticket stays In Review.

## Integration archive audit
All nine archived body and context SHA256 values agree with their saved result manifests. Every entry has a nonempty full compiler context, provenance, comparison and diagnostics. All nine are explicitly nonmatching with positive scores. This checks archive integrity and reproducibility inputs; it does not promote drafts to production source or assert semantic equivalence.
