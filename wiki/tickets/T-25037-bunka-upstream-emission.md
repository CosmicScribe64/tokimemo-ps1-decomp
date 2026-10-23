---
id: T-25037
title: Match BUNKA_SD year-load scheduling through upstream emission
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-22
links: [[[tickets/T-25023-bunka-scheduler-closure]], [[as1-scheduler-rule]]]
---

## Goal

Match BUNKA_SD func_80139290 by recovering a C form that emits the year-byte load before store-address preparation while retaining the compare-chain delay-slot constants and register allocation.

## Acceptance criteria

- [x] Establish upstream uopt/ugen evidence before new causal source tests.
- [x] Literal NON_MATCHING C match followed by default-C proof, affected checksum and declaration checks.
- [x] Review against CODING_STANDARDS.md before Done (root review; integration pending).

## Notes

Isolated worktree /private/tmp/tokimemo-t25037, branch codex/t25037-bunka, base 242c0c3a3. Sole source ownership src/ovl/BUNKA_SD/801388D0.c. Existing Docker image only. Prior T-25020/T-25023 negatives are binding experiment history; no repeat of broad grouping, volatile or real-local grids. No compiler exceptions, inline assembly or instruction patches.

## Comments

2026-10-22: Claimed the remaining four-position draft. The known sufficient IR arrangement is evidence of feasibility, not a C solution. Investigation starts by capturing uopt output and mapping the emission of store-value constants and the duplicated year-memory expression.

### Optimized ucode baseline and first causal tests

Docker full-TU baseline reproduces 379 words and four changed positions (166/167/171/172). Captured normal ugen input through a scratch copying wrapper; production passes and flags are unchanged. The 0x49/0x66 arm ucode emits constant store then `rldc` 0x60 then `rlod` year at the exit. The existing 0x3B empty test moves `rlod` to arm entry before its store, retaining the later constant preload.

Tests saved only under ignored build/t25037: an early empty comparison with 0x60 in arm 0x49 emits both entry preloads, but in year-before-constant order; it also leaves an extra comparison path (381 words, cost18). Reversing the comparison operands has identical output. An early empty comparison with 1 in arm 0x66 keeps379 words but still emits year first; the global mover takes that address (cost6). These reject the simple hypothesis that an explicit comparison can make the required constant precede the year preload. No source changes retained.

### Case 0x49 closed without collateral instruction changes

Pinned upstream `uoptemit.c` shows two emission paths: block-entry reloads are emitted in ascending register order (`emit_rstrs`), whereas edge reloads use the linked `rlodbb` list (`emit_rlods`). This explains why reading both values early cannot reverse t0-before-t2 entry order. A separate empty `g->unk_1104.u == 0x60` test creates the constant's preload in an earlier block; a subsequent year test creates the year preload in the next block. A plain year condition leaves an unwanted branch after as1 short-cutting (381 words, cost14).

Replacing that second test with `if ((u32)g->unk_03E < 0) {}` retains uopt's preload placement, but its unsigned-false comparison folds later. Actual whole-TU result is **379/379 words, cost2, only positions171/172**, all of case0x49 and its head filler matching. Equivalent >=0, >UINT_MAX, and assigning the false comparison to the zero store also cost2. The two empty tests are the selected clear causal form; no function match yet. Case0x66 still requires its unallocated store-value constant1 before the year preload.

### Match, verification and review

Case0x66 closes with the empty frame==1 test before store1 and the unsigned-false year test after it. Store-before-test forms close the order but add one selector `subu`; moving the first test ahead removes that extra instruction. The selected full-TU C has379/379 words and cost0. All artificial tests are marked FAKE and have no C side effects.

Guarded candidate and promoted default source separately pass tools/funcdiff_verify.py private exact_match: correct address/span and **1516 literal bytes equal target**. The fallback and guard are removed. All11 current BUNKA_SD C units were rebuilt using their generated Ninja commands, followed by the normal linker, objcopy and SHA1 commands: **BUNKA_SD.bin OK**. Ordinary Ninja first attempted to regenerate the read-only shared assembly and stopped there; executing the exact selected build commands avoided writes to shared inputs. No build-rule changes were made. check_headers and migrate_globals pass; sync_protos --check-branch reports0 new disagreements,15 known.

Root reviewed the C against standards and spec on2026-10-22: no findings. Empty nonvolatile comparisons preserve behavior, unsigned<0 is false, each mechanism is marked FAKE with ticket and reason, no compiler/build exceptions. Ticket is In Review for root's full27-unit integration gate and main push. Complete negative ledger and reproducibility notes: drafts/T-25037/README.md. This branch adds one matched function/1516 bytes.

## Root acceptance

Independent source review and integrated literal1516-byte default-C proof pass. Fresh27/27binary checksums and declaration guards pass, prototypes0new/15known. No fallback or build exceptions. Accepted and moved to Done.
